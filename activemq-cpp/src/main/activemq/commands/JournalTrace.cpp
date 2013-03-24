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

#include <activemq/commands/JournalTrace.h>
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
 *  Command code for OpenWire format for JournalTrace
 *
 *  NOTE!: This file is auto generated - do not modify!
 *         if you need to make a change, please see the Java Classes in the
 *         activemq-cpp-openwire-generator module
 *
 */

////////////////////////////////////////////////////////////////////////////////
JournalTrace::JournalTrace() :
    BaseDataStructure(), message("") {

}

////////////////////////////////////////////////////////////////////////////////
JournalTrace::~JournalTrace() {
}

////////////////////////////////////////////////////////////////////////////////
JournalTrace* JournalTrace::cloneDataStructure() const {
    std::auto_ptr<JournalTrace> journalTrace(new JournalTrace());

    // Copy the data from the base class or classes
    journalTrace->copyDataStructure(this);

    return journalTrace.release();
}

////////////////////////////////////////////////////////////////////////////////
void JournalTrace::copyDataStructure(const DataStructure* src) {

    // Protect against invalid self assignment.
    if (this == src) {
        return;
    }

    const JournalTrace* srcPtr = dynamic_cast<const JournalTrace*>(src);

    if (srcPtr == NULL || src == NULL) {
        throw decaf::lang::exceptions::NullPointerException(
            __FILE__, __LINE__,
            "JournalTrace::copyDataStructure - src is NULL or invalid");
    }

    // Copy the data of the base class or classes
    BaseDataStructure::copyDataStructure(src);

    this->setMessage(srcPtr->getMessage());
}

////////////////////////////////////////////////////////////////////////////////
unsigned char JournalTrace::getDataStructureType() const {
    return JournalTrace::ID_JOURNALTRACE;
}

////////////////////////////////////////////////////////////////////////////////
std::string JournalTrace::toString() const {

    ostringstream stream;

    stream << "JournalTrace { ";
    stream << "Message = " << this->getMessage();
    stream << " }";

    return stream.str();
}

////////////////////////////////////////////////////////////////////////////////
bool JournalTrace::equals(const DataStructure* value) const {

    if (this == value) {
        return true;
    }

    const JournalTrace* valuePtr = dynamic_cast<const JournalTrace*>(value);

    if (valuePtr == NULL || value == NULL) {
        return false;
    }

    if (this->getMessage() != valuePtr->getMessage()) {
        return false;
    }
    if (!BaseDataStructure::equals(value)) {
        return false;
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////
const std::string& JournalTrace::getMessage() const {
    return message;
}

////////////////////////////////////////////////////////////////////////////////
std::string& JournalTrace::getMessage() {
    return message;
}

////////////////////////////////////////////////////////////////////////////////
void JournalTrace::setMessage(const std::string& message) {
    this->message = message;
}

