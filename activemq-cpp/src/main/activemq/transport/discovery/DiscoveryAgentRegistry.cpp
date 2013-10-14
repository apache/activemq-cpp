/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "DiscoveryAgentRegistry.h"

#include <activemq/exceptions/ActiveMQException.h>
#include <activemq/transport/discovery/DiscoveryAgentFactory.h>

using namespace std;
using namespace activemq;
using namespace activemq::exceptions;
using namespace activemq::transport;
using namespace activemq::transport::discovery;
using namespace decaf;
using namespace decaf::util;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
namespace {
    DiscoveryAgentRegistry* theOnlyInstance;
}

////////////////////////////////////////////////////////////////////////////////
DiscoveryAgentRegistry::DiscoveryAgentRegistry() : registry() {
}

////////////////////////////////////////////////////////////////////////////////
DiscoveryAgentRegistry::~DiscoveryAgentRegistry() {
    try {
        this->unregisterAllFactories();
    }
    AMQ_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
DiscoveryAgentFactory* DiscoveryAgentRegistry::findFactory(const std::string& name) const {

    if (!this->registry.containsKey(name)) {
        throw NoSuchElementException(__FILE__, __LINE__,
            "No Matching Factory Registered for format := %s", name.c_str());
    }

    return this->registry.get(name);
}

////////////////////////////////////////////////////////////////////////////////
void DiscoveryAgentRegistry::registerFactory(const std::string& name, DiscoveryAgentFactory* factory) {

    if (name == "") {
        throw IllegalArgumentException(__FILE__, __LINE__, "DiscoveryAgentFactory name cannot be the empty string");
    }

    if (factory == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "Supplied DiscoveryAgentFactory pointer was NULL");
    }

    this->registry.put(name, factory);
}

////////////////////////////////////////////////////////////////////////////////
void DiscoveryAgentRegistry::unregisterFactory(const std::string& name) {
    if (this->registry.containsKey(name)) {
        delete this->registry.get(name);
        this->registry.remove(name);
    }
}

////////////////////////////////////////////////////////////////////////////////
void DiscoveryAgentRegistry::unregisterAllFactories() {

    Pointer<Iterator<DiscoveryAgentFactory*> > iterator(this->registry.values().iterator());
    while (iterator->hasNext()) {
        delete iterator->next();
    }

    this->registry.clear();
}

////////////////////////////////////////////////////////////////////////////////
std::vector<std::string> DiscoveryAgentRegistry::getAgentNames() const {
    return this->registry.keySet().toArray();
}

////////////////////////////////////////////////////////////////////////////////
DiscoveryAgentRegistry& DiscoveryAgentRegistry::getInstance() {
    return *theOnlyInstance;
}

////////////////////////////////////////////////////////////////////////////////
void DiscoveryAgentRegistry::initialize() {
    theOnlyInstance = new DiscoveryAgentRegistry();
}

////////////////////////////////////////////////////////////////////////////////
void DiscoveryAgentRegistry::shutdown() {
    theOnlyInstance->unregisterAllFactories();
    delete theOnlyInstance;
    theOnlyInstance = NULL;
}
