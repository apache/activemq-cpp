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
  integration/IntegrationCommon.cpp \
  integration/TestSupport.cpp \
  integration/TestRegistry.cpp \
  integration/connector/openwire/OpenwireAsyncSenderTest.cpp \
  integration/connector/openwire/OpenwireDurableTest.cpp \
  integration/connector/openwire/OpenwireExpirationTest.cpp \
  integration/connector/openwire/OpenwireSimpleRollbackTest.cpp \
  integration/connector/openwire/OpenwireSimpleTest.cpp \
  integration/connector/openwire/OpenwireSlowListenerTest.cpp \
  integration/connector/openwire/OpenwireTransactionTest.cpp \
  integration/connector/openwire/OpenwireTempDestinationTest.cpp \
  integration/connector/stomp/AsyncSenderTest.cpp \
  integration/connector/stomp/DurableTest.cpp \
  integration/connector/stomp/ExpirationTest.cpp \
  integration/connector/stomp/SimpleRollbackTest.cpp \
  integration/connector/stomp/SimpleTest.cpp \
  integration/connector/stomp/TransactionTest.cpp \
  integration/connector/stomp/StompStressTests.cpp \
  main.cpp

## Compile this as part of make check
check_PROGRAMS = activemq-test-integration

## 
## Compiler/Linker Options
##
activemq_test_integration_SOURCES = $(cc_sources)
activemq_test_integration_CXXFLAGS = $(AMQ_TEST_CXXFLAGS) -I$(srcdir)/../main @CPPUNIT_CFLAGS@
activemq_test_integration_LDADD= $(AMQ_TEST_LIBS) @CPPUNIT_LIBS@

