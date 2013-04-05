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

#include <activemq/commands/LocalTransactionId.h>
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
 *  Command code for OpenWire format for LocalTransactionId
 *
 *  NOTE!: This file is auto generated - do not modify!
 *         if you need to make a change, please see the Java Classes in the
 *         activemq-cpp-openwire-generator module
 *
 */

////////////////////////////////////////////////////////////////////////////////
LocalTransactionId::LocalTransactionId() :
    TransactionId(), value(0), connectionId(NULL) {

}

////////////////////////////////////////////////////////////////////////////////
LocalTransactionId::LocalTransactionId(const LocalTransactionId& other) :
    TransactionId(), value(0), connectionId(NULL) {

    this->copyDataStructure(&other);
}

////////////////////////////////////////////////////////////////////////////////
LocalTransactionId::~LocalTransactionId() {
}

////////////////////////////////////////////////////////////////////////////////
LocalTransactionId* LocalTransactionId::cloneDataStructure() const {
    std::auto_ptr<LocalTransactionId> localTransactionId(new LocalTransactionId());

    // Copy the data from the base class or classes
    localTransactionId->copyDataStructure(this);

    return localTransactionId.release();
}

////////////////////////////////////////////////////////////////////////////////
void LocalTransactionId::copyDataStructure(const DataStructure* src) {

    // Protect against invalid self assignment.
    if (this == src) {
        return;
    }

    const LocalTransactionId* srcPtr = dynamic_cast<const LocalTransactionId*>(src);

    if (srcPtr == NULL || src == NULL) {
        throw decaf::lang::exceptions::NullPointerException(
            __FILE__, __LINE__,
            "LocalTransactionId::copyDataStructure - src is NULL or invalid");
    }

    // Copy the data of the base class or classes
    TransactionId::copyDataStructure(src);

    this->setValue(srcPtr->getValue());
    this->setConnectionId(srcPtr->getConnectionId());
}

////////////////////////////////////////////////////////////////////////////////
unsigned char LocalTransactionId::getDataStructureType() const {
    return LocalTransactionId::ID_LOCALTRANSACTIONID;
}

////////////////////////////////////////////////////////////////////////////////
std::string LocalTransactionId::toString() const {

    ostringstream stream;

    stream << "TX:"
           << this->connectionId->toString() << ":"
           << this->value;

    return stream.str();
}

////////////////////////////////////////////////////////////////////////////////
bool LocalTransactionId::equals(const DataStructure* value) const {

    if (this == value) {
        return true;
    }

    const LocalTransactionId* valuePtr = dynamic_cast<const LocalTransactionId*>(value);

    if (valuePtr == NULL || value == NULL) {
        return false;
    }

    if (this->getValue() != valuePtr->getValue()) {
        return false;
    }
    if (this->getConnectionId() != NULL) {
        if (!this->getConnectionId()->equals( valuePtr->getConnectionId().get())) {
            return false;
        }
    } else if (valuePtr->getConnectionId() != NULL) {
        return false;
    }
    if (!TransactionId::equals(value)) {
        return false;
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////
long long LocalTransactionId::getValue() const {
    return value;
}

////////////////////////////////////////////////////////////////////////////////
void LocalTransactionId::setValue(long long value) {
    this->value = value;
}

////////////////////////////////////////////////////////////////////////////////
const decaf::lang::Pointer<ConnectionId>& LocalTransactionId::getConnectionId() const {
    return connectionId;
}

////////////////////////////////////////////////////////////////////////////////
decaf::lang::Pointer<ConnectionId>& LocalTransactionId::getConnectionId() {
    return connectionId;
}

////////////////////////////////////////////////////////////////////////////////
void LocalTransactionId::setConnectionId(const decaf::lang::Pointer<ConnectionId>& connectionId) {
    this->connectionId = connectionId;
}

////////////////////////////////////////////////////////////////////////////////
int LocalTransactionId::compareTo(const LocalTransactionId& value) const {

    if (this == &value) {
        return 0;
    }

    if (this->value > value.value) {
        return 1;
    } else if(this->value < value.value) {
        return -1;
    }

    int connectionIdComp = this->connectionId->compareTo(*(value.connectionId));
    if (connectionIdComp != 0) {
        return connectionIdComp;
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
bool LocalTransactionId::equals(const LocalTransactionId& value) const {
    return this->equals((const DataStructure*)&value);
}

////////////////////////////////////////////////////////////////////////////////
bool LocalTransactionId::operator==(const LocalTransactionId& value) const {
    return this->compareTo(value) == 0;
}

////////////////////////////////////////////////////////////////////////////////
bool LocalTransactionId::operator<(const LocalTransactionId& value) const {
    return this->compareTo(value) < 0;
}

////////////////////////////////////////////////////////////////////////////////
LocalTransactionId& LocalTransactionId::operator= (const LocalTransactionId& other) {
    this->copyDataStructure(&other);
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
int LocalTransactionId::getHashCode() const {
    return decaf::util::HashCode<std::string>()(this->toString());
}

