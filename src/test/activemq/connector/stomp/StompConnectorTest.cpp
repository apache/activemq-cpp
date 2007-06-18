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

#include "StompConnectorTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION( activemq::connector::stomp::StompConnectorTest );

using namespace std;
using namespace activemq;
using namespace activemq::connector;
using namespace activemq::connector::stomp;

void StompConnectorTest::testSessions()
{
    std::string connectionId = "testConnectionId";
    StompResponseBuilder responseBuilder("testConnectionId");
    transport::MockTransport transport( &responseBuilder );
    util::Properties properties;

    // Using a pointer for the connector so we ensure the proper destruction
    // order of objects - connector before the transport.
    StompConnector* connector = new StompConnector( &transport, properties );

    connector->start();

    SessionInfo* info1 = connector->createSession( cms::Session::AUTO_ACKNOWLEDGE );
    CPPUNIT_ASSERT( info1->getAckMode() == cms::Session::AUTO_ACKNOWLEDGE );
    CPPUNIT_ASSERT( info1->getConnectionId() == connectionId );

    SessionInfo* info2 = connector->createSession( cms::Session::DUPS_OK_ACKNOWLEDGE );
    CPPUNIT_ASSERT( info2->getAckMode() == cms::Session::DUPS_OK_ACKNOWLEDGE );
    CPPUNIT_ASSERT( info2->getConnectionId() == connectionId );

    SessionInfo* info3 = connector->createSession( cms::Session::CLIENT_ACKNOWLEDGE );
    CPPUNIT_ASSERT( info3->getAckMode() == cms::Session::CLIENT_ACKNOWLEDGE );
    CPPUNIT_ASSERT( info3->getConnectionId() == connectionId );

    SessionInfo* info4 = connector->createSession( cms::Session::SESSION_TRANSACTED );
    CPPUNIT_ASSERT( info4->getAckMode() == cms::Session::SESSION_TRANSACTED );
    CPPUNIT_ASSERT( info4->getConnectionId() == connectionId );

    delete info1;
    delete info2;
    delete info3;
    delete info4;

    // Delete the connector here - this assures the propery order
    // of destruction.
    delete connector;
}

void StompConnectorTest::testConsumers()
{
    std::string connectionId = "testConnectionId";
    StompResponseBuilder responseBuilder("testConnectionId");
    transport::MockTransport transport( &responseBuilder );
    util::Properties properties;

    // Using a pointer for the connector so we ensure the proper destruction
    // order of objects - connector before the transport.
    StompConnector* connector = new StompConnector( &transport, properties );

    connector->start();

    SessionInfo* info1 = connector->createSession( cms::Session::AUTO_ACKNOWLEDGE );
    std::string sel1 = "";
    StompTopic dest1( "dummy.topic.1" );
    ConsumerInfo* cinfo1 = connector->createConsumer( &dest1, info1, sel1 );
    connector->startConsumer( cinfo1 );
    CPPUNIT_ASSERT( cinfo1->getSessionInfo() == info1 );
    CPPUNIT_ASSERT( cinfo1->getDestination()->toProviderString() == dest1.toProviderString() );
    CPPUNIT_ASSERT( cinfo1->getMessageSelector() == sel1 );

    SessionInfo* info2 = connector->createSession( cms::Session::DUPS_OK_ACKNOWLEDGE );
    std::string sel2 = "mysel2";
    StompTopic dest2( "dummy.topic.2" );
    ConsumerInfo* cinfo2 = connector->createConsumer( &dest2, info2, sel2 );
    connector->startConsumer( cinfo2 );
    CPPUNIT_ASSERT( cinfo2->getSessionInfo() == info2 );
    CPPUNIT_ASSERT( cinfo2->getDestination()->toProviderString() == dest2.toProviderString() );
    CPPUNIT_ASSERT( cinfo2->getMessageSelector() == sel2 );

    SessionInfo* info3 = connector->createSession( cms::Session::CLIENT_ACKNOWLEDGE );
    std::string sel3 = "mysel3";
    StompQueue dest3( "dummy.queue.1" );
    ConsumerInfo* cinfo3 = connector->createConsumer( &dest3, info3, sel3 );
    connector->startConsumer( cinfo3 );
    CPPUNIT_ASSERT( cinfo3->getSessionInfo() == info3 );
    CPPUNIT_ASSERT( cinfo3->getDestination()->toProviderString() == dest3.toProviderString() );
    CPPUNIT_ASSERT( cinfo3->getMessageSelector() == sel3 );

    SessionInfo* info4 = connector->createSession( cms::Session::SESSION_TRANSACTED );
    std::string sel4 = "";
    StompTopic dest4( "dummy.queue.2" );
    ConsumerInfo* cinfo4 = connector->createConsumer( &dest4, info4, sel4 );
    connector->startConsumer( cinfo4 );
    CPPUNIT_ASSERT( cinfo4->getSessionInfo() == info4 );
    CPPUNIT_ASSERT( cinfo4->getDestination()->toProviderString() == dest4.toProviderString() );
    CPPUNIT_ASSERT( cinfo4->getMessageSelector() == sel4 );

    delete cinfo1;
    delete cinfo2;
    delete cinfo3;
    delete cinfo4;

    delete info1;
    delete info2;
    delete info3;
    delete info4;

    // Delete the connector here - this assures the propery order
    // of destruction.
    delete connector;
}

