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

#ifndef _ACTIVEMQ_TEST_OPENWIRE_OPENWIREXATRANSACTIONSTEST_H_
#define _ACTIVEMQ_TEST_OPENWIRE_OPENWIREXATRANSACTIONSTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <activemq/util/IntegrationCommon.h>
#include <activemq/util/IdGenerator.h>

#include <cms/Xid.h>

namespace activemq {
namespace test {
namespace openwire {

    class OpenwireXATransactionsTest : public CppUnit::TestFixture {

        CPPUNIT_TEST_SUITE( OpenwireXATransactionsTest );
        CPPUNIT_TEST( testCreateXAConnectionFactory );
        CPPUNIT_TEST( testCreateXAConnection );
        CPPUNIT_TEST( testCreateXASession );
        CPPUNIT_TEST( testGetXAResource );
        CPPUNIT_TEST( testXAResource_Exception1 );
        CPPUNIT_TEST( testXAResource_Exception2 );
        CPPUNIT_TEST( testXAResource_Exception3 );
        CPPUNIT_TEST( testSendReceiveOutsideTX );
        CPPUNIT_TEST( testSendReceiveTransactedBatches );
        CPPUNIT_TEST( testSendRollback );
        CPPUNIT_TEST( testWithTTLSet );
        CPPUNIT_TEST( testSendRollbackCommitRollback );
        CPPUNIT_TEST_SUITE_END();

    private:

        static const int batchCount;
        static const int batchSize;

        util::IdGenerator txIdGen;

    public:

        OpenwireXATransactionsTest();
        virtual ~OpenwireXATransactionsTest();

        virtual std::string getBrokerURL() const {
            return activemq::util::IntegrationCommon::getInstance().getOpenwireURL();
        }

        virtual void setUp() {}
        virtual void tearDown() {}

        void testCreateXAConnectionFactory();
        void testCreateXAConnection();
        void testCreateXASession();
        void testGetXAResource();
        void testSendReceiveOutsideTX();
        void testSendReceiveTransactedBatches();
        void testSendRollback();
        void testWithTTLSet();
        void testSendRollbackCommitRollback();
        void testXAResource_Exception1();
        void testXAResource_Exception2();
        void testXAResource_Exception3();

    private:

        cms::Xid* createXid() const;

    };

}}}

#endif /* _ACTIVEMQ_TEST_OPENWIRE_OPENWIREXATRANSACTIONSTEST_H_ */
