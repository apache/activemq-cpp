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
#include <activemq/connector/openwire/commands/ActiveMQDestination.h>
#include <activemq/exceptions/NullPointerException.h>

using namespace std;
using namespace activemq;
using namespace activemq::exceptions;
using namespace activemq::connector;
using namespace activemq::connector::openwire;
using namespace activemq::connector::openwire::commands;

////////////////////////////////////////////////////////////////////////////////
const std::string ActiveMQDestination::ADVISORY_PREFIX = "ActiveMQ.Advisory.";
const std::string ActiveMQDestination::CONSUMER_ADVISORY_PREFIX = 
    ActiveMQDestination::ADVISORY_PREFIX + "Consumers.";
const std::string ActiveMQDestination::PRODUCER_ADVISORY_PREFIX = 
    ActiveMQDestination::ADVISORY_PREFIX + "Producers.";
const std::string ActiveMQDestination::CONNECTION_ADVISORY_PREFIX = 
    ActiveMQDestination::ADVISORY_PREFIX + "Connections.";
const std::string ActiveMQDestination::DEFAULT_ORDERED_TARGET = "coordinator";
const std::string ActiveMQDestination::TEMP_PREFIX = "{TD{";
const std::string ActiveMQDestination::TEMP_POSTFIX = "}TD}";
const std::string ActiveMQDestination::COMPOSITE_SEPARATOR = ",";

////////////////////////////////////////////////////////////////////////////////
ActiveMQDestination::ActiveMQDestination()
{
    this->physicalName = "";
    this->orderedTarget = DEFAULT_ORDERED_TARGET;
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQDestination::~ActiveMQDestination()
{
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQDestination::copyDataStructure( const DataStructure* src ) {

    // Copy the data of the base class or classes
    BaseDataStructure::copyDataStructure( src );

    const ActiveMQDestination* srcPtr = 
        dynamic_cast<const ActiveMQDestination*>( src );

    if( srcPtr == NULL || src == NULL ) {
    
        throw exceptions::NullPointerException(
            __FILE__, __LINE__,
            "BrokerId::copyDataStructure - src is NULL or invalid" );
    }
    
    this->setPhysicalName( srcPtr->getPhysicalName() );
}

////////////////////////////////////////////////////////////////////////////////
unsigned char ActiveMQDestination::getDataStructureType() const
{
    return ActiveMQDestination::ID_ACTIVEMQDESTINATION; 
}
