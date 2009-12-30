/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <activemq/commands/ActiveMQMessage.h>

using namespace std;
using namespace activemq;
using namespace activemq::util;
using namespace activemq::commands;

////////////////////////////////////////////////////////////////////////////////
ActiveMQMessage::ActiveMQMessage() :
    ActiveMQMessageTemplate<cms::Message>()
{}

////////////////////////////////////////////////////////////////////////////////
unsigned char ActiveMQMessage::getDataStructureType() const {
    return ActiveMQMessage::ID_ACTIVEMQMESSAGE;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMessage::copyDataStructure( const DataStructure* src ) {
    ActiveMQMessageTemplate<cms::Message>::copyDataStructure( src );
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQMessage* ActiveMQMessage::cloneDataStructure() const {
    ActiveMQMessage* message = new ActiveMQMessage();
    message->copyDataStructure( this );
    return message;
}

////////////////////////////////////////////////////////////////////////////////
std::string ActiveMQMessage::toString() const{
    return ActiveMQMessageTemplate<cms::Message>::toString();
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQMessage::equals( const DataStructure* value ) const {
    return ActiveMQMessageTemplate<cms::Message>::equals( value );
}
