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

#include <activemq/commands/ProducerId.h>
#include <activemq/exceptions/ActiveMQException.h>
#include <activemq/state/CommandVisitor.h>
#include <decaf/internal/util/StringUtils.h>
#include <decaf/lang/Long.h>
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
 *  Command code for OpenWire format for ProducerId
 *
 *  NOTE!: This file is auto generated - do not modify!
 *         if you need to make a change, please see the Java Classes in the
 *         activemq-cpp-openwire-generator module
 *
 */

////////////////////////////////////////////////////////////////////////////////
ProducerId::ProducerId() :
    BaseDataStructure(), connectionId(""), value(0), sessionId(0), parentId() {

}

////////////////////////////////////////////////////////////////////////////////
ProducerId::ProducerId(const ProducerId& other) :
    BaseDataStructure(), connectionId(""), value(0), sessionId(0), parentId() {

    this->copyDataStructure(&other);
}

////////////////////////////////////////////////////////////////////////////////
ProducerId::ProducerId( const SessionId& sessionId, long long consumerId ) : 
    BaseDataStructure(), connectionId(""), value(0), sessionId(0), parentId() {

    this->connectionId = sessionId.getConnectionId();
    this->sessionId = sessionId.getValue();
    this->value = consumerId;
}

////////////////////////////////////////////////////////////////////////////////
ProducerId::ProducerId(std::string producerKey) :
    BaseDataStructure(), connectionId(""), value(0), sessionId(0), parentId() {

    // Parse off the producerId
    std::size_t p = producerKey.rfind( ':' );

    if (p != std::string::npos) {
        value = Long::parseLong(producerKey.substr(p + 1, std::string::npos));
        producerKey = producerKey.substr(0, p);
    }

    setProducerSessionKey(producerKey);
}

////////////////////////////////////////////////////////////////////////////////
ProducerId::~ProducerId() {
}

////////////////////////////////////////////////////////////////////////////////
ProducerId* ProducerId::cloneDataStructure() const {
    std::auto_ptr<ProducerId> producerId(new ProducerId());

    // Copy the data from the base class or classes
    producerId->copyDataStructure(this);

    return producerId.release();
}

////////////////////////////////////////////////////////////////////////////////
void ProducerId::copyDataStructure(const DataStructure* src) {

    // Protect against invalid self assignment.
    if (this == src) {
        return;
    }

    const ProducerId* srcPtr = dynamic_cast<const ProducerId*>(src);

    if (srcPtr == NULL || src == NULL) {
        throw decaf::lang::exceptions::NullPointerException(
            __FILE__, __LINE__,
            "ProducerId::copyDataStructure - src is NULL or invalid");
    }

    // Copy the data of the base class or classes
    BaseDataStructure::copyDataStructure(src);

    this->setConnectionId(srcPtr->getConnectionId());
    this->setValue(srcPtr->getValue());
    this->setSessionId(srcPtr->getSessionId());
}

////////////////////////////////////////////////////////////////////////////////
unsigned char ProducerId::getDataStructureType() const {
    return ProducerId::ID_PRODUCERID;
}

////////////////////////////////////////////////////////////////////////////////
std::string ProducerId::toString() const {

    ostringstream stream;

    stream << this->connectionId << ":"
           << this->sessionId << ":"
           << this->value;

    return stream.str();
}

////////////////////////////////////////////////////////////////////////////////
bool ProducerId::equals(const DataStructure* value) const {

    if (this == value) {
        return true;
    }

    const ProducerId* valuePtr = dynamic_cast<const ProducerId*>(value);

    if (valuePtr == NULL || value == NULL) {
        return false;
    }

    if (this->getConnectionId() != valuePtr->getConnectionId()) {
        return false;
    }
    if (this->getValue() != valuePtr->getValue()) {
        return false;
    }
    if (this->getSessionId() != valuePtr->getSessionId()) {
        return false;
    }
    if (!BaseDataStructure::equals(value)) {
        return false;
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////
const std::string& ProducerId::getConnectionId() const {
    return connectionId;
}

////////////////////////////////////////////////////////////////////////////////
std::string& ProducerId::getConnectionId() {
    return connectionId;
}

////////////////////////////////////////////////////////////////////////////////
void ProducerId::setConnectionId(const std::string& connectionId) {
    this->connectionId = connectionId;
}

////////////////////////////////////////////////////////////////////////////////
long long ProducerId::getValue() const {
    return value;
}

////////////////////////////////////////////////////////////////////////////////
void ProducerId::setValue(long long value) {
    this->value = value;
}

////////////////////////////////////////////////////////////////////////////////
long long ProducerId::getSessionId() const {
    return sessionId;
}

////////////////////////////////////////////////////////////////////////////////
void ProducerId::setSessionId(long long sessionId) {
    this->sessionId = sessionId;
}

////////////////////////////////////////////////////////////////////////////////
int ProducerId::compareTo(const ProducerId& value) const {

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

    if (this->sessionId > value.sessionId) {
        return 1;
    } else if(this->sessionId < value.sessionId) {
        return -1;
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
bool ProducerId::equals(const ProducerId& value) const {
    return this->equals((const DataStructure*)&value);
}

////////////////////////////////////////////////////////////////////////////////
bool ProducerId::operator==(const ProducerId& value) const {
    return this->compareTo(value) == 0;
}

////////////////////////////////////////////////////////////////////////////////
bool ProducerId::operator<(const ProducerId& value) const {
    return this->compareTo(value) < 0;
}

////////////////////////////////////////////////////////////////////////////////
ProducerId& ProducerId::operator= (const ProducerId& other) {
    this->copyDataStructure(&other);
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
int ProducerId::getHashCode() const {
    return decaf::util::HashCode<std::string>()(this->toString());
}

////////////////////////////////////////////////////////////////////////////////
const Pointer<SessionId>& ProducerId::getParentId() const {
    if (this->parentId == NULL) {
        this->parentId.reset(new SessionId(this));
    }
    return this->parentId;
}

////////////////////////////////////////////////////////////////////////////////
void ProducerId::setProducerSessionKey( std::string sessionKey ) {

    // Parse off the value
    std::size_t p = sessionKey.rfind( ':' );

    if (p != std::string::npos) {
        this->sessionId = Long::parseLong(sessionKey.substr(p + 1, std::string::npos));
        sessionKey = sessionKey.substr(0, p);
    }

    // The rest is the value
    this->connectionId = sessionKey;
}
