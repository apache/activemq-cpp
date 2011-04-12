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
#include <activemq/commands/ActiveMQDestination.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <activemq/util/URISupport.h>
#include <activemq/util/CMSExceptionSupport.h>

#include <activemq/commands/ActiveMQTopic.h>
#include <activemq/commands/ActiveMQQueue.h>
#include <activemq/commands/ActiveMQTempTopic.h>
#include <activemq/commands/ActiveMQTempQueue.h>

using namespace std;
using namespace activemq;
using namespace activemq::util;
using namespace activemq::commands;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

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
const std::string ActiveMQDestination::QUEUE_QUALIFIED_PREFIX = "queue://";
const std::string ActiveMQDestination::TOPIC_QUALIFIED_PREFIX = "topic://";
const std::string ActiveMQDestination::TEMP_QUEUE_QUALIFED_PREFIX = "temp-queue://";
const std::string ActiveMQDestination::TEMP_TOPIC_QUALIFED_PREFIX = "temp-topic://";

////////////////////////////////////////////////////////////////////////////////
ActiveMQDestination::ActiveMQDestination() :
    BaseDataStructure(), exclusive(false), ordered(false), advisory(false), orderedTarget(DEFAULT_ORDERED_TARGET), physicalName(), options() {

}

////////////////////////////////////////////////////////////////////////////////
ActiveMQDestination::ActiveMQDestination( const std::string& physicalName ) :
    BaseDataStructure(), exclusive(false), ordered(false), advisory(false), orderedTarget(DEFAULT_ORDERED_TARGET), physicalName(), options() {

    this->setPhysicalName( physicalName );
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQDestination::~ActiveMQDestination() throw() {
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

        throw decaf::lang::exceptions::NullPointerException(
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

    switch( this->getDestinationType() ) {

        case cms::Destination::TOPIC:
            return std::string( "topic://" ) + this->getPhysicalName();
        case cms::Destination::TEMPORARY_TOPIC:
            return std::string( "temp-topic://" ) + this->getPhysicalName();
        case cms::Destination::TEMPORARY_QUEUE:
            return std::string( "temp-queue://" ) + this->getPhysicalName();
        default:
            return std::string( "queue://" ) + this->getPhysicalName();
    }
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQDestination::equals( const DataStructure* value ) const {

    if( this == value ) {
        return true;
    }

    const ActiveMQDestination* valuePtr = dynamic_cast<const ActiveMQDestination*>( value );

    if( valuePtr == NULL || value == NULL ) {
        return false;
    }

    return this->getPhysicalName() == valuePtr->getPhysicalName();
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

////////////////////////////////////////////////////////////////////////////////
Pointer<ActiveMQDestination> ActiveMQDestination::createDestination( int type, const std::string& name ) {

    Pointer<ActiveMQDestination> result;

    if( name.find( QUEUE_QUALIFIED_PREFIX ) == 0 ) {
        result.reset( new ActiveMQQueue( name.substr( QUEUE_QUALIFIED_PREFIX.length() ) ) );
        return result;
    } else if( name.find( TOPIC_QUALIFIED_PREFIX ) == 0 ) {
        result.reset( new ActiveMQTopic( name.substr( TOPIC_QUALIFIED_PREFIX.length() ) ) );
        return result;
    } else if( name.find( TEMP_QUEUE_QUALIFED_PREFIX ) == 0 ) {
        result.reset( new ActiveMQTempQueue( name.substr( TEMP_QUEUE_QUALIFED_PREFIX.length() ) ) );
        return result;
    } else if( name.find( TEMP_TOPIC_QUALIFED_PREFIX ) == 0 ) {
        result.reset( new ActiveMQTempTopic( name.substr( TEMP_TOPIC_QUALIFED_PREFIX.length() ) ) );
        return result;
    }

    switch( type ) {
        case cms::Destination::QUEUE:
            result.reset( new ActiveMQQueue( name ) );
            return result;
        case cms::Destination::TOPIC:
            result.reset( new ActiveMQTopic( name ) );
            return result;
        case cms::Destination::TEMPORARY_QUEUE:
            result.reset( new ActiveMQTempQueue( name ) );
            return result;
        case cms::Destination::TEMPORARY_TOPIC:
            result.reset( new ActiveMQTempTopic( name ) );
            return result;
        default:
            throw IllegalArgumentException(
                __FILE__, __LINE__,
                "Invalid default destination type: %d", type );
    }
}
