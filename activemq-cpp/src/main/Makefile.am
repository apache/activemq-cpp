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
    activemq/core/ActiveMQConsumer.cpp \
    activemq/core/ActiveMQConnection.cpp \
    activemq/core/ActiveMQSession.cpp \
    activemq/core/ActiveMQConstants.cpp \
    activemq/core/ActiveMQTransaction.cpp \
    activemq/core/ActiveMQProducer.cpp \
    activemq/core/ActiveMQConnectionFactory.cpp \
    activemq/logger/LoggerHierarchy.cpp \
    activemq/logger/LogWriter.cpp \
    activemq/logger/Logger.cpp \
    activemq/logger/LogManager.cpp \
    activemq/logger/SimpleLogger.cpp \
    activemq/support/InitDirector.cpp \
    activemq/io/BlockingByteArrayInputStream.cpp \
    activemq/io/ByteArrayInputStream.cpp \
    activemq/io/BufferedInputStream.cpp \
    activemq/io/DataInputStream.cpp \
    activemq/io/DataOutputStream.cpp \
    activemq/io/BufferedOutputStream.cpp \
    activemq/io/ByteArrayOutputStream.cpp \
    activemq/concurrent/PooledThread.cpp \
    activemq/concurrent/ThreadPool.cpp \
    activemq/concurrent/Mutex.cpp \
    activemq/concurrent/Thread.cpp \
    activemq/exceptions/ActiveMQException.cpp \
    activemq/connector/stomp/StompSessionManager.cpp \
    activemq/connector/stomp/commands/CommandConstants.cpp \
    activemq/connector/stomp/StompCommandReader.cpp \
    activemq/connector/stomp/StompCommandWriter.cpp \
    activemq/connector/stomp/StompConnectorFactory.cpp \
    activemq/connector/stomp/marshal/Marshaler.cpp \
    activemq/connector/stomp/StompConnector.cpp \
    activemq/connector/openwire/OpenWireFormat.cpp \
    activemq/connector/openwire/marshal/BaseDataStreamMarshaller.cpp \
    activemq/connector/openwire/utils/HexTable.cpp \
    activemq/connector/openwire/utils/BooleanStream.cpp \
    activemq/connector/openwire/marshal/PrimitiveMapMarshaller.cpp \
    activemq/connector/ConnectorFactoryMap.cpp \
    activemq/network/SocketInputStream.cpp \
    activemq/network/ServerSocket.cpp \
    activemq/network/TcpSocket.cpp \
    activemq/network/BufferedSocket.cpp \
    activemq/network/SocketOutputStream.cpp \
    activemq/network/SocketFactory.cpp \
    activemq/transport/IOTransportFactory.cpp \
    activemq/transport/ResponseCorrelator.cpp \
    activemq/transport/TcpTransport.cpp \
    activemq/transport/IOTransport.cpp \
    activemq/transport/TransportFactoryMap.cpp \
    activemq/transport/TcpTransportFactory.cpp \
    activemq/transport/TransportFilter.cpp \
    activemq/util/StringTokenizer.cpp \
    activemq/util/Guid.cpp \
    activemq/util/Date.cpp \
    activemq/util/Math.cpp \
    activemq/util/PrimitiveMap.cpp

