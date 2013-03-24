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

#include <activemq/commands/ConsumerControl.h>
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
 *  Command code for OpenWire format for ConsumerControl
 *
 *  NOTE!: This file is auto generated - do not modify!
 *         if you need to make a change, please see the Java Classes in the
 *         activemq-cpp-openwire-generator module
 *
 */

////////////////////////////////////////////////////////////////////////////////
ConsumerControl::ConsumerControl() :
    BaseCommand(), destination(NULL), close(false), consumerId(NULL), prefetch(0), flush(false), start(false), stop(false) {

}

////////////////////////////////////////////////////////////////////////////////
ConsumerControl::~ConsumerControl() {
}

////////////////////////////////////////////////////////////////////////////////
ConsumerControl* ConsumerControl::cloneDataStructure() const {
    std::auto_ptr<ConsumerControl> consumerControl(new ConsumerControl());

    // Copy the data from the base class or classes
    consumerControl->copyDataStructure(this);

    return consumerControl.release();
}

////////////////////////////////////////////////////////////////////////////////
void ConsumerControl::copyDataStructure(const DataStructure* src) {

    // Protect against invalid self assignment.
    if (this == src) {
        return;
    }

    const ConsumerControl* srcPtr = dynamic_cast<const ConsumerControl*>(src);

    if (srcPtr == NULL || src == NULL) {
        throw decaf::lang::exceptions::NullPointerException(
            __FILE__, __LINE__,
            "ConsumerControl::copyDataStructure - src is NULL or invalid");
    }

    // Copy the data of the base class or classes
    BaseCommand::copyDataStructure(src);

    this->setDestination(srcPtr->getDestination());
    this->setClose(srcPtr->isClose());
    this->setConsumerId(srcPtr->getConsumerId());
    this->setPrefetch(srcPtr->getPrefetch());
    this->setFlush(srcPtr->isFlush());
    this->setStart(srcPtr->isStart());
    this->setStop(srcPtr->isStop());
}

////////////////////////////////////////////////////////////////////////////////
unsigned char ConsumerControl::getDataStructureType() const {
    return ConsumerControl::ID_CONSUMERCONTROL;
}

////////////////////////////////////////////////////////////////////////////////
std::string ConsumerControl::toString() const {

    ostringstream stream;

    stream << "ConsumerControl { "
           << "commandId = " << this->getCommandId() << ", "
           << "responseRequired = " << boolalpha << this->isResponseRequired();
    stream << ", ";
    stream << "Destination = ";
    if (this->getDestination() != NULL) {
        stream << this->getDestination()->toString();
    } else {
        stream << "NULL";
    }
    stream << ", ";
    stream << "Close = " << this->isClose();
    stream << ", ";
    stream << "ConsumerId = ";
    if (this->getConsumerId() != NULL) {
        stream << this->getConsumerId()->toString();
    } else {
        stream << "NULL";
    }
    stream << ", ";
    stream << "Prefetch = " << this->getPrefetch();
    stream << ", ";
    stream << "Flush = " << this->isFlush();
    stream << ", ";
    stream << "Start = " << this->isStart();
    stream << ", ";
    stream << "Stop = " << this->isStop();
    stream << " }";

    return stream.str();
}

////////////////////////////////////////////////////////////////////////////////
bool ConsumerControl::equals(const DataStructure* value) const {

    if (this == value) {
        return true;
    }

    const ConsumerControl* valuePtr = dynamic_cast<const ConsumerControl*>(value);

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
    if (this->isClose() != valuePtr->isClose()) {
        return false;
    }
    if (this->getConsumerId() != NULL) {
        if (!this->getConsumerId()->equals( valuePtr->getConsumerId().get())) {
            return false;
        }
    } else if (valuePtr->getConsumerId() != NULL) {
        return false;
    }
    if (this->getPrefetch() != valuePtr->getPrefetch()) {
        return false;
    }
    if (this->isFlush() != valuePtr->isFlush()) {
        return false;
    }
    if (this->isStart() != valuePtr->isStart()) {
        return false;
    }
    if (this->isStop() != valuePtr->isStop()) {
        return false;
    }
    if (!BaseCommand::equals(value)) {
        return false;
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////
const decaf::lang::Pointer<ActiveMQDestination>& ConsumerControl::getDestination() const {
    return destination;
}

////////////////////////////////////////////////////////////////////////////////
decaf::lang::Pointer<ActiveMQDestination>& ConsumerControl::getDestination() {
    return destination;
}

////////////////////////////////////////////////////////////////////////////////
void ConsumerControl::setDestination(const decaf::lang::Pointer<ActiveMQDestination>& destination) {
    this->destination = destination;
}

////////////////////////////////////////////////////////////////////////////////
bool ConsumerControl::isClose() const {
    return close;
}

////////////////////////////////////////////////////////////////////////////////
void ConsumerControl::setClose(bool close) {
    this->close = close;
}

////////////////////////////////////////////////////////////////////////////////
const decaf::lang::Pointer<ConsumerId>& ConsumerControl::getConsumerId() const {
    return consumerId;
}

////////////////////////////////////////////////////////////////////////////////
decaf::lang::Pointer<ConsumerId>& ConsumerControl::getConsumerId() {
    return consumerId;
}

////////////////////////////////////////////////////////////////////////////////
void ConsumerControl::setConsumerId(const decaf::lang::Pointer<ConsumerId>& consumerId) {
    this->consumerId = consumerId;
}

////////////////////////////////////////////////////////////////////////////////
int ConsumerControl::getPrefetch() const {
    return prefetch;
}

////////////////////////////////////////////////////////////////////////////////
void ConsumerControl::setPrefetch(int prefetch) {
    this->prefetch = prefetch;
}

////////////////////////////////////////////////////////////////////////////////
bool ConsumerControl::isFlush() const {
    return flush;
}

////////////////////////////////////////////////////////////////////////////////
void ConsumerControl::setFlush(bool flush) {
    this->flush = flush;
}

////////////////////////////////////////////////////////////////////////////////
bool ConsumerControl::isStart() const {
    return start;
}

////////////////////////////////////////////////////////////////////////////////
void ConsumerControl::setStart(bool start) {
    this->start = start;
}

////////////////////////////////////////////////////////////////////////////////
bool ConsumerControl::isStop() const {
    return stop;
}

////////////////////////////////////////////////////////////////////////////////
void ConsumerControl::setStop(bool stop) {
    this->stop = stop;
}

////////////////////////////////////////////////////////////////////////////////
decaf::lang::Pointer<commands::Command> ConsumerControl::visit(activemq::state::CommandVisitor* visitor) {
    return visitor->processConsumerControl(this);
}
