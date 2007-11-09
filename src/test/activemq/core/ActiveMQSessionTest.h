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

#ifndef _ACTIVEMQ_CORE_ACTIVEMQSESSIONTEST_H_
#define _ACTIVEMQ_CORE_ACTIVEMQSESSIONTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <cms/Connection.h>
#include <cms/MessageListener.h>

#include <decaf/util/concurrent/Concurrent.h>
#include <decaf/util/concurrent/Mutex.h>
#include <activemq/core/ActiveMQConnection.h>
#include <activemq/transport/MockTransport.h>
#include <activemq/util/Config.h>

namespace activemq{
namespace core{

    class ActiveMQSessionTest : public CppUnit::TestFixture
    {
        CPPUNIT_TEST_SUITE( ActiveMQSessionTest );
        CPPUNIT_TEST( testAutoAcking );
        CPPUNIT_TEST( testClientAck );
        CPPUNIT_TEST( testTransactional );
        CPPUNIT_TEST( testExpiration );
        CPPUNIT_TEST_SUITE_END();

    private:

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

        class MyCMSMessageListener : public cms::MessageListener
        {
        public:

            std::vector<cms::Message*> messages;
            decaf::util::concurrent::Mutex mutex;
            bool ack;

        public:

            MyCMSMessageListener( bool ack = false ){
                this->ack = ack;
            }

            virtual ~MyCMSMessageListener(){
                clear();
            }

            virtual void setAck( bool ack ){
                this->ack = ack;
            }

            virtual void clear() {
                std::vector<cms::Message*>::iterator itr =
                    messages.begin();

                for( ; itr != messages.end(); ++itr )
                {
                    delete *itr;
                }

                messages.clear();
            }

            virtual void onMessage( const cms::Message* message )
            {
                synchronized( &mutex )
                {
                    if( ack ){
                        message->acknowledge();
                    }

                    messages.push_back( message->clone() );

                    mutex.notifyAll();
                }
            }
        };

        ActiveMQConnection* connection;
        transport::MockTransport* dTransport;
        MyExceptionListener exListener;

    public:    // CPPUNIT Method Overrides.

        void setUp();
        void tearDown();
        void injectTextMessage( const std::string message,
                                const cms::Destination& destination,
                                const long long timeStamp = -1,
                                const long long timeToLive = -1 );

    public:

        ActiveMQSessionTest(void) {}
        virtual ~ActiveMQSessionTest(void) {}

        void testAutoAcking();
        void testClientAck();
        void testTransactional();
        void testExpiration();

    };

}}

#endif /*_ACTIVEMQ_CORE_ACTIVEMQSESSIONTEST_H_*/
