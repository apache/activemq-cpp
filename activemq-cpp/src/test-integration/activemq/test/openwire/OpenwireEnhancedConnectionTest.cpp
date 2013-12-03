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

#include "OpenwireEnhancedConnectionTest.h"

#include <activemq/core/ActiveMQConnectionFactory.h>
#include <activemq/core/ActiveMQConnection.h>
#include <activemq/core/ActiveMQSession.h>
#include <activemq/exceptions/ActiveMQException.h>

#include <decaf/lang/Pointer.h>
#include <decaf/lang/Thread.h>
#include <decaf/lang/Thread.h>
#include <decaf/util/UUID.h>
#include <decaf/util/concurrent/TimeUnit.h>

#include <cms/ConnectionFactory.h>
#include <cms/Connection.h>
#include <cms/Session.h>
#include <cms/ConnectionFactory.h>
#include <cms/Connection.h>
#include <cms/DestinationListener.h>
#include <cms/DestinationSource.h>
#include <cms/EnhancedConnection.h>

#include <memory>

using namespace cms;
using namespace std;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace activemq;
using namespace activemq::core;
using namespace activemq::commands;
using namespace activemq::exceptions;
using namespace activemq::test;
using namespace activemq::test::openwire;

////////////////////////////////////////////////////////////////////////////////
OpenwireEnhancedConnectionTest::OpenwireEnhancedConnectionTest() {
}

////////////////////////////////////////////////////////////////////////////////
OpenwireEnhancedConnectionTest::~OpenwireEnhancedConnectionTest() {
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class TestDestinationListener : public DestinationListener {
    public:

        int queueCount;
        int topicCount;
        int tempQueueCount;
        int tempTopicCount;

        TestDestinationListener() : DestinationListener(),
                                    queueCount(0),
                                    topicCount(0),
                                    tempQueueCount(0),
                                    tempTopicCount(0) {
        }

        virtual void onDestinationEvent(cms::DestinationEvent* event) {

            cms::Destination::DestinationType type = event->getDestination()->getDestinationType();
            switch (type) {
                case cms::Destination::QUEUE:
                    if (event->isAddOperation()) {
                        queueCount++;
                    } else {
                        queueCount--;
                    }
                    break;
                case cms::Destination::TOPIC:
                    if (event->isAddOperation()) {
                        topicCount++;
                    } else {
                        topicCount--;
                    }
                    break;
                case cms::Destination::TEMPORARY_QUEUE:
                    if (event->isAddOperation()) {
                        tempQueueCount++;
                    } else {
                        tempQueueCount--;
                    }
                    break;
                case cms::Destination::TEMPORARY_TOPIC:
                    if (event->isAddOperation()) {
                        tempTopicCount++;
                    } else {
                        tempTopicCount--;
                    }
                    break;
                default:
                    break;
            }
        }

        void reset() {
            queueCount = 0;
            topicCount = 0;
            tempQueueCount = 0;
            tempTopicCount = 0;
        }
    };

}

