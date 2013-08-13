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

#ifndef _DECAF_UTIL_CONCURRENT_LOCKS_REENTRANTREADWRITELOCKTEST_H_
#define _DECAF_UTIL_CONCURRENT_LOCKS_REENTRANTREADWRITELOCKTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <decaf/util/concurrent/ExecutorsTestSupport.h>

namespace decaf {
namespace util {
namespace concurrent {
namespace locks {

    class ReentrantReadWriteLockTest : public ExecutorsTestSupport {

        CPPUNIT_TEST_SUITE( ReentrantReadWriteLockTest );
        CPPUNIT_TEST( testConstructor );
        CPPUNIT_TEST( testLock );
        CPPUNIT_TEST( testFairLock );
        CPPUNIT_TEST( testGetWriteHoldCount );
        CPPUNIT_TEST( testUnlockIllegalMonitorStateException );
        CPPUNIT_TEST( testReadHoldingWriteLock );
        CPPUNIT_TEST( testReadHoldingWriteLock2 );
        CPPUNIT_TEST( testReadHoldingWriteLock3 );
        CPPUNIT_TEST( testReadHoldingWriteLockFair );
        CPPUNIT_TEST( testAwaitIllegalMonitor );
        CPPUNIT_TEST( testSignalIllegalMonitor );
        CPPUNIT_TEST( testAwaitNanosTimeout );
        CPPUNIT_TEST( testAwaitTimeout );
        CPPUNIT_TEST( testAwaitUntilTimeout );
        CPPUNIT_TEST( testHasQueuedThreadNPE );
        CPPUNIT_TEST( testHasWaitersNPE );
        CPPUNIT_TEST( testGetWaitQueueLengthNPE );
        CPPUNIT_TEST( testGetWaitingThreadsNPE );
        CPPUNIT_TEST( testHasWaitersIAE );
        CPPUNIT_TEST( testHasWaitersIMSE );
        CPPUNIT_TEST( testGetWaitQueueLengthIAE );
        CPPUNIT_TEST( testGetWaitQueueLengthIMSE );
        CPPUNIT_TEST( testGetWaitingThreadsIAE );
        CPPUNIT_TEST( testGetWaitingThreadsIMSE );
        CPPUNIT_TEST( testToString );
        CPPUNIT_TEST( testReadLockToString );
        CPPUNIT_TEST( testWriteLockToString );
        CPPUNIT_TEST( testWriteLockInterruptiblyInterrupted );
        CPPUNIT_TEST( testWriteTryLockInterrupted );
        CPPUNIT_TEST( testReadLockInterruptiblyInterrupted );
        CPPUNIT_TEST( testReadTryLockInterrupted );
        CPPUNIT_TEST( testWriteTryLockWhenLocked );
        CPPUNIT_TEST( testReadTryLockWhenLocked );
        CPPUNIT_TEST( testMultipleReadLocks );
        CPPUNIT_TEST( testReadAfterWriteLock );
        CPPUNIT_TEST( testWriteHoldingWriteLock4 );
        CPPUNIT_TEST( testReadHoldingWriteLockFair2 );
        CPPUNIT_TEST( testReadHoldingWriteLockFair3 );
        CPPUNIT_TEST( testWriteHoldingWriteLockFair4 );
        CPPUNIT_TEST( testTryLockWhenReadLocked );
        CPPUNIT_TEST( testWriteAfterMultipleReadLocks );
        CPPUNIT_TEST( testTryLockWhenReadLockedFair );
        CPPUNIT_TEST( testWriteTryLockWhenReadLockedFair );
        CPPUNIT_TEST( testWriteTryLockTimeout );
        CPPUNIT_TEST( testReadTryLockTimeout );
        CPPUNIT_TEST( testWriteLockInterruptibly );
        CPPUNIT_TEST( testReadLockInterruptibly );
        CPPUNIT_TEST( testAwait );
        CPPUNIT_TEST( testAwaitUninterruptibly );
        CPPUNIT_TEST( testAwaitInterrupt );
        CPPUNIT_TEST( testAwaitNanosInterrupt );
        CPPUNIT_TEST( testAwaitUntilInterrupt );
        CPPUNIT_TEST( testSignalAll );
        CPPUNIT_TEST( testHasQueuedThreads );
        CPPUNIT_TEST( testHasQueuedThread );
        CPPUNIT_TEST( testGetQueueLength );
        CPPUNIT_TEST( testGetQueuedThreads );
        CPPUNIT_TEST( testHasWaiters );
        CPPUNIT_TEST( testGetWaitQueueLength );
        CPPUNIT_TEST( testGetWaitingThreads );
        CPPUNIT_TEST( testMultipleReaderThreads );
        CPPUNIT_TEST_SUITE_END();

    public:

        ReentrantReadWriteLockTest();
        virtual ~ReentrantReadWriteLockTest();

        void testConstructor();
        void testLock();
        void testFairLock();
        void testWriteLockInterruptiblyInterrupted();
        void testGetWriteHoldCount();
        void testUnlockIllegalMonitorStateException();
        void testReadHoldingWriteLock();
        void testReadHoldingWriteLock2();
        void testReadHoldingWriteLock3();
        void testReadHoldingWriteLockFair();
        void testAwaitIllegalMonitor();
        void testSignalIllegalMonitor();
        void testAwaitNanosTimeout();
        void testAwaitTimeout();
        void testAwaitUntilTimeout();
        void testHasQueuedThreadNPE();
        void testHasWaitersNPE();
        void testGetWaitQueueLengthNPE();
        void testGetWaitingThreadsNPE();
        void testHasWaitersIAE();
        void testHasWaitersIMSE();
        void testGetWaitQueueLengthIAE();
        void testGetWaitQueueLengthIMSE();
        void testGetWaitingThreadsIAE();
        void testGetWaitingThreadsIMSE();
        void testToString();
        void testReadLockToString();
        void testWriteLockToString();
        void testWriteTryLockInterrupted();
        void testReadLockInterruptiblyInterrupted();
        void testReadTryLockInterrupted();
        void testWriteTryLockWhenLocked();
        void testReadTryLockWhenLocked();
        void testMultipleReadLocks();
        void testWriteAfterMultipleReadLocks();
        void testReadAfterWriteLock();
        void testWriteHoldingWriteLock4();
        void testReadHoldingWriteLockFair2();
        void testReadHoldingWriteLockFair3();
        void testWriteHoldingWriteLockFair4();
        void testTryLockWhenReadLocked();
        void testWriteTryLockWhenReadLocked();
        void testTryLockWhenReadLockedFair();
        void testWriteTryLockWhenReadLockedFair();
        void testWriteTryLockTimeout();
        void testReadTryLockTimeout();
        void testWriteLockInterruptibly();
        void testReadLockInterruptibly();
        void testAwait();
        void testAwaitUninterruptibly();
        void testAwaitInterrupt();
        void testAwaitNanosInterrupt();
        void testAwaitUntilInterrupt();
        void testSignalAll();
        void testHasQueuedThreads();
        void testHasQueuedThread();
        void testGetQueueLength();
        void testGetQueuedThreads();
        void testHasWaiters();
        void testGetWaitQueueLength();
        void testGetWaitingThreads();
        void testMultipleReaderThreads();

    };

}}}}

#endif /* _DECAF_UTIL_CONCURRENT_LOCKS_REENTRANTREADWRITELOCKTEST_H_ */
