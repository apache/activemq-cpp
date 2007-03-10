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

CPPUNIT_TEST_SUITE_REGISTRATION( integration::connector::stomp::SimpleTest );

#include <activemq/concurrent/Thread.h>
#include <activemq/connector/stomp/StompConnector.h>
#include <activemq/util/SimpleProperties.h>
#include <activemq/transport/TransportFactory.h>
#include <activemq/util/Guid.h>
#include <activemq/util/SimpleProperties.h>
#include <activemq/util/StringTokenizer.h>
#include <activemq/connector/ConnectorFactoryMap.h>
#include <activemq/network/SocketFactory.h>
#include <activemq/transport/TransportFactory.h>
#include <activemq/network/Socket.h>
#include <activemq/exceptions/NullPointerException.h>
#include <activemq/core/ActiveMQConnection.h>
#include <activemq/core/ActiveMQConsumer.h>
#include <activemq/core/ActiveMQProducer.h>
#include <activemq/util/StringTokenizer.h>
#include <activemq/util/Boolean.h>

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
using namespace integration::connector::stomp;

SimpleTest::SimpleTest()
:
    testSupport( "stomp://localhost:61613" )
{
    testSupport.initialize();
}

SimpleTest::~SimpleTest()
{
}

void SimpleTest::test()
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
    AMQ_CATCHALL_THROW( ActiveMQException )
}

