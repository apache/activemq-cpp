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
#include <activemq/commands/ActiveMQObjectMessage.h>

using namespace std;
using namespace activemq;
using namespace activemq::commands;

////////////////////////////////////////////////////////////////////////////////
ActiveMQObjectMessage::ActiveMQObjectMessage() :
    ActiveMQMessageTemplate<cms::ObjectMessage>() {
}

////////////////////////////////////////////////////////////////////////////////
unsigned char ActiveMQObjectMessage::getDataStructureType() const {
    return ActiveMQObjectMessage::ID_ACTIVEMQOBJECTMESSAGE;
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQObjectMessage* ActiveMQObjectMessage::cloneDataStructure() const {
    std::auto_ptr<ActiveMQObjectMessage> message( new ActiveMQObjectMessage() );
    message->copyDataStructure( this );
    return message.release();
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQObjectMessage::copyDataStructure( const DataStructure* src ) {
    ActiveMQMessageTemplate<cms::ObjectMessage>::copyDataStructure( src );
}

////////////////////////////////////////////////////////////////////////////////
std::string ActiveMQObjectMessage::toString() const{
    return ActiveMQMessageTemplate<cms::ObjectMessage>::toString();
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQObjectMessage::equals( const DataStructure* value ) const {
    return ActiveMQMessageTemplate<cms::ObjectMessage>::equals( value );
}
