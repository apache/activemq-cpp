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
#include "ConnectionFactoryMgr.h"
#include "BrokerMonitor.h"
#include "CmsMessageCreator.h"

#include <cms/Message.h>
#include <activemq/cmsutil/MessageCreator.h>
#include <decaf/lang/System.h>
#include <decaf/lang/Exception.h>
#include <decaf/util/concurrent/TimeUnit.h>
#include <stdio.h>

using namespace decaf::lang;
using namespace decaf::util::concurrent;
using namespace activemq::cmsutil;
using namespace cms;
using namespace cms::stress;

////////////////////////////////////////////////////////////////////////////////
Receiver::Receiver(const std::string& url, const std::string& queueOrTopicName,
                   bool isTopic, BrokerMonitor* monitor, CountDownLatch* quit,
                   long long receiveTimeout, bool useThreadPool) :
    url(url),
    mutexForCmsTemplate(),
    mutexGeneral(),
    closing(false),
    brokerOnline(true),
    ready(1),
    quit(quit),
    messageListener(NULL),
    cmsTemplate(NULL),
    asyncReceiverThread(NULL),
    receiveTimeout(receiveTimeout),
    cmsTemplateCreateTime(System::currentTimeMillis()),
    useThreadPool(useThreadPool),
    numOfMessagingTasks(0),
    monitor(monitor),
    selector() {

    ConnectionFactory* connectionFactory = ConnectionFactoryMgr::getConnectionFactory(url);
    cmsTemplateCreateTime = System::currentTimeMillis();
    cmsTemplate = new CmsTemplate(connectionFactory);
    cmsTemplate->setDefaultDestinationName(queueOrTopicName);
    cmsTemplate->setPubSubDomain(isTopic);
    cmsTemplate->setReceiveTimeout(receiveTimeout);
}

////////////////////////////////////////////////////////////////////////////////
Receiver::~Receiver() {

    closing = true;

    //delete cmsTemplate
    mutexForCmsTemplate.lock();
    if (cmsTemplate) {
        delete cmsTemplate;
        cmsTemplate = NULL;
    }
    mutexForCmsTemplate.unlock();

    //wait until all outstanding messaging tasks are done
    while (getNumOfMessagingTasks() > 0) {
        Thread::sleep(100);
    }
}

////////////////////////////////////////////////////////////////////////////////
void Receiver::receiveMessage(std::string& message, ErrorCode& errorCode,
                              const std::string &selector, bool retryOnError) {

    long long stopRetryTime = System::currentTimeMillis() + receiveTimeout;
    errorCode = CMS_SUCCESS;

    if (receiveTimeout == 0 /*CmsTemplate::RECEIVE_TIMEOUT_NO_WAIT*/) {
        retryOnError = false;
    } else if (receiveTimeout == -1 /*CmsTemplate::RECEIVE_TIMEOUT_INDEFINITE_WAIT*/) {
        retryOnError = true;
    }

    if (monitor != NULL) {
        if (monitor->isBrokerOk()) {
            if (!brokerOnline) {
                mutexForCmsTemplate.lock();
                if (cmsTemplate) {
                    cmsTemplateCreateTime = System::currentTimeMillis();
                    CmsTemplate* cmsTemplate = new CmsTemplate(cmsTemplate->getConnectionFactory());
                    cmsTemplate->setDefaultDestinationName(cmsTemplate->getDefaultDestinationName());
                    cmsTemplate->setPubSubDomain(cmsTemplate->isPubSubDomain());
                    cmsTemplate->setReceiveTimeout(cmsTemplate->getReceiveTimeout());
                    delete cmsTemplate;
                }
                mutexForCmsTemplate.unlock();

                brokerOnline = true;
            }
        } else {
            brokerOnline = false;
            errorCode = CMS_ERROR_MESSAGE_BROKER_ERROR;
            return;
        }
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
        if (cmsTemplate) {
            cmsTemplate->setReceiveTimeout(timeoutForThisLoop);

            cms::Message* cmsMessage = NULL;
            try {
                if (selector != "") {
                    cmsMessage = cmsTemplate->receiveSelected(selector);
                } else {
                    cmsMessage = cmsTemplate->receive();
                }
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
void Receiver::registerMessageListener(ReceiverListener* messageListener, ErrorCode& errorCode,
                                       const std::string& selector, int id) {
    errorCode = CMS_SUCCESS;
    char buffer[512];

    if (id != 0) {
        sprintf(buffer, "TestListener-%d", id);
    } else {
        sprintf(buffer, "TestAsyncListener");
    }

    mutexGeneral.lock();
    if (messageListener == NULL) {
        errorCode = CMS_ERROR_INVALID_MESSAGELISTENER;
        mutexGeneral.unlock();
        return;
    }

    if (messageListener != NULL) {
        errorCode = CMS_ERROR_A_MESSAGELISTENER_HAS_BEEN_REGISTERED_ALREADY;
        mutexGeneral.unlock();
        return;
    }

    this->messageListener = messageListener;
    this->selector = selector;

    asyncReceiverThread = new Thread(this, buffer);
    asyncReceiverThread->start();
    mutexGeneral.unlock();

    this->waitUntilReady();
}

////////////////////////////////////////////////////////////////////////////////
void Receiver::run() {
    ready.countDown();
    while (!closing) {
        std::string message = "";

        ErrorCode errorCode = CMS_SUCCESS;

        Receiver::receiveMessage(message, errorCode, selector, false);
        if (quit->getCount() == 0) {
            closing = true;
        }

        if ((message != "") && (!closing)) {
            if (useThreadPool) {
                MessagingTask* task = new MessagingTask(this, message);

                increaseNumOfMessagingTasks();
                task->queue();
            } else {
                try {
                    executeMessagingTask(message, false);
                } catch (...) {
                }
            }
        } else if (!closing) {
            if (errorCode == CMS_ERROR_CAUGHT_CMS_EXCEPTION || errorCode == CMS_ERROR_MESSAGE_BROKER_ERROR) {
                long long sleepTime = 0;
                mutexForCmsTemplate.lock();
                sleepTime = cmsTemplate->getReceiveTimeout();
                mutexForCmsTemplate.unlock();

                if (quit->await(sleepTime)) {
                    closing = true;
                }
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void Receiver::executeMessagingTask(const std::string& message, bool isDecreaseNumOfMessagingTasks) {
    if (!closing) {
        mutexGeneral.lock();
        ReceiverListener* copy = messageListener;
        mutexGeneral.unlock();
        if (copy) {
            copy->onMessage(message);
        }
    }

    if (isDecreaseNumOfMessagingTasks) {
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
    long result = 0;

    mutexGeneral.lock();
    result = numOfMessagingTasks;
    mutexGeneral.unlock();

    return result;
}

////////////////////////////////////////////////////////////////////////////////
void Receiver::close() {
    closing = true;
    if (asyncReceiverThread) {
        asyncReceiverThread->join();
        delete asyncReceiverThread;
        asyncReceiverThread = NULL;
    }
}
