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

#ifndef _ACTIVEMQ_CONNECTOR_STOMP_STOMPSESSIONMANAGERTEST_H_
#define _ACTIVEMQ_CONNECTOR_STOMP_STOMPSESSIONMANAGERTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <activemq/connector/stomp/StompSessionManager.h>
#include <activemq/connector/stomp/StompResponseBuilder.h>
#include <activemq/connector/stomp/StompConnector.h>
#include <activemq/connector/stomp/StompTopic.h>
#include <activemq/connector/stomp/StompQueue.h>
#include <activemq/transport/MockTransport.h>
#include <activemq/connector/stomp/commands/TextMessageCommand.h>
#include <activemq/connector/stomp/commands/SubscribeCommand.h>
#include <activemq/transport/CommandListener.h>
#include <cms/Session.h>
#include <vector>

namespace activemq{
namespace connector{
namespace stomp{

    class StompSessionManagerTest : public CppUnit::TestFixture
    {

        CPPUNIT_TEST_SUITE( StompSessionManagerTest );
        CPPUNIT_TEST( testSessions );
        CPPUNIT_TEST( testConsumers );
        CPPUNIT_TEST( testCommand );
        CPPUNIT_TEST( testSendingCommands );
        CPPUNIT_TEST( testSubscribeOptions );
        CPPUNIT_TEST_SUITE_END();

    private:

        StompConnector* connector;
        StompSessionManager* manager;
        transport::MockTransport* transport;
        std::string connectionId;

    public:

        typedef std::pair<std::string, std::string> MyProperty;

        void setUp();
        void tearDown();

        class MyCommandListener : public transport::CommandListener{
        public:

            transport::Command* cmd;
            commands::SubscribeCommand* subscribe;

            // Properties that should be in an command
            std::vector<MyProperty> expected;

        public:

            MyCommandListener(){
                cmd = NULL;
                subscribe = NULL;
            }
            virtual ~MyCommandListener(){}

            virtual void onCommand( transport::Command* command ){
                cmd = command;

                subscribe = dynamic_cast< commands::SubscribeCommand* >( command );
                if( subscribe )
                {
                    const decaf::util::Properties& properties =
                        subscribe->getProperties();

                    for( size_t ix = 0; ix < expected.size(); ++ix )
                    {
                        std::string value =
                            properties.getProperty( expected[ix].first, "" );

                        CPPUNIT_ASSERT( value == expected[ix].second );
                    }
                }
            }
        };

        class MyMessageListener : public ConsumerMessageListener{
        public:

            std::vector<ConsumerInfo*> consumers;

        public:
            virtual ~MyMessageListener(){}

            virtual void onConsumerMessage( ConsumerInfo* consumer,
                core::ActiveMQMessage* msg )
            {
                consumers.push_back( consumer );
                delete msg;
            }
        };

        virtual ~StompSessionManagerTest() {}

        void testSessions();
        void testConsumers();
        void testCommand();
        void testSendingCommands();
        void testSubscribeOptions();

    };

}}}

#endif /*_ACTIVEMQ_CONNECTOR_STOMP_STOMPSESSIONMANAGERTEST_H_*/
