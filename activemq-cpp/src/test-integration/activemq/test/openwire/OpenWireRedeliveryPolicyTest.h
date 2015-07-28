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

#ifndef _ACTIVEMQ_TEST_OPENWIRE_OPENWIREREDELIVERYPOLICYTEST_H_
#define _ACTIVEMQ_TEST_OPENWIRE_OPENWIREREDELIVERYPOLICYTEST_H_

#include <activemq/test/CMSTestFixture.h>
#include <activemq/util/IntegrationCommon.h>

namespace activemq {
namespace test {
namespace openwire {

    class OpenWireRedeliveryPolicyTest : public CMSTestFixture {

        CPPUNIT_TEST_SUITE( OpenWireRedeliveryPolicyTest );
        CPPUNIT_TEST( testGetNext );
        CPPUNIT_TEST( testGetNextWithInitialDelay );
        CPPUNIT_TEST( testExponentialRedeliveryPolicyDelaysDeliveryOnRollback );
        CPPUNIT_TEST( testNornalRedeliveryPolicyDelaysDeliveryOnRollback );
        CPPUNIT_TEST( testDLQHandling );
        CPPUNIT_TEST( testInfiniteMaximumNumberOfRedeliveries );
        CPPUNIT_TEST( testZeroMaximumNumberOfRedeliveries );
        CPPUNIT_TEST( testRepeatedRedeliveryReceiveNoCommit );
        CPPUNIT_TEST( testRepeatedRedeliveryOnMessageNoCommit );
        CPPUNIT_TEST( testInitialRedeliveryDelayZero );
        CPPUNIT_TEST( testInitialRedeliveryDelayOne );
        CPPUNIT_TEST( testRedeliveryDelayOne );
        CPPUNIT_TEST( testMaximumRedeliveryDelay );
        CPPUNIT_TEST_SUITE_END();

    public:

        OpenWireRedeliveryPolicyTest();
        virtual ~OpenWireRedeliveryPolicyTest();

        virtual void setUp() {}
        virtual void tearDown() {}

        virtual std::string getBrokerURL() const;

        void testGetNext();
        void testGetNextWithInitialDelay();
        void testExponentialRedeliveryPolicyDelaysDeliveryOnRollback();
        void testNornalRedeliveryPolicyDelaysDeliveryOnRollback();
        void testDLQHandling();
        void testInfiniteMaximumNumberOfRedeliveries();
        void testMaximumRedeliveryDelay();
        void testZeroMaximumNumberOfRedeliveries();
        void testRepeatedRedeliveryReceiveNoCommit();
        void testRepeatedRedeliveryOnMessageNoCommit();
        void testInitialRedeliveryDelayZero();
        void testInitialRedeliveryDelayOne();
        void testRedeliveryDelayOne();

    };

}}}

#endif /* _ACTIVEMQ_TEST_OPENWIRE_OPENWIREREDELIVERYPOLICYTEST_H_ */
