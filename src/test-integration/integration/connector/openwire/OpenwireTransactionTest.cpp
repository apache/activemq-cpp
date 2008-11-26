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

#include "OpenwireTransactionTest.h"
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

#include <memory>

using namespace std;
using namespace cms;
using namespace activemq;
using namespace activemq::connector;
using namespace activemq::transport;
using namespace activemq::core;
using namespace activemq::util;
using namespace activemq::exceptions;
using namespace decaf::net;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace decaf::lang;

using namespace integration;
using namespace integration::connector;
using namespace integration::connector::openwire;

////////////////////////////////////////////////////////////////////////////////
OpenwireTransactionTest::OpenwireTransactionTest() {
}

////////////////////////////////////////////////////////////////////////////////
OpenwireTransactionTest::~OpenwireTransactionTest() {
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireTransactionTest::setUp() {
    testSupport = new TestSupport;
    testSupport->initialize( IntegrationCommon::getInstance().getOpenwireURL(),
                             cms::Session::SESSION_TRANSACTED);
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireTransactionTest::tearDown() {
    delete testSupport;
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireTransactionTest::testSendReceiveTransactedBatches() {

    try
    {
        // Create CMS Object for Comms, this one is owned by the TestSupport
        // class.
        cms::Session* session = testSupport->getSession();

        auto_ptr<cms::Topic> topic( session->createTopic("MYTOPIC") );
        auto_ptr<cms::MessageConsumer> consumer( session->createConsumer( topic.get() ) );
        auto_ptr<cms::MessageProducer> producer( session->createProducer( topic.get() ) );

        producer->setDeliveryMode( DeliveryMode::NON_PERSISTENT );

        auto_ptr<TextMessage> message( session->createTextMessage( "Batch Message" ) );

        for( int j = 0; j < batchCount; j++ ) {

            for( int i = 0; i < batchSize; i++ ) {
                producer->send( message.get() );
            }

            session->commit();

            for( int i = 0; i < batchSize; i++ ) {
                message.reset( dynamic_cast<TextMessage*>( consumer->receive( 1000 * 5 ) ) );

                CPPUNIT_ASSERT_MESSAGE(
                    "Failed to receive all messages in batch", message.get() != NULL );
                CPPUNIT_ASSERT( string("Batch Message") == message->getText() );
            }

            session->commit();
        }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireTransactionTest::testSendRollback() {

    try
    {
        // Create CMS Object for Comms, this one is owned by the TestSupport
        // class.
        cms::Session* session = testSupport->getSession();

        auto_ptr<cms::Topic> topic( session->createTopic("MYTOPIC") );
        auto_ptr<cms::MessageConsumer> consumer( session->createConsumer( topic.get() ) );
        auto_ptr<cms::MessageProducer> producer( session->createProducer( topic.get() ) );

        producer->setDeliveryMode( DeliveryMode::NON_PERSISTENT );

        auto_ptr<TextMessage> outbound1( session->createTextMessage( "First Message" ) );
        auto_ptr<TextMessage> outbound2( session->createTextMessage( "Second Message" ) );

        // sends a message
        producer->send( outbound1.get() );
        session->commit();

        // sends a message that gets rollbacked
        producer->send( session->createTextMessage( "I'm going to get rolled back." ) );
        session->rollback();

        // sends a message
        producer->send( outbound2.get() );
        session->commit();

        // receives the first message
        auto_ptr<TextMessage> inbound1(
            dynamic_cast<TextMessage*>( consumer->receive( 1500 ) ) );

        // receives the second message
        auto_ptr<TextMessage> inbound2(
            dynamic_cast<TextMessage*>( consumer->receive( 4000 ) ) );

        // validates that the rollbacked was not consumed
        session->commit();

        CPPUNIT_ASSERT( outbound1->getText() == inbound1->getText() );
        CPPUNIT_ASSERT( outbound2->getText() == inbound2->getText() );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}
