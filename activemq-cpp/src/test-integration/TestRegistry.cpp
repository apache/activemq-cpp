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

#include "activemq/test/openwire/OpenwireAdvisorysTest.h"
#include "activemq/test/openwire/OpenwireAsyncSenderTest.h"
#include "activemq/test/openwire/OpenwireClientAckTest.h"
#include "activemq/test/openwire/OpenwireCmsConnectionStartStopTest.h"
#include "activemq/test/openwire/OpenWireCmsSendWithAsyncCallbackTest.h"
#include "activemq/test/openwire/OpenwireCmsTemplateTest.h"
#include "activemq/test/openwire/OpenwireDurableTest.h"
#include "activemq/test/openwire/OpenwireExpirationTest.h"
#include "activemq/test/openwire/OpenwireEnhancedConnectionTest.h"
#include "activemq/test/openwire/OpenwireIndividualAckTest.h"
#include "activemq/test/openwire/OpenwireJmsMessageGroupsTest.h"
#include "activemq/test/openwire/OpenwireJmsRecoverTest.h"
#include "activemq/test/openwire/OpenwireMessageCompressionTest.h"
#include "activemq/test/openwire/OpenwireMessagePriorityTest.h"
#include "activemq/test/openwire/OpenwireMapMessageTest.h"
#include "activemq/test/openwire/OpenwireNonBlockingRedeliveryTest.h"
#include "activemq/test/openwire/OpenwireOptimizedAckTest.h"
#include "activemq/test/openwire/OpenwireQueueBrowserTest.h"
#include "activemq/test/openwire/OpenwireSimpleRollbackTest.h"
#include "activemq/test/openwire/OpenwireSimpleTest.h"
#include "activemq/test/openwire/OpenwireTransactionTest.h"
#include "activemq/test/openwire/OpenwireTempDestinationTest.h"
#include "activemq/test/openwire/OpenwireSlowListenerTest.h"
#include "activemq/test/openwire/OpenwireVirtualTopicTest.h"
#include "activemq/test/openwire/OpenwireXATransactionsTest.h"
#include "activemq/test/stomp/StompAsyncSenderTest.h"
#include "activemq/test/stomp/StompBulkMessageTest.h"
#include "activemq/test/stomp/StompCmsTemplateTest.h"
#include "activemq/test/stomp/StompCmsConnectionStartStopTest.h"
#include "activemq/test/stomp/StompDurableTest.h"
#include "activemq/test/stomp/StompExpirationTest.h"
#include "activemq/test/stomp/StompSimpleRollbackTest.h"
#include "activemq/test/stomp/StompSimpleTest.h"
#include "activemq/test/stomp/StompSlowListenerTest.h"
#include "activemq/test/stomp/StompTransactionTest.h"
#include "activemq/test/stomp/StompJmsMessageGroupsTest.h"

// Openwire Tests
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::test::openwire::OpenwireAdvisorysTest );
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::test::openwire::OpenwireAsyncSenderTest );
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::test::openwire::OpenwireClientAckTest );
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::test::openwire::OpenwireCmsConnectionStartStopTest );
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::test::openwire::OpenWireCmsSendWithAsyncCallbackTest );
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::test::openwire::OpenwireCmsTemplateTest );
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::test::openwire::OpenwireDurableTest );
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::test::openwire::OpenwireExpirationTest );
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::test::openwire::OpenwireEnhancedConnectionTest );
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::test::openwire::OpenwireIndividualAckTest );
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::test::openwire::OpenwireJmsMessageGroupsTest );
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::test::openwire::OpenwireJmsRecoverTest );
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::test::openwire::OpenwireMessageCompressionTest );
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::test::openwire::OpenwireMessagePriorityTest );
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::test::openwire::OpenwireMapMessageTest );
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::test::openwire::OpenwireNonBlockingRedeliveryTest );
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::test::openwire::OpenwireOptimizedAckTest );
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::test::openwire::OpenwireQueueBrowserTest );
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::test::openwire::OpenwireSimpleRollbackTest );
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::test::openwire::OpenwireSimpleTest );
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::test::openwire::OpenwireTransactionTest );
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::test::openwire::OpenwireSlowListenerTest );
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::test::openwire::OpenwireTempDestinationTest );
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::test::openwire::OpenwireVirtualTopicTest );
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::test::openwire::OpenwireXATransactionsTest );

// Stomp Tests
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::test::stomp::StompAsyncSenderTest );
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::test::stomp::StompBulkMessageTest );
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::test::stomp::StompCmsTemplateTest );
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::test::stomp::StompCmsConnectionStartStopTest );
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::test::stomp::StompDurableTest );
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::test::stomp::StompExpirationTest );
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::test::stomp::StompSimpleTest );
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::test::stomp::StompSlowListenerTest );
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::test::stomp::StompJmsMessageGroupsTest );
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::test::stomp::StompTransactionTest );
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::test::stomp::StompSimpleRollbackTest );
