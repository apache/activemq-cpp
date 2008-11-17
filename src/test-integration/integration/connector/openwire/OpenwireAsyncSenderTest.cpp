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

#include "OpenwireAsyncSenderTest.h"

#include <integration/IntegrationCommon.h>

#include <decaf/lang/Thread.h>
#include <activemq/connector/openwire/OpenWireConnector.h>
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

using namespace activemq::connector::openwire;
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

using namespace integration;
using namespace integration::connector::openwire;

////////////////////////////////////////////////////////////////////////////////
OpenwireAsyncSenderTest::OpenwireAsyncSenderTest() {
}

////////////////////////////////////////////////////////////////////////////////
OpenwireAsyncSenderTest::~OpenwireAsyncSenderTest() {
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireAsyncSenderTest::test1()
{
    try{
        std::string brokerURI =
            IntegrationCommon::getInstance().getOpenwireURL() +
            "&connection.useAsyncSend=true";
        ActiveMQConnectionFactory* connectionFactory =
            new ActiveMQConnectionFactory(brokerURI);
        cms::Connection* connection = connectionFactory->createConnection();

        ActiveMQConnection* amqConnection = dynamic_cast<ActiveMQConnection*>( connection );
        CPPUNIT_ASSERT( amqConnection != NULL );

        OpenWireConnector* connector =
            dynamic_cast<OpenWireConnector*>( amqConnection->getConnectionData()->getConnector() );
        CPPUNIT_ASSERT( amqConnection != NULL );

        CPPUNIT_ASSERT( connector->isUseAsyncSend() );
        CPPUNIT_ASSERT( !connector->isAlwaysSyncSend() );

        delete connectionFactory;
        connection->start();
        connection->stop();
        delete connection;
        CPPUNIT_ASSERT( true );
    } catch(...) {
        CPPUNIT_ASSERT( false );
    }
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireAsyncSenderTest::test2()
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
        cms::Session* session = testSupport->getSession();
        cms::Topic* topic = testSupport->getSession()->createTopic("mytopic");
        cms::MessageConsumer* consumer =
            session->createConsumer( topic );
        consumer->setMessageListener( testSupport );
        cms::MessageProducer* producer =
            session->createProducer( topic );
        producer->setDeliveryMode( DeliveryMode::NON_PERSISTENT );

        // Send some text messages
        testSupport->produceTextMessages(
            *producer, IntegrationCommon::defaultMsgCount );

        // Send some bytes messages.
        testSupport->produceBytesMessages(
            *producer, IntegrationCommon::defaultMsgCount );

        // Wait for the messages to get here
        testSupport->waitForMessages( IntegrationCommon::defaultMsgCount * 2 );

        unsigned int numReceived = testSupport->getNumReceived();
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
    AMQ_CATCHALL_THROW( ActiveMQException )
}
