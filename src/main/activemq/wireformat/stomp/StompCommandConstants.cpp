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

#include "StompCommandConstants.h"
#include <string.h>

using namespace std;
using namespace activemq;
using namespace activemq::wireformat;
using namespace activemq::wireformat::stomp;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
// Stomp Command Ids
const std::string StompCommandConstants::CONNECT = "CONNECT";
const std::string StompCommandConstants::CONNECTED = "CONNECTED";
const std::string StompCommandConstants::DISCONNECT = "DISCONNECT";
const std::string StompCommandConstants::SUBSCRIBE = "SUBSCRIBE";
const std::string StompCommandConstants::UNSUBSCRIBE = "UNSUBSCRIBE";
const std::string StompCommandConstants::MESSAGE = "MESSAGE";
const std::string StompCommandConstants::SEND = "SEND";
const std::string StompCommandConstants::BEGIN = "BEGIN";
const std::string StompCommandConstants::COMMIT = "COMMIT";
const std::string StompCommandConstants::ABORT = "ABORT";
const std::string StompCommandConstants::ACK = "ACK";
const std::string StompCommandConstants::ERROR_CMD = "ERROR";
const std::string StompCommandConstants::RECEIPT = "RECEIPT";

////////////////////////////////////////////////////////////////////////////////
// Known Stomp Headers
const std::string StompCommandConstants::HEADER_DESTINATION = "destination";
const std::string StompCommandConstants::HEADER_TRANSACTIONID = "transaction";
const std::string StompCommandConstants::HEADER_CONTENTLENGTH = "content-length";
const std::string StompCommandConstants::HEADER_SESSIONID = "session";
const std::string StompCommandConstants::HEADER_RECEIPT_REQUIRED = "receipt";
const std::string StompCommandConstants::HEADER_RECEIPTID = "receipt-id";
const std::string StompCommandConstants::HEADER_MESSAGEID = "message-id";
const std::string StompCommandConstants::HEADER_ACK = "ack";
const std::string StompCommandConstants::HEADER_LOGIN = "login";
const std::string StompCommandConstants::HEADER_PASSWORD = "passcode";
const std::string StompCommandConstants::HEADER_CLIENT_ID = "client-id";
const std::string StompCommandConstants::HEADER_MESSAGE = "message";
const std::string StompCommandConstants::HEADER_CORRELATIONID = "correlation-id";
const std::string StompCommandConstants::HEADER_REQUESTID = "request-id";
const std::string StompCommandConstants::HEADER_RESPONSEID = "response-id";
const std::string StompCommandConstants::HEADER_EXPIRES = "expires";
const std::string StompCommandConstants::HEADER_PERSISTENT = "persistent";
const std::string StompCommandConstants::HEADER_REPLYTO = "reply-to";
const std::string StompCommandConstants::HEADER_TYPE = "type";
const std::string StompCommandConstants::HEADER_DISPATCH_ASYNC = "activemq.dispatchAsync";
const std::string StompCommandConstants::HEADER_EXCLUSIVE = "activemq.exclusive";
const std::string StompCommandConstants::HEADER_MAXPENDINGMSGLIMIT = "activemq.maximumPendingMessageLimit";
const std::string StompCommandConstants::HEADER_NOLOCAL = "activemq.noLocal";
const std::string StompCommandConstants::HEADER_PREFETCHSIZE = "activemq.prefetchSize";
const std::string StompCommandConstants::HEADER_JMSPRIORITY = "priority";
const std::string StompCommandConstants::HEADER_CONSUMERPRIORITY = "activemq.priority";
const std::string StompCommandConstants::HEADER_RETROACTIVE = "activemq.retroactive";
const std::string StompCommandConstants::HEADER_SUBSCRIPTIONNAME = "activemq.subscriptionName";
const std::string StompCommandConstants::HEADER_OLDSUBSCRIPTIONNAME = "activemq.subcriptionName";
const std::string StompCommandConstants::HEADER_TIMESTAMP = "timestamp";
const std::string StompCommandConstants::HEADER_REDELIVERED = "redelivered";
const std::string StompCommandConstants::HEADER_REDELIVERYCOUNT = "redelivery_count";
const std::string StompCommandConstants::HEADER_SELECTOR = "selector";
const std::string StompCommandConstants::HEADER_ID = "id";
const std::string StompCommandConstants::HEADER_SUBSCRIPTION = "subscription";
const std::string StompCommandConstants::HEADER_TRANSFORMATION = "transformation";
const std::string StompCommandConstants::HEADER_TRANSFORMATION_ERROR = "transformation-error";

////////////////////////////////////////////////////////////////////////////////
// Stomp Ack Modes
const std::string StompCommandConstants::ACK_CLIENT = "client";
const std::string StompCommandConstants::ACK_AUTO = "auto";
const std::string StompCommandConstants::ACK_INDIVIDUAL = "client-individual";

////////////////////////////////////////////////////////////////////////////////
// Supported Stomp Message Types
const std::string StompCommandConstants::TEXT = "text";
const std::string StompCommandConstants::BYTES = "bytes";

////////////////////////////////////////////////////////////////////////////////
// Prefix Id's for Topics and Queues
const std::string StompCommandConstants::QUEUE_PREFIX = "/queue/";
const std::string StompCommandConstants::TOPIC_PREFIX = "/topic/";
const std::string StompCommandConstants::TEMPQUEUE_PREFIX = "/temp-queue/";
const std::string StompCommandConstants::TEMPTOPIC_PREFIX = "/temp-topic/";
