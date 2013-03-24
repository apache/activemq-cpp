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

#include <activemq/commands/DiscoveryEvent.h>
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
 *  Command code for OpenWire format for DiscoveryEvent
 *
 *  NOTE!: This file is auto generated - do not modify!
 *         if you need to make a change, please see the Java Classes in the
 *         activemq-cpp-openwire-generator module
 *
 */

////////////////////////////////////////////////////////////////////////////////
DiscoveryEvent::DiscoveryEvent() :
    BaseDataStructure(), serviceName(""), brokerName("") {

}

////////////////////////////////////////////////////////////////////////////////
DiscoveryEvent::~DiscoveryEvent() {
}

////////////////////////////////////////////////////////////////////////////////
DiscoveryEvent* DiscoveryEvent::cloneDataStructure() const {
    std::auto_ptr<DiscoveryEvent> discoveryEvent(new DiscoveryEvent());

    // Copy the data from the base class or classes
    discoveryEvent->copyDataStructure(this);

    return discoveryEvent.release();
}

////////////////////////////////////////////////////////////////////////////////
void DiscoveryEvent::copyDataStructure(const DataStructure* src) {

    // Protect against invalid self assignment.
    if (this == src) {
        return;
    }

    const DiscoveryEvent* srcPtr = dynamic_cast<const DiscoveryEvent*>(src);

    if (srcPtr == NULL || src == NULL) {
        throw decaf::lang::exceptions::NullPointerException(
            __FILE__, __LINE__,
            "DiscoveryEvent::copyDataStructure - src is NULL or invalid");
    }

    // Copy the data of the base class or classes
    BaseDataStructure::copyDataStructure(src);

    this->setServiceName(srcPtr->getServiceName());
    this->setBrokerName(srcPtr->getBrokerName());
}

////////////////////////////////////////////////////////////////////////////////
unsigned char DiscoveryEvent::getDataStructureType() const {
    return DiscoveryEvent::ID_DISCOVERYEVENT;
}

////////////////////////////////////////////////////////////////////////////////
std::string DiscoveryEvent::toString() const {

    ostringstream stream;

    stream << "DiscoveryEvent { ";
    stream << "ServiceName = " << this->getServiceName();
    stream << ", ";
    stream << "BrokerName = " << this->getBrokerName();
    stream << " }";

    return stream.str();
}

////////////////////////////////////////////////////////////////////////////////
bool DiscoveryEvent::equals(const DataStructure* value) const {

    if (this == value) {
        return true;
    }

    const DiscoveryEvent* valuePtr = dynamic_cast<const DiscoveryEvent*>(value);

    if (valuePtr == NULL || value == NULL) {
        return false;
    }

    if (this->getServiceName() != valuePtr->getServiceName()) {
        return false;
    }
    if (this->getBrokerName() != valuePtr->getBrokerName()) {
        return false;
    }
    if (!BaseDataStructure::equals(value)) {
        return false;
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////
const std::string& DiscoveryEvent::getServiceName() const {
    return serviceName;
}

////////////////////////////////////////////////////////////////////////////////
std::string& DiscoveryEvent::getServiceName() {
    return serviceName;
}

////////////////////////////////////////////////////////////////////////////////
void DiscoveryEvent::setServiceName(const std::string& serviceName) {
    this->serviceName = serviceName;
}

////////////////////////////////////////////////////////////////////////////////
const std::string& DiscoveryEvent::getBrokerName() const {
    return brokerName;
}

////////////////////////////////////////////////////////////////////////////////
std::string& DiscoveryEvent::getBrokerName() {
    return brokerName;
}

////////////////////////////////////////////////////////////////////////////////
void DiscoveryEvent::setBrokerName(const std::string& brokerName) {
    this->brokerName = brokerName;
}

