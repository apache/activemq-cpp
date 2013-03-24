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

#include <activemq/commands/ConnectionError.h>
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
 *  Command code for OpenWire format for ConnectionError
 *
 *  NOTE!: This file is auto generated - do not modify!
 *         if you need to make a change, please see the Java Classes in the
 *         activemq-cpp-openwire-generator module
 *
 */

////////////////////////////////////////////////////////////////////////////////
ConnectionError::ConnectionError() :
    BaseCommand(), exception(NULL), connectionId(NULL) {

}

////////////////////////////////////////////////////////////////////////////////
ConnectionError::~ConnectionError() {
}

////////////////////////////////////////////////////////////////////////////////
ConnectionError* ConnectionError::cloneDataStructure() const {
    std::auto_ptr<ConnectionError> connectionError(new ConnectionError());

    // Copy the data from the base class or classes
    connectionError->copyDataStructure(this);

    return connectionError.release();
}

////////////////////////////////////////////////////////////////////////////////
void ConnectionError::copyDataStructure(const DataStructure* src) {

    // Protect against invalid self assignment.
    if (this == src) {
        return;
    }

    const ConnectionError* srcPtr = dynamic_cast<const ConnectionError*>(src);

    if (srcPtr == NULL || src == NULL) {
        throw decaf::lang::exceptions::NullPointerException(
            __FILE__, __LINE__,
            "ConnectionError::copyDataStructure - src is NULL or invalid");
    }

    // Copy the data of the base class or classes
    BaseCommand::copyDataStructure(src);

    this->setException(srcPtr->getException());
    this->setConnectionId(srcPtr->getConnectionId());
}

////////////////////////////////////////////////////////////////////////////////
unsigned char ConnectionError::getDataStructureType() const {
    return ConnectionError::ID_CONNECTIONERROR;
}

////////////////////////////////////////////////////////////////////////////////
std::string ConnectionError::toString() const {

    ostringstream stream;

    stream << "ConnectionError { "
           << "commandId = " << this->getCommandId() << ", "
           << "responseRequired = " << boolalpha << this->isResponseRequired();
    stream << ", ";
    stream << "Exception = ";
    if (this->getException() != NULL) {
        stream << this->getException()->toString();
    } else {
        stream << "NULL";
    }
    stream << ", ";
    stream << "ConnectionId = ";
    if (this->getConnectionId() != NULL) {
        stream << this->getConnectionId()->toString();
    } else {
        stream << "NULL";
    }
    stream << " }";

    return stream.str();
}

////////////////////////////////////////////////////////////////////////////////
bool ConnectionError::equals(const DataStructure* value) const {

    if (this == value) {
        return true;
    }

    const ConnectionError* valuePtr = dynamic_cast<const ConnectionError*>(value);

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
    if (this->getConnectionId() != NULL) {
        if (!this->getConnectionId()->equals( valuePtr->getConnectionId().get())) {
            return false;
        }
    } else if (valuePtr->getConnectionId() != NULL) {
        return false;
    }
    if (!BaseCommand::equals(value)) {
        return false;
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////
const decaf::lang::Pointer<BrokerError>& ConnectionError::getException() const {
    return exception;
}

////////////////////////////////////////////////////////////////////////////////
decaf::lang::Pointer<BrokerError>& ConnectionError::getException() {
    return exception;
}

////////////////////////////////////////////////////////////////////////////////
void ConnectionError::setException(const decaf::lang::Pointer<BrokerError>& exception) {
    this->exception = exception;
}

////////////////////////////////////////////////////////////////////////////////
const decaf::lang::Pointer<ConnectionId>& ConnectionError::getConnectionId() const {
    return connectionId;
}

////////////////////////////////////////////////////////////////////////////////
decaf::lang::Pointer<ConnectionId>& ConnectionError::getConnectionId() {
    return connectionId;
}

////////////////////////////////////////////////////////////////////////////////
void ConnectionError::setConnectionId(const decaf::lang::Pointer<ConnectionId>& connectionId) {
    this->connectionId = connectionId;
}

////////////////////////////////////////////////////////////////////////////////
decaf::lang::Pointer<commands::Command> ConnectionError::visit(activemq::state::CommandVisitor* visitor) {
    return visitor->processConnectionError(this);
}
