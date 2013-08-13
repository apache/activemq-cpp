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

#include "ReentrantReadWriteLock.h"

#include <decaf/lang/Exception.h>
#include <decaf/lang/Integer.h>
#include <decaf/lang/Thread.h>
#include <decaf/lang/ThreadLocal.h>
#include <decaf/lang/exceptions/IllegalMonitorStateException.h>
#include <decaf/util/concurrent/locks/AbstractQueuedSynchronizer.h>
#include <decaf/util/concurrent/atomic/AtomicReference.h>

using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace decaf::util::concurrent::locks;
using namespace decaf::util::concurrent::atomic;

////////////////////////////////////////////////////////////////////////////////
namespace {

    /**
     * A counter for per-thread read hold counts. Maintained as a ThreadLocal;
     * cached in cachedHoldCounter in class Sync
     */
    struct HoldCounter {
        Thread* thread;
        int count;

        HoldCounter() : thread(Thread::currentThread()), count(0) {}
    };

    class ThreadLocalHoldCounter : public ThreadLocal<HoldCounter> {
    public:

        virtual ~ThreadLocalHoldCounter() {}

        virtual HoldCounter initialValue() const {
            return HoldCounter();
        }
    };

    class Sync : public AbstractQueuedSynchronizer {
    private:

        /*
         * Read vs write count extraction constants and functions.
         * Lock state is logically divided into two unsigned shorts:
         * The lower one representing the exclusive (writer) lock hold count,
         * and the upper the shared (reader) hold count.
         */

        static const int SHARED_SHIFT;
        static const int SHARED_UNIT;
        static const int MAX_COUNT;
        static const int EXCLUSIVE_MASK;

    private:

        Sync(const Sync&);
        Sync& operator=(const Sync&);

    public:

        /** Returns the number of shared holds represented in count  */
        static int sharedCount(int c) {
            return ((unsigned int)c >> SHARED_SHIFT);
        }

        /** Returns the number of exclusive holds represented in count  */
        static int exclusiveCount(int c) {
            return c & EXCLUSIVE_MASK;
        }

    private:

        /**
         * The number of reentrant read locks held by current thread.
         * Initialized only in constructor and readObject.
         * Removed whenever a thread's read hold count drops to 0.
         */
        ThreadLocalHoldCounter readHolds;

        /**
         * firstReader is the first thread to have acquired the read lock.
         * firstReaderHoldCount is firstReader's hold count.
         *
         * <p>More precisely, firstReader is the unique thread that last
         * changed the shared count from 0 to 1, and has not released the
         * read lock since then; NULL if there is no such thread.
         *
         * <p>This allows tracking of read holds for uncontended read
         * locks to be very cheap.
         */
        Thread* firstReader;
        int firstReaderHoldCount;

    public:

        Sync() : readHolds(), firstReader(NULL), firstReaderHoldCount(0) {}

        virtual ~Sync() {}

        virtual bool isFair() const = 0;

        /*
         * Note that tryRelease and tryAcquire can be called by
         * Conditions. So it is possible that their arguments contain
         * both read and write holds that are all released during a
         * condition wait and re-established in tryAcquire.
         */

        virtual bool tryRelease(int releases) {

            if (!this->isHeldExclusively()) {
                throw IllegalMonitorStateException(__FILE__, __LINE__, "Sync lock not held exclusively");
            }

            int nextc = getState() - releases;
            bool free = exclusiveCount(nextc) == 0;
            if (free) {
                setExclusiveOwnerThread(NULL);
            }
            setState(nextc);
            return free;
        }

        bool tryAcquire(int acquires) {
            /*
             * Walkthrough:
             * 1. If read count is nonzero or write count is nonzero
             *    and owner is a different thread, fail.
             * 2. If count would saturate, fail. (This can only
             *    happen if count is already nonzero.)
             * 3. Otherwise, this thread is eligible for lock if it is
             *    either a reentrant acquire or queue policy allows it.
             *    If so, update state and set owner.
             */
            Thread* current = Thread::currentThread();
            int c = getState();
            int w = exclusiveCount(c);

            if (c != 0) {
                // (Note: if c != 0 and w == 0 then shared count != 0)
                if (w == 0 || current != getExclusiveOwnerThread()) {
                    return false;
                }
                if (w + exclusiveCount(acquires) > MAX_COUNT) {
                    throw new RuntimeException(__FILE__, __LINE__, "Maximum lock count exceeded");
                }
                // Reentrant acquire
                setState(c + acquires);
                return true;
            }

            if (writerShouldBlock() || !compareAndSetState(c, c + acquires)) {
                return false;
            }

            setExclusiveOwnerThread(current);
            return true;
        }

