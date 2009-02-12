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
    activemq/library/ActiveMQCPP.cpp \
    activemq/io/LoggingInputStream.cpp \
    activemq/io/LoggingOutputStream.cpp \
    activemq/exceptions/ActiveMQException.cpp \
    activemq/util/PrimitiveMap.cpp \
    activemq/util/PrimitiveList.cpp \
    activemq/util/PrimitiveValueNode.cpp \
    activemq/util/LongSequenceGenerator.cpp \
    activemq/util/URISupport.cpp \
    activemq/util/MemoryUsage.cpp \
    activemq/cmsutil/DynamicDestinationResolver.cpp \
    activemq/cmsutil/ResourceLifecycleManager.cpp \
    activemq/cmsutil/CmsAccessor.cpp \
    activemq/cmsutil/CmsDestinationAccessor.cpp \
    activemq/cmsutil/SessionPool.cpp \
    activemq/cmsutil/PooledSession.cpp \
    activemq/cmsutil/CmsTemplate.cpp \
    activemq/core/ActiveMQConsumer.cpp \
    activemq/core/ActiveMQConnection.cpp \
    activemq/core/ActiveMQConnectionSupport.cpp \
    activemq/core/ActiveMQConnectionMetaData.cpp \
    activemq/core/ActiveMQSession.cpp \
    activemq/core/ActiveMQConstants.cpp \
    activemq/core/ActiveMQTransactionContext.cpp \
    activemq/core/ActiveMQProducer.cpp \
    activemq/core/ActiveMQConnectionFactory.cpp \
    activemq/core/ActiveMQSessionExecutor.cpp \
    activemq/state/ConnectionState.cpp \
    activemq/state/ConnectionStateTracker.cpp \
    activemq/state/ConsumerState.cpp \
    activemq/state/ProducerState.cpp \
    activemq/state/SessionState.cpp \
    activemq/state/TransactionState.cpp \
    activemq/state/Tracked.cpp \
    activemq/transport/failover/BackupTransport.cpp \
    activemq/transport/failover/FailoverTransport.cpp \
    activemq/transport/failover/FailoverTransportFactory.cpp \
    activemq/transport/TransportFilter.cpp \
    activemq/transport/TransportRegistry.cpp \
    activemq/transport/AbstractTransportFactory.cpp \
    activemq/transport/IOTransport.cpp \
    activemq/transport/mock/MockTransport.cpp \
    activemq/transport/mock/MockTransportFactory.cpp \
    activemq/transport/correlator/ResponseCorrelator.cpp \
    activemq/transport/tcp/TcpTransport.cpp \
    activemq/transport/tcp/TcpTransportFactory.cpp \
    activemq/transport/logging/LoggingTransport.cpp \
    activemq/wireformat/WireFormatRegistry.cpp \
    activemq/wireformat/openwire/OpenWireFormat.cpp \
    activemq/wireformat/openwire/OpenWireFormatFactory.cpp \
    activemq/wireformat/openwire/OpenWireFormatNegotiator.cpp \
    activemq/wireformat/openwire/OpenWireResponseBuilder.cpp \
    activemq/wireformat/openwire/marshal/BaseDataStreamMarshaller.cpp \
    activemq/wireformat/openwire/utils/HexTable.cpp \
    activemq/wireformat/openwire/utils/BooleanStream.cpp \
    activemq/wireformat/openwire/utils/OpenwireStringSupport.cpp \
    activemq/wireformat/openwire/utils/MessagePropertyInterceptor.cpp \
    activemq/wireformat/openwire/marshal/PrimitiveMapMarshaller.cpp \
    decaf/internal/DecafRuntime.cpp \
    decaf/internal/AprPool.cpp \
    decaf/internal/util/ByteArrayAdapter.cpp \
    decaf/internal/util/HexStringParser.cpp \
    decaf/internal/io/StandardErrorOutputStream.cpp \
    decaf/internal/io/StandardOutputStream.cpp \
    decaf/internal/io/StandardInputStream.cpp \
    decaf/internal/nio/BufferFactory.cpp \
    decaf/internal/nio/ByteArrayPerspective.cpp \
    decaf/internal/nio/ByteArrayBuffer.cpp \
    decaf/internal/nio/CharArrayBuffer.cpp \
    decaf/internal/nio/DoubleArrayBuffer.cpp \
    decaf/internal/nio/FloatArrayBuffer.cpp \
    decaf/internal/nio/LongArrayBuffer.cpp \
    decaf/internal/nio/IntArrayBuffer.cpp \
    decaf/internal/nio/ShortArrayBuffer.cpp \
    decaf/internal/net/URIEncoderDecoder.cpp \
    decaf/internal/net/URIHelper.cpp \
    decaf/net/ServerSocket.cpp \
    decaf/net/SocketOutputStream.cpp \
    decaf/net/BufferedSocket.cpp \
    decaf/net/SocketInputStream.cpp \
    decaf/net/SocketError.cpp \
    decaf/net/TcpSocket.cpp \
    decaf/net/SocketFactory.cpp \
    decaf/net/URI.cpp \
    decaf/net/URL.cpp \
    decaf/net/URLEncoder.cpp \
    decaf/net/URLDecoder.cpp \
    decaf/lang/Exception.cpp \
    decaf/lang/Thread.cpp \
    decaf/lang/Byte.cpp \
    decaf/lang/Character.cpp \
    decaf/lang/Boolean.cpp \
    decaf/lang/Short.cpp \
    decaf/lang/Integer.cpp \
    decaf/lang/Long.cpp \
    decaf/lang/Float.cpp \
    decaf/lang/Double.cpp \
    decaf/lang/Math.cpp \
    decaf/lang/System.cpp \
    decaf/io/BufferedOutputStream.cpp \
    decaf/io/BufferedInputStream.cpp \
    decaf/io/ByteArrayInputStream.cpp \
    decaf/io/DataInputStream.cpp \
    decaf/io/DataOutputStream.cpp \
    decaf/io/ByteArrayOutputStream.cpp \
    decaf/io/BlockingByteArrayInputStream.cpp \
    decaf/util/concurrent/Mutex.cpp \
    decaf/util/concurrent/CountDownLatch.cpp \
    decaf/util/concurrent/PooledThread.cpp \
    decaf/util/concurrent/TimeUnit.cpp \
    decaf/util/concurrent/ThreadPool.cpp \
    decaf/util/concurrent/atomic/AtomicBoolean.cpp \
    decaf/util/concurrent/atomic/AtomicInteger.cpp \
    decaf/util/Date.cpp \
    decaf/util/UUID.cpp \
    decaf/util/StringTokenizer.cpp \
    decaf/util/Random.cpp \
    decaf/util/logging/LoggerHierarchy.cpp \
    decaf/util/logging/Logger.cpp \
    decaf/util/logging/LogWriter.cpp \
    decaf/util/logging/SimpleLogger.cpp \
    decaf/util/logging/LogManager.cpp \
    decaf/nio/Buffer.cpp \
    decaf/nio/ByteBuffer.cpp \
    decaf/nio/CharBuffer.cpp \
    decaf/nio/DoubleBuffer.cpp \
    decaf/nio/FloatBuffer.cpp \
    decaf/nio/IntBuffer.cpp \
    decaf/nio/LongBuffer.cpp \
    decaf/nio/ShortBuffer.cpp

