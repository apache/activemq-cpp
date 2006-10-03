SRCDIR = src
OUTDIR = out
MAKESUPPORT_HOME = $(PWD)

OFILES = \
	$(OUTDIR)/main/activemq/exceptions/ActiveMQException.o \
	\
	$(OUTDIR)/main/activemq/support/InitDirector.o \
	\
	$(OUTDIR)/main/activemq/transport/IOTransport.o \
	$(OUTDIR)/main/activemq/transport/TcpTransport.o \
	$(OUTDIR)/main/activemq/transport/ResponseCorrelator.o \
	$(OUTDIR)/main/activemq/transport/TransportFactoryMap.o \
	$(OUTDIR)/main/activemq/transport/IOTransportFactory.o \
	$(OUTDIR)/main/activemq/transport/TcpTransportFactory.o \
	\
	$(OUTDIR)/main/activemq/connector/ConnectorFactoryMap.o \
	\
	$(OUTDIR)/main/activemq/connector/stomp/StompConnector.o \
	$(OUTDIR)/main/activemq/connector/stomp/StompConnectorFactory.o \
	$(OUTDIR)/main/activemq/connector/stomp/StompCommandReader.o \
	$(OUTDIR)/main/activemq/connector/stomp/StompCommandWriter.o \
	$(OUTDIR)/main/activemq/connector/stomp/StompSessionManager.o \
    \
	$(OUTDIR)/main/activemq/connector/stomp/commands/CommandConstants.o \
    \
	$(OUTDIR)/main/activemq/connector/stomp/marshal/Marshaler.o \
    \
	$(OUTDIR)/main/activemq/connector/openwire/utils/BooleanStream.o \
	$(OUTDIR)/main/activemq/connector/openwire/OpenWireFormatFactory.o \
	$(OUTDIR)/main/activemq/connector/openwire/OpenWireFormat.o \
    \
	$(OUTDIR)/main/activemq/connector/openwire/commands/WireFormatInfo.o \
    \
	$(OUTDIR)/main/activemq/core/ActiveMQConnectionFactory.o \
	$(OUTDIR)/main/activemq/core/ActiveMQConnection.o \
	$(OUTDIR)/main/activemq/core/ActiveMQSession.o \
	$(OUTDIR)/main/activemq/core/ActiveMQProducer.o \
	$(OUTDIR)/main/activemq/core/ActiveMQConsumer.o \
	$(OUTDIR)/main/activemq/core/ActiveMQTransaction.o \
	$(OUTDIR)/main/activemq/core/ActiveMQConstants.o \
	\
	$(OUTDIR)/main/activemq/io/DataInputStream.o \
	$(OUTDIR)/main/activemq/io/DataOutputStream.o \
	$(OUTDIR)/main/activemq/io/BufferedInputStream.o \
	$(OUTDIR)/main/activemq/io/BufferedOutputStream.o \
	$(OUTDIR)/main/activemq/io/ByteArrayInputStream.o \
	$(OUTDIR)/main/activemq/io/ByteArrayOutputStream.o \
    \
	$(OUTDIR)/main/activemq/logger/SimpleLogger.o \
	$(OUTDIR)/main/activemq/logger/LogWriter.o \
	$(OUTDIR)/main/activemq/logger/LogManager.o \
	$(OUTDIR)/main/activemq/logger/LoggerHierarchy.o \
    \
    $(OUTDIR)/main/activemq/network/ServerSocket.o \
    $(OUTDIR)/main/activemq/network/TcpSocket.o \
    $(OUTDIR)/main/activemq/network/BufferedSocket.o \
    $(OUTDIR)/main/activemq/network/SocketFactory.o \
    $(OUTDIR)/main/activemq/network/SocketInputStream.o \
    $(OUTDIR)/main/activemq/network/SocketOutputStream.o\
    \
    $(OUTDIR)/main/activemq/util/Math.o \
    $(OUTDIR)/main/activemq/util/Guid.o \
    $(OUTDIR)/main/activemq/util/StringTokenizer.o \
    \
    $(OUTDIR)/main/activemq/concurrent/Thread.o \
    $(OUTDIR)/main/activemq/concurrent/Mutex.o \
    $(OUTDIR)/main/activemq/concurrent/ThreadPool.o \
    $(OUTDIR)/main/activemq/concurrent/PooledThread.o \
    \
    $(OUTDIR)/main/activemq/connector/openwire/commands/ActiveMQBytesMessage.o \
    $(OUTDIR)/main/activemq/connector/openwire/commands/ActiveMQTextMessage.o \
    $(OUTDIR)/main/activemq/connector/openwire/commands/ActiveMQMessage.o \
    $(OUTDIR)/main/activemq/connector/openwire/commands/ActiveMQObjectMessage.o \
    $(OUTDIR)/main/activemq/connector/openwire/commands/ActiveMQMapMessage.o \
    $(OUTDIR)/main/activemq/connector/openwire/commands/ActiveMQStreamMessage.o \
    $(OUTDIR)/main/activemq/connector/openwire/commands/ActiveMQDestination.o \
    $(OUTDIR)/main/activemq/connector/openwire/commands/ActiveMQTempDestination.o \
    $(OUTDIR)/main/activemq/connector/openwire/commands/ActiveMQQueue.o \
    $(OUTDIR)/main/activemq/connector/openwire/commands/ActiveMQTopic.o \
    $(OUTDIR)/main/activemq/connector/openwire/commands/ActiveMQTempTopic.o \
    $(OUTDIR)/main/activemq/connector/openwire/commands/ActiveMQTempQueue.o \
    \
    $(OUTDIR)/main/activemq/connector/openwire/commands/BrokerInfo.o \
    $(OUTDIR)/main/activemq/connector/openwire/commands/ConnectionControl.o \
    $(OUTDIR)/main/activemq/connector/openwire/commands/ConnectionError.o \
    $(OUTDIR)/main/activemq/connector/openwire/commands/ConnectionId.o \
    $(OUTDIR)/main/activemq/connector/openwire/commands/ConnectionInfo.o \
    $(OUTDIR)/main/activemq/connector/openwire/commands/ConsumerControl.o \
    $(OUTDIR)/main/activemq/connector/openwire/commands/ConsumerId.o \
    $(OUTDIR)/main/activemq/connector/openwire/commands/ConsumerInfo.o \
    $(OUTDIR)/main/activemq/connector/openwire/commands/ControlCommand.o \
    $(OUTDIR)/main/activemq/connector/openwire/commands/DataArrayResponse.o \
    $(OUTDIR)/main/activemq/connector/openwire/commands/DataResponse.o \
    $(OUTDIR)/main/activemq/connector/openwire/commands/DestinationInfo.o \
    $(OUTDIR)/main/activemq/connector/openwire/commands/DiscoveryEvent.o \
    $(OUTDIR)/main/activemq/connector/openwire/commands/ExceptionResponse.o \
    $(OUTDIR)/main/activemq/connector/openwire/commands/FlushCommand.o \
    $(OUTDIR)/main/activemq/connector/openwire/commands/IntegerResponse.o \
    $(OUTDIR)/main/activemq/connector/openwire/commands/JournalQueueAck.o \
    $(OUTDIR)/main/activemq/connector/openwire/commands/JournalTopicAck.o \
    $(OUTDIR)/main/activemq/connector/openwire/commands/JournalTrace.o \
    $(OUTDIR)/main/activemq/connector/openwire/commands/JournalTransaction.o \
    $(OUTDIR)/main/activemq/connector/openwire/commands/KeepAliveInfo.o \
    $(OUTDIR)/main/activemq/connector/openwire/commands/LastPartialCommand.o \
    $(OUTDIR)/main/activemq/connector/openwire/commands/LocalTransactionId.o \
    $(OUTDIR)/main/activemq/connector/openwire/commands/Message.o \
    $(OUTDIR)/main/activemq/connector/openwire/commands/MessageAck.o \
    $(OUTDIR)/main/activemq/connector/openwire/commands/MessageDispatch.o \
    $(OUTDIR)/main/activemq/connector/openwire/commands/MessageDispatchNotification.o \
    $(OUTDIR)/main/activemq/connector/openwire/commands/MessageId.o \
    $(OUTDIR)/main/activemq/connector/openwire/commands/MessagePull.o \
    $(OUTDIR)/main/activemq/connector/openwire/commands/NetworkBridgeFilter.o \
    $(OUTDIR)/main/activemq/connector/openwire/commands/PartialCommand.o \
    $(OUTDIR)/main/activemq/connector/openwire/commands/ProducerId.o \
    $(OUTDIR)/main/activemq/connector/openwire/commands/ProducerInfo.o \
    $(OUTDIR)/main/activemq/connector/openwire/commands/RemoveInfo.o \
    $(OUTDIR)/main/activemq/connector/openwire/commands/RemoveSubscriptionInfo.o \
    $(OUTDIR)/main/activemq/connector/openwire/commands/ReplayCommand.o \
    $(OUTDIR)/main/activemq/connector/openwire/commands/Response.o \
    $(OUTDIR)/main/activemq/connector/openwire/commands/SessionId.o \
    $(OUTDIR)/main/activemq/connector/openwire/commands/SessionInfo.o \
    $(OUTDIR)/main/activemq/connector/openwire/commands/ShutdownInfo.o \
    $(OUTDIR)/main/activemq/connector/openwire/commands/SubscriptionInfo.o \
    $(OUTDIR)/main/activemq/connector/openwire/commands/TransactionId.o \
    $(OUTDIR)/main/activemq/connector/openwire/commands/TransactionInfo.o \
    $(OUTDIR)/main/activemq/connector/openwire/commands/WireFormatInfo.o \
    $(OUTDIR)/main/activemq/connector/openwire/commands/XATransactionId.o 