        bool tryReleaseShared(int unused DECAF_UNUSED) {
            Thread* current = Thread::currentThread();
            if (firstReader == current) {
                if (firstReaderHoldCount == 1) {
                    firstReader = NULL;
                } else {
                    firstReaderHoldCount--;
                }
            } else {
                HoldCounter rh = readHolds.get();
                int count = rh.count;
                if (count <= 1) {
                    readHolds.remove();
                    if (count <= 0) {
                        throw IllegalMonitorStateException(
                            __FILE__, __LINE__, "attempt to unlock read lock, not locked by current thread");
                    }
                }
                --rh.count;
                readHolds.set(rh);
            }

            for (;;) {
                int c = getState();
                int nextc = c - SHARED_UNIT;
                if (compareAndSetState(c, nextc)) {
                    // Releasing the read lock has no effect on readers, but it may allow
                    // waiting writers to proceed if both read and write locks are now free.
                    return nextc == 0;
                }
            }
        }

        virtual int tryAcquireShared(int unused DECAF_UNUSED) {
            /*
             * Walk through:
             * 1. If write lock held by another thread, fail.
             * 2. Otherwise, this thread is eligible for lock wrt state, so
             *    ask if it should block because of queue policy. If not, try
             *    to grant by CASing state and updating count. Note that step
             *    does not check for reentrant acquires, which is postponed to
             *    full version to avoid having to check hold count in the more
             *    typical non-reentrant case.
             * 3. If step 2 fails either because thread apparently not eligible
             *    or CAS fails or count saturated, chain to version with full
             *    retry loop.
             */
            Thread* current = Thread::currentThread();
            int c = getState();
            if (exclusiveCount(c) != 0 && getExclusiveOwnerThread() != current) {
                return -1;
            }
            int r = sharedCount(c);
            if (!readerShouldBlock() && r < MAX_COUNT && compareAndSetState(c, c + SHARED_UNIT)) {
                if (r == 0) {
                    firstReader = current;
                    firstReaderHoldCount = 1;
                } else if (firstReader == current) {
                    firstReaderHoldCount++;
                } else {
                    HoldCounter rh = readHolds.get();
                    rh.count++;
                    readHolds.set(rh);
                }
                return 1;
            }

            return fullTryAcquireShared(current);
        }

        /**
         * Full version of acquire for reads, that handles CAS misses
         * and reentrant reads not dealt with in tryAcquireShared.
         */
        int fullTryAcquireShared(Thread* current) {

            HoldCounter rh;
            for (;;) {
                int c = getState();
                if (exclusiveCount(c) != 0) {
                    if (getExclusiveOwnerThread() != current) {
                        return -1;
                    }
                    // else we hold the exclusive lock; blocking here
                    // would cause deadlock.
                } else if (readerShouldBlock()) {
                    // Make sure we're not acquiring read lock reentrantly
                    if (firstReader == current) {
                        if (firstReaderHoldCount > 0) {
                            throw Exception(__FILE__, __LINE__, "Read lock should not be aquired reentrantlly.");
                        }
                    } else {
                        rh = readHolds.get();
                        if (rh.count == 0) {
                            readHolds.remove();
                        }

                        if (rh.count == 0) {
                            return -1;
                        }
                    }
                }
                if (sharedCount(c) == MAX_COUNT) {
                    throw Exception(__FILE__, __LINE__, "Maximum lock count exceeded");
                }
                if (compareAndSetState(c, c + SHARED_UNIT)) {
                    if (sharedCount(c) == 0) {
                        firstReader = current;
                        firstReaderHoldCount = 1;
                    } else if (firstReader == current) {
                        firstReaderHoldCount++;
                    } else {
                        rh = readHolds.get();
                        rh.count++;
                        readHolds.set(rh);
                    }
                    return 1;
                }
            }
        }

