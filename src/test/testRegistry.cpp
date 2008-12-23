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

#include <activemq/connector/stomp/commands/UnsubscribeCommandTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::connector::stomp::commands::UnsubscribeCommandTest );
#include <activemq/connector/stomp/commands/TextMessageCommandTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::connector::stomp::commands::TextMessageCommandTest );
#include <activemq/connector/stomp/commands/SubscribeCommandTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::connector::stomp::commands::SubscribeCommandTest );
#include <activemq/connector/stomp/commands/ReceiptCommandTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::connector::stomp::commands::ReceiptCommandTest );
#include <activemq/connector/stomp/commands/MessageCommandTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::connector::stomp::commands::MessageCommandTest );
#include <activemq/connector/stomp/commands/ErrorCommandTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::connector::stomp::commands::ErrorCommandTest );
#include <activemq/connector/stomp/commands/DisconnectCommandTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::connector::stomp::commands::DisconnectCommandTest );
#include <activemq/connector/stomp/commands/ConnectedCommandTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::connector::stomp::commands::ConnectedCommandTest );
#include <activemq/connector/stomp/commands/ConnectCommandTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::connector::stomp::commands::ConnectCommandTest );
#include <activemq/connector/stomp/commands/CommitCommandTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::connector::stomp::commands::CommitCommandTest );
#include <activemq/connector/stomp/commands/CommandConstantsTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::connector::stomp::commands::CommandConstantsTest );
#include <activemq/connector/stomp/commands/BytesMessageCommandTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::connector::stomp::commands::BytesMessageCommandTest );
#include <activemq/connector/stomp/commands/BeginCommandTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::connector::stomp::commands::BeginCommandTest );
#include <activemq/connector/stomp/commands/AckCommandTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::connector::stomp::commands::AckCommandTest );
#include <activemq/connector/stomp/commands/AbortCommandTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::connector::stomp::commands::AbortCommandTest );

#include <activemq/connector/stomp/marshal/MarshalerTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::connector::stomp::marshal::MarshalerTest );

#include <activemq/connector/stomp/StompSessionManagerTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::connector::stomp::StompSessionManagerTest );
#include <activemq/connector/stomp/StompFrameTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::connector::stomp::StompFrameTest );
#include <activemq/connector/stomp/StompDestinationTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::connector::stomp::StompDestinationTest );
#include <activemq/connector/stomp/StompConnectorTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::connector::stomp::StompConnectorTest );
#include <activemq/connector/stomp/StompCommandWriterTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::connector::stomp::StompCommandWriterTest );
#include <activemq/connector/stomp/StompCommandReaderTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::connector::stomp::StompCommandReaderTest );

#include <activemq/connector/openwire/commands/BrokerInfoTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::connector::openwire::commands::BrokerInfoTest );
#include <activemq/connector/openwire/commands/BrokerIdTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::connector::openwire::commands::BrokerIdTest );
#include <activemq/connector/openwire/commands/ActiveMQTopicTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::connector::openwire::commands::ActiveMQTopicTest );
#include <activemq/connector/openwire/commands/ActiveMQTextMessageTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::connector::openwire::commands::ActiveMQTextMessageTest );
#include <activemq/connector/openwire/commands/ActiveMQTempTopicTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::connector::openwire::commands::ActiveMQTempTopicTest );
#include <activemq/connector/openwire/commands/ActiveMQTempQueueTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::connector::openwire::commands::ActiveMQTempQueueTest );
#include <activemq/connector/openwire/commands/ActiveMQQueueTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::connector::openwire::commands::ActiveMQQueueTest );
#include <activemq/connector/openwire/commands/ActiveMQMessageTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::connector::openwire::commands::ActiveMQMessageTest );
#include <activemq/connector/openwire/commands/ActiveMQMapMessageTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::connector::openwire::commands::ActiveMQMapMessageTest );
#include <activemq/connector/openwire/commands/ActiveMQDestinationTest2.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::connector::openwire::commands::ActiveMQDestinationTest );
#include <activemq/connector/openwire/commands/ActiveMQBytesMessageTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::connector::openwire::commands::ActiveMQBytesMessageTest );

#include <activemq/connector/openwire/marshal/BaseDataStreamMarshallerTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::connector::openwire::marshal::BaseDataStreamMarshallerTest );
#include <activemq/connector/openwire/marshal/PrimitiveMapMarshallerTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::connector::openwire::marshal::PrimitiveMapMarshallerTest );

#include <activemq/connector/openwire/utils/BooleanStreamTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::connector::openwire::utils::BooleanStreamTest );
#include <activemq/connector/openwire/utils/HexTableTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::connector::openwire::utils::HexTableTest );
#include <activemq/connector/openwire/utils/OpenwireStringSupportTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::connector::openwire::utils::OpenwireStringSupportTest );
#include <activemq/connector/openwire/utils/MessagePropertyInterceptorTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::connector::openwire::utils::MessagePropertyInterceptorTest );

#include <activemq/connector/openwire/OpenWireFormatTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::connector::openwire::OpenWireFormatTest );

#include <activemq/connector/ConnectorFactoryMapRegistrarTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::connector::ConnectorFactoryMapRegistrarTest );
#include <activemq/connector/ConnectorFactoryMapTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::connector::ConnectorFactoryMapTest );

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

#include <activemq/transport/filters/ResponseCorrelatorTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::transport::filters::ResponseCorrelatorTest );

#include <activemq/transport/IOTransportTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::transport::IOTransportTest );
#include <activemq/transport/TransportFactoryMapRegistrarTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::transport::TransportFactoryMapRegistrarTest );
#include <activemq/transport/TransportFactoryMapTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::transport::TransportFactoryMapTest );

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
#include <activemq/util/URISupportTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::util::URISupportTest );
#include <activemq/util/MemoryUsageTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( activemq::util::MemoryUsageTest );

#include <decaf/internal/util/ByteArrayAdapterTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::internal::util::ByteArrayAdapterTest );
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
#include <decaf/io/DataInputStreamTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::io::DataInputStreamTest );
#include <decaf/io/DataOutputStreamTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::io::DataOutputStreamTest );

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

#include <decaf/util/DateTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::util::DateTest );
#include <decaf/util/UUIDTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::util::UUIDTest );
#include <decaf/util/ListTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::util::ListTest );
#include <decaf/util/MapTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::util::MapTest );
#include <decaf/util/QueueTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::util::QueueTest );
#include <decaf/util/RandomTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::util::RandomTest );
#include <decaf/util/SetTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::util::SetTest );
#include <decaf/util/StringTokenizerTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::util::StringTokenizerTest );
