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

#include <activemq/commands/ConsumerId.h>
#include <activemq/exceptions/ActiveMQException.h>
#include <activemq/state/CommandVisitor.h>
#include <decaf/internal/util/StringUtils.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/util/HashCode.h>
#include <sstream>

using namespace std;
using namespace activemq;
using namespace activemq::exceptions;
using namespace activemq::commands;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::internal::util;

/*
 *
 *  Command code for OpenWire format for ConsumerId
 *
 *  NOTE!: This file is auto generated - do not modify!
 *         if you need to make a change, please see the Java Classes in the
 *         activemq-cpp-openwire-generator module
 *
 */

////////////////////////////////////////////////////////////////////////////////
ConsumerId::ConsumerId() :
    BaseDataStructure(), connectionId(""), sessionId(0), value(0), parentId() {

}

////////////////////////////////////////////////////////////////////////////////
ConsumerId::ConsumerId(const ConsumerId& other) :
    BaseDataStructure(), connectionId(""), sessionId(0), value(0), parentId() {

    this->copyDataStructure(&other);
}

////////////////////////////////////////////////////////////////////////////////
ConsumerId::ConsumerId(const SessionId& sessionId, long long consumerId) :
    BaseDataStructure(), connectionId(""), sessionId(0), value(0), parentId() {

    this->connectionId = sessionId.getConnectionId();
    this->sessionId = sessionId.getValue();
    this->value = consumerId;
}

////////////////////////////////////////////////////////////////////////////////
ConsumerId::~ConsumerId() {
}

////////////////////////////////////////////////////////////////////////////////
ConsumerId* ConsumerId::cloneDataStructure() const {
    std::auto_ptr<ConsumerId> consumerId(new ConsumerId());

    // Copy the data from the base class or classes
    consumerId->copyDataStructure(this);

    return consumerId.release();
}

////////////////////////////////////////////////////////////////////////////////
void ConsumerId::copyDataStructure(const DataStructure* src) {

    // Protect against invalid self assignment.
    if (this == src) {
        return;
    }

    const ConsumerId* srcPtr = dynamic_cast<const ConsumerId*>(src);

    if (srcPtr == NULL || src == NULL) {
        throw decaf::lang::exceptions::NullPointerException(
            __FILE__, __LINE__,
            "ConsumerId::copyDataStructure - src is NULL or invalid");
    }

    // Copy the data of the base class or classes
    BaseDataStructure::copyDataStructure(src);

    this->setConnectionId(srcPtr->getConnectionId());
    this->setSessionId(srcPtr->getSessionId());
    this->setValue(srcPtr->getValue());
}

////////////////////////////////////////////////////////////////////////////////
unsigned char ConsumerId::getDataStructureType() const {
    return ConsumerId::ID_CONSUMERID;
}

////////////////////////////////////////////////////////////////////////////////
std::string ConsumerId::toString() const {

    ostringstream stream;

    stream << this->connectionId << ":"
           << this->sessionId << ":"
           << this->value;

    return stream.str();
}

////////////////////////////////////////////////////////////////////////////////
bool ConsumerId::equals(const DataStructure* value) const {

    if (this == value) {
        return true;
    }

    const ConsumerId* valuePtr = dynamic_cast<const ConsumerId*>(value);

    if (valuePtr == NULL || value == NULL) {
        return false;
    }

    if (this->getConnectionId() != valuePtr->getConnectionId()) {
        return false;
    }
    if (this->getSessionId() != valuePtr->getSessionId()) {
        return false;
    }
    if (this->getValue() != valuePtr->getValue()) {
        return false;
    }
    if (!BaseDataStructure::equals(value)) {
        return false;
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////
const std::string& ConsumerId::getConnectionId() const {
    return connectionId;
}

////////////////////////////////////////////////////////////////////////////////
std::string& ConsumerId::getConnectionId() {
    return connectionId;
}

////////////////////////////////////////////////////////////////////////////////
void ConsumerId::setConnectionId(const std::string& connectionId) {
    this->connectionId = connectionId;
}

////////////////////////////////////////////////////////////////////////////////
long long ConsumerId::getSessionId() const {
    return sessionId;
}

////////////////////////////////////////////////////////////////////////////////
void ConsumerId::setSessionId(long long sessionId) {
    this->sessionId = sessionId;
}

////////////////////////////////////////////////////////////////////////////////
long long ConsumerId::getValue() const {
    return value;
}

////////////////////////////////////////////////////////////////////////////////
void ConsumerId::setValue(long long value) {
    this->value = value;
}

////////////////////////////////////////////////////////////////////////////////
int ConsumerId::compareTo(const ConsumerId& value) const {

    if (this == &value) {
        return 0;
    }

    int connectionIdComp = StringUtils::compareIgnoreCase(this->connectionId.c_str(), value.connectionId.c_str());
    if (connectionIdComp != 0) {
        return connectionIdComp;
    }

    if (this->sessionId > value.sessionId) {
        return 1;
    } else if(this->sessionId < value.sessionId) {
        return -1;
    }

    if (this->value > value.value) {
        return 1;
    } else if(this->value < value.value) {
        return -1;
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
bool ConsumerId::equals(const ConsumerId& value) const {
    return this->equals((const DataStructure*)&value);
}

////////////////////////////////////////////////////////////////////////////////
bool ConsumerId::operator==(const ConsumerId& value) const {
    return this->compareTo(value) == 0;
}

////////////////////////////////////////////////////////////////////////////////
bool ConsumerId::operator<(const ConsumerId& value) const {
    return this->compareTo(value) < 0;
}

////////////////////////////////////////////////////////////////////////////////
ConsumerId& ConsumerId::operator= (const ConsumerId& other) {
    this->copyDataStructure(&other);
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
int ConsumerId::getHashCode() const {
    return decaf::util::HashCode<std::string>()(this->toString());
}

////////////////////////////////////////////////////////////////////////////////
const Pointer<SessionId>& ConsumerId::getParentId() const {
    if (this->parentId == NULL) {
        this->parentId.reset(new SessionId(this));
    }
    return this->parentId;
}