        /**
         * Performs tryLock for write, enabling barging in both modes.
         * This is identical in effect to tryAcquire except for lack
         * of calls to writerShouldBlock.
         */
        bool tryWriteLock() {
            Thread* current = Thread::currentThread();
            int c = getState();
            if (c != 0) {
                int w = exclusiveCount(c);
                if (w == 0 || current != getExclusiveOwnerThread()) {
                    return false;
                }
                if (w == MAX_COUNT) {
                    throw new Exception(__FILE__, __LINE__, "Maximum lock count exceeded");
                }
            }
            if (!compareAndSetState(c, c + 1)) {
                return false;
            }
            setExclusiveOwnerThread(current);
            return true;
        }

        /**
         * Performs tryLock for read, enabling barging in both modes.
         * This is identical in effect to tryAcquireShared except for
         * lack of calls to readerShouldBlock.
         */
        bool tryReadLock() {
            Thread* current = Thread::currentThread();
            for (;;) {
                int c = getState();
                if (exclusiveCount(c) != 0 && getExclusiveOwnerThread() != current) {
                    return false;
                }
                int r = sharedCount(c);
                if (r == MAX_COUNT) {
                    throw Exception(__FILE__, __LINE__, "Maximum lock count exceeded");
                }
                if (compareAndSetState(c, c + SHARED_UNIT)) {
                    if (r == 0) {
                        firstReader = current;
                        firstReaderHoldCount = 1;
                    } else if (firstReader == current) {
                        firstReaderHoldCount++;
                    } else {
                        HoldCounter rh = readHolds.get();//cachedHoldCounter;
                        rh.count++;
                        readHolds.set(rh);
                    }
                    return true;
                }
            }
        }

        virtual bool isHeldExclusively() const {
            // While we must in general read state before owner, we don't need to do
            // so to check if current thread is owner
            return getExclusiveOwnerThread() == Thread::currentThread();
        }

        ConditionObject* newCondition() {
            return AbstractQueuedSynchronizer::createDefaultConditionObject();
        }

        Thread* getOwner() {
            // Must read state before owner to ensure memory consistency
            return ((exclusiveCount(getState()) == 0)? NULL : getExclusiveOwnerThread());
        }

        int getReadLockCount() {
            return sharedCount(getState());
        }

        bool isWriteLocked() {
            return exclusiveCount(getState()) != 0;
        }

        int getWriteHoldCount() {
            return isHeldExclusively() ? exclusiveCount(getState()) : 0;
        }

        int getReadHoldCount() {
            if (getReadLockCount() == 0) {
                return 0;
            }

            Thread* current = Thread::currentThread();
            if (firstReader == current) {
                return firstReaderHoldCount;
            }

            int count = readHolds.get().count;
            if (count == 0) {
                readHolds.remove();
            }
            return count;
        }

        int getCount() {
            return getState();
        }

    protected:

        /**
         * @returns true if the current thread, when trying to acquire the read lock,
         *          and otherwise eligible to do so, should block because of policy for
         *          overtaking other waiting threads.
         */
        virtual bool readerShouldBlock() const = 0;

        /**
         * @returns true if the current thread, when trying to acquire the write lock,
         *          and otherwise eligible to do so, should block because of policy for
         *          overtaking other waiting threads.
         */
        virtual bool writerShouldBlock() const = 0;

    };

    const int Sync::SHARED_SHIFT   = 16;
    const int Sync::SHARED_UNIT    = (1 << SHARED_SHIFT);
    const int Sync::MAX_COUNT      = (1 << SHARED_SHIFT) - 1;
    const int Sync::EXCLUSIVE_MASK = (1 << SHARED_SHIFT) - 1;

    class FairSync : public Sync {
    public:

        virtual ~FairSync() {}

        virtual bool readerShouldBlock() const {
            return this->hasQueuedPredecessors();
        }

        virtual bool writerShouldBlock() const {
            return this->hasQueuedPredecessors();
        }

        virtual bool isFair() const {
            return true;
        }
    };

    class NonFairSync : public Sync {
    public:

        virtual ~NonFairSync() {}

        virtual bool readerShouldBlock() const {
            return false;
        }

