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

// All CPP Unit tests are registered in here so we can disable them and
// enable them easily in one place.

#include <activemq/commands/BrokerInfoTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::commands::BrokerInfoTest );
#include <activemq/commands/BrokerIdTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::commands::BrokerIdTest );
#include <activemq/commands/ActiveMQTopicTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::commands::ActiveMQTopicTest );
#include <activemq/commands/ActiveMQTextMessageTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::commands::ActiveMQTextMessageTest );
#include <activemq/commands/ActiveMQTempTopicTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::commands::ActiveMQTempTopicTest );
#include <activemq/commands/ActiveMQTempQueueTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::commands::ActiveMQTempQueueTest );
#include <activemq/commands/ActiveMQQueueTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::commands::ActiveMQQueueTest );
#include <activemq/commands/ActiveMQMessageTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::commands::ActiveMQMessageTest );
#include <activemq/commands/ActiveMQMapMessageTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::commands::ActiveMQMapMessageTest );
#include <activemq/commands/ActiveMQDestinationTest2.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::commands::ActiveMQDestinationTest );
#include <activemq/commands/ActiveMQBytesMessageTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::commands::ActiveMQBytesMessageTest );
#include <activemq/commands/ActiveMQStreamMessageTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::commands::ActiveMQStreamMessageTest );

#include <activemq/wireformat/openwire/marshal/BaseDataStreamMarshallerTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::BaseDataStreamMarshallerTest );
#include <activemq/wireformat/openwire/marshal/PrimitiveTypesMarshallerTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::PrimitiveTypesMarshallerTest );

#include <activemq/wireformat/openwire/utils/BooleanStreamTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::utils::BooleanStreamTest );
#include <activemq/wireformat/openwire/utils/HexTableTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::utils::HexTableTest );
#include <activemq/wireformat/openwire/utils/OpenwireStringSupportTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::utils::OpenwireStringSupportTest );
#include <activemq/wireformat/openwire/utils/MessagePropertyInterceptorTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::utils::MessagePropertyInterceptorTest );

#include <activemq/wireformat/openwire/OpenWireFormatTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::OpenWireFormatTest );

#include <activemq/cmsutil/CmsAccessorTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::cmsutil::CmsAccessorTest );
#include <activemq/cmsutil/CmsDestinationAccessorTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::cmsutil::CmsDestinationAccessorTest );
#include <activemq/cmsutil/CmsTemplateTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::cmsutil::CmsTemplateTest );
#include <activemq/cmsutil/DynamicDestinationResolverTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::cmsutil::DynamicDestinationResolverTest );
#include <activemq/cmsutil/SessionPoolTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::cmsutil::SessionPoolTest );

#include <activemq/core/ActiveMQConnectionFactoryTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::core::ActiveMQConnectionFactoryTest );
#include <activemq/core/ActiveMQConnectionTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::core::ActiveMQConnectionTest );
#include <activemq/core/ActiveMQSessionTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::core::ActiveMQSessionTest );
#include <activemq/core/MessageDispatchChannelTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::core::MessageDispatchChannelTest );

#include <activemq/state/ConnectionStateTrackerTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::state::ConnectionStateTrackerTest );
#include <activemq/state/ConnectionStateTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::state::ConnectionStateTest );
#include <activemq/state/ConsumerStateTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::state::ConsumerStateTest );
#include <activemq/state/ProducerStateTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::state::ProducerStateTest );
#include <activemq/state/SessionStateTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::state::SessionStateTest );
#include <activemq/state/TransactionStateTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::state::TransactionStateTest );

#include <activemq/transport/failover/FailoverTransportTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::transport::failover::FailoverTransportTest );

#include <activemq/transport/correlator/ResponseCorrelatorTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::transport::correlator::ResponseCorrelatorTest );

#include <activemq/transport/mock/MockTransportFactoryTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::transport::mock::MockTransportFactoryTest );

#include <activemq/transport/inactivity/InactivityMonitorTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::transport::inactivity::InactivityMonitorTest );

#include <activemq/transport/TransportRegistryTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::transport::TransportRegistryTest );
#include <activemq/transport/IOTransportTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::transport::IOTransportTest );

#include <activemq/exceptions/ActiveMQExceptionTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::exceptions::ActiveMQExceptionTest );

