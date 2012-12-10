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
#include <memory>

namespace cmstemplate {

    class ReceiverListener {
    public:

        virtual ~ReceiverListener() {}

        virtual void onMessage(const std::string& message) = 0;

    };

    class Receiver : public decaf::lang::Runnable {
    private:

        std::string url;
        decaf::util::concurrent::Mutex mutexForCmsTemplate;
        decaf::util::concurrent::Mutex mutexGeneral;
        bool closing;
        decaf::util::concurrent::CountDownLatch ready;
        ReceiverListener* messageListener;
        std::auto_ptr<activemq::cmsutil::CmsTemplate> cmsTemplate;
        std::auto_ptr<decaf::lang::Thread> asyncReceiverThread;
        long long receiveTimeout;
        bool bUseThreadPool;
        long long cmsTemplateCreateTime;
        long numOfMessagingTasks;

    private:

        static decaf::util::concurrent::ThreadPoolExecutor* threadPoolExecutor;

    private:

        virtual void waitUntilReady();

        void increaseNumOfMessagingTasks();
        void decreaseNumOfMessagingTasks();
        long getNumOfMessagingTasks();

    private:

        Receiver(const Receiver&);
        Receiver& operator= (const Receiver&);

    public:

        Receiver(const std::string& url, const std::string& queueOrTopicName, bool isTopic, long long receiveTimeout = 2000, bool useThreadPool = true);

        virtual ~Receiver();

        static void initialize(int reservedThreads, int maxThreads);
        static void unInitialize();

        void close();

        virtual void run();

        void registerMessageListener(ReceiverListener* messageListener, ErrorCode& errorCode);

        void receiveMessage(std::string& message, ErrorCode& errorCode, bool retryOnError = true);

        static bool isMessageExpired(cms::Message* message);

        void queueMessagingTask(const std::string& message);

        void executeMessagingTask(const std::string& message, bool bDecreaseNumOfMessagingTasks = true);

    };
}

#endif /** _CMSTEMPLATE_RECEIVER_H_ */