OTESTFILES = \
    $(OUTDIR)/test/main.o \
    \
    $(OUTDIR)/test/activemq/core/ActiveMQDestinationTest.o \
    $(OUTDIR)/test/activemq/core/ActiveMQConnectionFactoryTest.o \
    $(OUTDIR)/test/activemq/core/ActiveMQConnectionTest.o \
    $(OUTDIR)/test/activemq/core/ActiveMQSessionTest.o \
    \
    $(OUTDIR)/test/activemq/concurrent/MutexTest.o \
    $(OUTDIR)/test/activemq/concurrent/ThreadPoolTest.o \
    $(OUTDIR)/test/activemq/concurrent/ThreadTest.o \
    \
    $(OUTDIR)/test/activemq/connector/stomp/StompConnectorTest.o \
    $(OUTDIR)/test/activemq/connector/stomp/StompFrameTest.o \
    $(OUTDIR)/test/activemq/connector/stomp/StompCommandReaderTest.o \
    $(OUTDIR)/test/activemq/connector/stomp/StompCommandWriterTest.o \
    $(OUTDIR)/test/activemq/connector/stomp/StompSessionManagerTest.o \
    \
    $(OUTDIR)/test/activemq/connector/stomp/commands/CommandConstantsTest.o \
	$(OUTDIR)/test/activemq/connector/stomp/commands/AbortCommandTest.o \
	$(OUTDIR)/test/activemq/connector/stomp/commands/AckCommandTest.o \
	$(OUTDIR)/test/activemq/connector/stomp/commands/BeginCommandTest.o \
	$(OUTDIR)/test/activemq/connector/stomp/commands/CommitCommandTest.o \
	$(OUTDIR)/test/activemq/connector/stomp/commands/ConnectCommandTest.o \
	$(OUTDIR)/test/activemq/connector/stomp/commands/ConnectedCommandTest.o \
	$(OUTDIR)/test/activemq/connector/stomp/commands/DisconnectCommandTest.o \
	$(OUTDIR)/test/activemq/connector/stomp/commands/ErrorCommandTest.o \
	$(OUTDIR)/test/activemq/connector/stomp/commands/ReceiptCommandTest.o \
	$(OUTDIR)/test/activemq/connector/stomp/commands/SubscribeCommandTest.o \
	$(OUTDIR)/test/activemq/connector/stomp/commands/UnsubscribeCommandTest.o \
	$(OUTDIR)/test/activemq/connector/stomp/commands/MessageCommandTest.o \
	$(OUTDIR)/test/activemq/connector/stomp/commands/BytesMessageCommandTest.o \
	$(OUTDIR)/test/activemq/connector/stomp/commands/TextMessageCommandTest.o \
    \
	$(OUTDIR)/test/activemq/connector/stomp/marshal/MarshalerTest.o \
    \
    $(OUTDIR)/test/activemq/connector/ConnectorFactoryMapRegistrarTest.o \
    $(OUTDIR)/test/activemq/connector/ConnectorFactoryMapTest.o \
    \
    $(OUTDIR)/test/activemq/connector/openwire/utils/BooleanStreamTest.o \
    \
    $(OUTDIR)/test/activemq/exceptions/ActiveMQExceptionTest.o \
    \
    $(OUTDIR)/test/activemq/io/BufferedInputStreamTest.o \
	$(OUTDIR)/test/activemq/io/BufferedOutputStreamTest.o \
	$(OUTDIR)/test/activemq/io/ByteArrayInputStreamTest.o \
	$(OUTDIR)/test/activemq/io/ByteArrayOutputStreamTest.o \
	$(OUTDIR)/test/activemq/io/DataInputStreamTest.o \
	$(OUTDIR)/test/activemq/io/DataOutputStreamTest.o \
	\
	$(OUTDIR)/test/activemq/logger/LoggerTest.o \
	\
	$(OUTDIR)/test/activemq/network/SocketFactoryTest.o \
	$(OUTDIR)/test/activemq/network/SocketTest.o \
	\
	$(OUTDIR)/test/activemq/transport/DummyTransportFactory.o \
	$(OUTDIR)/test/activemq/transport/IOTransportTest.o \
	$(OUTDIR)/test/activemq/transport/ResponseCorrelatorTest.o \
	$(OUTDIR)/test/activemq/transport/TransportFactoryMapTest.o \
	$(OUTDIR)/test/activemq/transport/TransportFactoryMapRegistrarTest.o \
	\
	$(OUTDIR)/test/activemq/util/GuidTest.o \
	$(OUTDIR)/test/activemq/util/IntegerTest.o \
	$(OUTDIR)/test/activemq/util/LongTest.o \
	$(OUTDIR)/test/activemq/util/BooleanTest.o \
	$(OUTDIR)/test/activemq/util/QueueTest.o \
	$(OUTDIR)/test/activemq/util/StringTokenizerTest.o

OINTEGRATIONFILES = \
	$(OUTDIR)/test-integration/main.o \
	\
	$(OUTDIR)/test-integration/integration/simple/SimpleTester.o \
	$(OUTDIR)/test-integration/integration/transactional/TransactionTester.o \
	$(OUTDIR)/test-integration/integration/common/AbstractTester.o \
	$(OUTDIR)/test-integration/integration/common/IntegrationCommon.o \
	$(OUTDIR)/test-integration/integration/various/SimpleRollbackTest.o
	

# Increment this to get a build specific library.
VERSION = 0_0_2

LIBRARY_NAME   = activemq-cpp-$(VERSION)
LIBFILE        = $(OUTDIR)/lib$(LIBRARY_NAME).a
TESTEXE        = $(OUTDIR)/activemqTest
INTEGRATIONEXE = $(OUTDIR)/activemqIntegrationTests

DEFINES          =

include $(MAKESUPPORT_HOME)/makefile.cfg