#include <activemq/util/LongSequenceGeneratorTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::util::LongSequenceGeneratorTest );
#include <activemq/util/PrimitiveValueNodeTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::util::PrimitiveValueNodeTest );
#include <activemq/util/PrimitiveListTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::util::PrimitiveListTest );
#include <activemq/util/PrimitiveMapTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::util::PrimitiveMapTest );
#include <activemq/util/PrimitiveValueConverterTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::util::PrimitiveValueConverterTest );
#include <activemq/util/URISupportTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::util::URISupportTest );
#include <activemq/util/MemoryUsageTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::util::MemoryUsageTest );

#include <activemq/threads/DedicatedTaskRunnerTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::threads::DedicatedTaskRunnerTest );
#include <activemq/threads/CompositeTaskRunnerTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::threads::CompositeTaskRunnerTest );

#include <activemq/wireformat/WireFormatRegistryTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::WireFormatRegistryTest );

#include <decaf/internal/util/ByteArrayAdapterTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::internal::util::ByteArrayAdapterTest );
#include <decaf/internal/util/TimerTaskHeapTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::internal::util::TimerTaskHeapTest );

#include <decaf/internal/nio/ByteArrayPerspectiveTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::internal::nio::ByteArrayPerspectiveTest );
#include <decaf/internal/nio/ByteArrayBufferTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::internal::nio::ByteArrayBufferTest );
#include <decaf/internal/nio/BufferFactoryTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::internal::nio::BufferFactoryTest );
#include <decaf/internal/nio/CharArrayBufferTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::internal::nio::CharArrayBufferTest );
#include <decaf/internal/nio/DoubleArrayBufferTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::internal::nio::DoubleArrayBufferTest );
#include <decaf/internal/nio/FloatArrayBufferTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::internal::nio::FloatArrayBufferTest );
#include <decaf/internal/nio/LongArrayBufferTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::internal::nio::LongArrayBufferTest );
#include <decaf/internal/nio/IntArrayBufferTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::internal::nio::IntArrayBufferTest );
#include <decaf/internal/nio/ShortArrayBufferTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::internal::nio::ShortArrayBufferTest );

#include <decaf/internal/net/URIEncoderDecoderTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::internal::net::URIEncoderDecoderTest );
#include <decaf/internal/net/URIHelperTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::internal::net::URIHelperTest );

#include <decaf/nio/BufferTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::nio::BufferTest );

#include <decaf/io/InputStreamTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::io::InputStreamTest );
#include <decaf/io/OutputStreamTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::io::OutputStreamTest );
#include <decaf/io/FilterInputStreamTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::io::FilterInputStreamTest );
#include <decaf/io/FilterOutputStreamTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::io::FilterOutputStreamTest );
#include <decaf/io/BufferedInputStreamTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::io::BufferedInputStreamTest );
#include <decaf/io/BufferedOutputStreamTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::io::BufferedOutputStreamTest );
#include <decaf/io/ByteArrayInputStreamTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::io::ByteArrayInputStreamTest );
#include <decaf/io/ByteArrayOutputStreamTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::io::ByteArrayOutputStreamTest );
#include <decaf/io/PushbackInputStreamTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::io::PushbackInputStreamTest );
#include <decaf/io/DataInputStreamTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::io::DataInputStreamTest );
#include <decaf/io/DataOutputStreamTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::io::DataOutputStreamTest );
#include <decaf/io/WriterTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::io::WriterTest );
#include <decaf/io/ReaderTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::io::ReaderTest );
#include <decaf/io/OutputStreamWriterTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::io::OutputStreamWriterTest );
#include <decaf/io/InputStreamReaderTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::io::InputStreamReaderTest );

#include <decaf/lang/MathTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::lang::MathTest );
#include <decaf/lang/ByteTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::lang::ByteTest );
#include <decaf/lang/CharacterTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::lang::CharacterTest );
#include <decaf/lang/BooleanTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::lang::BooleanTest );
#include <decaf/lang/ShortTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::lang::ShortTest );
#include <decaf/lang/IntegerTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::lang::IntegerTest );
#include <decaf/lang/LongTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::lang::LongTest );
#include <decaf/lang/FloatTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::lang::FloatTest );
#include <decaf/lang/DoubleTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::lang::DoubleTest );
#include <decaf/lang/ExceptionTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::lang::ExceptionTest );
#include <decaf/lang/ThreadTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::lang::ThreadTest );
#include <decaf/lang/SystemTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::lang::SystemTest );
#include <decaf/lang/PointerTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::lang::PointerTest );

