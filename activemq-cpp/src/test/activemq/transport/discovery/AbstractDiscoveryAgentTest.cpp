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

#include "AbstractDiscoveryAgentTest.h"

#include <activemq/transport/discovery/DiscoveryListener.h>
#include <activemq/transport/discovery/AbstractDiscoveryAgent.h>
#include <activemq/transport/discovery/DiscoveryAgentFactory.h>
#include <activemq/transport/discovery/DiscoveryAgentRegistry.h>

#include <decaf/net/URI.h>
#include <decaf/util/concurrent/CountDownLatch.h>

using namespace activemq;
using namespace activemq::transport;
using namespace activemq::transport::discovery;
using namespace decaf;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace decaf::net;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
namespace {

    class MockDiscoveryAgent : public AbstractDiscoveryAgent {
    private:

        bool reported;

    public:

        virtual ~MockDiscoveryAgent() {}

        virtual std::string toString() const { return "MockDiscoveryAgent"; }

    protected:

        virtual void doStart() {
            reported = false;
        }

        virtual void doStop() {}

        virtual void doAdvertizeSelf() {}

        virtual void doDiscovery() {
            try {
                if (!reported) {
                    Thread::sleep(1000);
                    processLiveService("dummy", "mock://localhost");
                    reported = true;
                } else {
                    Thread::sleep(500);
                }
            } catch (InterruptedException& ex) {
            }
        }

    };

    class MockDiscoveryAgentFactory : public DiscoveryAgentFactory {
    public:

        virtual ~MockDiscoveryAgentFactory() {}

        virtual decaf::lang::Pointer<DiscoveryAgent> createAgent(const decaf::net::URI& agentURI) {
            return Pointer<DiscoveryAgent>(new MockDiscoveryAgent);
        }

    };

    class MockDiscoveryListener : public DiscoveryListener {
    public:

        CountDownLatch* added;
        CountDownLatch* removed;

    public:

        MockDiscoveryListener(CountDownLatch* added, CountDownLatch* removed) :
            DiscoveryListener(), added(added), removed(removed) {}
        virtual ~MockDiscoveryListener() {}

        virtual void onServiceAdd(const activemq::commands::DiscoveryEvent* event) {
            added->countDown();
        }

        virtual void onServiceRemove(const activemq::commands::DiscoveryEvent* event) {
            removed->countDown();
        }
    };

}

////////////////////////////////////////////////////////////////////////////////
AbstractDiscoveryAgentTest::AbstractDiscoveryAgentTest() {
}

////////////////////////////////////////////////////////////////////////////////
AbstractDiscoveryAgentTest::~AbstractDiscoveryAgentTest() {
}

////////////////////////////////////////////////////////////////////////////////
void AbstractDiscoveryAgentTest::test() {

    CountDownLatch added(1);
    CountDownLatch removed(1);

    MockDiscoveryListener listener(&added, &removed);

    DiscoveryAgentRegistry& registry = DiscoveryAgentRegistry::getInstance();
    registry.registerFactory("mock", new MockDiscoveryAgentFactory);

    CPPUNIT_ASSERT_EQUAL(1, (int) registry.getAgentNames().size());

    DiscoveryAgentFactory* factory = registry.findFactory("mock");
    CPPUNIT_ASSERT(factory != NULL);

    Pointer<DiscoveryAgent> agent(factory->createAgent(URI("mock://default")));
    CPPUNIT_ASSERT(agent != NULL);

    agent->setDiscoveryListener(&listener);
    agent->start();

    CPPUNIT_ASSERT_MESSAGE("Should have discovered by now", added.await(60000));
    CPPUNIT_ASSERT_MESSAGE("Should have timed out by now", removed.await(60000));

    registry.unregisterAllFactories();
}
