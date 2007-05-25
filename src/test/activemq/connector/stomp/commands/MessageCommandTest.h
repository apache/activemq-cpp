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

#ifndef _ACTIVEMQ_CONNECTOR_STOMP_COMMANDS_MESSAGECOMMANDTEST_H_
#define _ACTIVEMQ_CONNECTOR_STOMP_COMMANDS_MESSAGECOMMANDTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <activemq/connector/stomp/commands/MessageCommand.h>
#include <activemq/core/ActiveMQAckHandler.h>
#include <activemq/connector/stomp/StompTopic.h>
#include <activemq/util/Config.h>
#include <cms/Message.h>

namespace activemq{
namespace connector{
namespace stomp{
namespace commands{

    class MessageCommandTest : public CppUnit::TestFixture
    {
        CPPUNIT_TEST_SUITE( MessageCommandTest );
        CPPUNIT_TEST( test );
        CPPUNIT_TEST( testSetProperties );
        CPPUNIT_TEST( testFailedGetProperties );
        CPPUNIT_TEST( testGetPropertyNames );
        CPPUNIT_TEST( testClearProperties );
        CPPUNIT_TEST_SUITE_END();

    protected:

        class TestAckHandler : public core::ActiveMQAckHandler
        {
        public:

            TestAckHandler(void) { wasAcked = false; }
            virtual ~TestAckHandler(void) {}

            virtual void acknowledgeMessage( const core::ActiveMQMessage* message AMQCPP_UNUSED)
                throw ( cms::CMSException )
            {
                wasAcked = true;
            }

        public:

            bool wasAcked;

        };

    public:

        MessageCommandTest() {}
        virtual ~MessageCommandTest() {}

