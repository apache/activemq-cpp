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

#include "ReentrantReadWriteLockTest.h"

#include <decaf/util/Date.h>
#include <decaf/lang/Runnable.h>
#include <decaf/util/concurrent/locks/Lock.h>
#include <decaf/util/concurrent/locks/ReentrantReadWriteLock.h>

using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace decaf::util::concurrent::locks;

////////////////////////////////////////////////////////////////////////////////
namespace {

    const int SIZE = 256;

    class InterruptibleLockRunnable : public Runnable {
    public:

        ReentrantReadWriteLockTest* test;
        ReentrantReadWriteLock* lock;

        InterruptibleLockRunnable(ReentrantReadWriteLockTest* test, ReentrantReadWriteLock* lock) :
            Runnable(), test(test), lock(lock) {}
        virtual ~InterruptibleLockRunnable() {}

        virtual void run() {
            try {
                lock->writeLock().lockInterruptibly();
            } catch (InterruptedException& success){
            }
        }
    };

    class InterruptedLockRunnable : public Runnable {
    public:

        ReentrantReadWriteLockTest* test;
        ReentrantReadWriteLock* lock;

        InterruptedLockRunnable(ReentrantReadWriteLockTest* test, ReentrantReadWriteLock* lock) :
            Runnable(), test(test), lock(lock) {}
        virtual ~InterruptedLockRunnable() {}

        virtual void run() {
            try {
                lock->writeLock().lockInterruptibly();
                test->threadShouldThrow();
            } catch (InterruptedException& success){
            }
        }
    };

    class PublicReentrantReadWriteLock : public ReentrantReadWriteLock {
    public:

        PublicReentrantReadWriteLock() : ReentrantReadWriteLock() {}

        Collection<Thread*>* getQueuedThreadsPublic() const {
            return getQueuedThreads();
        }

