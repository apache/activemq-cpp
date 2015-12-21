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

#include "AdvisoryTest.h"

#include <activemq/core/ActiveMQConnectionFactory.h>
#include <activemq/core/ActiveMQConnection.h>
#include <activemq/core/ActiveMQSession.h>
#include <activemq/commands/ActiveMQTempTopic.h>
#include <activemq/commands/ActiveMQTempQueue.h>
#include <activemq/commands/ActiveMQMessage.h>
#include <activemq/commands/ConnectionInfo.h>
#include <activemq/commands/DestinationInfo.h>
#include <activemq/exceptions/ActiveMQException.h>
#include <activemq/util/CMSListener.h>
#include <activemq/util/IntegrationCommon.h>
#include <activemq/util/AdvisorySupport.h>

#include <decaf/lang/exceptions/ClassCastException.h>
#include <decaf/lang/Pointer.h>
#include <decaf/lang/Thread.h>
#include <decaf/lang/Runnable.h>
#include <decaf/util/concurrent/TimeUnit.h>
#include <decaf/util/UUID.h>

#include <cms/ConnectionFactory.h>
#include <cms/Connection.h>
#include <cms/Session.h>
#include <cms/MessageConsumer.h>
#include <cms/MessageProducer.h>
#include <cms/MessageListener.h>
#include <cms/ConnectionFactory.h>
#include <cms/Connection.h>
#include <cms/Message.h>
#include <cms/Destination.h>
#include <cms/TextMessage.h>

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
using namespace activemq::util;
using namespace activemq::commands;
using namespace activemq::exceptions;
using namespace activemq::test;

////////////////////////////////////////////////////////////////////////////////
AdvisoryTest::AdvisoryTest() {
}

////////////////////////////////////////////////////////////////////////////////
AdvisoryTest::~AdvisoryTest() {
}

////////////////////////////////////////////////////////////////////////////////
void AdvisoryTest::testTempDestinationCompositeAdvisoryTopic() {

    std::auto_ptr<ConnectionFactory> factory(
        ConnectionFactory::createCMSConnectionFactory(getBrokerURL()));
    CPPUNIT_ASSERT(factory.get() != NULL);

    std::auto_ptr<Connection> connection(factory->createConnection());
    CPPUNIT_ASSERT(connection.get() != NULL);

    std::auto_ptr<Session> session(connection->createSession());
    CPPUNIT_ASSERT(session.get() != NULL);

    std::auto_ptr<ActiveMQDestination> composite(
        AdvisorySupport::getTempDestinationCompositeAdvisoryTopic());

    std::auto_ptr<MessageConsumer> consumer(session->createConsumer(dynamic_cast<Topic*>(composite.get())));

    connection->start();

    // Create one of each
    std::auto_ptr<Topic> tempTopic(session->createTemporaryTopic());
    std::auto_ptr<Queue> tempQueue(session->createTemporaryQueue());

    // Create a consumer to ensure destination creation based on protocol.
    std::auto_ptr<MessageConsumer> tempTopicConsumer(session->createConsumer(tempTopic.get()));
    std::auto_ptr<MessageConsumer> tempQueueConsumer(session->createConsumer(tempQueue.get()));

    // Should be an advisory for each
    std::auto_ptr<cms::Message> advisory1(consumer->receive(2000));
    CPPUNIT_ASSERT(advisory1.get() != NULL);
    std::auto_ptr<cms::Message> advisory2(consumer->receive(2000));
    CPPUNIT_ASSERT(advisory2.get() != NULL);

    ActiveMQMessage* tempTopicAdvisory = dynamic_cast<ActiveMQMessage*>(advisory1.get());
    ActiveMQMessage* tempQueueAdvisory = dynamic_cast<ActiveMQMessage*>(advisory2.get());

    // Create one of each
    std::auto_ptr<Topic> topic(session->createTopic(UUID::randomUUID().toString()));
    std::auto_ptr<Queue> queue(session->createQueue(UUID::randomUUID().toString()));

    // Create a producer to ensure destination creation based on protocol.
    std::auto_ptr<MessageProducer> topicProducer(session->createProducer(topic.get()));
    std::auto_ptr<MessageProducer> queueProducer(session->createProducer(queue.get()));

    // Should not be an advisory for each
    std::auto_ptr<cms::Message> advisory3(consumer->receive(500));
    CPPUNIT_ASSERT(advisory3.get() == NULL);
    std::auto_ptr<cms::Message> advisory4(consumer->receive(500));
    CPPUNIT_ASSERT(advisory4.get() == NULL);

    connection->close();
}
