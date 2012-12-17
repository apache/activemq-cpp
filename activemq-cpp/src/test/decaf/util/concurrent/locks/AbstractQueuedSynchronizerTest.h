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

#ifndef _DECAF_UTIL_CONCURRENT_LOCKS_ABSTRACTQUEUEDSYNCHRONIZERTEST_H_
#define _DECAF_UTIL_CONCURRENT_LOCKS_ABSTRACTQUEUEDSYNCHRONIZERTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <decaf/util/concurrent/ExecutorsTestSupport.h>

namespace decaf {
namespace util {
namespace concurrent {
namespace locks {

    class AbstractQueuedSynchronizerTest : public ExecutorsTestSupport {

        CPPUNIT_TEST_SUITE( AbstractQueuedSynchronizerTest );
        CPPUNIT_TEST( testIsHeldExclusively );
        CPPUNIT_TEST( testAcquire );
        CPPUNIT_TEST( testTryAcquire );
        CPPUNIT_TEST( testhasQueuedThreads );
        CPPUNIT_TEST( testIsQueuedNPE );
        CPPUNIT_TEST( testIsQueued );
        CPPUNIT_TEST( testGetFirstQueuedThread );
        CPPUNIT_TEST( testHasContended );
        CPPUNIT_TEST( testGetQueuedThreads );
        CPPUNIT_TEST( testGetExclusiveQueuedThreads );
        CPPUNIT_TEST( testGetSharedQueuedThreads );
        CPPUNIT_TEST( testInterruptedException2 );
        CPPUNIT_TEST( testTryAcquireWhenSynced );
        CPPUNIT_TEST( testAcquireNanosTimeout );
        CPPUNIT_TEST( testGetState );
        CPPUNIT_TEST( testAcquireInterruptibly1 );
        CPPUNIT_TEST( testAcquireInterruptibly2 );
        CPPUNIT_TEST( testOwns );
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
        CPPUNIT_TEST( testToString );
        CPPUNIT_TEST( testGetStateWithReleaseShared );
        CPPUNIT_TEST( testReleaseShared );
        CPPUNIT_TEST( testAcquireSharedInterruptibly );
        CPPUNIT_TEST( testAsquireSharedTimed );
        CPPUNIT_TEST( testAcquireSharedInterruptiblyInterruptedException );
        CPPUNIT_TEST( testAcquireSharedNanosInterruptedException );
        CPPUNIT_TEST( testAcquireSharedNanosTimeout );
        CPPUNIT_TEST_SUITE_END();

    public:

        AbstractQueuedSynchronizerTest();
        virtual ~AbstractQueuedSynchronizerTest();

        void testIsHeldExclusively();
        void testAcquire();
        void testTryAcquire();
        void testhasQueuedThreads();
        void testIsQueuedNPE();
        void testIsQueued();
        void testGetFirstQueuedThread();
        void testHasContended();
        void testGetQueuedThreads();
        void testGetExclusiveQueuedThreads();
        void testGetSharedQueuedThreads();
        void testInterruptedException2();
        void testTryAcquireWhenSynced();
        void testAcquireNanosTimeout();
        void testGetState();
        void testAcquireInterruptibly1();
        void testAcquireInterruptibly2();
        void testOwns();
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
        void testToString();
        void testGetStateWithReleaseShared();
        void testReleaseShared();
        void testAcquireSharedInterruptibly();
        void testAsquireSharedTimed();
        void testAcquireSharedInterruptiblyInterruptedException();
        void testAcquireSharedNanosInterruptedException();
        void testAcquireSharedNanosTimeout();

    };

}}}}

#endif /* _DECAF_UTIL_CONCURRENT_LOCKS_ABSTRACTQUEUEDSYNCHRONIZERTEST_H_ */
