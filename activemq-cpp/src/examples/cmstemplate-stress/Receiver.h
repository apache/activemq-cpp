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

#ifndef _CMSTEMPLATE_RECEIVER_H_
#define _CMSTEMPLATE_RECEIVER_H_

#include <decaf/lang/Runnable.h>
#include <activemq/cmsutil/CmsTemplate.h>
#include <decaf/util/concurrent/CountDownLatch.h>
#include <decaf/util/concurrent/ThreadPoolExecutor.h>
#include <decaf/util/concurrent/LinkedBlockingQueue.h>
#include <decaf/lang/exceptions/RuntimeException.h>
#include "CmsMessageHandlerDefinitions.h"

#include <decaf/util/concurrent/TimeUnit.h>
#include <string>

namespace cmstemplate {

    class Receiver : public decaf::lang::Runnable {
    public:

        typedef void DECAF_STDCALL (*RecvMessageListener)( const std::string& message);

    private:
        std::string m_url;
        decaf::util::concurrent::Mutex m_mutexForCmsTemplate;
        decaf::util::concurrent::Mutex m_mutexGeneral;
        bool m_isClosing;

        decaf::util::concurrent::CountDownLatch m_ready;

        RecvMessageListener m_messageListener;

        activemq::cmsutil::CmsTemplate* m_cmsTemplate;

        decaf::lang::Thread* m_asyncReceiverThread;

        long long m_receiveTimeout;

        bool m_bUseThreadPool; //determines if we should use the thread pool to process async received messages or not

        long long m_cmsTemplateCreateTime;

        static decaf::util::concurrent::ThreadPoolExecutor* m_threadPoolExecutor;
        long m_numOfMessagingTasks; //number of pending messaging tasks created by this receiver that has been queued in the threadpool

        virtual void WaitUntilReady();

        void IncreaseNumOfMessagingTasks();
        void DecreaseNumOfMessagingTasks();
        long GetNumOfMessagingTasks();

    public:

        Receiver(const std::string& url, const std::string& queueOrTopicName, bool isTopic, long long receiveTimeout = 2000, bool useThreadPool = true);

        virtual ~Receiver();

        static void Initialize(int reservedThreads, int maxThreads);
        static void UnInitialize();

        void Close();

        virtual void run();

        void RegisterMessageListener(const RecvMessageListener m_messageListener, ErrorCode& errorCode);

        void ReceiveMessage(std::string& message, ErrorCode& errorCode, bool retryOnError = true);

        static bool IsMessageExpired(cms::Message* message);

        void QueueMessagingTask(const std::string& message);

        void ExecuteMessagingTask(const std::string& message, bool bDecreaseNumOfMessagingTasks = true);

    };
}

#endif /** _CMSTEMPLATE_RECEIVER_H_ */
