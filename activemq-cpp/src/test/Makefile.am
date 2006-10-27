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
  activemq/connector/openwire/marshal/HexTableTest.cpp \
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
  activemq/util/GuidTest.cpp \
  activemq/util/IntegerTest.cpp \
  activemq/util/LongTest.cpp \
  activemq/util/QueueTest.cpp \
  activemq/util/StringTokenizerTest.cpp \
  main.cpp

## Compile this as part of make check
check_PROGRAMS = activemq-test
## Also run the tests as part of make check
TESTS = $(check_PROGRAMS)

## 
## Compiler/Linker Options
##
activemq_test_SOURCES = $(cc_sources)
activemq_test_CXXFLAGS = -I$(srcdir)/../main @CPPUNIT_CFLAGS@
activemq_test_LDADD= ../main/libactivemq-cpp.a @CPPUNIT_LIBS@

