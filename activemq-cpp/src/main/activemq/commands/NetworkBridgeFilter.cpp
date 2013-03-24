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

#include <activemq/commands/NetworkBridgeFilter.h>
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
 *  Command code for OpenWire format for NetworkBridgeFilter
 *
 *  NOTE!: This file is auto generated - do not modify!
 *         if you need to make a change, please see the Java Classes in the
 *         activemq-cpp-openwire-generator module
 *
 */

////////////////////////////////////////////////////////////////////////////////
NetworkBridgeFilter::NetworkBridgeFilter() :
    BaseDataStructure(), networkTTL(0), networkBrokerId(NULL) {

}

////////////////////////////////////////////////////////////////////////////////
NetworkBridgeFilter::~NetworkBridgeFilter() {
}

////////////////////////////////////////////////////////////////////////////////
NetworkBridgeFilter* NetworkBridgeFilter::cloneDataStructure() const {
    std::auto_ptr<NetworkBridgeFilter> networkBridgeFilter(new NetworkBridgeFilter());

    // Copy the data from the base class or classes
    networkBridgeFilter->copyDataStructure(this);

    return networkBridgeFilter.release();
}

////////////////////////////////////////////////////////////////////////////////
void NetworkBridgeFilter::copyDataStructure(const DataStructure* src) {

    // Protect against invalid self assignment.
    if (this == src) {
        return;
    }

    const NetworkBridgeFilter* srcPtr = dynamic_cast<const NetworkBridgeFilter*>(src);

    if (srcPtr == NULL || src == NULL) {
        throw decaf::lang::exceptions::NullPointerException(
            __FILE__, __LINE__,
            "NetworkBridgeFilter::copyDataStructure - src is NULL or invalid");
    }

    // Copy the data of the base class or classes
    BaseDataStructure::copyDataStructure(src);

    this->setNetworkTTL(srcPtr->getNetworkTTL());
    this->setNetworkBrokerId(srcPtr->getNetworkBrokerId());
}

////////////////////////////////////////////////////////////////////////////////
unsigned char NetworkBridgeFilter::getDataStructureType() const {
    return NetworkBridgeFilter::ID_NETWORKBRIDGEFILTER;
}

////////////////////////////////////////////////////////////////////////////////
std::string NetworkBridgeFilter::toString() const {

    ostringstream stream;

    stream << "NetworkBridgeFilter { ";
    stream << "NetworkTTL = " << this->getNetworkTTL();
    stream << ", ";
    stream << "NetworkBrokerId = ";
    if (this->getNetworkBrokerId() != NULL) {
        stream << this->getNetworkBrokerId()->toString();
    } else {
        stream << "NULL";
    }
    stream << " }";

    return stream.str();
}

////////////////////////////////////////////////////////////////////////////////
bool NetworkBridgeFilter::equals(const DataStructure* value) const {

    if (this == value) {
        return true;
    }

    const NetworkBridgeFilter* valuePtr = dynamic_cast<const NetworkBridgeFilter*>(value);

    if (valuePtr == NULL || value == NULL) {
        return false;
    }

    if (this->getNetworkTTL() != valuePtr->getNetworkTTL()) {
        return false;
    }
    if (this->getNetworkBrokerId() != NULL) {
        if (!this->getNetworkBrokerId()->equals( valuePtr->getNetworkBrokerId().get())) {
            return false;
        }
    } else if (valuePtr->getNetworkBrokerId() != NULL) {
        return false;
    }
    if (!BaseDataStructure::equals(value)) {
        return false;
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////
int NetworkBridgeFilter::getNetworkTTL() const {
    return networkTTL;
}

////////////////////////////////////////////////////////////////////////////////
void NetworkBridgeFilter::setNetworkTTL(int networkTTL) {
    this->networkTTL = networkTTL;
}

////////////////////////////////////////////////////////////////////////////////
const decaf::lang::Pointer<BrokerId>& NetworkBridgeFilter::getNetworkBrokerId() const {
    return networkBrokerId;
}

////////////////////////////////////////////////////////////////////////////////
decaf::lang::Pointer<BrokerId>& NetworkBridgeFilter::getNetworkBrokerId() {
    return networkBrokerId;
}

////////////////////////////////////////////////////////////////////////////////
void NetworkBridgeFilter::setNetworkBrokerId(const decaf::lang::Pointer<BrokerId>& networkBrokerId) {
    this->networkBrokerId = networkBrokerId;
}