#include <decaf/net/SocketFactoryTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::net::SocketFactoryTest );
#include <decaf/net/SocketTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::net::SocketTest );
#include <decaf/net/URITest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::net::URITest );
#include <decaf/net/URISyntaxExceptionTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::net::URISyntaxExceptionTest );
#include <decaf/net/URLEncoderTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::net::URLEncoderTest );
#include <decaf/net/URLDecoderTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::net::URLDecoderTest );

#include <decaf/util/concurrent/ConcurrentStlMapTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::util::concurrent::ConcurrentStlMapTest );
#include <decaf/util/concurrent/CountDownLatchTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::util::concurrent::CountDownLatchTest );
#include <decaf/util/concurrent/MutexTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::util::concurrent::MutexTest );
#include <decaf/util/concurrent/ThreadPoolTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::util::concurrent::ThreadPoolTest );
#include <decaf/util/concurrent/TimeUnitTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::util::concurrent::TimeUnitTest );

#include <decaf/util/concurrent/atomic/AtomicBooleanTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::util::concurrent::atomic::AtomicBooleanTest );
#include <decaf/util/concurrent/atomic/AtomicIntegerTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::util::concurrent::atomic::AtomicIntegerTest );
#include <decaf/util/concurrent/atomic/AtomicReferenceTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::util::concurrent::atomic::AtomicReferenceTest );

#include <decaf/util/concurrent/locks/LockSupportTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::util::concurrent::locks::LockSupportTest );

#include <decaf/util/DateTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::util::DateTest );
#include <decaf/util/UUIDTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::util::UUIDTest );
#include <decaf/util/ListTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::util::ListTest );
#include <decaf/util/StlMapTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::util::StlMapTest );
#include <decaf/util/PropertiesTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::util::PropertiesTest );
#include <decaf/util/QueueTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::util::QueueTest );
#include <decaf/util/RandomTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::util::RandomTest );
#include <decaf/util/SetTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::util::SetTest );
#include <decaf/util/StringTokenizerTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::util::StringTokenizerTest );
#include <decaf/util/TimerTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::util::TimerTest );
#include <decaf/util/PriorityQueueTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::util::PriorityQueueTest );

