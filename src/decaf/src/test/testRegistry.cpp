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
//
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

#include <decaf/lang/BooleanTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::lang::BooleanTest );
#include <decaf/lang/ExceptionTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::lang::ExceptionTest );
#include <decaf/lang/IntegerTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::lang::IntegerTest );
#include <decaf/lang/LongTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::lang::LongTest );
#include <decaf/lang/ThreadTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::lang::ThreadTest );

#include <decaf/net/SocketFactoryTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::net::SocketFactoryTest );
#include <decaf/net/SocketTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::net::SocketTest );

#include <decaf/util/concurrent/CountDownLatchTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::util::concurrent::CountDownLatchTest );
#include <decaf/util/concurrent/MutexTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::util::concurrent::MutexTest );
#include <decaf/util/concurrent/ThreadPoolTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::util::concurrent::ThreadPoolTest );

#include <decaf/util/DateTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::util::DateTest );
#include <decaf/util/GuidTest.h>
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::util::GuidTest );
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
