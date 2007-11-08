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

#ifndef _ACTIVEMQ_CONNECTOR_STOMP_STOMPCONNECTORTEST_H_
#define _ACTIVEMQ_CONNECTOR_STOMP_STOMPCONNECTORTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <activemq/connector/stomp/StompResponseBuilder.h>
#include <activemq/connector/stomp/StompConnector.h>
#include <activemq/transport/Command.h>
#include <activemq/connector/stomp/commands/ConnectedCommand.h>
#include <activemq/connector/stomp/commands/TextMessageCommand.h>
#include <activemq/connector/stomp/commands/BytesMessageCommand.h>
#include <activemq/connector/stomp/StompTopic.h>
#include <activemq/connector/stomp/StompQueue.h>
#include <activemq/transport/MockTransport.h>
#include <activemq/util/Config.h>

#include <decaf/io/ByteArrayOutputStream.h>
#include <algorithm>

namespace activemq{
namespace connector{
namespace stomp{

    class StompConnectorTest : public CppUnit::TestFixture
    {
        CPPUNIT_TEST_SUITE( StompConnectorTest );
        CPPUNIT_TEST( testSessions );
        CPPUNIT_TEST( testConsumers );
        CPPUNIT_TEST( testProducers );
        CPPUNIT_TEST( testCommand );
        CPPUNIT_TEST( testSendingCommands );
        CPPUNIT_TEST( testException );
        CPPUNIT_TEST_SUITE_END();

    private:

        StompConnector* connector;
        transport::MockTransport* transport;
        std::string connectionId;

    public:

        StompConnectorTest() {}
        virtual ~StompConnectorTest() {}

        void setUp();
        void tearDown();

        class MyExceptionListener : public cms::ExceptionListener{
            public:
                int num;
                MyExceptionListener(){ num=0;}
                virtual ~MyExceptionListener(){}
                virtual void onException( const cms::CMSException& ex AMQCPP_UNUSED){
                    num++;
                }
        };

        class MyCommandListener : public transport::CommandListener{
        public:

            transport::Command* cmd;

        public:

            MyCommandListener(){
                cmd = NULL;
            }
            virtual ~MyCommandListener(){}

            virtual void onCommand( transport::Command* command ){
                cmd = command;
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

        void testSessions();
        void testConsumers();
        void testProducers();
        void testCommand();
        void testSendingCommands();
        void testException();

    };

}}}

#endif /*_ACTIVEMQ_CONNECTOR_STOMP_STOMPCONNECTORTEST_H_*/
