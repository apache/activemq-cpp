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

#include <activemq/commands/ExceptionResponse.h>
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
 *  Command code for OpenWire format for ExceptionResponse
 *
 *  NOTE!: This file is auto generated - do not modify!
 *         if you need to make a change, please see the Java Classes in the
 *         activemq-cpp-openwire-generator module
 *
 */

////////////////////////////////////////////////////////////////////////////////
ExceptionResponse::ExceptionResponse() :
    Response(), exception(NULL) {

}

////////////////////////////////////////////////////////////////////////////////
ExceptionResponse::~ExceptionResponse() {
}

////////////////////////////////////////////////////////////////////////////////
ExceptionResponse* ExceptionResponse::cloneDataStructure() const {
    std::auto_ptr<ExceptionResponse> exceptionResponse(new ExceptionResponse());

    // Copy the data from the base class or classes
    exceptionResponse->copyDataStructure(this);

    return exceptionResponse.release();
}

////////////////////////////////////////////////////////////////////////////////
void ExceptionResponse::copyDataStructure(const DataStructure* src) {

    // Protect against invalid self assignment.
    if (this == src) {
        return;
    }

    const ExceptionResponse* srcPtr = dynamic_cast<const ExceptionResponse*>(src);

    if (srcPtr == NULL || src == NULL) {
        throw decaf::lang::exceptions::NullPointerException(
            __FILE__, __LINE__,
            "ExceptionResponse::copyDataStructure - src is NULL or invalid");
    }

    // Copy the data of the base class or classes
    Response::copyDataStructure(src);

    this->setException(srcPtr->getException());
}

////////////////////////////////////////////////////////////////////////////////
unsigned char ExceptionResponse::getDataStructureType() const {
    return ExceptionResponse::ID_EXCEPTIONRESPONSE;
}

////////////////////////////////////////////////////////////////////////////////
std::string ExceptionResponse::toString() const {

    ostringstream stream;

    stream << "ExceptionResponse { ";
    stream << "Exception = ";
    if (this->getException() != NULL) {
        stream << this->getException()->toString();
    } else {
        stream << "NULL";
    }
    stream << " }";

    return stream.str();
}

////////////////////////////////////////////////////////////////////////////////
bool ExceptionResponse::equals(const DataStructure* value) const {

    if (this == value) {
        return true;
    }

    const ExceptionResponse* valuePtr = dynamic_cast<const ExceptionResponse*>(value);

    if (valuePtr == NULL || value == NULL) {
        return false;
    }

    if (this->getException() != NULL) {
        if (!this->getException()->equals( valuePtr->getException().get())) {
            return false;
        }
    } else if (valuePtr->getException() != NULL) {
        return false;
    }
    if (!Response::equals(value)) {
        return false;
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////
const decaf::lang::Pointer<BrokerError>& ExceptionResponse::getException() const {
    return exception;
}

////////////////////////////////////////////////////////////////////////////////
decaf::lang::Pointer<BrokerError>& ExceptionResponse::getException() {
    return exception;
}

////////////////////////////////////////////////////////////////////////////////
void ExceptionResponse::setException(const decaf::lang::Pointer<BrokerError>& exception) {
    this->exception = exception;
}

