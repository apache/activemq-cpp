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

#include <activemq/commands/LastPartialCommand.h>
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
 *  Command code for OpenWire format for LastPartialCommand
 *
 *  NOTE!: This file is auto generated - do not modify!
 *         if you need to make a change, please see the Java Classes in the
 *         activemq-cpp-openwire-generator module
 *
 */

////////////////////////////////////////////////////////////////////////////////
LastPartialCommand::LastPartialCommand() :
    PartialCommand() {

}

////////////////////////////////////////////////////////////////////////////////
LastPartialCommand::~LastPartialCommand() {
}

////////////////////////////////////////////////////////////////////////////////
LastPartialCommand* LastPartialCommand::cloneDataStructure() const {
    std::auto_ptr<LastPartialCommand> lastPartialCommand(new LastPartialCommand());

    // Copy the data from the base class or classes
    lastPartialCommand->copyDataStructure(this);

    return lastPartialCommand.release();
}

////////////////////////////////////////////////////////////////////////////////
void LastPartialCommand::copyDataStructure(const DataStructure* src) {

    // Protect against invalid self assignment.
    if (this == src) {
        return;
    }

    const LastPartialCommand* srcPtr = dynamic_cast<const LastPartialCommand*>(src);

    if (srcPtr == NULL || src == NULL) {
        throw decaf::lang::exceptions::NullPointerException(
            __FILE__, __LINE__,
            "LastPartialCommand::copyDataStructure - src is NULL or invalid");
    }

    // Copy the data of the base class or classes
    PartialCommand::copyDataStructure(src);

}

////////////////////////////////////////////////////////////////////////////////
unsigned char LastPartialCommand::getDataStructureType() const {
    return LastPartialCommand::ID_LASTPARTIALCOMMAND;
}

////////////////////////////////////////////////////////////////////////////////
std::string LastPartialCommand::toString() const {

    ostringstream stream;

    stream << "LastPartialCommand { ";
    stream << " }";

    return stream.str();
}

////////////////////////////////////////////////////////////////////////////////
bool LastPartialCommand::equals(const DataStructure* value) const {

    if (this == value) {
        return true;
    }

    const LastPartialCommand* valuePtr = dynamic_cast<const LastPartialCommand*>(value);

    if (valuePtr == NULL || value == NULL) {
        return false;
    }

    if (!PartialCommand::equals(value)) {
        return false;
    }
    return true;
}

