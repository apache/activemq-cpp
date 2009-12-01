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
    activemq/cmsutil/CmsAccessorTest.cpp \
    activemq/cmsutil/CmsDestinationAccessorTest.cpp \
    activemq/cmsutil/CmsTemplateTest.cpp \
    activemq/cmsutil/DynamicDestinationResolverTest.cpp \
    activemq/cmsutil/SessionPoolTest.cpp \
    activemq/commands/ActiveMQBytesMessageTest.cpp \
    activemq/commands/ActiveMQDestinationTest2.cpp \
    activemq/commands/ActiveMQMapMessageTest.cpp \
    activemq/commands/ActiveMQMessageTest.cpp \
    activemq/commands/ActiveMQQueueTest.cpp \
    activemq/commands/ActiveMQStreamMessageTest.cpp \
    activemq/commands/ActiveMQTempQueueTest.cpp \
    activemq/commands/ActiveMQTempTopicTest.cpp \
    activemq/commands/ActiveMQTextMessageTest.cpp \
    activemq/commands/ActiveMQTopicTest.cpp \
    activemq/commands/BrokerIdTest.cpp \
    activemq/commands/BrokerInfoTest.cpp \
    activemq/core/ActiveMQConnectionFactoryTest.cpp \
    activemq/core/ActiveMQConnectionTest.cpp \
    activemq/core/ActiveMQSessionTest.cpp \
    activemq/core/MessageDispatchChannelTest.cpp \
    activemq/exceptions/ActiveMQExceptionTest.cpp \
    activemq/state/ConnectionStateTest.cpp \
    activemq/state/ConnectionStateTrackerTest.cpp \
    activemq/state/ConsumerStateTest.cpp \
    activemq/state/ProducerStateTest.cpp \
    activemq/state/SessionStateTest.cpp \
    activemq/state/TransactionStateTest.cpp \
    activemq/threads/CompositeTaskRunnerTest.cpp \
    activemq/threads/DedicatedTaskRunnerTest.cpp \
    activemq/transport/IOTransportTest.cpp \
    activemq/transport/TransportRegistryTest.cpp \
    activemq/transport/correlator/ResponseCorrelatorTest.cpp \
    activemq/transport/failover/FailoverTransportTest.cpp \
    activemq/transport/inactivity/InactivityMonitorTest.cpp \
    activemq/transport/mock/MockTransportFactoryTest.cpp \
    activemq/util/LongSequenceGeneratorTest.cpp \
    activemq/util/MemoryUsageTest.cpp \
    activemq/util/PrimitiveListTest.cpp \
    activemq/util/PrimitiveMapTest.cpp \
    activemq/util/PrimitiveValueConverterTest.cpp \
    activemq/util/PrimitiveValueNodeTest.cpp \
    activemq/util/URISupportTest.cpp \
    activemq/wireformat/WireFormatRegistryTest.cpp \
    activemq/wireformat/openwire/OpenWireFormatTest.cpp \
    activemq/wireformat/openwire/marshal/BaseDataStreamMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/PrimitiveTypesMarshallerTest.cpp \
    activemq/wireformat/openwire/utils/BooleanStreamTest.cpp \
    activemq/wireformat/openwire/utils/HexTableTest.cpp \
    activemq/wireformat/openwire/utils/MessagePropertyInterceptorTest.cpp \
    activemq/wireformat/openwire/utils/OpenwireStringSupportTest.cpp \
    decaf/internal/net/URIEncoderDecoderTest.cpp \
    decaf/internal/net/URIHelperTest.cpp \
    decaf/internal/nio/BufferFactoryTest.cpp \
    decaf/internal/nio/ByteArrayBufferTest.cpp \
    decaf/internal/nio/ByteArrayPerspectiveTest.cpp \
    decaf/internal/nio/CharArrayBufferTest.cpp \
    decaf/internal/nio/DoubleArrayBufferTest.cpp \
    decaf/internal/nio/FloatArrayBufferTest.cpp \
    decaf/internal/nio/IntArrayBufferTest.cpp \
    decaf/internal/nio/LongArrayBufferTest.cpp \
    decaf/internal/nio/ShortArrayBufferTest.cpp \
    decaf/internal/util/ByteArrayAdapterTest.cpp \
    decaf/internal/util/TimerTaskHeapTest.cpp \
    decaf/internal/util/concurrent/TransferQueueTest.cpp \
    decaf/internal/util/concurrent/TransferStackTest.cpp \
    decaf/io/BufferedInputStreamTest.cpp \
    decaf/io/BufferedOutputStreamTest.cpp \
    decaf/io/ByteArrayInputStreamTest.cpp \
    decaf/io/ByteArrayOutputStreamTest.cpp \
    decaf/io/DataInputStreamTest.cpp \
    decaf/io/DataOutputStreamTest.cpp \
    decaf/io/FilterInputStreamTest.cpp \
    decaf/io/FilterOutputStreamTest.cpp \
    decaf/lang/BooleanTest.cpp \
    decaf/lang/ByteTest.cpp \
    decaf/lang/CharacterTest.cpp \
    decaf/lang/DoubleTest.cpp \
    decaf/lang/ExceptionTest.cpp \
    decaf/lang/FloatTest.cpp \
    decaf/lang/IntegerTest.cpp \
    decaf/lang/LongTest.cpp \
    decaf/lang/MathTest.cpp \
    decaf/lang/PointerTest.cpp \
    decaf/lang/ShortTest.cpp \
    decaf/lang/SystemTest.cpp \
    decaf/lang/ThreadTest.cpp \
    decaf/net/SocketFactoryTest.cpp \
    decaf/net/SocketTest.cpp \
    decaf/net/URISyntaxExceptionTest.cpp \
    decaf/net/URITest.cpp \
    decaf/net/URLDecoderTest.cpp \
    decaf/net/URLEncoderTest.cpp \
    decaf/nio/BufferTest.cpp \
    decaf/util/DateTest.cpp \
    decaf/util/Endian.cpp \
    decaf/util/ListTest.cpp \
    decaf/util/PriorityQueueTest.cpp \
    decaf/util/PropertiesTest.cpp \
    decaf/util/QueueTest.cpp \
    decaf/util/RandomTest.cpp \
    decaf/util/SetTest.cpp \
    decaf/util/StlMapTest.cpp \
    decaf/util/StringTokenizerTest.cpp \
    decaf/util/TimerTest.cpp \
    decaf/util/UUIDTest.cpp \
    decaf/util/concurrent/ConcurrentStlMapTest.cpp \
    decaf/util/concurrent/CountDownLatchTest.cpp \
    decaf/util/concurrent/MutexTest.cpp \
    decaf/util/concurrent/SynchronousQueueTest.cpp \
    decaf/util/concurrent/ThreadPoolTest.cpp \
    decaf/util/concurrent/TimeUnitTest.cpp \
    decaf/util/concurrent/atomic/AtomicBooleanTest.cpp \
    decaf/util/concurrent/atomic/AtomicIntegerTest.cpp \
    decaf/util/concurrent/atomic/AtomicReferenceTest.cpp \
    decaf/util/concurrent/locks/LockSupportTest.cpp \
    main.cpp \
    testRegistry.cpp \
    util/teamcity/TeamCityProgressListener.cpp


