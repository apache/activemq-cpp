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

#include "Sender.h"

#include "CmsMessageCreator.h"
#include "ConnectionFactoryMgr.h"

#include <cms/ConnectionFactory.h>

using namespace decaf::lang;
using namespace cms;
using namespace cms::stress;
using namespace activemq::cmsutil;

////////////////////////////////////////////////////////////////////////////////
Sender::Sender(const std::string& url, const std::string& queueOrTopicName,
               bool isTopic, bool isDeliveryPersistent, int timeToLive) : cmsTemplateMutex(), cmsTemplate() {

    ConnectionFactory* connectionFactory = ConnectionFactoryMgr::getConnectionFactory(url);

    cmsTemplate = new CmsTemplate(connectionFactory);

    cmsTemplate->setExplicitQosEnabled(true);
    cmsTemplate->setDefaultDestinationName(queueOrTopicName);
    cmsTemplate->setPubSubDomain(isTopic);
    cmsTemplate->setDeliveryPersistent(isDeliveryPersistent);
    cmsTemplate->setTimeToLive(timeToLive);
}

////////////////////////////////////////////////////////////////////////////////
Sender::~Sender() {
    cmsTemplateMutex.lock();
    if (cmsTemplate) {
        delete cmsTemplate;
        cmsTemplate = NULL;
    }
    cmsTemplateMutex.unlock();
}

////////////////////////////////////////////////////////////////////////////////
void Sender::SendMessage(const std::string& message, ErrorCode& errorCode,
                         const std::string& header, const std::string& value) {

    CmsMessageCreator messageCreator(message, header, value);

    try {
        cmsTemplateMutex.lock();
        cmsTemplate->send(&messageCreator);
        cmsTemplateMutex.unlock();
        errorCode = CMS_SUCCESS;
    } catch (cms::CMSException& ex) {
        cmsTemplateMutex.unlock();
        errorCode = CMS_ERROR_CAUGHT_CMS_EXCEPTION;
    }
}
