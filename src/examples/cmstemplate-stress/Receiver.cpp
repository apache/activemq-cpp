/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "Receiver.h"
#include "MessagingTask.h"
#include "decaf/lang/System.h"
#include "ConnectionFactoryMgr.h"

#include <cms/Message.h>
#include <decaf/lang/Exception.h>
#include <decaf/util/concurrent/TimeUnit.h>

using namespace std;
using namespace cms;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace activemq::cmsutil;
using namespace cmstemplate;

////////////////////////////////////////////////////////////////////////////////
ThreadPoolExecutor* Receiver::threadPoolExecutor = NULL;

////////////////////////////////////////////////////////////////////////////////
Receiver::Receiver(const string & url, const string & queueOrTopicName,
                   bool isTopic, long long receiveTimeout, bool useThreadPool) :
        url(url),
        mutexForCmsTemplate(),
        mutexGeneral(),
        closing(false),
        ready(1),
        messageListener(NULL),
        cmsTemplate(NULL),
        asyncReceiverThread(NULL),
        receiveTimeout(receiveTimeout),
        bUseThreadPool(useThreadPool),
        cmsTemplateCreateTime(0),
        numOfMessagingTasks(0) {

    ConnectionFactory* connectionFactory = ConnectionFactoryMgr::getConnectionFactory(url);
    cmsTemplate.reset(new CmsTemplate(connectionFactory));
    cmsTemplate->setDefaultDestinationName(queueOrTopicName);
    cmsTemplate->setPubSubDomain(isTopic);
    cmsTemplate->setReceiveTimeout(receiveTimeout);
}

////////////////////////////////////////////////////////////////////////////////
Receiver::~Receiver() {

    try {
        closing = true;

        // wait until all outstanding messaging tasks are done
        while (true) {
            long numOfMessagingTasks = getNumOfMessagingTasks();
            if (numOfMessagingTasks <= 0) {
                break;
            }
            Thread::sleep(1000);
        }

        if (asyncReceiverThread.get() != NULL) {
            asyncReceiverThread->join();
        }

    } catch (...) {
    }
}

////////////////////////////////////////////////////////////////////////////////
void Receiver::initialize(int reservedThreads, int maxThreads) {
    threadPoolExecutor = new ThreadPoolExecutor(reservedThreads, maxThreads, 5, TimeUnit::SECONDS, new LinkedBlockingQueue<Runnable*>());
    threadPoolExecutor->prestartCoreThread();
}

////////////////////////////////////////////////////////////////////////////////
void Receiver::unInitialize() {
    if (threadPoolExecutor != NULL) {
        try {
            threadPoolExecutor->shutdown();
            threadPoolExecutor->awaitTermination(10000, TimeUnit::MILLISECONDS);
        } catch (Exception& ie) {
        }
        delete threadPoolExecutor;
        threadPoolExecutor = NULL;
    }
}

////////////////////////////////////////////////////////////////////////////////
void Receiver::receiveMessage(std::string& message, ErrorCode& errorCode, bool retryOnError) {
    long long stopRetryTime = System::currentTimeMillis() + receiveTimeout;
    errorCode = CMS_SUCCESS;

    if (receiveTimeout == 0/*CmsTemplate::RECEIVE_TIMEOUT_NO_WAIT*/) {
        retryOnError = false;
    } else if (receiveTimeout == -1/*CmsTemplate::RECEIVE_TIMEOUT_INDEFINITE_WAIT*/) {
        retryOnError = true;
    }

    do {
        long long timeoutForThisLoop;
        if (receiveTimeout <= 0) {
            timeoutForThisLoop = receiveTimeout;
        } else {
            timeoutForThisLoop = stopRetryTime - System::currentTimeMillis();
            if (timeoutForThisLoop <= 0) {
                errorCode = CMS_ERROR_RECEIVER_TIMEDOUT;
                break;
            }
        }

        mutexForCmsTemplate.lock();
        if (cmsTemplate.get() != NULL) {
            cmsTemplate->setReceiveTimeout(timeoutForThisLoop);

            cms::Message* cmsMessage = NULL;
            try {
                cmsMessage = cmsTemplate->receive();
            } catch (cms::CMSException& ex) {
                mutexForCmsTemplate.unlock();
                errorCode = CMS_ERROR_CAUGHT_CMS_EXCEPTION;
                break;
            }

            mutexForCmsTemplate.unlock();
            if (cmsMessage == NULL) {
                break;
            }

            if (isMessageExpired(cmsMessage)) {
                errorCode = CMS_ERROR_INVALID_MESSAGE;
                delete cmsMessage;
                continue;
            }

            wstring text;
            cms::TextMessage* txtMessage = dynamic_cast<cms::TextMessage*>(cmsMessage);
            if (txtMessage) {

                message = txtMessage->getText();
            }
            delete cmsMessage;
        } else {
            mutexForCmsTemplate.unlock();
        }
    } while (errorCode != CMS_SUCCESS && retryOnError && System::currentTimeMillis() < stopRetryTime);
}