void StompConnectorTest::testProducers()
{
    std::string connectionId = "testConnectionId";
    StompResponseBuilder responseBuilder("testConnectionId");
    transport::MockTransport transport( &responseBuilder );
    util::Properties properties;

    // Using a pointer for the connector so we ensure the proper destruction
    // order of objects - connector before the transport.
    StompConnector* connector = new StompConnector( &transport, properties );

    connector->start();

    SessionInfo* info1 = connector->createSession( cms::Session::AUTO_ACKNOWLEDGE );
    StompTopic dest1( "dummy.topic.1" );
    ProducerInfo* pinfo1 = connector->createProducer( &dest1, info1 );
    CPPUNIT_ASSERT( pinfo1->getSessionInfo() == info1 );
    CPPUNIT_ASSERT( pinfo1->getDestination()->toProviderString() == dest1.toProviderString() );

    SessionInfo* info2 = connector->createSession( cms::Session::DUPS_OK_ACKNOWLEDGE );
    StompTopic dest2( "dummy.topic.2" );
    ProducerInfo* pinfo2 = connector->createProducer( &dest2, info2 );
    CPPUNIT_ASSERT( pinfo2->getSessionInfo() == info2 );
    CPPUNIT_ASSERT( pinfo2->getDestination()->toProviderString() == dest2.toProviderString() );

    SessionInfo* info3 = connector->createSession( cms::Session::CLIENT_ACKNOWLEDGE );
    StompQueue dest3( "dummy.queue.1" );
    ProducerInfo* pinfo3 = connector->createProducer( &dest3, info3 );
    CPPUNIT_ASSERT( pinfo3->getSessionInfo() == info3 );
    CPPUNIT_ASSERT( pinfo3->getDestination()->toProviderString() == dest3.toProviderString() );

    SessionInfo* info4 = connector->createSession( cms::Session::SESSION_TRANSACTED );
    StompTopic dest4( "dummy.queue.2" );
    ProducerInfo* pinfo4 = connector->createProducer( &dest4, info4 );
    CPPUNIT_ASSERT( pinfo4->getSessionInfo() == info4 );
    CPPUNIT_ASSERT( pinfo4->getDestination()->toProviderString() == dest4.toProviderString() );

    delete pinfo1;
    delete pinfo2;
    delete pinfo3;
    delete pinfo4;

    delete info1;
    delete info2;
    delete info3;
    delete info4;

    // Delete the connector here - this assures the propery order
    // of destruction.
    delete connector;
}

