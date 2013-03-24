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

#include <activemq/commands/ProducerAck.h>
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
 *  Command code for OpenWire format for ProducerAck
 *
 *  NOTE!: This file is auto generated - do not modify!
 *         if you need to make a change, please see the Java Classes in the
 *         activemq-cpp-openwire-generator module
 *
 */

////////////////////////////////////////////////////////////////////////////////
ProducerAck::ProducerAck() :
    BaseCommand(), producerId(NULL), size(0) {

}

////////////////////////////////////////////////////////////////////////////////
ProducerAck::~ProducerAck() {
}

////////////////////////////////////////////////////////////////////////////////
ProducerAck* ProducerAck::cloneDataStructure() const {
    std::auto_ptr<ProducerAck> producerAck(new ProducerAck());

    // Copy the data from the base class or classes
    producerAck->copyDataStructure(this);

    return producerAck.release();
}

////////////////////////////////////////////////////////////////////////////////
void ProducerAck::copyDataStructure(const DataStructure* src) {

    // Protect against invalid self assignment.
    if (this == src) {
        return;
    }

    const ProducerAck* srcPtr = dynamic_cast<const ProducerAck*>(src);

    if (srcPtr == NULL || src == NULL) {
        throw decaf::lang::exceptions::NullPointerException(
            __FILE__, __LINE__,
            "ProducerAck::copyDataStructure - src is NULL or invalid");
    }

    // Copy the data of the base class or classes
    BaseCommand::copyDataStructure(src);

    this->setProducerId(srcPtr->getProducerId());
    this->setSize(srcPtr->getSize());
}

////////////////////////////////////////////////////////////////////////////////
unsigned char ProducerAck::getDataStructureType() const {
    return ProducerAck::ID_PRODUCERACK;
}

////////////////////////////////////////////////////////////////////////////////
std::string ProducerAck::toString() const {

    ostringstream stream;

    stream << "ProducerAck { "
           << "commandId = " << this->getCommandId() << ", "
           << "responseRequired = " << boolalpha << this->isResponseRequired();
    stream << ", ";
    stream << "ProducerId = ";
    if (this->getProducerId() != NULL) {
        stream << this->getProducerId()->toString();
    } else {
        stream << "NULL";
    }
    stream << ", ";
    stream << "Size = " << this->getSize();
    stream << " }";

    return stream.str();
}

////////////////////////////////////////////////////////////////////////////////
bool ProducerAck::equals(const DataStructure* value) const {

    if (this == value) {
        return true;
    }

    const ProducerAck* valuePtr = dynamic_cast<const ProducerAck*>(value);

    if (valuePtr == NULL || value == NULL) {
        return false;
    }

    if (this->getProducerId() != NULL) {
        if (!this->getProducerId()->equals( valuePtr->getProducerId().get())) {
            return false;
        }
    } else if (valuePtr->getProducerId() != NULL) {
        return false;
    }
    if (this->getSize() != valuePtr->getSize()) {
        return false;
    }
    if (!BaseCommand::equals(value)) {
        return false;
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////
const decaf::lang::Pointer<ProducerId>& ProducerAck::getProducerId() const {
    return producerId;
}

////////////////////////////////////////////////////////////////////////////////
decaf::lang::Pointer<ProducerId>& ProducerAck::getProducerId() {
    return producerId;
}

////////////////////////////////////////////////////////////////////////////////
void ProducerAck::setProducerId(const decaf::lang::Pointer<ProducerId>& producerId) {
    this->producerId = producerId;
}

////////////////////////////////////////////////////////////////////////////////
int ProducerAck::getSize() const {
    return size;
}

////////////////////////////////////////////////////////////////////////////////
void ProducerAck::setSize(int size) {
    this->size = size;
}

////////////////////////////////////////////////////////////////////////////////
decaf::lang::Pointer<commands::Command> ProducerAck::visit(activemq::state::CommandVisitor* visitor) {
    return visitor->processProducerAck(this);
}
