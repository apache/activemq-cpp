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

#include "SimpleTest.h"
#include <integration/IntegrationCommon.h>

#include <decaf/lang/Thread.h>
#include <activemq/connector/stomp/StompConnector.h>
#include <decaf/util/Properties.h>
#include <activemq/transport/TransportFactory.h>
#include <decaf/util/UUID.h>
#include <decaf/util/Properties.h>
#include <decaf/util/StringTokenizer.h>
#include <activemq/connector/ConnectorFactoryMap.h>
#include <decaf/net/SocketFactory.h>
#include <activemq/transport/TransportFactory.h>
#include <decaf/net/Socket.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <activemq/core/ActiveMQConnectionFactory.h>
#include <activemq/core/ActiveMQConnection.h>
#include <activemq/core/ActiveMQConsumer.h>
#include <activemq/core/ActiveMQProducer.h>
#include <decaf/util/StringTokenizer.h>
#include <decaf/lang/Boolean.h>

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
using namespace decaf::net;
using namespace activemq::transport;
using namespace decaf::util::concurrent;
using namespace decaf::lang;
using namespace decaf::util;

using namespace integration;
using namespace integration::connector::stomp;

SimpleTest::SimpleTest()
{
}

SimpleTest::~SimpleTest()
{
}

void SimpleTest::testAutoAck()
{
    try
    {
        TestSupport testSupport( IntegrationCommon::getInstance().getStompURL() );
        testSupport.initialize();

        if( IntegrationCommon::debug ) {
            cout << "Starting activemqcms test (sending "
                 << IntegrationCommon::defaultMsgCount
                 << " messages per type and sleeping "
                 << IntegrationCommon::defaultDelay
                 << " milli-seconds) ...\n"
                 << endl;
        }

        // Create CMS Object for Comms
        cms::Session* session = testSupport.getSession();
        cms::Topic* topic = session->createTopic("mytopic");
        cms::MessageConsumer* consumer =
            session->createConsumer( topic );
        consumer->setMessageListener( &testSupport );
        cms::MessageProducer* producer =
            session->createProducer( topic );

        // Send some text messages
        testSupport.produceTextMessages(
            *producer, IntegrationCommon::defaultMsgCount );

        // Send some bytes messages.
        testSupport.produceTextMessages(
            *producer, IntegrationCommon::defaultMsgCount );

        // Wait for the messages to get here
        testSupport.waitForMessages( IntegrationCommon::defaultMsgCount * 2 );

        unsigned int numReceived = testSupport.getNumReceived();
        if( IntegrationCommon::debug ) {
            printf("received: %d\n", numReceived );
        }
        CPPUNIT_ASSERT(
            numReceived == IntegrationCommon::defaultMsgCount * 2 );

        if( IntegrationCommon::debug ) {
            printf("Shutting Down\n" );
        }
        delete producer;
        delete consumer;
        delete topic;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
}

void SimpleTest::testClientAck()
{
    try
    {
        TestSupport testSupport(IntegrationCommon::getInstance().getStompURL(), cms::Session::CLIENT_ACKNOWLEDGE );
        testSupport.initialize();

        if( IntegrationCommon::debug ) {
            cout << "Starting activemqcms test (sending "
                 << IntegrationCommon::defaultMsgCount
                 << " messages per type and sleeping "
                 << IntegrationCommon::defaultDelay
                 << " milli-seconds) ...\n"
                 << endl;
        }

        // Create CMS Object for Comms
        cms::Session* session = testSupport.getSession();
        cms::Topic* topic = session->createTopic("mytopic");
        cms::MessageConsumer* consumer =
            session->createConsumer( topic );
        consumer->setMessageListener( &testSupport );
        cms::MessageProducer* producer =
            session->createProducer( topic );

        // Send some text messages
        testSupport.produceTextMessages(
            *producer, IntegrationCommon::defaultMsgCount );

        // Send some bytes messages.
        testSupport.produceTextMessages(
            *producer, IntegrationCommon::defaultMsgCount );

        // Wait for the messages to get here
        testSupport.waitForMessages( IntegrationCommon::defaultMsgCount * 2 );

        unsigned int numReceived = testSupport.getNumReceived();
        if( IntegrationCommon::debug ) {
            printf("received: %d\n", numReceived );
        }
        CPPUNIT_ASSERT(
            numReceived == IntegrationCommon::defaultMsgCount * 2 );

        if( IntegrationCommon::debug ) {
            printf("Shutting Down\n" );
        }
        delete producer;
        delete consumer;
        delete topic;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
}

void SimpleTest::testProducerWithNullDestination()
{
    try
    {
        TestSupport testSupport(IntegrationCommon::getInstance().getStompURL(), cms::Session::CLIENT_ACKNOWLEDGE );
        testSupport.initialize();

        if( IntegrationCommon::debug ) {
            cout << "Starting activemqcms test (sending "
                 << IntegrationCommon::defaultMsgCount
                 << " messages per type and sleeping "
                 << IntegrationCommon::defaultDelay
                 << " milli-seconds) ...\n"
                 << endl;
        }

        // Create CMS Object for Comms
        cms::Session* session = testSupport.getSession();
        cms::Topic* topic = session->createTopic(UUID::randomUUID().toString());
        cms::MessageConsumer* consumer =  session->createConsumer( topic );
        consumer->setMessageListener( &testSupport );
        cms::MessageProducer* producer = session->createProducer( NULL );

        cms::TextMessage* textMsg = session->createTextMessage();

        // Send some text messages
        producer->send( topic, textMsg );

        delete textMsg;

        // Wait for the messages to get here
        testSupport.waitForMessages( 1 );

        unsigned int numReceived = testSupport.getNumReceived();
        if( IntegrationCommon::debug ) {
            printf("received: %d\n", numReceived );
        }
        CPPUNIT_ASSERT_EQUAL( 1, (int)numReceived );

        if( IntegrationCommon::debug ) {
            printf("Shutting Down\n" );
        }
        delete producer;
        delete consumer;
        delete topic;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
}

void SimpleTest::testSyncReceive()
{
    try
    {
        TestSupport testSupport(IntegrationCommon::getInstance().getStompURL(), cms::Session::AUTO_ACKNOWLEDGE );
        testSupport.initialize();

        if( IntegrationCommon::debug ) {
            cout << "Starting activemqcms test (sending "
                 << IntegrationCommon::defaultMsgCount
                 << " messages per type and sleeping "
                 << IntegrationCommon::defaultDelay
                 << " milli-seconds) ...\n"
                 << endl;
        }

        // Create CMS Object for Comms
        cms::Session* session = testSupport.getSession();
        cms::Topic* topic = session->createTopic(UUID::randomUUID().toString());
        cms::MessageConsumer* consumer = session->createConsumer( topic );
        cms::MessageProducer* producer = session->createProducer( topic );

        cms::TextMessage* textMsg = session->createTextMessage();

        // Send some text messages
        producer->send( textMsg );

        delete textMsg;

        cms::Message* message = consumer->receive(1000);
        CPPUNIT_ASSERT( message != NULL );
        delete message;

        if( IntegrationCommon::debug ) {
            printf("Shutting Down\n" );
        }
        delete producer;
        delete consumer;
        delete topic;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
}

void SimpleTest::testSyncReceiveClientAck()
{
    try
    {
        TestSupport testSupport(IntegrationCommon::getInstance().getStompURL(), cms::Session::CLIENT_ACKNOWLEDGE );
        testSupport.initialize();

        if( IntegrationCommon::debug ) {
            cout << "Starting activemqcms test (sending "
                 << IntegrationCommon::defaultMsgCount
                 << " messages per type and sleeping "
                 << IntegrationCommon::defaultDelay
                 << " milli-seconds) ...\n"
                 << endl;
        }

        // Create CMS Object for Comms
        cms::Session* session = testSupport.getSession();
        cms::Topic* topic = session->createTopic(UUID::randomUUID().toString());
        cms::MessageConsumer* consumer = session->createConsumer( topic );
        cms::MessageProducer* producer = session->createProducer( topic );

        cms::TextMessage* textMsg = session->createTextMessage();

        // Send some text messages
        producer->send( textMsg );

        delete textMsg;

        cms::Message* message = consumer->receive(1000);
        CPPUNIT_ASSERT( message != NULL );
        message->acknowledge();
        delete message;

        if( IntegrationCommon::debug ) {
            printf("Shutting Down\n" );
        }
        delete producer;
        delete consumer;
        delete topic;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
}

void SimpleTest::testMultipleConnections()
{
    try
    {

        if( IntegrationCommon::debug ) {
            cout << "Starting activemqcms test (sending "
                 << IntegrationCommon::defaultMsgCount
                 << " messages per type and sleeping "
                 << IntegrationCommon::defaultDelay
                 << " milli-seconds) ...\n"
                 << endl;
        }

        // Create CMS Object for Comms
        cms::ConnectionFactory* factory =
            new ActiveMQConnectionFactory(
                IntegrationCommon::getInstance().getStompURL() );
        cms::Connection* connection1 = factory->createConnection();
        connection1->start();

        cms::Connection* connection2 = factory->createConnection();
        connection2->start();

        CPPUNIT_ASSERT( connection1->getClientID() != connection2->getClientID() );

        cms::Session* session1 = connection1->createSession();
        cms::Session* session2 = connection2->createSession();

        cms::Topic* topic = session1->createTopic(UUID::randomUUID().toString());


        cms::MessageConsumer* consumer1 = session1->createConsumer( topic );
        cms::MessageConsumer* consumer2 = session2->createConsumer( topic );

        cms::MessageProducer* producer = session2->createProducer( topic );

        cms::TextMessage* textMsg = session2->createTextMessage();

        // Send some text messages
        producer->send( textMsg );

        delete textMsg;

        cms::Message* message = consumer1->receive(1000);
        CPPUNIT_ASSERT( message != NULL );
        delete message;

        message = consumer2->receive(1000);
        CPPUNIT_ASSERT( message != NULL );
        delete message;

        if( IntegrationCommon::debug ) {
            printf("Shutting Down\n" );
        }

        connection1->close();
        connection2->close();

        delete producer;
        delete consumer1;
        delete consumer2;
        delete topic;
        delete session1;
        delete session2;
        delete connection1;
        delete connection2;
        delete factory;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
}

void SimpleTest::testMultipleSessions()
{
    try
    {

        if( IntegrationCommon::debug ) {
            cout << "Starting activemqcms test (sending "
                 << IntegrationCommon::defaultMsgCount
                 << " messages per type and sleeping "
                 << IntegrationCommon::defaultDelay
                 << " milli-seconds) ...\n"
                 << endl;
        }

        // Create CMS Object for Comms
        cms::ConnectionFactory* factory =
            new ActiveMQConnectionFactory( IntegrationCommon::getInstance().getStompURL() );
        cms::Connection* connection = factory->createConnection();
        connection->start();

        cms::Session* session1 = connection->createSession();
        cms::Session* session2 = connection->createSession();

        cms::Topic* topic = session1->createTopic(UUID::randomUUID().toString());

        cms::MessageConsumer* consumer1 = session1->createConsumer( topic );
        cms::MessageConsumer* consumer2 = session2->createConsumer( topic );

        cms::MessageProducer* producer = session2->createProducer( topic );

        cms::TextMessage* textMsg = session2->createTextMessage();

        // Send some text messages
        producer->send( textMsg );

        delete textMsg;

        cms::Message* message = consumer1->receive(1000);
        CPPUNIT_ASSERT( message != NULL );
        delete message;

        message = consumer2->receive(1000);
        CPPUNIT_ASSERT( message != NULL );
        delete message;

        if( IntegrationCommon::debug ) {
            printf("Shutting Down\n" );
        }

        connection->close();

        delete producer;
        delete consumer1;
        delete consumer2;
        delete topic;
        delete session1;
        delete session2;
        delete connection;
        delete factory;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
}

void SimpleTest::testReceiveAlreadyInQueue() {

        try
    {

        if( IntegrationCommon::debug ) {
            cout << "Starting activemqcms test (sending "
                 << IntegrationCommon::defaultMsgCount
                 << " messages per type and sleeping "
                 << IntegrationCommon::defaultDelay
                 << " milli-seconds) ...\n"
                 << endl;
        }

        // Create CMS Object for Comms
        cms::ConnectionFactory* factory =
            new ActiveMQConnectionFactory(
                IntegrationCommon::getInstance().getStompURL() );
        cms::Connection* connection = factory->createConnection();

        cms::Session* session = connection->createSession();

        cms::Topic* topic = session->createTopic(UUID::randomUUID().toString());

        cms::MessageConsumer* consumer = session->createConsumer( topic );

        cms::MessageProducer* producer = session->createProducer( topic );

        cms::TextMessage* textMsg = session->createTextMessage();

        // Send some text messages
        producer->send( textMsg );

        delete textMsg;

        Thread::sleep( 100 );

        connection->start();

        cms::Message* message = consumer->receive(1000);
        CPPUNIT_ASSERT( message != NULL );
        delete message;

        if( IntegrationCommon::debug ) {
            printf("Shutting Down\n" );
        }

        connection->close();

        delete producer;
        delete consumer;
        delete topic;
        delete session;
        delete connection;
        delete factory;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
}
