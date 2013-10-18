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

#include "DiscoveryAgentRegistryTest.h"

#include <activemq/transport/discovery/DiscoveryAgent.h>
#include <activemq/transport/discovery/DiscoveryAgentFactory.h>
#include <activemq/transport/discovery/DiscoveryAgentRegistry.h>

#include <decaf/net/URI.h>

using namespace activemq;
using namespace activemq::transport;
using namespace activemq::transport::discovery;
using namespace decaf;
using namespace decaf::util;
using namespace decaf::net;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
namespace {

    class MockDiscoveryAgent : public DiscoveryAgent {
    public:

        virtual void start() {}
        virtual void stop() {}
        virtual void setDiscoveryListener(DiscoveryListener* listener) {}
        virtual void registerService(const std::string& name) {}
        virtual void serviceFailed(const activemq::commands::DiscoveryEvent& event) {}
        virtual std::string toString() const { return "MockDiscoveryAgent"; }
        virtual URI getDiscoveryURI() const { return URI(); }
        virtual void setDiscoveryURI(const URI& discoveryURI) {}
    };

    class MockDiscoveryAgentFactory : public DiscoveryAgentFactory {
    public:

        virtual ~MockDiscoveryAgentFactory() {}

        virtual decaf::lang::Pointer<DiscoveryAgent> createAgent(const decaf::net::URI& agentURI) {
            return Pointer<DiscoveryAgent>(new MockDiscoveryAgent);
        }

    };

}

////////////////////////////////////////////////////////////////////////////////
DiscoveryAgentRegistryTest::DiscoveryAgentRegistryTest() {
}

////////////////////////////////////////////////////////////////////////////////
DiscoveryAgentRegistryTest::~DiscoveryAgentRegistryTest() {
}

////////////////////////////////////////////////////////////////////////////////
void DiscoveryAgentRegistryTest::test() {

    DiscoveryAgentRegistry& registry = DiscoveryAgentRegistry::getInstance();
    registry.registerFactory("mock", new MockDiscoveryAgentFactory);

    CPPUNIT_ASSERT_EQUAL(1, (int) registry.getAgentNames().size());

    DiscoveryAgentFactory* factory = registry.findFactory("mock");
    CPPUNIT_ASSERT(factory != NULL);

    Pointer<DiscoveryAgent> agent(factory->createAgent(URI("mock://default")));
    CPPUNIT_ASSERT(agent != NULL);

    Pointer<MockDiscoveryAgent> mock = agent.dynamicCast<MockDiscoveryAgent>();
    CPPUNIT_ASSERT(mock != NULL);

    registry.unregisterAllFactories();
}
