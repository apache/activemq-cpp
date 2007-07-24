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
  decaf/lang/BooleanTest.cpp \
  decaf/lang/IntegerTest.cpp \
  decaf/lang/LongTest.cpp \
  decaf/lang/ThreadTest.cpp \
  decaf/lang/ExceptionTest.cpp \
  decaf/io/BufferedInputStreamTest.cpp \
  decaf/io/BufferedOutputStreamTest.cpp \
  decaf/io/ByteArrayInputStreamTest.cpp \
  decaf/io/ByteArrayOutputStreamTest.cpp \
  decaf/io/DataInputStreamTest.cpp \
  decaf/io/DataOutputStreamTest.cpp \
  decaf/net/SocketFactoryTest.cpp \
  decaf/net/SocketTest.cpp \
  decaf/util/StringTokenizerTest.cpp \
  decaf/util/Endian.cpp \
  decaf/util/DateTest.cpp \
  decaf/util/GuidTest.cpp \
  decaf/util/RandomTest.cpp \
  decaf/util/MapTest.cpp \
  decaf/util/QueueTest.cpp \
  decaf/util/SetTest.cpp \
  decaf/util/concurrent/CountDownLatchTest.cpp \
  decaf/util/concurrent/MutexTest.cpp \
  decaf/util/concurrent/ThreadPoolTest.cpp \
  main.cpp

h_sources = \
  decaf/lang/BooleanTest.h \
  decaf/lang/IntegerTest.h \
  decaf/lang/LongTest.h \
  decaf/lang/ThreadTest.h \
  decaf/lang/ExceptionTest.h \
  decaf/io/BufferedInputStreamTest.h \
  decaf/io/BufferedOutputStreamTest.h \
  decaf/io/ByteArrayInputStreamTest.h \
  decaf/io/ByteArrayOutputStreamTest.h \
  decaf/io/DataInputStreamTest.h \
  decaf/io/DataOutputStreamTest.h \
  decaf/net/SocketFactoryTest.h \
  decaf/net/SocketTest.h \
  decaf/util/StringTokenizerTest.h \
  decaf/util/Endian.h \
  decaf/util/DateTest.h \
  decaf/util/GuidTest.h \
  decaf/util/RandomTest.h \
  decaf/util/MapTest.h \
  decaf/util/QueueTest.h \
  decaf/util/SetTest.h \
  decaf/util/concurrent/CountDownLatchTest.h \
  decaf/util/concurrent/MutexTest.h \
  decaf/util/concurrent/ThreadPoolTest.h

## Compile this as part of make check
check_PROGRAMS = decaf-test
## Also run the tests as part of make check
TESTS = $(check_PROGRAMS)

## 
## Compiler/Linker Options
##
decaf_test_SOURCES = $(cc_sources)
decaf_test_CXXFLAGS = $(DECAF_TEST_CXXFLAGS) -I$(srcdir)/../main @CPPUNIT_CFLAGS@
decaf_test_LDADD= $(DECAF_TEST_LIBS) @CPPUNIT_LIBS@ 
