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

#ifndef _CMS_STRESS_BROKERMONITOR_H_
#define _CMS_STRESS_BROKERMONITOR_H_

#include <decaf/util/Config.h>

#include <decaf/lang/Runnable.h>
#include <decaf/util/concurrent/CountDownLatch.h>

#include <activemq/cmsutil/CmsTemplate.h>
#include <activemq/cmsutil/MessageCreator.h>

namespace cms {
    class Session;
namespace stress {

    class Client;

    class BrokerMonitor: public decaf::lang::Runnable, activemq::cmsutil::MessageCreator {
    private:

        bool closing;
        bool brokerOk;
        std::string url;
        int interval;
        decaf::lang::Thread* brokerMonitorThread;
        decaf::util::concurrent::CountDownLatch* quit;

    private:

        activemq::cmsutil::CmsTemplate* createCmsTemplate(cms::ConnectionFactory* connectionFactory);

    public:

        BrokerMonitor(const std::string& url, int interval,
                      decaf::util::concurrent::CountDownLatch* quit);

        virtual ~BrokerMonitor();

        virtual cms::Message* createMessage(cms::Session* session);

        virtual void run();

        void start();

        void close();

        bool isBrokerOk();

    };

}}

#endif /** _CMS_STRESS_BROKERMONITOR_H_ */
