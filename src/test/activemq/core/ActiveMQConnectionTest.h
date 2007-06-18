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

#ifndef _ACTIVEMQ_CORE_ACTIVEMQCONNECTIONTEST_H_
#define _ACTIVEMQ_CORE_ACTIVEMQCONNECTIONTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <cms/Connection.h>
#include <cms/ExceptionListener.h>

#include <activemq/core/ActiveMQMessage.h>
#include <activemq/connector/ConsumerMessageListener.h>
#include <activemq/connector/ConsumerInfo.h>
#include <activemq/transport/Transport.h>
#include <activemq/transport/CommandListener.h>
#include <activemq/util/Config.h>
#include <activemq/core/ActiveMQConnection.h>

namespace activemq{
namespace core{

    class ActiveMQConnectionTest : public CppUnit::TestFixture
    {
        CPPUNIT_TEST_SUITE( ActiveMQConnectionTest );
        CPPUNIT_TEST( test1WithStomp );
        CPPUNIT_TEST( test2WithStomp );
        CPPUNIT_TEST( test2WithOpenwire );
        CPPUNIT_TEST_SUITE_END();

    public:

        ActiveMQConnectionTest() {};
        virtual ~ActiveMQConnectionTest() {}

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

        class MyMessageListener :
            public connector::ConsumerMessageListener
        {
        public:

            std::vector<connector::ConsumerInfo*> consumers;

        public:
            virtual ~MyMessageListener(){}

            virtual void onConsumerMessage(
                connector::ConsumerInfo* consumer,
                core::ActiveMQMessage* msg AMQCPP_UNUSED )
            {
                consumers.push_back( consumer );
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

            std::vector<ActiveMQMessage*> messages;

        public:
            virtual ~MyDispatcher(){}

            virtual void dispatch( DispatchData& data )
                throw ( exceptions::ActiveMQException )
            {
                messages.push_back( data.getMessage() );
            }
        };

        void test1WithStomp();
        void test2WithStomp();
        void test2WithOpenwire();

    };

}}

#endif /*_ACTIVEMQ_CORE_ACTIVEMQCONNECTIONTEST_H_*/
