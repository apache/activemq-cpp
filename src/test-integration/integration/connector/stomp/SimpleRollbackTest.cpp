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

#include "SimpleRollbackTest.h"

#include <integration/IntegrationCommon.h>

#include <sstream>

#include <activemq/core/ActiveMQConnectionFactory.h>
#include <activemq/exceptions/ActiveMQException.h>
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
#include <activemq/core/ActiveMQConnection.h>
#include <activemq/core/ActiveMQConsumer.h>
#include <activemq/core/ActiveMQProducer.h>
#include <decaf/util/StringTokenizer.h>
#include <decaf/lang/Boolean.h>
#include <activemq/util/Config.h>

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
#include <cms/Session.h>

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

using namespace std;
using namespace integration;
using namespace integration::connector::stomp;

SimpleRollbackTest::SimpleRollbackTest()
{
    try
    {
        string url = IntegrationCommon::getInstance().getStompURL();
        numReceived = 0;

        // Default amount to send and receive
        msgCount = 1;

        // Create a Factory
        connectionFactory = new ActiveMQConnectionFactory( url );

        // Now create the connection
        connection = connectionFactory->createConnection();

        // Set ourself as a recipient of Exceptions
        connection->setExceptionListener( this );
        connection->start();

        // Create a Session
        session = connection->createSession(
            cms::Session::SESSION_TRANSACTED );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

SimpleRollbackTest::~SimpleRollbackTest()
{
    try
    {
        session->close();
        connection->close();

        delete session;
        delete connection;
        delete connectionFactory;
    }
    AMQ_CATCH_NOTHROW( ActiveMQException )
    AMQ_CATCHALL_NOTHROW( )
}

void SimpleRollbackTest::test()
{
    try
    {
        // Create CMS Object for Comms
        cms::Topic* topic = session->createTopic("mytopic");
        cms::MessageConsumer* consumer =
            session->createConsumer( topic );
        consumer->setMessageListener( this );
        cms::MessageProducer* producer =
            session->createProducer( topic );

        cms::TextMessage* textMsg =
            session->createTextMessage();

        for( size_t ix = 0; ix < msgCount; ++ix )
        {
            ostringstream lcStream;
            lcStream << "SimpleTest - Message #" << ix << ends;
            textMsg->setText( lcStream.str() );
            producer->send( textMsg );
        }

        delete textMsg;

        Thread::sleep( 100 );

        session->commit();

        textMsg = session->createTextMessage();

        for( size_t ix = 0; ix < msgCount; ++ix )
        {
            ostringstream lcStream;
            lcStream << "SimpleTest - Message #" << ix << ends;
            textMsg->setText( lcStream.str() );
            producer->send( textMsg );
        }

        delete textMsg;

        Thread::sleep( 500 );

        session->rollback();

        Thread::sleep( 500 );

        textMsg = session->createTextMessage();
        textMsg->setText( "SimpleTest - Message after Rollback" );
        producer->send( textMsg );
        delete textMsg;

        Thread::sleep( 15000 );

        CPPUNIT_ASSERT( true );

        textMsg = session->createTextMessage();
        textMsg->setText( "SimpleTest - Message after Rollback" );
        producer->send( textMsg );
        delete textMsg;

        if( IntegrationCommon::debug ) {
            printf( "Shutting Down\n" );
        }

        delete producer;
        delete consumer;
        delete topic;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

void SimpleRollbackTest::onException( const cms::CMSException& error AMQCPP_UNUSED)
{
    bool AbstractTester = false;
    CPPUNIT_ASSERT( AbstractTester );
}

void SimpleRollbackTest::onMessage( const cms::Message* message )
{
    try
    {
        // Got a text message.
        const cms::TextMessage* txtMsg =
            dynamic_cast<const cms::TextMessage*>(message);

        if( txtMsg != NULL )
        {
            if( IntegrationCommon::debug ) {
                printf("received text msg: %s\n", txtMsg->getText().c_str() );
            }

            numReceived++;

            // Signal that we got one
            synchronized( &mutex )
            {
                mutex.notifyAll();
            }

            return;
        }

        // Got a bytes msg.
        const cms::BytesMessage* bytesMsg =
            dynamic_cast<const cms::BytesMessage*>(message);

        if( bytesMsg != NULL )
        {
            const unsigned char* bytes = bytesMsg->getBodyBytes();

            string transcode( (const char*)bytes, bytesMsg->getBodyLength() );

            if( IntegrationCommon::debug ) {
                printf("Received Bytes Message: %s", transcode.c_str() );
            }

            numReceived++;

            // Signal that we got one
            synchronized( &mutex )
            {
                mutex.notifyAll();
            }

            return;
        }
    }
    AMQ_CATCH_NOTHROW( ActiveMQException )
    AMQ_CATCHALL_NOTHROW( )
}
