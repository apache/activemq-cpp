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

#include <activemq/commands/JournalTopicAck.h>
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
 *  Command code for OpenWire format for JournalTopicAck
 *
 *  NOTE!: This file is auto generated - do not modify!
 *         if you need to make a change, please see the Java Classes in the
 *         activemq-cpp-openwire-generator module
 *
 */

////////////////////////////////////////////////////////////////////////////////
JournalTopicAck::JournalTopicAck() :
    BaseDataStructure(), destination(NULL), messageId(NULL), messageSequenceId(0), subscritionName(""), clientId(""), transactionId(NULL) {

}

////////////////////////////////////////////////////////////////////////////////
JournalTopicAck::~JournalTopicAck() {
}

////////////////////////////////////////////////////////////////////////////////
JournalTopicAck* JournalTopicAck::cloneDataStructure() const {
    std::auto_ptr<JournalTopicAck> journalTopicAck(new JournalTopicAck());

    // Copy the data from the base class or classes
    journalTopicAck->copyDataStructure(this);

    return journalTopicAck.release();
}

////////////////////////////////////////////////////////////////////////////////
void JournalTopicAck::copyDataStructure(const DataStructure* src) {

    // Protect against invalid self assignment.
    if (this == src) {
        return;
    }

    const JournalTopicAck* srcPtr = dynamic_cast<const JournalTopicAck*>(src);

    if (srcPtr == NULL || src == NULL) {
        throw decaf::lang::exceptions::NullPointerException(
            __FILE__, __LINE__,
            "JournalTopicAck::copyDataStructure - src is NULL or invalid");
    }

    // Copy the data of the base class or classes
    BaseDataStructure::copyDataStructure(src);

    this->setDestination(srcPtr->getDestination());
    this->setMessageId(srcPtr->getMessageId());
    this->setMessageSequenceId(srcPtr->getMessageSequenceId());
    this->setSubscritionName(srcPtr->getSubscritionName());
    this->setClientId(srcPtr->getClientId());
    this->setTransactionId(srcPtr->getTransactionId());
}

////////////////////////////////////////////////////////////////////////////////
unsigned char JournalTopicAck::getDataStructureType() const {
    return JournalTopicAck::ID_JOURNALTOPICACK;
}

////////////////////////////////////////////////////////////////////////////////
std::string JournalTopicAck::toString() const {

    ostringstream stream;

    stream << "JournalTopicAck { ";
    stream << "Destination = ";
    if (this->getDestination() != NULL) {
        stream << this->getDestination()->toString();
    } else {
        stream << "NULL";
    }
    stream << ", ";
    stream << "MessageId = ";
    if (this->getMessageId() != NULL) {
        stream << this->getMessageId()->toString();
    } else {
        stream << "NULL";
    }
    stream << ", ";
    stream << "MessageSequenceId = " << this->getMessageSequenceId();
    stream << ", ";
    stream << "SubscritionName = " << this->getSubscritionName();
    stream << ", ";
    stream << "ClientId = " << this->getClientId();
    stream << ", ";
    stream << "TransactionId = ";
    if (this->getTransactionId() != NULL) {
        stream << this->getTransactionId()->toString();
    } else {
        stream << "NULL";
    }
    stream << " }";

    return stream.str();
}

////////////////////////////////////////////////////////////////////////////////
bool JournalTopicAck::equals(const DataStructure* value) const {

    if (this == value) {
        return true;
    }

    const JournalTopicAck* valuePtr = dynamic_cast<const JournalTopicAck*>(value);

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
    if (this->getMessageId() != NULL) {
        if (!this->getMessageId()->equals( valuePtr->getMessageId().get())) {
            return false;
        }
    } else if (valuePtr->getMessageId() != NULL) {
        return false;
    }
    if (this->getMessageSequenceId() != valuePtr->getMessageSequenceId()) {
        return false;
    }
    if (this->getSubscritionName() != valuePtr->getSubscritionName()) {
        return false;
    }
    if (this->getClientId() != valuePtr->getClientId()) {
        return false;
    }
    if (this->getTransactionId() != NULL) {
        if (!this->getTransactionId()->equals( valuePtr->getTransactionId().get())) {
            return false;
        }
    } else if (valuePtr->getTransactionId() != NULL) {
        return false;
    }
    if (!BaseDataStructure::equals(value)) {
        return false;
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////
const decaf::lang::Pointer<ActiveMQDestination>& JournalTopicAck::getDestination() const {
    return destination;
}

////////////////////////////////////////////////////////////////////////////////
decaf::lang::Pointer<ActiveMQDestination>& JournalTopicAck::getDestination() {
    return destination;
}

////////////////////////////////////////////////////////////////////////////////
void JournalTopicAck::setDestination(const decaf::lang::Pointer<ActiveMQDestination>& destination) {
    this->destination = destination;
}

////////////////////////////////////////////////////////////////////////////////
const decaf::lang::Pointer<MessageId>& JournalTopicAck::getMessageId() const {
    return messageId;
}

////////////////////////////////////////////////////////////////////////////////
decaf::lang::Pointer<MessageId>& JournalTopicAck::getMessageId() {
    return messageId;
}

////////////////////////////////////////////////////////////////////////////////
void JournalTopicAck::setMessageId(const decaf::lang::Pointer<MessageId>& messageId) {
    this->messageId = messageId;
}

////////////////////////////////////////////////////////////////////////////////
long long JournalTopicAck::getMessageSequenceId() const {
    return messageSequenceId;
}

////////////////////////////////////////////////////////////////////////////////
void JournalTopicAck::setMessageSequenceId(long long messageSequenceId) {
    this->messageSequenceId = messageSequenceId;
}

////////////////////////////////////////////////////////////////////////////////
const std::string& JournalTopicAck::getSubscritionName() const {
    return subscritionName;
}

////////////////////////////////////////////////////////////////////////////////
std::string& JournalTopicAck::getSubscritionName() {
    return subscritionName;
}

////////////////////////////////////////////////////////////////////////////////
void JournalTopicAck::setSubscritionName(const std::string& subscritionName) {
    this->subscritionName = subscritionName;
}

////////////////////////////////////////////////////////////////////////////////
const std::string& JournalTopicAck::getClientId() const {
    return clientId;
}

////////////////////////////////////////////////////////////////////////////////
std::string& JournalTopicAck::getClientId() {
    return clientId;
}

////////////////////////////////////////////////////////////////////////////////
void JournalTopicAck::setClientId(const std::string& clientId) {
    this->clientId = clientId;
}

////////////////////////////////////////////////////////////////////////////////
const decaf::lang::Pointer<TransactionId>& JournalTopicAck::getTransactionId() const {
    return transactionId;
}

////////////////////////////////////////////////////////////////////////////////
decaf::lang::Pointer<TransactionId>& JournalTopicAck::getTransactionId() {
    return transactionId;
}

////////////////////////////////////////////////////////////////////////////////
void JournalTopicAck::setTransactionId(const decaf::lang::Pointer<TransactionId>& transactionId) {
    this->transactionId = transactionId;
}

