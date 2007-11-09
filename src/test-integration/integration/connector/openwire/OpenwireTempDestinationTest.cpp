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

#include "OpenwireTempDestinationTest.h"

#include <integration/IntegrationCommon.h>

#include <decaf/lang/Thread.h>
#include <decaf/util/concurrent/Mutex.h>
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
using namespace integration::connector;
using namespace integration::connector::openwire;

///////////////////////////////////////////////////////////////////////////////
OpenwireTempDestinationTest::OpenwireTempDestinationTest()
  : testSupport( IntegrationCommon::getInstance().getOpenwireURL() )
{
    testSupport.initialize();
}

///////////////////////////////////////////////////////////////////////////////
OpenwireTempDestinationTest::~OpenwireTempDestinationTest()
{}

///////////////////////////////////////////////////////////////////////////////
void OpenwireTempDestinationTest::test()
{
    try
    {
        if( IntegrationCommon::debug ) {
            cout << "Starting activemqcms Temp Destination test (sending "
                 << IntegrationCommon::defaultMsgCount
                 << " messages per type and sleeping "
                 << IntegrationCommon::defaultDelay
                << " milli-seconds) ...\n"
                << endl;
        }

        // Create CMS Object for Comms
        cms::Session* session = testSupport.getSession();

        cms::Destination* requestTopic = session->createTopic( UUID::randomUUID().toString() );
        cms::Destination* responseTopic = session->createTemporaryTopic();

        Consumer* requestConsumer = new Consumer( testSupport.getConnection(),
                                                  session,
                                                  requestTopic );
        Consumer* responseConsumer = new Consumer( testSupport.getConnection(),
                                                   session,
                                                   responseTopic );

        // Launch the Consumers in new Threads.
        Thread requestorThread( requestConsumer );
        Thread responderThread( responseConsumer );
        requestorThread.start();
        responderThread.start();
        Thread::sleep( 100 );

        cms::MessageProducer* producer =
            session->createProducer( requestTopic );

        // Send some bytes messages.
        testSupport.produceTextMessages(
            *producer, IntegrationCommon::defaultMsgCount, responseTopic );

        // Let the request consumer get all its messages
        waitForMessages( *requestConsumer,
                         IntegrationCommon::defaultMsgCount );

        // Check that we got them all.
        CPPUNIT_ASSERT( requestConsumer->getNumReceived() ==
                        IntegrationCommon::defaultMsgCount );

        // Let the response consumer get all its messages
        waitForMessages( *responseConsumer,
                         IntegrationCommon::defaultMsgCount );

        // Check that we got them all.
        CPPUNIT_ASSERT( responseConsumer->getNumReceived() ==
                        IntegrationCommon::defaultMsgCount );

        // Shutdown the Consumer Threads.
        requestConsumer->stop();
        responseConsumer->stop();
        requestorThread.join();
        responderThread.join();

        delete producer;
        delete requestConsumer;
        delete responseConsumer;

        delete requestTopic;
        delete responseTopic;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

///////////////////////////////////////////////////////////////////////////////
void OpenwireTempDestinationTest::waitForMessages(
    Consumer& consumer,
    unsigned int count )
{
    try
    {
        synchronized( &( consumer.getOnMsgMutex() ) )
        {
            unsigned int stopAtZero = count + 10;

            while( consumer.getNumReceived() < count )
            {
                consumer.getOnMsgMutex().wait( 500 );

                if( --stopAtZero == 0 )
                {
                    break;
                }
            }
        }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

///////////////////////////////////////////////////////////////////////////////
OpenwireTempDestinationTest::Consumer::Consumer(
    cms::Connection* connection,
    cms::Session* session,
    cms::Destination* destination )
{
    this->connection = connection;
    this->destination = destination;
    this->session = session;
    this->consumer = NULL;
    this->numReceived = 0;
}

///////////////////////////////////////////////////////////////////////////////
OpenwireTempDestinationTest::Consumer::~Consumer() {
    delete consumer;
}

///////////////////////////////////////////////////////////////////////////////
void OpenwireTempDestinationTest::Consumer::stop() {

    try {

        // singal the thread to quit
        synchronized( &mutex ) {
            mutex.notifyAll();
        }

    } catch( CMSException& e ) {
        e.printStackTrace();
    }
}

///////////////////////////////////////////////////////////////////////////////
void OpenwireTempDestinationTest::Consumer::run() {

    try {

        consumer = session->createConsumer( destination );

        consumer->setMessageListener( this );

        // Sleep while asynchronous messages come in.
        synchronized( &mutex ) {
            mutex.wait();
        }

    } catch( CMSException& e ) {
        e.printStackTrace();
    }
}

///////////////////////////////////////////////////////////////////////////////
void OpenwireTempDestinationTest::Consumer::onMessage(
    const Message* message ) {

    try
    {
        const cms::Destination* replyTo = message->getCMSReplyTo();

        if( replyTo != NULL ) {

            cms::MessageProducer* producer = session->createProducer( replyTo );
            cms::Message* response = session->createMessage();

            // Send it back to the replyTo Destination
            producer->send( response );

            delete response;
            delete producer;
        }

        numReceived++;

        // Signal anyone waiting on us getting new messages.
        synchronized( &onMsgMutex ){
            onMsgMutex.notifyAll();
        }

    } catch( CMSException& e ) {
        e.printStackTrace();
    }
}
