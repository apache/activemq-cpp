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
ThreadPoolExecutor* Receiver::m_threadPoolExecutor=NULL;

////////////////////////////////////////////////////////////////////////////////
Receiver::Receiver(const string & url, const string & queueOrTopicName,
                   bool isTopic, long long receiveTimeout, bool useThreadPool) : m_ready(1) {

    m_url = url;
    m_messageListener = NULL;

    m_bUseThreadPool = useThreadPool;
    m_receiveTimeout = receiveTimeout;
    m_asyncReceiverThread = NULL;
    ConnectionFactory* connectionFactory = ConnectionFactoryMgr::GetConnectionFactory(m_url);
    m_cmsTemplateCreateTime = System::currentTimeMillis();
    m_cmsTemplate = new CmsTemplate(connectionFactory);
    m_cmsTemplate->setDefaultDestinationName(queueOrTopicName);
    m_cmsTemplate->setPubSubDomain(isTopic);
    m_cmsTemplate->setReceiveTimeout(receiveTimeout);

    m_numOfMessagingTasks = 0;

    m_isClosing = false;
}

////////////////////////////////////////////////////////////////////////////////
Receiver::~Receiver() {

    try {
        m_isClosing = true;

        //delete m_cmsTemplate
        m_mutexForCmsTemplate.lock();
        if (m_cmsTemplate) {
            delete m_cmsTemplate;
            m_cmsTemplate = NULL;
        }
        m_mutexForCmsTemplate.unlock();

        //wait until all outstanding messaging tasks are done
        while (true) {
            long numOfMessagingTasks = GetNumOfMessagingTasks();
            if (numOfMessagingTasks <= 0) {
                break;
            }
            Thread::sleep(1000);
        }
    } catch (...) {
    }
}

////////////////////////////////////////////////////////////////////////////////
void Receiver::Initialize(int reservedThreads, int maxThreads) {
    m_threadPoolExecutor = new ThreadPoolExecutor(reservedThreads, maxThreads, 5, TimeUnit::SECONDS, new LinkedBlockingQueue<Runnable*>());
    m_threadPoolExecutor->prestartCoreThread();
}

////////////////////////////////////////////////////////////////////////////////
void Receiver::UnInitialize() {
    if (m_threadPoolExecutor != NULL) {
        try {
            m_threadPoolExecutor->shutdown();
            m_threadPoolExecutor->awaitTermination(10000, TimeUnit::MILLISECONDS);

        } catch (Exception & ie) {
        }
        delete m_threadPoolExecutor;
        m_threadPoolExecutor = NULL;
    }
}

