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

#include <decaf/util/concurrent/ExecutorsTestSupport.h>
#include <decaf/util/concurrent/CountDownLatch.h>
#include <decaf/util/concurrent/Concurrent.h>
#include <decaf/lang/Thread.h>
#include <decaf/util/concurrent/ThreadPoolExecutor.h>
#include <decaf/util/concurrent/Mutex.h>
#include <decaf/util/Config.h>

namespace decaf{
namespace util{
namespace concurrent{

    class ThreadPoolExecutorTest : public ExecutorsTestSupport {
    private:

        CPPUNIT_TEST_SUITE( ThreadPoolExecutorTest );
        CPPUNIT_TEST( testConstructor1 );
        CPPUNIT_TEST( testSimpleTasks );
        CPPUNIT_TEST( testMoreTasksThanMaxPoolSize );
        CPPUNIT_TEST( testTasksThatThrow );
        CPPUNIT_TEST( testAwaitTermination );
        CPPUNIT_TEST( testPrestartCoreThread );
        CPPUNIT_TEST( testPrestartAllCoreThreads );
        CPPUNIT_TEST( testGetCompletedTaskCount );
        CPPUNIT_TEST( testGetCorePoolSize );
        CPPUNIT_TEST( testGetKeepAliveTime );
        CPPUNIT_TEST( testGetThreadFactory );
        CPPUNIT_TEST( testSetThreadFactory );
        CPPUNIT_TEST( testSetThreadFactoryNull );
        CPPUNIT_TEST( testGetRejectedExecutionHandler );
        CPPUNIT_TEST( testSetRejectedExecutionHandler );
        CPPUNIT_TEST( testSetRejectedExecutionHandlerNull );
        CPPUNIT_TEST( testGetLargestPoolSize );
        CPPUNIT_TEST( testGetMaximumPoolSize );
        CPPUNIT_TEST( testGetPoolSize );
        //CPPUNIT_TEST( testGetTaskCount );
        CPPUNIT_TEST( testIsShutdown );
        CPPUNIT_TEST( testIsTerminated );
        CPPUNIT_TEST( testIsTerminating );
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
        void testPrestartCoreThread();
        void testPrestartAllCoreThreads();
        void testGetCompletedTaskCount();
        void testGetCorePoolSize();
        void testGetKeepAliveTime();
        void testGetThreadFactory();
        void testSetThreadFactory();
        void testSetThreadFactoryNull();
        void testGetRejectedExecutionHandler();
        void testSetRejectedExecutionHandler();
        void testSetRejectedExecutionHandlerNull();
        void testGetLargestPoolSize();
        void testGetMaximumPoolSize();
        void testGetPoolSize();
        void testGetTaskCount();
        void testIsShutdown();
        void testIsTerminated();
        void testIsTerminating();

    };

}}}

#endif /*_DECAF_UTIL_CONCURRENT_THREADPOOLEXECUTORTEST_H_*/
