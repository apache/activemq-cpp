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
  activemq/util/PrimitiveMapBenchmark.cpp \
  decaf/lang/BooleanBenchmark.cpp \
  decaf/util/PropertiesBenchmark.cpp \
  decaf/util/QueueBenchmark.cpp \
  decaf/util/SetBenchmark.cpp \
  decaf/util/MapBenchmark.cpp \
  decaf/io/DataInputStreamBenchmark.cpp \
  decaf/io/DataOutputStreamBenchmark.cpp \
  benchmark/PerformanceTimer.cpp \
  testRegistry.cpp \
  main.cpp

hh_sources = \
  activemq/util/PrimitiveMapBenchmark.h \
  decaf/lang/BooleanBenchmark.h \
  decaf/util/PropertiesBenchmark.h \
  decaf/util/QueueBenchmark.h \
  decaf/util/SetBenchmark.h \
  decaf/util/MapBenchmark.h \
  decaf/io/DataInputStreamBenchmark.h \
  decaf/io/DataOutputStreamBenchmark.h \
  benchmark/BenchmarkBase.h \
  benchmark/PerformanceTimer.h

## Compile this as part of make check
check_PROGRAMS = activemq-test-benchmarks

## 
## Compiler/Linker Options
##
activemq_test_benchmarks_SOURCES = $(cc_sources)
activemq_test_benchmarks_CXXFLAGS = $(AMQ_TEST_CXXFLAGS) $(DECAF_TEST_CXXFLAGS) -I$(srcdir)/../main @CPPUNIT_CFLAGS@
activemq_test_benchmarks_LDADD= $(AMQ_TEST_LIBS) $(DECAF_TEST_LIBS) @CPPUNIT_LIBS@

