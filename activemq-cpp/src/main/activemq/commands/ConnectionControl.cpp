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

#include <activemq/commands/ConnectionControl.h>
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
 *  Command code for OpenWire format for ConnectionControl
 *
 *  NOTE!: This file is auto generated - do not modify!
 *         if you need to make a change, please see the Java Classes in the
 *         activemq-cpp-openwire-generator module
 *
 */

////////////////////////////////////////////////////////////////////////////////
ConnectionControl::ConnectionControl() :
    BaseCommand(), close(false), exit(false), faultTolerant(false), resume(false), suspend(false), connectedBrokers(""), reconnectTo(""), 
      rebalanceConnection(false), token() {

}

////////////////////////////////////////////////////////////////////////////////
ConnectionControl::~ConnectionControl() {
}

////////////////////////////////////////////////////////////////////////////////
ConnectionControl* ConnectionControl::cloneDataStructure() const {
    std::auto_ptr<ConnectionControl> connectionControl(new ConnectionControl());

    // Copy the data from the base class or classes
    connectionControl->copyDataStructure(this);

    return connectionControl.release();
}

////////////////////////////////////////////////////////////////////////////////
void ConnectionControl::copyDataStructure(const DataStructure* src) {

    // Protect against invalid self assignment.
    if (this == src) {
        return;
    }

    const ConnectionControl* srcPtr = dynamic_cast<const ConnectionControl*>(src);

    if (srcPtr == NULL || src == NULL) {
        throw decaf::lang::exceptions::NullPointerException(
            __FILE__, __LINE__,
            "ConnectionControl::copyDataStructure - src is NULL or invalid");
    }

    // Copy the data of the base class or classes
    BaseCommand::copyDataStructure(src);

    this->setClose(srcPtr->isClose());
    this->setExit(srcPtr->isExit());
    this->setFaultTolerant(srcPtr->isFaultTolerant());
    this->setResume(srcPtr->isResume());
    this->setSuspend(srcPtr->isSuspend());
    this->setConnectedBrokers(srcPtr->getConnectedBrokers());
    this->setReconnectTo(srcPtr->getReconnectTo());
    this->setRebalanceConnection(srcPtr->isRebalanceConnection());
    this->setToken(srcPtr->getToken());
}

////////////////////////////////////////////////////////////////////////////////
unsigned char ConnectionControl::getDataStructureType() const {
    return ConnectionControl::ID_CONNECTIONCONTROL;
}

////////////////////////////////////////////////////////////////////////////////
std::string ConnectionControl::toString() const {

    ostringstream stream;

    stream << "ConnectionControl { "
           << "commandId = " << this->getCommandId() << ", "
           << "responseRequired = " << boolalpha << this->isResponseRequired();
    stream << ", ";
    stream << "Close = " << this->isClose();
    stream << ", ";
    stream << "Exit = " << this->isExit();
    stream << ", ";
    stream << "FaultTolerant = " << this->isFaultTolerant();
    stream << ", ";
    stream << "Resume = " << this->isResume();
    stream << ", ";
    stream << "Suspend = " << this->isSuspend();
    stream << ", ";
    stream << "ConnectedBrokers = " << this->getConnectedBrokers();
    stream << ", ";
    stream << "ReconnectTo = " << this->getReconnectTo();
    stream << ", ";
    stream << "RebalanceConnection = " << this->isRebalanceConnection();
    stream << ", ";
    stream << "Token = ";
    if (this->getToken().size() > 0) {
        stream << "[";
        for (size_t itoken = 0; itoken < this->getToken().size(); ++itoken) {
            stream << this->getToken()[itoken] << ",";
        }
        stream << "]";
    } else {
        stream << "NULL";
    }
    stream << " }";

    return stream.str();
}

