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

#ifndef ACTIVEMQ_TEST_OPENWIRE_OPENWIREMESSAGELISTENERREDELIVERYTEST_H_
#define ACTIVEMQ_TEST_OPENWIRE_OPENWIREMESSAGELISTENERREDELIVERYTEST_H_

#include <activemq/test/CMSTestFixture.h>
#include <activemq/util/IntegrationCommon.h>

namespace activemq {
namespace test {
namespace openwire {

    class OpenWireMessageListenerRedeliveryTest : public CMSTestFixture {
    private:

        CPPUNIT_TEST_SUITE( OpenWireMessageListenerRedeliveryTest );
        CPPUNIT_TEST( testQueueRollbackConsumerListener );
        CPPUNIT_TEST( testQueueSessionListenerExceptionRetry );
        CPPUNIT_TEST( testQueueSessionListenerExceptionDlq );
        CPPUNIT_TEST( testTransactedQueueSessionListenerExceptionDlq );
        CPPUNIT_TEST_SUITE_END();

    public:

        OpenWireMessageListenerRedeliveryTest();
        virtual ~OpenWireMessageListenerRedeliveryTest();

        virtual std::string getBrokerURL() const {
            return activemq::util::IntegrationCommon::getInstance().getOpenwireURL();
        }

        void testQueueRollbackConsumerListener();
        void testQueueSessionListenerExceptionRetry();
        void testQueueSessionListenerExceptionDlq();
        void testTransactedQueueSessionListenerExceptionDlq();

    };

}}}

#endif /* ACTIVEMQ_TEST_OPENWIRE_OPENWIREMESSAGELISTENERREDELIVERYTEST_H_ */
