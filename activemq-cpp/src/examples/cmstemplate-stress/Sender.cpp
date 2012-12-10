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

using namespace std;
using namespace cms;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::util;
using namespace activemq::cmsutil;
using namespace cmstemplate;

////////////////////////////////////////////////////////////////////////////////
Sender::Sender(const string& url, const string& queueOrTopicName, bool isTopic, bool isDeliveryPersistent, int timeToLive) :
    cmsTemplateMutex(), cmsTemplate(NULL) {

    ConnectionFactory* connectionFactory = ConnectionFactoryMgr::getConnectionFactory(url);

    cmsTemplate.reset(new CmsTemplate(connectionFactory));
    cmsTemplate->setExplicitQosEnabled(true);
    cmsTemplate->setDefaultDestinationName(queueOrTopicName);
    cmsTemplate->setPubSubDomain(isTopic);
    cmsTemplate->setDeliveryPersistent(isDeliveryPersistent);
    cmsTemplate->setTimeToLive(timeToLive);
}

////////////////////////////////////////////////////////////////////////////////
Sender::~Sender() {
    try {
        cmsTemplateMutex.lock();
        cmsTemplate.reset(NULL);
        cmsTemplateMutex.unlock();
    } catch (...) {
    }
}

////////////////////////////////////////////////////////////////////////////////
void Sender::SendMessage(string& message, ErrorCode& errorCode) {

    // create a MessageCreator
    CmsMessageCreator messageCreator(message);

    // send message through a CmsTemplate
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