h_sources = \
    activemq/core/ActiveMQSessionResource.h \
    activemq/core/ActiveMQProducer.h \
    activemq/core/ActiveMQMessage.h \
    activemq/core/ActiveMQConnectionData.h \
    activemq/core/ActiveMQDestination.h \
    activemq/core/ActiveMQConnection.h \
    activemq/core/ActiveMQTransaction.h \
    activemq/core/ActiveMQMessageListener.h \
    activemq/core/ActiveMQConnectionFactory.h \
    activemq/core/ActiveMQConsumer.h \
    activemq/core/ActiveMQSession.h \
    activemq/core/ActiveMQAckHandler.h \
    activemq/core/ActiveMQConstants.h \
    activemq/logger/LoggerCommon.h \
    activemq/logger/LogRecord.h \
    activemq/logger/SimpleLogger.h \
    activemq/logger/LoggerDefines.h \
    activemq/logger/LoggerHierarchy.h \
    activemq/logger/LogWriter.h \
    activemq/logger/SimpleFormatter.h \
    activemq/logger/Filter.h \
    activemq/logger/Formatter.h \
    activemq/logger/LogManager.h \
    activemq/logger/Logger.h \
    activemq/logger/StreamHandler.h \
    activemq/logger/ConsoleHandler.h \
    activemq/logger/PropertiesChangeListener.h \
    activemq/logger/MarkBlockLogger.h \
    activemq/logger/Handler.h \
    activemq/support/LibraryInit.h \
    activemq/support/InitDirector.h \
    activemq/io/StandardErrorOutputStream.h \
    activemq/io/BlockingByteArrayInputStream.h \
    activemq/io/ByteArrayInputStream.h \
    activemq/io/InputStream.h \
    activemq/io/DataOutputStream.h \
    activemq/io/BufferedOutputStream.h \
    activemq/io/Reader.h \
    activemq/io/BufferedInputStream.h \
    activemq/io/DataInputStream.h \
    activemq/io/Writer.h \
    activemq/io/ByteArrayOutputStream.h \
    activemq/io/OutputStream.h \
    activemq/io/IOException.h \
    activemq/io/FilterInputStream.h \
    activemq/io/FilterOutputStream.h \
    activemq/io/EOFException.h \
    activemq/concurrent/Lock.h \
    activemq/concurrent/ThreadPool.h \
    activemq/concurrent/Thread.h \
    activemq/concurrent/TaskListener.h \
    activemq/concurrent/Synchronizable.h \
    activemq/concurrent/Runnable.h \
    activemq/concurrent/Mutex.h \
    activemq/concurrent/PooledThread.h \
    activemq/concurrent/Concurrent.h \
    activemq/concurrent/PooledThreadListener.h \
    activemq/exceptions/InvalidStateException.h \
    activemq/exceptions/RuntimeException.h \
    activemq/exceptions/InterruptedException.h \
    activemq/exceptions/ActiveMQException.h \
    activemq/exceptions/IllegalMonitorStateException.h \
    activemq/exceptions/ExceptionDefines.h \
    activemq/exceptions/NoSuchElementException.h \
    activemq/exceptions/UnsupportedOperationException.h \
    activemq/exceptions/NullPointerException.h \
    activemq/exceptions/IllegalArgumentException.h \
    activemq/exceptions/IllegalStateException.h \
    activemq/exceptions/IndexOutOfBoundsException.h \
    activemq/connector/ConsumerMessageListener.h \
    activemq/connector/ConnectorException.h \
    activemq/connector/TransactionInfo.h \
    activemq/connector/SessionInfo.h \
    activemq/connector/ConnectorResource.h \
    activemq/connector/ConnectorFactory.h \
    activemq/connector/stomp/StompSessionInfo.h \
    activemq/connector/stomp/StompTransactionInfo.h \
    activemq/connector/stomp/StompSessionManager.h \
    activemq/connector/stomp/StompQueue.h \
    activemq/connector/stomp/StompConnectorFactory.h \
    activemq/connector/stomp/StompFrame.h \
    activemq/connector/stomp/commands/DisconnectCommand.h \
    activemq/connector/stomp/commands/AbortCommand.h \
    activemq/connector/stomp/commands/ErrorCommand.h \
    activemq/connector/stomp/commands/CommandConstants.h \
    activemq/connector/stomp/commands/UnsubscribeCommand.h \
    activemq/connector/stomp/commands/AbstractCommand.h \
    activemq/connector/stomp/commands/StompMessage.h \
    activemq/connector/stomp/commands/AckCommand.h \
    activemq/connector/stomp/commands/StompCommand.h \
    activemq/connector/stomp/commands/BytesMessageCommand.h \
    activemq/connector/stomp/commands/ReceiptCommand.h \
    activemq/connector/stomp/commands/SubscribeCommand.h \
    activemq/connector/stomp/commands/BeginCommand.h \
    activemq/connector/stomp/commands/TextMessageCommand.h \
    activemq/connector/stomp/commands/ConnectCommand.h \
    activemq/connector/stomp/commands/ConnectedCommand.h \
    activemq/connector/stomp/commands/MessageCommand.h \
    activemq/connector/stomp/commands/CommitCommand.h \
    activemq/connector/stomp/StompCommandListener.h \
    activemq/connector/stomp/marshal/Marshalable.h \
    activemq/connector/stomp/marshal/MarshalException.h \
    activemq/connector/stomp/marshal/Marshaler.h \
    activemq/connector/stomp/StompCommandReader.h \
    activemq/connector/stomp/StompCommandWriter.h \
    activemq/connector/stomp/StompConnectorException.h \
    activemq/connector/stomp/StompProducerInfo.h \
    activemq/connector/stomp/StompSelector.h \
    activemq/connector/stomp/StompConsumerInfo.h \
    activemq/connector/stomp/StompConnector.h \
    activemq/connector/stomp/StompDestination.h \
    activemq/connector/stomp/StompTopic.h \
    activemq/connector/openwire/OpenWireFormat.h \
    activemq/connector/openwire/marshal/BaseDataStreamMarshaller.h \
    activemq/connector/openwire/utils/HexTable.h \
    activemq/connector/openwire/utils/BooleanStream.h \
    activemq/connector/openwire/marshal/PrimitiveMapMarshaller.h \
    activemq/connector/ConnectorFactoryMap.h \
    activemq/connector/Connector.h \
    activemq/connector/ConnectorFactoryMapRegistrar.h \
    activemq/connector/ProducerInfo.h \
    activemq/connector/ConsumerInfo.h \
    activemq/network/SocketException.h \
    activemq/network/TcpSocket.h \
    activemq/network/ServerSocket.h \
    activemq/network/Socket.h \
    activemq/network/SocketInputStream.h \
    activemq/network/BufferedSocket.h \
    activemq/network/SocketOutputStream.h \
    activemq/network/SocketFactory.h \
    activemq/transport/IOTransportFactory.h \
    activemq/transport/Transport.h \
    activemq/transport/TransportFactory.h \
    activemq/transport/CommandListener.h \
    activemq/transport/ExceptionResponse.h \
    activemq/transport/ResponseCorrelator.h \
    activemq/transport/IOTransport.h \
    activemq/transport/TcpTransport.h \
    activemq/transport/TransportExceptionListener.h \
    activemq/transport/BrokerError.h \
    activemq/transport/CommandReader.h \
    activemq/transport/TcpTransportFactory.h \
    activemq/transport/Command.h \
    activemq/transport/CommandWriter.h \
    activemq/transport/TransportFactoryMapRegistrar.h \
    activemq/transport/TransportFactoryMap.h \
    activemq/transport/FutureResponse.h \
    activemq/transport/Response.h \
    activemq/transport/CommandIOException.h \
    activemq/transport/TransportFilter.h \
    activemq/util/Endian.h \
    activemq/util/Number.h \
    activemq/util/Config.h \
    activemq/util/Integer.h \
    activemq/util/Guid.h \
    activemq/util/Properties.h \
    activemq/util/Long.h \
    activemq/util/SimpleProperties.h \
    activemq/util/Boolean.h \
    activemq/util/StringTokenizer.h \
    activemq/util/Queue.h \
    activemq/util/Date.h \
    activemq/util/Map.h \
    activemq/util/Math.h \
    activemq/util/PrimitiveMap.h \
    cms/DeliveryMode.h \
    cms/TemporaryQueue.h \
    cms/MapMessage.h \
    cms/Destination.h \
    cms/Connection.h \
    cms/TemporaryTopic.h \
    cms/Stoppable.h \
    cms/Topic.h \
    cms/BytesMessage.h \
    cms/CMSException.h \
    cms/Session.h \
    cms/Startable.h \
    cms/Closeable.h \
    cms/MessageListener.h \
    cms/TextMessage.h \
    cms/ExceptionListener.h \
    cms/Queue.h \
    cms/ConnectionFactory.h \
    cms/MessageProducer.h \
    cms/Message.h \
    cms/MessageConsumer.h

include activemq/connector/openwire/commands/srcmakefile.mk
include activemq/connector/openwire/marshal/v2/srcmakefile.mk

##
## Compiler / Linker Info
##

lib_LIBRARIES= libactivemq-cpp.a
libactivemq_cpp_a_SOURCES= $(h_sources) $(cc_sources)

##
## Uncomment to build a shared lib
##
##lib_LTLIBRARIES= libactivemq-cpp.la
##libactivemq_cpp_la_SOURCES= $(h_sources) $(cc_sources)
##libactivemq_cpp_la_LDFLAGS= -version-info $(ACTIVEMQ_LIBRARY_VERSION) -release $(ACTIVEMQ_VERSION)
##libactivemq_cpp_la_LIBADD=-lpthread -ldl -luuid


##
## Packaging Info
##
library_includedir=$(includedir)/$(ACTIVEMQ_LIBRARY_NAME)-$(ACTIVEMQ_VERSION)
nobase_library_include_HEADERS = $(h_sources)