////////////////////////////////////////////////////////////////////////////////
void Receiver::waitUntilReady() {
    ready.await();
}

////////////////////////////////////////////////////////////////////////////////
void Receiver::registerMessageListener(ReceiverListener* messageListener, ErrorCode& errorCode) {
    errorCode = CMS_SUCCESS;

    mutexGeneral.lock();
    if (messageListener == NULL) {
        errorCode = CMS_ERROR_INVALID_MESSAGELISTENER;
        mutexGeneral.unlock();
        return;
    }

    if (this->messageListener != NULL) {
        errorCode = CMS_ERROR_A_MESSAGELISTENER_HAS_BEEN_REGISTERED_ALREADY;
        mutexGeneral.unlock();
        return;
    }

    this->messageListener = messageListener;

    asyncReceiverThread.reset(new Thread(this, "AsyncReceiver"));
    asyncReceiverThread->start();
    mutexGeneral.unlock();

    this->waitUntilReady();
}

////////////////////////////////////////////////////////////////////////////////
void Receiver::run() {
    ready.countDown();
    while (!closing) {
        string message = "";

        ErrorCode errorCode = CMS_SUCCESS;

        receiveMessage(message, errorCode, false);
        if (message != "") {
            if (bUseThreadPool) {
                queueMessagingTask(message);
            } else {
                try {
                    executeMessagingTask(message, false);
                } catch (...) {
                }
            }
        } else {
            if (errorCode == CMS_ERROR_CAUGHT_CMS_EXCEPTION || errorCode == CMS_ERROR_MESSAGE_BROKER_ERROR) {
                long long sleepTime = 0;
                mutexForCmsTemplate.lock();
                sleepTime = cmsTemplate->getReceiveTimeout();
                mutexForCmsTemplate.unlock();
                Thread::sleep(sleepTime);
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void Receiver::queueMessagingTask(const string& message) {
    if (message != "" && (!closing)) {
        MessagingTask* task = new MessagingTask(this, message);
        increaseNumOfMessagingTasks();
        threadPoolExecutor->execute(task);
    }
}

////////////////////////////////////////////////////////////////////////////////
void Receiver::executeMessagingTask(const string& message, bool bDecreaseNumOfMessagingTasks/*=true*/) {
    if ((!closing)) {
        mutexGeneral.lock();
        ReceiverListener* copy = this->messageListener;
        mutexGeneral.unlock();
        if (copy) {
            copy->onMessage(message);
        }
    }

    if (bDecreaseNumOfMessagingTasks) {
        decreaseNumOfMessagingTasks();
    }
}

////////////////////////////////////////////////////////////////////////////////
bool Receiver::isMessageExpired(cms::Message* message) {
    long long expireTime = message->getCMSExpiration();
    long long currentTime = System::currentTimeMillis();
    if (expireTime > 0 && currentTime > expireTime) {
        return true;
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////
void Receiver::increaseNumOfMessagingTasks() {
    mutexGeneral.lock();
    numOfMessagingTasks++;
    mutexGeneral.unlock();
}

////////////////////////////////////////////////////////////////////////////////
void Receiver::decreaseNumOfMessagingTasks() {
    mutexGeneral.lock();
    numOfMessagingTasks--;
    mutexGeneral.unlock();
}

////////////////////////////////////////////////////////////////////////////////
long Receiver::getNumOfMessagingTasks() {
    long numOfMessagingTasks = 0;
    mutexGeneral.lock();
    this->numOfMessagingTasks = numOfMessagingTasks;
    mutexGeneral.unlock();
    return numOfMessagingTasks;
}

////////////////////////////////////////////////////////////////////////////////
void Receiver::close() {
    closing = true;
}