        virtual bool writerShouldBlock() const {
            return false;
            // TODO - add apparentlyFirstQueuedIsExclusive
        }

        virtual bool isFair() const {
            return false;
        }
    };

    class ReadLock : public decaf::util::concurrent::locks::Lock {
    private:

        ReadLock(const ReadLock&);
        ReadLock& operator= (const ReadLock&);

    public:

        Sync* sync;

        ReadLock(Sync* sync) : Lock(), sync(sync) {}

        virtual ~ReadLock() {}

        /**
         * Acquires the read lock.
         *
         * <p>Acquires the read lock if the write lock is not held by
         * another thread and returns immediately.
         *
         * <p>If the write lock is held by another thread then
         * the current thread becomes disabled for thread scheduling
         * purposes and lies dormant until the read lock has been acquired.
         */
        virtual void lock() {
            sync->acquireShared(1);
        }

        /**
         * Acquires the read lock unless the current thread is
         *
         * <p>Acquires the read lock if the write lock is not held
         * by another thread and returns immediately.
         *
         * <p>If the write lock is held by another thread then the
         * current thread becomes disabled for thread scheduling
         * purposes and lies dormant until one of two things happens:
         *
         * <ul>
         *   <li>The read lock is acquired by the current thread; or
         *   <li>Some other thread interrupts the current thread.
         * </ul>
         *
         * <p>If the current thread:
         *
         * <ul>
         *   <li>has its interrupted status set on entry to this method; or
         *   <li>is interrupted while acquiring the read lock,
         * </ul>
         *
         * then nterruptedException is thrown and the current thread's interrupted
         * status is cleared.
         *
         * <p>In this implementation, as this method is an explicit interruption
         * point, preference is given to responding to the interrupt over normal
         * or reentrant acquisition of the lock.
         *
         * @throws InterruptedException if the current thread is interrupted
         */
        void lockInterruptibly() {
            sync->acquireSharedInterruptibly(1);
        }

        /**
         * Acquires the read lock only if the write lock is not held by
         * another thread at the time of invocation.
         *
         * <p>Acquires the read lock if the write lock is not held by
         * another thread and returns immediately with the value
         * {@code true}. Even when this lock has been set to use a
         * fair ordering policy, a call to {@code tryLock()}
         * <em>will</em> immediately acquire the read lock if it is
         * available, whether or not other threads are currently
         * waiting for the read lock.  This &quot;barging&quot; behavior
         * can be useful in certain circumstances, even though it
         * breaks fairness. If you want to honor the fairness setting
         * for this lock, then use {@link #tryLock(long, TimeUnit)
         * tryLock(0, TimeUnit.SECONDS) } which is almost equivalent
         * (it also detects interruption).
         *
         * <p>If the write lock is held by another thread then
         * this method will return immediately with the value
         * {@code false}.
         *
         * @return {@code true} if the read lock was acquired
         */
        virtual bool tryLock() {
            return sync->tryReadLock();
        }

        /**
         * Acquires the read lock if the write lock is not held by
         * another thread within the given waiting time and the
         * current thread has not been interrupted.
         *
         * <p>Acquires the read lock if the write lock is not held by
         * another thread and returns immediately with the value
         * {@code true}. If this lock has been set to use a fair
         * ordering policy then an available lock <em>will not</em> be
         * acquired if any other threads are waiting for the
         * lock. This is in contrast to the {@link #tryLock()}
         * method. If you want a timed {@code tryLock} that does
         * permit barging on a fair lock then combine the timed and
         * un-timed forms together:
         *
         * <pre>if (lock.tryLock() || lock.tryLock(timeout, unit) ) { ... }
         * </pre>
         *
         * <p>If the write lock is held by another thread then the
         * current thread becomes disabled for thread scheduling
         * purposes and lies dormant until one of three things happens:
         *
         * <ul>
         *   <li>The read lock is acquired by the current thread; or
         *   <li>Some other thread interrupts the current thread; or
         *   <li>The specified waiting time elapses.
         * </ul>
         *
         * <p>If the read lock is acquired then the value {@code true} is returned.
         * <p>If the current thread:
         *
         * <ul>
         *   <li>has its interrupted status set on entry to this method; or
         *   <li>is interrupted while acquiring the read lock,
         * </ul> then InterruptedException is thrown and the current thread's
         * interrupted status is cleared.
         *
         * <p>If the specified waiting time elapses then the value
         * {@code false} is returned.  If the time is less than or
         * equal to zero, the method will not wait at all.
         *
         * <p>In this implementation, as this method is an explicit
         * interruption point, preference is given to responding to
         * the interrupt over normal or reentrant acquisition of the
         * lock, and over reporting the elapse of the waiting time.
         *
         * @param timeout the time to wait for the read lock
         * @param unit the time unit of the timeout argument
         *
         * @return {@code true} if the read lock was acquired
         *
         * @throws InterruptedException if the current thread is interrupted
         */
        virtual bool tryLock(long long timeout, const TimeUnit& unit) {
            return sync->tryAcquireSharedNanos(1, unit.toNanos(timeout));
        }

