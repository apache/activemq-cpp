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

#include <activemq/commands/JournalQueueAck.h>
#include <activemq/exceptions/ActiveMQException.h>
#include <activemq/state/CommandVisitor.h>
#include <decaf/lang/exceptions/NullPointerException.h>

using namespace std;
using namespace activemq;
using namespace activemq::exceptions;
using namespace activemq::commands;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

/*
 *
 *  Command code for OpenWire format for JournalQueueAck
 *
 *  NOTE!: This file is auto generated - do not modify!
 *         if you need to make a change, please see the Java Classes in the
 *         activemq-cpp-openwire-generator module
 *
 */

////////////////////////////////////////////////////////////////////////////////
JournalQueueAck::JournalQueueAck() :
    BaseDataStructure(), destination(NULL), messageAck(NULL) {

}

////////////////////////////////////////////////////////////////////////////////
JournalQueueAck::~JournalQueueAck() {
}

////////////////////////////////////////////////////////////////////////////////
JournalQueueAck* JournalQueueAck::cloneDataStructure() const {
    std::auto_ptr<JournalQueueAck> journalQueueAck(new JournalQueueAck());

    // Copy the data from the base class or classes
    journalQueueAck->copyDataStructure(this);

    return journalQueueAck.release();
}

////////////////////////////////////////////////////////////////////////////////
void JournalQueueAck::copyDataStructure(const DataStructure* src) {

    // Protect against invalid self assignment.
    if (this == src) {
        return;
    }

    const JournalQueueAck* srcPtr = dynamic_cast<const JournalQueueAck*>(src);

    if (srcPtr == NULL || src == NULL) {
        throw decaf::lang::exceptions::NullPointerException(
            __FILE__, __LINE__,
            "JournalQueueAck::copyDataStructure - src is NULL or invalid");
    }

    // Copy the data of the base class or classes
    BaseDataStructure::copyDataStructure(src);

    this->setDestination(srcPtr->getDestination());
    this->setMessageAck(srcPtr->getMessageAck());
}

////////////////////////////////////////////////////////////////////////////////
unsigned char JournalQueueAck::getDataStructureType() const {
    return JournalQueueAck::ID_JOURNALQUEUEACK;
}

////////////////////////////////////////////////////////////////////////////////
std::string JournalQueueAck::toString() const {

    ostringstream stream;

    stream << "JournalQueueAck { ";
    stream << "Destination = ";
    if (this->getDestination() != NULL) {
        stream << this->getDestination()->toString();
    } else {
        stream << "NULL";
    }
    stream << ", ";
    stream << "MessageAck = ";
    if (this->getMessageAck() != NULL) {
        stream << this->getMessageAck()->toString();
    } else {
        stream << "NULL";
    }
    stream << " }";

    return stream.str();
}

////////////////////////////////////////////////////////////////////////////////
bool JournalQueueAck::equals(const DataStructure* value) const {

    if (this == value) {
        return true;
    }

    const JournalQueueAck* valuePtr = dynamic_cast<const JournalQueueAck*>(value);

    if (valuePtr == NULL || value == NULL) {
        return false;
    }

    if (this->getDestination() != NULL) {
        if (!this->getDestination()->equals( valuePtr->getDestination().get())) {
            return false;
        }
    } else if (valuePtr->getDestination() != NULL) {
        return false;
    }
    if (this->getMessageAck() != NULL) {
        if (!this->getMessageAck()->equals( valuePtr->getMessageAck().get())) {
            return false;
        }
    } else if (valuePtr->getMessageAck() != NULL) {
        return false;
    }
    if (!BaseDataStructure::equals(value)) {
        return false;
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////
const decaf::lang::Pointer<ActiveMQDestination>& JournalQueueAck::getDestination() const {
    return destination;
}

////////////////////////////////////////////////////////////////////////////////
decaf::lang::Pointer<ActiveMQDestination>& JournalQueueAck::getDestination() {
    return destination;
}

////////////////////////////////////////////////////////////////////////////////
void JournalQueueAck::setDestination(const decaf::lang::Pointer<ActiveMQDestination>& destination) {
    this->destination = destination;
}

////////////////////////////////////////////////////////////////////////////////
const decaf::lang::Pointer<MessageAck>& JournalQueueAck::getMessageAck() const {
    return messageAck;
}

////////////////////////////////////////////////////////////////////////////////
decaf::lang::Pointer<MessageAck>& JournalQueueAck::getMessageAck() {
    return messageAck;
}

////////////////////////////////////////////////////////////////////////////////
void JournalQueueAck::setMessageAck(const decaf::lang::Pointer<MessageAck>& messageAck) {
    this->messageAck = messageAck;
}