#    activemq/wireformat/stomp/StompWireFormat.cpp \
#    activemq/wireformat/stomp/StompWireFormatFactory.cpp \
#    activemq/wireformat/stomp/StompResponseBuilder.cpp \
#    activemq/wireformat/stomp/marshal/Marshaler.cpp \
#    activemq/wireformat/stomp/commands/CommandConstants.cpp

h_sources = \
    activemq/library/ActiveMQCPP.h \
    activemq/core/ActiveMQProducer.h \
    activemq/core/ActiveMQConnection.h \
    activemq/core/ActiveMQConnectionSupport.h \
    activemq/core/ActiveMQConnectionMetaData.h \
    activemq/core/ActiveMQTransactionContext.h \
    activemq/core/ActiveMQConnectionFactory.h \
    activemq/core/ActiveMQConsumer.h \
    activemq/core/ActiveMQSession.h \
    activemq/core/ActiveMQAckHandler.h \
    activemq/core/ActiveMQConstants.h \
    activemq/core/ActiveMQSessionExecutor.h \
    activemq/core/DispatchData.h \
    activemq/core/Dispatcher.h \
    activemq/core/Synchronization.h \
    activemq/cmsutil/CachedConsumer.h \
    activemq/cmsutil/CachedProducer.h \
    activemq/cmsutil/CmsAccessor.h \
    activemq/cmsutil/CmsDestinationAccessor.h \
    activemq/cmsutil/CmsTemplate.h \
    activemq/cmsutil/DestinationResolver.h \
    activemq/cmsutil/DynamicDestinationResolver.h \
    activemq/cmsutil/MessageCreator.h \
    activemq/cmsutil/PooledSession.h \
    activemq/cmsutil/ProducerCallback.h \
    activemq/cmsutil/ResourceLifecycleManager.h \
    activemq/cmsutil/SessionCallback.h \
    activemq/cmsutil/SessionPool.h \
    activemq/io/LoggingInputStream.h \
    activemq/io/LoggingOutputStream.h \
    activemq/exceptions/ActiveMQException.h \
    activemq/exceptions/BrokerException.h \
    activemq/exceptions/ExceptionDefines.h \
    activemq/state/CommandVisitor.h \
    activemq/state/CommandVisitorAdapter.h \
    activemq/state/ConnectionState.h \
    activemq/state/ConnectionStateTracker.h \
    activemq/state/ConsumerState.h \
    activemq/state/ProducerState.h \
    activemq/state/SessionState.h \
    activemq/state/TransactionState.h \
    activemq/state/Tracked.h \
    activemq/transport/AbstractTransportFactory.h \
    activemq/transport/CommandIOException.h \
    activemq/transport/DefaultTransportListener.h \
    activemq/transport/IOTransport.h \
    activemq/transport/Transport.h \
    activemq/transport/TransportListener.h \
    activemq/transport/TransportFilter.h \
    activemq/transport/TransportFactory.h \
    activemq/transport/TransportRegistry.h \
    activemq/transport/failover/BackupTransport.h \
    activemq/transport/failover/FailoverTransport.h \
    activemq/transport/failover/FailoverTransportFactory.h \
    activemq/transport/mock/MockTransport.h \
    activemq/transport/mock/MockTransportFactory.h \
    activemq/transport/correlator/FutureResponse.h \
    activemq/transport/correlator/ResponseCorrelator.h \
    activemq/transport/tcp/TcpTransport.h \
    activemq/transport/tcp/TcpTransportFactory.h \
    activemq/transport/logging/LoggingTransport.h \
    activemq/util/Config.h \
    activemq/util/ActiveMQProperties.h \
    activemq/util/LongSequenceGenerator.h \
    activemq/util/PrimitiveValueNode.h \
    activemq/util/PrimitiveMap.h \
    activemq/util/PrimitiveList.h \
    activemq/util/URISupport.h \
    activemq/util/MemoryUsage.h \
    activemq/util/Usage.h \
    activemq/wireformat/WireFormat.h \
    activemq/wireformat/WireFormatNegotiator.h \
    activemq/wireformat/WireFormatFactory.h \
    activemq/wireformat/WireFormatRegistry.h \
    activemq/wireformat/openwire/OpenWireFormat.h \
    activemq/wireformat/openwire/OpenWireFormatNegotiator.h \
    activemq/wireformat/openwire/OpenWireResponseBuilder.h \
    activemq/wireformat/openwire/marshal/BaseDataStreamMarshaller.h \
    activemq/wireformat/openwire/utils/HexTable.h \
    activemq/wireformat/openwire/utils/BooleanStream.h \
    activemq/wireformat/openwire/utils/OpenwireStringSupport.h \
    activemq/wireformat/openwire/utils/MessagePropertyInterceptor.h \
    activemq/wireformat/openwire/marshal/PrimitiveMapMarshaller.h \
    cms/DeliveryMode.h \
    cms/TemporaryQueue.h \
    cms/MapMessage.h \
    cms/Destination.h \
    cms/Connection.h \
    cms/ConnectionMetaData.h \
    cms/TemporaryTopic.h \
    cms/Stoppable.h \
    cms/Topic.h \
    cms/BytesMessage.h \
    cms/CMSException.h \
    cms/CMSProperties.h \
    cms/Session.h \
    cms/Startable.h \
    cms/Closeable.h \
    cms/MessageListener.h \
    cms/ObjectMessage.h \
    cms/TextMessage.h \
    cms/ExceptionListener.h \
    cms/Queue.h \
    cms/ConnectionFactory.h \
    cms/MessageProducer.h \
    cms/Message.h \
    cms/MessageConsumer.h \
    cms/Config.h \
    decaf/net/BindException.h \
    decaf/net/ConnectException.h \
    decaf/net/HttpRetryException.h \
    decaf/net/MalformedURLException.h \
    decaf/net/NoRouteToHostException.h \
    decaf/net/PortUnreachableException.h \
    decaf/net/ProtocolException.h \
    decaf/net/SocketException.h \
    decaf/net/SocketTimeoutException.h \
    decaf/net/UnknownHostException.h \
    decaf/net/UnknownServiceException.h \
    decaf/net/URISyntaxException.h \
    decaf/net/BufferedSocket.h \
    decaf/net/TcpSocket.h \
    decaf/net/SocketError.h \
    decaf/net/SocketOutputStream.h \
    decaf/net/Socket.h \
    decaf/net/SocketInputStream.h \
    decaf/net/ServerSocket.h \
    decaf/net/SocketFactory.h \
    decaf/net/URI.h \
    decaf/net/URL.h \
    decaf/net/URLEncoder.h \
    decaf/net/URLDecoder.h \
    decaf/lang/Throwable.h \
    decaf/lang/Exception.h \
    decaf/lang/Byte.h \
    decaf/lang/Character.h \
    decaf/lang/CharSequence.h \
    decaf/lang/Comparable.h \
    decaf/lang/Iterable.h \
    decaf/lang/Short.h \
    decaf/lang/Integer.h \
    decaf/lang/Long.h \
    decaf/lang/Float.h \
    decaf/lang/Double.h \
    decaf/lang/Thread.h \
    decaf/lang/Number.h \
    decaf/lang/Runnable.h \
    decaf/lang/Runtime.h \
    decaf/lang/Math.h \
    decaf/lang/System.h \
    decaf/lang/Boolean.h \
    decaf/lang/exceptions/NoSuchElementException.h \
    decaf/lang/exceptions/RuntimeException.h \
    decaf/lang/exceptions/IndexOutOfBoundsException.h \
    decaf/lang/exceptions/NullPointerException.h \
    decaf/lang/exceptions/IllegalStateException.h \
    decaf/lang/exceptions/IllegalMonitorStateException.h \
    decaf/lang/exceptions/InvalidStateException.h \
    decaf/lang/exceptions/ExceptionDefines.h \
    decaf/lang/exceptions/IllegalArgumentException.h \
    decaf/lang/exceptions/UnsupportedOperationException.h \
    decaf/lang/exceptions/InterruptedException.h \
    decaf/lang/exceptions/NumberFormatException.h \
    decaf/io/BufferedOutputStream.h \
    decaf/io/OutputStream.h \
    decaf/io/Writer.h \
    decaf/io/ByteArrayOutputStream.h \
    decaf/io/Closeable.h \
    decaf/io/DataOutputStream.h \
    decaf/io/ByteArrayInputStream.h \
    decaf/io/FilterInputStream.h \
    decaf/io/Reader.h \
    decaf/io/BlockingByteArrayInputStream.h \
    decaf/io/DataInputStream.h \
    decaf/io/EOFException.h \
    decaf/io/FilterOutputStream.h \
    decaf/io/BufferedInputStream.h \
    decaf/io/InputStream.h \
    decaf/io/IOException.h \
    decaf/io/InterruptedIOException.h \
    decaf/util/concurrent/BrokenBarrierException.h \
    decaf/util/concurrent/Delayed.h \
    decaf/util/concurrent/Callable.h \
    decaf/util/concurrent/CancellationException.h \
    decaf/util/concurrent/Concurrent.h \
    decaf/util/concurrent/ConcurrentMap.h \
    decaf/util/concurrent/ConcurrentStlMap.h \
    decaf/util/concurrent/CountDownLatch.h \
    decaf/util/concurrent/Executor.h \
    decaf/util/concurrent/ExecutionException.h \
    decaf/util/concurrent/Lock.h \
    decaf/util/concurrent/Mutex.h \
    decaf/util/concurrent/PooledThread.h \
    decaf/util/concurrent/PooledThreadListener.h \
    decaf/util/concurrent/RejectedExecutionException.h \
    decaf/util/concurrent/Synchronizable.h \
    decaf/util/concurrent/TaskListener.h \
    decaf/util/concurrent/TimeUnit.h \
    decaf/util/concurrent/ThreadPool.h \
    decaf/util/concurrent/ThreadFactory.h \
    decaf/util/concurrent/TimeoutException.h \
    decaf/util/concurrent/atomic/AtomicBoolean.h \
    decaf/util/concurrent/atomic/AtomicInteger.h \
    decaf/util/concurrent/atomic/AtomicReference.h \
    decaf/util/concurrent/locks/Lock.h \
    decaf/util/concurrent/locks/Condition.h \
    decaf/util/concurrent/locks/ReadWriteLock.h \
    decaf/util/Config.h \
    decaf/util/Date.h \
    decaf/util/UUID.h \
    decaf/util/Iterator.h \
    decaf/util/ListIterator.h \
    decaf/util/Collection.h \
    decaf/util/Comparator.h \
    decaf/util/List.h \
    decaf/util/Map.h \
    decaf/util/StlMap.h \
    decaf/util/StlList.h \
    decaf/util/StlSet.h \
    decaf/util/Properties.h \
    decaf/util/Queue.h \
    decaf/util/Set.h \
    decaf/util/Random.h \
    decaf/util/StringTokenizer.h \
    decaf/util/logging/Handler.h \
    decaf/util/logging/Logger.h \
    decaf/util/logging/LoggerCommon.h \
    decaf/util/logging/ConsoleHandler.h \
    decaf/util/logging/SimpleFormatter.h \
    decaf/util/logging/Formatter.h \
    decaf/util/logging/LoggerHierarchy.h \
    decaf/util/logging/LogManager.h \
    decaf/util/logging/MarkBlockLogger.h \
    decaf/util/logging/StreamHandler.h \
    decaf/util/logging/LoggerDefines.h \
    decaf/util/logging/Filter.h \
    decaf/util/logging/LogRecord.h \
    decaf/util/logging/LogWriter.h \
    decaf/util/logging/SimpleLogger.h \
    decaf/util/logging/PropertiesChangeListener.h \
    decaf/internal/AprPool.h \
    decaf/internal/DecafRuntime.h \
    decaf/internal/util/HexStringParser.h \
    decaf/internal/util/ByteArrayAdapter.h \
    decaf/internal/io/StandardErrorOutputStream.h \
    decaf/internal/io/StandardOutputStream.h \
    decaf/internal/io/StandardInputStream.h \
    decaf/internal/nio/BufferFactory.h \
    decaf/internal/nio/ByteArrayPerspective.h \
    decaf/internal/nio/ByteArrayBuffer.h \
    decaf/internal/nio/CharArrayBuffer.h \
    decaf/internal/nio/DoubleArrayBuffer.h \
    decaf/internal/nio/FloatArrayBuffer.h \
    decaf/internal/nio/LongArrayBuffer.h \
    decaf/internal/nio/IntArrayBuffer.h \
    decaf/internal/nio/ShortArrayBuffer.h \
    decaf/internal/net/URIEncoderDecoder.h \
    decaf/internal/net/URIHelper.h \
    decaf/internal/net/URIType.h \
    decaf/nio/Buffer.h \
    decaf/nio/ByteBuffer.h \
    decaf/nio/CharBuffer.h \
    decaf/nio/DoubleBuffer.h \
    decaf/nio/FloatBuffer.h \
    decaf/nio/IntBuffer.h \
    decaf/nio/LongBuffer.h \
    decaf/nio/ShortBuffer.h \
    decaf/nio/BufferOverflowException.h \
    decaf/nio/BufferUnderflowException.h \
    decaf/nio/InvalidMarkException.h \
    decaf/nio/ReadOnlyBufferException.h

