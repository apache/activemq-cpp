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

#include <activemq/commands/BrokerInfo.h>
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
 *  Command code for OpenWire format for BrokerInfo
 *
 *  NOTE!: This file is auto generated - do not modify!
 *         if you need to make a change, please see the Java Classes in the
 *         activemq-cpp-openwire-generator module
 *
 */

////////////////////////////////////////////////////////////////////////////////
BrokerInfo::BrokerInfo() :
    BaseCommand(), brokerId(NULL), brokerURL(""), peerBrokerInfos(), brokerName(""), slaveBroker(false), masterBroker(false), 
      faultTolerantConfiguration(false), duplexConnection(false), networkConnection(false), connectionId(0), brokerUploadUrl(""), 
      networkProperties("") {

}

////////////////////////////////////////////////////////////////////////////////
BrokerInfo::~BrokerInfo() {
}

////////////////////////////////////////////////////////////////////////////////
BrokerInfo* BrokerInfo::cloneDataStructure() const {
    std::auto_ptr<BrokerInfo> brokerInfo(new BrokerInfo());

    // Copy the data from the base class or classes
    brokerInfo->copyDataStructure(this);

    return brokerInfo.release();
}

////////////////////////////////////////////////////////////////////////////////
void BrokerInfo::copyDataStructure(const DataStructure* src) {

    // Protect against invalid self assignment.
    if (this == src) {
        return;
    }

    const BrokerInfo* srcPtr = dynamic_cast<const BrokerInfo*>(src);

    if (srcPtr == NULL || src == NULL) {
        throw decaf::lang::exceptions::NullPointerException(
            __FILE__, __LINE__,
            "BrokerInfo::copyDataStructure - src is NULL or invalid");
    }

    // Copy the data of the base class or classes
    BaseCommand::copyDataStructure(src);

    this->setBrokerId(srcPtr->getBrokerId());
    this->setBrokerURL(srcPtr->getBrokerURL());
    this->setPeerBrokerInfos(srcPtr->getPeerBrokerInfos());
    this->setBrokerName(srcPtr->getBrokerName());
    this->setSlaveBroker(srcPtr->isSlaveBroker());
    this->setMasterBroker(srcPtr->isMasterBroker());
    this->setFaultTolerantConfiguration(srcPtr->isFaultTolerantConfiguration());
    this->setDuplexConnection(srcPtr->isDuplexConnection());
    this->setNetworkConnection(srcPtr->isNetworkConnection());
    this->setConnectionId(srcPtr->getConnectionId());
    this->setBrokerUploadUrl(srcPtr->getBrokerUploadUrl());
    this->setNetworkProperties(srcPtr->getNetworkProperties());
}

////////////////////////////////////////////////////////////////////////////////
unsigned char BrokerInfo::getDataStructureType() const {
    return BrokerInfo::ID_BROKERINFO;
}

////////////////////////////////////////////////////////////////////////////////
std::string BrokerInfo::toString() const {

    ostringstream stream;

    stream << "BrokerInfo { "
           << "commandId = " << this->getCommandId() << ", "
           << "responseRequired = " << boolalpha << this->isResponseRequired();
    stream << ", ";
    stream << "BrokerId = ";
    if (this->getBrokerId() != NULL) {
        stream << this->getBrokerId()->toString();
    } else {
        stream << "NULL";
    }
    stream << ", ";
    stream << "BrokerURL = " << this->getBrokerURL();
    stream << ", ";
    stream << "PeerBrokerInfos = ";
    if (this->getPeerBrokerInfos().size() > 0) {
        stream << "[";
        for (size_t ipeerBrokerInfos = 0; ipeerBrokerInfos < this->getPeerBrokerInfos().size(); ++ipeerBrokerInfos) {
            if (this->getPeerBrokerInfos()[ipeerBrokerInfos] != NULL) {
                stream << this->getPeerBrokerInfos()[ipeerBrokerInfos]->toString() << ", ";
            } else {
                stream << "NULL" << ", ";
            }
        }
        stream << "]";
    } else {
        stream << "NULL";
    }
    stream << ", ";
    stream << "BrokerName = " << this->getBrokerName();
    stream << ", ";
    stream << "SlaveBroker = " << this->isSlaveBroker();
    stream << ", ";
    stream << "MasterBroker = " << this->isMasterBroker();
    stream << ", ";
    stream << "FaultTolerantConfiguration = " << this->isFaultTolerantConfiguration();
    stream << ", ";
    stream << "DuplexConnection = " << this->isDuplexConnection();
    stream << ", ";
    stream << "NetworkConnection = " << this->isNetworkConnection();
    stream << ", ";
    stream << "ConnectionId = " << this->getConnectionId();
    stream << ", ";
    stream << "BrokerUploadUrl = " << this->getBrokerUploadUrl();
    stream << ", ";
    stream << "NetworkProperties = " << this->getNetworkProperties();
    stream << " }";

    return stream.str();
}

