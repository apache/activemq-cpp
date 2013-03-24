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

#include <activemq/commands/DestinationInfo.h>
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
 *  Command code for OpenWire format for DestinationInfo
 *
 *  NOTE!: This file is auto generated - do not modify!
 *         if you need to make a change, please see the Java Classes in the
 *         activemq-cpp-openwire-generator module
 *
 */

////////////////////////////////////////////////////////////////////////////////
DestinationInfo::DestinationInfo() :
    BaseCommand(), connectionId(NULL), destination(NULL), operationType(0), timeout(0), brokerPath() {

}

////////////////////////////////////////////////////////////////////////////////
DestinationInfo::~DestinationInfo() {
}

////////////////////////////////////////////////////////////////////////////////
DestinationInfo* DestinationInfo::cloneDataStructure() const {
    std::auto_ptr<DestinationInfo> destinationInfo(new DestinationInfo());

    // Copy the data from the base class or classes
    destinationInfo->copyDataStructure(this);

    return destinationInfo.release();
}

////////////////////////////////////////////////////////////////////////////////
void DestinationInfo::copyDataStructure(const DataStructure* src) {

    // Protect against invalid self assignment.
    if (this == src) {
        return;
    }

    const DestinationInfo* srcPtr = dynamic_cast<const DestinationInfo*>(src);

    if (srcPtr == NULL || src == NULL) {
        throw decaf::lang::exceptions::NullPointerException(
            __FILE__, __LINE__,
            "DestinationInfo::copyDataStructure - src is NULL or invalid");
    }

    // Copy the data of the base class or classes
    BaseCommand::copyDataStructure(src);

    this->setConnectionId(srcPtr->getConnectionId());
    this->setDestination(srcPtr->getDestination());
    this->setOperationType(srcPtr->getOperationType());
    this->setTimeout(srcPtr->getTimeout());
    this->setBrokerPath(srcPtr->getBrokerPath());
}

////////////////////////////////////////////////////////////////////////////////
unsigned char DestinationInfo::getDataStructureType() const {
    return DestinationInfo::ID_DESTINATIONINFO;
}

////////////////////////////////////////////////////////////////////////////////
std::string DestinationInfo::toString() const {

    ostringstream stream;

    stream << "DestinationInfo { "
           << "commandId = " << this->getCommandId() << ", "
           << "responseRequired = " << boolalpha << this->isResponseRequired();
    stream << ", ";
    stream << "ConnectionId = ";
    if (this->getConnectionId() != NULL) {
        stream << this->getConnectionId()->toString();
    } else {
        stream << "NULL";
    }
    stream << ", ";
    stream << "Destination = ";
    if (this->getDestination() != NULL) {
        stream << this->getDestination()->toString();
    } else {
        stream << "NULL";
    }
    stream << ", ";
    stream << "OperationType = " << (int)this->getOperationType();
    stream << ", ";
    stream << "Timeout = " << this->getTimeout();
    stream << ", ";
    stream << "BrokerPath = ";
    if (this->getBrokerPath().size() > 0) {
        stream << "[";
        for (size_t ibrokerPath = 0; ibrokerPath < this->getBrokerPath().size(); ++ibrokerPath) {
            if (this->getBrokerPath()[ibrokerPath] != NULL) {
                stream << this->getBrokerPath()[ibrokerPath]->toString() << ", ";
            } else {
                stream << "NULL" << ", ";
            }
        }
        stream << "]";
    } else {
        stream << "NULL";
    }
    stream << " }";

    return stream.str();
}

////////////////////////////////////////////////////////////////////////////////
bool DestinationInfo::equals(const DataStructure* value) const {

    if (this == value) {
        return true;
    }

    const DestinationInfo* valuePtr = dynamic_cast<const DestinationInfo*>(value);

    if (valuePtr == NULL || value == NULL) {
        return false;
    }

    if (this->getConnectionId() != NULL) {
        if (!this->getConnectionId()->equals( valuePtr->getConnectionId().get())) {
            return false;
        }
    } else if (valuePtr->getConnectionId() != NULL) {
        return false;
    }
    if (this->getDestination() != NULL) {
        if (!this->getDestination()->equals( valuePtr->getDestination().get())) {
            return false;
        }
    } else if (valuePtr->getDestination() != NULL) {
        return false;
    }
    if (this->getOperationType() != valuePtr->getOperationType()) {
        return false;
    }
    if (this->getTimeout() != valuePtr->getTimeout()) {
        return false;
    }
    for (size_t ibrokerPath = 0; ibrokerPath < this->getBrokerPath().size(); ++ibrokerPath) {
        if (this->getBrokerPath()[ibrokerPath] != NULL ) {
            if (!this->getBrokerPath()[ibrokerPath]->equals( valuePtr->getBrokerPath()[ibrokerPath].get())) {
                return false;
            }
        } else if (valuePtr->getBrokerPath()[ibrokerPath] != NULL) {
            return false;
        }
    }
    if (!BaseCommand::equals(value)) {
        return false;
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////
const decaf::lang::Pointer<ConnectionId>& DestinationInfo::getConnectionId() const {
    return connectionId;
}

////////////////////////////////////////////////////////////////////////////////
decaf::lang::Pointer<ConnectionId>& DestinationInfo::getConnectionId() {
    return connectionId;
}

////////////////////////////////////////////////////////////////////////////////
void DestinationInfo::setConnectionId(const decaf::lang::Pointer<ConnectionId>& connectionId) {
    this->connectionId = connectionId;
}

////////////////////////////////////////////////////////////////////////////////
const decaf::lang::Pointer<ActiveMQDestination>& DestinationInfo::getDestination() const {
    return destination;
}

////////////////////////////////////////////////////////////////////////////////
decaf::lang::Pointer<ActiveMQDestination>& DestinationInfo::getDestination() {
    return destination;
}

////////////////////////////////////////////////////////////////////////////////
void DestinationInfo::setDestination(const decaf::lang::Pointer<ActiveMQDestination>& destination) {
    this->destination = destination;
}

////////////////////////////////////////////////////////////////////////////////
unsigned char DestinationInfo::getOperationType() const {
    return operationType;
}

////////////////////////////////////////////////////////////////////////////////
void DestinationInfo::setOperationType(unsigned char operationType) {
    this->operationType = operationType;
}

////////////////////////////////////////////////////////////////////////////////
long long DestinationInfo::getTimeout() const {
    return timeout;
}

////////////////////////////////////////////////////////////////////////////////
void DestinationInfo::setTimeout(long long timeout) {
    this->timeout = timeout;
}

////////////////////////////////////////////////////////////////////////////////
const std::vector< decaf::lang::Pointer<BrokerId> >& DestinationInfo::getBrokerPath() const {
    return brokerPath;
}

////////////////////////////////////////////////////////////////////////////////
std::vector< decaf::lang::Pointer<BrokerId> >& DestinationInfo::getBrokerPath() {
    return brokerPath;
}

////////////////////////////////////////////////////////////////////////////////
void DestinationInfo::setBrokerPath(const std::vector< decaf::lang::Pointer<BrokerId> >& brokerPath) {
    this->brokerPath = brokerPath;
}

////////////////////////////////////////////////////////////////////////////////
decaf::lang::Pointer<commands::Command> DestinationInfo::visit(activemq::state::CommandVisitor* visitor) {
    return visitor->processDestinationInfo(this);
}