        void test(void)
        {
            TestAckHandler ackHandler;
            MessageCommand cmd;

            CPPUNIT_ASSERT( cmd.getStompCommandId() ==
                            CommandConstants::SEND );

            CPPUNIT_ASSERT( cmd.isResponseRequired() == false );
            cmd.setResponseRequired( true );
            cmd.setCommandId( 123 );
            CPPUNIT_ASSERT( cmd.isResponseRequired() == false );
            CPPUNIT_ASSERT( cmd.getCommandId() == 0 );
            cmd.setCorrelationId( 99 );
            CPPUNIT_ASSERT( cmd.getCorrelationId() == 99 );
            CPPUNIT_ASSERT( cmd.getTransactionId() == "" );
            cmd.setTransactionId( "ID:123456" );
            CPPUNIT_ASSERT( std::string( cmd.getTransactionId() ) ==
                            "ID:123456" );
            StompTopic topic("testTopic");
            cmd.setCMSDestination( &topic );

            StompFrame* frame = cmd.marshal().clone();

            CPPUNIT_ASSERT( frame != NULL );

            MessageCommand cmd1( frame );

            CPPUNIT_ASSERT( cmd.getCommandId() == cmd1.getCommandId() );
            CPPUNIT_ASSERT( cmd.getStompCommandId() == CommandConstants::SEND );
            CPPUNIT_ASSERT( cmd.isResponseRequired() == cmd1.isResponseRequired() );
            CPPUNIT_ASSERT( cmd.getCorrelationId() == cmd1.getCorrelationId() );
            CPPUNIT_ASSERT( std::string(cmd.getTransactionId()) == cmd1.getTransactionId() );

            cmd.setAckHandler( &ackHandler );
            cmd.acknowledge();
            CPPUNIT_ASSERT( ackHandler.wasAcked == true );

            CPPUNIT_ASSERT(
                cmd.getProperties().hasProperty( "test" ) == false );
            cmd.getProperties().setProperty( "test", "value" );
            CPPUNIT_ASSERT(
                cmd.getProperties().hasProperty( "test" ) == true );
            CPPUNIT_ASSERT(
                std::string( cmd.getProperties().getProperty( "test" ) ) ==
                "value" );

            CPPUNIT_ASSERT( cmd.getCMSCorrelationID() == "" );
            cmd.setCMSCorrelationID( "ID:1234567" );
            CPPUNIT_ASSERT( std::string( cmd.getCMSCorrelationID() ) ==
                            "ID:1234567" );
            CPPUNIT_ASSERT( cmd.getCMSDeliveryMode() ==
                            cms::DeliveryMode::PERSISTENT );
            cmd.setCMSDeliveryMode( cms::DeliveryMode::NON_PERSISTENT );
            CPPUNIT_ASSERT( cmd.getCMSDeliveryMode() ==
                            cms::DeliveryMode::NON_PERSISTENT );
            CPPUNIT_ASSERT( cmd.getCMSDestination()->toProviderString() ==
                            "/topic/testTopic" );
            CPPUNIT_ASSERT( cmd.getCMSExpiration() == 0 );
            cmd.setCMSExpiration( 123 );
            CPPUNIT_ASSERT( cmd.getCMSExpiration() == 123 );
            CPPUNIT_ASSERT( cmd.getCMSMessageID() == "" );
            cmd.setCMSMessageID( "ID:1234567" );
            CPPUNIT_ASSERT( std::string( cmd.getCMSMessageID() ) ==
                            "ID:1234567" );
            CPPUNIT_ASSERT( cmd.getCMSPriority() == 0 );
            cmd.setCMSPriority( 5 );
            CPPUNIT_ASSERT( cmd.getCMSPriority() == 5 );
            CPPUNIT_ASSERT( cmd.getCMSRedelivered() == false );
            cmd.setCMSRedelivered( true );
            CPPUNIT_ASSERT( cmd.getCMSRedelivered() == true );
            CPPUNIT_ASSERT( cmd.getCMSReplyTo() == NULL );
            cmd.setCMSReplyTo( &topic );
            CPPUNIT_ASSERT( cmd.getCMSReplyTo()->toProviderString() ==
                            "/topic/testTopic" );
            CPPUNIT_ASSERT( cmd.getCMSTimestamp() == 0 );
            cmd.setCMSTimestamp( 123 );
            CPPUNIT_ASSERT( cmd.getCMSTimestamp() == 123 );
            CPPUNIT_ASSERT( cmd.getCMSType() == "" );
            cmd.setCMSType( "test" );
            CPPUNIT_ASSERT( std::string( cmd.getCMSType() ) ==
                            "test" );
            CPPUNIT_ASSERT( cmd.getRedeliveryCount() == 0 );
            cmd.setRedeliveryCount( 123 );
            CPPUNIT_ASSERT( cmd.getRedeliveryCount() == 123 );

            cms::Message* cmd2 = cmd.clone();

            CPPUNIT_ASSERT( cmd.getCMSPriority() == cmd2->getCMSPriority() );
            CPPUNIT_ASSERT( cmd.getCMSTimestamp() == cmd2->getCMSTimestamp() );
            CPPUNIT_ASSERT( cmd.getCMSExpiration() == cmd2->getCMSExpiration() );
            CPPUNIT_ASSERT( cmd.getCMSDeliveryMode() == cmd2->getCMSDeliveryMode() );
            CPPUNIT_ASSERT( std::string(cmd.getCMSCorrelationID()) == cmd2->getCMSCorrelationID() );
            CPPUNIT_ASSERT( cmd.getCMSReplyTo()->toProviderString() == cmd2->getCMSReplyTo()->toProviderString() );
            CPPUNIT_ASSERT( std::string(cmd.getCMSType()) == cmd2->getCMSType() );
            CPPUNIT_ASSERT( std::string(cmd.getCMSMessageID()) == cmd2->getCMSMessageID() );

            core::ActiveMQMessage* message =
                dynamic_cast< core::ActiveMQMessage* >( cmd2 );

            CPPUNIT_ASSERT( message != NULL );
            CPPUNIT_ASSERT( cmd.getRedeliveryCount() ==
                            message->getRedeliveryCount() );

            StompCommand* cmd4 =
                dynamic_cast< StompCommand* >( cmd2 );

            CPPUNIT_ASSERT( cmd4 != NULL );
            CPPUNIT_ASSERT( cmd.getCommandId() == cmd4->getCommandId() );
            CPPUNIT_ASSERT( cmd.getStompCommandId() == cmd4->getStompCommandId() );
            CPPUNIT_ASSERT( cmd.isResponseRequired() == cmd4->isResponseRequired() );
            CPPUNIT_ASSERT( cmd.getCorrelationId() == cmd4->getCorrelationId() );
            CPPUNIT_ASSERT( std::string(cmd.getTransactionId()) ==
                            cmd4->getTransactionId() );

            delete cmd2;
        }

