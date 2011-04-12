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

#ifndef _DECAF_UTIL_CONCURRENT_LOCKS_REENTRANTLOCK_H_
#define _DECAF_UTIL_CONCURRENT_LOCKS_REENTRANTLOCK_H_

#include <decaf/util/Config.h>

#include <decaf/util/concurrent/locks/Lock.h>

namespace decaf {
namespace util {
namespace concurrent {
namespace locks {

    class LockHandle;

    /**
     * A reentrant mutual exclusion Lock with extended capabilities.
     *
     * A ReentrantLock is owned by the thread last successfully locking, but not yet unlocking it.
     * A thread invoking lock will return, successfully acquiring the lock, when the lock is not
     * owned by another thread. The method will return immediately if the current thread already
     * owns the lock. This can be checked using methods isHeldByCurrentThread(), and getHoldCount().
     *
     * The constructor for this class accepts an optional fairness parameter. When set true, under
     * contention, locks favor granting access to the longest-waiting thread. Otherwise this lock
     * does not guarantee any particular access order. Programs using fair locks accessed by many
     * threads may display lower overall throughput (i.e., are slower; often much slower) than
     * those using the default setting, but have smaller variances in times to obtain locks and
     * guarantee lack of starvation. Note however, that fairness of locks does not guarantee
     * fairness of thread scheduling. Thus, one of many threads using a fair lock may obtain it
     * multiple times in succession while other active threads are not progressing and not currently
     * holding the lock. Also note that the untimed tryLock method does not honor the fairness
     * setting. It will succeed if the lock is available even if other threads are waiting.
     *
     * It is recommended practice to always immediately follow a call to lock with a try block,
     * most typically in a before/after construction such as:
     *
     *    class X {
     *    private:
     *
     *        ReentrantLock lock;
     *        // ...
     *
     *    public:
     *
     *        void m() {
     *            lock.lock();  // block until condition holds
     *
     *            try {
     *                // ... method body
     *            } finally {
     *                lock.unlock()
     *            }
     *        }
     *    }
     *
     * In addition to implementing the Lock interface, this class defines methods isLocked and
     * getLockQueueLength, as well as some associated protected access methods that may be useful
     * for instrumentation and monitoring.
     *
     * @since 1.0
     */
    class DECAF_API ReentrantLock : public Lock {
    private:

        LockHandle* handle;

    private:

        ReentrantLock( const ReentrantLock& );
        ReentrantLock& operator= ( const ReentrantLock& );

    public:

        ReentrantLock();

        virtual ~ReentrantLock();

        /**
         * Acquires the lock.
         *
         * Acquires the lock if it is not held by another thread and returns immediately, setting
         * the lock hold count to one.
         *
         * If the current thread already holds the lock then the hold count is incremented by one
         * and the method returns immediately.
         *
         * If the lock is held by another thread then the current thread becomes disabled for thread
         * scheduling purposes and lies dormant until the lock has been acquired, at which time the
         * lock hold count is set to one.
         *
         * @throws RuntimeException if an error occurs while acquiring the lock.
         */
        virtual void lock();

        /**
         * Acquires the lock unless the current thread is interrupted.
         *
         * Acquires the lock if it is not held by another thread and returns immediately, setting
         * the lock hold count to one.
         *
         * If the current thread already holds this lock then the hold count is incremented by one and
         * the method returns immediately.
         *
         * If the lock is held by another thread then the current thread becomes disabled for thread
         * scheduling purposes and lies dormant until one of two things happens:
         *
         *  * The lock is acquired by the current thread; or
         *  * Some other thread interrupts the current thread.
         *
         * If the lock is acquired by the current thread then the lock hold count is set to one.
         *
         * If the current thread:
         *
         *     * has its interrupted status set on entry to this method; or
         *     * is interrupted while acquiring the lock,
         *
         * then InterruptedException is thrown and the current thread's interrupted status is cleared.
         *
         * In this implementation, as this method is an explicit interruption point, preference is given
         * to responding to the interrupt over normal or reentrant acquisition of the lock.
         *
         * @throws RuntimeException if an error occurs while acquiring the lock.
         * @throws InterruptedException
         *         if the current thread is interrupted while acquiring the lock (and
         *         interruption of lock acquisition is supported).
         */
        virtual void lockInterruptibly();

        /**
         * Acquires the lock only if it is not held by another thread at the time of invocation.
         *
         * Acquires the lock if it is not held by another thread and returns immediately with the
         * value true, setting the lock hold count to one. Even when this lock has been set to use
         * a fair ordering policy, a call to tryLock() will immediately acquire the lock if it is
         * available, whether or not other threads are currently waiting for the lock. This
         * "barging" behavior can be useful in certain circumstances, even though it breaks fairness.
         * If you want to honor the fairness setting for this lock, then use tryLock(0, TimeUnit.SECONDS)
         * which is almost equivalent (it also detects interruption).
         *
         * If the current thread already holds this lock then the hold count is incremented by one
         * and the method returns true.
         *
         * If the lock is held by another thread then this method will return immediately with the
         * value false.
         *
         * @returns true if the lock was acquired and false otherwise
         *
         * @throws RuntimeException if an error occurs while acquiring the lock.
         */
        virtual bool tryLock();

