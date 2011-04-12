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

#ifndef _CMS_TYPES_H_
#define _CMS_TYPES_H_

#include <cms.h>

#include <cms/ConnectionFactory.h>
#include <cms/Connection.h>
#include <cms/Session.h>
#include <cms/MessageConsumer.h>
#include <cms/MessageProducer.h>
#include <cms/Message.h>
#include <cms/Destination.h>

/**
 * Structure used to Wrap the CMS ConnectionFactory type.
 */
struct CMS_ConnectionFactory {
    cms::ConnectionFactory* factory;
};

/**
 * Structure used to Wrap the CMS Connection type.
 */
struct CMS_Connection {
    cms::Connection* connection;
};

/**
 * Structure used to Wrap the CMS Session type.
 */
struct CMS_Session {
    cms::Session* session;
};

/**
 * Structure used to Wrap the CMS Consumer type.
 */
struct CMS_MessageConsumer {
    cms::MessageConsumer* consumer;
};

/**
 * Structure used to Wrap the CMS Producer type.
 */
struct CMS_MessageProducer {
    cms::MessageProducer* producer;
};

/**
 * Structure used to Wrap the CMS Message type.
 */
struct CMS_Message {
    cms::Message* message;
    MESSAGE_TYPE type;
};

/**
 * Structure used to Wrap the CMS Destination type.
 */
struct CMS_Destination {
    cms::Destination* destination;
    DESTINATION_TYPE type;
};

#endif /* _CMS_TYPES_H_ */
