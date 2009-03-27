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

#ifndef _DECAF_UTIL_CONCURRENT_MUTEXTEST_H_
#define _DECAF_UTIL_CONCURRENT_MUTEXTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <decaf/lang/Thread.h>
#include <decaf/lang/Runnable.h>
#include <decaf/util/concurrent/Concurrent.h>
#include <decaf/util/concurrent/Mutex.h>
#include <decaf/util/Random.h>
#include <time.h>

namespace decaf{
namespace util{
namespace concurrent{

    class MutexTest : public CppUnit::TestFixture {

        CPPUNIT_TEST_SUITE( MutexTest );
        CPPUNIT_TEST( test );
        CPPUNIT_TEST( testWait );
        CPPUNIT_TEST( testTimedWait );
        CPPUNIT_TEST( testNotify );
        CPPUNIT_TEST( testNotifyAll );
        CPPUNIT_TEST( testRecursiveLock );
        CPPUNIT_TEST( testDoubleLock );
        CPPUNIT_TEST( testStressMutex );
        CPPUNIT_TEST_SUITE_END();

    public:

        virtual ~MutexTest(){}
        virtual void setUp(){}
        virtual void tearDown(){}

        void testTimedWait();
        void testWait();
        void test();
        void testNotify();
        void testNotifyAll();
        void testRecursiveLock();
        void testDoubleLock();
        void testStressMutex();

    };

}}}

#endif /*_DECAF_UTIL_CONCURRENT_MUTEXTEST_H_*/
