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

#ifndef _CMS_STRESS_RECEIVER_H_
#define _CMS_STRESS_RECEIVER_H_

#include <decaf/util/Config.h>

#include <decaf/lang/Runnable.h>
#include <activemq/cmsutil/CmsTemplate.h>
#include <decaf/util/concurrent/CountDownLatch.h>
#include <decaf/lang/exceptions/RuntimeException.h>

#include <string>

#include "CmsMessageHandlerDefinitions.h"

namespace cms {
namespace stress {

    class BrokerMonitor;

    class ReceiverListener {
    public:

        virtual ~ReceiverListener() {}

        virtual void onMessage(const std::string& message) = 0;

    };

    class Receiver: public decaf::lang::Runnable {
    private:

        std::string url;
        decaf::util::concurrent::Mutex mutexForCmsTemplate;
        decaf::util::concurrent::Mutex mutexGeneral;
        bool closing;
        bool brokerOnline;
        decaf::util::concurrent::CountDownLatch ready;
        decaf::util::concurrent::CountDownLatch* quit;
        ReceiverListener* messageListener;
        activemq::cmsutil::CmsTemplate* cmsTemplate;
        decaf::lang::Thread* asyncReceiverThread;
        long long receiveTimeout;
        long long cmsTemplateCreateTime;
        bool useThreadPool;
        long numOfMessagingTasks;
        BrokerMonitor* monitor;
        std::string selector;

    private:

        virtual void waitUntilReady();
        void increaseNumOfMessagingTasks();
        void decreaseNumOfMessagingTasks();
        long getNumOfMessagingTasks();

    public:

        Receiver(const std::string& url,
                 const std::string& queueOrTopicName,
                 bool isTopic,
                 BrokerMonitor* monitor,
                 decaf::util::concurrent::CountDownLatch* quit,
                 long long receiveTimeout = 2000,
                 bool useThreadPool = true);

        virtual ~Receiver();

        void close();

        virtual void run();

        void registerMessageListener(ReceiverListener* messageListener,
                                     ErrorCode& errorCode, const std::string& selector, int id = 0);

        void receiveMessage(std::string& message, ErrorCode& errorCode,
                            const std::string& selector, bool retryOnError = true);

        static bool isMessageExpired(cms::Message* message);

        void executeMessagingTask(const std::string& message,
                                  bool bDecreaseNumOfMessagingTasks = true);

    };

}}

#endif /** _CMS_STRESS_RECEIVER_H_ */
