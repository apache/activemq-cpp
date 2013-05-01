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
#include <activemq/commands/ConnectionControl.h>
#include <activemq/mock/MockBrokerService.h>
#include <decaf/lang/Pointer.h>
#include <decaf/lang/Thread.h>
#include <decaf/util/UUID.h>

using namespace activemq;
using namespace activemq::mock;
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

    Thread::sleep( 1000 );
    CPPUNIT_ASSERT( failover->isConnected() == true );

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

    transport->start();

    Thread::sleep( 1000 );
    CPPUNIT_ASSERT( failover->isConnected() == true );

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

    Thread::sleep( 1000 );

    CPPUNIT_ASSERT( listener.caughtException == true );
    CPPUNIT_ASSERT( failover->isConnected() == false );

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

    transport->start();

    Thread::sleep( 2000 );
    CPPUNIT_ASSERT( failover->isConnected() == true );

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

    Thread::sleep( 1000 );
    CPPUNIT_ASSERT( failover->isConnected() == true );

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

    Thread::sleep( 1000 );
    CPPUNIT_ASSERT( failover->isConnected() == true );

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

    Thread::sleep( 1000 );
    CPPUNIT_ASSERT( failover->isConnected() == true );

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

    Thread::sleep( 1000 );
    CPPUNIT_ASSERT( failover->isConnected() == true );

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
void FailoverTransportTest::testWithOpewireCommands() {

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

    Thread::sleep( 1000 );
    CPPUNIT_ASSERT( failover->isConnected() == true );

    Pointer<ConnectionInfo> connection = createConnection();
    transport->request( connection );
    Pointer<SessionInfo> session1 = createSession( connection );
    transport->request( session1 );
    Pointer<SessionInfo> session2 = createSession( connection );
    transport->request( session2 );
    Pointer<ConsumerInfo> consumer1 = createConsumer( session1 );
    transport->request( consumer1 );
    Pointer<ConsumerInfo> consumer2 = createConsumer( session1 );
    transport->request( consumer2 );
    Pointer<ConsumerInfo> consumer3 = createConsumer( session2 );
    transport->request( consumer3 );

    Pointer<ProducerInfo> producer1 = createProducer( session2 );
    transport->request( producer1 );

    // Remove the Producers
    this->disposeOf( producer1, transport );

    // Remove the Consumers
    this->disposeOf( consumer1, transport );
    this->disposeOf( consumer2, transport );
    this->disposeOf( consumer3, transport );

    // Remove the Session instances.
    this->disposeOf( session1, transport );
    this->disposeOf( session2, transport );

    // Indicate that we are done.
    Pointer<ShutdownInfo> shutdown( new ShutdownInfo() );
    transport->oneway( shutdown );

    transport->close();
}

////////////////////////////////////////////////////////////////////////////////
Pointer<ConnectionInfo> FailoverTransportTest::createConnection() {

    Pointer<ConnectionId> id( new ConnectionId() );
    id->setValue( UUID::randomUUID().toString() );

    Pointer<ConnectionInfo> info( new ConnectionInfo() );
    info->setClientId( UUID::randomUUID().toString() );
    info->setConnectionId( id );

    return info;
}

////////////////////////////////////////////////////////////////////////////////
Pointer<SessionInfo> FailoverTransportTest::createSession( const Pointer<ConnectionInfo>& parent ) {

    static int idx = 1;

    Pointer<SessionId> id( new SessionId() );
    id->setConnectionId( parent->getConnectionId()->getValue() );
    id->setValue( idx++ );

    Pointer<SessionInfo> info( new SessionInfo() );
    info->setSessionId( id );

    return info;
}

////////////////////////////////////////////////////////////////////////////////
Pointer<ConsumerInfo> FailoverTransportTest::createConsumer( const Pointer<SessionInfo>& parent ) {

    static int idx = 1;

    Pointer<ConsumerId> id( new ConsumerId() );
    id->setConnectionId( parent->getSessionId()->getConnectionId() );
    id->setSessionId( parent->getSessionId()->getValue() );
    id->setValue( idx++ );

    Pointer<ConsumerInfo> info( new ConsumerInfo() );
    info->setConsumerId( id );

    return info;
}

