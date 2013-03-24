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

#include <activemq/commands/TransactionId.h>
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
 *  Command code for OpenWire format for TransactionId
 *
 *  NOTE!: This file is auto generated - do not modify!
 *         if you need to make a change, please see the Java Classes in the
 *         activemq-cpp-openwire-generator module
 *
 */

////////////////////////////////////////////////////////////////////////////////
TransactionId::TransactionId() :
    BaseDataStructure() {

}

////////////////////////////////////////////////////////////////////////////////
TransactionId::TransactionId(const TransactionId& other) :
    BaseDataStructure() {

    this->copyDataStructure(&other);
}

////////////////////////////////////////////////////////////////////////////////
TransactionId::~TransactionId() {
}

////////////////////////////////////////////////////////////////////////////////
TransactionId* TransactionId::cloneDataStructure() const {
    std::auto_ptr<TransactionId> transactionId(new TransactionId());

    // Copy the data from the base class or classes
    transactionId->copyDataStructure(this);

    return transactionId.release();
}

////////////////////////////////////////////////////////////////////////////////
void TransactionId::copyDataStructure(const DataStructure* src) {

    // Protect against invalid self assignment.
    if (this == src) {
        return;
    }

    const TransactionId* srcPtr = dynamic_cast<const TransactionId*>(src);

    if (srcPtr == NULL || src == NULL) {
        throw decaf::lang::exceptions::NullPointerException(
            __FILE__, __LINE__,
            "TransactionId::copyDataStructure - src is NULL or invalid");
    }

    // Copy the data of the base class or classes
    BaseDataStructure::copyDataStructure(src);

}

////////////////////////////////////////////////////////////////////////////////
unsigned char TransactionId::getDataStructureType() const {
    return TransactionId::ID_TRANSACTIONID;
}

////////////////////////////////////////////////////////////////////////////////
std::string TransactionId::toString() const {

    ostringstream stream;

    stream << "TransactionId { ";
    stream << " }";

    return stream.str();
}

////////////////////////////////////////////////////////////////////////////////
bool TransactionId::equals(const DataStructure* value) const {

    if (this == value) {
        return true;
    }

    const TransactionId* valuePtr = dynamic_cast<const TransactionId*>(value);

    if (valuePtr == NULL || value == NULL) {
        return false;
    }

    if (!BaseDataStructure::equals(value)) {
        return false;
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////
int TransactionId::compareTo(const TransactionId& value) const {

    if (this == &value) {
        return 0;
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
bool TransactionId::equals(const TransactionId& value) const {
    return this->equals((const DataStructure*)&value);
}

////////////////////////////////////////////////////////////////////////////////
bool TransactionId::operator==(const TransactionId& value) const {
    return this->compareTo(value) == 0;
}

////////////////////////////////////////////////////////////////////////////////
bool TransactionId::operator<(const TransactionId& value) const {
    return this->compareTo(value) < 0;
}

////////////////////////////////////////////////////////////////////////////////
TransactionId& TransactionId::operator= (const TransactionId& other) {
    this->copyDataStructure(&other);
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
int TransactionId::getHashCode() const {
    return decaf::util::HashCode<std::string>()(this->toString());
}