////////////////////////////////////////////////////////////////////////////////
void Receiver::ReceiveMessage(std::string& message, ErrorCode& errorCode, bool retryOnError) {
    long long stopRetryTime = System::currentTimeMillis() + m_receiveTimeout;
    errorCode = CMS_SUCCESS;

    if (m_receiveTimeout == 0/*CmsTemplate::RECEIVE_TIMEOUT_NO_WAIT*/) {
        retryOnError = false;
    } else if (m_receiveTimeout == -1/*CmsTemplate::RECEIVE_TIMEOUT_INDEFINITE_WAIT*/) {
        retryOnError = true;
    }

    do {
        long long timeoutForThisLoop;
        if (m_receiveTimeout <= 0) {
            timeoutForThisLoop = m_receiveTimeout;
        } else {
            timeoutForThisLoop = stopRetryTime - System::currentTimeMillis();
            if (timeoutForThisLoop <= 0) {
                errorCode = CMS_ERROR_RECEIVER_TIMEDOUT;
                break;
            }
        }

        m_mutexForCmsTemplate.lock();
        if (m_cmsTemplate) {
            m_cmsTemplate->setReceiveTimeout(timeoutForThisLoop);

            cms::Message* cmsMessage = NULL;
            try {
                cmsMessage = m_cmsTemplate->receive();
            } catch (cms::CMSException& ex) {
                m_mutexForCmsTemplate.unlock();
                errorCode = CMS_ERROR_CAUGHT_CMS_EXCEPTION;
                break;
            }

            m_mutexForCmsTemplate.unlock();
            if (cmsMessage == NULL) {
                break;
            }

            if (IsMessageExpired(cmsMessage)) {
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
            m_mutexForCmsTemplate.unlock();
        }
    } while (errorCode != CMS_SUCCESS && retryOnError && System::currentTimeMillis() < stopRetryTime);
}

////////////////////////////////////////////////////////////////////////////////
void Receiver::WaitUntilReady() {
    m_ready.await();
}

////////////////////////////////////////////////////////////////////////////////
void Receiver::RegisterMessageListener(const RecvMessageListener messageListener, ErrorCode& errorCode) {
    errorCode = CMS_SUCCESS;

    m_mutexGeneral.lock();
    if (messageListener == NULL) {
        errorCode = CMS_ERROR_INVALID_MESSAGELISTENER;
        m_mutexGeneral.unlock();
        return;
    }

    if (m_messageListener != NULL) {
        errorCode = CMS_ERROR_A_MESSAGELISTENER_HAS_BEEN_REGISTERED_ALREADY;
        m_mutexGeneral.unlock();
        return;
    }

    m_messageListener = messageListener;

    m_asyncReceiverThread = new Thread(this, "AsyncReceiver");
    m_asyncReceiverThread->start();
    m_mutexGeneral.unlock();

    this->WaitUntilReady();
}

////////////////////////////////////////////////////////////////////////////////
void Receiver::run() {
    m_ready.countDown();
    while (!m_isClosing) {
        string message = "";

        ErrorCode errorCode = CMS_SUCCESS;

        Receiver::ReceiveMessage(message, errorCode, false);
        if (message != "") {
            if (m_bUseThreadPool) {
                QueueMessagingTask(message);
            } else {
                try {
                    ExecuteMessagingTask(message, false);
                } catch (...) {
                }
            }
        } else {
            if (errorCode == CMS_ERROR_CAUGHT_CMS_EXCEPTION || errorCode == CMS_ERROR_MESSAGE_BROKER_ERROR) {
                long long sleepTime = 0;
                m_mutexForCmsTemplate.lock();
                sleepTime = m_cmsTemplate->getReceiveTimeout();
                m_mutexForCmsTemplate.unlock();
                Thread::sleep(sleepTime);
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void Receiver::QueueMessagingTask(const string& message) {
    if (message != "" && (!m_isClosing)) {
        MessagingTask* task = new MessagingTask(this, message);
        m_threadPoolExecutor->execute(task);
        IncreaseNumOfMessagingTasks();
    }
}

////////////////////////////////////////////////////////////////////////////////
void Receiver::ExecuteMessagingTask(const string& message, bool bDecreaseNumOfMessagingTasks/*=true*/) {
    if ((!m_isClosing)) {
        m_mutexGeneral.lock();
        RecvMessageListener copy = m_messageListener;
        m_mutexGeneral.unlock();
        if (copy) {
            (*copy)(message); //listener will release the message and make reference count 0
        }
    }

    if (bDecreaseNumOfMessagingTasks) {
        DecreaseNumOfMessagingTasks();
    }
}

////////////////////////////////////////////////////////////////////////////////
bool Receiver::IsMessageExpired(cms::Message* message) {
    long long expireTime = message->getCMSExpiration();
    long long currentTime = System::currentTimeMillis();
    if (expireTime > 0 && currentTime > expireTime) {
        return true;
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////
void Receiver::IncreaseNumOfMessagingTasks() {
    m_mutexGeneral.lock();
    m_numOfMessagingTasks++;
    m_mutexGeneral.unlock();
}

////////////////////////////////////////////////////////////////////////////////
void Receiver::DecreaseNumOfMessagingTasks() {
    m_mutexGeneral.lock();
    m_numOfMessagingTasks--;
    m_mutexGeneral.unlock();
}

////////////////////////////////////////////////////////////////////////////////
long Receiver::GetNumOfMessagingTasks() {
    long numOfMessagingTasks = 0;
    m_mutexGeneral.lock();
    numOfMessagingTasks = m_numOfMessagingTasks;
    m_mutexGeneral.unlock();
    return numOfMessagingTasks;
}

////////////////////////////////////////////////////////////////////////////////
void Receiver::Close() {
    m_isClosing = true;
}