        void testSetProperties(){
            MessageCommand cmd;

            cmd.setBooleanProperty( "bool", true );
            CPPUNIT_ASSERT( cmd.getBooleanProperty( "bool" ) == true );

            cmd.setByteProperty( "byte", 1 );
            CPPUNIT_ASSERT( cmd.getByteProperty( "byte" ) == 1 );

            cmd.setDoubleProperty( "double", 2.2 );
            CPPUNIT_ASSERT( cmd.getDoubleProperty( "double" ) == 2.2 );

            cmd.setFloatProperty( "float", 3.3f );
            CPPUNIT_ASSERT( cmd.getFloatProperty( "float" ) == 3.3f );

            cmd.setIntProperty( "int", 4 );
            CPPUNIT_ASSERT( cmd.getIntProperty( "int" ) == 4 );

            cmd.setLongProperty( "long", 5LL );
            CPPUNIT_ASSERT( cmd.getLongProperty( "long" ) == 5LL );

            cmd.setShortProperty( "short", 6 );
            CPPUNIT_ASSERT( cmd.getShortProperty( "short" ) == 6 );

            cmd.setStringProperty( "string", "hello" );
            CPPUNIT_ASSERT( cmd.getStringProperty( "string" ) == "hello" );

            cmd.setStringProperty( "JMSXGroupID", "hello" );
            CPPUNIT_ASSERT( cmd.getStringProperty( "JMSXGroupID" ) == "hello" );
        }

        void testFailedGetProperties(){
            MessageCommand cmd;

            // Test getting a string property that doesn't exist.
            try {
                std::string str = cmd.getStringProperty( "text" );
                CPPUNIT_ASSERT(false);
            } catch( cms::CMSException& e){
            }

            // Test getting a bool property that doesn't exist.
            try {
                cmd.getBooleanProperty( "text" );
                CPPUNIT_ASSERT(false);
            } catch( cms::CMSException& e){
            }

            // Test accessing non-existent property
            try{
                cmd.getIntProperty("string");
                CPPUNIT_ASSERT(false);
            } catch( cms::CMSException& e ){
            }

            // Test failed extraction
            try{
                cmd.setStringProperty("string", "hello");
                cmd.getIntProperty("string");
                CPPUNIT_ASSERT(false);
            } catch( cms::CMSException& e ){
            }
        }

        void testGetPropertyNames(){

            MessageCommand cmd;

            std::vector<std::string> names = cmd.getPropertyNames();
            CPPUNIT_ASSERT( names.size() == 0 );

            cmd.setIntProperty( "int1", 1 );
            names = cmd.getPropertyNames();
            CPPUNIT_ASSERT( names.size() == 1 );
            CPPUNIT_ASSERT( names[0] == "int1" );

            cmd.setIntProperty( "int2", 2 );
            names = cmd.getPropertyNames();
            CPPUNIT_ASSERT( names.size() == 2 );
            CPPUNIT_ASSERT( names[0] == "int1" || names[1] == "int1" );
            CPPUNIT_ASSERT( names[0] == "int2" || names[1] == "int2" );

            // Make sure headers aren't included in the property names.
            cmd.setCMSExpiration( 10000 );
            names = cmd.getPropertyNames();
            CPPUNIT_ASSERT( names.size() == 2 );
        }

        void testClearProperties(){
            MessageCommand cmd;

            cmd.setIntProperty( "int1", 1 );
            cmd.setIntProperty( "int2", 2 );

            CPPUNIT_ASSERT( cmd.propertyExists( "int1" ) == true );
            CPPUNIT_ASSERT( cmd.propertyExists( "int2" ) == true );

            cmd.clearProperties();

            CPPUNIT_ASSERT( cmd.propertyExists( "int1" ) == false );
            CPPUNIT_ASSERT( cmd.propertyExists( "int2" ) == false );
        }

    };

}}}}

#endif /*_ACTIVEMQ_CONNECTOR_STOMP_COMMANDS_MESSAGECOMMANDTEST_H_*/
