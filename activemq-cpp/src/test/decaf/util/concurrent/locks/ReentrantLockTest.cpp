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

#include "ReentrantLockTest.h"

#include <decaf/lang/System.h>
#include <decaf/lang/Thread.h>
#include <decaf/lang/Runnable.h>
#include <decaf/util/Date.h>
#include <decaf/util/concurrent/TimeUnit.h>
#include <decaf/util/concurrent/locks/LockSupport.h>
#include <decaf/util/concurrent/locks/ReentrantLock.h>

using namespace std;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace decaf::util::concurrent::locks;

////////////////////////////////////////////////////////////////////////////////
namespace {

    class InterruptibleLockRunnable : public Runnable {
    private:

        ReentrantLock* lock;
        ReentrantLockTest* parent;

    private:

        InterruptibleLockRunnable(const InterruptibleLockRunnable&);
        InterruptibleLockRunnable operator= (const InterruptibleLockRunnable&);

    public:

        InterruptibleLockRunnable(ReentrantLock* lock, ReentrantLockTest* parent) :
            Runnable(), lock(lock), parent(parent) {}
        virtual ~InterruptibleLockRunnable() {}

        virtual void run() {
            try {
                lock->lockInterruptibly();
            } catch(InterruptedException& success){}
        }
    };

    class InterruptedLockRunnable : public Runnable {
    private:

        ReentrantLock* lock;
        ReentrantLockTest* parent;

    private:

        InterruptedLockRunnable(const InterruptedLockRunnable&);
        InterruptedLockRunnable operator= (const InterruptedLockRunnable&);

    public:

        InterruptedLockRunnable(ReentrantLock* lock, ReentrantLockTest* parent) :
            Runnable(), lock(lock), parent(parent) {}
        virtual ~InterruptedLockRunnable() {}

        virtual void run() {
            try {
                lock->lockInterruptibly();
                parent->threadShouldThrow();
            } catch(InterruptedException& success) {}
        }
    };

    class PublicReentrantLock : public ReentrantLock {
    public:

        PublicReentrantLock() : ReentrantLock() {}

        Collection<Thread*>* getQueuedThreads() const {
            return ReentrantLock::getQueuedThreads();
        }

        Collection<Thread*>* getWaitingThreads(Condition* condition) {
            return ReentrantLock::getWaitingThreads(condition);
        }
    };

    class UninterruptableThread : public Thread {
    private:

        ReentrantLock* lock;
        Condition* c;
        Mutex monitor;

    private:

        UninterruptableThread(const UninterruptableThread&);
        UninterruptableThread operator= (const UninterruptableThread&);

    public:

        volatile bool canAwake;
        volatile bool interrupted;
        volatile bool lockStarted;

        UninterruptableThread(ReentrantLock* lock, Condition* c) :
            Thread(), lock(lock), c(c), monitor(), canAwake(false), interrupted(false), lockStarted(false) {
        }
        virtual ~UninterruptableThread() {}

        virtual void run() {

            synchronized(&monitor) {
                lock->lock();
                lockStarted = true;

                while (!canAwake) {
                    c->awaitUninterruptibly();
                }

                interrupted = isInterrupted();
                lock->unlock();
            }
        }
    };

}

////////////////////////////////////////////////////////////////////////////////
ReentrantLockTest::ReentrantLockTest() {
}