h_sources = \
    activemq/cmsutil/CmsAccessorTest.h \
    activemq/cmsutil/CmsDestinationAccessorTest.h \
    activemq/cmsutil/CmsTemplateTest.h \
    activemq/cmsutil/DummyConnection.h \
    activemq/cmsutil/DummyConnectionFactory.h \
    activemq/cmsutil/DummyConsumer.h \
    activemq/cmsutil/DummyMessage.h \
    activemq/cmsutil/DummyMessageCreator.h \
    activemq/cmsutil/DummyProducer.h \
    activemq/cmsutil/DummySession.h \
    activemq/cmsutil/DynamicDestinationResolverTest.h \
    activemq/cmsutil/MessageContext.h \
    activemq/cmsutil/SessionPoolTest.h \
    activemq/commands/ActiveMQBytesMessageTest.h \
    activemq/commands/ActiveMQDestinationTest2.h \
    activemq/commands/ActiveMQMapMessageTest.h \
    activemq/commands/ActiveMQMessageTest.h \
    activemq/commands/ActiveMQQueueTest.h \
    activemq/commands/ActiveMQStreamMessageTest.h \
    activemq/commands/ActiveMQTempQueueTest.h \
    activemq/commands/ActiveMQTempTopicTest.h \
    activemq/commands/ActiveMQTextMessageTest.h \
    activemq/commands/ActiveMQTopicTest.h \
    activemq/commands/BrokerIdTest.h \
    activemq/commands/BrokerInfoTest.h \
    activemq/core/ActiveMQConnectionFactoryTest.h \
    activemq/core/ActiveMQConnectionTest.h \
    activemq/core/ActiveMQSessionTest.h \
    activemq/core/MessageDispatchChannelTest.h \
    activemq/exceptions/ActiveMQExceptionTest.h \
    activemq/state/ConnectionStateTest.h \
    activemq/state/ConnectionStateTrackerTest.h \
    activemq/state/ConsumerStateTest.h \
    activemq/state/ProducerStateTest.h \
    activemq/state/SessionStateTest.h \
    activemq/state/TransactionStateTest.h \
    activemq/threads/CompositeTaskRunnerTest.h \
    activemq/threads/DedicatedTaskRunnerTest.h \
    activemq/transport/IOTransportTest.h \
    activemq/transport/TransportRegistryTest.h \
    activemq/transport/correlator/ResponseCorrelatorTest.h \
    activemq/transport/failover/FailoverTransportTest.h \
    activemq/transport/inactivity/InactivityMonitorTest.h \
    activemq/transport/mock/MockTransportFactoryTest.h \
    activemq/util/LongSequenceGeneratorTest.h \
    activemq/util/MemoryUsageTest.h \
    activemq/util/PrimitiveListTest.h \
    activemq/util/PrimitiveMapTest.h \
    activemq/util/PrimitiveValueConverterTest.h \
    activemq/util/PrimitiveValueNodeTest.h \
    activemq/util/URISupportTest.h \
    activemq/wireformat/WireFormatRegistryTest.h \
    activemq/wireformat/openwire/OpenWireFormatTest.h \
    activemq/wireformat/openwire/marshal/BaseDataStreamMarshallerTest.h \
    activemq/wireformat/openwire/marshal/PrimitiveTypesMarshallerTest.h \
    activemq/wireformat/openwire/utils/BooleanStreamTest.h \
    activemq/wireformat/openwire/utils/HexTableTest.h \
    activemq/wireformat/openwire/utils/MessagePropertyInterceptorTest.h \
    activemq/wireformat/openwire/utils/OpenwireStringSupportTest.h \
    decaf/internal/net/URIEncoderDecoderTest.h \
    decaf/internal/net/URIHelperTest.h \
    decaf/internal/nio/BufferFactoryTest.h \
    decaf/internal/nio/ByteArrayBufferTest.h \
    decaf/internal/nio/ByteArrayPerspectiveTest.h \
    decaf/internal/nio/CharArrayBufferTest.h \
    decaf/internal/nio/DoubleArrayBufferTest.h \
    decaf/internal/nio/FloatArrayBufferTest.h \
    decaf/internal/nio/IntArrayBufferTest.h \
    decaf/internal/nio/LongArrayBufferTest.h \
    decaf/internal/nio/ShortArrayBufferTest.h \
    decaf/internal/util/ByteArrayAdapterTest.h \
    decaf/internal/util/TimerTaskHeapTest.h \
    decaf/internal/util/concurrent/TransferQueueTest.h \
    decaf/internal/util/concurrent/TransferStackTest.h \
    decaf/io/BufferedInputStreamTest.h \
    decaf/io/BufferedOutputStreamTest.h \
    decaf/io/ByteArrayInputStreamTest.h \
    decaf/io/ByteArrayOutputStreamTest.h \
    decaf/io/DataInputStreamTest.h \
    decaf/io/DataOutputStreamTest.h \
    decaf/io/FilterInputStreamTest.h \
    decaf/io/FilterOutputStreamTest.h \
    decaf/lang/BooleanTest.h \
    decaf/lang/ByteTest.h \
    decaf/lang/CharacterTest.h \
    decaf/lang/DoubleTest.h \
    decaf/lang/ExceptionTest.h \
    decaf/lang/FloatTest.h \
    decaf/lang/IntegerTest.h \
    decaf/lang/LongTest.h \
    decaf/lang/MathTest.h \
    decaf/lang/PointerTest.h \
    decaf/lang/ShortTest.h \
    decaf/lang/SystemTest.h \
    decaf/lang/ThreadTest.h \
    decaf/net/SocketFactoryTest.h \
    decaf/net/SocketTest.h \
    decaf/net/URISyntaxExceptionTest.h \
    decaf/net/URITest.h \
    decaf/net/URLDecoderTest.h \
    decaf/net/URLEncoderTest.h \
    decaf/nio/BufferTest.h \
    decaf/util/DateTest.h \
    decaf/util/Endian.h \
    decaf/util/ListTest.h \
    decaf/util/PriorityQueueTest.h \
    decaf/util/PropertiesTest.h \
    decaf/util/QueueTest.h \
    decaf/util/RandomTest.h \
    decaf/util/SetTest.h \
    decaf/util/StlMapTest.h \
    decaf/util/StringTokenizerTest.h \
    decaf/util/TimerTest.h \
    decaf/util/UUIDTest.h \
    decaf/util/concurrent/ConcurrentStlMapTest.h \
    decaf/util/concurrent/CountDownLatchTest.h \
    decaf/util/concurrent/MutexTest.h \
    decaf/util/concurrent/SynchronousQueueTest.h \
    decaf/util/concurrent/ThreadPoolTest.h \
    decaf/util/concurrent/TimeUnitTest.h \
    decaf/util/concurrent/atomic/AtomicBooleanTest.h \
    decaf/util/concurrent/atomic/AtomicIntegerTest.h \
    decaf/util/concurrent/atomic/AtomicReferenceTest.h \
    decaf/util/concurrent/locks/LockSupportTest.h \
    util/teamcity/TeamCityProgressListener.h


include activemq/wireformat/openwire/marshal/v1/srcmakefile.mk
include activemq/wireformat/openwire/marshal/v2/srcmakefile.mk
include activemq/wireformat/openwire/marshal/v3/srcmakefile.mk
include activemq/wireformat/openwire/marshal/v4/srcmakefile.mk
include activemq/wireformat/openwire/marshal/v5/srcmakefile.mk

## Compile this as part of make check
check_PROGRAMS = activemq-test

## Also run the tests as part of make check
## TESTS = $(check_PROGRAMS)

## 
## Compiler/Linker Options
##
activemq_test_SOURCES = $(cc_sources)
activemq_test_CXXFLAGS = $(AMQ_TEST_CXXFLAGS) -I$(srcdir)/../main @CPPUNIT_CFLAGS@
activemq_test_LDADD= $(AMQ_TEST_LIBS) @CPPUNIT_LIBS@ 
