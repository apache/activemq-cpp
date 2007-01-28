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
  activemq/concurrent/MutexTest.cpp \
  activemq/concurrent/ThreadPoolTest.cpp \
  activemq/concurrent/ThreadTest.cpp \
  activemq/connector/ConnectorFactoryMapRegistrarTest.cpp \
  activemq/connector/ConnectorFactoryMapTest.cpp \
  activemq/connector/stomp/commands/AbortCommandTest.cpp \
  activemq/connector/stomp/commands/AckCommandTest.cpp \
  activemq/connector/stomp/commands/BeginCommandTest.cpp \
  activemq/connector/stomp/commands/BytesMessageCommandTest.cpp \
  activemq/connector/stomp/commands/CommandConstantsTest.cpp \
  activemq/connector/stomp/commands/CommitCommandTest.cpp \
  activemq/connector/stomp/commands/ConnectCommandTest.cpp \
  activemq/connector/stomp/commands/ConnectedCommandTest.cpp \
  activemq/connector/stomp/commands/DisconnectCommandTest.cpp \
  activemq/connector/stomp/commands/ErrorCommandTest.cpp \
  activemq/connector/stomp/commands/MessageCommandTest.cpp \
  activemq/connector/stomp/commands/ReceiptCommandTest.cpp \
  activemq/connector/stomp/commands/SubscribeCommandTest.cpp \
  activemq/connector/stomp/commands/TextMessageCommandTest.cpp \
  activemq/connector/stomp/commands/UnsubscribeCommandTest.cpp \
  activemq/connector/stomp/marshal/MarshalerTest.cpp \
  activemq/connector/stomp/StompCommandReaderTest.cpp \
  activemq/connector/stomp/StompCommandWriterTest.cpp \
  activemq/connector/stomp/StompConnectorTest.cpp \
  activemq/connector/stomp/StompFrameTest.cpp \
  activemq/connector/stomp/StompSessionManagerTest.cpp \
  activemq/connector/openwire/marshal/PrimitiveMapMarshallerTest.cpp \
  activemq/connector/openwire/marshal/BaseDataStreamMarshallerTest.cpp \
  activemq/connector/openwire/utils/HexTableTest.cpp \
  activemq/connector/openwire/utils/BooleanStreamTest.cpp \
  activemq/connector/openwire/utils/OpenwireStringSupportTest.cpp \
  activemq/connector/openwire/commands/ActiveMQDestinationTest2.cpp \
  activemq/core/ActiveMQConnectionFactoryTest.cpp \
  activemq/core/ActiveMQConnectionTest.cpp \
  activemq/core/ActiveMQDestinationTest.cpp \
  activemq/core/ActiveMQSessionTest.cpp \
  activemq/exceptions/ActiveMQExceptionTest.cpp \
  activemq/io/BufferedInputStreamTest.cpp \
  activemq/io/BufferedOutputStreamTest.cpp \
  activemq/io/ByteArrayInputStreamTest.cpp \
  activemq/io/ByteArrayOutputStreamTest.cpp \
  activemq/io/DataInputStreamTest.cpp \
  activemq/io/DataOutputStreamTest.cpp \
  activemq/logger/LoggerTest.cpp \
  activemq/network/SocketFactoryTest.cpp \
  activemq/network/SocketTest.cpp \
  activemq/transport/DummyTransportFactory.cpp \
  activemq/transport/IOTransportTest.cpp \
  activemq/transport/ResponseCorrelatorTest.cpp \
  activemq/transport/TransportFactoryMapRegistrarTest.cpp \
  activemq/transport/TransportFactoryMapTest.cpp \
  activemq/util/BooleanTest.cpp \
  activemq/util/DateTest.cpp \
  activemq/util/GuidTest.cpp \
  activemq/util/IntegerTest.cpp \
  activemq/util/LongTest.cpp \
  activemq/util/MapTest.cpp \
  activemq/util/PrimitiveMapTest.cpp \
  activemq/util/QueueTest.cpp \
  activemq/util/StringTokenizerTest.cpp \
  activemq/util/URISupportTest.cpp \
  main.cpp

## Compile this as part of make check
check_PROGRAMS = activemq-test
## Also run the tests as part of make check
TESTS = $(check_PROGRAMS)

## 
## Compiler/Linker Options
##
CXXFLAGS = @CXXFLAGS@ -Wno-non-virtual-dtor -Wno-unused-parameter -Wno-uninitialized
activemq_test_SOURCES = $(cc_sources)
activemq_test_CXXFLAGS = -I$(srcdir)/../main @CPPUNIT_CFLAGS@
activemq_test_LDADD= ../main/libactivemq-cpp.a @CPPUNIT_LIBS@

