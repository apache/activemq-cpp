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

#include "connector/openwire/OpenwireAsyncSenderTest.h"
#include "connector/openwire/OpenwireDurableTest.h"
#include "connector/openwire/OpenwireExpirationTest.h"
#include "connector/openwire/OpenwireSimpleRollbackTest.h"
#include "connector/openwire/OpenwireSimpleTest.h"
#include "connector/openwire/OpenwireSlowListenerTest.h"
#include "connector/openwire/OpenwireTempDestinationTest.h"
#include "connector/openwire/OpenwireTransactionTest.h"

#include "connector/stomp/AsyncSenderTest.h"
#include "connector/stomp/CmsTemplateTest.h"
#include "connector/stomp/DurableTest.h"
#include "connector/stomp/ExpirationTest.h"
#include "connector/stomp/SimpleRollbackTest.h"
#include "connector/stomp/SimpleTest.h"
#include "connector/stomp/TransactionTest.h"
#include "connector/stomp/StompStressTests.h"

CPPUNIT_TEST_SUITE_REGISTRATION( integration::connector::openwire::OpenwireAsyncSenderTest );
CPPUNIT_TEST_SUITE_REGISTRATION( integration::connector::openwire::OpenwireDurableTest );
CPPUNIT_TEST_SUITE_REGISTRATION( integration::connector::openwire::OpenwireExpirationTest );
CPPUNIT_TEST_SUITE_REGISTRATION( integration::connector::openwire::OpenwireSimpleRollbackTest );
CPPUNIT_TEST_SUITE_REGISTRATION( integration::connector::openwire::OpenwireSimpleTest );
CPPUNIT_TEST_SUITE_REGISTRATION( integration::connector::openwire::OpenwireSlowListenerTest );
CPPUNIT_TEST_SUITE_REGISTRATION( integration::connector::openwire::OpenwireTempDestinationTest );
CPPUNIT_TEST_SUITE_REGISTRATION( integration::connector::openwire::OpenwireTransactionTest );

CPPUNIT_TEST_SUITE_REGISTRATION( integration::connector::stomp::AsyncSenderTest );
CPPUNIT_TEST_SUITE_REGISTRATION( integration::connector::stomp::CmsTemplateTest );
CPPUNIT_TEST_SUITE_REGISTRATION( integration::connector::stomp::DurableTest );
CPPUNIT_TEST_SUITE_REGISTRATION( integration::connector::stomp::ExpirationTest );
CPPUNIT_TEST_SUITE_REGISTRATION( integration::connector::stomp::SimpleRollbackTest );
CPPUNIT_TEST_SUITE_REGISTRATION( integration::connector::stomp::SimpleTest );
CPPUNIT_TEST_SUITE_REGISTRATION( integration::connector::stomp::TransactionTest );
CPPUNIT_TEST_SUITE_REGISTRATION( integration::connector::stomp::StompStressTests );
