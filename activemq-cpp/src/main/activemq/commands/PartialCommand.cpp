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

#include <activemq/commands/PartialCommand.h>
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
 *  Command code for OpenWire format for PartialCommand
 *
 *  NOTE!: This file is auto generated - do not modify!
 *         if you need to make a change, please see the Java Classes in the
 *         activemq-cpp-openwire-generator module
 *
 */

////////////////////////////////////////////////////////////////////////////////
PartialCommand::PartialCommand() :
    BaseDataStructure(), commandId(0), data() {

}

////////////////////////////////////////////////////////////////////////////////
PartialCommand::~PartialCommand() {
}

////////////////////////////////////////////////////////////////////////////////
PartialCommand* PartialCommand::cloneDataStructure() const {
    std::auto_ptr<PartialCommand> partialCommand(new PartialCommand());

    // Copy the data from the base class or classes
    partialCommand->copyDataStructure(this);

    return partialCommand.release();
}

////////////////////////////////////////////////////////////////////////////////
void PartialCommand::copyDataStructure(const DataStructure* src) {

    // Protect against invalid self assignment.
    if (this == src) {
        return;
    }

    const PartialCommand* srcPtr = dynamic_cast<const PartialCommand*>(src);

    if (srcPtr == NULL || src == NULL) {
        throw decaf::lang::exceptions::NullPointerException(
            __FILE__, __LINE__,
            "PartialCommand::copyDataStructure - src is NULL or invalid");
    }

    // Copy the data of the base class or classes
    BaseDataStructure::copyDataStructure(src);

    this->setCommandId(srcPtr->getCommandId());
    this->setData(srcPtr->getData());
}

////////////////////////////////////////////////////////////////////////////////
unsigned char PartialCommand::getDataStructureType() const {
    return PartialCommand::ID_PARTIALCOMMAND;
}

////////////////////////////////////////////////////////////////////////////////
std::string PartialCommand::toString() const {

    ostringstream stream;

    stream << "PartialCommand { ";
    stream << "CommandId = " << this->getCommandId();
    stream << ", ";
    stream << "Data = ";
    if (this->getData().size() > 0) {
        stream << "[";
        for (size_t idata = 0; idata < this->getData().size(); ++idata) {
            stream << this->getData()[idata] << ",";
        }
        stream << "]";
    } else {
        stream << "NULL";
    }
    stream << " }";

    return stream.str();
}

////////////////////////////////////////////////////////////////////////////////
bool PartialCommand::equals(const DataStructure* value) const {

    if (this == value) {
        return true;
    }

    const PartialCommand* valuePtr = dynamic_cast<const PartialCommand*>(value);

    if (valuePtr == NULL || value == NULL) {
        return false;
    }

    if (this->getCommandId() != valuePtr->getCommandId()) {
        return false;
    }
    for (size_t idata = 0; idata < this->getData().size(); ++idata) {
        if (this->getData()[idata] != valuePtr->getData()[idata]) {
            return false;
        }
    }
    if (!BaseDataStructure::equals(value)) {
        return false;
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////
int PartialCommand::getCommandId() const {
    return commandId;
}

////////////////////////////////////////////////////////////////////////////////
void PartialCommand::setCommandId(int commandId) {
    this->commandId = commandId;
}

////////////////////////////////////////////////////////////////////////////////
const std::vector<unsigned char>& PartialCommand::getData() const {
    return data;
}

////////////////////////////////////////////////////////////////////////////////
std::vector<unsigned char>& PartialCommand::getData() {
    return data;
}

////////////////////////////////////////////////////////////////////////////////
void PartialCommand::setData(const std::vector<unsigned char>& data) {
    this->data = data;
}

