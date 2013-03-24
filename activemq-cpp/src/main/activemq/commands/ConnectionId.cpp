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

using namespace std;
using namespace activemq;
using namespace activemq::exceptions;
using namespace activemq::commands;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::internal::util;

/*
 *
 *  Command code for OpenWire format for ConnectionId
 *
 *  NOTE!: This file is auto generated - do not modify!
 *         if you need to make a change, please see the Java Classes in the
 *         activemq-cpp-openwire-generator module
 *
 */

////////////////////////////////////////////////////////////////////////////////
ConnectionId::ConnectionId() :
    BaseDataStructure(), value("") {

}

////////////////////////////////////////////////////////////////////////////////
ConnectionId::ConnectionId(const ConnectionId& other) :
    BaseDataStructure(), value("") {

    this->copyDataStructure(&other);
}

////////////////////////////////////////////////////////////////////////////////
ConnectionId::ConnectionId(const SessionId* sessionId)
    : BaseDataStructure(), value("") {

    this->value = sessionId->getConnectionId();
}

////////////////////////////////////////////////////////////////////////////////
ConnectionId::ConnectionId(const ProducerId* producerId)
    : BaseDataStructure(), value("") {

    this->value = producerId->getConnectionId();
}

////////////////////////////////////////////////////////////////////////////////
ConnectionId::ConnectionId(const ConsumerId* consumerId)
    : BaseDataStructure(), value("") {

    this->value = consumerId->getConnectionId();
}

////////////////////////////////////////////////////////////////////////////////
ConnectionId::~ConnectionId() {
}

////////////////////////////////////////////////////////////////////////////////
ConnectionId* ConnectionId::cloneDataStructure() const {
    std::auto_ptr<ConnectionId> connectionId(new ConnectionId());

    // Copy the data from the base class or classes
    connectionId->copyDataStructure(this);

    return connectionId.release();
}

////////////////////////////////////////////////////////////////////////////////
void ConnectionId::copyDataStructure(const DataStructure* src) {

    // Protect against invalid self assignment.
    if (this == src) {
        return;
    }

    const ConnectionId* srcPtr = dynamic_cast<const ConnectionId*>(src);

    if (srcPtr == NULL || src == NULL) {
        throw decaf::lang::exceptions::NullPointerException(
            __FILE__, __LINE__,
            "ConnectionId::copyDataStructure - src is NULL or invalid");
    }

    // Copy the data of the base class or classes
    BaseDataStructure::copyDataStructure(src);

    this->setValue(srcPtr->getValue());
}

////////////////////////////////////////////////////////////////////////////////
unsigned char ConnectionId::getDataStructureType() const {
    return ConnectionId::ID_CONNECTIONID;
}

////////////////////////////////////////////////////////////////////////////////
std::string ConnectionId::toString() const {

    return this->value;
}

////////////////////////////////////////////////////////////////////////////////
bool ConnectionId::equals(const DataStructure* value) const {

    if (this == value) {
        return true;
    }

    const ConnectionId* valuePtr = dynamic_cast<const ConnectionId*>(value);

    if (valuePtr == NULL || value == NULL) {
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
const std::string& ConnectionId::getValue() const {
    return value;
}

////////////////////////////////////////////////////////////////////////////////
std::string& ConnectionId::getValue() {
    return value;
}

////////////////////////////////////////////////////////////////////////////////
void ConnectionId::setValue(const std::string& value) {
    this->value = value;
}

////////////////////////////////////////////////////////////////////////////////
int ConnectionId::compareTo(const ConnectionId& value) const {

    if (this == &value) {
        return 0;
    }

    int valueComp = StringUtils::compareIgnoreCase(this->value.c_str(), value.value.c_str());
    if (valueComp != 0) {
        return valueComp;
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
bool ConnectionId::equals(const ConnectionId& value) const {
    return this->equals((const DataStructure*)&value);
}

////////////////////////////////////////////////////////////////////////////////
bool ConnectionId::operator==(const ConnectionId& value) const {
    return this->compareTo(value) == 0;
}

////////////////////////////////////////////////////////////////////////////////
bool ConnectionId::operator<(const ConnectionId& value) const {
    return this->compareTo(value) < 0;
}

////////////////////////////////////////////////////////////////////////////////
ConnectionId& ConnectionId::operator= (const ConnectionId& other) {
    this->copyDataStructure(&other);
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
int ConnectionId::getHashCode() const {
    return decaf::util::HashCode<std::string>()(this->toString());
}

