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

#include <activemq/commands/ControlCommand.h>
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
 *  Command code for OpenWire format for ControlCommand
 *
 *  NOTE!: This file is auto generated - do not modify!
 *         if you need to make a change, please see the Java Classes in the
 *         activemq-cpp-openwire-generator module
 *
 */

////////////////////////////////////////////////////////////////////////////////
ControlCommand::ControlCommand() :
    BaseCommand(), command("") {

}

////////////////////////////////////////////////////////////////////////////////
ControlCommand::~ControlCommand() {
}

////////////////////////////////////////////////////////////////////////////////
ControlCommand* ControlCommand::cloneDataStructure() const {
    std::auto_ptr<ControlCommand> controlCommand(new ControlCommand());

    // Copy the data from the base class or classes
    controlCommand->copyDataStructure(this);

    return controlCommand.release();
}

////////////////////////////////////////////////////////////////////////////////
void ControlCommand::copyDataStructure(const DataStructure* src) {

    // Protect against invalid self assignment.
    if (this == src) {
        return;
    }

    const ControlCommand* srcPtr = dynamic_cast<const ControlCommand*>(src);

    if (srcPtr == NULL || src == NULL) {
        throw decaf::lang::exceptions::NullPointerException(
            __FILE__, __LINE__,
            "ControlCommand::copyDataStructure - src is NULL or invalid");
    }

    // Copy the data of the base class or classes
    BaseCommand::copyDataStructure(src);

    this->setCommand(srcPtr->getCommand());
}

////////////////////////////////////////////////////////////////////////////////
unsigned char ControlCommand::getDataStructureType() const {
    return ControlCommand::ID_CONTROLCOMMAND;
}

////////////////////////////////////////////////////////////////////////////////
std::string ControlCommand::toString() const {

    ostringstream stream;

    stream << "ControlCommand { "
           << "commandId = " << this->getCommandId() << ", "
           << "responseRequired = " << boolalpha << this->isResponseRequired();
    stream << ", ";
    stream << "Command = " << this->getCommand();
    stream << " }";

    return stream.str();
}

////////////////////////////////////////////////////////////////////////////////
bool ControlCommand::equals(const DataStructure* value) const {

    if (this == value) {
        return true;
    }

    const ControlCommand* valuePtr = dynamic_cast<const ControlCommand*>(value);

    if (valuePtr == NULL || value == NULL) {
        return false;
    }

    if (this->getCommand() != valuePtr->getCommand()) {
        return false;
    }
    if (!BaseCommand::equals(value)) {
        return false;
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////
const std::string& ControlCommand::getCommand() const {
    return command;
}

////////////////////////////////////////////////////////////////////////////////
std::string& ControlCommand::getCommand() {
    return command;
}

////////////////////////////////////////////////////////////////////////////////
void ControlCommand::setCommand(const std::string& command) {
    this->command = command;
}

////////////////////////////////////////////////////////////////////////////////
decaf::lang::Pointer<commands::Command> ControlCommand::visit(activemq::state::CommandVisitor* visitor) {
    return visitor->processControlCommand(this);
}
