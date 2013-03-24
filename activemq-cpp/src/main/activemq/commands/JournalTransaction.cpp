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

#include <activemq/commands/JournalTransaction.h>
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
 *  Command code for OpenWire format for JournalTransaction
 *
 *  NOTE!: This file is auto generated - do not modify!
 *         if you need to make a change, please see the Java Classes in the
 *         activemq-cpp-openwire-generator module
 *
 */

////////////////////////////////////////////////////////////////////////////////
JournalTransaction::JournalTransaction() :
    BaseDataStructure(), transactionId(NULL), type(0), wasPrepared(false) {

}

////////////////////////////////////////////////////////////////////////////////
JournalTransaction::~JournalTransaction() {
}

////////////////////////////////////////////////////////////////////////////////
JournalTransaction* JournalTransaction::cloneDataStructure() const {
    std::auto_ptr<JournalTransaction> journalTransaction(new JournalTransaction());

    // Copy the data from the base class or classes
    journalTransaction->copyDataStructure(this);

    return journalTransaction.release();
}

////////////////////////////////////////////////////////////////////////////////
void JournalTransaction::copyDataStructure(const DataStructure* src) {

    // Protect against invalid self assignment.
    if (this == src) {
        return;
    }

    const JournalTransaction* srcPtr = dynamic_cast<const JournalTransaction*>(src);

    if (srcPtr == NULL || src == NULL) {
        throw decaf::lang::exceptions::NullPointerException(
            __FILE__, __LINE__,
            "JournalTransaction::copyDataStructure - src is NULL or invalid");
    }

    // Copy the data of the base class or classes
    BaseDataStructure::copyDataStructure(src);

    this->setTransactionId(srcPtr->getTransactionId());
    this->setType(srcPtr->getType());
    this->setWasPrepared(srcPtr->getWasPrepared());
}

////////////////////////////////////////////////////////////////////////////////
unsigned char JournalTransaction::getDataStructureType() const {
    return JournalTransaction::ID_JOURNALTRANSACTION;
}

////////////////////////////////////////////////////////////////////////////////
std::string JournalTransaction::toString() const {

    ostringstream stream;

    stream << "JournalTransaction { ";
    stream << "TransactionId = ";
    if (this->getTransactionId() != NULL) {
        stream << this->getTransactionId()->toString();
    } else {
        stream << "NULL";
    }
    stream << ", ";
    stream << "Type = " << (int)this->getType();
    stream << ", ";
    stream << "WasPrepared = " << this->getWasPrepared();
    stream << " }";

    return stream.str();
}

////////////////////////////////////////////////////////////////////////////////
bool JournalTransaction::equals(const DataStructure* value) const {

    if (this == value) {
        return true;
    }

    const JournalTransaction* valuePtr = dynamic_cast<const JournalTransaction*>(value);

    if (valuePtr == NULL || value == NULL) {
        return false;
    }

    if (this->getTransactionId() != NULL) {
        if (!this->getTransactionId()->equals( valuePtr->getTransactionId().get())) {
            return false;
        }
    } else if (valuePtr->getTransactionId() != NULL) {
        return false;
    }
    if (this->getType() != valuePtr->getType()) {
        return false;
    }
    if (this->getWasPrepared() != valuePtr->getWasPrepared()) {
        return false;
    }
    if (!BaseDataStructure::equals(value)) {
        return false;
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////
const decaf::lang::Pointer<TransactionId>& JournalTransaction::getTransactionId() const {
    return transactionId;
}

////////////////////////////////////////////////////////////////////////////////
decaf::lang::Pointer<TransactionId>& JournalTransaction::getTransactionId() {
    return transactionId;
}

////////////////////////////////////////////////////////////////////////////////
void JournalTransaction::setTransactionId(const decaf::lang::Pointer<TransactionId>& transactionId) {
    this->transactionId = transactionId;
}

////////////////////////////////////////////////////////////////////////////////
unsigned char JournalTransaction::getType() const {
    return type;
}

////////////////////////////////////////////////////////////////////////////////
void JournalTransaction::setType(unsigned char type) {
    this->type = type;
}

////////////////////////////////////////////////////////////////////////////////
bool JournalTransaction::getWasPrepared() const {
    return wasPrepared;
}

////////////////////////////////////////////////////////////////////////////////
void JournalTransaction::setWasPrepared(bool wasPrepared) {
    this->wasPrepared = wasPrepared;
}

