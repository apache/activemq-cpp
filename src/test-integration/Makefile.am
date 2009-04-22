# ---------------------------------------------------------------------------
# Licensed to the Apache Software Foundation (ASF) under one or more
# contributor license agreements.  See the NOTICE file distributed with
# this work for additional information regarding copyright ownership.
# The ASF licenses this file to You under the Apache License, Version 2.0
# (the "License"); you may not use this file except in compliance with
# the License.  You may obtain a copy of the License at
# 
# http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# ---------------------------------------------------------------------------

cc_sources = \
  activemq/util/CMSListener.cpp \
  activemq/util/CMSProvider.cpp \
  activemq/util/IntegrationCommon.cpp \
  activemq/test/AsyncSenderTest.cpp \
  activemq/test/CmsTemplateTest.cpp \
  activemq/test/DurableTest.cpp \
  activemq/test/ExpirationTest.cpp \
  activemq/test/SimpleRollbackTest.cpp \
  activemq/test/SimpleTest.cpp \
  activemq/test/TransactionTest.cpp \
  activemq/test/SlowListenerTest.cpp \
  activemq/test/JmsMessageGroupsTest.cpp \
  activemq/test/stomp/StompAsyncSenderTest.cpp \
  activemq/test/stomp/StompCmsTemplateTest.cpp \
  activemq/test/stomp/StompDurableTest.cpp \
  activemq/test/stomp/StompExpirationTest.cpp \
  activemq/test/stomp/StompSimpleRollbackTest.cpp \
  activemq/test/stomp/StompSimpleTest.cpp \
  activemq/test/stomp/StompTransactionTest.cpp \
  activemq/test/stomp/StompSlowListenerTest.cpp \
  activemq/test/stomp/StompJmsMessageGroupsTest.cpp \
  activemq/test/openwire/OpenwireAsyncSenderTest.cpp \
  activemq/test/openwire/OpenwireCmsTemplateTest.cpp \
  activemq/test/openwire/OpenwireDurableTest.cpp \
  activemq/test/openwire/OpenwireExpirationTest.cpp \
  activemq/test/openwire/OpenwireSimpleRollbackTest.cpp \
  activemq/test/openwire/OpenwireSimpleTest.cpp \
  activemq/test/openwire/OpenwireTransactionTest.cpp \
  activemq/test/openwire/OpenwireTempDestinationTest.cpp \
  activemq/test/openwire/OpenwireSlowListenerTest.cpp \
  activemq/test/openwire/OpenwireJmsMessageGroupsTest.cpp \
  TestRegistry.cpp \
  util/teamcity/TeamCityProgressListener.cpp \
  main.cpp

## Compile this as part of make check
check_PROGRAMS = activemq-test-integration

## 
## Compiler/Linker Options
##
activemq_test_integration_SOURCES = $(cc_sources)
activemq_test_integration_CXXFLAGS = $(AMQ_TEST_CXXFLAGS) -I$(srcdir)/../main @CPPUNIT_CFLAGS@
activemq_test_integration_LDADD= $(AMQ_TEST_LIBS) @CPPUNIT_LIBS@

