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
#include <activemq/commands/ActiveMQQueue.h>

using namespace std;
using namespace activemq;
using namespace activemq::commands;

////////////////////////////////////////////////////////////////////////////////
ActiveMQQueue::ActiveMQQueue() : ActiveMQDestination()
{}

////////////////////////////////////////////////////////////////////////////////
ActiveMQQueue::ActiveMQQueue( const std::string& name ) : ActiveMQDestination( name )
{}

////////////////////////////////////////////////////////////////////////////////
ActiveMQQueue::~ActiveMQQueue() throw() {
}

////////////////////////////////////////////////////////////////////////////////
unsigned char ActiveMQQueue::getDataStructureType() const {
    return ActiveMQQueue::ID_ACTIVEMQQUEUE;
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQQueue* ActiveMQQueue::cloneDataStructure() const {
    std::auto_ptr<ActiveMQQueue> message( new ActiveMQQueue() );
    message->copyDataStructure( this );
    return message.release();
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQQueue::copyDataStructure( const DataStructure* src ) {
    ActiveMQDestination::copyDataStructure( src );
}

////////////////////////////////////////////////////////////////////////////////
std::string ActiveMQQueue::toString() const {
    return ActiveMQDestination::toString();
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQQueue::equals( const DataStructure* value ) const {
    return ActiveMQDestination::equals( value );
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQQueue::equals( const cms::Destination& value ) const {
    const ActiveMQDestination* dest = dynamic_cast<const ActiveMQDestination*>( &value );
    return ActiveMQDestination::equals( dest );
}
