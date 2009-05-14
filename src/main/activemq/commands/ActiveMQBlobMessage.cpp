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

using namespace std;
using namespace activemq;
using namespace activemq::commands;

////////////////////////////////////////////////////////////////////////////////
const std::string ActiveMQBlobMessage::BINARY_MIME_TYPE = "application/octet-stream";

////////////////////////////////////////////////////////////////////////////////
ActiveMQBlobMessage::ActiveMQBlobMessage() :
    mimeType( ActiveMQBlobMessage::BINARY_MIME_TYPE ), deletedByBroker( false ) {
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
    std::ostringstream stream;

    stream << "Begin Class = ActiveMQBlobMessage" << std::endl;
    stream << ActiveMQMessageTemplate<cms::Message>::toString();
    stream << "End Class = ActiveMQBlobMessage" << std::endl;

    return stream.str();
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQBlobMessage::equals( const DataStructure* value ) const {
    return ActiveMQMessageTemplate<cms::Message>::equals( value );
}
