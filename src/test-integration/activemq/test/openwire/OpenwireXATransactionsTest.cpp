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

#include "OpenwireXATransactionsTest.h"

#include <activemq/core/ActiveMQXAConnectionFactory.h>
#include <activemq/core/ActiveMQXAConnection.h>
#include <activemq/core/ActiveMQXASession.h>
#include <activemq/exceptions/ActiveMQException.h>

#include <decaf/lang/Thread.h>

#include <cms/ConnectionFactory.h>
#include <cms/Connection.h>
#include <cms/Session.h>
#include <cms/MessageConsumer.h>
#include <cms/MessageProducer.h>
#include <cms/MessageListener.h>
#include <cms/XAConnectionFactory.h>
#include <cms/XAConnection.h>
#include <cms/Message.h>
#include <cms/TextMessage.h>

#include <memory>

using namespace cms;
using namespace std;
using namespace decaf;
using namespace decaf::lang;
using namespace activemq;
using namespace activemq::core;
using namespace activemq::test;
using namespace activemq::test::openwire;

////////////////////////////////////////////////////////////////////////////////
OpenwireXATransactionsTest::OpenwireXATransactionsTest() {
}

////////////////////////////////////////////////////////////////////////////////
OpenwireXATransactionsTest::~OpenwireXATransactionsTest() {
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireXATransactionsTest::testCreateXAConnectionFactory() {

    std::auto_ptr<XAConnectionFactory> factory(
        XAConnectionFactory::createCMSXAConnectionFactory( getBrokerURL() ) );
    CPPUNIT_ASSERT( factory.get() != NULL );

    ConnectionFactory* cmsFactory = dynamic_cast<ConnectionFactory*>( factory.get() );
    CPPUNIT_ASSERT( cmsFactory != NULL );

    ActiveMQConnectionFactory* amqFactory = dynamic_cast<ActiveMQConnectionFactory*>( factory.get() );
    CPPUNIT_ASSERT( amqFactory != NULL );

    ActiveMQXAConnectionFactory* amqXAFactory = dynamic_cast<ActiveMQXAConnectionFactory*>( factory.get() );
    CPPUNIT_ASSERT( amqXAFactory != NULL );
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireXATransactionsTest::testCreateXAConnection() {

    std::auto_ptr<XAConnectionFactory> factory(
        XAConnectionFactory::createCMSXAConnectionFactory( getBrokerURL() ) );
    CPPUNIT_ASSERT( factory.get() != NULL );

    std::auto_ptr<XAConnection> connection( factory->createXAConnection() );
    CPPUNIT_ASSERT( connection.get() != NULL );

    Connection* cmsConnection = dynamic_cast<Connection*>( connection.get() );
    CPPUNIT_ASSERT( cmsConnection != NULL );

    ActiveMQConnection* amqConnection = dynamic_cast<ActiveMQConnection*>( connection.get() );
    CPPUNIT_ASSERT( amqConnection != NULL );

    ActiveMQXAConnection* amqXAConnection = dynamic_cast<ActiveMQXAConnection*>( connection.get() );
    CPPUNIT_ASSERT( amqXAConnection != NULL );
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireXATransactionsTest::testCreateXASession() {

    std::auto_ptr<XAConnectionFactory> factory(
        XAConnectionFactory::createCMSXAConnectionFactory( getBrokerURL() ) );
    CPPUNIT_ASSERT( factory.get() != NULL );

    std::auto_ptr<XAConnection> connection( factory->createXAConnection() );
    CPPUNIT_ASSERT( connection.get() != NULL );

    std::auto_ptr<XASession> session( connection->createXASession() );
    CPPUNIT_ASSERT( session.get() != NULL );

    Session* cmsSession = dynamic_cast<Session*>( session.get() );
    CPPUNIT_ASSERT( cmsSession != NULL );

    ActiveMQSession* amqSession = dynamic_cast<ActiveMQSession*>( session.get() );
    CPPUNIT_ASSERT( amqSession != NULL );

    ActiveMQXASession* amqXASession = dynamic_cast<ActiveMQXASession*>( session.get() );
    CPPUNIT_ASSERT( amqXASession != NULL );
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireXATransactionsTest::testSendReceiveOutsideTX() {

    std::auto_ptr<XAConnectionFactory> factory(
        XAConnectionFactory::createCMSXAConnectionFactory( getBrokerURL() ) );
    CPPUNIT_ASSERT( factory.get() != NULL );

    std::auto_ptr<XAConnection> connection( factory->createXAConnection() );
    CPPUNIT_ASSERT( connection.get() != NULL );

    std::auto_ptr<XASession> session( connection->createXASession() );
    CPPUNIT_ASSERT( session.get() != NULL );

    std::auto_ptr<Destination> destination( session->createTemporaryQueue() );
    std::auto_ptr<MessageProducer> producer( session->createProducer( destination.get() ) );
    std::auto_ptr<MessageConsumer> consumer( session->createConsumer( destination.get() ) );

    connection->start();

    for( int i = 0; i < 50; ++i ) {
        std::auto_ptr<Message> message( session->createTextMessage( "TEST" ) );
        producer->send( message.get() );
    }

    for( int i = 0; i < 50; ++i ) {
        std::auto_ptr<Message> message( consumer->receive( 3000 ) );
        CPPUNIT_ASSERT( message.get() != NULL );
        CPPUNIT_ASSERT( dynamic_cast<TextMessage*>( message.get() ) != NULL );
    }
}
