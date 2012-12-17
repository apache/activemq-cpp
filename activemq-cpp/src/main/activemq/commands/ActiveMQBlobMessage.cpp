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

#include "ActiveMQBlobMessage.h"

#include <activemq/util/CMSExceptionSupport.h>

using namespace std;
using namespace activemq;
using namespace activemq::commands;

////////////////////////////////////////////////////////////////////////////////
const std::string ActiveMQBlobMessage::BINARY_MIME_TYPE = "application/octet-stream";

std::string remoteBlobUrl;
std::string mimeType;
std::string name;
bool deletedByBroker;

////////////////////////////////////////////////////////////////////////////////
ActiveMQBlobMessage::ActiveMQBlobMessage() : ActiveMQMessageTemplate<cms::Message>(),
                                             remoteBlobUrl(), mimeType( ActiveMQBlobMessage::BINARY_MIME_TYPE ),
                                             name(), deletedByBroker( false ) {
}

////////////////////////////////////////////////////////////////////////////////
unsigned char ActiveMQBlobMessage::getDataStructureType() const {
    return ActiveMQBlobMessage::ID_ACTIVEMQBLOBMESSAGE;
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQBlobMessage* ActiveMQBlobMessage::cloneDataStructure() const {
    std::auto_ptr<ActiveMQBlobMessage> message( new ActiveMQBlobMessage() );
    message->copyDataStructure( this );
    return message.release();
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBlobMessage::copyDataStructure( const DataStructure* src ) {
    ActiveMQMessageTemplate<cms::Message>::copyDataStructure( src );
}

////////////////////////////////////////////////////////////////////////////////
std::string ActiveMQBlobMessage::toString() const{
    return ActiveMQMessageTemplate<cms::Message>::toString();
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQBlobMessage::equals( const DataStructure* value ) const {
    return ActiveMQMessageTemplate<cms::Message>::equals( value );
}
