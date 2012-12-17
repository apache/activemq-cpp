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

#ifndef _ACTIVEMQ_UTIL_ADVISORYSUPPORTTEST_H_
#define _ACTIVEMQ_UTIL_ADVISORYSUPPORTTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace activemq {
namespace util {

    class AdvisorySupportTest : public CppUnit::TestFixture {
    private:

        CPPUNIT_TEST_SUITE( AdvisorySupportTest );
        CPPUNIT_TEST( testGetTempDestinationCompositeAdvisoryTopic );
        CPPUNIT_TEST( testGetAllDestinationsCompositeAdvisoryTopic );
        CPPUNIT_TEST( testGetAllDestinationAdvisoryTopics );
        CPPUNIT_TEST( testGetConnectionAdvisoryTopic );
        CPPUNIT_TEST( testGetQueueAdvisoryTopic );
        CPPUNIT_TEST( testGetTopicAdvisoryTopic );
        CPPUNIT_TEST( testGetTempQueueAdvisoryTopic );
        CPPUNIT_TEST( testGetTempTopicAdvisoryTopic );
        CPPUNIT_TEST( testGetConsumerAdvisoryTopic );
        CPPUNIT_TEST( testGetProducerAdvisoryTopic );
        CPPUNIT_TEST( testGetExpiredMessageTopic );
        CPPUNIT_TEST( testGetExpiredTopicMessageAdvisoryTopic );
        CPPUNIT_TEST( testGetExpiredQueueMessageAdvisoryTopic );
        CPPUNIT_TEST( testGetNoConsumersAdvisoryTopic );
        CPPUNIT_TEST( testGetNoTopicConsumersAdvisoryTopic );
        CPPUNIT_TEST( testGetNoQueueConsumersAdvisoryTopic );
        CPPUNIT_TEST( testGetSlowConsumerAdvisoryTopic );
        CPPUNIT_TEST( testGetFastProducerAdvisoryTopic );
        CPPUNIT_TEST( testGetMessageDiscardedAdvisoryTopic );
        CPPUNIT_TEST( testGetMessageDeliveredAdvisoryTopic );
        CPPUNIT_TEST( testGetMessageConsumedAdvisoryTopic );
        CPPUNIT_TEST( testGetMessageDLQdAdvisoryTopic );
        CPPUNIT_TEST( testGetMasterBrokerAdvisoryTopic );
        CPPUNIT_TEST( testGetNetworkBridgeAdvisoryTopic );
        CPPUNIT_TEST( testGetFullAdvisoryTopic );
        CPPUNIT_TEST( testGetDestinationAdvisoryTopic );
        CPPUNIT_TEST( testIsDestinationAdvisoryTopic );
        CPPUNIT_TEST( testIsTempDestinationAdvisoryTopic );
        CPPUNIT_TEST( testIsAdvisoryTopic );
        CPPUNIT_TEST( testIsConnectionAdvisoryTopic );
        CPPUNIT_TEST( testIsProducerAdvisoryTopic );
        CPPUNIT_TEST( testIsConsumerAdvisoryTopic );
        CPPUNIT_TEST( testIsSlowConsumerAdvisoryTopic );
        CPPUNIT_TEST( testIsFastProducerAdvisoryTopic );
        CPPUNIT_TEST( testIsMessageConsumedAdvisoryTopic );
        CPPUNIT_TEST( testIsMasterBrokerAdvisoryTopic );
        CPPUNIT_TEST( testIsMessageDeliveredAdvisoryTopic );
        CPPUNIT_TEST( testIsMessageDiscardedAdvisoryTopic );
        CPPUNIT_TEST( testIsMessageDLQdAdvisoryTopic );
        CPPUNIT_TEST( testIsFullAdvisoryTopic );
        CPPUNIT_TEST( testIsNetworkBridgeAdvisoryTopic );
        CPPUNIT_TEST_SUITE_END();

    public:

        AdvisorySupportTest();
        virtual ~AdvisorySupportTest();

        void testGetTempDestinationCompositeAdvisoryTopic();
        void testGetAllDestinationsCompositeAdvisoryTopic();
        void testGetAllDestinationAdvisoryTopics();
        void testGetConnectionAdvisoryTopic();
        void testGetQueueAdvisoryTopic();
        void testGetTopicAdvisoryTopic();
        void testGetTempQueueAdvisoryTopic();
        void testGetTempTopicAdvisoryTopic();
        void testGetConsumerAdvisoryTopic();
        void testGetProducerAdvisoryTopic();
        void testGetExpiredMessageTopic();
        void testGetExpiredTopicMessageAdvisoryTopic();
        void testGetExpiredQueueMessageAdvisoryTopic();
        void testGetNoConsumersAdvisoryTopic();
        void testGetNoTopicConsumersAdvisoryTopic();
        void testGetNoQueueConsumersAdvisoryTopic();
        void testGetSlowConsumerAdvisoryTopic();
        void testGetFastProducerAdvisoryTopic();
        void testGetMessageDiscardedAdvisoryTopic();
        void testGetMessageDeliveredAdvisoryTopic();
        void testGetMessageConsumedAdvisoryTopic();
        void testGetMessageDLQdAdvisoryTopic();
        void testGetMasterBrokerAdvisoryTopic();
        void testGetNetworkBridgeAdvisoryTopic();
        void testGetFullAdvisoryTopic();
        void testGetDestinationAdvisoryTopic();

        void testIsDestinationAdvisoryTopic();
        void testIsTempDestinationAdvisoryTopic();
        void testIsAdvisoryTopic();
        void testIsConnectionAdvisoryTopic();
        void testIsProducerAdvisoryTopic();
        void testIsConsumerAdvisoryTopic();
        void testIsSlowConsumerAdvisoryTopic();
        void testIsFastProducerAdvisoryTopic();
        void testIsMessageConsumedAdvisoryTopic();
        void testIsMasterBrokerAdvisoryTopic();
        void testIsMessageDeliveredAdvisoryTopic();
        void testIsMessageDiscardedAdvisoryTopic();
        void testIsMessageDLQdAdvisoryTopic();
        void testIsFullAdvisoryTopic();
        void testIsNetworkBridgeAdvisoryTopic();

    };

}}

#endif /* _ACTIVEMQ_UTIL_ADVISORYSUPPORTTEST_H_ */