////////////////////////////////////////////////////////////////////////////////
bool BrokerInfo::equals(const DataStructure* value) const {

    if (this == value) {
        return true;
    }

    const BrokerInfo* valuePtr = dynamic_cast<const BrokerInfo*>(value);

    if (valuePtr == NULL || value == NULL) {
        return false;
    }

    if (this->getBrokerId() != NULL) {
        if (!this->getBrokerId()->equals( valuePtr->getBrokerId().get())) {
            return false;
        }
    } else if (valuePtr->getBrokerId() != NULL) {
        return false;
    }
    if (this->getBrokerURL() != valuePtr->getBrokerURL()) {
        return false;
    }
    for (size_t ipeerBrokerInfos = 0; ipeerBrokerInfos < this->getPeerBrokerInfos().size(); ++ipeerBrokerInfos) {
        if (this->getPeerBrokerInfos()[ipeerBrokerInfos] != NULL ) {
            if (!this->getPeerBrokerInfos()[ipeerBrokerInfos]->equals( valuePtr->getPeerBrokerInfos()[ipeerBrokerInfos].get())) {
                return false;
            }
        } else if (valuePtr->getPeerBrokerInfos()[ipeerBrokerInfos] != NULL) {
            return false;
        }
    }
    if (this->getBrokerName() != valuePtr->getBrokerName()) {
        return false;
    }
    if (this->isSlaveBroker() != valuePtr->isSlaveBroker()) {
        return false;
    }
    if (this->isMasterBroker() != valuePtr->isMasterBroker()) {
        return false;
    }
    if (this->isFaultTolerantConfiguration() != valuePtr->isFaultTolerantConfiguration()) {
        return false;
    }
    if (this->isDuplexConnection() != valuePtr->isDuplexConnection()) {
        return false;
    }
    if (this->isNetworkConnection() != valuePtr->isNetworkConnection()) {
        return false;
    }
    if (this->getConnectionId() != valuePtr->getConnectionId()) {
        return false;
    }
    if (this->getBrokerUploadUrl() != valuePtr->getBrokerUploadUrl()) {
        return false;
    }
    if (this->getNetworkProperties() != valuePtr->getNetworkProperties()) {
        return false;
    }
    if (!BaseCommand::equals(value)) {
        return false;
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////
const decaf::lang::Pointer<BrokerId>& BrokerInfo::getBrokerId() const {
    return brokerId;
}

////////////////////////////////////////////////////////////////////////////////
decaf::lang::Pointer<BrokerId>& BrokerInfo::getBrokerId() {
    return brokerId;
}

////////////////////////////////////////////////////////////////////////////////
void BrokerInfo::setBrokerId(const decaf::lang::Pointer<BrokerId>& brokerId) {
    this->brokerId = brokerId;
}

////////////////////////////////////////////////////////////////////////////////
const std::string& BrokerInfo::getBrokerURL() const {
    return brokerURL;
}

////////////////////////////////////////////////////////////////////////////////
std::string& BrokerInfo::getBrokerURL() {
    return brokerURL;
}

////////////////////////////////////////////////////////////////////////////////
void BrokerInfo::setBrokerURL(const std::string& brokerURL) {
    this->brokerURL = brokerURL;
}

////////////////////////////////////////////////////////////////////////////////
const std::vector< decaf::lang::Pointer<BrokerInfo> >& BrokerInfo::getPeerBrokerInfos() const {
    return peerBrokerInfos;
}

////////////////////////////////////////////////////////////////////////////////
std::vector< decaf::lang::Pointer<BrokerInfo> >& BrokerInfo::getPeerBrokerInfos() {
    return peerBrokerInfos;
}

////////////////////////////////////////////////////////////////////////////////
void BrokerInfo::setPeerBrokerInfos(const std::vector< decaf::lang::Pointer<BrokerInfo> >& peerBrokerInfos) {
    this->peerBrokerInfos = peerBrokerInfos;
}

////////////////////////////////////////////////////////////////////////////////
const std::string& BrokerInfo::getBrokerName() const {
    return brokerName;
}

////////////////////////////////////////////////////////////////////////////////
std::string& BrokerInfo::getBrokerName() {
    return brokerName;
}

////////////////////////////////////////////////////////////////////////////////
void BrokerInfo::setBrokerName(const std::string& brokerName) {
    this->brokerName = brokerName;
}

////////////////////////////////////////////////////////////////////////////////
bool BrokerInfo::isSlaveBroker() const {
    return slaveBroker;
}

////////////////////////////////////////////////////////////////////////////////
void BrokerInfo::setSlaveBroker(bool slaveBroker) {
    this->slaveBroker = slaveBroker;
}

////////////////////////////////////////////////////////////////////////////////
bool BrokerInfo::isMasterBroker() const {
    return masterBroker;
}

////////////////////////////////////////////////////////////////////////////////
void BrokerInfo::setMasterBroker(bool masterBroker) {
    this->masterBroker = masterBroker;
}

////////////////////////////////////////////////////////////////////////////////
bool BrokerInfo::isFaultTolerantConfiguration() const {
    return faultTolerantConfiguration;
}

////////////////////////////////////////////////////////////////////////////////
void BrokerInfo::setFaultTolerantConfiguration(bool faultTolerantConfiguration) {
    this->faultTolerantConfiguration = faultTolerantConfiguration;
}

////////////////////////////////////////////////////////////////////////////////
bool BrokerInfo::isDuplexConnection() const {
    return duplexConnection;
}

////////////////////////////////////////////////////////////////////////////////
void BrokerInfo::setDuplexConnection(bool duplexConnection) {
    this->duplexConnection = duplexConnection;
}

////////////////////////////////////////////////////////////////////////////////
bool BrokerInfo::isNetworkConnection() const {
    return networkConnection;
}

////////////////////////////////////////////////////////////////////////////////
void BrokerInfo::setNetworkConnection(bool networkConnection) {
    this->networkConnection = networkConnection;
}

////////////////////////////////////////////////////////////////////////////////
long long BrokerInfo::getConnectionId() const {
    return connectionId;
}

////////////////////////////////////////////////////////////////////////////////
void BrokerInfo::setConnectionId(long long connectionId) {
    this->connectionId = connectionId;
}

////////////////////////////////////////////////////////////////////////////////
const std::string& BrokerInfo::getBrokerUploadUrl() const {
    return brokerUploadUrl;
}

////////////////////////////////////////////////////////////////////////////////
std::string& BrokerInfo::getBrokerUploadUrl() {
    return brokerUploadUrl;
}

////////////////////////////////////////////////////////////////////////////////
void BrokerInfo::setBrokerUploadUrl(const std::string& brokerUploadUrl) {
    this->brokerUploadUrl = brokerUploadUrl;
}

////////////////////////////////////////////////////////////////////////////////
const std::string& BrokerInfo::getNetworkProperties() const {
    return networkProperties;
}

////////////////////////////////////////////////////////////////////////////////
std::string& BrokerInfo::getNetworkProperties() {
    return networkProperties;
}

////////////////////////////////////////////////////////////////////////////////
void BrokerInfo::setNetworkProperties(const std::string& networkProperties) {
    this->networkProperties = networkProperties;
}

////////////////////////////////////////////////////////////////////////////////
decaf::lang::Pointer<commands::Command> BrokerInfo::visit(activemq::state::CommandVisitor* visitor) {
    return visitor->processBrokerInfo(this);
}
