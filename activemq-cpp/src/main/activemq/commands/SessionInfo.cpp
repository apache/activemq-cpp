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

#include <activemq/commands/SessionInfo.h>
#include <activemq/exceptions/ActiveMQException.h>
#include <activemq/state/CommandVisitor.h>
#include <cms/Session.h>
#include <decaf/lang/exceptions/NullPointerException.h>

using namespace std;
using namespace activemq;
using namespace activemq::exceptions;
using namespace activemq::commands;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

/*
 *
 *  Command code for OpenWire format for SessionInfo
 *
 *  NOTE!: This file is auto generated - do not modify!
 *         if you need to make a change, please see the Java Classes in the
 *         activemq-cpp-openwire-generator module
 *
 */

////////////////////////////////////////////////////////////////////////////////
SessionInfo::SessionInfo() :
    BaseCommand(), sessionId(NULL), ackMode((unsigned int)cms::Session::AUTO_ACKNOWLEDGE) {

}

////////////////////////////////////////////////////////////////////////////////
SessionInfo::~SessionInfo() {
}

////////////////////////////////////////////////////////////////////////////////
SessionInfo* SessionInfo::cloneDataStructure() const {
    std::auto_ptr<SessionInfo> sessionInfo(new SessionInfo());

    // Copy the data from the base class or classes
    sessionInfo->copyDataStructure(this);

    return sessionInfo.release();
}

////////////////////////////////////////////////////////////////////////////////
void SessionInfo::copyDataStructure(const DataStructure* src) {

    // Protect against invalid self assignment.
    if (this == src) {
        return;
    }

    const SessionInfo* srcPtr = dynamic_cast<const SessionInfo*>(src);

    if (srcPtr == NULL || src == NULL) {
        throw decaf::lang::exceptions::NullPointerException(
            __FILE__, __LINE__,
            "SessionInfo::copyDataStructure - src is NULL or invalid");
    }

    // Copy the data of the base class or classes
    BaseCommand::copyDataStructure(src);

    this->setSessionId(srcPtr->getSessionId());
}

////////////////////////////////////////////////////////////////////////////////
unsigned char SessionInfo::getDataStructureType() const {
    return SessionInfo::ID_SESSIONINFO;
}

////////////////////////////////////////////////////////////////////////////////
std::string SessionInfo::toString() const {

    ostringstream stream;

    stream << "SessionInfo { "
           << "commandId = " << this->getCommandId() << ", "
           << "responseRequired = " << boolalpha << this->isResponseRequired();
    stream << ", ";
    stream << "SessionId = ";
    if (this->getSessionId() != NULL) {
        stream << this->getSessionId()->toString();
    } else {
        stream << "NULL";
    }
    stream << " }";

    return stream.str();
}

////////////////////////////////////////////////////////////////////////////////
bool SessionInfo::equals(const DataStructure* value) const {

    if (this == value) {
        return true;
    }

    const SessionInfo* valuePtr = dynamic_cast<const SessionInfo*>(value);

    if (valuePtr == NULL || value == NULL) {
        return false;
    }

    if (this->getSessionId() != NULL) {
        if (!this->getSessionId()->equals( valuePtr->getSessionId().get())) {
            return false;
        }
    } else if (valuePtr->getSessionId() != NULL) {
        return false;
    }
    if (!BaseCommand::equals(value)) {
        return false;
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////
const decaf::lang::Pointer<SessionId>& SessionInfo::getSessionId() const {
    return sessionId;
}

////////////////////////////////////////////////////////////////////////////////
decaf::lang::Pointer<SessionId>& SessionInfo::getSessionId() {
    return sessionId;
}

////////////////////////////////////////////////////////////////////////////////
void SessionInfo::setSessionId(const decaf::lang::Pointer<SessionId>& sessionId) {
    this->sessionId = sessionId;
}

////////////////////////////////////////////////////////////////////////////////
decaf::lang::Pointer<commands::Command> SessionInfo::visit(activemq::state::CommandVisitor* visitor) {
    return visitor->processSessionInfo(this);
}
////////////////////////////////////////////////////////////////////////////////
Pointer<RemoveInfo> SessionInfo::createRemoveCommand() const {
    Pointer<RemoveInfo> info(new RemoveInfo());
    info->setResponseRequired(this->isResponseRequired());
    info->setObjectId(this->getSessionId());
    return info;
}

