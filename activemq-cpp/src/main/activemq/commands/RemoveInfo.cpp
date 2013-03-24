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

#include <activemq/commands/RemoveInfo.h>
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
 *  Command code for OpenWire format for RemoveInfo
 *
 *  NOTE!: This file is auto generated - do not modify!
 *         if you need to make a change, please see the Java Classes in the
 *         activemq-cpp-openwire-generator module
 *
 */

////////////////////////////////////////////////////////////////////////////////
RemoveInfo::RemoveInfo() :
    BaseCommand(), objectId(NULL), lastDeliveredSequenceId(0) {

}

////////////////////////////////////////////////////////////////////////////////
RemoveInfo::~RemoveInfo() {
}

////////////////////////////////////////////////////////////////////////////////
RemoveInfo* RemoveInfo::cloneDataStructure() const {
    std::auto_ptr<RemoveInfo> removeInfo(new RemoveInfo());

    // Copy the data from the base class or classes
    removeInfo->copyDataStructure(this);

    return removeInfo.release();
}

////////////////////////////////////////////////////////////////////////////////
void RemoveInfo::copyDataStructure(const DataStructure* src) {

    // Protect against invalid self assignment.
    if (this == src) {
        return;
    }

    const RemoveInfo* srcPtr = dynamic_cast<const RemoveInfo*>(src);

    if (srcPtr == NULL || src == NULL) {
        throw decaf::lang::exceptions::NullPointerException(
            __FILE__, __LINE__,
            "RemoveInfo::copyDataStructure - src is NULL or invalid");
    }

    // Copy the data of the base class or classes
    BaseCommand::copyDataStructure(src);

    this->setObjectId(srcPtr->getObjectId());
    this->setLastDeliveredSequenceId(srcPtr->getLastDeliveredSequenceId());
}

////////////////////////////////////////////////////////////////////////////////
unsigned char RemoveInfo::getDataStructureType() const {
    return RemoveInfo::ID_REMOVEINFO;
}

////////////////////////////////////////////////////////////////////////////////
std::string RemoveInfo::toString() const {

    ostringstream stream;

    stream << "RemoveInfo { "
           << "commandId = " << this->getCommandId() << ", "
           << "responseRequired = " << boolalpha << this->isResponseRequired();
    stream << ", ";
    stream << "ObjectId = ";
    if (this->getObjectId() != NULL) {
        stream << this->getObjectId()->toString();
    } else {
        stream << "NULL";
    }
    stream << ", ";
    stream << "LastDeliveredSequenceId = " << this->getLastDeliveredSequenceId();
    stream << " }";

    return stream.str();
}

////////////////////////////////////////////////////////////////////////////////
bool RemoveInfo::equals(const DataStructure* value) const {

    if (this == value) {
        return true;
    }

    const RemoveInfo* valuePtr = dynamic_cast<const RemoveInfo*>(value);

    if (valuePtr == NULL || value == NULL) {
        return false;
    }

    if (this->getObjectId() != NULL) {
        if (!this->getObjectId()->equals( valuePtr->getObjectId().get())) {
            return false;
        }
    } else if (valuePtr->getObjectId() != NULL) {
        return false;
    }
    if (this->getLastDeliveredSequenceId() != valuePtr->getLastDeliveredSequenceId()) {
        return false;
    }
    if (!BaseCommand::equals(value)) {
        return false;
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////
const decaf::lang::Pointer<DataStructure>& RemoveInfo::getObjectId() const {
    return objectId;
}

////////////////////////////////////////////////////////////////////////////////
decaf::lang::Pointer<DataStructure>& RemoveInfo::getObjectId() {
    return objectId;
}

////////////////////////////////////////////////////////////////////////////////
void RemoveInfo::setObjectId(const decaf::lang::Pointer<DataStructure>& objectId) {
    this->objectId = objectId;
}

////////////////////////////////////////////////////////////////////////////////
long long RemoveInfo::getLastDeliveredSequenceId() const {
    return lastDeliveredSequenceId;
}

////////////////////////////////////////////////////////////////////////////////
void RemoveInfo::setLastDeliveredSequenceId(long long lastDeliveredSequenceId) {
    this->lastDeliveredSequenceId = lastDeliveredSequenceId;
}

////////////////////////////////////////////////////////////////////////////////
decaf::lang::Pointer<commands::Command> RemoveInfo::visit(activemq::state::CommandVisitor* visitor) {
    return visitor->processRemoveInfo(this);
}
