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
        CPPUNIT_TEST( testConstructor );
        CPPUNIT_TEST( testSimpleTasks );
        CPPUNIT_TEST( testSimpleTasksCallerOwns );
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
        CPPUNIT_TEST( testGetTaskCount );
        CPPUNIT_TEST( testIsShutdown );
        CPPUNIT_TEST( testIsTerminated );
        CPPUNIT_TEST( testIsTerminating );
        CPPUNIT_TEST( testGetQueue );
        CPPUNIT_TEST( testRemove );
        CPPUNIT_TEST( testShutDownNow );
        CPPUNIT_TEST( testConstructor1 );
        CPPUNIT_TEST( testConstructor2 );
        CPPUNIT_TEST( testConstructor3 );
        CPPUNIT_TEST( testConstructor4 );
        CPPUNIT_TEST( testConstructor5 );
        CPPUNIT_TEST( testConstructor6 );
        CPPUNIT_TEST( testConstructor7 );
        CPPUNIT_TEST( testConstructor8 );
        CPPUNIT_TEST( testConstructor9 );
        CPPUNIT_TEST( testConstructor10 );
        CPPUNIT_TEST( testConstructor11 );
        CPPUNIT_TEST( testConstructor12 );
        CPPUNIT_TEST( testConstructor13 );
        CPPUNIT_TEST( testConstructor14 );
        CPPUNIT_TEST( testConstructor15 );
        CPPUNIT_TEST( testConstructor16 );
        CPPUNIT_TEST( testConstructor17 );
        CPPUNIT_TEST( testConstructor18 );
        CPPUNIT_TEST( testConstructor19 );
        CPPUNIT_TEST( testConstructor20 );
        CPPUNIT_TEST( testConstructorNullPointerException1 );
        CPPUNIT_TEST( testConstructorNullPointerException2 );
        CPPUNIT_TEST( testConstructorNullPointerException3 );
        CPPUNIT_TEST( testConstructorNullPointerException4 );
        CPPUNIT_TEST( testConstructorNullPointerException5 );
        CPPUNIT_TEST( testConstructorNullPointerException6 );
        CPPUNIT_TEST( testConstructorNullPointerException7 );
        CPPUNIT_TEST( testConstructorNullPointerException8 );
        CPPUNIT_TEST( testSaturatedExecute1 );
        CPPUNIT_TEST( testSaturatedExecute2 );
        CPPUNIT_TEST( testSaturatedExecute3 );
        CPPUNIT_TEST( testSaturatedExecute4 );
        CPPUNIT_TEST( testRejectedExecutionExceptionOnShutdown );
        CPPUNIT_TEST( testCallerRunsOnShutdown );
        CPPUNIT_TEST( testDiscardOnShutdown );
        CPPUNIT_TEST( testDiscardOldestOnShutdown );
        CPPUNIT_TEST( testExecuteNull );
        CPPUNIT_TEST( testCorePoolSizeIllegalArgumentException );
        CPPUNIT_TEST( testMaximumPoolSizeIllegalArgumentException1 );
        CPPUNIT_TEST( testMaximumPoolSizeIllegalArgumentException2 );
        CPPUNIT_TEST( testKeepAliveTimeIllegalArgumentException );
        CPPUNIT_TEST( testTerminated );
        CPPUNIT_TEST( testBeforeAfter );
        CPPUNIT_TEST( testConcurrentRandomDelayedThreads );
        CPPUNIT_TEST( testRapidCreateAndDestroyExecutor );
        CPPUNIT_TEST_SUITE_END();

    private:

        decaf::util::concurrent::Mutex myMutex;

    public:

        ThreadPoolExecutorTest() : myMutex() {}
        virtual ~ThreadPoolExecutorTest() {}

        void testConstructor();
        void testSimpleTasks();
        void testSimpleTasksCallerOwns();
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
        void testGetQueue();
        void testRemove();
        void testShutDownNow();
        void testConstructor1();
        void testConstructor2();
        void testConstructor3();
        void testConstructor4();
        void testConstructor5();
        void testConstructor6();
        void testConstructor7();
        void testConstructor8();
        void testConstructor9();
        void testConstructor10();
        void testConstructor11();
        void testConstructor12();
        void testConstructor13();
        void testConstructor14();
        void testConstructor15();
        void testConstructor16();
        void testConstructor17();
        void testConstructor18();
        void testConstructor19();
        void testConstructor20();
        void testConstructorNullPointerException1();
        void testConstructorNullPointerException2();
        void testConstructorNullPointerException3();
        void testConstructorNullPointerException4();
        void testConstructorNullPointerException5();
        void testConstructorNullPointerException6();
        void testConstructorNullPointerException7();
        void testConstructorNullPointerException8();
        void testSaturatedExecute1();
        void testSaturatedExecute2();
        void testSaturatedExecute3();
        void testSaturatedExecute4();
        void testRejectedExecutionExceptionOnShutdown();
        void testCallerRunsOnShutdown();
        void testDiscardOnShutdown();
        void testDiscardOldestOnShutdown();
        void testExecuteNull();
        void testCorePoolSizeIllegalArgumentException();
        void testMaximumPoolSizeIllegalArgumentException1();
        void testMaximumPoolSizeIllegalArgumentException2();
        void testKeepAliveTimeIllegalArgumentException();
        void testTerminated();
        void testBeforeAfter();
        void testConcurrentRandomDelayedThreads();
        void testRapidCreateAndDestroyExecutor();

    };

}}}

#endif /*_DECAF_UTIL_CONCURRENT_THREADPOOLEXECUTORTEST_H_*/
