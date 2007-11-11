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
#include <decaf/lang/exceptions/NullPointerException.h>
#include <activemq/util/URISupport.h>

using namespace std;
using namespace activemq;
using namespace activemq::util;
using namespace activemq::connector;
using namespace activemq::connector::openwire;
using namespace activemq::connector::openwire::commands;
using namespace decaf::lang;

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
const std::string ActiveMQDestination::DestinationFilter::ANY_CHILD = ">";
const std::string ActiveMQDestination::DestinationFilter::ANY_DESCENDENT = "*";

////////////////////////////////////////////////////////////////////////////////
ActiveMQDestination::ActiveMQDestination() {

    this->physicalName = "";
    this->orderedTarget = DEFAULT_ORDERED_TARGET;
    this->exclusive = false;
    this->ordered = false;
    this->advisory = false;
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQDestination::ActiveMQDestination( const std::string& physicalName ) {

    this->setPhysicalName( physicalName );
    this->orderedTarget = DEFAULT_ORDERED_TARGET;
    this->exclusive = false;
    this->ordered = false;
    this->advisory = false;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQDestination::setPhysicalName( const std::string& physicalName ) {

    this->physicalName = physicalName;

    size_t pos = physicalName.find_first_of('?');
    if( pos != string::npos ) {

        std::string optstring = physicalName.substr( pos + 1 );
        this->physicalName = physicalName.substr( 0, pos );
        URISupport::parseQuery( optstring, &options.getProperties() );
    }

    this->advisory = physicalName.find_first_of( ADVISORY_PREFIX ) == 0;
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
    this->setAdvisory( srcPtr->isAdvisory() );
    this->setOrdered( srcPtr->isOrdered() );
    this->setExclusive( srcPtr->isExclusive() );
    this->setOrderedTarget( srcPtr->getOrderedTarget() );
    this->options.copy( &srcPtr->getOptions() );
}

////////////////////////////////////////////////////////////////////////////////
std::string ActiveMQDestination::toString() const {
    std::ostringstream stream;

    stream << "Begin Class = ActiveMQDestination" << std::endl;

    stream << " Value of exclusive = "
           << std::boolalpha << exclusive << std::endl;
    stream << " Value of ordered = "
           << std::boolalpha << ordered << std::endl;
    stream << " Value of advisory = "
           << std::boolalpha << advisory << std::endl;
    stream << " Value of orderedTarget = " << orderedTarget << std::endl;
    stream << " Value of physicalName = " << physicalName << std::endl;
    stream << " Value of options = " << this->options.toString() << std::endl;
    stream << BaseDataStructure::toString();
    stream << "End Class = ActiveMQDestination" << std::endl;

    return stream.str();
}

////////////////////////////////////////////////////////////////////////////////
unsigned char ActiveMQDestination::getDataStructureType() const {
    return ActiveMQDestination::ID_ACTIVEMQDESTINATION;
}

////////////////////////////////////////////////////////////////////////////////
std::string ActiveMQDestination::getClientId(
    const ActiveMQDestination* destination ) {
	
    std::string answer = "";
    if( destination != NULL && destination->isTemporary() ) {
        std::string name = destination->getPhysicalName();
        size_t start = name.find( TEMP_PREFIX );
        if( start != std::string::npos ) {
            start += TEMP_PREFIX.length();
            size_t stop = name.rfind( TEMP_POSTFIX );
            if( stop > start && stop < name.length() ) {
                answer = name.substr( start, stop-start );
            }
        }
    }
    return answer;
}
