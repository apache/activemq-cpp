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

#include "ActiveMQConnectionFactoryTest.h"

#include <cms/Connection.h>
#include <decaf/util/concurrent/Concurrent.h>
#include <decaf/util/concurrent/Mutex.h>
#include <decaf/lang/Thread.h>
#include <activemq/core/ActiveMQConnectionFactory.h>
#include <activemq/core/ActiveMQConnection.h>
#include <activemq/core/ActiveMQSession.h>
#include <activemq/core/ActiveMQProducer.h>
#include <activemq/commands/ActiveMQTextMessage.h>
#include <activemq/transport/TransportListener.h>
#include <memory>

using namespace std;
using namespace decaf::lang;
using namespace activemq;
using namespace activemq::core;
using namespace activemq::commands;
using namespace activemq::transport;

////////////////////////////////////////////////////////////////////////////////
namespace activemq{
namespace core{

    class MyTransportListener : public TransportListener {
    private:

        bool interrupted;
        bool resumed;

    public:

        MyTransportListener() : interrupted(false), resumed(true) {}

        bool isInterrupted() const {
            return this->interrupted;
        }

        bool isResumed() const {
            return this->resumed;
        }

        virtual void onCommand( const Pointer<Command> command ) {
        }

        virtual void onException( const decaf::lang::Exception& ex ) {
        }

        virtual void transportInterrupted() {
            this->interrupted = true;
        }