        /**
         * Acquires the lock if it is not held by another thread within the given waiting time and the
         * current thread has not been interrupted.
         *
         * Acquires the lock if it is not held by another thread and returns immediately with the value
         * true, setting the lock hold count to one. If this lock has been set to use a fair ordering
         * policy then an available lock will not be acquired if any other threads are waiting for the
         * lock. This is in contrast to the tryLock() method. If you want a timed tryLock that does
         * permit barging on a fair lock then combine the timed and un-timed forms together:
         *
         *    if (lock.tryLock() || lock.tryLock(timeout, unit) ) { ... }
         *
         * If the current thread already holds this lock then the hold count is incremented by one and
         * the method returns true.
         *
         * If the lock is held by another thread then the current thread becomes disabled for thread
         * scheduling purposes and lies dormant until one of three things happens:
         *
         *     * The lock is acquired by the current thread; or
         *     * Some other thread interrupts the current thread; or
         *     * The specified waiting time elapses
         *
         * If the lock is acquired then the value true is returned and the lock hold count is set to one.
         *
         * If the current thread:
         *
         *     * has its interrupted status set on entry to this method; or
         *     * is interrupted while acquiring the lock,
         *
         * then InterruptedException is thrown and the current thread's interrupted status is cleared.
         *
         * If the specified waiting time elapses then the value false is returned. If the time is less
         * than or equal to zero, the method will not wait at all.
         *
         * In this implementation, as this method is an explicit interruption point, preference is given
         * to responding to the interrupt over normal or reentrant acquisition of the lock, and over
         * reporting the elapse of the waiting time.
         *
         * @param time
         *        the maximum time to wait for the lock
         * @param unit
         *        the time unit of the time argument
         *
         * @returns true if the lock was acquired and false if the waiting time elapsed
         *          before the lock was acquired
         *
         * @throws RuntimeException if an error occurs while acquiring the lock.
         * @throws InterruptedException
         *         if the current thread is interrupted while acquiring the lock (and
         *         interruption of lock acquisition is supported)
         */
        virtual bool tryLock( long long time, const TimeUnit& unit );

        /**
         * Attempts to release this lock.
         *
         * If the current thread is the holder of this lock then the hold count is decremented.
         * If the hold count is now zero then the lock is released. If the current thread is not
         * the holder of this lock then IllegalMonitorStateException is thrown.
         *
         * @throws RuntimeException if an error occurs while acquiring the lock.
         */
        virtual void unlock();

        /**
         * Returns a Condition instance for use with this Lock instance.
         *
         * The returned Condition instance supports the same usages as do the Mutex Class' methods
         * (wait, notify, and notifyAll).
         *
         *   * If this lock is not held when any of the Condition waiting or signalling methods are
         *     called, then an IllegalMonitorStateException is thrown.
         *   * When the condition waiting methods are called the lock is released and, before they
         *     return, the lock is reacquired and the lock hold count restored to what it was when
         *     the method was called.
         *   * If a thread is interrupted while waiting then the wait will terminate, an
         *     InterruptedException will be thrown, and the thread's interrupted status will be cleared.
         *   * Waiting threads are signaled in FIFO order.
         *   * The ordering of lock reacquisition for threads returning from waiting methods is the
         *     same as for threads initially acquiring the lock, which is in the default case not
         *     specified, but for fair locks favors those threads that have been waiting the longest.
         *
         * @throws RuntimeException if an error occurs while creating the Condition.
         * @throws UnsupportedOperationException
         *         if this Lock implementation does not support conditions
         */
        virtual Condition* newCondition();

        /**
         * Queries the number of holds on this lock by the current thread.
         *
         * A thread has a hold on a lock for each lock action that is not matched by an unlock action.
         *
         * The hold count information is typically only used for testing and debugging purposes. For
         * example, if a certain section of code should not be entered with the lock already held
         * then we can assert that fact:
         *
         *    class X {
         *    private:
         *
         *        ReentrantLock lock;
         *        // ...
         *
         *    public:
         *
         *        void m() {
         *            assert( lock.getHoldCount() == 0 );
         *            lock.lock();
         *            try {
         *                // ... method body
         *            } catch(...) {
         *               lock.unlock();
         *           }
         *        }
         *    }
         *
         * @returns the number of holds on this lock by the current thread, or zero if this lock
         *          is not held by the current thread
         */
        int getHoldCount() const;

        /**
         * Queries if this lock is held by the current thread.
         *
         * This method is typically used for debugging and testing. For example, a method that
         * should only be called while a lock is held can assert that this is the case:
         *
         *  class X {
         *  private:
         *      ReentrantLock lock;
         *      // ...
         *
         *  public:
         *      void m() {
         *          assert( lock.isHeldByCurrentThread() );
         *          // ... method body
         *      }
         *  }
         *
         * It can also be used to ensure that a reentrant lock is used in a non-reentrant
         * manner, for example:
         *
         *   class X {
         *   private:
         *      ReentrantLock lock;
         *      // ...
         *
         *   public:
         *      void m() {
         *         assert !lock.isHeldByCurrentThread();
         *         lock.lock();
         *         try {
         *             // ... method body
         *         } finally {
         *             lock.unlock();
         *         }
         *     }
         *   }
         *
         * @returns true if current thread holds this lock and false otherwise
         */
        bool isHeldByCurrentThread() const;

        /**
         * Queries if this lock is held by any thread. This method is designed for use in
         * monitoring of the system state, not for synchronization control.
         *
         * @returns true if any thread holds this lock and false otherwise
         */
        bool isLocked() const;

        /**
         * Returns true if this lock has fairness set true.
         *
         * @returns true if this lock has fairness set true
         */
        bool isFair() const;

        /**
         * Returns a string identifying this lock, as well as its lock state. The state, in
         * brackets, includes either the String "Unlocked" or the String "Locked by" followed
         * by the name of the owning thread.
         *
         * @returns a string identifying this lock, as well as its lock state
         */
        std::string toString() const;

    };

}}}}

#endif /* _DECAF_UTIL_CONCURRENT_LOCKS_REENTRANTLOCK_H_ */
