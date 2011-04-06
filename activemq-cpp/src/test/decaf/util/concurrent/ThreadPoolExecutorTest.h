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

#ifndef _DECAF_UTIL_CONCURRENT_THREADPOOLEXECUTORTEST_H_
#define _DECAF_UTIL_CONCURRENT_THREADPOOLEXECUTORTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <decaf/util/concurrent/CountDownLatch.h>
#include <decaf/util/concurrent/Concurrent.h>
#include <decaf/lang/Thread.h>
#include <decaf/util/concurrent/ThreadPoolExecutor.h>
#include <decaf/util/concurrent/Mutex.h>
#include <decaf/util/Config.h>

namespace decaf{
namespace util{
namespace concurrent{

    class ThreadPoolExecutorTest : public CppUnit::TestFixture {
    private:

        CPPUNIT_TEST_SUITE( ThreadPoolExecutorTest );
        CPPUNIT_TEST( testConstructor1 );
        CPPUNIT_TEST( testSimpleTasks );
        CPPUNIT_TEST( testMoreTasksThanMaxPoolSize );
        CPPUNIT_TEST( testTasksThatThrow );
        CPPUNIT_TEST( testAwaitTermination );
        CPPUNIT_TEST_SUITE_END();

    private:

        decaf::util::concurrent::Mutex myMutex;

    public:

        ThreadPoolExecutorTest() {}
        virtual ~ThreadPoolExecutorTest() {}

        void testConstructor1();
        void testSimpleTasks();
        void testMoreTasksThanMaxPoolSize();
        void testTasksThatThrow();
        void testAwaitTermination();

    };

}}}

#endif /*_DECAF_UTIL_CONCURRENT_THREADPOOLEXECUTORTEST_H_*/
