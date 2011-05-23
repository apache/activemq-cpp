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

#ifndef _DECAF_UTIL_CONCURRENT_LOCKS_REENTRANTLOCKTEST_H_
#define _DECAF_UTIL_CONCURRENT_LOCKS_REENTRANTLOCKTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <decaf/util/concurrent/ExecutorsTestSupport.h>

namespace decaf {
namespace util {
namespace concurrent {
namespace locks {

    class ReentrantLockTest : public ExecutorsTestSupport {

        CPPUNIT_TEST_SUITE( ReentrantLockTest );
        CPPUNIT_TEST( testConstructor );
        CPPUNIT_TEST( testLock );
        CPPUNIT_TEST( testFairLock );
        CPPUNIT_TEST( testUnlockIllegalMonitorStateException );
        CPPUNIT_TEST( testTryLock );
        CPPUNIT_TEST( testhasQueuedThreads );
        CPPUNIT_TEST( testGetQueueLength );
        CPPUNIT_TEST( testGetQueueLengthFair );
        CPPUNIT_TEST( testHasQueuedThreadNPE );
        CPPUNIT_TEST( testHasQueuedThread );
        CPPUNIT_TEST( testGetQueuedThreads );
        CPPUNIT_TEST( testInterruptedException2 );
        CPPUNIT_TEST( testTryLockWhenLocked );
        CPPUNIT_TEST( testTryLockTimeout );
        CPPUNIT_TEST( testGetHoldCount );
        CPPUNIT_TEST( testIsLocked );
        CPPUNIT_TEST( testLockInterruptibly1 );
        CPPUNIT_TEST( testLockInterruptibly2 );
        CPPUNIT_TEST( testAwaitIllegalMonitor );
        CPPUNIT_TEST( testSignalIllegalMonitor );
        CPPUNIT_TEST( testAwaitNanosTimeout );
        CPPUNIT_TEST( testAwaitTimeout );
        CPPUNIT_TEST( testAwaitUntilTimeout );
        CPPUNIT_TEST( testAwait );
        CPPUNIT_TEST( testHasWaitersNPE );
        CPPUNIT_TEST( testGetWaitQueueLengthNPE );
        CPPUNIT_TEST( testGetWaitingThreadsNPE );
        CPPUNIT_TEST( testHasWaitersIAE );
        CPPUNIT_TEST( testHasWaitersIMSE );
        CPPUNIT_TEST( testGetWaitQueueLengthIAE );
        CPPUNIT_TEST( testGetWaitQueueLengthIMSE );
        CPPUNIT_TEST( testGetWaitingThreadsIAE );
        CPPUNIT_TEST( testGetWaitingThreadsIMSE );
        CPPUNIT_TEST( testHasWaiters );
        CPPUNIT_TEST( testGetWaitQueueLength );
        CPPUNIT_TEST( testGetWaitingThreads );
        CPPUNIT_TEST( testAwaitUninterruptibly );
        CPPUNIT_TEST( testAwaitInterrupt );
        CPPUNIT_TEST( testAwaitNanosInterrupt );
        CPPUNIT_TEST( testAwaitUntilInterrupt );
        CPPUNIT_TEST( testSignalAll );
        CPPUNIT_TEST( testAwaitLockCount );
        CPPUNIT_TEST( testToString );
        CPPUNIT_TEST_SUITE_END();

    public:

        ReentrantLockTest();
        virtual ~ReentrantLockTest();

        void testConstructor();
        void testLock();
        void testFairLock();
        void testUnlockIllegalMonitorStateException();
        void testTryLock();
        void testhasQueuedThreads();
        void testGetQueueLength();
        void testGetQueueLengthFair();
        void testHasQueuedThreadNPE();
        void testHasQueuedThread();
        void testGetQueuedThreads();
        void testInterruptedException2();
        void testTryLockWhenLocked();
        void testTryLockTimeout();
        void testGetHoldCount();
        void testIsLocked();
        void testLockInterruptibly1();
        void testLockInterruptibly2();
        void testAwaitIllegalMonitor();
        void testSignalIllegalMonitor();
        void testAwaitNanosTimeout();
        void testAwaitTimeout();
        void testAwaitUntilTimeout();
        void testAwait();
        void testHasWaitersNPE();
        void testGetWaitQueueLengthNPE();
        void testGetWaitingThreadsNPE();
        void testHasWaitersIAE();
        void testHasWaitersIMSE();
        void testGetWaitQueueLengthIAE();
        void testGetWaitQueueLengthIMSE();
        void testGetWaitingThreadsIAE();
        void testGetWaitingThreadsIMSE();
        void testHasWaiters();
        void testGetWaitQueueLength();
        void testGetWaitingThreads();
        void testAwaitUninterruptibly();
        void testAwaitInterrupt();
        void testAwaitNanosInterrupt();
        void testAwaitUntilInterrupt();
        void testSignalAll();
        void testAwaitLockCount();
        void testToString();

    };

}}}}

#endif /* _DECAF_UTIL_CONCURRENT_LOCKS_REENTRANTLOCKTEST_H_ */