        Collection<Thread*>* getWaitingThreadsPublic(Condition* c) const {
            return getWaitingThreads(c);
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
ReentrantReadWriteLockTest::ReentrantReadWriteLockTest() {
}

////////////////////////////////////////////////////////////////////////////////
ReentrantReadWriteLockTest::~ReentrantReadWriteLockTest() {
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantReadWriteLockTest::testConstructor() {

    ReentrantReadWriteLock rl;
    CPPUNIT_ASSERT(!rl.isFair());
    CPPUNIT_ASSERT(!rl.isWriteLocked());
    CPPUNIT_ASSERT_EQUAL(0, rl.getReadLockCount());

    ReentrantReadWriteLock r2(true);
    CPPUNIT_ASSERT(r2.isFair());
    CPPUNIT_ASSERT(!r2.isWriteLocked());
    CPPUNIT_ASSERT_EQUAL(0, r2.getReadLockCount());
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantReadWriteLockTest::testLock() {

    ReentrantReadWriteLock rl;
    rl.writeLock().lock();
    CPPUNIT_ASSERT(rl.isWriteLocked());
    CPPUNIT_ASSERT(rl.isWriteLockedByCurrentThread());
    CPPUNIT_ASSERT_EQUAL(0, rl.getReadLockCount());
    rl.writeLock().unlock();
    CPPUNIT_ASSERT(!rl.isWriteLocked());
    CPPUNIT_ASSERT(!rl.isWriteLockedByCurrentThread());
    CPPUNIT_ASSERT_EQUAL(0, rl.getReadLockCount());
    rl.readLock().lock();
    CPPUNIT_ASSERT(!rl.isWriteLocked());
    CPPUNIT_ASSERT(!rl.isWriteLockedByCurrentThread());
    CPPUNIT_ASSERT_EQUAL(1, rl.getReadLockCount());
    rl.readLock().unlock();
    CPPUNIT_ASSERT(!rl.isWriteLocked());
    CPPUNIT_ASSERT(!rl.isWriteLockedByCurrentThread());
    CPPUNIT_ASSERT_EQUAL(0, rl.getReadLockCount());
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantReadWriteLockTest::testFairLock() {

    ReentrantReadWriteLock rl(true);
    rl.writeLock().lock();
    CPPUNIT_ASSERT(rl.isWriteLocked());
    CPPUNIT_ASSERT(rl.isWriteLockedByCurrentThread());
    CPPUNIT_ASSERT_EQUAL(0, rl.getReadLockCount());
    rl.writeLock().unlock();
    CPPUNIT_ASSERT(!rl.isWriteLocked());
    CPPUNIT_ASSERT(!rl.isWriteLockedByCurrentThread());
    CPPUNIT_ASSERT_EQUAL(0, rl.getReadLockCount());
    rl.readLock().lock();
    CPPUNIT_ASSERT(!rl.isWriteLocked());
    CPPUNIT_ASSERT(!rl.isWriteLockedByCurrentThread());
    CPPUNIT_ASSERT_EQUAL(1, rl.getReadLockCount());
    rl.readLock().unlock();
    CPPUNIT_ASSERT(!rl.isWriteLocked());
    CPPUNIT_ASSERT(!rl.isWriteLockedByCurrentThread());
    CPPUNIT_ASSERT_EQUAL(0, rl.getReadLockCount());
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantReadWriteLockTest::testGetWriteHoldCount() {
    ReentrantReadWriteLock lock;
    for (int i = 1; i <= SIZE; i++) {
        lock.writeLock().lock();
        CPPUNIT_ASSERT_EQUAL(i,lock.getWriteHoldCount());
    }
    for (int i = SIZE; i > 0; i--) {
        lock.writeLock().unlock();
        CPPUNIT_ASSERT_EQUAL(i-1,lock.getWriteHoldCount());
    }
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantReadWriteLockTest::testUnlockIllegalMonitorStateException() {

    ReentrantReadWriteLock rl;
    try {
        rl.writeLock().unlock();
        shouldThrow();
    } catch (IllegalMonitorStateException& success) {
    }
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class TestWriteLockInterruptiblyInterruptedRunnable : public Runnable {
    private:

        ReentrantReadWriteLockTest* test;
        ReentrantReadWriteLock* lock;

    public:

        TestWriteLockInterruptiblyInterruptedRunnable(ReentrantReadWriteLockTest* test, ReentrantReadWriteLock* lock) :
            Runnable(), test(test), lock(lock) {}
        virtual ~TestWriteLockInterruptiblyInterruptedRunnable() {}

        virtual void run() {
            try {
                lock->writeLock().lockInterruptibly();
                lock->writeLock().unlock();
                lock->writeLock().lockInterruptibly();
                lock->writeLock().unlock();
            } catch (InterruptedException& success) {
            }
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantReadWriteLockTest::testWriteLockInterruptiblyInterrupted() {
    ReentrantReadWriteLock lock;
    TestWriteLockInterruptiblyInterruptedRunnable runnable(this, &lock);
    Thread t(&runnable);

    try {
        lock.writeLock().lock();
        t.start();
        Thread::sleep(SHORT_DELAY_MS);
        t.interrupt();
        Thread::sleep(SHORT_DELAY_MS);
        lock.writeLock().unlock();
        t.join();
    } catch(Exception& e){
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class TestWriteTryLockInterruptedRunnable : public Runnable {
    private:

        ReentrantReadWriteLockTest* test;
        ReentrantReadWriteLock* lock;

    public:

        TestWriteTryLockInterruptedRunnable(ReentrantReadWriteLockTest* test, ReentrantReadWriteLock* lock) :
            Runnable(), test(test), lock(lock) {}
        virtual ~TestWriteTryLockInterruptedRunnable() {}

        virtual void run() {
            try {
                lock->writeLock().tryLock(1000, TimeUnit::MILLISECONDS);
            } catch (InterruptedException& success) {
            }
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantReadWriteLockTest::testWriteTryLockInterrupted() {
    ReentrantReadWriteLock lock;
    TestWriteTryLockInterruptedRunnable runnable(this, &lock);
    Thread t(&runnable);

    lock.writeLock().lock();

    try {
        t.start();
        t.interrupt();
        lock.writeLock().unlock();
        t.join();
    } catch (Exception& e) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class TestReadLockInterruptiblyInterruptedRunnable : public Runnable {
    private:

        ReentrantReadWriteLockTest* test;
        ReentrantReadWriteLock* lock;

    public:

        TestReadLockInterruptiblyInterruptedRunnable(ReentrantReadWriteLockTest* test, ReentrantReadWriteLock* lock) :
            Runnable(), test(test), lock(lock) {}
        virtual ~TestReadLockInterruptiblyInterruptedRunnable() {}

        virtual void run() {
            try {
                lock->readLock().lockInterruptibly();
            } catch (InterruptedException& success) {
            }
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantReadWriteLockTest::testReadLockInterruptiblyInterrupted() {
    ReentrantReadWriteLock lock;
    TestReadLockInterruptiblyInterruptedRunnable runnable(this, &lock);
    Thread t(&runnable);

    lock.writeLock().lock();

    try {
        t.start();
        Thread::sleep( SHORT_DELAY_MS);
        t.interrupt();
        Thread::sleep(SHORT_DELAY_MS);
        lock.writeLock().unlock();
        t.join();
    } catch (Exception& e) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class TestReadTryLockInterruptedRunnable : public Runnable {
    private:

        ReentrantReadWriteLockTest* test;
        ReentrantReadWriteLock* lock;

    public:

        TestReadTryLockInterruptedRunnable(ReentrantReadWriteLockTest* test, ReentrantReadWriteLock* lock) :
            Runnable(), test(test), lock(lock) {}
        virtual ~TestReadTryLockInterruptedRunnable() {}

        virtual void run() {
            try {
                lock->readLock().tryLock(1000, TimeUnit::MILLISECONDS);
                test->threadShouldThrow();
            } catch (InterruptedException& success) {
            }
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantReadWriteLockTest::testReadTryLockInterrupted() {
    ReentrantReadWriteLock lock;
    TestReadTryLockInterruptedRunnable runnable(this, &lock);
    Thread t(&runnable);

    lock.writeLock().lock();

    try {
        t.start();
        t.interrupt();
        t.join();
    } catch (Exception& e) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class TestWriteTryLockWhenLockedRunnable : public Runnable {
    private:

        ReentrantReadWriteLockTest* test;
        ReentrantReadWriteLock* lock;

    public:

        TestWriteTryLockWhenLockedRunnable(ReentrantReadWriteLockTest* test, ReentrantReadWriteLock* lock) :
            Runnable(), test(test), lock(lock) {}
        virtual ~TestWriteTryLockWhenLockedRunnable() {}

        virtual void run() {
            test->threadAssertFalse(lock->writeLock().tryLock());
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantReadWriteLockTest::testWriteTryLockWhenLocked() {

    ReentrantReadWriteLock lock;
    TestWriteTryLockWhenLockedRunnable runnable(this, &lock);
    Thread t(&runnable);
    lock.writeLock().lock();

    try {
        t.start();
        t.join();
        lock.writeLock().unlock();
    } catch (Exception& e) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class TestReadTryLockWhenLockedRunnable : public Runnable {
    private:

        ReentrantReadWriteLockTest* test;
        ReentrantReadWriteLock* lock;

    public:

        TestReadTryLockWhenLockedRunnable(ReentrantReadWriteLockTest* test, ReentrantReadWriteLock* lock) :
            Runnable(), test(test), lock(lock) {}
        virtual ~TestReadTryLockWhenLockedRunnable() {}

        virtual void run() {
            test->threadAssertFalse(lock->readLock().tryLock());
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantReadWriteLockTest::testReadTryLockWhenLocked() {
    ReentrantReadWriteLock lock;
    TestReadTryLockWhenLockedRunnable runnable(this, &lock);
    Thread t(&runnable);

    lock.writeLock().lock();

    try {
        t.start();
        t.join();
        lock.writeLock().unlock();
    } catch (Exception& e) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class TestMultipleReadLocksRunnable : public Runnable {
    private:

        ReentrantReadWriteLockTest* test;
        ReentrantReadWriteLock* lock;

    public:

        TestMultipleReadLocksRunnable(ReentrantReadWriteLockTest* test, ReentrantReadWriteLock* lock) :
            Runnable(), test(test), lock(lock) {}
        virtual ~TestMultipleReadLocksRunnable() {}

        virtual void run() {
            test->threadAssertTrue(lock->readLock().tryLock());
            lock->readLock().unlock();
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantReadWriteLockTest::testMultipleReadLocks() {
    ReentrantReadWriteLock lock;
    TestMultipleReadLocksRunnable runnable(this, &lock);
    Thread t(&runnable);

    lock.readLock().lock();
    try {
        t.start();
        t.join();
        lock.readLock().unlock();
    } catch (Exception& e) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class TestWriteAfterMultipleReadLocksRunnable1 : public Runnable {
    private:

        ReentrantReadWriteLockTest* test;
        ReentrantReadWriteLock* lock;

    public:

        TestWriteAfterMultipleReadLocksRunnable1(ReentrantReadWriteLockTest* test, ReentrantReadWriteLock* lock) :
            Runnable(), test(test), lock(lock) {}
        virtual ~TestWriteAfterMultipleReadLocksRunnable1() {}

        virtual void run() {
            lock->readLock().lock();
            lock->readLock().unlock();
        }
    };

    class TestWriteAfterMultipleReadLocksRunnable2 : public Runnable {
    private:

        ReentrantReadWriteLockTest* test;
        ReentrantReadWriteLock* lock;

    public:

        TestWriteAfterMultipleReadLocksRunnable2(ReentrantReadWriteLockTest* test, ReentrantReadWriteLock* lock) :
            Runnable(), test(test), lock(lock) {}
        virtual ~TestWriteAfterMultipleReadLocksRunnable2() {}

        virtual void run() {
            lock->writeLock().lock();
            lock->writeLock().unlock();
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantReadWriteLockTest::testWriteAfterMultipleReadLocks() {

    ReentrantReadWriteLock lock;
    TestWriteAfterMultipleReadLocksRunnable1 runnable1(this, &lock);
    TestWriteAfterMultipleReadLocksRunnable2 runnable2(this, &lock);
    Thread t1(&runnable1);
    Thread t2(&runnable2);

    lock.readLock().lock();

    try {
        t1.start();
        t2.start();
        Thread::sleep( SHORT_DELAY_MS);
        lock.readLock().unlock();
        t1.join(MEDIUM_DELAY_MS);
        t2.join(MEDIUM_DELAY_MS);
        CPPUNIT_ASSERT(!t1.isAlive());
        CPPUNIT_ASSERT(!t2.isAlive());
    } catch (Exception& e) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class TestReadAfterWriteLockRunnable : public Runnable {
    private:

        ReentrantReadWriteLockTest* test;
        ReentrantReadWriteLock* lock;

    public:

        TestReadAfterWriteLockRunnable(ReentrantReadWriteLockTest* test, ReentrantReadWriteLock* lock) :
            Runnable(), test(test), lock(lock) {}
        virtual ~TestReadAfterWriteLockRunnable() {}

        virtual void run() {
            lock->readLock().lock();
            lock->readLock().unlock();
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantReadWriteLockTest::testReadAfterWriteLock() {
    ReentrantReadWriteLock lock;
    TestReadAfterWriteLockRunnable runnable(this, &lock);
    Thread t1(&runnable);
    Thread t2(&runnable);

    lock.writeLock().lock();

    try {
        t1.start();
        t2.start();
        Thread::sleep( SHORT_DELAY_MS);
        lock.writeLock().unlock();
        t1.join(MEDIUM_DELAY_MS);
        t2.join(MEDIUM_DELAY_MS);
        CPPUNIT_ASSERT(!t1.isAlive());
        CPPUNIT_ASSERT(!t2.isAlive());
    } catch (Exception& e) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantReadWriteLockTest::testReadHoldingWriteLock() {

    ReentrantReadWriteLock lock;
    lock.writeLock().lock();
    CPPUNIT_ASSERT(lock.readLock().tryLock());
    lock.readLock().unlock();
    lock.writeLock().unlock();
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class TestReadHoldingWriteLockRunnable2 : public Runnable {
    private:

        ReentrantReadWriteLockTest* test;
        ReentrantReadWriteLock* lock;

    public:

        TestReadHoldingWriteLockRunnable2(ReentrantReadWriteLockTest* test, ReentrantReadWriteLock* lock) :
            Runnable(), test(test), lock(lock) {}
        virtual ~TestReadHoldingWriteLockRunnable2() {}

        virtual void run() {
            lock->readLock().lock();
            lock->readLock().unlock();
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantReadWriteLockTest::testReadHoldingWriteLock2() {
    ReentrantReadWriteLock lock;
    TestReadHoldingWriteLockRunnable2 runnable(this, &lock);
    Thread t1(&runnable);
    Thread t2(&runnable);

    lock.writeLock().lock();

    try {
        t1.start();
        t2.start();
        lock.readLock().lock();
        lock.readLock().unlock();
        Thread::sleep( SHORT_DELAY_MS);
        lock.readLock().lock();
        lock.readLock().unlock();
        lock.writeLock().unlock();
        t1.join(MEDIUM_DELAY_MS);
        t2.join(MEDIUM_DELAY_MS);
        CPPUNIT_ASSERT(!t1.isAlive());
        CPPUNIT_ASSERT(!t2.isAlive());
    } catch (Exception& e) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class TestReadHoldingWriteLockRunnable3 : public Runnable {
    private:

        ReentrantReadWriteLockTest* test;
        ReentrantReadWriteLock* lock;

    public:

        TestReadHoldingWriteLockRunnable3(ReentrantReadWriteLockTest* test, ReentrantReadWriteLock* lock) :
            Runnable(), test(test), lock(lock) {}
        virtual ~TestReadHoldingWriteLockRunnable3() {}

        virtual void run() {
            lock->writeLock().lock();
            lock->writeLock().unlock();
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantReadWriteLockTest::testReadHoldingWriteLock3() {
    ReentrantReadWriteLock lock;
    TestReadHoldingWriteLockRunnable3 runnable(this, &lock);
    Thread t1(&runnable);
    Thread t2(&runnable);

    lock.writeLock().lock();

    try {
        t1.start();
        t2.start();
        lock.readLock().lock();
        lock.readLock().unlock();
        Thread::sleep( SHORT_DELAY_MS);
        lock.readLock().lock();
        lock.readLock().unlock();
        lock.writeLock().unlock();
        t1.join(MEDIUM_DELAY_MS);
        t2.join(MEDIUM_DELAY_MS);
        CPPUNIT_ASSERT(!t1.isAlive());
        CPPUNIT_ASSERT(!t2.isAlive());
    } catch (Exception& e) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class TestWriteHoldingWriteLock4Runnable : public Runnable {
    private:

        ReentrantReadWriteLockTest* test;
        ReentrantReadWriteLock* lock;

    public:

        TestWriteHoldingWriteLock4Runnable(ReentrantReadWriteLockTest* test, ReentrantReadWriteLock* lock) :
            Runnable(), test(test), lock(lock) {}
        virtual ~TestWriteHoldingWriteLock4Runnable() {}

        virtual void run() {
            lock->writeLock().lock();
            lock->writeLock().unlock();
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantReadWriteLockTest::testWriteHoldingWriteLock4() {
    ReentrantReadWriteLock lock;
    TestWriteHoldingWriteLock4Runnable runnable(this, &lock);
    Thread t1(&runnable);
    Thread t2(&runnable);

    lock.writeLock().lock();

    try {
        t1.start();
        t2.start();
        lock.writeLock().lock();
        lock.writeLock().unlock();
        Thread::sleep( SHORT_DELAY_MS);
        lock.writeLock().lock();
        lock.writeLock().unlock();
        lock.writeLock().unlock();
        t1.join(MEDIUM_DELAY_MS);
        t2.join(MEDIUM_DELAY_MS);
        CPPUNIT_ASSERT(!t1.isAlive());
        CPPUNIT_ASSERT(!t2.isAlive());
    } catch (Exception& e) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantReadWriteLockTest::testReadHoldingWriteLockFair() {

    ReentrantReadWriteLock lock(true);
    lock.writeLock().lock();
    CPPUNIT_ASSERT(lock.readLock().tryLock());
    lock.readLock().unlock();
    lock.writeLock().unlock();
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class TestReadHoldingWriteLockFair2Runnable : public Runnable {
    private:

        ReentrantReadWriteLockTest* test;
        ReentrantReadWriteLock* lock;

    public:

        TestReadHoldingWriteLockFair2Runnable(ReentrantReadWriteLockTest* test, ReentrantReadWriteLock* lock) :
            Runnable(), test(test), lock(lock) {}
        virtual ~TestReadHoldingWriteLockFair2Runnable() {}

        virtual void run() {
            lock->readLock().lock();
            lock->readLock().unlock();
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantReadWriteLockTest::testReadHoldingWriteLockFair2() {
    ReentrantReadWriteLock lock(true);
    TestReadHoldingWriteLockFair2Runnable runnable(this, &lock);
    Thread t1(&runnable);
    Thread t2(&runnable);

    lock.writeLock().lock();

    try {
        t1.start();
        t2.start();
        lock.readLock().lock();
        lock.readLock().unlock();
        Thread::sleep( SHORT_DELAY_MS);
        lock.readLock().lock();
        lock.readLock().unlock();
        lock.writeLock().unlock();
        t1.join(MEDIUM_DELAY_MS);
        t2.join(MEDIUM_DELAY_MS);
        CPPUNIT_ASSERT(!t1.isAlive());
        CPPUNIT_ASSERT(!t2.isAlive());
    } catch (Exception& e) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class TestReadHoldingWriteLockFair3Runnable : public Runnable {
    private:

        ReentrantReadWriteLockTest* test;
        ReentrantReadWriteLock* lock;

    public:

        TestReadHoldingWriteLockFair3Runnable(ReentrantReadWriteLockTest* test, ReentrantReadWriteLock* lock) :
            Runnable(), test(test), lock(lock) {}
        virtual ~TestReadHoldingWriteLockFair3Runnable() {}

        virtual void run() {
            lock->writeLock().lock();
            lock->writeLock().unlock();
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantReadWriteLockTest::testReadHoldingWriteLockFair3() {
    ReentrantReadWriteLock lock(true);
    TestReadHoldingWriteLockFair3Runnable runnable(this, &lock);
    Thread t1(&runnable);
    Thread t2(&runnable);

    lock.writeLock().lock();

    try {
        t1.start();
        t2.start();
        lock.readLock().lock();
        lock.readLock().unlock();
        Thread::sleep( SHORT_DELAY_MS);
        lock.readLock().lock();
        lock.readLock().unlock();
        lock.writeLock().unlock();
        t1.join(MEDIUM_DELAY_MS);
        t2.join(MEDIUM_DELAY_MS);
        CPPUNIT_ASSERT(!t1.isAlive());
        CPPUNIT_ASSERT(!t2.isAlive());
    } catch (Exception& e) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class TestWriteHoldingWriteLockFair4Runnable : public Runnable {
    private:

        ReentrantReadWriteLockTest* test;
        ReentrantReadWriteLock* lock;

    public:

        TestWriteHoldingWriteLockFair4Runnable(ReentrantReadWriteLockTest* test, ReentrantReadWriteLock* lock) :
            Runnable(), test(test), lock(lock) {}
        virtual ~TestWriteHoldingWriteLockFair4Runnable() {}

        virtual void run() {
            lock->writeLock().lock();
            lock->writeLock().unlock();
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantReadWriteLockTest::testWriteHoldingWriteLockFair4() {
    ReentrantReadWriteLock lock(true);
    TestWriteHoldingWriteLockFair4Runnable runnable(this, &lock);
    Thread t1(&runnable);
    Thread t2(&runnable);
    lock.writeLock().lock();

    try {
        t1.start();
        t2.start();
        Thread::sleep( SHORT_DELAY_MS);
        CPPUNIT_ASSERT(lock.isWriteLockedByCurrentThread());
        CPPUNIT_ASSERT(lock.getWriteHoldCount() == 1);
        lock.writeLock().lock();
        CPPUNIT_ASSERT(lock.getWriteHoldCount() == 2);
        lock.writeLock().unlock();
        lock.writeLock().lock();
        lock.writeLock().unlock();
        lock.writeLock().unlock();
        t1.join(MEDIUM_DELAY_MS);
        t2.join(MEDIUM_DELAY_MS);
        CPPUNIT_ASSERT(!t1.isAlive());
        CPPUNIT_ASSERT(!t2.isAlive());
    } catch (Exception& e) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class TestTryLockWhenReadLockedRunnable : public Runnable {
    private:

        ReentrantReadWriteLockTest* test;
        ReentrantReadWriteLock* lock;

    public:

        TestTryLockWhenReadLockedRunnable(ReentrantReadWriteLockTest* test, ReentrantReadWriteLock* lock) :
            Runnable(), test(test), lock(lock) {}
        virtual ~TestTryLockWhenReadLockedRunnable() {}

        virtual void run() {
            test->threadAssertTrue(lock->readLock().tryLock());
            lock->readLock().unlock();
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantReadWriteLockTest::testTryLockWhenReadLocked() {
    ReentrantReadWriteLock lock;
    TestTryLockWhenReadLockedRunnable runnable(this, &lock);
    Thread t(&runnable);

    lock.readLock().lock();
    try {
        t.start();
        t.join();
        lock.readLock().unlock();
    } catch (Exception& e) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class TestWriteTryLockWhenReadLockedRunnable : public Runnable {
    private:

        ReentrantReadWriteLockTest* test;
        ReentrantReadWriteLock* lock;

    public:

        TestWriteTryLockWhenReadLockedRunnable(ReentrantReadWriteLockTest* test, ReentrantReadWriteLock* lock) :
            Runnable(), test(test), lock(lock) {}
        virtual ~TestWriteTryLockWhenReadLockedRunnable() {}

        virtual void run() {
            test->threadAssertFalse(lock->writeLock().tryLock());
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantReadWriteLockTest::testWriteTryLockWhenReadLocked() {
    ReentrantReadWriteLock lock;
    TestWriteTryLockWhenReadLockedRunnable runnable(this, &lock);
    Thread t(&runnable);
    lock.readLock().lock();

    try {
        t.start();
        t.join();
        lock.readLock().unlock();
    } catch (Exception& e) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class TestTryLockWhenReadLockedFairRunnable : public Runnable {
    private:

        ReentrantReadWriteLockTest* test;
        ReentrantReadWriteLock* lock;

    public:

        TestTryLockWhenReadLockedFairRunnable(ReentrantReadWriteLockTest* test, ReentrantReadWriteLock* lock) :
            Runnable(), test(test), lock(lock) {}
        virtual ~TestTryLockWhenReadLockedFairRunnable() {}

        virtual void run() {
            test->threadAssertTrue(lock->readLock().tryLock());
            lock->readLock().unlock();
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantReadWriteLockTest::testTryLockWhenReadLockedFair() {
    ReentrantReadWriteLock lock(true);
    TestTryLockWhenReadLockedFairRunnable runnable(this, &lock);
    Thread t(&runnable);
    lock.readLock().lock();

    try {
        t.start();
        t.join();
        lock.readLock().unlock();
    } catch (Exception& e) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class TestWriteTryLockWhenReadLockedFairRunnable : public Runnable {
    private:

        ReentrantReadWriteLockTest* test;
        ReentrantReadWriteLock* lock;

    public:

        TestWriteTryLockWhenReadLockedFairRunnable(ReentrantReadWriteLockTest* test, ReentrantReadWriteLock* lock) :
            Runnable(), test(test), lock(lock) {}
        virtual ~TestWriteTryLockWhenReadLockedFairRunnable() {}

        virtual void run() {
            test->threadAssertFalse(lock->writeLock().tryLock());
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantReadWriteLockTest::testWriteTryLockWhenReadLockedFair() {
    ReentrantReadWriteLock lock(true);
    TestWriteTryLockWhenReadLockedFairRunnable runnable(this, &lock);
    Thread t(&runnable);
    lock.readLock().lock();

    try {
        t.start();
        t.join();
        lock.readLock().unlock();
    } catch (Exception& e) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class TestWriteTryLockTimeoutRunnable : public Runnable {
    private:

        ReentrantReadWriteLockTest* test;
        ReentrantReadWriteLock* lock;

    public:

        TestWriteTryLockTimeoutRunnable(ReentrantReadWriteLockTest* test, ReentrantReadWriteLock* lock) :
            Runnable(), test(test), lock(lock) {}
        virtual ~TestWriteTryLockTimeoutRunnable() {}

        virtual void run() {
            try {
                test->threadAssertFalse(lock->writeLock().tryLock(1, TimeUnit::MILLISECONDS));
            } catch (Exception& ex) {
                test->threadUnexpectedException();
            }
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantReadWriteLockTest::testWriteTryLockTimeout() {
    ReentrantReadWriteLock lock;
    TestWriteTryLockTimeoutRunnable runnable(this, &lock);
    Thread t(&runnable);
    lock.writeLock().lock();

    try {
        t.start();
        t.join();
        lock.writeLock().unlock();
    } catch (Exception& e) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class TestReadTryLockTimeoutRunnable : public Runnable {
    private:

        ReentrantReadWriteLockTest* test;
        ReentrantReadWriteLock* lock;

    public:

        TestReadTryLockTimeoutRunnable(ReentrantReadWriteLockTest* test, ReentrantReadWriteLock* lock) :
            Runnable(), test(test), lock(lock) {}
        virtual ~TestReadTryLockTimeoutRunnable() {}

        virtual void run() {
            try {
                test->threadAssertFalse(lock->readLock().tryLock(1, TimeUnit::MILLISECONDS));
            } catch (Exception& ex) {
                test->threadUnexpectedException();
            }
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantReadWriteLockTest::testReadTryLockTimeout() {
    ReentrantReadWriteLock lock;
    TestReadTryLockTimeoutRunnable runnable(this, &lock);
    Thread t(&runnable);
    lock.writeLock().lock();

    try {
        t.start();
        t.join();
        lock.writeLock().unlock();
    } catch (Exception& e) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class TestWriteLockInterruptiblyRunnable : public Runnable {
    private:

        ReentrantReadWriteLockTest* test;
        ReentrantReadWriteLock* lock;

    public:

        TestWriteLockInterruptiblyRunnable(ReentrantReadWriteLockTest* test, ReentrantReadWriteLock* lock) :
            Runnable(), test(test), lock(lock) {}
        virtual ~TestWriteLockInterruptiblyRunnable() {}

        virtual void run() {
            try {
                lock->writeLock().lockInterruptibly();
                test->threadShouldThrow();
            } catch(InterruptedException& success) {
            }
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantReadWriteLockTest::testWriteLockInterruptibly() {
    ReentrantReadWriteLock lock;
    TestWriteLockInterruptiblyRunnable runnable(this, &lock);
    Thread t(&runnable);
    try {
        lock.writeLock().lockInterruptibly();
    } catch (Exception& e) {
        unexpectedException();
    }

    try {
        t.start();
        Thread::sleep(SHORT_DELAY_MS);
        t.interrupt();
        Thread::sleep(SHORT_DELAY_MS);
        t.join();
        lock.writeLock().unlock();
    } catch (Exception& e) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class TestReadLockInterruptiblyRunnable : public Runnable {
    private:

        ReentrantReadWriteLockTest* test;
        ReentrantReadWriteLock* lock;

    public:

        TestReadLockInterruptiblyRunnable(ReentrantReadWriteLockTest* test, ReentrantReadWriteLock* lock) :
            Runnable(), test(test), lock(lock) {}
        virtual ~TestReadLockInterruptiblyRunnable() {}

        virtual void run() {
            try {
                lock->readLock().lockInterruptibly();
                test->threadShouldThrow();
            } catch(InterruptedException& success) {
            }
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantReadWriteLockTest::testReadLockInterruptibly() {
    ReentrantReadWriteLock lock;
    TestReadLockInterruptiblyRunnable runnable(this, &lock);
    Thread t(&runnable);

    try {
        lock.writeLock().lockInterruptibly();
    } catch (Exception& e) {
        unexpectedException();
    }

    try {
        t.start();
        Thread::sleep(SHORT_DELAY_MS);
        t.interrupt();
        t.join();
        lock.writeLock().unlock();
    } catch (Exception& e) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantReadWriteLockTest::testAwaitIllegalMonitor() {

    ReentrantReadWriteLock lock;
    Pointer<Condition> c(lock.writeLock().newCondition());
    try {
        c->await();
        shouldThrow();
    } catch (IllegalMonitorStateException& success) {
    } catch (Exception& ex) {
        shouldThrow();
    }
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantReadWriteLockTest::testSignalIllegalMonitor() {

    ReentrantReadWriteLock lock;
    Pointer<Condition> c(lock.writeLock().newCondition());
    try {
        c->signal();
        shouldThrow();
    } catch (IllegalMonitorStateException& success) {
    } catch (Exception& ex) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantReadWriteLockTest::testAwaitNanosTimeout() {

    ReentrantReadWriteLock lock;
    Pointer<Condition> c(lock.writeLock().newCondition());

    try {
        lock.writeLock().lock();
        long long t = c->awaitNanos(100);
        CPPUNIT_ASSERT(t <= 0);
        lock.writeLock().unlock();
    } catch (Exception& ex) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantReadWriteLockTest::testAwaitTimeout() {

    ReentrantReadWriteLock lock;
    Pointer<Condition> c(lock.writeLock().newCondition());
    try {
        lock.writeLock().lock();
        CPPUNIT_ASSERT(!c->await(SHORT_DELAY_MS, TimeUnit::MILLISECONDS));
        lock.writeLock().unlock();
    } catch (Exception& ex) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantReadWriteLockTest::testAwaitUntilTimeout() {

    ReentrantReadWriteLock lock;
    Pointer<Condition> c(lock.writeLock().newCondition());
    try {
        lock.writeLock().lock();
        Date d;
        CPPUNIT_ASSERT(!c->awaitUntil(Date(d.getTime() + SHORT_DELAY_MS)));
        lock.writeLock().unlock();
    } catch (Exception& ex) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class TestAwaitRunnable : public Runnable {
    private:

        ReentrantReadWriteLockTest* test;
        ReentrantReadWriteLock* lock;
        Condition* cond;

    public:

        TestAwaitRunnable(ReentrantReadWriteLockTest* test, ReentrantReadWriteLock* lock, Condition* cond) :
            Runnable(), test(test), lock(lock), cond(cond) {}
        virtual ~TestAwaitRunnable() {}

        virtual void run() {
            try {
                lock->writeLock().lock();
                cond->await();
                lock->writeLock().unlock();
            } catch(InterruptedException& e) {
                test->threadUnexpectedException();
            }
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantReadWriteLockTest::testAwait() {
    ReentrantReadWriteLock lock;
    Pointer<Condition> c(lock.writeLock().newCondition());
    TestAwaitRunnable runnable(this, &lock, c.get());
    Thread t(&runnable);

    try {
        t.start();
        Thread::sleep(SHORT_DELAY_MS);
        lock.writeLock().lock();
        c->signal();
        lock.writeLock().unlock();
        t.join(SHORT_DELAY_MS);
        CPPUNIT_ASSERT(!t.isAlive());
    } catch (Exception& ex) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class UninterruptableThread : public Thread {
    private:

        ReentrantReadWriteLockTest* test;
        decaf::util::concurrent::locks::Lock& lock;
        Condition* cond;

    public:

        volatile bool canAwake;
        volatile bool interrupted;
        volatile bool lockStarted;

    public:

        UninterruptableThread(ReentrantReadWriteLockTest* test, decaf::util::concurrent::locks::Lock& lock, Condition* cond) :
            Thread(), test(test), lock(lock), cond(cond), canAwake(false), interrupted(false), lockStarted(false) {}
        virtual ~UninterruptableThread() {}

        virtual void run() {
            lock.lock();
            lockStarted = true;

            while (!canAwake) {
                cond->awaitUninterruptibly();
            }

            interrupted = isInterrupted();
            lock.unlock();
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantReadWriteLockTest::testAwaitUninterruptibly() {

    ReentrantReadWriteLock lock;
    Pointer<Condition> c(lock.writeLock().newCondition());
    UninterruptableThread thread(this, lock.writeLock(), c.get());

    try {

        thread.start();

        while (!thread.lockStarted) {
            Thread::sleep(100);
        }

        lock.writeLock().lock();
        try {
            thread.interrupt();
            thread.canAwake = true;
            c->signal();
            lock.writeLock().unlock();
        } catch(...) {
            lock.writeLock().unlock();
        }

        thread.join();
        CPPUNIT_ASSERT(thread.interrupted);
        CPPUNIT_ASSERT(!thread.isAlive());
    } catch (Exception& ex) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class TestAwaitInterruptRunnable : public Runnable {
    private:

        ReentrantReadWriteLockTest* test;
        ReentrantReadWriteLock* lock;
        Condition* cond;

    public:

        TestAwaitInterruptRunnable(ReentrantReadWriteLockTest* test, ReentrantReadWriteLock* lock, Condition* cond) :
            Runnable(), test(test), lock(lock), cond(cond) {}
        virtual ~TestAwaitInterruptRunnable() {}

        virtual void run() {
            try {
                lock->writeLock().lock();
                cond->await();
                lock->writeLock().unlock();
                test->threadShouldThrow();
            } catch(InterruptedException& success) {
            }
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantReadWriteLockTest::testAwaitInterrupt() {

    ReentrantReadWriteLock lock;
    Pointer<Condition> c(lock.writeLock().newCondition());
    TestAwaitInterruptRunnable runnable(this, &lock, c.get());
    Thread t(&runnable);

    try {
        t.start();
        Thread::sleep(SHORT_DELAY_MS);
        t.interrupt();
        t.join(SHORT_DELAY_MS);
        CPPUNIT_ASSERT(!t.isAlive());
    } catch (Exception& ex) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class TestAwaitNanosInterruptRunnable : public Runnable {
    private:

        ReentrantReadWriteLockTest* test;
        ReentrantReadWriteLock* lock;
        Condition* cond;

    public:

        TestAwaitNanosInterruptRunnable(ReentrantReadWriteLockTest* test, ReentrantReadWriteLock* lock, Condition* cond) :
            Runnable(), test(test), lock(lock), cond(cond) {}
        virtual ~TestAwaitNanosInterruptRunnable() {}

        virtual void run() {
            try {
                lock->writeLock().lock();
                cond->awaitNanos(ReentrantReadWriteLockTest::SHORT_DELAY_MS * 2 * 1000000);
                lock->writeLock().unlock();
                test->threadShouldThrow();
            } catch(InterruptedException& success) {
            }
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantReadWriteLockTest::testAwaitNanosInterrupt() {
    ReentrantReadWriteLock lock;
    Pointer<Condition> c(lock.writeLock().newCondition());
    TestAwaitNanosInterruptRunnable runnable(this, &lock, c.get());
    Thread t(&runnable);

    try {
        t.start();
        Thread::sleep(SHORT_DELAY_MS);
        t.interrupt();
        t.join(SHORT_DELAY_MS);
        CPPUNIT_ASSERT(!t.isAlive());
    } catch (Exception& ex) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class TestAwaitUntilInterruptRunnable : public Runnable {
    private:

        ReentrantReadWriteLockTest* test;
        ReentrantReadWriteLock* lock;
        Condition* cond;

    public:

        TestAwaitUntilInterruptRunnable(ReentrantReadWriteLockTest* test, ReentrantReadWriteLock* lock, Condition* cond) :
            Runnable(), test(test), lock(lock), cond(cond) {}
        virtual ~TestAwaitUntilInterruptRunnable() {}

        virtual void run() {
            try {
                lock->writeLock().lock();
                decaf::util::Date d;
                cond->awaitUntil(decaf::util::Date(d.getTime() + 10000));
                lock->writeLock().unlock();
                test->threadShouldThrow();
            } catch(InterruptedException& success) {
            }
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantReadWriteLockTest::testAwaitUntilInterrupt() {
    ReentrantReadWriteLock lock;
    Pointer<Condition> c(lock.writeLock().newCondition());
    TestAwaitUntilInterruptRunnable runnable(this, &lock, c.get());
    Thread t(&runnable);

    try {
        t.start();
        Thread::sleep(SHORT_DELAY_MS);
        t.interrupt();
        t.join(SHORT_DELAY_MS);
        CPPUNIT_ASSERT(!t.isAlive());
    } catch (Exception& ex) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class TestSignalAllRunnable : public Runnable {
    private:

        ReentrantReadWriteLockTest* test;
        ReentrantReadWriteLock* lock;
        Condition* cond;

    public:

        TestSignalAllRunnable(ReentrantReadWriteLockTest* test, ReentrantReadWriteLock* lock, Condition* cond) :
            Runnable(), test(test), lock(lock), cond(cond) {}
        virtual ~TestSignalAllRunnable() {}

        virtual void run() {
            try {
                lock->writeLock().lock();
                cond->await();
                lock->writeLock().unlock();
            } catch(InterruptedException& e) {
                test->threadUnexpectedException();
            }
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantReadWriteLockTest::testSignalAll() {
    ReentrantReadWriteLock lock;
    Pointer<Condition> c(lock.writeLock().newCondition());
    TestSignalAllRunnable runnable(this, &lock, c.get());
    Thread t1(&runnable);
    Thread t2(&runnable);

    try {
        t1.start();
        t2.start();
        Thread::sleep(SHORT_DELAY_MS);
        lock.writeLock().lock();
        c->signalAll();
        lock.writeLock().unlock();
        t1.join(SHORT_DELAY_MS);
        t2.join(SHORT_DELAY_MS);
        CPPUNIT_ASSERT(!t1.isAlive());
        CPPUNIT_ASSERT(!t2.isAlive());
    } catch (Exception& ex) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantReadWriteLockTest::testHasQueuedThreads() {
    ReentrantReadWriteLock lock;
    InterruptedLockRunnable interrupted(this, &lock);
    InterruptibleLockRunnable interruptable(this, &lock);
    Thread t1(&interrupted);
    Thread t2(&interruptable);

    try {
        CPPUNIT_ASSERT(!lock.hasQueuedThreads());
        lock.writeLock().lock();
        t1.start();
        Thread::sleep(SHORT_DELAY_MS);
        CPPUNIT_ASSERT(lock.hasQueuedThreads());
        t2.start();
        Thread::sleep(SHORT_DELAY_MS);
        CPPUNIT_ASSERT(lock.hasQueuedThreads());
        t1.interrupt();
        Thread::sleep(SHORT_DELAY_MS);
        CPPUNIT_ASSERT(lock.hasQueuedThreads());
        lock.writeLock().unlock();
        Thread::sleep(SHORT_DELAY_MS);
        CPPUNIT_ASSERT(!lock.hasQueuedThreads());
        t1.join();
        t2.join();
    } catch (Exception& e) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantReadWriteLockTest::testHasQueuedThreadNPE() {
    ReentrantReadWriteLock sync;
    try {
        sync.hasQueuedThread(NULL);
        shouldThrow();
    } catch (NullPointerException& success) {
    }
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantReadWriteLockTest::testHasQueuedThread() {
    ReentrantReadWriteLock sync;
    InterruptedLockRunnable interrupted(this, &sync);
    InterruptibleLockRunnable interruptable(this, &sync);
    Thread t1(&interrupted);
    Thread t2(&interruptable);

    try {
        CPPUNIT_ASSERT(!sync.hasQueuedThread(&t1));
        CPPUNIT_ASSERT(!sync.hasQueuedThread(&t2));
        sync.writeLock().lock();
        t1.start();
        Thread::sleep(SHORT_DELAY_MS);
        CPPUNIT_ASSERT(sync.hasQueuedThread(&t1));
        t2.start();
        Thread::sleep(SHORT_DELAY_MS);
        CPPUNIT_ASSERT(sync.hasQueuedThread(&t1));
        CPPUNIT_ASSERT(sync.hasQueuedThread(&t2));
        t1.interrupt();
        Thread::sleep(SHORT_DELAY_MS);
        CPPUNIT_ASSERT(!sync.hasQueuedThread(&t1));
        CPPUNIT_ASSERT(sync.hasQueuedThread(&t2));
        sync.writeLock().unlock();
        Thread::sleep(SHORT_DELAY_MS);
        CPPUNIT_ASSERT(!sync.hasQueuedThread(&t1));
        Thread::sleep(SHORT_DELAY_MS);
        CPPUNIT_ASSERT(!sync.hasQueuedThread(&t2));
        t1.join();
        t2.join();
    } catch (Exception& e) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantReadWriteLockTest::testGetQueueLength() {
    ReentrantReadWriteLock lock;
    InterruptedLockRunnable interrupted(this, &lock);
    InterruptibleLockRunnable interruptable(this, &lock);
    Thread t1(&interrupted);
    Thread t2(&interruptable);

    try {
        CPPUNIT_ASSERT_EQUAL(0, lock.getQueueLength());
        lock.writeLock().lock();
        t1.start();
        Thread::sleep(SHORT_DELAY_MS);
        CPPUNIT_ASSERT_EQUAL(1, lock.getQueueLength());
        t2.start();
        Thread::sleep(SHORT_DELAY_MS);
        CPPUNIT_ASSERT_EQUAL(2, lock.getQueueLength());
        t1.interrupt();
        Thread::sleep(SHORT_DELAY_MS);
        CPPUNIT_ASSERT_EQUAL(1, lock.getQueueLength());
        lock.writeLock().unlock();
        Thread::sleep(SHORT_DELAY_MS);
        CPPUNIT_ASSERT_EQUAL(0, lock.getQueueLength());
        t1.join();
        t2.join();
    } catch (Exception& e) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantReadWriteLockTest::testGetQueuedThreads() {
    PublicReentrantReadWriteLock lock;
    InterruptedLockRunnable interrupted(this, &lock);
    InterruptibleLockRunnable interruptable(this, &lock);
    Thread t1(&interrupted);
    Thread t2(&interruptable);

    try {
        CPPUNIT_ASSERT(Pointer<Collection<Thread*> >(lock.getQueuedThreadsPublic())->isEmpty());
        lock.writeLock().lock();
        CPPUNIT_ASSERT(Pointer<Collection<Thread*> >(lock.getQueuedThreadsPublic())->isEmpty());
        t1.start();
        Thread::sleep(SHORT_DELAY_MS);
        CPPUNIT_ASSERT(Pointer<Collection<Thread*> >(lock.getQueuedThreadsPublic())->contains(&t1));
        t2.start();
        Thread::sleep(SHORT_DELAY_MS);
        CPPUNIT_ASSERT(Pointer<Collection<Thread*> >(lock.getQueuedThreadsPublic())->contains(&t1));
        CPPUNIT_ASSERT(Pointer<Collection<Thread*> >(lock.getQueuedThreadsPublic())->contains(&t2));
        t1.interrupt();
        Thread::sleep(SHORT_DELAY_MS);
        CPPUNIT_ASSERT(!Pointer<Collection<Thread*> >(lock.getQueuedThreadsPublic())->contains(&t1));
        CPPUNIT_ASSERT(Pointer<Collection<Thread*> >(lock.getQueuedThreadsPublic())->contains(&t2));
        lock.writeLock().unlock();
        Thread::sleep(SHORT_DELAY_MS);
        CPPUNIT_ASSERT(Pointer<Collection<Thread*> >(lock.getQueuedThreadsPublic())->isEmpty());
        t1.join();
        t2.join();
    } catch (Exception& e) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantReadWriteLockTest::testHasWaitersNPE() {
    ReentrantReadWriteLock lock;
    try {
        lock.hasWaiters(NULL);
        shouldThrow();
    } catch (NullPointerException& success) {
    } catch (Exception& ex) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantReadWriteLockTest::testGetWaitQueueLengthNPE() {
ReentrantReadWriteLock lock;
    try {
        lock.getWaitQueueLength(NULL);
        shouldThrow();
    } catch (NullPointerException& success) {
    } catch (Exception& ex) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantReadWriteLockTest::testGetWaitingThreadsNPE() {
    PublicReentrantReadWriteLock lock;
    try {
        lock.getWaitingThreadsPublic(NULL);
        shouldThrow();
    } catch (NullPointerException& success) {
    } catch (Exception& ex) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantReadWriteLockTest::testHasWaitersIAE() {
    ReentrantReadWriteLock lock;
    Pointer<Condition> c(lock.writeLock().newCondition());
    ReentrantReadWriteLock lock2;
    try {
        lock2.hasWaiters(c.get());
        shouldThrow();
    } catch (IllegalArgumentException& success) {
    } catch (Exception& ex) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantReadWriteLockTest::testHasWaitersIMSE() {
    ReentrantReadWriteLock lock;
    Pointer<Condition> c(lock.writeLock().newCondition());
    try {
        lock.hasWaiters(c.get());
        shouldThrow();
    } catch (IllegalMonitorStateException& success) {
    } catch (Exception& ex) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantReadWriteLockTest::testGetWaitQueueLengthIAE() {
    ReentrantReadWriteLock lock;
    Pointer<Condition> c(lock.writeLock().newCondition());
    ReentrantReadWriteLock lock2;
    try {
        lock2.getWaitQueueLength(c.get());
        shouldThrow();
    } catch (IllegalArgumentException& success) {
    } catch (Exception& ex) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantReadWriteLockTest::testGetWaitQueueLengthIMSE() {
    ReentrantReadWriteLock lock;
    Pointer<Condition> c(lock.writeLock().newCondition());
    try {
        lock.getWaitQueueLength(c.get());
        shouldThrow();
    } catch (IllegalMonitorStateException& success) {
    } catch (Exception& ex) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantReadWriteLockTest::testGetWaitingThreadsIAE() {
    PublicReentrantReadWriteLock lock;
    Pointer<Condition> c(lock.writeLock().newCondition());
    PublicReentrantReadWriteLock lock2;
    try {
        lock2.getWaitingThreadsPublic(c.get());
        shouldThrow();
    } catch (IllegalArgumentException& success) {
    } catch (Exception& ex) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantReadWriteLockTest::testGetWaitingThreadsIMSE() {
    PublicReentrantReadWriteLock lock;
    Pointer<Condition> c(lock.writeLock().newCondition());
    try {
        lock.getWaitingThreadsPublic(c.get());
        shouldThrow();
    } catch (IllegalMonitorStateException& success) {
    } catch (Exception& ex) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class TestHasWaitersRunnable : public Runnable {
    private:

        ReentrantReadWriteLockTest* test;
        ReentrantReadWriteLock* lock;
        Condition* cond;

    public:

        TestHasWaitersRunnable(ReentrantReadWriteLockTest* test, ReentrantReadWriteLock* lock, Condition* cond) :
            Runnable(), test(test), lock(lock), cond(cond) {}
        virtual ~TestHasWaitersRunnable() {}

        virtual void run() {
            try {
                lock->writeLock().lock();
                test->threadAssertFalse(lock->hasWaiters(cond));
                test->threadAssertEquals(0, lock->getWaitQueueLength(cond));
                cond->await();
                lock->writeLock().unlock();
            } catch(InterruptedException& e) {
                test->threadUnexpectedException();
            }
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantReadWriteLockTest::testHasWaiters() {
    ReentrantReadWriteLock lock;
    Pointer<Condition> c(lock.writeLock().newCondition());
    TestHasWaitersRunnable runnable(this, &lock, c.get());
    Thread t(&runnable);

    try {
        t.start();
        Thread::sleep( SHORT_DELAY_MS);
        lock.writeLock().lock();
        CPPUNIT_ASSERT(lock.hasWaiters(c.get()));
        CPPUNIT_ASSERT_EQUAL(1, lock.getWaitQueueLength(c.get()));
        c->signal();
        lock.writeLock().unlock();
        Thread::sleep(SHORT_DELAY_MS);
        lock.writeLock().lock();
        CPPUNIT_ASSERT(!lock.hasWaiters(c.get()));
        CPPUNIT_ASSERT_EQUAL(0, lock.getWaitQueueLength(c.get()));
        lock.writeLock().unlock();
        t.join(SHORT_DELAY_MS);
        CPPUNIT_ASSERT(!t.isAlive());
    } catch (Exception& ex) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class TestGetWaitQueueLengthRunnable : public Runnable {
    private:

        ReentrantReadWriteLockTest* test;
        ReentrantReadWriteLock* lock;
        Condition* cond;

    public:

        TestGetWaitQueueLengthRunnable(ReentrantReadWriteLockTest* test, ReentrantReadWriteLock* lock, Condition* cond) :
            Runnable(), test(test), lock(lock), cond(cond) {}
        virtual ~TestGetWaitQueueLengthRunnable() {}

        virtual void run() {
            try {
                lock->writeLock().lock();
                test->threadAssertFalse(lock->hasWaiters(cond));
                test->threadAssertEquals(0, lock->getWaitQueueLength(cond));
                cond->await();
                lock->writeLock().unlock();
            } catch(InterruptedException& e) {
                test->threadUnexpectedException();
            }
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantReadWriteLockTest::testGetWaitQueueLength() {
    ReentrantReadWriteLock lock;
    Pointer<Condition> c(lock.writeLock().newCondition());
    TestGetWaitQueueLengthRunnable runnable(this, &lock, c.get());
    Thread t(&runnable);

    try {
        t.start();
        Thread::sleep( SHORT_DELAY_MS);
        lock.writeLock().lock();
        CPPUNIT_ASSERT(lock.hasWaiters(c.get()));
        CPPUNIT_ASSERT_EQUAL(1, lock.getWaitQueueLength(c.get()));
        c->signal();
        lock.writeLock().unlock();
        Thread::sleep(SHORT_DELAY_MS);
        lock.writeLock().lock();
        CPPUNIT_ASSERT(!lock.hasWaiters(c.get()));
        CPPUNIT_ASSERT_EQUAL(0, lock.getWaitQueueLength(c.get()));
        lock.writeLock().unlock();
        t.join(SHORT_DELAY_MS);
        CPPUNIT_ASSERT(!t.isAlive());
    } catch (Exception& ex) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class TestGetWaitingThreadsRunnable1 : public Runnable {
    private:

        ReentrantReadWriteLockTest* test;
        PublicReentrantReadWriteLock* lock;
        Condition* cond;

    public:

        TestGetWaitingThreadsRunnable1(ReentrantReadWriteLockTest* test, PublicReentrantReadWriteLock* lock, Condition* cond) :
            Runnable(), test(test), lock(lock), cond(cond) {}
        virtual ~TestGetWaitingThreadsRunnable1() {}

        virtual void run() {
            try {
                lock->writeLock().lock();
                test->threadAssertTrue(
                    Pointer<Collection<Thread*> >(lock->getWaitingThreadsPublic(cond))->isEmpty());
                cond->await();
                lock->writeLock().unlock();
            } catch(InterruptedException& e) {
                test->threadUnexpectedException();
            }
        }
    };

    class TestGetWaitingThreadsRunnable2 : public Runnable {
    private:

        ReentrantReadWriteLockTest* test;
        PublicReentrantReadWriteLock* lock;
        Condition* cond;

    public:

        TestGetWaitingThreadsRunnable2(ReentrantReadWriteLockTest* test, PublicReentrantReadWriteLock* lock, Condition* cond) :
            Runnable(), test(test), lock(lock), cond(cond) {}
        virtual ~TestGetWaitingThreadsRunnable2() {}

        virtual void run() {
            try {
                lock->writeLock().lock();
                test->threadAssertFalse(
                    Pointer<Collection<Thread*> >(lock->getWaitingThreadsPublic(cond))->isEmpty());
                cond->await();
                lock->writeLock().unlock();
            } catch(InterruptedException& e) {
                test->threadUnexpectedException();
            }
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantReadWriteLockTest::testGetWaitingThreads() {
    PublicReentrantReadWriteLock lock;
    Pointer<Condition> c(lock.writeLock().newCondition());
    TestGetWaitingThreadsRunnable1 runnable1(this, &lock, c.get());
    TestGetWaitingThreadsRunnable2 runnable2(this, &lock, c.get());
    Thread t1(&runnable1);
    Thread t2(&runnable2);

    try {
        lock.writeLock().lock();
        CPPUNIT_ASSERT(Pointer<Collection<Thread*> >(lock.getWaitingThreadsPublic(c.get()))->isEmpty());
        lock.writeLock().unlock();
        t1.start();
        Thread::sleep( SHORT_DELAY_MS);
        t2.start();
        Thread::sleep(SHORT_DELAY_MS);
        lock.writeLock().lock();
        CPPUNIT_ASSERT(lock.hasWaiters(c.get()));
        CPPUNIT_ASSERT(Pointer<Collection<Thread*> >(lock.getWaitingThreadsPublic(c.get()))->contains(&t1));
        CPPUNIT_ASSERT(Pointer<Collection<Thread*> >(lock.getWaitingThreadsPublic(c.get()))->contains(&t2));
        c->signalAll();
        lock.writeLock().unlock();
        Thread::sleep(SHORT_DELAY_MS);
        lock.writeLock().lock();
        CPPUNIT_ASSERT(!lock.hasWaiters(c.get()));
        CPPUNIT_ASSERT(Pointer<Collection<Thread*> >(lock.getWaitingThreadsPublic(c.get()))->isEmpty());
        lock.writeLock().unlock();
        t1.join(SHORT_DELAY_MS);
        t2.join(SHORT_DELAY_MS);
        CPPUNIT_ASSERT(!t1.isAlive());
        CPPUNIT_ASSERT(!t2.isAlive());
    } catch (Exception& ex) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantReadWriteLockTest::testToString() {
    ReentrantReadWriteLock lock;
    std::string us = lock.toString();
    CPPUNIT_ASSERT(us.find_first_of("Write locks = 0") != std::string::npos);
    CPPUNIT_ASSERT(us.find_first_of("Read locks = 0") != std::string::npos);
    lock.writeLock().lock();
    std::string ws = lock.toString();
    CPPUNIT_ASSERT(ws.find_first_of("Write locks = 1") != std::string::npos);
    CPPUNIT_ASSERT(ws.find_first_of("Read locks = 0") != std::string::npos);
    lock.writeLock().unlock();
    lock.readLock().lock();
    lock.readLock().lock();
    std::string rs = lock.toString();
    CPPUNIT_ASSERT(rs.find_first_of("Write locks = 0") != std::string::npos);
    CPPUNIT_ASSERT(rs.find_first_of("Read locks = 2") != std::string::npos);
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantReadWriteLockTest::testReadLockToString() {
    ReentrantReadWriteLock lock;
    std::string us = lock.readLock().toString();
    CPPUNIT_ASSERT(us.find_first_of("Read locks = 0") != std::string::npos);
    lock.readLock().lock();
    lock.readLock().lock();
    std::string rs = lock.readLock().toString();
    CPPUNIT_ASSERT(rs.find_first_of("Read locks = 2") != std::string::npos);
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantReadWriteLockTest::testWriteLockToString() {
    ReentrantReadWriteLock lock;
    std::string us = lock.writeLock().toString();
    CPPUNIT_ASSERT(us.find_first_of("Unlocked") != std::string::npos);
    lock.writeLock().lock();
    std::string ls = lock.writeLock().toString();
    CPPUNIT_ASSERT(ls.find_first_of("Locked") != std::string::npos);
}