////////////////////////////////////////////////////////////////////////////////
ReentrantLockTest::~ReentrantLockTest() {
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantLockTest::testConstructor() {
    ReentrantLock rl;
    CPPUNIT_ASSERT(!rl.isFair());
    ReentrantLock r2(true);
    CPPUNIT_ASSERT(r2.isFair());
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantLockTest::testLock() {
    ReentrantLock rl;
    rl.lock();
    CPPUNIT_ASSERT(rl.isLocked());
    rl.unlock();
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantLockTest::testFairLock() {
    ReentrantLock rl(true);
    rl.lock();
    CPPUNIT_ASSERT(rl.isLocked());
    rl.unlock();
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantLockTest::testUnlockIllegalMonitorStateException() {
    ReentrantLock rl;
    try {
        rl.unlock();
        shouldThrow();
    } catch(IllegalMonitorStateException& success) {
    }
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantLockTest::testTryLock() {
    ReentrantLock rl;
    CPPUNIT_ASSERT(rl.tryLock());
    CPPUNIT_ASSERT(rl.isLocked());
    rl.unlock();
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantLockTest::testhasQueuedThreads() {

    ReentrantLock lock;

    InterruptedLockRunnable interrupted(&lock, this);
    InterruptibleLockRunnable interruptable(&lock, this);

    Thread t1(&interrupted);
    Thread t2(&interruptable);

    try {
        CPPUNIT_ASSERT(!lock.hasQueuedThreads());
        lock.lock();
        t1.start();
        Thread::sleep(SHORT_DELAY_MS);
        CPPUNIT_ASSERT(lock.hasQueuedThreads());
        t2.start();
        Thread::sleep(SHORT_DELAY_MS);
        CPPUNIT_ASSERT(lock.hasQueuedThreads());
        t1.interrupt();
        Thread::sleep(SHORT_DELAY_MS);
        CPPUNIT_ASSERT(lock.hasQueuedThreads());
        lock.unlock();
        Thread::sleep(SHORT_DELAY_MS);
        CPPUNIT_ASSERT(!lock.hasQueuedThreads());
        t1.join();
        t2.join();
    } catch(Exception& e) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantLockTest::testGetQueueLength() {

    ReentrantLock lock;

    InterruptedLockRunnable interrupted(&lock, this);
    InterruptibleLockRunnable interruptable(&lock, this);

    Thread t1(&interrupted);
    Thread t2(&interruptable);

    try {
        CPPUNIT_ASSERT_EQUAL(0, lock.getQueueLength());
        lock.lock();
        t1.start();
        Thread::sleep(SHORT_DELAY_MS);
        CPPUNIT_ASSERT_EQUAL(1, lock.getQueueLength());
        t2.start();
        Thread::sleep(SHORT_DELAY_MS);
        CPPUNIT_ASSERT_EQUAL(2, lock.getQueueLength());
        t1.interrupt();
        Thread::sleep(SHORT_DELAY_MS);
        CPPUNIT_ASSERT_EQUAL(1, lock.getQueueLength());
        lock.unlock();
        Thread::sleep(SHORT_DELAY_MS);
        CPPUNIT_ASSERT_EQUAL(0, lock.getQueueLength());
        t1.join();
        t2.join();
    } catch(Exception& e) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantLockTest::testGetQueueLengthFair() {

    ReentrantLock lock(true);

    InterruptedLockRunnable interrupted(&lock, this);
    InterruptibleLockRunnable interruptable(&lock, this);

    Thread t1(&interrupted);
    Thread t2(&interruptable);

    try {
        CPPUNIT_ASSERT_EQUAL(0, lock.getQueueLength());
        lock.lock();
        t1.start();
        Thread::sleep( SHORT_DELAY_MS);
        CPPUNIT_ASSERT_EQUAL(1, lock.getQueueLength());
        t2.start();
        Thread::sleep(SHORT_DELAY_MS);
        CPPUNIT_ASSERT_EQUAL(2, lock.getQueueLength());
        t1.interrupt();
        Thread::sleep(SHORT_DELAY_MS);
        CPPUNIT_ASSERT_EQUAL(1, lock.getQueueLength());
        lock.unlock();
        Thread::sleep(SHORT_DELAY_MS);
        CPPUNIT_ASSERT_EQUAL(0, lock.getQueueLength());
        t1.join();
        t2.join();
    } catch(Exception& e) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantLockTest::testHasQueuedThreadNPE() {

    ReentrantLock sync;
    try {
        sync.hasQueuedThread(NULL);
        shouldThrow();
    } catch(NullPointerException& success) {
    }
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantLockTest::testHasQueuedThread() {

    ReentrantLock sync;

    InterruptedLockRunnable interrupted(&sync, this);
    InterruptibleLockRunnable interruptable(&sync, this);

    Thread t1(&interrupted);
    Thread t2(&interruptable);

    try {
        CPPUNIT_ASSERT(!sync.hasQueuedThread(&t1));
        CPPUNIT_ASSERT(!sync.hasQueuedThread(&t2));
        sync.lock();
        t1.start();
        Thread::sleep( SHORT_DELAY_MS);
        CPPUNIT_ASSERT(sync.hasQueuedThread(&t1));
        t2.start();
        Thread::sleep(SHORT_DELAY_MS);
        CPPUNIT_ASSERT(sync.hasQueuedThread(&t1));
        CPPUNIT_ASSERT(sync.hasQueuedThread(&t2));
        t1.interrupt();
        Thread::sleep(SHORT_DELAY_MS);
        CPPUNIT_ASSERT(!sync.hasQueuedThread(&t1));
        CPPUNIT_ASSERT(sync.hasQueuedThread(&t2));
        sync.unlock();
        Thread::sleep(SHORT_DELAY_MS);
        CPPUNIT_ASSERT(!sync.hasQueuedThread(&t1));
        Thread::sleep(SHORT_DELAY_MS);
        CPPUNIT_ASSERT(!sync.hasQueuedThread(&t2));
        t1.join();
        t2.join();
    } catch(Exception& e) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantLockTest::testGetQueuedThreads() {

    PublicReentrantLock lock;

    InterruptedLockRunnable interrupted(&lock, this);
    InterruptibleLockRunnable interruptable(&lock, this);

    Thread t1(&interrupted);
    Thread t2(&interruptable);

    try {
        CPPUNIT_ASSERT(std::auto_ptr<Collection<Thread*> >(lock.getQueuedThreads())->isEmpty());
        lock.lock();
        CPPUNIT_ASSERT(std::auto_ptr<Collection<Thread*> >(lock.getQueuedThreads())->isEmpty());
        t1.start();
        Thread::sleep( SHORT_DELAY_MS);
        CPPUNIT_ASSERT(std::auto_ptr<Collection<Thread*> >(lock.getQueuedThreads())->contains(&t1));
        t2.start();
        Thread::sleep(SHORT_DELAY_MS);
        CPPUNIT_ASSERT(std::auto_ptr<Collection<Thread*> >(lock.getQueuedThreads())->contains(&t1));
        CPPUNIT_ASSERT(std::auto_ptr<Collection<Thread*> >(lock.getQueuedThreads())->contains(&t2));
        t1.interrupt();
        Thread::sleep(SHORT_DELAY_MS);
        CPPUNIT_ASSERT(!std::auto_ptr<Collection<Thread*> >(lock.getQueuedThreads())->contains(&t1));
        CPPUNIT_ASSERT(std::auto_ptr<Collection<Thread*> >(lock.getQueuedThreads())->contains(&t2));
        lock.unlock();
        Thread::sleep(SHORT_DELAY_MS);
        CPPUNIT_ASSERT(std::auto_ptr<Collection<Thread*> >(lock.getQueuedThreads())->isEmpty());
        t1.join();
        t2.join();
    } catch(Exception& e) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class TestInterruptedException2Runnable : public Runnable {
    private:

        ReentrantLock* lock;
        ReentrantLockTest* parent;

    private:

        TestInterruptedException2Runnable(const TestInterruptedException2Runnable&);
        TestInterruptedException2Runnable operator= (const TestInterruptedException2Runnable&);

    public:

        TestInterruptedException2Runnable(ReentrantLock* lock, ReentrantLockTest* parent) :
            Runnable(), lock(lock), parent(parent) {}
        virtual ~TestInterruptedException2Runnable() {}

        virtual void run() {
            try {
                lock->tryLock(ReentrantLockTest::MEDIUM_DELAY_MS, TimeUnit::MILLISECONDS);
                parent->threadShouldThrow();
            } catch(InterruptedException& success){}
        }
    };

}

////////////////////////////////////////////////////////////////////////////////
void ReentrantLockTest::testInterruptedException2() {

    ReentrantLock lock;
    lock.lock();

    TestInterruptedException2Runnable runnable(&lock, this);
    Thread t(&runnable);

    try {
        t.start();
        t.interrupt();
    } catch(Exception& e) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class TestTryLockWhenLockedRunnable : public Runnable {
    private:

        ReentrantLock* lock;
        ReentrantLockTest* parent;

    private:

        TestTryLockWhenLockedRunnable(const TestTryLockWhenLockedRunnable&);
        TestTryLockWhenLockedRunnable operator= (const TestTryLockWhenLockedRunnable&);

    public:

        TestTryLockWhenLockedRunnable(ReentrantLock* lock, ReentrantLockTest* parent) :
            Runnable(), lock(lock), parent(parent) {}
        virtual ~TestTryLockWhenLockedRunnable() {}

        virtual void run() {
            parent->threadAssertFalse(lock->tryLock());
        }
    };

}

////////////////////////////////////////////////////////////////////////////////
void ReentrantLockTest::testTryLockWhenLocked() {

    ReentrantLock lock;
    TestTryLockWhenLockedRunnable runnable(&lock, this);
    lock.lock();
    Thread t(&runnable);

    try {
        t.start();
        t.join();
        lock.unlock();
    } catch(Exception& e) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class TestTryLockTimeoutRunnable : public Runnable {
    private:

        ReentrantLock* lock;
        ReentrantLockTest* parent;

    private:

        TestTryLockTimeoutRunnable(const TestTryLockTimeoutRunnable&);
        TestTryLockTimeoutRunnable operator= (const TestTryLockTimeoutRunnable&);

    public:

        TestTryLockTimeoutRunnable(ReentrantLock* lock, ReentrantLockTest* parent) :
            Runnable(), lock(lock), parent(parent) {}
        virtual ~TestTryLockTimeoutRunnable() {}

        virtual void run() {
            try {
                parent->threadAssertFalse(lock->tryLock(1, TimeUnit::MILLISECONDS));
            } catch (Exception& ex) {
                parent->threadUnexpectedException();
            }
        }
    };

}

////////////////////////////////////////////////////////////////////////////////
void ReentrantLockTest::testTryLockTimeout() {
    ReentrantLock lock;
    lock.lock();
    TestTryLockTimeoutRunnable runnable(&lock, this);
    Thread t(&runnable);

    try {
        t.start();
        t.join();
        lock.unlock();
    } catch(Exception& e) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantLockTest::testGetHoldCount() {
    ReentrantLock lock;
    const int SIZE = 50;

    for(int i = 1; i <= SIZE; i++) {
        lock.lock();
        CPPUNIT_ASSERT_EQUAL(i,lock.getHoldCount());
    }
    for(int i = SIZE; i > 0; i--) {
        lock.unlock();
        CPPUNIT_ASSERT_EQUAL(i-1,lock.getHoldCount());
    }
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class TestIsLockedRunnable : public Runnable {
    private:

        ReentrantLock* lock;
        ReentrantLockTest* parent;

    private:

        TestIsLockedRunnable(const TestIsLockedRunnable&);
        TestIsLockedRunnable operator= (const TestIsLockedRunnable&);

    public:

        TestIsLockedRunnable(ReentrantLock* lock, ReentrantLockTest* parent) :
            Runnable(), lock(lock), parent(parent) {}
        virtual ~TestIsLockedRunnable() {}

        virtual void run() {
            lock->lock();
            try {
                Thread::sleep(ReentrantLockTest::SMALL_DELAY_MS);
            } catch(Exception& e) {
                parent->threadUnexpectedException();
            }
            lock->unlock();
        }
    };

}

////////////////////////////////////////////////////////////////////////////////
void ReentrantLockTest::testIsLocked() {

    ReentrantLock lock;
    lock.lock();
    CPPUNIT_ASSERT(lock.isLocked());
    lock.unlock();
    CPPUNIT_ASSERT(!lock.isLocked());
    TestIsLockedRunnable runnable(&lock, this);
    Thread t(&runnable);

    try {
        t.start();
        Thread::sleep(SHORT_DELAY_MS);
        CPPUNIT_ASSERT(lock.isLocked());
        t.join();
        CPPUNIT_ASSERT(!lock.isLocked());
    } catch(Exception& e) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantLockTest::testLockInterruptibly1() {
    ReentrantLock lock;
    lock.lock();
    InterruptedLockRunnable runnable(&lock, this);
    Thread t(&runnable);

    try {
        t.start();
        Thread::sleep(SHORT_DELAY_MS);
        t.interrupt();
        Thread::sleep(SHORT_DELAY_MS);
        lock.unlock();
        t.join();
    } catch(Exception& e) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantLockTest::testLockInterruptibly2() {
    ReentrantLock lock;
    try {
        lock.lockInterruptibly();
    } catch(Exception& e) {
        unexpectedException();
    }

    InterruptedLockRunnable runnable(&lock, this);
    Thread t(&runnable);

    try {
        t.start();
        t.interrupt();
        CPPUNIT_ASSERT(lock.isLocked());
        CPPUNIT_ASSERT(lock.isHeldByCurrentThread());
        t.join();
    } catch(Exception& e) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantLockTest::testAwaitIllegalMonitor() {
    ReentrantLock lock;
    std::auto_ptr<Condition> c(lock.newCondition());
    try {
        c->await();
        shouldThrow();
    } catch(IllegalMonitorStateException& success) {
    } catch(Exception& ex) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantLockTest::testSignalIllegalMonitor() {
    ReentrantLock lock;
    std::auto_ptr<Condition> c(lock.newCondition());
    try {
        c->signal();
        shouldThrow();
    } catch(IllegalMonitorStateException& success) {
    } catch(Exception& ex) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantLockTest::testAwaitNanosTimeout() {
    ReentrantLock lock;
    std::auto_ptr<Condition> c(lock.newCondition());
    try {
        lock.lock();
        long long t = c->awaitNanos(100);
        CPPUNIT_ASSERT(t <= 0);
        lock.unlock();
    } catch(Exception& ex) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantLockTest::testAwaitTimeout() {
    ReentrantLock lock;
    std::auto_ptr<Condition> c(lock.newCondition());
    try {
        lock.lock();
        c->await(SHORT_DELAY_MS, TimeUnit::MILLISECONDS);
        lock.unlock();
    } catch(Exception& ex) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantLockTest::testAwaitUntilTimeout() {

    ReentrantLock lock;
    std::auto_ptr<Condition> c(lock.newCondition());
    try {
        lock.lock();
        Date d;
        c->awaitUntil(Date(d.getTime() + 10));
        lock.unlock();
    } catch(Exception& ex) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class TestAwaitRunnable : public Runnable {
    private:

        ReentrantLock* lock;
        Condition* condition;
        ReentrantLockTest* parent;

    private:

        TestAwaitRunnable(const TestAwaitRunnable&);
        TestAwaitRunnable operator= (const TestAwaitRunnable&);

    public:

        TestAwaitRunnable(ReentrantLock* lock, Condition* condition, ReentrantLockTest* parent) :
            Runnable(), lock(lock), condition(condition), parent(parent) {}
        virtual ~TestAwaitRunnable() {}

        virtual void run() {
            try {
                lock->lock();
                condition->await();
                lock->unlock();
            } catch(Exception& e) {
                parent->threadUnexpectedException();
            }
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantLockTest::testAwait() {

    ReentrantLock lock;
    std::auto_ptr<Condition> c(lock.newCondition());
    TestAwaitRunnable runnable(&lock, c.get(), this);
    Thread t(&runnable);

    try {
        t.start();
        Thread::sleep(SHORT_DELAY_MS);
        lock.lock();
        c->signal();
        lock.unlock();
        t.join(SHORT_DELAY_MS);
        CPPUNIT_ASSERT(!t.isAlive());
    } catch (Exception& ex) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantLockTest::testHasWaitersNPE() {
    ReentrantLock lock;
    try {
        lock.hasWaiters(NULL);
        shouldThrow();
    } catch(NullPointerException& success) {
    } catch(Exception& ex) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantLockTest::testGetWaitQueueLengthNPE() {
    ReentrantLock lock;
    try {
        lock.getWaitQueueLength(NULL);
        shouldThrow();
    } catch(NullPointerException& success) {
    } catch(Exception& ex) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantLockTest::testGetWaitingThreadsNPE() {
    PublicReentrantLock lock;
    try {
        lock.getWaitingThreads(NULL);
        shouldThrow();
    } catch(NullPointerException& success) {
    } catch(Exception& ex) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantLockTest::testHasWaitersIAE() {
    ReentrantLock lock;
    std::auto_ptr<Condition> c(lock.newCondition());
    ReentrantLock lock2;
    try {
        lock2.hasWaiters(c.get());
        shouldThrow();
    } catch(IllegalArgumentException& success) {
    } catch(Exception& ex) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantLockTest::testHasWaitersIMSE() {
    ReentrantLock lock;
    std::auto_ptr<Condition> c(lock.newCondition());
    try {
        lock.hasWaiters(c.get());
        shouldThrow();
    } catch(IllegalMonitorStateException& success) {
    } catch(Exception& ex) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantLockTest::testGetWaitQueueLengthIAE() {
    ReentrantLock lock;
    std::auto_ptr<Condition> c(lock.newCondition());
    ReentrantLock lock2;
    try {
        lock2.getWaitQueueLength(c.get());
        shouldThrow();
    } catch(IllegalArgumentException& success) {
    } catch(Exception& ex) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantLockTest::testGetWaitQueueLengthIMSE() {
    ReentrantLock lock;
    std::auto_ptr<Condition> c(lock.newCondition());
    try {
        lock.getWaitQueueLength(c.get());
        shouldThrow();
    } catch(IllegalMonitorStateException& success) {
    } catch(Exception& ex) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantLockTest::testGetWaitingThreadsIAE() {

    PublicReentrantLock lock;
    std::auto_ptr<Condition> c(lock.newCondition());
    PublicReentrantLock lock2;
    try {
        lock2.getWaitingThreads(c.get());
        shouldThrow();
    } catch(IllegalArgumentException& success) {
    } catch(Exception& ex) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantLockTest::testGetWaitingThreadsIMSE() {

    PublicReentrantLock lock;
    std::auto_ptr<Condition> c(lock.newCondition());
    try {
        lock.getWaitingThreads(c.get());
        shouldThrow();
    } catch(IllegalMonitorStateException& success) {
    } catch(Exception& ex) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class TestHasWaitersRunnable : public Runnable {
    private:

        ReentrantLock* lock;
        Condition* condition;
        ReentrantLockTest* parent;

    private:

        TestHasWaitersRunnable(const TestHasWaitersRunnable&);
        TestHasWaitersRunnable operator= (const TestHasWaitersRunnable&);

    public:

        TestHasWaitersRunnable(ReentrantLock* lock, Condition* condition, ReentrantLockTest* parent) :
            Runnable(), lock(lock), condition(condition), parent(parent) {}
        virtual ~TestHasWaitersRunnable() {}

        virtual void run() {
            try {
                lock->lock();
                parent->threadAssertFalse(lock->hasWaiters(condition));
                parent->threadAssertEquals(0, lock->getWaitQueueLength(condition));
                condition->await();
                lock->unlock();
            } catch(Exception& e) {
                parent->threadUnexpectedException();
            }
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantLockTest::testHasWaiters() {

    ReentrantLock lock;
    std::auto_ptr<Condition> c(lock.newCondition());
    TestHasWaitersRunnable runnable(&lock, c.get(), this);
    Thread t(&runnable);

    try {
        t.start();
        Thread::sleep(SHORT_DELAY_MS);
        lock.lock();
        CPPUNIT_ASSERT(lock.hasWaiters(c.get()));
        CPPUNIT_ASSERT_EQUAL(1, lock.getWaitQueueLength(c.get()));
        c->signal();
        lock.unlock();
        Thread::sleep(SHORT_DELAY_MS);
        lock.lock();
        CPPUNIT_ASSERT(!lock.hasWaiters(c.get()));
        CPPUNIT_ASSERT_EQUAL(0, lock.getWaitQueueLength(c.get()));
        lock.unlock();
        t.join(SHORT_DELAY_MS);
        CPPUNIT_ASSERT(!t.isAlive());
    } catch(Exception& ex) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class TestGetWaitQueueLengthRunnable1 : public Runnable {
    private:

        ReentrantLock* lock;
        Condition* condition;
        ReentrantLockTest* parent;

    private:

        TestGetWaitQueueLengthRunnable1(const TestGetWaitQueueLengthRunnable1&);
        TestGetWaitQueueLengthRunnable1 operator= (const TestGetWaitQueueLengthRunnable1&);

    public:

        TestGetWaitQueueLengthRunnable1(ReentrantLock* lock, Condition* condition, ReentrantLockTest* parent) :
            Runnable(), lock(lock), condition(condition), parent(parent) {}
        virtual ~TestGetWaitQueueLengthRunnable1() {}

        virtual void run() {
            try {
                lock->lock();
                parent->threadAssertFalse(lock->hasWaiters(condition));
                parent->threadAssertEquals(0, lock->getWaitQueueLength(condition));
                condition->await();
                lock->unlock();
            } catch(Exception& e) {
                parent->threadUnexpectedException();
            }
        }
    };

    class TestGetWaitQueueLengthRunnable2 : public Runnable {
    private:

        ReentrantLock* lock;
        Condition* condition;
        ReentrantLockTest* parent;

    private:

        TestGetWaitQueueLengthRunnable2(const TestGetWaitQueueLengthRunnable2&);
        TestGetWaitQueueLengthRunnable2 operator= (const TestGetWaitQueueLengthRunnable2&);

    public:

        TestGetWaitQueueLengthRunnable2(ReentrantLock* lock, Condition* condition, ReentrantLockTest* parent) :
            Runnable(), lock(lock), condition(condition), parent(parent) {}
        virtual ~TestGetWaitQueueLengthRunnable2() {}

        virtual void run() {
            try {
                lock->lock();
                parent->threadAssertTrue(lock->hasWaiters(condition));
                parent->threadAssertEquals(1, lock->getWaitQueueLength(condition));
                condition->await();
                lock->unlock();
            } catch(Exception& e) {
                parent->threadUnexpectedException();
            }
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantLockTest::testGetWaitQueueLength() {

    ReentrantLock lock;
    std::auto_ptr<Condition> c(lock.newCondition());
    TestGetWaitQueueLengthRunnable1 runnable1(&lock, c.get(), this);
    Thread t1(&runnable1);
    TestGetWaitQueueLengthRunnable2 runnable2(&lock, c.get(), this);
    Thread t2(&runnable2);

    try {
        t1.start();
        Thread::sleep(SHORT_DELAY_MS);
        t2.start();
        Thread::sleep(SHORT_DELAY_MS);
        lock.lock();
        CPPUNIT_ASSERT(lock.hasWaiters(c.get()));
        CPPUNIT_ASSERT_EQUAL(2, lock.getWaitQueueLength(c.get()));
        c->signalAll();
        lock.unlock();
        Thread::sleep(SHORT_DELAY_MS);
        lock.lock();
        CPPUNIT_ASSERT(!lock.hasWaiters(c.get()));
        CPPUNIT_ASSERT_EQUAL(0, lock.getWaitQueueLength(c.get()));
        lock.unlock();
        t1.join(SHORT_DELAY_MS);
        t2.join(SHORT_DELAY_MS);
        CPPUNIT_ASSERT(!t1.isAlive());
        CPPUNIT_ASSERT(!t2.isAlive());
    } catch(Exception& ex) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class TestGetWaitingThreadsRunnable1 : public Runnable {
    private:

        PublicReentrantLock* lock;
        Condition* condition;
        ReentrantLockTest* parent;

    private:

        TestGetWaitingThreadsRunnable1(const TestGetWaitingThreadsRunnable1&);
        TestGetWaitingThreadsRunnable1 operator= (const TestGetWaitingThreadsRunnable1&);

    public:

        TestGetWaitingThreadsRunnable1(PublicReentrantLock* lock, Condition* condition, ReentrantLockTest* parent) :
            Runnable(), lock(lock), condition(condition), parent(parent) {}
        virtual ~TestGetWaitingThreadsRunnable1() {}

        virtual void run() {
            try {
                lock->lock();
                parent->threadAssertTrue(std::auto_ptr<Collection<Thread*> >(lock->getWaitingThreads(condition))->isEmpty());
                condition->await();
                lock->unlock();
            } catch(Exception& e) {
                parent->threadUnexpectedException();
            }
        }
    };

    class TestGetWaitingThreadsRunnable2 : public Runnable {
    private:

        PublicReentrantLock* lock;
        Condition* condition;
        ReentrantLockTest* parent;

    private:

        TestGetWaitingThreadsRunnable2(const TestGetWaitingThreadsRunnable2&);
        TestGetWaitingThreadsRunnable2 operator= (const TestGetWaitingThreadsRunnable2&);

    public:

        TestGetWaitingThreadsRunnable2(PublicReentrantLock* lock, Condition* condition, ReentrantLockTest* parent) :
            Runnable(), lock(lock), condition(condition), parent(parent) {}
        virtual ~TestGetWaitingThreadsRunnable2() {}

        virtual void run() {
            try {
                lock->lock();
                parent->threadAssertFalse(std::auto_ptr<Collection<Thread*> >(lock->getWaitingThreads(condition))->isEmpty());
                condition->await();
                lock->unlock();
            } catch(Exception& e) {
                parent->threadUnexpectedException();
            }
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantLockTest::testGetWaitingThreads() {

    PublicReentrantLock lock;
    std::auto_ptr<Condition> c(lock.newCondition());
    TestGetWaitingThreadsRunnable1 runnable1(&lock, c.get(), this);
    Thread t1(&runnable1);
    TestGetWaitingThreadsRunnable2 runnable2(&lock, c.get(), this);
    Thread t2(&runnable2);

    try {
        lock.lock();
        CPPUNIT_ASSERT(std::auto_ptr<Collection<Thread*> >(lock.getWaitingThreads(c.get()))->isEmpty());
        lock.unlock();
        t1.start();
        Thread::sleep(SHORT_DELAY_MS);
        t2.start();
        Thread::sleep(SHORT_DELAY_MS);
        lock.lock();
        CPPUNIT_ASSERT(lock.hasWaiters(c.get()));
        CPPUNIT_ASSERT(std::auto_ptr<Collection<Thread*> >(lock.getWaitingThreads(c.get()))->contains(&t1));
        CPPUNIT_ASSERT(std::auto_ptr<Collection<Thread*> >(lock.getWaitingThreads(c.get()))->contains(&t2));
        c->signalAll();
        lock.unlock();
        Thread::sleep(SHORT_DELAY_MS);
        lock.lock();
        CPPUNIT_ASSERT(!lock.hasWaiters(c.get()));
        CPPUNIT_ASSERT(std::auto_ptr<Collection<Thread*> >(lock.getWaitingThreads(c.get()))->isEmpty());
        lock.unlock();
        t1.join(SHORT_DELAY_MS);
        t2.join(SHORT_DELAY_MS);
        CPPUNIT_ASSERT(!t1.isAlive());
        CPPUNIT_ASSERT(!t2.isAlive());
    } catch(Exception& ex) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantLockTest::testAwaitUninterruptibly() {

    ReentrantLock lock;
    std::auto_ptr<Condition> c(lock.newCondition());
    UninterruptableThread thread(&lock, c.get());

    try {
        thread.start();

        while(!thread.lockStarted) {
            Thread::sleep(100);
        }

        lock.lock();
        try {
            thread.interrupt();
            thread.canAwake = true;
            c->signal();
        } catch(...) {
        }
        lock.unlock();

        thread.join();
        CPPUNIT_ASSERT(thread.interrupted);
        CPPUNIT_ASSERT(!thread.isAlive());
    } catch(Exception& ex) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class TestAwaitInterruptRunnable : public Runnable {
    private:

        ReentrantLock* lock;
        Condition* condition;
        ReentrantLockTest* parent;

    private:

        TestAwaitInterruptRunnable(const TestAwaitInterruptRunnable&);
        TestAwaitInterruptRunnable operator= (const TestAwaitInterruptRunnable&);

    public:

        TestAwaitInterruptRunnable(ReentrantLock* lock, Condition* condition, ReentrantLockTest* parent) :
            Runnable(), lock(lock), condition(condition), parent(parent) {}
        virtual ~TestAwaitInterruptRunnable() {}

        virtual void run() {
            try {
                lock->lock();
                condition->await();
                lock->unlock();
                parent->threadShouldThrow();
            } catch(InterruptedException& e) {
            }
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantLockTest::testAwaitInterrupt() {

    ReentrantLock lock;
    std::auto_ptr<Condition> c(lock.newCondition());
    TestAwaitInterruptRunnable runnable(&lock, c.get(), this);
    Thread t(&runnable);

    try {
        t.start();
        Thread::sleep(SHORT_DELAY_MS);
        t.interrupt();
        t.join(SHORT_DELAY_MS);
        CPPUNIT_ASSERT(!t.isAlive());
    } catch(Exception& ex) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class TestAwaitNanosInterruptRunnable : public Runnable {
    private:

        ReentrantLock* lock;
        Condition* condition;
        ReentrantLockTest* parent;

    private:

        TestAwaitNanosInterruptRunnable(const TestAwaitNanosInterruptRunnable&);
        TestAwaitNanosInterruptRunnable operator= (const TestAwaitNanosInterruptRunnable&);

    public:

        TestAwaitNanosInterruptRunnable(ReentrantLock* lock, Condition* condition, ReentrantLockTest* parent) :
            Runnable(), lock(lock), condition(condition), parent(parent) {}
        virtual ~TestAwaitNanosInterruptRunnable() {}

        virtual void run() {
            try {
                lock->lock();
                condition->awaitNanos(1000 * 1000 * 1000); // 1 sec
                lock->unlock();
                parent->threadShouldThrow();
            } catch(InterruptedException& e) {
            }
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantLockTest::testAwaitNanosInterrupt() {

    ReentrantLock lock;
    std::auto_ptr<Condition> c(lock.newCondition());
    TestAwaitNanosInterruptRunnable runnable(&lock, c.get(), this);
    Thread t(&runnable);

    try {
        t.start();
        Thread::sleep(SHORT_DELAY_MS);
        t.interrupt();
        t.join(SHORT_DELAY_MS);
        CPPUNIT_ASSERT(!t.isAlive());
    } catch(Exception& ex) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class TestAwaitUntilInterruptRunnable : public Runnable {
    private:

        ReentrantLock* lock;
        Condition* condition;
        ReentrantLockTest* parent;

    private:

        TestAwaitUntilInterruptRunnable(const TestAwaitUntilInterruptRunnable&);
        TestAwaitUntilInterruptRunnable operator= (const TestAwaitUntilInterruptRunnable&);

    public:

        TestAwaitUntilInterruptRunnable(ReentrantLock* lock, Condition* condition, ReentrantLockTest* parent) :
            Runnable(), lock(lock), condition(condition), parent(parent) {}
        virtual ~TestAwaitUntilInterruptRunnable() {}

        virtual void run() {
            try {
                lock->lock();
                Date d;
                condition->awaitUntil(Date(d.getTime() + 10000));
                lock->unlock();
                parent->threadShouldThrow();
            } catch(InterruptedException& e) {
            }
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantLockTest::testAwaitUntilInterrupt() {

    ReentrantLock lock;
    std::auto_ptr<Condition> c(lock.newCondition());
    TestAwaitNanosInterruptRunnable runnable(&lock, c.get(), this);
    Thread t(&runnable);

    try {
        t.start();
        Thread::sleep(SHORT_DELAY_MS);
        t.interrupt();
        t.join(SHORT_DELAY_MS);
        CPPUNIT_ASSERT(!t.isAlive());
    } catch(Exception& ex) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class TestSignalAllRunnable : public Runnable {
    private:

        ReentrantLock* lock;
        Condition* condition;
        ReentrantLockTest* parent;

    private:

        TestSignalAllRunnable(const TestSignalAllRunnable&);
        TestSignalAllRunnable operator= (const TestSignalAllRunnable&);

    public:

        TestSignalAllRunnable(ReentrantLock* lock, Condition* condition, ReentrantLockTest* parent) :
            Runnable(), lock(lock), condition(condition), parent(parent) {}
        virtual ~TestSignalAllRunnable() {}

        virtual void run() {
            try {
                lock->lock();
                condition->await();
                lock->unlock();
            } catch(InterruptedException& e) {
                parent->threadUnexpectedException();
            }
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantLockTest::testSignalAll() {

    ReentrantLock lock;
    std::auto_ptr<Condition> c(lock.newCondition());
    TestSignalAllRunnable runnable1(&lock, c.get(), this);
    Thread t1(&runnable1);
    TestSignalAllRunnable runnable2(&lock, c.get(), this);
    Thread t2(&runnable2);

    try {
        t1.start();
        t2.start();
        Thread::sleep(SHORT_DELAY_MS);
        lock.lock();
        c->signalAll();
        lock.unlock();
        t1.join(SHORT_DELAY_MS);
        t2.join(SHORT_DELAY_MS);
        CPPUNIT_ASSERT(!t1.isAlive());
        CPPUNIT_ASSERT(!t2.isAlive());
    } catch(Exception& ex) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class TestAwaitLockCountRunnable1 : public Runnable {
    private:

        ReentrantLock* lock;
        Condition* condition;
        ReentrantLockTest* parent;

    private:

        TestAwaitLockCountRunnable1(const TestAwaitLockCountRunnable1&);
        TestAwaitLockCountRunnable1 operator= (const TestAwaitLockCountRunnable1&);

    public:

        TestAwaitLockCountRunnable1(ReentrantLock* lock, Condition* condition, ReentrantLockTest* parent) :
            Runnable(), lock(lock), condition(condition), parent(parent) {}
        virtual ~TestAwaitLockCountRunnable1() {}

        virtual void run() {
            try {
                lock->lock();
                parent->threadAssertEquals(1, lock->getHoldCount());
                condition->await();
                parent->threadAssertEquals(1, lock->getHoldCount());
                lock->unlock();
            } catch(InterruptedException& e) {
                parent->threadUnexpectedException();
            }
        }
    };

    class TestAwaitLockCountRunnable2 : public Runnable {
    private:

        ReentrantLock* lock;
        Condition* condition;
        ReentrantLockTest* parent;

    private:

        TestAwaitLockCountRunnable2(const TestAwaitLockCountRunnable2&);
        TestAwaitLockCountRunnable2 operator= (const TestAwaitLockCountRunnable2&);

    public:

        TestAwaitLockCountRunnable2(ReentrantLock* lock, Condition* condition, ReentrantLockTest* parent) :
            Runnable(), lock(lock), condition(condition), parent(parent) {}
        virtual ~TestAwaitLockCountRunnable2() {}

        virtual void run() {
            try {
                lock->lock();
                lock->lock();
                parent->threadAssertEquals(2, lock->getHoldCount());
                condition->await();
                parent->threadAssertEquals(2, lock->getHoldCount());
                lock->unlock();
                lock->unlock();
            } catch(InterruptedException& e) {
                parent->threadUnexpectedException();
            }
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantLockTest::testAwaitLockCount() {

    ReentrantLock lock;
    std::auto_ptr<Condition> c(lock.newCondition());
    TestAwaitLockCountRunnable1 runnable1(&lock, c.get(), this);
    Thread t1(&runnable1);
    TestAwaitLockCountRunnable2 runnable2(&lock, c.get(), this);
    Thread t2(&runnable2);

    try {
        t1.start();
        t2.start();
        Thread::sleep( SHORT_DELAY_MS);
        lock.lock();
        c->signalAll();
        lock.unlock();
        t1.join(SHORT_DELAY_MS);
        t2.join(SHORT_DELAY_MS);
        CPPUNIT_ASSERT(!t1.isAlive());
        CPPUNIT_ASSERT(!t2.isAlive());
    } catch(Exception& ex) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantLockTest::testToString() {

    ReentrantLock lock;
    std::string us = lock.toString();
    CPPUNIT_ASSERT((int)us.find_first_of("Unlocked") >= 0);
    lock.lock();
    std::string ls = lock.toString();
    CPPUNIT_ASSERT((int)ls.find_first_of("Locked") >= 0);
}
