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

#ifndef _ACTIVEMQ_CONNECTOR_STOMP_COMMAND_UNSUBSCRIBECOMMANDTEST_H_
#define _ACTIVEMQ_CONNECTOR_STOMP_COMMAND_UNSUBSCRIBECOMMANDTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <activemq/connector/stomp/commands/UnsubscribeCommand.h>

namespace activemq{
namespace connector{
namespace stomp{
namespace commands{

    class UnsubscribeCommandTest : public CppUnit::TestFixture
    {
        CPPUNIT_TEST_SUITE( UnsubscribeCommandTest );
        CPPUNIT_TEST( test );
        CPPUNIT_TEST_SUITE_END();
        
    public:
    	UnsubscribeCommandTest() {}
    	virtual ~UnsubscribeCommandTest() {}

        void test(void)
        {
            UnsubscribeCommand cmd;

            CPPUNIT_ASSERT( cmd.getStompCommandId() == 
                            CommandConstants::UNSUBSCRIBE );
            
            CPPUNIT_ASSERT( cmd.isResponseRequired() == false );
            cmd.setResponseRequired( true );
            cmd.setCommandId( 123 );
            CPPUNIT_ASSERT( cmd.isResponseRequired() == false );
            CPPUNIT_ASSERT( cmd.getCommandId() == 0 );
            cmd.setCorrelationId( 99 );
            CPPUNIT_ASSERT( cmd.getCorrelationId() == 0 );
            CPPUNIT_ASSERT( cmd.getTransactionId() == "" );
            cmd.setTransactionId( "ID:123456" );
            CPPUNIT_ASSERT( std::string( cmd.getTransactionId() ) == 
                            "ID:123456" );
            CPPUNIT_ASSERT( cmd.getDestination() == "" );
            cmd.setDestination( "456987" );
            CPPUNIT_ASSERT( std::string( cmd.getDestination() ) == 
                            "456987" );
            
            StompFrame* frame = cmd.marshal().clone();
            
            CPPUNIT_ASSERT( frame != NULL );
            
            UnsubscribeCommand cmd1( frame );
            
            CPPUNIT_ASSERT( cmd.getCommandId() == cmd1.getCommandId() );
            CPPUNIT_ASSERT( cmd.getStompCommandId() == cmd1.getStompCommandId() );
            CPPUNIT_ASSERT( cmd.isResponseRequired() == cmd1.isResponseRequired() );
            CPPUNIT_ASSERT( cmd.getCorrelationId() == cmd1.getCorrelationId() );
            CPPUNIT_ASSERT( cmd.getTransactionId() == cmd1.getTransactionId() );
            CPPUNIT_ASSERT( cmd.getDestination() == cmd1.getDestination() );
            
        }
    };

}}}}

#endif /*_ACTIVEMQ_CONNECTOR_STOMP_COMMAND_UNSUBSCRIBECOMMANDTEST_H_*/
