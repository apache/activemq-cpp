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
#include <activemq/commands/ActiveMQTopic.h>

#include <activemq/util/CMSExceptionSupport.h>

using namespace std;
using namespace activemq;
using namespace activemq::commands;

////////////////////////////////////////////////////////////////////////////////
ActiveMQTopic::ActiveMQTopic() : ActiveMQDestination() {
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQTopic::ActiveMQTopic( const std::string& name ) :
    ActiveMQDestination( name )
{}

////////////////////////////////////////////////////////////////////////////////
ActiveMQTopic::~ActiveMQTopic() throw() {
}

////////////////////////////////////////////////////////////////////////////////
unsigned char ActiveMQTopic::getDataStructureType() const {
    return ActiveMQTopic::ID_ACTIVEMQTOPIC;
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQTopic* ActiveMQTopic::cloneDataStructure() const {
    std::auto_ptr<ActiveMQTopic> message( new ActiveMQTopic() );
    message->copyDataStructure( this );
    return message.release();
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQTopic::copyDataStructure( const DataStructure* src ) {
    ActiveMQDestination::copyDataStructure( src );
}

////////////////////////////////////////////////////////////////////////////////
std::string ActiveMQTopic::toString() const {
    return ActiveMQDestination::toString();
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQTopic::equals( const DataStructure* value ) const {
    return ActiveMQDestination::equals( value );
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQTopic::equals( const cms::Destination& value ) const {
    const ActiveMQDestination* dest = dynamic_cast<const ActiveMQDestination*>( &value );
    return ActiveMQDestination::equals( dest );
}