        /**
         * Attempts to release this lock.
         *
         * <p> If the number of readers is now zero then the lock
         * is made available for write lock attempts.
         */
        virtual void unlock() {
            sync->releaseShared(1);
        }

        /**
         * Throws UnsupportedOperationException because ReadLocks do not support conditions.
         * @throws UnsupportedOperationException always
         */
        virtual Condition* newCondition() {
            throw new UnsupportedOperationException();
        }

        /**
         * Returns a string identifying this lock, as well as its lock state.
         * The state, in brackets, includes the String {@code "Read locks ="}
         * followed by the number of held read locks.
         *
         * @return a string identifying this lock, as well as its lock state
         */
        virtual std::string toString() const {
            int r = sync->getReadLockCount();
            return std::string("ReadLock ") + "[Read locks = " + Integer::toString(r) + "]";
        }
    };

    class WriteLock : public decaf::util::concurrent::locks::Lock {
    private:

        WriteLock(const WriteLock&);
        WriteLock& operator= (const WriteLock&);

    public:

        Sync* sync;

        WriteLock(Sync* sync) : Lock(), sync(sync) {}

        virtual ~WriteLock() {}

        /**
         * Acquires the write lock.
         *
         * <p>Acquires the write lock if neither the read nor write lock
         * are held by another thread
         * and returns immediately, setting the write lock hold count to
         * one.
         *
         * <p>If the current thread already holds the write lock then the
         * hold count is incremented by one and the method returns
         * immediately.
         *
         * <p>If the lock is held by another thread then the current
         * thread becomes disabled for thread scheduling purposes and
         * lies dormant until the write lock has been acquired, at which
         * time the write lock hold count is set to one.
         */
        virtual void lock() {
            sync->acquire(1);
        }

        /**
         * Acquires the write lock unless the current thread is interrupted
         *
         * <p>Acquires the write lock if neither the read nor write lock are held
         * by another thread and returns immediately, setting the write lock hold
         * count to one.
         *
         * <p>If the current thread already holds this lock then the hold count
         * is incremented by one and the method returns immediately.
         *
         * <p>If the lock is held by another thread then the current thread
         * becomes disabled for thread scheduling purposes and lies dormant until
         * one of two things happens:
         *
         * <ul>
         *   <li>The write lock is acquired by the current thread; or
         *   <li>Some other thread interrupts the current thread.
         * </ul>
         *
         * <p>If the write lock is acquired by the current thread then the
         * lock hold count is set to one.
         *
         * <p>If the current thread:
         *
         * <ul>
         *   <li>has its interrupted status set on entry to this method; or
         *   <li>is interrupted while acquiring the write lock,
         * </ul>
         *
         * then InterruptedException is thrown and the current thread's
         * interrupted status is cleared.
         *
         * <p>In this implementation, as this method is an explicit interruption
         * point, preference is given to responding to the interrupt over normal or
         * reentrant acquisition of the lock.
         *
         * @throws InterruptedException if the current thread is interrupted
         */
        virtual void lockInterruptibly() {
            sync->acquireInterruptibly(1);
        }

