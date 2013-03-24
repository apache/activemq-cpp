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

#include <activemq/commands/ReplayCommand.h>
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
 *  Command code for OpenWire format for ReplayCommand
 *
 *  NOTE!: This file is auto generated - do not modify!
 *         if you need to make a change, please see the Java Classes in the
 *         activemq-cpp-openwire-generator module
 *
 */

////////////////////////////////////////////////////////////////////////////////
ReplayCommand::ReplayCommand() :
    BaseCommand(), firstNakNumber(0), lastNakNumber(0) {

}

////////////////////////////////////////////////////////////////////////////////
ReplayCommand::~ReplayCommand() {
}

////////////////////////////////////////////////////////////////////////////////
ReplayCommand* ReplayCommand::cloneDataStructure() const {
    std::auto_ptr<ReplayCommand> replayCommand(new ReplayCommand());

    // Copy the data from the base class or classes
    replayCommand->copyDataStructure(this);

    return replayCommand.release();
}

////////////////////////////////////////////////////////////////////////////////
void ReplayCommand::copyDataStructure(const DataStructure* src) {

    // Protect against invalid self assignment.
    if (this == src) {
        return;
    }

    const ReplayCommand* srcPtr = dynamic_cast<const ReplayCommand*>(src);

    if (srcPtr == NULL || src == NULL) {
        throw decaf::lang::exceptions::NullPointerException(
            __FILE__, __LINE__,
            "ReplayCommand::copyDataStructure - src is NULL or invalid");
    }

    // Copy the data of the base class or classes
    BaseCommand::copyDataStructure(src);

    this->setFirstNakNumber(srcPtr->getFirstNakNumber());
    this->setLastNakNumber(srcPtr->getLastNakNumber());
}

////////////////////////////////////////////////////////////////////////////////
unsigned char ReplayCommand::getDataStructureType() const {
    return ReplayCommand::ID_REPLAYCOMMAND;
}

////////////////////////////////////////////////////////////////////////////////
std::string ReplayCommand::toString() const {

    ostringstream stream;

    stream << "ReplayCommand { "
           << "commandId = " << this->getCommandId() << ", "
           << "responseRequired = " << boolalpha << this->isResponseRequired();
    stream << ", ";
    stream << "FirstNakNumber = " << this->getFirstNakNumber();
    stream << ", ";
    stream << "LastNakNumber = " << this->getLastNakNumber();
    stream << " }";

    return stream.str();
}

////////////////////////////////////////////////////////////////////////////////
bool ReplayCommand::equals(const DataStructure* value) const {

    if (this == value) {
        return true;
    }

    const ReplayCommand* valuePtr = dynamic_cast<const ReplayCommand*>(value);

    if (valuePtr == NULL || value == NULL) {
        return false;
    }

    if (this->getFirstNakNumber() != valuePtr->getFirstNakNumber()) {
        return false;
    }
    if (this->getLastNakNumber() != valuePtr->getLastNakNumber()) {
        return false;
    }
    if (!BaseCommand::equals(value)) {
        return false;
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////
int ReplayCommand::getFirstNakNumber() const {
    return firstNakNumber;
}

////////////////////////////////////////////////////////////////////////////////
void ReplayCommand::setFirstNakNumber(int firstNakNumber) {
    this->firstNakNumber = firstNakNumber;
}

////////////////////////////////////////////////////////////////////////////////
int ReplayCommand::getLastNakNumber() const {
    return lastNakNumber;
}

////////////////////////////////////////////////////////////////////////////////
void ReplayCommand::setLastNakNumber(int lastNakNumber) {
    this->lastNakNumber = lastNakNumber;
}

////////////////////////////////////////////////////////////////////////////////
decaf::lang::Pointer<commands::Command> ReplayCommand::visit(activemq::state::CommandVisitor* visitor) {
    return visitor->processReplayCommand(this);
}
