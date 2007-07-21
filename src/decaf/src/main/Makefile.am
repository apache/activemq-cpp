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
   decaf/net/ServerSocket.cpp \
   decaf/net/SocketOutputStream.cpp \
   decaf/net/BufferedSocket.cpp \
   decaf/net/SocketInputStream.cpp \
   decaf/net/SocketError.cpp \
   decaf/net/TcpSocket.cpp \
   decaf/net/SocketFactory.cpp \
   decaf/lang/Exception.cpp \
   decaf/lang/Thread.cpp \
   decaf/lang/Boolean.cpp \
   decaf/lang/Integer.cpp \
   decaf/lang/Long.cpp \
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
   decaf/util/concurrent/ThreadPool.cpp \
   decaf/util/Date.cpp \
   decaf/util/Guid.cpp \
   decaf/util/StringTokenizer.cpp \
   decaf/util/Random.cpp \
   decaf/util/logging/LoggerHierarchy.cpp \
   decaf/util/logging/Logger.cpp \
   decaf/util/logging/LogWriter.cpp \
   decaf/util/logging/SimpleLogger.cpp \
   decaf/util/logging/LogManager.cpp

h_sources = \
   decaf/net/BufferedSocket.h \
   decaf/net/SocketException.h \
   decaf/net/TcpSocket.h \
   decaf/net/SocketError.h \
   decaf/net/SocketOutputStream.h \
   decaf/net/Socket.h \
   decaf/net/SocketInputStream.h \
   decaf/net/ServerSocket.h \
   decaf/net/SocketFactory.h \
   decaf/lang/Boolean.h \
   decaf/lang/Throwable.h \
   decaf/lang/Exception.h \
   decaf/lang/Character.h \
   decaf/lang/Integer.h \
   decaf/lang/Thread.h \
   decaf/lang/Number.h \
   decaf/lang/Runnable.h \
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
   decaf/lang/Math.h \
   decaf/lang/Long.h \
   decaf/io/BufferedOutputStream.h \
   decaf/io/StandardErrorOutputStream.h \
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
   decaf/util/concurrent/Lock.h \
   decaf/util/concurrent/Concurrent.h \
   decaf/util/concurrent/CountDownLatch.h \
   decaf/util/concurrent/Synchronizable.h \
   decaf/util/concurrent/Mutex.h \
   decaf/util/concurrent/PooledThread.h \
   decaf/util/concurrent/PooledThreadListener.h \
   decaf/util/concurrent/TaskListener.h \
   decaf/util/concurrent/ThreadPool.h \
   decaf/util/Date.h \
   decaf/util/Guid.h \
   decaf/util/Iterator.h \
   decaf/util/Collection.h \
   decaf/util/Map.h \
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
   decaf/util/logging/PropertiesChangeListener.h

##
## LibTool build step
##
lib_LTLIBRARIES= libdecaf.la
libdecaf_la_SOURCES= $(h_sources) $(cc_sources)
libdecaf_la_CXXFLAGS= $(DECAF_CXXFLAGS)
libdecaf_la_LDFLAGS= -version-info $(DECAF_LIBRARY_VERSION) -release $(DECAF_VERSION)
libdecaf_la_LIBADD= $(DECAF_LIBS) $(APR_LIBS)

##
## Packaging Info
##
library_includedir=$(includedir)/$(DECAF_LIBRARY_NAME)-$(DECAF_VERSION)
nobase_library_include_HEADERS = $(h_sources)