////////////////////////////////////////////////////////////////////////////////
bool ConnectionControl::equals(const DataStructure* value) const {

    if (this == value) {
        return true;
    }

    const ConnectionControl* valuePtr = dynamic_cast<const ConnectionControl*>(value);

    if (valuePtr == NULL || value == NULL) {
        return false;
    }

    if (this->isClose() != valuePtr->isClose()) {
        return false;
    }
    if (this->isExit() != valuePtr->isExit()) {
        return false;
    }
    if (this->isFaultTolerant() != valuePtr->isFaultTolerant()) {
        return false;
    }
    if (this->isResume() != valuePtr->isResume()) {
        return false;
    }
    if (this->isSuspend() != valuePtr->isSuspend()) {
        return false;
    }
    if (this->getConnectedBrokers() != valuePtr->getConnectedBrokers()) {
        return false;
    }
    if (this->getReconnectTo() != valuePtr->getReconnectTo()) {
        return false;
    }
    if (this->isRebalanceConnection() != valuePtr->isRebalanceConnection()) {
        return false;
    }
    for (size_t itoken = 0; itoken < this->getToken().size(); ++itoken) {
        if (this->getToken()[itoken] != valuePtr->getToken()[itoken]) {
            return false;
        }
    }
    if (!BaseCommand::equals(value)) {
        return false;
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////
bool ConnectionControl::isClose() const {
    return close;
}

////////////////////////////////////////////////////////////////////////////////
void ConnectionControl::setClose(bool close) {
    this->close = close;
}

////////////////////////////////////////////////////////////////////////////////
bool ConnectionControl::isExit() const {
    return exit;
}

////////////////////////////////////////////////////////////////////////////////
void ConnectionControl::setExit(bool exit) {
    this->exit = exit;
}

////////////////////////////////////////////////////////////////////////////////
bool ConnectionControl::isFaultTolerant() const {
    return faultTolerant;
}

////////////////////////////////////////////////////////////////////////////////
void ConnectionControl::setFaultTolerant(bool faultTolerant) {
    this->faultTolerant = faultTolerant;
}

////////////////////////////////////////////////////////////////////////////////
bool ConnectionControl::isResume() const {
    return resume;
}

////////////////////////////////////////////////////////////////////////////////
void ConnectionControl::setResume(bool resume) {
    this->resume = resume;
}

////////////////////////////////////////////////////////////////////////////////
bool ConnectionControl::isSuspend() const {
    return suspend;
}

////////////////////////////////////////////////////////////////////////////////
void ConnectionControl::setSuspend(bool suspend) {
    this->suspend = suspend;
}

////////////////////////////////////////////////////////////////////////////////
const std::string& ConnectionControl::getConnectedBrokers() const {
    return connectedBrokers;
}

////////////////////////////////////////////////////////////////////////////////
std::string& ConnectionControl::getConnectedBrokers() {
    return connectedBrokers;
}

////////////////////////////////////////////////////////////////////////////////
void ConnectionControl::setConnectedBrokers(const std::string& connectedBrokers) {
    this->connectedBrokers = connectedBrokers;
}

////////////////////////////////////////////////////////////////////////////////
const std::string& ConnectionControl::getReconnectTo() const {
    return reconnectTo;
}

////////////////////////////////////////////////////////////////////////////////
std::string& ConnectionControl::getReconnectTo() {
    return reconnectTo;
}

////////////////////////////////////////////////////////////////////////////////
void ConnectionControl::setReconnectTo(const std::string& reconnectTo) {
    this->reconnectTo = reconnectTo;
}

////////////////////////////////////////////////////////////////////////////////
bool ConnectionControl::isRebalanceConnection() const {
    return rebalanceConnection;
}

////////////////////////////////////////////////////////////////////////////////
void ConnectionControl::setRebalanceConnection(bool rebalanceConnection) {
    this->rebalanceConnection = rebalanceConnection;
}

////////////////////////////////////////////////////////////////////////////////
const std::vector<unsigned char>& ConnectionControl::getToken() const {
    return token;
}

////////////////////////////////////////////////////////////////////////////////
std::vector<unsigned char>& ConnectionControl::getToken() {
    return token;
}

////////////////////////////////////////////////////////////////////////////////
void ConnectionControl::setToken(const std::vector<unsigned char>& token) {
    this->token = token;
}

////////////////////////////////////////////////////////////////////////////////
decaf::lang::Pointer<commands::Command> ConnectionControl::visit(activemq::state::CommandVisitor* visitor) {
    return visitor->processConnectionControl(this);
}
