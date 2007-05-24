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

#include "OpenwireSlowListenerTest.h"
#include <integration/IntegrationCommon.h>

#include <activemq/concurrent/Thread.h>
#include <activemq/connector/stomp/StompConnector.h>
#include <activemq/util/Properties.h>
#include <activemq/transport/TransportFactory.h>
#include <activemq/util/Guid.h>
#include <activemq/util/Properties.h>
#include <activemq/util/StringTokenizer.h>
#include <activemq/connector/ConnectorFactoryMap.h>
#include <activemq/network/SocketFactory.h>
#include <activemq/transport/TransportFactory.h>
#include <activemq/network/Socket.h>
#include <activemq/exceptions/NullPointerException.h>
#include <activemq/core/ActiveMQConnectionFactory.h>
#include <activemq/core/ActiveMQConnection.h>
#include <activemq/core/ActiveMQConsumer.h>
#include <activemq/core/ActiveMQProducer.h>
#include <activemq/util/StringTokenizer.h>
#include <activemq/util/Boolean.h>
#include <activemq/util/Date.h>

#include <cms/Connection.h>
#include <cms/MessageConsumer.h>
#include <cms/MessageProducer.h>
#include <cms/MessageListener.h>
#include <cms/Startable.h>
#include <cms/Closeable.h>
#include <cms/MessageListener.h>
#include <cms/ExceptionListener.h>
#include <cms/Topic.h>
#include <cms/Queue.h>
#include <cms/TemporaryTopic.h>
#include <cms/TemporaryQueue.h>
#include <cms/Session.h>
#include <cms/BytesMessage.h>
#include <cms/TextMessage.h>
#include <cms/MapMessage.h>

using namespace activemq::connector::stomp;
using namespace activemq::transport;
using namespace activemq::util;
using namespace std;
using namespace cms;
using namespace activemq;
using namespace activemq::core;
using namespace activemq::util;
using namespace activemq::connector;
using namespace activemq::exceptions;
using namespace activemq::network;
using namespace activemq::transport;
using namespace activemq::concurrent;

using namespace integration;
using namespace integration::connector::openwire;

OpenwireSlowListenerTest::OpenwireSlowListenerTest()
{
}

OpenwireSlowListenerTest::~OpenwireSlowListenerTest()
{
}

void OpenwireSlowListenerTest::SlowListener::onMessage(const cms::Message* message) {

    synchronized( &threadIds ) {
        count++;
        threadIds.add( Thread::getId() );
    }

    Thread::sleep(10);

}

void OpenwireSlowListenerTest::test()
{
    try
    {
        SlowListener listener;

        ActiveMQConnectionFactory* connectionFactory;
        connectionFactory =
            new activemq::core::ActiveMQConnectionFactory(
                IntegrationCommon::getInstance().getOpenwireURL() );
        cms::Connection* connection = connectionFactory->createConnection();
        cms::Session* session = connection->createSession( cms::Session::AUTO_ACKNOWLEDGE );
        cms::Destination* destination = session->createTopic(Guid::createGUIDString());
        cms::MessageProducer* producer = session->createProducer( destination );

        const unsigned int numConsumers = 5;
        cms::MessageConsumer* consumers[numConsumers];

        // Create several consumers for the same destination.
        for (unsigned int i = 0; i < numConsumers; i++)
        {
            consumers[i] = session->createConsumer( destination );
            consumers[i]->setMessageListener(&listener);
        }

        connection->start();

        cms::BytesMessage* message = session->createBytesMessage();

        unsigned int msgCount = 50;
        for (unsigned int i = 0; i < msgCount; i++){
            producer->send(message);
        }
        delete message;

        // Wait no more than 10 seconds for all the messages to come in.
        waitForMessages( msgCount * numConsumers, 10000, listener );

        connection->close();

        synchronized( &listener.threadIds ) {

            // Make sure that the listenerwas always accessed by the same thread
            // and that it received all the messages from all consumers.
            CPPUNIT_ASSERT_EQUAL( 1, (int)listener.threadIds.size() );
            CPPUNIT_ASSERT_EQUAL( (msgCount * numConsumers), listener.count );
        }
        for (unsigned int i = 0; i < numConsumers; i++)
        {
            delete consumers[i];
        }

        delete destination;
        delete producer;
        delete session;
        delete connection;
        delete connectionFactory;
    } catch( ActiveMQException& ex ) {
        ex.printStackTrace();
        throw ex;
    }
}

void OpenwireSlowListenerTest::waitForMessages( unsigned int count,
    long long maxWaitTime,
    OpenwireSlowListenerTest::SlowListener& l )
{
    long long startTime = Date::getCurrentTimeMilliseconds();

    synchronized( &l.threadIds ) {

        while( l.count < count ) {

            long long curTime = Date::getCurrentTimeMilliseconds();
            if( (curTime - startTime) >= maxWaitTime ) {
                return;
            }

            l.threadIds.wait( 500 );
        }

    }
}



