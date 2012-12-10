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

#ifndef _CMSTEMPLATE_SENDER_H_
#define _CMSTEMPLATE_SENDER_H_

#include <activemq/cmsutil/CmsTemplate.h>
#include <activemq/cmsutil/MessageCreator.h>
#include <decaf/util/concurrent/CountDownLatch.h>
#include <decaf/lang/Runnable.h>

#include "CmsMessageHandlerDefinitions.h"

namespace cmstemplate {

    class Sender {
    private:

        decaf::util::concurrent::Mutex cmsTemplateMutex;
        std::auto_ptr<activemq::cmsutil::CmsTemplate> cmsTemplate;

    private:

        Sender(const Sender&);
        Sender& operator= (const Sender&);

    public:

        Sender(const std::string& url, const std::string& queueOrTopicName,
               bool isTopic, bool isDeliveryPersistent, int timeToLive);

        virtual ~Sender();

        void SendMessage(std::string& msg, ErrorCode& errorCode);

    };
}

#endif /** _CMSTEMPLATE_SENDER_H_ */