#    activemq/wireformat/stomp/StompWireFormat.h \
#    activemq/wireformat/stomp/StompWireFormatFactory.h \
#    activemq/wireformat/stomp/StompResponseBuilder.h \
#    activemq/wireformat/stomp/StompFrame.h \
#    activemq/wireformat/stomp/marshal/Marshalable.h \
#    activemq/wireformat/stomp/marshal/MarshalException.h \
#    activemq/wireformat/stomp/marshal/Marshaler.h \
#    activemq/wireformat/stomp/commands/CommandConstants.h

include activemq/commands/srcmakefile.mk
include activemq/wireformat/openwire/marshal/v1/srcmakefile.mk
include activemq/wireformat/openwire/marshal/v2/srcmakefile.mk
include activemq/wireformat/openwire/marshal/v3/srcmakefile.mk

##
## Compiler / Linker Info
##

lib_LTLIBRARIES= libactivemq-cpp.la
libactivemq_cpp_la_SOURCES= $(h_sources) $(cc_sources)
libactivemq_cpp_la_CXXFLAGS= $(AMQ_CXXFLAGS)
libactivemq_cpp_la_LDFLAGS= -version-info $(ACTIVEMQ_LIBRARY_VERSION)
libactivemq_cpp_la_LIBADD= $(AMQ_LIBS)

##
## Packaging Info
##
library_includedir=$(includedir)/$(ACTIVEMQ_LIBRARY_NAME)-$(ACTIVEMQ_VERSION)
nobase_library_include_HEADERS = $(h_sources)