////////////////////////////////////////////////////////////////////////////////
void OpenwireEnhancedConnectionTest::testDestinationSourceGetters() {

    TestDestinationListener listener;

    std::auto_ptr<ConnectionFactory> factory(
        ConnectionFactory::createCMSConnectionFactory( getBrokerURL() ) );
    CPPUNIT_ASSERT( factory.get() != NULL );

    std::auto_ptr<Connection> connection( factory->createConnection() );
    CPPUNIT_ASSERT( connection.get() != NULL );

    std::auto_ptr<Session> session( connection->createSession() );
    CPPUNIT_ASSERT( session.get() != NULL );

    ActiveMQConnection* amq = dynamic_cast<ActiveMQConnection*>(connection.get());
    CPPUNIT_ASSERT(amq != NULL);

    cms::EnhancedConnection* enhanced = dynamic_cast<cms::EnhancedConnection*>(connection.get());
    CPPUNIT_ASSERT(enhanced != NULL);

    std::auto_ptr<cms::DestinationSource> source(enhanced->getDestinationSource());
    CPPUNIT_ASSERT(source.get() != NULL);

    source->setListener(&listener);

    connection->start();
    source->start();

    TimeUnit::SECONDS.sleep(2);

    int currentQueueCount = listener.queueCount;
    int currentTopicCount = listener.topicCount;
    int currentTempQueueCount = listener.tempQueueCount;
    int currentTempTopicCount = listener.tempTopicCount;

    std::auto_ptr<Destination> destination1(session->createTopic(UUID::randomUUID().toString()));
    std::auto_ptr<MessageConsumer> consumer1(session->createConsumer(destination1.get()));
    std::auto_ptr<Destination> destination2(session->createQueue(UUID::randomUUID().toString()) );
    std::auto_ptr<MessageConsumer> consumer2(session->createConsumer(destination2.get()));

    consumer1->close();
    consumer2->close();

    std::auto_ptr<Destination> destination3( session->createTemporaryQueue() );
    std::auto_ptr<Destination> destination4( session->createTemporaryTopic() );

    TimeUnit::SECONDS.sleep(2);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Should be one Queue", currentQueueCount + 1, listener.queueCount);
    CPPUNIT_ASSERT_MESSAGE("Should be at least Topic", listener.topicCount > currentTopicCount);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Should be one temp Queue", currentTempQueueCount + 1, listener.tempQueueCount);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Should be one temp Topic", currentTempTopicCount + 1, listener.tempTopicCount);

    amq->destroyDestination(destination1.get());
    amq->destroyDestination(destination2.get());

    TimeUnit::SECONDS.sleep(2);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Should be no Queues created by this test",
                                 currentQueueCount, listener.queueCount);

    source->stop();

    std::auto_ptr<Destination> destination5( session->createTemporaryQueue() );
    std::auto_ptr<Destination> destination6( session->createTemporaryTopic() );

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Temp Queue Counts shouldn't change", currentTempQueueCount + 1, listener.tempQueueCount);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Temp Topic Counts shouldn't change", currentTempTopicCount + 1, listener.tempTopicCount);

    listener.reset();
    source->start();

    std::auto_ptr<Destination> destination7( session->createTemporaryQueue() );
    std::auto_ptr<Destination> destination8( session->createTemporaryTopic() );

    TimeUnit::SECONDS.sleep(2);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Should be three total temp Queues from this test",
                                 currentTempQueueCount + 3, listener.tempQueueCount);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Should be three total temp Topics from this test",
                                 currentTempTopicCount + 3, listener.tempTopicCount);

    source->stop();
    connection->close();
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireEnhancedConnectionTest::testDestinationSource() {

    TestDestinationListener listener;

    std::auto_ptr<ConnectionFactory> factory(
        ConnectionFactory::createCMSConnectionFactory( getBrokerURL() ) );
    CPPUNIT_ASSERT( factory.get() != NULL );

    std::auto_ptr<Connection> connection( factory->createConnection() );
    CPPUNIT_ASSERT( connection.get() != NULL );

    std::auto_ptr<Session> session( connection->createSession() );
    CPPUNIT_ASSERT( session.get() != NULL );

    ActiveMQConnection* amq = dynamic_cast<ActiveMQConnection*>(connection.get());
    CPPUNIT_ASSERT(amq != NULL);

    cms::EnhancedConnection* enhanced = dynamic_cast<cms::EnhancedConnection*>(connection.get());
    CPPUNIT_ASSERT(enhanced != NULL);

    std::auto_ptr<cms::DestinationSource> source(enhanced->getDestinationSource());
    CPPUNIT_ASSERT(source.get() != NULL);

    source->setListener(&listener);

    connection->start();
    source->start();

    TimeUnit::SECONDS.sleep(2);

    int currTempQueueCount = (int)source->getTemporaryQueues().size();
    int currTempTopicCount = (int)source->getTemporaryTopics().size();

    std::auto_ptr<Destination> destination1(session->createTemporaryQueue());
    std::auto_ptr<Destination> destination2(session->createTemporaryTopic());
    std::auto_ptr<Destination> destination3(session->createTemporaryQueue());
    std::auto_ptr<Destination> destination4(session->createTemporaryTopic());
    std::auto_ptr<Destination> destination5(session->createTemporaryQueue());
    std::auto_ptr<Destination> destination6(session->createTemporaryTopic());

    TimeUnit::SECONDS.sleep(2);

    std::vector<cms::TemporaryQueue*> tempQueues = source->getTemporaryQueues();
    std::vector<cms::TemporaryTopic*> tempTopics = source->getTemporaryTopics();

    CPPUNIT_ASSERT_EQUAL(currTempQueueCount + 3, (int)tempQueues.size());
    CPPUNIT_ASSERT_EQUAL(currTempTopicCount + 3, (int)tempTopics.size());

    for (int i = 0; i < 3; ++i) {
        delete tempQueues[i];
        delete tempTopics[i];
    }

    source->stop();
    connection->close();
}
