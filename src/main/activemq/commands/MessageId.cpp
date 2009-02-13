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
#include <activemq/commands/MessageId.h>
#include <activemq/state/CommandVisitor.h>
#include <activemq/exceptions/ActiveMQException.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <apr_strings.h>

using namespace std;
using namespace activemq;
using namespace activemq::exceptions;
using namespace activemq::commands;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

/*
 *
 *  Command and marshaling code for OpenWire format for MessageId
 *
 *
 *  NOTE!: This file is auto generated - do not modify!
 *         if you need to make a change, please see the Java Classes in the
 *         activemq-core module
 *
 */
////////////////////////////////////////////////////////////////////////////////
MessageId::MessageId() {

    this->producerSequenceId = 0;
    this->brokerSequenceId = 0;
}

////////////////////////////////////////////////////////////////////////////////
MessageId::MessageId( const MessageId& other ) {
    this->copyDataStructure( &other );
}

////////////////////////////////////////////////////////////////////////////////
MessageId::~MessageId() {

}

////////////////////////////////////////////////////////////////////////////////
MessageId* MessageId::cloneDataStructure() const {
    std::auto_ptr<MessageId> messageId( new MessageId() );

    // Copy the data from the base class or classes
    messageId->copyDataStructure( this );

    return messageId.release();
}

////////////////////////////////////////////////////////////////////////////////
void MessageId::copyDataStructure( const DataStructure* src ) {

    // Protect against invalid self assignment.
    if( this == src ) {
        return;
    }

    const MessageId* srcPtr = dynamic_cast<const MessageId*>( src );

    if( srcPtr == NULL || src == NULL ) {
        throw decaf::lang::exceptions::NullPointerException(
            __FILE__, __LINE__,
            "MessageId::copyDataStructure - src is NULL or invalid" );
    }

    // Copy the data of the base class or classes
    BaseDataStructure::copyDataStructure( src );

    this->setProducerId( srcPtr->getProducerId() );
    this->setProducerSequenceId( srcPtr->getProducerSequenceId() );
    this->setBrokerSequenceId( srcPtr->getBrokerSequenceId() );
}

////////////////////////////////////////////////////////////////////////////////
unsigned char MessageId::getDataStructureType() const {
    return MessageId::ID_MESSAGEID;
}

////////////////////////////////////////////////////////////////////////////////
std::string MessageId::toString() const {

    ostringstream stream;

    stream << "Begin Class = MessageId" << std::endl;
    stream << " Value of MessageId::ID_MESSAGEID = 110" << std::endl;
    stream << " Value of ProducerId is Below:" << std::endl;
    if( this->getProducerId() != NULL ) {
        stream << this->getProducerId()->toString() << std::endl;
    } else {
        stream << "   Object is NULL" << std::endl;
    }
    stream << " Value of ProducerSequenceId = " << this->getProducerSequenceId() << std::endl;
    stream << " Value of BrokerSequenceId = " << this->getBrokerSequenceId() << std::endl;
    stream << BaseDataStructure::toString();
    stream << "End Class = MessageId" << std::endl;

    return stream.str();
}

////////////////////////////////////////////////////////////////////////////////
bool MessageId::equals( const DataStructure* value ) const {

    if( this == value ) {
        return true;
    }

    const MessageId* valuePtr = dynamic_cast<const MessageId*>( value );

    if( valuePtr == NULL || value == NULL ) {
        return false;
    }

    if( this->getProducerId() != NULL ) {
        if( !this->getProducerId()->equals( valuePtr->getProducerId().get() ) ) {
            return false;
        }
    } else if( valuePtr->getProducerId() != NULL ) {
        return false;
    }
    if( this->getProducerSequenceId() != valuePtr->getProducerSequenceId() ) {
        return false;
    }
    if( this->getBrokerSequenceId() != valuePtr->getBrokerSequenceId() ) {
        return false;
    }
    if( !BaseDataStructure::equals( value ) ) {
        return false;
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////
const decaf::lang::Pointer<ProducerId>& MessageId::getProducerId() const {
    return producerId;
}

////////////////////////////////////////////////////////////////////////////////
decaf::lang::Pointer<ProducerId>& MessageId::getProducerId() {
    return producerId;
}

////////////////////////////////////////////////////////////////////////////////
void MessageId::setProducerId( const decaf::lang::Pointer<ProducerId>& producerId ) {
    this->producerId = producerId;
}

////////////////////////////////////////////////////////////////////////////////
long long MessageId::getProducerSequenceId() const {
    return producerSequenceId;
}

////////////////////////////////////////////////////////////////////////////////
void MessageId::setProducerSequenceId( long long producerSequenceId ) {
    this->producerSequenceId = producerSequenceId;
}

////////////////////////////////////////////////////////////////////////////////
long long MessageId::getBrokerSequenceId() const {
    return brokerSequenceId;
}

////////////////////////////////////////////////////////////////////////////////
void MessageId::setBrokerSequenceId( long long brokerSequenceId ) {
    this->brokerSequenceId = brokerSequenceId;
}

////////////////////////////////////////////////////////////////////////////////
int MessageId::compareTo( const MessageId& value ) const {

    if( this == &value ) {
        return 0;
    }

    int producerIdComp = this->producerId->compareTo( *( value.producerId ) );
    if( producerIdComp != 0 ) {
        return producerIdComp;
    }

    if( this->producerSequenceId > value.producerSequenceId ) {
        return 1;
    } else if( this->producerSequenceId < value.producerSequenceId ) {
        return -1;
    }

    if( this->brokerSequenceId > value.brokerSequenceId ) {
        return 1;
    } else if( this->brokerSequenceId < value.brokerSequenceId ) {
        return -1;
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
bool MessageId::equals( const MessageId& value ) const {
    return this->equals( &value );
}

////////////////////////////////////////////////////////////////////////////////
bool MessageId::operator==( const MessageId& value ) const {
    return this->compareTo( value ) == 0;
}

////////////////////////////////////////////////////////////////////////////////
bool MessageId::operator<( const MessageId& value ) const {
    return this->compareTo( value ) < 0;
}
////////////////////////////////////////////////////////////////////////////////
MessageId& MessageId::operator= ( const MessageId& other ) {
    this->copyDataStructure( &other );
}

