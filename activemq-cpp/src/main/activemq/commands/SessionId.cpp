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

#include <activemq/commands/ConnectionId.h>
#include <activemq/commands/ConsumerId.h>
#include <activemq/commands/ProducerId.h>
#include <activemq/commands/SessionId.h>
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
 *  Command code for OpenWire format for SessionId
 *
 *  NOTE!: This file is auto generated - do not modify!
 *         if you need to make a change, please see the Java Classes in the
 *         activemq-cpp-openwire-generator module
 *
 */

////////////////////////////////////////////////////////////////////////////////
SessionId::SessionId() :
    BaseDataStructure(), connectionId(""), value(0), parentId() {

}

////////////////////////////////////////////////////////////////////////////////
SessionId::SessionId(const SessionId& other) :
    BaseDataStructure(), connectionId(""), value(0), parentId() {

    this->copyDataStructure(&other);
}

////////////////////////////////////////////////////////////////////////////////
SessionId::SessionId(const ConnectionId* connectionId, long long sessionId) :
    BaseDataStructure(), connectionId(""), value(0), parentId() {

    this->connectionId = connectionId->getValue();
    this->value = sessionId;
}

////////////////////////////////////////////////////////////////////////////////
SessionId::SessionId(const ProducerId* producerId) :
    BaseDataStructure(), connectionId(""), value(0), parentId() {

    this->connectionId = producerId->getConnectionId();
    this->value = producerId->getSessionId();
}

////////////////////////////////////////////////////////////////////////////////
SessionId::SessionId(const ConsumerId* consumerId) :
    BaseDataStructure(), connectionId(""), value(0), parentId() {

    this->connectionId = consumerId->getConnectionId();
    this->value = consumerId->getSessionId();
}

////////////////////////////////////////////////////////////////////////////////
SessionId::~SessionId() {
}

////////////////////////////////////////////////////////////////////////////////
SessionId* SessionId::cloneDataStructure() const {
    std::auto_ptr<SessionId> sessionId(new SessionId());

    // Copy the data from the base class or classes
    sessionId->copyDataStructure(this);

    return sessionId.release();
}

////////////////////////////////////////////////////////////////////////////////
void SessionId::copyDataStructure(const DataStructure* src) {

    // Protect against invalid self assignment.
    if (this == src) {
        return;
    }

    const SessionId* srcPtr = dynamic_cast<const SessionId*>(src);

    if (srcPtr == NULL || src == NULL) {
        throw decaf::lang::exceptions::NullPointerException(
            __FILE__, __LINE__,
            "SessionId::copyDataStructure - src is NULL or invalid");
    }

    // Copy the data of the base class or classes
    BaseDataStructure::copyDataStructure(src);

    this->setConnectionId(srcPtr->getConnectionId());
    this->setValue(srcPtr->getValue());
}

////////////////////////////////////////////////////////////////////////////////
unsigned char SessionId::getDataStructureType() const {
    return SessionId::ID_SESSIONID;
}

////////////////////////////////////////////////////////////////////////////////
std::string SessionId::toString() const {

    ostringstream stream;

    stream << this->connectionId << ":" << this->value;

    return stream.str();
}

////////////////////////////////////////////////////////////////////////////////
bool SessionId::equals(const DataStructure* value) const {

    if (this == value) {
        return true;
    }

    const SessionId* valuePtr = dynamic_cast<const SessionId*>(value);

    if (valuePtr == NULL || value == NULL) {
        return false;
    }

    if (this->getConnectionId() != valuePtr->getConnectionId()) {
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
const std::string& SessionId::getConnectionId() const {
    return connectionId;
}

////////////////////////////////////////////////////////////////////////////////
std::string& SessionId::getConnectionId() {
    return connectionId;
}

////////////////////////////////////////////////////////////////////////////////
void SessionId::setConnectionId(const std::string& connectionId) {
    this->connectionId = connectionId;
}

////////////////////////////////////////////////////////////////////////////////
long long SessionId::getValue() const {
    return value;
}

////////////////////////////////////////////////////////////////////////////////
void SessionId::setValue(long long value) {
    this->value = value;
}

////////////////////////////////////////////////////////////////////////////////
int SessionId::compareTo(const SessionId& value) const {

    if (this == &value) {
        return 0;
    }

    int connectionIdComp = StringUtils::compareIgnoreCase(this->connectionId.c_str(), value.connectionId.c_str());
    if (connectionIdComp != 0) {
        return connectionIdComp;
    }

    if (this->value > value.value) {
        return 1;
    } else if(this->value < value.value) {
        return -1;
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
bool SessionId::equals(const SessionId& value) const {
    return this->equals((const DataStructure*)&value);
}

////////////////////////////////////////////////////////////////////////////////
bool SessionId::operator==(const SessionId& value) const {
    return this->compareTo(value) == 0;
}

////////////////////////////////////////////////////////////////////////////////
bool SessionId::operator<(const SessionId& value) const {
    return this->compareTo(value) < 0;
}

////////////////////////////////////////////////////////////////////////////////
SessionId& SessionId::operator= (const SessionId& other) {
    this->copyDataStructure(&other);
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
int SessionId::getHashCode() const {
    return decaf::util::HashCode<std::string>()(this->toString());
}

////////////////////////////////////////////////////////////////////////////////
const Pointer<ConnectionId>& SessionId::getParentId() const {
    if (this->parentId == NULL) {
        this->parentId.reset(new ConnectionId(this));
    }
    return this->parentId;
}