        /**
         * Acquires the write lock only if it is not held by another thread
         * at the time of invocation.
         *
         * <p>Acquires the write lock if neither the read nor write lock are held
         * by another thread and returns immediately with the value {@code true},
         * setting the write lock hold count to one. Even when this lock has
         * been set to use a fair ordering policy, a call to tryLock() immediately
         * acquire the lock if it is available, whether or not other threads are
         * currently waiting for the write lock.  This &quot;barging&quot; behavior
         * can be useful in certain circumstances, even though it breaks fairness.
         * If you want to honor the fairness setting for this lock, then use
         * tryLock(0, TimeUnit.SECONDS) which is almost equivalent (it also
         * detects interruption).
         *
         * <p> If the current thread already holds this lock then the hold count is
         * incremented by one and the method returns true.
         *
         * <p>If the lock is held by another thread then this method will return
         * immediately with the value {@code false}.
         *
         * @return if the lock was free and was acquired by the current thread, or
         * the write lock was already held by the current thread; and false otherwise.
         */
        virtual bool tryLock() {
            return sync->tryWriteLock();
        }

        /**
         * Acquires the write lock if it is not held by another thread
         * within the given waiting time and the current thread has
         * not been interrupted.
         *
         * <p>Acquires the write lock if neither the read nor write lock
         * are held by another thread
         * and returns immediately with the value true,
         * setting the write lock hold count to one. If this lock has been
         * set to use a fair ordering policy then an available lock
         * <em>will not</em> be acquired if any other threads are
         * waiting for the write lock. This is in contrast to the
         * tryLock() method. If you want a timed tryLock
         * that does permit barging on a fair lock then combine the
         * timed and un-timed forms together:
         *
         * <pre>if (lock.tryLock() || lock.tryLock(timeout, unit) ) { ... }
         * </pre>
         *
         * <p>If the current thread already holds this lock then the
         * hold count is incremented by one and the method returns
         * true.
         *
         * <p>If the lock is held by another thread then the current
         * thread becomes disabled for thread scheduling purposes and
         * lies dormant until one of three things happens:
         *
         * <ul>
         *   <li>The write lock is acquired by the current thread; or
         *   <li>Some other thread interrupts the current thread; or
         *   <li>The specified waiting time elapses
         * </ul>
         *
         * <p>If the write lock is acquired then the value {@code true} is
         * returned and the write lock hold count is set to one.
         *
         * <p>If the current thread:
         *
         * <ul>
         *   <li>has its interrupted status set on entry to this method; or
         *   <li>is interrupted while acquiring the write lock,
         * </ul>
         *
         * then InterruptedException is thrown and the current
         * thread's interrupted status is cleared.
         *
         * <p>If the specified waiting time elapses then the value
         * false is returned.  If the time is less than or
         * equal to zero, the method will not wait at all.
         *
         * <p>In this implementation, as this method is an explicit
         * interruption point, preference is given to responding to
         * the interrupt over normal or reentrant acquisition of the
         * lock, and over reporting the elapse of the waiting time.
         *
         * @param timeout the time to wait for the write lock
         * @param unit the time unit of the timeout argument
         *
         * @return true if the lock was free and was acquired
         * by the current thread, or the write lock was already held by the
         * current thread; and false if the waiting time
         * elapsed before the lock could be acquired.
         *
         * @throws InterruptedException if the current thread is interrupted
         */
        virtual bool tryLock(long long timeout, const TimeUnit& unit) {
            return sync->tryAcquireNanos(1, unit.toNanos(timeout));
        }

        /**
         * Attempts to release this lock.
         *
         * <p>If the current thread is the holder of this lock then the hold
         * count is decremented. If the hold count is now zero then the lock
         * is released.  If the current thread is not the holder of this lock
         * then IllegalMonitorStateException is thrown.
         *
         * @throws IllegalMonitorStateException if the current thread does not
         *         hold this lock.
         */
        virtual void unlock() {
            sync->release(1);
        }

