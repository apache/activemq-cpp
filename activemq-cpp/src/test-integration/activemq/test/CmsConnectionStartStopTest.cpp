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

#include "CmsConnectionStartStopTest.h"

#include <activemq/core/ActiveMQConnectionFactory.h>
#include <activemq/exceptions/ActiveMQException.h>

#include <decaf/lang/Runnable.h>
#include <decaf/lang/Pointer.h>
#include <decaf/lang/Integer.h>
#include <decaf/util/Random.h>
#include <decaf/util/concurrent/CopyOnWriteArrayList.h>
#include <decaf/util/concurrent/TimeUnit.h>
#include <decaf/util/concurrent/ThreadPoolExecutor.h>
#include <decaf/util/concurrent/LinkedBlockingQueue.h>

using namespace std;
using namespace cms;
using namespace activemq;
using namespace activemq::test;
using namespace activemq::core;
using namespace activemq::exceptions;

using namespace decaf;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace decaf::lang;

////////////////////////////////////////////////////////////////////////////////
CmsConnectionStartStopTest::CmsConnectionStartStopTest() :
    CMSTestFixture(), startedConnection(), stoppedConnection() {
}

////////////////////////////////////////////////////////////////////////////////
CmsConnectionStartStopTest::~CmsConnectionStartStopTest() {
}

////////////////////////////////////////////////////////////////////////////////
void CmsConnectionStartStopTest::setUp() {

    CMSTestFixture::setUp();

    Pointer<ActiveMQConnectionFactory> factory(new ActiveMQConnectionFactory(cmsProvider->getBrokerURL()));

    startedConnection.reset(factory->createConnection());
    startedConnection->start();
    stoppedConnection.reset(factory->createConnection());
}

////////////////////////////////////////////////////////////////////////////////
void CmsConnectionStartStopTest::tearDown() {

    startedConnection->close();
    stoppedConnection->close();

    startedConnection.reset(NULL);
    stoppedConnection.reset(NULL);

    CMSTestFixture::tearDown();
}

////////////////////////////////////////////////////////////////////////////////
void CmsConnectionStartStopTest::testStoppedConsumerHoldsMessagesTillStarted() {

    Pointer<Session> startedSession(startedConnection->createSession());
    Pointer<Session> stoppedSession(stoppedConnection->createSession());

    // Setup the consumers.
    Pointer<Topic> topic(startedSession->createTopic("test"));
    Pointer<MessageConsumer> startedConsumer(startedSession->createConsumer(topic.get()));
    Pointer<MessageConsumer> stoppedConsumer(stoppedSession->createConsumer(topic.get()));

    // Send the message.
    Pointer<MessageProducer> producer(startedSession->createProducer(topic.get()));
    Pointer<TextMessage> message(startedSession->createTextMessage("Hello"));
    producer->send(message.get());

    // Test the assertions.
    Pointer<Message> m(startedConsumer->receive(2000));
    CPPUNIT_ASSERT(m != NULL);

    m.reset(stoppedConsumer->receive(2000));
    CPPUNIT_ASSERT(m == NULL);

    stoppedConnection->start();
    m.reset(stoppedConsumer->receive(5000));
    CPPUNIT_ASSERT(m != NULL);

    startedSession->close();
    stoppedSession->close();
}

////////////////////////////////////////////////////////////////////////////////
void CmsConnectionStartStopTest::testMultipleConnectionStops() {

    testStoppedConsumerHoldsMessagesTillStarted();
    stoppedConnection->stop();
    testStoppedConsumerHoldsMessagesTillStarted();
    stoppedConnection->stop();
    testStoppedConsumerHoldsMessagesTillStarted();
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class CreateSessionRunnable : public Runnable {
    private:

        Connection* connection;
        Random rand;
        CopyOnWriteArrayList<std::string>* exceptions;

    private:

        CreateSessionRunnable(const CreateSessionRunnable&);
        CreateSessionRunnable& operator= (const CreateSessionRunnable&);

    public:

        CreateSessionRunnable(Connection* connection, CopyOnWriteArrayList<std::string>* exceptions) :
            Runnable(), connection(connection), rand(), exceptions(exceptions) {
        }

        virtual ~CreateSessionRunnable() {}

        virtual void run() {
            try {
                TimeUnit::MILLISECONDS.sleep(rand.nextInt(10));
                Pointer<Session>(connection->createSession());
            } catch (CMSException& e) {
                exceptions->add(e.getMessage());
            }
        }
    };

    class StartStopRunnable : public Runnable {
    private:

        Connection* connection;
        Random rand;
        CopyOnWriteArrayList<std::string>* exceptions;

    private:

        StartStopRunnable(const StartStopRunnable&);
        StartStopRunnable& operator= (const StartStopRunnable&);

    public:

        StartStopRunnable(Connection* connection, CopyOnWriteArrayList<std::string>* exceptions) :
            Runnable(), connection(connection), rand(), exceptions(exceptions) {
        }

        virtual ~StartStopRunnable() {}

        virtual void run() {
            try {
                TimeUnit::MILLISECONDS.sleep(rand.nextInt(10));
                connection->start();
                connection->stop();
            } catch (CMSException& e) {
                exceptions->add(e.getMessage());
            }

        }
    };

}

////////////////////////////////////////////////////////////////////////////////
void CmsConnectionStartStopTest::testConcurrentSessionCreateWithStart() {

    ThreadPoolExecutor executor(50, Integer::MAX_VALUE, 60LL, TimeUnit::SECONDS, new LinkedBlockingQueue<Runnable*>());

    CopyOnWriteArrayList<std::string> exceptions;
    Random rand;

    for (int i=0; i<2000; i++) {
        executor.execute(new CreateSessionRunnable(stoppedConnection.get(), &exceptions));
        executor.execute(new StartStopRunnable(stoppedConnection.get(), &exceptions));
    }

    executor.shutdown();
    CPPUNIT_ASSERT_MESSAGE("executor terminated", executor.awaitTermination(45, TimeUnit::SECONDS));
    CPPUNIT_ASSERT_MESSAGE("no exceptions: " + exceptions.toString(), exceptions.isEmpty());
}
