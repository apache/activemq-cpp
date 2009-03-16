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

#include "FailoverTransportTest.h"

#include <activemq/transport/failover/FailoverTransportFactory.h>
#include <activemq/transport/failover/FailoverTransport.h>
#include <activemq/transport/mock/MockTransport.h>
#include <activemq/exceptions/ActiveMQException.h>
#include <activemq/commands/ActiveMQMessage.h>
#include <decaf/lang/Pointer.h>
#include <decaf/lang/Thread.h>

using namespace activemq;
using namespace activemq::commands;
using namespace activemq::transport;
using namespace activemq::transport::failover;
using namespace activemq::transport::mock;
using namespace activemq::exceptions;
using namespace decaf::io;
using namespace decaf::lang;
using namespace decaf::util;

////////////////////////////////////////////////////////////////////////////////
FailoverTransportTest::FailoverTransportTest() {
}

////////////////////////////////////////////////////////////////////////////////
FailoverTransportTest::~FailoverTransportTest() {
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransportTest::testTransportCreate() {

    std::string uri = "failover://(mock://localhost:61616)?randomize=false";

    DefaultTransportListener listener;
    FailoverTransportFactory factory;

    Pointer<Transport> transport( factory.create( uri ) );
    CPPUNIT_ASSERT( transport != NULL );
    transport->setTransportListener( &listener );

    FailoverTransport* failover = dynamic_cast<FailoverTransport*>(
        transport->narrow( typeid( FailoverTransport ) ) );

    CPPUNIT_ASSERT( failover != NULL );
    CPPUNIT_ASSERT( failover->isRandomize() == false );

    transport->start();
    transport->close();
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransportTest::testTransportCreateWithBackups() {

    std::string uri = "failover://(mock://localhost:61616,mock://localhost:61618)?randomize=false&backup=true";

    DefaultTransportListener listener;
    FailoverTransportFactory factory;

    Pointer<Transport> transport( factory.create( uri ) );
    CPPUNIT_ASSERT( transport != NULL );
    transport->setTransportListener( &listener );

    FailoverTransport* failover = dynamic_cast<FailoverTransport*>(
        transport->narrow( typeid( FailoverTransport ) ) );

    CPPUNIT_ASSERT( failover != NULL );
    CPPUNIT_ASSERT( failover->isRandomize() == false );
    CPPUNIT_ASSERT( failover->isBackup() == true );

    Thread::sleep( 1000 );

    transport->start();
    transport->close();
}

////////////////////////////////////////////////////////////////////////////////
class FailToConnectListener : public DefaultTransportListener {
public:

    bool caughtException;

    FailToConnectListener() : caughtException( false ) {}

    virtual void onException( const decaf::lang::Exception& ex AMQCPP_UNUSED ) {
        caughtException = true;
    }
};

////////////////////////////////////////////////////////////////////////////////
void FailoverTransportTest::testTransportCreateFailOnCreate() {

    std::string uri =
        "failover://(mock://localhost:61616?failOnCreate=true)?useExponentialBackOff=false&maxReconnectAttempts=3&initialReconnectDelay=100";

    FailToConnectListener listener;
    FailoverTransportFactory factory;

    Pointer<Transport> transport( factory.create( uri ) );
    CPPUNIT_ASSERT( transport != NULL );
    transport->setTransportListener( &listener );

    FailoverTransport* failover = dynamic_cast<FailoverTransport*>(
        transport->narrow( typeid( FailoverTransport ) ) );

    CPPUNIT_ASSERT( failover != NULL );
    CPPUNIT_ASSERT( failover->getMaxReconnectAttempts() == 3 );

    transport->start();

    Thread::sleep( 2000 );

    CPPUNIT_ASSERT( listener.caughtException == true );

    transport->close();
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransportTest::testTransportCreateFailOnCreateSendMessage() {

    std::string uri =
        "failover://(mock://localhost:61616?failOnCreate=true)?useExponentialBackOff=false&maxReconnectAttempts=3&initialReconnectDelay=100";

    Pointer<ActiveMQMessage> message( new ActiveMQMessage() );

    FailToConnectListener listener;
    FailoverTransportFactory factory;

    Pointer<Transport> transport( factory.create( uri ) );
    CPPUNIT_ASSERT( transport != NULL );
    transport->setTransportListener( &listener );

    FailoverTransport* failover = dynamic_cast<FailoverTransport*>(
        transport->narrow( typeid( FailoverTransport ) ) );

    CPPUNIT_ASSERT( failover != NULL );
    CPPUNIT_ASSERT( failover->getMaxReconnectAttempts() == 3 );

    transport->start();

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should Throw a IOException",
        transport->oneway( message ),
        IOException );

    CPPUNIT_ASSERT( listener.caughtException == true );

    transport->close();
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransportTest::testFailingBackupCreation() {

    std::string uri =
        "failover://(mock://localhost:61616,"
                    "mock://localhost:61618?failOnCreate=true)?randomize=false&backup=true";

    DefaultTransportListener listener;
    FailoverTransportFactory factory;

    Pointer<Transport> transport( factory.create( uri ) );
    CPPUNIT_ASSERT( transport != NULL );
    transport->setTransportListener( &listener );

    FailoverTransport* failover = dynamic_cast<FailoverTransport*>(
        transport->narrow( typeid( FailoverTransport ) ) );

    CPPUNIT_ASSERT( failover != NULL );
    CPPUNIT_ASSERT( failover->isRandomize() == false );
    CPPUNIT_ASSERT( failover->isBackup() == true );

    Thread::sleep( 2000 );

    transport->start();
    transport->close();
}

////////////////////////////////////////////////////////////////////////////////
class MessageCountingListener : public DefaultTransportListener {
public:

    int numMessages;

    MessageCountingListener() : numMessages( 0 ) {}

    virtual void onCommand( const Pointer<Command>& command AMQCPP_UNUSED ) {
        numMessages++;
    }
};

////////////////////////////////////////////////////////////////////////////////
void FailoverTransportTest::testSendOnewayMessage() {

    std::string uri = "failover://(mock://localhost:61616)?randomize=false";

    const int numMessages = 1000;
    Pointer<ActiveMQMessage> message( new ActiveMQMessage() );

    MessageCountingListener messageCounter;
    DefaultTransportListener listener;
    FailoverTransportFactory factory;

    Pointer<Transport> transport( factory.create( uri ) );
    CPPUNIT_ASSERT( transport != NULL );
    transport->setTransportListener( &listener );

    FailoverTransport* failover = dynamic_cast<FailoverTransport*>(
        transport->narrow( typeid( FailoverTransport ) ) );

    CPPUNIT_ASSERT( failover != NULL );
    CPPUNIT_ASSERT( failover->isRandomize() == false );

    transport->start();

    MockTransport* mock = NULL;
    while( mock == NULL ) {
        mock = dynamic_cast<MockTransport*>( transport->narrow( typeid( MockTransport ) ) );
    }
    mock->setOutgoingListener( &messageCounter );

    for( int i = 0; i < numMessages; ++i ) {
        transport->oneway( message );
    }

    Thread::sleep( 2000 );

    CPPUNIT_ASSERT( messageCounter.numMessages = numMessages );

    transport->close();
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransportTest::testSendRequestMessage() {

    std::string uri = "failover://(mock://localhost:61616)?randomize=false";

    Pointer<ActiveMQMessage> message( new ActiveMQMessage() );

    MessageCountingListener messageCounter;
    DefaultTransportListener listener;
    FailoverTransportFactory factory;

    Pointer<Transport> transport( factory.create( uri ) );
    CPPUNIT_ASSERT( transport != NULL );
    transport->setTransportListener( &listener );

    FailoverTransport* failover = dynamic_cast<FailoverTransport*>(
        transport->narrow( typeid( FailoverTransport ) ) );

    CPPUNIT_ASSERT( failover != NULL );
    CPPUNIT_ASSERT( failover->isRandomize() == false );

    transport->start();

    MockTransport* mock = NULL;
    while( mock == NULL ) {
        mock = dynamic_cast<MockTransport*>( transport->narrow( typeid( MockTransport ) ) );
    }
    mock->setOutgoingListener( &messageCounter );

    transport->request( message );
    transport->request( message );
    transport->request( message );
    transport->request( message );
    Thread::sleep( 1000 );

    CPPUNIT_ASSERT( messageCounter.numMessages = 4 );

    transport->close();
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransportTest::testSendOnewayMessageFail() {

    std::string uri =
        "failover://(mock://localhost:61616?failOnSendMessage=true,"
                    "mock://localhost:61618)?randomize=false";

    Pointer<ActiveMQMessage> message( new ActiveMQMessage() );

    MessageCountingListener messageCounter;
    DefaultTransportListener listener;
    FailoverTransportFactory factory;

    Pointer<Transport> transport( factory.create( uri ) );
    CPPUNIT_ASSERT( transport != NULL );
    transport->setTransportListener( &listener );

    FailoverTransport* failover = dynamic_cast<FailoverTransport*>(
        transport->narrow( typeid( FailoverTransport ) ) );

    CPPUNIT_ASSERT( failover != NULL );
    CPPUNIT_ASSERT( failover->isRandomize() == false );

    transport->start();

    MockTransport* mock = NULL;
    while( mock == NULL ) {
        mock = dynamic_cast<MockTransport*>( transport->narrow( typeid( MockTransport ) ) );
    }
    mock->setOutgoingListener( &messageCounter );

    transport->oneway( message );
    transport->oneway( message );
    transport->oneway( message );
    transport->oneway( message );
    Thread::sleep( 1000 );

    CPPUNIT_ASSERT( messageCounter.numMessages = 4 );

    transport->close();
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransportTest::testSendRequestMessageFail() {

    std::string uri =
        "failover://(mock://localhost:61616?failOnSendMessage=true,"
                    "mock://localhost:61618)?randomize=false";

    Pointer<ActiveMQMessage> message( new ActiveMQMessage() );

    MessageCountingListener messageCounter;
    DefaultTransportListener listener;
    FailoverTransportFactory factory;

    Pointer<Transport> transport( factory.create( uri ) );
    CPPUNIT_ASSERT( transport != NULL );
    transport->setTransportListener( &listener );

    FailoverTransport* failover = dynamic_cast<FailoverTransport*>(
        transport->narrow( typeid( FailoverTransport ) ) );

    CPPUNIT_ASSERT( failover != NULL );
    CPPUNIT_ASSERT( failover->isRandomize() == false );

    transport->start();

    MockTransport* mock = NULL;
    while( mock == NULL ) {
        mock = dynamic_cast<MockTransport*>( transport->narrow( typeid( MockTransport ) ) );
    }
    mock->setOutgoingListener( &messageCounter );

    transport->request( message );
    transport->request( message );
    transport->request( message );
    transport->request( message );
    Thread::sleep( 1000 );

    CPPUNIT_ASSERT( messageCounter.numMessages = 4 );

    transport->close();
}
