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

#include <activemq/commands/RemoveSubscriptionInfo.h>
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
 *  Command code for OpenWire format for RemoveSubscriptionInfo
 *
 *  NOTE!: This file is auto generated - do not modify!
 *         if you need to make a change, please see the Java Classes in the
 *         activemq-cpp-openwire-generator module
 *
 */

////////////////////////////////////////////////////////////////////////////////
RemoveSubscriptionInfo::RemoveSubscriptionInfo() :
    BaseCommand(), connectionId(NULL), subcriptionName(""), clientId("") {

}

////////////////////////////////////////////////////////////////////////////////
RemoveSubscriptionInfo::~RemoveSubscriptionInfo() {
}

////////////////////////////////////////////////////////////////////////////////
RemoveSubscriptionInfo* RemoveSubscriptionInfo::cloneDataStructure() const {
    std::auto_ptr<RemoveSubscriptionInfo> removeSubscriptionInfo(new RemoveSubscriptionInfo());

    // Copy the data from the base class or classes
    removeSubscriptionInfo->copyDataStructure(this);

    return removeSubscriptionInfo.release();
}

////////////////////////////////////////////////////////////////////////////////
void RemoveSubscriptionInfo::copyDataStructure(const DataStructure* src) {

    // Protect against invalid self assignment.
    if (this == src) {
        return;
    }

    const RemoveSubscriptionInfo* srcPtr = dynamic_cast<const RemoveSubscriptionInfo*>(src);

    if (srcPtr == NULL || src == NULL) {
        throw decaf::lang::exceptions::NullPointerException(
            __FILE__, __LINE__,
            "RemoveSubscriptionInfo::copyDataStructure - src is NULL or invalid");
    }

    // Copy the data of the base class or classes
    BaseCommand::copyDataStructure(src);

    this->setConnectionId(srcPtr->getConnectionId());
    this->setSubcriptionName(srcPtr->getSubcriptionName());
    this->setClientId(srcPtr->getClientId());
}

////////////////////////////////////////////////////////////////////////////////
unsigned char RemoveSubscriptionInfo::getDataStructureType() const {
    return RemoveSubscriptionInfo::ID_REMOVESUBSCRIPTIONINFO;
}

////////////////////////////////////////////////////////////////////////////////
std::string RemoveSubscriptionInfo::toString() const {

    ostringstream stream;

    stream << "RemoveSubscriptionInfo { "
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
    stream << "SubcriptionName = " << this->getSubcriptionName();
    stream << ", ";
    stream << "ClientId = " << this->getClientId();
    stream << " }";

    return stream.str();
}

////////////////////////////////////////////////////////////////////////////////
bool RemoveSubscriptionInfo::equals(const DataStructure* value) const {

    if (this == value) {
        return true;
    }

    const RemoveSubscriptionInfo* valuePtr = dynamic_cast<const RemoveSubscriptionInfo*>(value);

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
    if (this->getSubcriptionName() != valuePtr->getSubcriptionName()) {
        return false;
    }
    if (this->getClientId() != valuePtr->getClientId()) {
        return false;
    }
    if (!BaseCommand::equals(value)) {
        return false;
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////
const decaf::lang::Pointer<ConnectionId>& RemoveSubscriptionInfo::getConnectionId() const {
    return connectionId;
}

////////////////////////////////////////////////////////////////////////////////
decaf::lang::Pointer<ConnectionId>& RemoveSubscriptionInfo::getConnectionId() {
    return connectionId;
}

////////////////////////////////////////////////////////////////////////////////
void RemoveSubscriptionInfo::setConnectionId(const decaf::lang::Pointer<ConnectionId>& connectionId) {
    this->connectionId = connectionId;
}

////////////////////////////////////////////////////////////////////////////////
const std::string& RemoveSubscriptionInfo::getSubcriptionName() const {
    return subcriptionName;
}

////////////////////////////////////////////////////////////////////////////////
std::string& RemoveSubscriptionInfo::getSubcriptionName() {
    return subcriptionName;
}

////////////////////////////////////////////////////////////////////////////////
void RemoveSubscriptionInfo::setSubcriptionName(const std::string& subcriptionName) {
    this->subcriptionName = subcriptionName;
}

////////////////////////////////////////////////////////////////////////////////
const std::string& RemoveSubscriptionInfo::getClientId() const {
    return clientId;
}

////////////////////////////////////////////////////////////////////////////////
std::string& RemoveSubscriptionInfo::getClientId() {
    return clientId;
}

////////////////////////////////////////////////////////////////////////////////
void RemoveSubscriptionInfo::setClientId(const std::string& clientId) {
    this->clientId = clientId;
}

////////////////////////////////////////////////////////////////////////////////
decaf::lang::Pointer<commands::Command> RemoveSubscriptionInfo::visit(activemq::state::CommandVisitor* visitor) {
    return visitor->processRemoveSubscriptionInfo(this);
}