void StompConnectorTest::testCommand()
{
    std::string connectionId = "testConnectionId";
    StompResponseBuilder responseBuilder("testConnectionId");
    transport::MockTransport transport( &responseBuilder );
    util::Properties properties;

    // Using a pointer for the connector so we ensure the proper destruction
    // order of objects - connector before the transport.
    StompConnector* connector = new StompConnector( &transport, properties );

    connector->start();

    StompTopic dest1( "dummy.topic" );
    StompTopic dest2( "dummy.topic2" );

    SessionInfo* info1 = connector->createSession( cms::Session::AUTO_ACKNOWLEDGE );
    ConsumerInfo* cinfo1 = connector->createConsumer( &dest1, info1, "" );
    connector->startConsumer( cinfo1 );

    SessionInfo* info2 = connector->createSession( cms::Session::DUPS_OK_ACKNOWLEDGE );
    ConsumerInfo* cinfo2 = connector->createConsumer( &dest1, info2, "" );
    connector->startConsumer( cinfo2 );

    SessionInfo* info3 = connector->createSession( cms::Session::CLIENT_ACKNOWLEDGE );
    ConsumerInfo* cinfo3 = connector->createConsumer( &dest2, info3, "" );
    connector->startConsumer( cinfo3 );

    SessionInfo* info4 = connector->createSession( cms::Session::SESSION_TRANSACTED );
    ConsumerInfo* cinfo4 = connector->createConsumer( &dest2, info4, "" );
    connector->startConsumer( cinfo4 );

    MyMessageListener listener;
    connector->setConsumerMessageListener( &listener );

    StompFrame* frame = new StompFrame();
    frame->setCommand( "MESSAGE" );
    frame->getProperties().setProperty(
        "destination", dest1.toProviderString() );
    const char* buffer = "hello world";
    frame->setBody( (const unsigned char*)buffer, 12 );

    commands::TextMessageCommand* msg =
        new commands::TextMessageCommand( frame ); // deleted by listener
    transport.fireCommand( msg );

    CPPUNIT_ASSERT( listener.consumers.size() == 2 );
    for( unsigned int ix=0; ix<listener.consumers.size(); ++ix ){
        CPPUNIT_ASSERT( listener.consumers[ix] == cinfo1 ||
            listener.consumers[ix] == cinfo2 );
    }

    // Clean up the consumers list
    listener.consumers.clear();

    frame = new StompFrame();
    frame->setCommand( "MESSAGE" );
    frame->getProperties().setProperty(
        "destination", dest2.toProviderString() );
    buffer = "hello world";
    frame->setBody( (unsigned char*)buffer, 12 );

    msg = new commands::TextMessageCommand( frame ); // deleted by listener
    transport.fireCommand( msg );

    CPPUNIT_ASSERT( listener.consumers.size() == 2 );
    for( unsigned int ix=0; ix<listener.consumers.size(); ++ix ){
        CPPUNIT_ASSERT( listener.consumers[ix] == cinfo3 ||
            listener.consumers[ix] == cinfo4 );
    }

    delete cinfo1;
    delete cinfo2;
    delete cinfo3;
    delete cinfo4;

    delete info1;
    delete info2;
    delete info3;
    delete info4;

    // Delete the connector here - this assures the propery order
    // of destruction.
    delete connector;
}

void StompConnectorTest::testSendingCommands()
{
    std::string connectionId = "testConnectionId";
    StompResponseBuilder responseBuilder("testConnectionId");
    transport::MockTransport transport( &responseBuilder );
    util::Properties properties;
    StompConnector* connector =
        new StompConnector( &transport, properties );
    connector->start();

    StompTopic dest1( "dummy.topic.1" );

    MyCommandListener cmdListener;
    transport.setOutgoingCommandListener( &cmdListener );

    SessionInfo* info1 = connector->createSession( cms::Session::AUTO_ACKNOWLEDGE );
    ConsumerInfo* cinfo1 = connector->createConsumer( &dest1, info1, "" );
    connector->startConsumer( cinfo1 );
    CPPUNIT_ASSERT( cmdListener.cmd != NULL );

    cmdListener.cmd = NULL;

    SessionInfo* info2 = connector->createSession( cms::Session::DUPS_OK_ACKNOWLEDGE );
    ConsumerInfo* cinfo2 = connector->createConsumer( &dest1, info2, "" );
    connector->startConsumer(cinfo2);
    CPPUNIT_ASSERT( cmdListener.cmd == NULL );

    cmdListener.cmd = NULL;

    delete cinfo1;
    CPPUNIT_ASSERT( cmdListener.cmd == NULL );

    cmdListener.cmd = NULL;

    delete cinfo2;
    CPPUNIT_ASSERT( cmdListener.cmd != NULL );

    delete info1;
    delete info2;

    delete connector;
    CPPUNIT_ASSERT( cmdListener.cmd != NULL );
}

void StompConnectorTest::testException()
{
    std::string connectionId = "testConnectionId";
    StompResponseBuilder responseBuilder("testConnectionId");
    transport::MockTransport transport( &responseBuilder );
    MyExceptionListener exListener;
    util::Properties properties;

    // Using a pointer for the connector so we ensure the proper destruction
    // order of objects - connector before the transport.
    StompConnector* connector = new StompConnector( &transport, properties );

    connector->setExceptionListener(&exListener);

    connector->start();

    // Initiate an exception from the transport.
    transport.fireException( exceptions::ActiveMQException(__FILE__, __LINE__, "test") );

    CPPUNIT_ASSERT( exListener.num == 1 );

    // Delete the connector here - this assures the propery order
    // of destruction.
    delete connector;
}
