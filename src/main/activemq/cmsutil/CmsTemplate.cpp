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

#include "CmsTemplate.h"

using namespace activemq::cmsutil;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
CmsTemplate::CmsTemplate() {
    initDefaults();
}

////////////////////////////////////////////////////////////////////////////////
CmsTemplate::CmsTemplate(cms::ConnectionFactory* connectionFactory) {
    initDefaults();
    setConnectionFactory(connectionFactory);
}

////////////////////////////////////////////////////////////////////////////////
CmsTemplate::~CmsTemplate() {
}

////////////////////////////////////////////////////////////////////////////////
void CmsTemplate::initDefaults() {
    defaultDestination = NULL;
    defaultDestinationName = "";
    messageIdEnabled = true;
    messageTimestampEnabled = true;
    pubSubNoLocal = false;
    receiveTimeout = RECEIVE_TIMEOUT_INDEFINITE_WAIT;
    explicitQosEnabled = false;
    deliveryMode = cms::DeliveryMode::PERSISTENT;
    priority = DEFAULT_PRIORITY;
    timeToLive = DEFAULT_TIME_TO_LIVE;
}

////////////////////////////////////////////////////////////////////////////////
void CmsTemplate::init() throw (cms::CMSException, IllegalStateException) {
    
    // Invoke the base class.
    CmsDestinationAccessor::init();
    
    // Make sure we have a valid default destination.
    checkDefaultDestination();   
}

////////////////////////////////////////////////////////////////////////////////
void CmsTemplate::checkDefaultDestination() throw (IllegalStateException) {
    if (this->defaultDestination == NULL) {
        throw IllegalStateException(
                __FILE__, __LINE__,
                "No defaultDestination or defaultDestinationName specified. Check configuration of CmsTemplate.");
    }
}