#include <decaf/util/zip/DeflaterTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::util::zip::DeflaterTest );
#include <decaf/util/zip/InflaterTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::util::zip::InflaterTest );
#include <decaf/util/zip/Adler32Test.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::util::zip::Adler32Test );
#include <decaf/util/zip/CRC32Test.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::util::zip::CRC32Test );
#include <decaf/util/zip/CheckedInputStreamTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::util::zip::CheckedInputStreamTest );
#include <decaf/util/zip/CheckedOutputStreamTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::util::zip::CheckedOutputStreamTest );
#include <decaf/util/zip/DeflaterOutputStreamTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::util::zip::DeflaterOutputStreamTest );
#include <decaf/util/zip/InflaterInputStreamTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::util::zip::InflaterInputStreamTest );

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Marshaler Tests
//
//#include <activemq/wireformat/openwire/marshal/v1/ActiveMQBlobMessageMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v1::ActiveMQBlobMessageMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v1/ActiveMQBytesMessageMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v1::ActiveMQBytesMessageMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v1/ActiveMQMapMessageMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v1::ActiveMQMapMessageMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v1/ActiveMQMessageMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v1::ActiveMQMessageMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v1/ActiveMQObjectMessageMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v1::ActiveMQObjectMessageMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v1/ActiveMQQueueMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v1::ActiveMQQueueMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v1/ActiveMQStreamMessageMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v1::ActiveMQStreamMessageMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v1/ActiveMQTempQueueMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v1::ActiveMQTempQueueMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v1/ActiveMQTempTopicMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v1::ActiveMQTempTopicMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v1/ActiveMQTextMessageMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v1::ActiveMQTextMessageMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v1/ActiveMQTopicMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v1::ActiveMQTopicMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v1/BrokerIdMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v1::BrokerIdMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v1/BrokerInfoMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v1::BrokerInfoMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v1/ConnectionControlMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v1::ConnectionControlMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v1/ConnectionErrorMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v1::ConnectionErrorMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v1/ConnectionIdMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v1::ConnectionIdMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v1/ConnectionInfoMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v1::ConnectionInfoMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v1/ConsumerControlMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v1::ConsumerControlMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v1/ConsumerIdMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v1::ConsumerIdMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v1/ConsumerInfoMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v1::ConsumerInfoMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v1/ControlCommandMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v1::ControlCommandMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v1/DataArrayResponseMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v1::DataArrayResponseMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v1/DataResponseMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v1::DataResponseMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v1/DestinationInfoMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v1::DestinationInfoMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v1/DiscoveryEventMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v1::DiscoveryEventMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v1/ExceptionResponseMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v1::ExceptionResponseMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v1/FlushCommandMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v1::FlushCommandMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v1/IntegerResponseMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v1::IntegerResponseMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v1/JournalQueueAckMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v1::JournalQueueAckMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v1/JournalTopicAckMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v1::JournalTopicAckMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v1/JournalTraceMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v1::JournalTraceMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v1/JournalTransactionMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v1::JournalTransactionMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v1/KeepAliveInfoMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v1::KeepAliveInfoMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v1/LastPartialCommandMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v1::LastPartialCommandMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v1/LocalTransactionIdMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v1::LocalTransactionIdMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v1/MessageAckMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v1::MessageAckMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v1/MessageDispatchMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v1::MessageDispatchMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v1/MessageDispatchNotificationMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v1::MessageDispatchNotificationMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v1/MessageIdMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v1::MessageIdMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v1/MessagePullMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v1::MessagePullMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v1/NetworkBridgeFilterMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v1::NetworkBridgeFilterMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v1/PartialCommandMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v1::PartialCommandMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v1/ProducerAckMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v1::ProducerAckMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v1/ProducerIdMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v1::ProducerIdMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v1/ProducerInfoMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v1::ProducerInfoMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v1/RemoveInfoMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v1::RemoveInfoMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v1/RemoveSubscriptionInfoMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v1::RemoveSubscriptionInfoMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v1/ReplayCommandMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v1::ReplayCommandMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v1/ResponseMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v1::ResponseMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v1/SessionIdMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v1::SessionIdMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v1/SessionInfoMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v1::SessionInfoMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v1/ShutdownInfoMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v1::ShutdownInfoMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v1/SubscriptionInfoMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v1::SubscriptionInfoMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v1/TransactionInfoMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v1::TransactionInfoMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v1/WireFormatInfoMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v1::WireFormatInfoMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v1/XATransactionIdMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v1::XATransactionIdMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v2/ActiveMQBlobMessageMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v2::ActiveMQBlobMessageMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v2/ActiveMQBytesMessageMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v2::ActiveMQBytesMessageMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v2/ActiveMQMapMessageMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v2::ActiveMQMapMessageMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v2/ActiveMQMessageMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v2::ActiveMQMessageMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v2/ActiveMQObjectMessageMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v2::ActiveMQObjectMessageMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v2/ActiveMQQueueMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v2::ActiveMQQueueMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v2/ActiveMQStreamMessageMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v2::ActiveMQStreamMessageMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v2/ActiveMQTempQueueMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v2::ActiveMQTempQueueMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v2/ActiveMQTempTopicMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v2::ActiveMQTempTopicMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v2/ActiveMQTextMessageMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v2::ActiveMQTextMessageMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v2/ActiveMQTopicMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v2::ActiveMQTopicMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v2/BrokerIdMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v2::BrokerIdMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v2/BrokerInfoMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v2::BrokerInfoMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v2/ConnectionControlMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v2::ConnectionControlMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v2/ConnectionErrorMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v2::ConnectionErrorMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v2/ConnectionIdMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v2::ConnectionIdMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v2/ConnectionInfoMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v2::ConnectionInfoMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v2/ConsumerControlMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v2::ConsumerControlMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v2/ConsumerIdMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v2::ConsumerIdMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v2/ConsumerInfoMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v2::ConsumerInfoMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v2/ControlCommandMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v2::ControlCommandMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v2/DataArrayResponseMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v2::DataArrayResponseMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v2/DataResponseMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v2::DataResponseMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v2/DestinationInfoMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v2::DestinationInfoMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v2/DiscoveryEventMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v2::DiscoveryEventMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v2/ExceptionResponseMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v2::ExceptionResponseMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v2/FlushCommandMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v2::FlushCommandMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v2/IntegerResponseMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v2::IntegerResponseMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v2/JournalQueueAckMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v2::JournalQueueAckMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v2/JournalTopicAckMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v2::JournalTopicAckMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v2/JournalTraceMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v2::JournalTraceMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v2/JournalTransactionMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v2::JournalTransactionMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v2/KeepAliveInfoMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v2::KeepAliveInfoMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v2/LastPartialCommandMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v2::LastPartialCommandMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v2/LocalTransactionIdMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v2::LocalTransactionIdMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v2/MessageAckMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v2::MessageAckMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v2/MessageDispatchMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v2::MessageDispatchMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v2/MessageDispatchNotificationMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v2::MessageDispatchNotificationMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v2/MessageIdMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v2::MessageIdMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v2/MessagePullMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v2::MessagePullMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v2/NetworkBridgeFilterMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v2::NetworkBridgeFilterMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v2/PartialCommandMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v2::PartialCommandMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v2/ProducerAckMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v2::ProducerAckMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v2/ProducerIdMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v2::ProducerIdMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v2/ProducerInfoMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v2::ProducerInfoMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v2/RemoveInfoMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v2::RemoveInfoMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v2/RemoveSubscriptionInfoMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v2::RemoveSubscriptionInfoMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v2/ReplayCommandMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v2::ReplayCommandMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v2/ResponseMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v2::ResponseMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v2/SessionIdMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v2::SessionIdMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v2/SessionInfoMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v2::SessionInfoMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v2/ShutdownInfoMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v2::ShutdownInfoMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v2/SubscriptionInfoMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v2::SubscriptionInfoMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v2/TransactionInfoMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v2::TransactionInfoMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v2/WireFormatInfoMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v2::WireFormatInfoMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v2/XATransactionIdMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v2::XATransactionIdMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v3/ActiveMQBlobMessageMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v3::ActiveMQBlobMessageMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v3/ActiveMQBytesMessageMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v3::ActiveMQBytesMessageMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v3/ActiveMQMapMessageMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v3::ActiveMQMapMessageMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v3/ActiveMQMessageMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v3::ActiveMQMessageMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v3/ActiveMQObjectMessageMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v3::ActiveMQObjectMessageMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v3/ActiveMQQueueMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v3::ActiveMQQueueMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v3/ActiveMQStreamMessageMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v3::ActiveMQStreamMessageMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v3/ActiveMQTempQueueMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v3::ActiveMQTempQueueMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v3/ActiveMQTempTopicMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v3::ActiveMQTempTopicMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v3/ActiveMQTextMessageMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v3::ActiveMQTextMessageMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v3/ActiveMQTopicMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v3::ActiveMQTopicMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v3/BrokerIdMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v3::BrokerIdMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v3/BrokerInfoMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v3::BrokerInfoMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v3/ConnectionControlMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v3::ConnectionControlMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v3/ConnectionErrorMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v3::ConnectionErrorMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v3/ConnectionIdMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v3::ConnectionIdMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v3/ConnectionInfoMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v3::ConnectionInfoMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v3/ConsumerControlMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v3::ConsumerControlMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v3/ConsumerIdMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v3::ConsumerIdMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v3/ConsumerInfoMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v3::ConsumerInfoMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v3/ControlCommandMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v3::ControlCommandMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v3/DataArrayResponseMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v3::DataArrayResponseMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v3/DataResponseMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v3::DataResponseMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v3/DestinationInfoMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v3::DestinationInfoMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v3/DiscoveryEventMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v3::DiscoveryEventMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v3/ExceptionResponseMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v3::ExceptionResponseMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v3/FlushCommandMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v3::FlushCommandMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v3/IntegerResponseMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v3::IntegerResponseMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v3/JournalQueueAckMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v3::JournalQueueAckMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v3/JournalTopicAckMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v3::JournalTopicAckMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v3/JournalTraceMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v3::JournalTraceMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v3/JournalTransactionMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v3::JournalTransactionMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v3/KeepAliveInfoMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v3::KeepAliveInfoMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v3/LastPartialCommandMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v3::LastPartialCommandMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v3/LocalTransactionIdMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v3::LocalTransactionIdMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v3/MessageAckMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v3::MessageAckMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v3/MessageDispatchMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v3::MessageDispatchMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v3/MessageDispatchNotificationMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v3::MessageDispatchNotificationMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v3/MessageIdMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v3::MessageIdMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v3/MessagePullMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v3::MessagePullMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v3/NetworkBridgeFilterMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v3::NetworkBridgeFilterMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v3/PartialCommandMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v3::PartialCommandMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v3/ProducerAckMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v3::ProducerAckMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v3/ProducerIdMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v3::ProducerIdMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v3/ProducerInfoMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v3::ProducerInfoMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v3/RemoveInfoMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v3::RemoveInfoMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v3/RemoveSubscriptionInfoMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v3::RemoveSubscriptionInfoMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v3/ReplayCommandMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v3::ReplayCommandMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v3/ResponseMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v3::ResponseMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v3/SessionIdMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v3::SessionIdMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v3/SessionInfoMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v3::SessionInfoMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v3/ShutdownInfoMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v3::ShutdownInfoMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v3/SubscriptionInfoMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v3::SubscriptionInfoMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v3/TransactionInfoMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v3::TransactionInfoMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v3/WireFormatInfoMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v3::WireFormatInfoMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v3/XATransactionIdMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v3::XATransactionIdMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v4/ActiveMQBlobMessageMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v4::ActiveMQBlobMessageMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v4/ActiveMQBytesMessageMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v4::ActiveMQBytesMessageMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v4/ActiveMQMapMessageMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v4::ActiveMQMapMessageMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v4/ActiveMQMessageMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v4::ActiveMQMessageMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v4/ActiveMQObjectMessageMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v4::ActiveMQObjectMessageMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v4/ActiveMQQueueMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v4::ActiveMQQueueMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v4/ActiveMQStreamMessageMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v4::ActiveMQStreamMessageMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v4/ActiveMQTempQueueMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v4::ActiveMQTempQueueMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v4/ActiveMQTempTopicMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v4::ActiveMQTempTopicMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v4/ActiveMQTextMessageMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v4::ActiveMQTextMessageMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v4/ActiveMQTopicMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v4::ActiveMQTopicMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v4/BrokerIdMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v4::BrokerIdMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v4/BrokerInfoMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v4::BrokerInfoMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v4/ConnectionControlMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v4::ConnectionControlMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v4/ConnectionErrorMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v4::ConnectionErrorMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v4/ConnectionIdMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v4::ConnectionIdMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v4/ConnectionInfoMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v4::ConnectionInfoMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v4/ConsumerControlMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v4::ConsumerControlMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v4/ConsumerIdMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v4::ConsumerIdMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v4/ConsumerInfoMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v4::ConsumerInfoMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v4/ControlCommandMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v4::ControlCommandMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v4/DataArrayResponseMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v4::DataArrayResponseMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v4/DataResponseMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v4::DataResponseMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v4/DestinationInfoMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v4::DestinationInfoMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v4/DiscoveryEventMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v4::DiscoveryEventMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v4/ExceptionResponseMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v4::ExceptionResponseMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v4/FlushCommandMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v4::FlushCommandMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v4/IntegerResponseMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v4::IntegerResponseMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v4/JournalQueueAckMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v4::JournalQueueAckMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v4/JournalTopicAckMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v4::JournalTopicAckMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v4/JournalTraceMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v4::JournalTraceMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v4/JournalTransactionMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v4::JournalTransactionMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v4/KeepAliveInfoMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v4::KeepAliveInfoMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v4/LastPartialCommandMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v4::LastPartialCommandMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v4/LocalTransactionIdMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v4::LocalTransactionIdMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v4/MessageAckMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v4::MessageAckMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v4/MessageDispatchMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v4::MessageDispatchMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v4/MessageDispatchNotificationMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v4::MessageDispatchNotificationMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v4/MessageIdMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v4::MessageIdMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v4/MessagePullMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v4::MessagePullMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v4/NetworkBridgeFilterMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v4::NetworkBridgeFilterMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v4/PartialCommandMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v4::PartialCommandMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v4/ProducerAckMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v4::ProducerAckMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v4/ProducerIdMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v4::ProducerIdMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v4/ProducerInfoMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v4::ProducerInfoMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v4/RemoveInfoMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v4::RemoveInfoMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v4/RemoveSubscriptionInfoMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v4::RemoveSubscriptionInfoMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v4/ReplayCommandMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v4::ReplayCommandMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v4/ResponseMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v4::ResponseMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v4/SessionIdMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v4::SessionIdMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v4/SessionInfoMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v4::SessionInfoMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v4/ShutdownInfoMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v4::ShutdownInfoMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v4/SubscriptionInfoMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v4::SubscriptionInfoMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v4/TransactionInfoMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v4::TransactionInfoMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v4/WireFormatInfoMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v4::WireFormatInfoMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v4/XATransactionIdMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v4::XATransactionIdMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v5/ActiveMQBlobMessageMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v5::ActiveMQBlobMessageMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v5/ActiveMQBytesMessageMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v5::ActiveMQBytesMessageMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v5/ActiveMQMapMessageMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v5::ActiveMQMapMessageMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v5/ActiveMQMessageMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v5::ActiveMQMessageMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v5/ActiveMQObjectMessageMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v5::ActiveMQObjectMessageMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v5/ActiveMQQueueMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v5::ActiveMQQueueMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v5/ActiveMQStreamMessageMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v5::ActiveMQStreamMessageMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v5/ActiveMQTempQueueMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v5::ActiveMQTempQueueMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v5/ActiveMQTempTopicMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v5::ActiveMQTempTopicMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v5/ActiveMQTextMessageMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v5::ActiveMQTextMessageMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v5/ActiveMQTopicMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v5::ActiveMQTopicMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v5/BrokerIdMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v5::BrokerIdMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v5/BrokerInfoMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v5::BrokerInfoMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v5/ConnectionControlMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v5::ConnectionControlMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v5/ConnectionErrorMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v5::ConnectionErrorMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v5/ConnectionIdMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v5::ConnectionIdMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v5/ConnectionInfoMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v5::ConnectionInfoMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v5/ConsumerControlMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v5::ConsumerControlMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v5/ConsumerIdMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v5::ConsumerIdMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v5/ConsumerInfoMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v5::ConsumerInfoMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v5/ControlCommandMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v5::ControlCommandMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v5/DataArrayResponseMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v5::DataArrayResponseMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v5/DataResponseMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v5::DataResponseMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v5/DestinationInfoMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v5::DestinationInfoMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v5/DiscoveryEventMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v5::DiscoveryEventMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v5/ExceptionResponseMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v5::ExceptionResponseMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v5/FlushCommandMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v5::FlushCommandMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v5/IntegerResponseMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v5::IntegerResponseMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v5/JournalQueueAckMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v5::JournalQueueAckMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v5/JournalTopicAckMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v5::JournalTopicAckMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v5/JournalTraceMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v5::JournalTraceMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v5/JournalTransactionMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v5::JournalTransactionMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v5/KeepAliveInfoMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v5::KeepAliveInfoMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v5/LastPartialCommandMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v5::LastPartialCommandMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v5/LocalTransactionIdMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v5::LocalTransactionIdMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v5/MessageAckMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v5::MessageAckMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v5/MessageDispatchMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v5::MessageDispatchMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v5/MessageDispatchNotificationMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v5::MessageDispatchNotificationMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v5/MessageIdMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v5::MessageIdMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v5/MessagePullMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v5::MessagePullMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v5/NetworkBridgeFilterMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v5::NetworkBridgeFilterMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v5/PartialCommandMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v5::PartialCommandMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v5/ProducerAckMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v5::ProducerAckMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v5/ProducerIdMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v5::ProducerIdMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v5/ProducerInfoMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v5::ProducerInfoMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v5/RemoveInfoMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v5::RemoveInfoMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v5/RemoveSubscriptionInfoMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v5::RemoveSubscriptionInfoMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v5/ReplayCommandMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v5::ReplayCommandMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v5/ResponseMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v5::ResponseMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v5/SessionIdMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v5::SessionIdMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v5/SessionInfoMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v5::SessionInfoMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v5/ShutdownInfoMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v5::ShutdownInfoMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v5/SubscriptionInfoMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v5::SubscriptionInfoMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v5/TransactionInfoMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v5::TransactionInfoMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v5/WireFormatInfoMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v5::WireFormatInfoMarshallerTest );
//#include <activemq/wireformat/openwire/marshal/v5/XATransactionIdMarshallerTest.h>
//CPPUNIT_TEST_SUITE_REGISTRATION( activemq::wireformat::openwire::marshal::v5::XATransactionIdMarshallerTest );