        /**
         * Returns a Condition instance for use with this Lock instance.
         *
         * <p>The returned Condition instance supports the same usages as do the
         * Mutex methods  wait, notify, and notifyAll when used with the built-in
         * mutex lock.
         *
         * <ul>
         *   <li>If this write lock is not held when any Condition method is called
         *   then an IllegalMonitorStateException is thrown.  (Read locks are
         *   held independently of write locks, so are not checked or affected.
         *   However it is essentially always an error to invoke a condition waiting
         *   method when the current thread has also acquired read locks, since other
         *   threads that could unblock it will not be able to acquire the write
         *   lock.)
         *   <li>When the condition waiting methods are called the write lock is
         *   released and, before they return, the write lock is reacquired and the
         *   lock hold count restored to what it was when the method was called.
         *   <li>If a thread is interrupted while waiting then the wait will terminate,
         *   an InterruptedException will be thrown, and the thread's interrupted
         *   status will be cleared.
         *   <li> Waiting threads are signalled in FIFO order.
         *   <li>The ordering of lock reacquisition for threads returning from
         *   waiting methods is the same as for threads initially acquiring the lock,
         *   which is in the default case not specified, but for <em>fair</em> locks
         *   favors those threads that have been waiting the longest.
         * </ul>
         *
         * @return the Condition object
         */
        virtual Condition* newCondition() {
            return sync->newCondition();
        }

        /**
         * Returns a string identifying this lock, as well as its lock state.  The
         * state, in brackets includes either the String "Unlocked" or the String
         * "Locked by" followed by the name of the owning thread.
         *
         * @return a string identifying this lock, as well as its lock state
         */
        virtual std::string toString() const {
            Thread* o = sync->getOwner();
            return std::string("WriteLock ") + ((o == NULL) ?
                "[Unlocked]" : "[Locked by thread " + o->getName() + "]");
        }

        /**
         * Queries if this write lock is held by the current thread.  Identical in
         * effect to  isWriteLockedByCurrentThread.
         *
         * @return true if the current thread holds this lock and false otherwise
         */
        virtual bool isHeldByCurrentThread() const {
            return sync->isHeldExclusively();
        }

