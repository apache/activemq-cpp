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

#include <activemq/commands/IntegerResponse.h>
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
 *  Command code for OpenWire format for IntegerResponse
 *
 *  NOTE!: This file is auto generated - do not modify!
 *         if you need to make a change, please see the Java Classes in the
 *         activemq-cpp-openwire-generator module
 *
 */

////////////////////////////////////////////////////////////////////////////////
IntegerResponse::IntegerResponse() :
    Response(), result(0) {

}

////////////////////////////////////////////////////////////////////////////////
IntegerResponse::~IntegerResponse() {
}

////////////////////////////////////////////////////////////////////////////////
IntegerResponse* IntegerResponse::cloneDataStructure() const {
    std::auto_ptr<IntegerResponse> integerResponse(new IntegerResponse());

    // Copy the data from the base class or classes
    integerResponse->copyDataStructure(this);

    return integerResponse.release();
}

////////////////////////////////////////////////////////////////////////////////
void IntegerResponse::copyDataStructure(const DataStructure* src) {

    // Protect against invalid self assignment.
    if (this == src) {
        return;
    }

    const IntegerResponse* srcPtr = dynamic_cast<const IntegerResponse*>(src);

    if (srcPtr == NULL || src == NULL) {
        throw decaf::lang::exceptions::NullPointerException(
            __FILE__, __LINE__,
            "IntegerResponse::copyDataStructure - src is NULL or invalid");
    }

    // Copy the data of the base class or classes
    Response::copyDataStructure(src);

    this->setResult(srcPtr->getResult());
}

////////////////////////////////////////////////////////////////////////////////
unsigned char IntegerResponse::getDataStructureType() const {
    return IntegerResponse::ID_INTEGERRESPONSE;
}

////////////////////////////////////////////////////////////////////////////////
std::string IntegerResponse::toString() const {

    ostringstream stream;

    stream << "IntegerResponse { ";
    stream << "Result = " << this->getResult();
    stream << " }";

    return stream.str();
}

////////////////////////////////////////////////////////////////////////////////
bool IntegerResponse::equals(const DataStructure* value) const {

    if (this == value) {
        return true;
    }

    const IntegerResponse* valuePtr = dynamic_cast<const IntegerResponse*>(value);

    if (valuePtr == NULL || value == NULL) {
        return false;
    }

    if (this->getResult() != valuePtr->getResult()) {
        return false;
    }
    if (!Response::equals(value)) {
        return false;
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////
int IntegerResponse::getResult() const {
    return result;
}

////////////////////////////////////////////////////////////////////////////////
void IntegerResponse::setResult(int result) {
    this->result = result;
}

