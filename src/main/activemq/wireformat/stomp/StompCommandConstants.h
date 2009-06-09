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
#ifndef _ACTIVEMQ_WIREFORMAT_STOMP_STOMPCOMMANDCONSTANTS_H_
#define _ACTIVEMQ_WIREFORMAT_STOMP_STOMPCOMMANDCONSTANTS_H_

#include <cms/Destination.h>
#include <activemq/util/Config.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>

#include <string>
#include <map>

namespace activemq{
namespace wireformat{
namespace stomp{

    class AMQCPP_API StompCommandConstants{
    public:

        // Stomp Command Ids
        static const std::string CONNECT;
        static const std::string CONNECTED;
        static const std::string DISCONNECT;
        static const std::string SUBSCRIBE;
        static const std::string UNSUBSCRIBE;
        static const std::string MESSAGE;
        static const std::string SEND;
        static const std::string BEGIN;
        static const std::string COMMIT;
        static const std::string ABORT;
        static const std::string ACK;
        static const std::string ERROR_CMD;
        static const std::string RECEIPT;

        // Known Stomp Headers
        static const std::string HEADER_DESTINATION;
        static const std::string HEADER_TRANSACTIONID;
        static const std::string HEADER_CONTENTLENGTH;
        static const std::string HEADER_SESSIONID;
        static const std::string HEADER_RECEIPT_REQUIRED;
        static const std::string HEADER_RECEIPTID;
        static const std::string HEADER_MESSAGEID;
        static const std::string HEADER_ACK;
        static const std::string HEADER_LOGIN;
        static const std::string HEADER_PASSWORD;
        static const std::string HEADER_CLIENT_ID;
        static const std::string HEADER_MESSAGE;
        static const std::string HEADER_CORRELATIONID;
        static const std::string HEADER_REQUESTID;
        static const std::string HEADER_RESPONSEID;
        static const std::string HEADER_EXPIRES;
        static const std::string HEADER_PERSISTENT;
        static const std::string HEADER_REPLYTO;
        static const std::string HEADER_TYPE;
        static const std::string HEADER_DISPATCH_ASYNC;
        static const std::string HEADER_EXCLUSIVE;
        static const std::string HEADER_MAXPENDINGMSGLIMIT;
        static const std::string HEADER_NOLOCAL;
        static const std::string HEADER_PREFETCHSIZE;
        static const std::string HEADER_JMSPRIORITY;
        static const std::string HEADER_CONSUMERPRIORITY;
        static const std::string HEADER_RETROACTIVE;
        static const std::string HEADER_SUBSCRIPTIONNAME;
        static const std::string HEADER_OLDSUBSCRIPTIONNAME;
        static const std::string HEADER_TIMESTAMP;
        static const std::string HEADER_REDELIVERED;
        static const std::string HEADER_REDELIVERYCOUNT;
        static const std::string HEADER_SELECTOR;
        static const std::string HEADER_ID;
        static const std::string HEADER_SUBSCRIPTION;
        static const std::string HEADER_TRANSFORMATION;
        static const std::string HEADER_TRANSFORMATION_ERROR;

        // Stomp Ack Modes
        static const std::string ACK_CLIENT;
        static const std::string ACK_AUTO;
        static const std::string ACK_INDIVIDUAL;

        // Supported Stomp Message Types
        static const std::string TEXT;
        static const std::string BYTES;

        // Prefix Id's for Topics and Queues
        static const std::string QUEUE_PREFIX;
        static const std::string TOPIC_PREFIX;
        static const std::string TEMPQUEUE_PREFIX;
        static const std::string TEMPTOPIC_PREFIX;

    };

}}}

#endif /*_ACTIVEMQ_WIREFORMAT_STOMP_COMMANDS_COMMANDCONSTANTS_H_*/
