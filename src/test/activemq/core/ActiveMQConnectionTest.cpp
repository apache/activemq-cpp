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

#include "ActiveMQConnectionTest.h"

#include <decaf/util/Properties.h>
#include <decaf/util/concurrent/Concurrent.h>
#include <decaf/util/concurrent/Mutex.h>
#include <decaf/lang/Thread.h>
#include <decaf/lang/Pointer.h>
#include <activemq/core/ActiveMQConnectionFactory.h>
#include <activemq/core/ActiveMQConnection.h>
#include <activemq/transport/Transport.h>
#include <activemq/transport/DefaultTransportListener.h>
#include <activemq/transport/mock/MockTransport.h>
#include <activemq/transport/mock/MockTransportFactory.h>
#include <activemq/transport/TransportRegistry.h>
#include <activemq/util/Config.h>
#include <activemq/commands/Message.h>

#include <cms/Connection.h>
#include <cms/ExceptionListener.h>

using namespace activemq;
using namespace activemq::core;
using namespace activemq::transport;
using namespace decaf;
using namespace decaf::util;
using namespace decaf::lang;

namespace activemq {
namespace core{

    class MyCommandListener : public transport::DefaultTransportListener{
    public:

        commands::Command* cmd;

    public:

        MyCommandListener(){
            cmd = NULL;
        }
        virtual ~MyCommandListener(){}

        virtual void onCommand( commands::Command* command ){
            cmd = command;
        }
    };

    class MyExceptionListener : public cms::ExceptionListener{
    public:

        bool caughtOne;

    public:

        MyExceptionListener(){ caughtOne = false; }
        virtual ~MyExceptionListener(){}

        virtual void onException(const cms::CMSException& ex AMQCPP_UNUSED){
            caughtOne = true;
        }
    };

    class MyDispatcher : public Dispatcher
    {
    public:

        std::vector< decaf::lang::Pointer<commands::Message> > messages;

    public:
        virtual ~MyDispatcher(){}

        virtual void dispatch( const decaf::lang::Pointer<commands::MessageDispatch>& data )
            throw ( exceptions::ActiveMQException )
        {
            messages.push_back( data->getMessage() );
        }
    };
}}

////////////////////////////////////////////////////////////////////////////////
//void ActiveMQConnectionTest::test1WithStomp()
//{
//    try
//    {
//        MyMessageListener listener;
//        MyExceptionListener exListener;
//        MyCommandListener cmdListener;
//        MyDispatcher msgListener;
//        std::string connectionId = "testConnectionId";
//        decaf::util::Properties* properties =
//            new decaf::util::Properties();
//        transport::Transport* transport = NULL;
//
//        // Default to Stomp
//        properties->setProperty( "wireFormat", "stomp" );
//        decaf::net::URI uri( "mock://mock?wireFormat=stomp" );
//
//        transport::TransportFactory* factory =
//            transport::TransportRegistry::getInstance().findFactory( "mock" );
//        if( factory == NULL ){
//            CPPUNIT_ASSERT( false );
//        }
//
//        // Create the transport.
//        transport = factory->createComposite( uri );
//        if( transport == NULL ){
//            CPPUNIT_ASSERT( false );
//        }
//
//        transport::mock::MockTransport* dTransport =
//            dynamic_cast< transport::mock::MockTransport*>( transport );
//
//        CPPUNIT_ASSERT( dTransport != NULL );
//
//        dTransport->setCommandListener( &cmdListener );
//
//        connector::stomp::StompConnector* connector =
//            new connector::stomp::StompConnector(
//                transport, *properties );
//
//        connector->start();
//
//        ActiveMQConnection connection(
//            new ActiveMQConnectionData(
//                connector, transport, properties) );
//
//        // First - make sure exceptions are working.
//        connection.setExceptionListener( &exListener );
//        CPPUNIT_ASSERT( exListener.caughtOne == false );
//        dTransport->fireException( exceptions::ActiveMQException( __FILE__, __LINE__, "test" ) );
//        CPPUNIT_ASSERT( exListener.caughtOne == true );
//
//        cms::Session* session1 = connection.createSession();
//        cms::Session* session2 = connection.createSession();
//        cms::Session* session3 = connection.createSession();
//
//        CPPUNIT_ASSERT( session1 != NULL );
//        CPPUNIT_ASSERT( session2 != NULL );
//        CPPUNIT_ASSERT( session3 != NULL );
//
//        connector::stomp::StompSessionInfo session;
//        connector::stomp::StompConsumerInfo consumer;
//
//        session.setSessionId( 1 );
//        session.setConnectionId( "TEST:123" );
//        session.setAckMode( cms::Session::AUTO_ACKNOWLEDGE );
//
//        connector::stomp::StompTopic myTopic( "test" );
//        consumer.setConsumerId( 1 );
//        consumer.setSessionInfo( &session );
//        consumer.setDestination( &myTopic );
//
//        connection.addDispatcher( &consumer, &msgListener );
//
//        connector::stomp::commands::TextMessageCommand* cmd =
//            new connector::stomp::commands::TextMessageCommand;
//        connector::stomp::StompTopic topic1( "test" );
//        cmd->setCMSDestination( &topic1 );
//
//        connector::ConsumerMessageListener* consumerListener =
//            dynamic_cast< connector::ConsumerMessageListener* >(
//                &connection );
//
//        connection.start();
//
//        CPPUNIT_ASSERT( consumerListener != NULL );
//
//        consumerListener->onConsumerMessage( &consumer, cmd );
//
//        CPPUNIT_ASSERT_EQUAL( 1, (int)msgListener.messages.size() );
//
//        connection.removeDispatcher( &consumer );
//
//        msgListener.messages.clear();
//        consumerListener->onConsumerMessage( &consumer, cmd );
//
//        CPPUNIT_ASSERT_EQUAL( 0, (int)msgListener.messages.size() );
//
//        connection.addDispatcher( &consumer, &msgListener );
//
//        connection.stop();
//        consumerListener->onConsumerMessage( &consumer, cmd );
//        connection.start();
//        CPPUNIT_ASSERT_EQUAL( 1, (int)msgListener.messages.size() );
//
//        delete cmd;
//        cmd = new connector::stomp::commands::TextMessageCommand;
//
//        connector::stomp::StompTopic topic2( "test" );
//        cmd->setCMSDestination( &topic2 );
//
//        consumerListener->onConsumerMessage( &consumer, cmd );
//        CPPUNIT_ASSERT_EQUAL( 2, (int)msgListener.messages.size() );
//
//        connection.removeDispatcher( &consumer );
//        msgListener.messages.clear();
//
//        session1->close();
//        session2->close();
//        session3->close();
//        connection.close();
//
//        exListener.caughtOne = false;
//        consumerListener->onConsumerMessage( &consumer, cmd );
//        CPPUNIT_ASSERT( exListener.caughtOne == true );
//
//        delete cmd;
//
//        delete session1;
//        delete session2;
//        delete session3;
//
//    } catch( exceptions::ActiveMQException& ex ) {
//        ex.printStackTrace();
//        throw ex;
//    }
//}

