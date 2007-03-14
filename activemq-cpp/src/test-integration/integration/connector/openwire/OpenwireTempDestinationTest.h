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

#include <activemq/concurrent/Mutex.h>

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

        TestSupport testSupport;

    public:

        OpenwireTempDestinationTest();
        virtual ~OpenwireTempDestinationTest();

        virtual void test();

    protected:

        class Consumer : public cms::MessageListener,
                         public activemq::concurrent::Runnable {

        private:

            cms::Connection* connection;
            cms::Session* session;
            cms::Destination* destination;
            cms::MessageConsumer* consumer;
            int numReceived;
            activemq::concurrent::Mutex mutex;

        public:

            Consumer( cms::Connection* connection,
                      cms::Session* session,
                      cms::Destination* destination );

            virtual ~Consumer();

            virtual int getNumReceived() const {
                return this->numReceived;
            }

            virtual void stop();
            virtual void run();
            virtual void onMessage( const cms::Message* message );
        };

    };

}}}

#endif /*_INTEGRATION_CONNECTOR_OPENWIRE_OPENWIRETEMPDESTINATIONTEST_H_*/
