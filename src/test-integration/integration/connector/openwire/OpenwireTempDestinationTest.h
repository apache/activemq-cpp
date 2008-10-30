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

#ifndef _INTEGRATION_CONNECTOR_OPENWIRE_OPENWIRETEMPDESTINATIONTEST_H_
#define _INTEGRATION_CONNECTOR_OPENWIRE_OPENWIRETEMPDESTINATIONTEST_H_

#include <decaf/util/concurrent/Mutex.h>
#include <decaf/util/concurrent/CountDownLatch.h>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <integration/TestSupport.h>

namespace integration{
namespace connector{
namespace openwire{

    class OpenwireTempDestinationTest : public CppUnit::TestFixture {

        CPPUNIT_TEST_SUITE( OpenwireTempDestinationTest );
        CPPUNIT_TEST( test );
        CPPUNIT_TEST_SUITE_END();

    private:

        TestSupport* testSupport;

    public:

        OpenwireTempDestinationTest();
        virtual ~OpenwireTempDestinationTest();

        virtual void test();

        virtual void setUp() {
            testSupport = new TestSupport;
            testSupport->initialize(IntegrationCommon::getInstance().getOpenwireURL());
        };
        virtual void tearDown() { delete testSupport; };

    protected:

        class Consumer : public cms::MessageListener,
                         public decaf::lang::Runnable {

        private:

            cms::Connection* connection;
            cms::Session* session;
            cms::Destination* destination;
            cms::MessageConsumer* consumer;
            unsigned int numReceived;
            decaf::util::concurrent::Mutex mutex;
            decaf::util::concurrent::Mutex onMsgMutex;
            decaf::util::concurrent::CountDownLatch ready;

        public:

            Consumer( cms::Connection* connection,
                      cms::Session* session,
                      cms::Destination* destination );

            virtual ~Consumer();

            virtual decaf::util::concurrent::Mutex& getOnMsgMutex() {
                return this->onMsgMutex;
            }

            virtual unsigned int getNumReceived() const {
                return this->numReceived;
            }

            virtual void waitUnitReady() {
                this->ready.await();
            }

            virtual void stop();
            virtual void run();
            virtual void onMessage( const cms::Message* message );
        };

        // Internal Wait method
        void waitForMessages( Consumer& consumer,
                              unsigned int count );

    };

}}}

#endif /*_INTEGRATION_CONNECTOR_OPENWIRE_OPENWIRETEMPDESTINATIONTEST_H_*/