////////////////////////////////////////////////////////////////////////////////
//void ActiveMQConnectionTest::test2WithStomp()
//{
//    try
//    {
//        MyMessageListener listener;
//        MyExceptionListener exListener;
//        MyCommandListener cmdListener;
//        MyDispatcher msgListener;
//        std::string connectionId = "testConnectionId";
//        decaf::util::Properties* properties =
//            new decaf::util::Properties();
//        transport::Transport* transport = NULL;
//
//        // Default to Stomp
//        properties->setProperty( "wireFormat", "stomp" );
//        decaf::net::URI uri( "mock://mock?wireFormat=stomp" );
//
//        transport::TransportFactory* factory =
//            transport::TransportRegistry::getInstance().findFactory( "mock" );
//        if( factory == NULL ){
//            CPPUNIT_ASSERT( false );
//        }
//
//        // Create the transport.
//        transport = factory->createComposite( uri );
//        if( transport == NULL ){
//            CPPUNIT_ASSERT( false );
//        }
//
//        transport::mock::MockTransport* dTransport =
//            dynamic_cast< transport::mock::MockTransport*>( transport );
//
//        CPPUNIT_ASSERT( dTransport != NULL );
//
//        dTransport->setCommandListener( &cmdListener );
//
//        connector::stomp::StompConnector* connector =
//            new connector::stomp::StompConnector(
//                transport, *properties );
//
//        connector->start();
//
//        ActiveMQConnection connection(
//            new ActiveMQConnectionData(
//                connector, transport, properties) );
//
//        connection.getClientID();
//        connection.close();
//
//        CPPUNIT_ASSERT( connection.getClientID() == "" );
//
//    } catch( exceptions::ActiveMQException& ex ) {
//        ex.printStackTrace();
//        throw ex;
//    }
//}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnectionTest::test2WithOpenwire()
{
    try
    {
        MyExceptionListener exListener;
        MyCommandListener cmdListener;
        MyDispatcher msgListener;
        std::string connectionId = "testConnectionId";
        Pointer<decaf::util::Properties> properties( new decaf::util::Properties() );
        Pointer<Transport> transport;

        // Default to Stomp
        properties->setProperty( "wireFormat", "openwire" );
        decaf::net::URI uri( "mock://mock?wireFormat=openwire" );

        transport::TransportFactory* factory =
            transport::TransportRegistry::getInstance().findFactory( "mock" );
        if( factory == NULL ){
            CPPUNIT_ASSERT( false );
        }

        // Create the transport.
        transport = factory->createComposite( uri );
        if( transport == NULL ){
            CPPUNIT_ASSERT( false );
        }

        transport->setTransportListener( &cmdListener );

        ActiveMQConnection connection( transport, properties );

        connection.getClientID();
        connection.close();

        CPPUNIT_ASSERT( connection.getClientID() == "" );

    } catch( exceptions::ActiveMQException& ex ) {
        ex.printStackTrace();
        throw ex;
    }
}
