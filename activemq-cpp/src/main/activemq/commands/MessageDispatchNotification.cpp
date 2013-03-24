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

#include <activemq/commands/MessageDispatchNotification.h>
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
 *  Command code for OpenWire format for MessageDispatchNotification
 *
 *  NOTE!: This file is auto generated - do not modify!
 *         if you need to make a change, please see the Java Classes in the
 *         activemq-cpp-openwire-generator module
 *
 */

////////////////////////////////////////////////////////////////////////////////
MessageDispatchNotification::MessageDispatchNotification() :
    BaseCommand(), consumerId(NULL), destination(NULL), deliverySequenceId(0), messageId(NULL) {

}

////////////////////////////////////////////////////////////////////////////////
MessageDispatchNotification::~MessageDispatchNotification() {
}

////////////////////////////////////////////////////////////////////////////////
MessageDispatchNotification* MessageDispatchNotification::cloneDataStructure() const {
    std::auto_ptr<MessageDispatchNotification> messageDispatchNotification(new MessageDispatchNotification());

    // Copy the data from the base class or classes
    messageDispatchNotification->copyDataStructure(this);

    return messageDispatchNotification.release();
}

////////////////////////////////////////////////////////////////////////////////
void MessageDispatchNotification::copyDataStructure(const DataStructure* src) {

    // Protect against invalid self assignment.
    if (this == src) {
        return;
    }

    const MessageDispatchNotification* srcPtr = dynamic_cast<const MessageDispatchNotification*>(src);

    if (srcPtr == NULL || src == NULL) {
        throw decaf::lang::exceptions::NullPointerException(
            __FILE__, __LINE__,
            "MessageDispatchNotification::copyDataStructure - src is NULL or invalid");
    }

    // Copy the data of the base class or classes
    BaseCommand::copyDataStructure(src);

    this->setConsumerId(srcPtr->getConsumerId());
    this->setDestination(srcPtr->getDestination());
    this->setDeliverySequenceId(srcPtr->getDeliverySequenceId());
    this->setMessageId(srcPtr->getMessageId());
}

////////////////////////////////////////////////////////////////////////////////
unsigned char MessageDispatchNotification::getDataStructureType() const {
    return MessageDispatchNotification::ID_MESSAGEDISPATCHNOTIFICATION;
}

////////////////////////////////////////////////////////////////////////////////
std::string MessageDispatchNotification::toString() const {

    ostringstream stream;

    stream << "MessageDispatchNotification { "
           << "commandId = " << this->getCommandId() << ", "
           << "responseRequired = " << boolalpha << this->isResponseRequired();
    stream << ", ";
    stream << "ConsumerId = ";
    if (this->getConsumerId() != NULL) {
        stream << this->getConsumerId()->toString();
    } else {
        stream << "NULL";
    }
    stream << ", ";
    stream << "Destination = ";
    if (this->getDestination() != NULL) {
        stream << this->getDestination()->toString();
    } else {
        stream << "NULL";
    }
    stream << ", ";
    stream << "DeliverySequenceId = " << this->getDeliverySequenceId();
    stream << ", ";
    stream << "MessageId = ";
    if (this->getMessageId() != NULL) {
        stream << this->getMessageId()->toString();
    } else {
        stream << "NULL";
    }
    stream << " }";

    return stream.str();
}

////////////////////////////////////////////////////////////////////////////////
bool MessageDispatchNotification::equals(const DataStructure* value) const {

    if (this == value) {
        return true;
    }

    const MessageDispatchNotification* valuePtr = dynamic_cast<const MessageDispatchNotification*>(value);

    if (valuePtr == NULL || value == NULL) {
        return false;
    }

    if (this->getConsumerId() != NULL) {
        if (!this->getConsumerId()->equals( valuePtr->getConsumerId().get())) {
            return false;
        }
    } else if (valuePtr->getConsumerId() != NULL) {
        return false;
    }
    if (this->getDestination() != NULL) {
        if (!this->getDestination()->equals( valuePtr->getDestination().get())) {
            return false;
        }
    } else if (valuePtr->getDestination() != NULL) {
        return false;
    }
    if (this->getDeliverySequenceId() != valuePtr->getDeliverySequenceId()) {
        return false;
    }
    if (this->getMessageId() != NULL) {
        if (!this->getMessageId()->equals( valuePtr->getMessageId().get())) {
            return false;
        }
    } else if (valuePtr->getMessageId() != NULL) {
        return false;
    }
    if (!BaseCommand::equals(value)) {
        return false;
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////
const decaf::lang::Pointer<ConsumerId>& MessageDispatchNotification::getConsumerId() const {
    return consumerId;
}

////////////////////////////////////////////////////////////////////////////////
decaf::lang::Pointer<ConsumerId>& MessageDispatchNotification::getConsumerId() {
    return consumerId;
}

////////////////////////////////////////////////////////////////////////////////
void MessageDispatchNotification::setConsumerId(const decaf::lang::Pointer<ConsumerId>& consumerId) {
    this->consumerId = consumerId;
}

////////////////////////////////////////////////////////////////////////////////
const decaf::lang::Pointer<ActiveMQDestination>& MessageDispatchNotification::getDestination() const {
    return destination;
}

////////////////////////////////////////////////////////////////////////////////
decaf::lang::Pointer<ActiveMQDestination>& MessageDispatchNotification::getDestination() {
    return destination;
}

////////////////////////////////////////////////////////////////////////////////
void MessageDispatchNotification::setDestination(const decaf::lang::Pointer<ActiveMQDestination>& destination) {
    this->destination = destination;
}

////////////////////////////////////////////////////////////////////////////////
long long MessageDispatchNotification::getDeliverySequenceId() const {
    return deliverySequenceId;
}

////////////////////////////////////////////////////////////////////////////////
void MessageDispatchNotification::setDeliverySequenceId(long long deliverySequenceId) {
    this->deliverySequenceId = deliverySequenceId;
}

////////////////////////////////////////////////////////////////////////////////
const decaf::lang::Pointer<MessageId>& MessageDispatchNotification::getMessageId() const {
    return messageId;
}

////////////////////////////////////////////////////////////////////////////////
decaf::lang::Pointer<MessageId>& MessageDispatchNotification::getMessageId() {
    return messageId;
}

////////////////////////////////////////////////////////////////////////////////
void MessageDispatchNotification::setMessageId(const decaf::lang::Pointer<MessageId>& messageId) {
    this->messageId = messageId;
}

////////////////////////////////////////////////////////////////////////////////
decaf::lang::Pointer<commands::Command> MessageDispatchNotification::visit(activemq::state::CommandVisitor* visitor) {
    return visitor->processMessageDispatchNotification(this);
}