        virtual void transportResumed() {
            this->resumed = true;
        }
    };

}}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnectionFactoryTest::test1WithOpenWire() {

    try
    {
        std::string URI =
            "mock://127.0.0.1:23232?wireFormat=openwire";

        ActiveMQConnectionFactory connectionFactory( URI );

        cms::Connection* connection =
            connectionFactory.createConnection();

        CPPUNIT_ASSERT( connection != NULL );

        delete connection;

        return;
    }
    AMQ_CATCH_NOTHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_NOTHROW( )

    CPPUNIT_ASSERT( false );
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnectionFactoryTest::test2WithOpenWire()
{
    try
    {
        std::string URI = std::string() +
            "mock://127.0.0.1:23232?wireFormat=openwire&"
            "username=" + username + "&password=" + password +
            "&client-id=" + clientId;

        ActiveMQConnectionFactory connectionFactory( URI );

        std::auto_ptr<cms::Connection> connection(
            connectionFactory.createConnection() );
        CPPUNIT_ASSERT( connection.get() != NULL );

        ActiveMQConnection* amqConnection =
            dynamic_cast< ActiveMQConnection* >( connection.get() );
        CPPUNIT_ASSERT( amqConnection != NULL );
        CPPUNIT_ASSERT( username == amqConnection->getUsername() );
        CPPUNIT_ASSERT( password == amqConnection->getPassword() );
        CPPUNIT_ASSERT( clientId == amqConnection->getClientID() );

        return;
    }
    AMQ_CATCH_NOTHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_NOTHROW( )

    CPPUNIT_ASSERT( false );
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnectionFactoryTest::test3WithOpenWire()
{
    try
    {
        std::string URI = std::string() +
            "mock://127.0.0.1:23232";

        ActiveMQConnectionFactory connectionFactory( URI );

        std::auto_ptr<cms::Connection> connection(
            connectionFactory.createConnection(username, password) );
        CPPUNIT_ASSERT( connection.get() != NULL );

        connection->setClientID(clientId);

        ActiveMQConnection* amqConnection =
            dynamic_cast< ActiveMQConnection* >( connection.get() );
        CPPUNIT_ASSERT( amqConnection != NULL );
        CPPUNIT_ASSERT( username == amqConnection->getUsername() );
        CPPUNIT_ASSERT( password == amqConnection->getPassword() );
        CPPUNIT_ASSERT( clientId == amqConnection->getClientID() );

        return;
    }
    AMQ_CATCH_NOTHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_NOTHROW( )

    CPPUNIT_ASSERT( false );
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnectionFactoryTest::test4WithOpenWire()
{
    try
    {
        std::string URI = std::string() +
            "mock://127.0.0.1:23232";

        ActiveMQConnectionFactory connectionFactory( URI, username, password );

        std::auto_ptr<cms::Connection> connection(
            connectionFactory.createConnection() );
        CPPUNIT_ASSERT( connection.get() != NULL );

        connection->setClientID(clientId);

        ActiveMQConnection* amqConnection =
            dynamic_cast< ActiveMQConnection* >( connection.get() );
        CPPUNIT_ASSERT( amqConnection != NULL );
        CPPUNIT_ASSERT( username == amqConnection->getUsername() );
        CPPUNIT_ASSERT( password == amqConnection->getPassword() );
        CPPUNIT_ASSERT( clientId == amqConnection->getClientID() );

        return;
    }
    AMQ_CATCH_NOTHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_NOTHROW( )

    CPPUNIT_ASSERT( false );
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnectionFactoryTest::testExceptionWithPortOutOfRange() {
    try
    {
        std::string URI = "tcp://127.0.0.2:70000";

        ActiveMQConnectionFactory connectionFactory( URI );

        auto_ptr<cms::Connection> connection(
            connectionFactory.createConnection() );

        CPPUNIT_ASSERT( false );
    }
    AMQ_CATCH_NOTHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_NOTHROW( )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnectionFactoryTest::testExceptionOnCreate() {
    try
    {
        std::string URI =
            "tcp://127.0.0.2:23232?soConnectTimeout=3000";

        ActiveMQConnectionFactory connectionFactory( URI );

        auto_ptr<cms::Connection> connection(
            connectionFactory.createConnection() );

        CPPUNIT_ASSERT( false );
    }
    AMQ_CATCH_NOTHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_NOTHROW( )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnectionFactoryTest::testCreateWithURIOptions()
{
    try
    {
        std::string URI = std::string() +
            "mock://127.0.0.1:23232?connection.producerWindowSize=65536";

        ActiveMQConnectionFactory connectionFactory( URI );

        std::auto_ptr<cms::Connection> connection(
            connectionFactory.createConnection() );
        CPPUNIT_ASSERT( connection.get() != NULL );

        ActiveMQConnection* amqConnection =
            dynamic_cast< ActiveMQConnection* >( connection.get() );

        std::auto_ptr<ActiveMQSession> session( dynamic_cast<ActiveMQSession*>(
            amqConnection->createSession() ) );

        std::auto_ptr<ActiveMQProducer> producer( dynamic_cast<ActiveMQProducer*>(
            session->createProducer( NULL ) ) );

        CPPUNIT_ASSERT( producer->getProducerInfo()->getWindowSize() == 65536 );

        return;
    }
    AMQ_CATCH_NOTHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_NOTHROW( )

    CPPUNIT_ASSERT( false );
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnectionFactoryTest::testTransportListener() {

    std::string URI = "failover://(mock://localhost:61616?failOnSendMessage=true,"
                      "mock://localhost:61618)?randomize=false";

    MyTransportListener listener;

    ActiveMQConnectionFactory connectionFactory(URI);

    std::auto_ptr<cms::Connection> connection(
        connectionFactory.createConnection());
    CPPUNIT_ASSERT(connection.get() != NULL);

    ActiveMQConnection* amqConnection =
        dynamic_cast< ActiveMQConnection* >( connection.get() );

    amqConnection->addTransportListener(&listener);

    std::auto_ptr<ActiveMQSession> session(
        dynamic_cast<ActiveMQSession*> (amqConnection->createSession()));

    std::auto_ptr<cms::Destination> destination(session->createTopic("TEST"));

    std::auto_ptr<ActiveMQProducer> producer(
        dynamic_cast<ActiveMQProducer*> (session->createProducer(destination.get())));

    std::auto_ptr<cms::TextMessage> message(session->createTextMessage());
    producer->send(message.get());

    Thread::sleep(2000);

    CPPUNIT_ASSERT( listener.isInterrupted() );
    CPPUNIT_ASSERT( listener.isResumed() );
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnectionFactoryTest::testURIOptionsProcessing() {

    try
    {
        std::string URI =
            "mock://127.0.0.1:23232?connection.dispatchAsync=true&"
            "connection.alwaysSyncSend=true&connection.useAsyncSend=true&"
            "connection.useCompression=true&connection.compressionLevel=7&"
            "connection.closeTimeout=10000";

        ActiveMQConnectionFactory connectionFactory( URI );

        CPPUNIT_ASSERT( connectionFactory.isDispatchAsync() == true );
        CPPUNIT_ASSERT( connectionFactory.isAlwaysSyncSend() == true );
        CPPUNIT_ASSERT( connectionFactory.isUseAsyncSend() == true );
        CPPUNIT_ASSERT( connectionFactory.isUseCompression() == true );
        CPPUNIT_ASSERT( connectionFactory.getCloseTimeout() == 10000 );
        CPPUNIT_ASSERT( connectionFactory.getCompressionLevel() == 7 );

        cms::Connection* connection =
            connectionFactory.createConnection();

        CPPUNIT_ASSERT( connection != NULL );

        ActiveMQConnection* amqConnection = dynamic_cast<ActiveMQConnection*>( connection );

        CPPUNIT_ASSERT( amqConnection->isDispatchAsync() == true );
        CPPUNIT_ASSERT( amqConnection->isAlwaysSyncSend() == true );
        CPPUNIT_ASSERT( amqConnection->isUseAsyncSend() == true );
        CPPUNIT_ASSERT( amqConnection->isUseCompression() == true );
        CPPUNIT_ASSERT( amqConnection->getCloseTimeout() == 10000 );
        CPPUNIT_ASSERT( amqConnection->getCompressionLevel() == 7 );

        delete connection;

        return;
    }
    AMQ_CATCH_NOTHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_NOTHROW( )

    CPPUNIT_ASSERT( false );
}
