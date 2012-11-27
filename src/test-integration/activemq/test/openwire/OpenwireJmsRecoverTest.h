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

#ifndef _ACTIVEMQ_TEST_OPENWIRE_OPENWIREJMSRECOVERTEST_H_
#define _ACTIVEMQ_TEST_OPENWIRE_OPENWIREJMSRECOVERTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <cms/ConnectionFactory.h>
#include <cms/Connection.h>
#include <cms/Destination.h>

#include <activemq/util/IntegrationCommon.h>

namespace activemq {
namespace test {
namespace openwire {

    class OpenwireJmsRecoverTest : public CppUnit::TestFixture {
    private:

        CPPUNIT_TEST_SUITE( OpenwireJmsRecoverTest );
        CPPUNIT_TEST( testQueueSynchRecover );
        CPPUNIT_TEST( testQueueAsynchRecover );
        CPPUNIT_TEST( testTopicSynchRecover );
        CPPUNIT_TEST( testTopicAsynchRecover );
        CPPUNIT_TEST( testQueueAsynchRecoverWithAutoAck );
        CPPUNIT_TEST( testTopicAsynchRecoverWithAutoAck );
        CPPUNIT_TEST_SUITE_END();

        cms::ConnectionFactory* factory;
        cms::Connection* connection;
        cms::Destination* destination;

    private:

        OpenwireJmsRecoverTest(const OpenwireJmsRecoverTest&);
        OpenwireJmsRecoverTest& operator= (const OpenwireJmsRecoverTest&);

    public:

        OpenwireJmsRecoverTest();
        virtual ~OpenwireJmsRecoverTest();

        virtual std::string getBrokerURL() const {
            return activemq::util::IntegrationCommon::getInstance().getOpenwireURL();
        }

        virtual void setUp();
        virtual void tearDown();

        void testQueueSynchRecover();
        void testQueueAsynchRecover();
        void testTopicSynchRecover();
        void testTopicAsynchRecover();
        void testQueueAsynchRecoverWithAutoAck();
        void testTopicAsynchRecoverWithAutoAck();

    private:

        void doTestSynchRecover();
        void doTestAsynchRecover();
        void doTestAsynchRecoverWithAutoAck();

    };

}}}

#endif /* _ACTIVEMQ_TEST_OPENWIRE_OPENWIREJMSRECOVERTEST_H_ */
