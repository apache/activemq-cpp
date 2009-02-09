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
#include <activemq/commands/SubscriptionInfo.h>
#include <activemq/state/CommandVisitor.h>
#include <activemq/exceptions/ActiveMQException.h>
#include <decaf/lang/exceptions/NullPointerException.h>

using namespace std;
using namespace activemq;
using namespace activemq::exceptions;
using namespace activemq::commands;
using namespace decaf::lang::exceptions;

/*
 *
 *  Command and marshaling code for OpenWire format for SubscriptionInfo
 *
 *
 *  NOTE!: This file is auto generated - do not modify!
 *         if you need to make a change, please see the Java Classes in the
 *         activemq-core module
 *
 */
////////////////////////////////////////////////////////////////////////////////
SubscriptionInfo::SubscriptionInfo() {

    this->clientId = "";
    this->selector = "";
    this->subcriptionName = "";
}

////////////////////////////////////////////////////////////////////////////////
SubscriptionInfo::~SubscriptionInfo() {

}

////////////////////////////////////////////////////////////////////////////////
SubscriptionInfo* SubscriptionInfo::cloneDataStructure() const {
    std::auto_ptr<SubscriptionInfo> subscriptionInfo( new SubscriptionInfo() );

    // Copy the data from the base class or classes
    subscriptionInfo->copyDataStructure( this );

    return subscriptionInfo.release();
}

////////////////////////////////////////////////////////////////////////////////
void SubscriptionInfo::copyDataStructure( const DataStructure* src ) {

    // Protect against invalid self assignment.
    if( this == src ) {
        return;
    }

    // Copy the data of the base class or classes
    BaseDataStructure::copyDataStructure( src );

    const SubscriptionInfo* srcPtr = dynamic_cast<const SubscriptionInfo*>( src );

    if( srcPtr == NULL || src == NULL ) {
        throw decaf::lang::exceptions::NullPointerException(
            __FILE__, __LINE__,
            "SubscriptionInfo::copyDataStructure - src is NULL or invalid" );
    }
    this->setClientId( srcPtr->getClientId() );
    this->setDestination( srcPtr->getDestination() );
    this->setSelector( srcPtr->getSelector() );
    this->setSubcriptionName( srcPtr->getSubcriptionName() );
    this->setSubscribedDestination( srcPtr->getSubscribedDestination() );
}

////////////////////////////////////////////////////////////////////////////////
unsigned char SubscriptionInfo::getDataStructureType() const {
    return SubscriptionInfo::ID_SUBSCRIPTIONINFO;
}

////////////////////////////////////////////////////////////////////////////////
std::string SubscriptionInfo::toString() const {

    ostringstream stream;

    stream << "Begin Class = SubscriptionInfo" << std::endl;
    stream << " Value of SubscriptionInfo::ID_SUBSCRIPTIONINFO = 55" << std::endl;
    stream << " Value of ClientId = " << this->getClientId() << std::endl;
    stream << " Value of Destination is Below:" << std::endl;
    if( this->getDestination() != NULL ) {
        stream << this->getDestination()->toString() << std::endl;
    } else {
        stream << "   Object is NULL" << std::endl;
    }
    stream << " Value of Selector = " << this->getSelector() << std::endl;
    stream << " Value of SubcriptionName = " << this->getSubcriptionName() << std::endl;
    stream << " Value of SubscribedDestination is Below:" << std::endl;
    if( this->getSubscribedDestination() != NULL ) {
        stream << this->getSubscribedDestination()->toString() << std::endl;
    } else {
        stream << "   Object is NULL" << std::endl;
    }
    stream << BaseDataStructure::toString();
    stream << "End Class = SubscriptionInfo" << std::endl;

    return stream.str();
}

////////////////////////////////////////////////////////////////////////////////
bool SubscriptionInfo::equals( const DataStructure* value ) const {

    if( this == value ) {
        return true;
    }

    const SubscriptionInfo* valuePtr = dynamic_cast<const SubscriptionInfo*>( value );

    if( valuePtr == NULL || value == NULL ) {
        return false;
    }

    if( this->getClientId() != valuePtr->getClientId() ) {
        return false;
    }
    if( this->getDestination() != NULL ) {
        if( !this->getDestination()->equals( valuePtr->getDestination().get() ) ) {
            return false;
        }
    } else if( valuePtr->getDestination() != NULL ) {
        return false;
    }
    if( this->getSelector() != valuePtr->getSelector() ) {
        return false;
    }
    if( this->getSubcriptionName() != valuePtr->getSubcriptionName() ) {
        return false;
    }
    if( this->getSubscribedDestination() != NULL ) {
        if( !this->getSubscribedDestination()->equals( valuePtr->getSubscribedDestination().get() ) ) {
            return false;
        }
    } else if( valuePtr->getSubscribedDestination() != NULL ) {
        return false;
    }
    if( !BaseDataStructure::equals( value ) ) {
        return false;
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////
const std::string& SubscriptionInfo::getClientId() const {
    return clientId;
}

////////////////////////////////////////////////////////////////////////////////
std::string& SubscriptionInfo::getClientId() {
    return clientId;
}

////////////////////////////////////////////////////////////////////////////////
void SubscriptionInfo::setClientId( const std::string& clientId ) {
    this->clientId = clientId;
}

////////////////////////////////////////////////////////////////////////////////
const decaf::lang::Pointer<ActiveMQDestination>& SubscriptionInfo::getDestination() const {
    return destination;
}

////////////////////////////////////////////////////////////////////////////////
decaf::lang::Pointer<ActiveMQDestination>& SubscriptionInfo::getDestination() {
    return destination;
}

////////////////////////////////////////////////////////////////////////////////
void SubscriptionInfo::setDestination( const decaf::lang::Pointer<ActiveMQDestination>& destination ) {
    this->destination = destination;
}

////////////////////////////////////////////////////////////////////////////////
const std::string& SubscriptionInfo::getSelector() const {
    return selector;
}

////////////////////////////////////////////////////////////////////////////////
std::string& SubscriptionInfo::getSelector() {
    return selector;
}

////////////////////////////////////////////////////////////////////////////////
void SubscriptionInfo::setSelector( const std::string& selector ) {
    this->selector = selector;
}

////////////////////////////////////////////////////////////////////////////////
const std::string& SubscriptionInfo::getSubcriptionName() const {
    return subcriptionName;
}

////////////////////////////////////////////////////////////////////////////////
std::string& SubscriptionInfo::getSubcriptionName() {
    return subcriptionName;
}

////////////////////////////////////////////////////////////////////////////////
void SubscriptionInfo::setSubcriptionName( const std::string& subcriptionName ) {
    this->subcriptionName = subcriptionName;
}

////////////////////////////////////////////////////////////////////////////////
const decaf::lang::Pointer<ActiveMQDestination>& SubscriptionInfo::getSubscribedDestination() const {
    return subscribedDestination;
}

////////////////////////////////////////////////////////////////////////////////
decaf::lang::Pointer<ActiveMQDestination>& SubscriptionInfo::getSubscribedDestination() {
    return subscribedDestination;
}

////////////////////////////////////////////////////////////////////////////////
void SubscriptionInfo::setSubscribedDestination( const decaf::lang::Pointer<ActiveMQDestination>& subscribedDestination ) {
    this->subscribedDestination = subscribedDestination;
}