////////////////////////////////////////////////////////////////////////////////
Pointer<ProducerInfo> FailoverTransportTest::createProducer( const Pointer<SessionInfo>& parent ) {

    static int idx = 1;

    Pointer<ProducerId> id( new ProducerId() );
    id->setConnectionId( parent->getSessionId()->getConnectionId() );
    id->setSessionId( parent->getSessionId()->getValue() );
    id->setValue( idx++ );

    Pointer<ProducerInfo> info( new ProducerInfo() );
    info->setProducerId( id );

    return info;
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransportTest::disposeOf( const Pointer<SessionInfo>& session,
                                       Pointer<Transport>& transport ) {

    Pointer<RemoveInfo> command( new RemoveInfo() );
    command->setObjectId( session->getSessionId() );
    transport->oneway( command );
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransportTest::disposeOf( const Pointer<ConsumerInfo>& consumer,
                                       Pointer<Transport>& transport ) {

    Pointer<RemoveInfo> command( new RemoveInfo() );
    command->setObjectId( consumer->getConsumerId() );
    transport->oneway( command );
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransportTest::disposeOf( const Pointer<ProducerInfo>& producer,
                                       Pointer<Transport>& transport ) {

    Pointer<RemoveInfo> command( new RemoveInfo() );
    command->setObjectId( producer->getProducerId() );
    transport->oneway( command );
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransportTest::testTransportHandlesConnectionControl() {

    std::string uri =
        "failover://(mock://localhost:61618?failOnCreate=true,mock://localhost:61616)?randomize=false";

    std::string reconnectStr = "mock://localhost:61613?name=Reconnect";

    Pointer<ConnectionControl> control( new ConnectionControl() );
    control->setReconnectTo( reconnectStr );
    control->setRebalanceConnection( true );

    DefaultTransportListener listener;
    FailoverTransportFactory factory;

    Pointer<Transport> transport( factory.create( uri ) );
    CPPUNIT_ASSERT( transport != NULL );
    transport->setTransportListener( &listener );

    FailoverTransport* failover = dynamic_cast<FailoverTransport*>(
        transport->narrow( typeid( FailoverTransport ) ) );

    failover->setUpdateURIsSupported(true);

    CPPUNIT_ASSERT( failover != NULL );
    CPPUNIT_ASSERT( failover->isRandomize() == false );

    transport->start();

    Thread::sleep( 3000 );
    CPPUNIT_ASSERT( failover->isConnected() == true );

    MockTransport* mock = NULL;
    while( mock == NULL ) {
        Thread::sleep( 100 );
        mock = dynamic_cast<MockTransport*>( transport->narrow( typeid( MockTransport ) ) );
    }

    LinkedList<URI> removals;
    removals.add( URI("mock://localhost:61616") );

    mock->fireCommand( control );
    Thread::sleep( 2000 );
    failover->removeURI( true, removals );

    Thread::sleep( 20000 );

    mock = NULL;
    while( mock == NULL ) {
        Thread::sleep( 100 );
        mock = dynamic_cast<MockTransport*>( transport->narrow( typeid( MockTransport ) ) );
    }

    CPPUNIT_ASSERT_EQUAL(std::string("Reconnect"), mock->getName());
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransportTest::testPriorityBackupConfig() {

    std::string uri = "failover://(mock://localhost:61616,"
                                  "mock://localhost:61618)?randomize=false&priorityBackup=true";

    DefaultTransportListener listener;
    FailoverTransportFactory factory;

    Pointer<Transport> transport( factory.create( uri ) );
    CPPUNIT_ASSERT( transport != NULL );
    transport->setTransportListener( &listener );

    FailoverTransport* failover = dynamic_cast<FailoverTransport*>(
        transport->narrow( typeid( FailoverTransport ) ) );

    CPPUNIT_ASSERT( failover != NULL );
    CPPUNIT_ASSERT( failover->isRandomize() == false );
    CPPUNIT_ASSERT( failover->isPriorityBackup() == true );

    transport->start();

    Thread::sleep( 1000 );
    CPPUNIT_ASSERT( failover->isConnected() == true );
    CPPUNIT_ASSERT( failover->isConnectedToPriority() == true );

    transport->close();
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransportTest::testUriOptionsApplied() {

    std::string uri = "failover://(mock://localhost:61616,mock://localhost:61618)?"
            "randomize=true&"
            "priorityBackup=true&"
            "initialReconnectDelay=222&"
            "useExponentialBackOff=false&"
            "maxReconnectAttempts=27&"
            "startupMaxReconnectAttempts=44&"
            "backup=true&"
            "trackMessages=false&"
            "maxCacheSize=16543217&"
            "timeout=500&"
            "updateURIsSupported=false&"
            "maxReconnectDelay=55555&"
            "priorityURIs=mock://localhost:61617,mock://localhost:61619";

    DefaultTransportListener listener;
    FailoverTransportFactory factory;

    Pointer<Transport> transport( factory.create( uri ) );
    CPPUNIT_ASSERT( transport != NULL );
    transport->setTransportListener( &listener );

    FailoverTransport* failover = dynamic_cast<FailoverTransport*>(
        transport->narrow( typeid( FailoverTransport ) ) );

    CPPUNIT_ASSERT( failover != NULL );
    CPPUNIT_ASSERT( failover->isRandomize() == true );
    CPPUNIT_ASSERT( failover->isPriorityBackup() == true );
    CPPUNIT_ASSERT( failover->isUseExponentialBackOff() == false );
    CPPUNIT_ASSERT( failover->getInitialReconnectDelay() == 222 );
    CPPUNIT_ASSERT( failover->getMaxReconnectAttempts() == 27 );
    CPPUNIT_ASSERT( failover->getStartupMaxReconnectAttempts() == 44 );
    CPPUNIT_ASSERT( failover->isBackup() == true );
    CPPUNIT_ASSERT( failover->isTrackMessages() == false );
    CPPUNIT_ASSERT( failover->getMaxCacheSize() == 16543217 );
    CPPUNIT_ASSERT( failover->isUpdateURIsSupported() == false );
    CPPUNIT_ASSERT( failover->getMaxReconnectDelay() == 55555 );

    const List<URI>& priorityUris = failover->getPriorityURIs();
    CPPUNIT_ASSERT( priorityUris.size() == 2 );

    transport->close();
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransportTest::testConnectedToMockBroker() {

    MockBrokerService broker1(61616);
    MockBrokerService broker2(61618);

    broker1.start();
    broker1.waitUntilStarted();

    std::string uri = "failover://(tcp://localhost:61616,"
                                  "tcp://localhost:61618)?randomize=false";

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

    int count = 0;
    while (!failover->isConnected() && count++ < 20) {
        Thread::sleep( 200 );
    }
    CPPUNIT_ASSERT( failover->isConnected() == true );
    CPPUNIT_ASSERT( failover->isConnectedToPriority() == false );

    transport->close();

    broker1.stop();
    broker1.waitUntilStopped();
}