        /**
         * Queries the number of holds on this write lock by the current thread.
         * A thread has a hold on a lock for each lock action that is not matched
         * by an unlock action.  Identical in effect to getWriteHoldCount.
         *
         * @return the number of holds on this lock by the current thread,
         *         or zero if this lock is not held by the current thread
         */
        virtual int getHoldCount() const {
            return sync->getWriteHoldCount();
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
namespace decaf {
namespace util {
namespace concurrent {
namespace locks {

    class ReentrantReadWriteLockImpl {
    private:

        ReentrantReadWriteLockImpl(const ReentrantReadWriteLockImpl&);
        ReentrantReadWriteLockImpl& operator= (const ReentrantReadWriteLockImpl&);

    public:

        decaf::util::concurrent::locks::Lock* readLock;
        decaf::util::concurrent::locks::Lock* writeLock;
        Sync* sync;

        ReentrantReadWriteLockImpl(bool fair) : readLock(NULL), writeLock(NULL), sync(NULL) {
            if (fair) {
                sync = new FairSync();
            } else {
                sync = new NonFairSync();
            }

            readLock = new ReadLock(sync);
            writeLock = new WriteLock(sync);
        }

        ~ReentrantReadWriteLockImpl() {
            delete readLock;
            delete writeLock;
            delete sync;
        }
    };

}}}}

////////////////////////////////////////////////////////////////////////////////
ReentrantReadWriteLock::ReentrantReadWriteLock() : ReadWriteLock(), impl(new ReentrantReadWriteLockImpl(false)) {
}

////////////////////////////////////////////////////////////////////////////////
ReentrantReadWriteLock::ReentrantReadWriteLock(bool fair) :
    ReadWriteLock(), impl(new ReentrantReadWriteLockImpl(fair)) {
}

////////////////////////////////////////////////////////////////////////////////
ReentrantReadWriteLock::~ReentrantReadWriteLock() {
    try {
        delete impl;
    }
    DECAF_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
decaf::util::concurrent::locks::Lock& ReentrantReadWriteLock::readLock() {
    return *(this->impl->readLock);
}

////////////////////////////////////////////////////////////////////////////////
decaf::util::concurrent::locks::Lock& ReentrantReadWriteLock::writeLock() {
    return *(this->impl->writeLock);
}

////////////////////////////////////////////////////////////////////////////////
bool ReentrantReadWriteLock::isFair() const {
    return this->impl->sync->isFair();
}

////////////////////////////////////////////////////////////////////////////////
Thread* ReentrantReadWriteLock::getOwner() const {
    return this->impl->sync->getOwner();
}

////////////////////////////////////////////////////////////////////////////////
int ReentrantReadWriteLock::getReadLockCount() const {
    return this->impl->sync->getReadLockCount();
}

////////////////////////////////////////////////////////////////////////////////
bool ReentrantReadWriteLock::isWriteLocked() const {
    return this->impl->sync->isWriteLocked();
}

////////////////////////////////////////////////////////////////////////////////
bool ReentrantReadWriteLock::isWriteLockedByCurrentThread() const {
    return this->impl->sync->isHeldExclusively();
}

////////////////////////////////////////////////////////////////////////////////
int ReentrantReadWriteLock::getWriteHoldCount() const {
    return this->impl->sync->getWriteHoldCount();
}

////////////////////////////////////////////////////////////////////////////////
int ReentrantReadWriteLock::getReadHoldCount() const {
    return this->impl->sync->getReadHoldCount();
}

////////////////////////////////////////////////////////////////////////////////
Collection<Thread*>* ReentrantReadWriteLock::getQueuedWriterThreads() const {
    return this->impl->sync->getExclusiveQueuedThreads();
}

////////////////////////////////////////////////////////////////////////////////
Collection<Thread*>* ReentrantReadWriteLock::getQueuedReaderThreads() const {
    return this->impl->sync->getSharedQueuedThreads();
}

////////////////////////////////////////////////////////////////////////////////
bool ReentrantReadWriteLock::hasQueuedThreads() const {
    return this->impl->sync->hasQueuedThreads();
}

////////////////////////////////////////////////////////////////////////////////
bool ReentrantReadWriteLock::hasQueuedThread(Thread* thread) const {
    return this->impl->sync->isQueued(thread);
}

////////////////////////////////////////////////////////////////////////////////
int ReentrantReadWriteLock::getQueueLength() const {
    return this->impl->sync->getQueueLength();
}

////////////////////////////////////////////////////////////////////////////////
Collection<Thread*>* ReentrantReadWriteLock::getQueuedThreads() const {
    return this->impl->sync->getQueuedThreads();
}

////////////////////////////////////////////////////////////////////////////////
bool ReentrantReadWriteLock::hasWaiters(Condition* condition) const {

    if (condition == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "The Condition to check was NULL");
    }

    const AbstractQueuedSynchronizer::ConditionObject* cond =
        dynamic_cast<const AbstractQueuedSynchronizer::ConditionObject*>(condition);

    if (cond == NULL) {
        throw IllegalArgumentException(__FILE__, __LINE__, "Condition is not associated with this Lock");
    }

    return this->impl->sync->hasWaiters(cond);
}

////////////////////////////////////////////////////////////////////////////////
int ReentrantReadWriteLock::getWaitQueueLength(Condition* condition) const {

    if (condition == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "The Condition to check was NULL");
    }

    const AbstractQueuedSynchronizer::ConditionObject* cond =
        dynamic_cast<const AbstractQueuedSynchronizer::ConditionObject*>(condition);

    if (cond == NULL) {
        throw IllegalArgumentException(__FILE__, __LINE__, "Condition is not associated with this Lock");
    }

    return this->impl->sync->getWaitQueueLength(cond);
}

////////////////////////////////////////////////////////////////////////////////
Collection<decaf::lang::Thread*>* ReentrantReadWriteLock::getWaitingThreads(Condition* condition) const {

    if (condition == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "The Condition to check was NULL");
    }

    const AbstractQueuedSynchronizer::ConditionObject* cond =
        dynamic_cast<const AbstractQueuedSynchronizer::ConditionObject*>(condition);

    if (cond == NULL) {
        throw IllegalArgumentException(__FILE__, __LINE__, "Condition is not associated with this Lock");
    }

    return this->impl->sync->getWaitingThreads(cond);
}

////////////////////////////////////////////////////////////////////////////////
std::string ReentrantReadWriteLock::toString() const {
    int c = this->impl->sync->getCount();
    int w = this->impl->sync->exclusiveCount(c);
    int r = this->impl->sync->sharedCount(c);

    return std::string("ReentrantReadWriteLock: ") +
            "[Write locks = " + Integer::toString(w) +
            ", Read locks = " + Integer::toString(r) + "]";
}
