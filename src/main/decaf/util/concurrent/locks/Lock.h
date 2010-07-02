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

#ifndef _DECAF_UTIL_CONCURRENT_LOCKS_LOCK_H_
#define _DECAF_UTIL_CONCURRENT_LOCKS_LOCK_H_

#include <decaf/util/Config.h>

#include <decaf/lang/exceptions/InterruptedException.h>
#include <decaf/lang/exceptions/UnsupportedOperationException.h>
#include <decaf/util/concurrent/locks/Condition.h>

namespace decaf {
namespace util {
namespace concurrent {
namespace locks {

    /**
     * Lock implementations provide more extensive locking operations than can be
     * obtained using synchronized statements. They allow more flexible structuring,
     * may have quite different properties, and may support multiple associated
     * Condition objects.
     *
     * A lock is a tool for controlling access to a shared resource by multiple
     * threads. Commonly, a lock provides exclusive access to a shared resource:
     * only one thread at a time can acquire the lock and all access to the shared
     * resource requires that the lock be acquired first. However, some locks may
     * allow concurrent access to a shared resource, such as the read lock of a
     * ReadWriteLock.
     *
     * While the scoping mechanism for synchronized statements makes it much easier
     * easier to program with monitor locks, and helps avoid many common programming
     * errors involving locks, there are occasions where you need to work with locks
     * in a more flexible way. For example, some algorithms for traversing concurrently
     * accessed data structures require the use of "hand-over-hand" or
     * "chain locking": you acquire the lock of node A, then node B, then release A
     * and acquire C, then release B and acquire D and so on. Implementations of the
     * Lock interface enable the use of such techniques by allowing a lock to be
     * acquired and released in different scopes, and allowing multiple locks to be
     * acquired and released in any order.
     *
     * With this increased flexibility comes additional responsibility. The absence of
     * block-structured locking removes the automatic release of locks that occurs with
     * synchronized statements. In most cases, the following idiom should be used:
     *
     *    Lock l = ...;
     *    l.lock();
     *    try {
     *        // access the resource protected by this lock
     *    } catch(...) {
     *        l.unlock();
     *    }
     *
     * When locking and unlocking occur in different scopes, care must be taken to ensure
     * that all code that is executed while the lock is held is protected by try-catch
     * ensure that the lock is released when necessary.
     *
     * Lock implementations provide additional functionality over the use of synchronized
     * methods and statements by providing a non-blocking attempt to acquire a lock
     * (tryLock()), an attempt to acquire the lock that can be interrupted (lockInterruptibly(),
     * and an attempt to acquire the lock that can timeout (tryLock(long, TimeUnit)).
     *
     * Note that Lock instances are just normal objects and can themselves be used as the
     * target in a synchronized statement.
     *
     * The three forms of lock acquisition (interruptible, non-interruptible, and timed)
     * may differ in their performance characteristics, ordering guarantees, or other
     * implementation qualities. Further, the ability to interrupt the ongoing acquisition
     * of a lock may not be available in a given Lock class. Consequently, an implementation
     * is not required to define exactly the same guarantees or semantics for all three forms
     * of lock acquisition, nor is it required to support interruption of an ongoing lock
     * acquisition. An implementation is required to clearly document the semantics and
     * guarantees provided by each of the locking methods. It must also obey the interruption
     * semantics as defined in this interface, to the extent that interruption of lock
     * acquisition is supported: which is either totally, or only on method entry.
     *
     * As interruption generally implies cancellation, and checks for interruption are
     * often infrequent, an implementation can favor responding to an interrupt over normal
     * method return. This is true even if it can be shown that the interrupt occurred after
     * another action may have unblocked the thread. An implementation should document this
     * behavior.
     *
     * @since 1.0
     */
    class DECAF_API Lock {
    public:

        virtual ~Lock() {}

        /**
         * Acquires the lock.
         *
         * If the lock is not available then the current thread becomes disabled for thread
         * scheduling purposes and lies dormant until the lock has been acquired.
         *
         * Implementation Considerations
         *
         * A Lock implementation may be able to detect erroneous use of the lock, such as an
         * invocation that would cause deadlock, and may throw an exception in such circumstances.
         * The circumstances and the exception type must be documented by that Lock implementation.
         *
         * @throws RuntimeException if an error occurs while acquiring the lock.
         */
        virtual void lock() = 0;

        /**
         * Acquires the lock unless the current thread is interrupted.
         * <p>
         * Acquires the lock if it is available and returns immediately.
         * <p>
         * If the lock is not available then the current thread becomes disabled for thread
         * scheduling purposes and lies dormant until one of two things happens:
         *
         *  * The lock is acquired by the current thread; or
         *  * Some other thread interrupts the current thread, and interruption of lock
         *    acquisition is supported.
         *
         * If the current thread:
         *
         *  * has its interrupted status set on entry to this method; or
         *  * is interrupted while acquiring the lock, and interruption of lock acquisition
         *    is supported,
         *
         * then InterruptedException is thrown and the current thread's interrupted status
         * is cleared.
         *
         * Implementation Considerations
         *
         * The ability to interrupt a lock acquisition in some implementations may not be
         * possible, and if possible may be an expensive operation. The programmer should
         * be aware that this may be the case. An implementation should document when this
         * is the case.
         *
         * An implementation can favor responding to an interrupt over normal method return.
         *
         * A Lock implementation may be able to detect erroneous use of the lock, such as an
         * invocation that would cause deadlock, and may throw an exception in such
         * circumstances.  The circumstances and the exception type must be documented by
         * that Lock implementation.
         *
         * @throws RuntimeException if an error occurs while acquiring the lock.
         * @throws InterruptedException
         *         if the current thread is interrupted while acquiring the lock (and
         *         interruption of lock acquisition is supported).
         */
        virtual void lockInterruptibly() = 0;

        /**
         * Acquires the lock only if it is free at the time of invocation.
         * <p>
         * Acquires the lock if it is available and returns immediately with the value true.
         * If the lock is not available then this method will return immediately with the
         * value false.
         * <p>
         * A typical usage idiom for this method would be:
         *
         *   Lock lock = ...;
         *   if (lock.tryLock()) {
         *       try {
         *            // manipulate protected state
         *       } catch(...) {
         *            lock.unlock();
         *       }
         *   } else {
         *       // perform alternative actions
         *   }
         *
         * This usage ensures that the lock is unlocked if it was acquired, and doesn't
         * try to unlock if the lock was not acquired.
         *
         * @returns true if the lock was acquired and false otherwise
         *
         * @throws RuntimeException if an error occurs while acquiring the lock.
         */
        virtual bool tryLock() = 0;

        /**
         * Acquires the lock if it is free within the given waiting time and the current
         * thread has not been interrupted.
         * <p>
         * If the lock is available this method returns immediately with the value true.
         * If the lock is not available then the current thread becomes disabled for thread
         * scheduling purposes and lies dormant until one of three things happens:
         *
         *   * The lock is acquired by the current thread; or
         *   * Some other thread interrupts the current thread, and interruption of lock
         *     acquisition is supported; or
         *   * The specified waiting time elapses
         *
         * If the lock is acquired then the value true is returned.
         *
         * If the current thread:
         *
         *   * has its interrupted status set on entry to this method; or
         *   * is interrupted while acquiring the lock, and interruption of lock acquisition
         *     is supported,
         *
         * then InterruptedException is thrown and the current thread's interrupted status
         * is cleared.
         *
         * If the specified waiting time elapses then the value false is returned. If the
         * time is less than or equal to zero, the method will not wait at all.
         *
         * Implementation Considerations
         *
         * The ability to interrupt a lock acquisition in some implementations may not
         * be possible, and if possible may be an expensive operation. The programmer should
         * be aware that this may be the case. An implementation should document when this
         * is the case.
         *
         * An implementation can favor responding to an interrupt over normal method return,
         * or reporting a timeout.
         *
         * A Lock implementation may be able to detect erroneous use of the lock, such as an
         * invocation that would cause deadlock, and may throw an (unchecked) exception in
         * such circumstances. The circumstances and the exception type must be documented by
         * that Lock implementation.
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
        virtual bool tryLock( long long time, const TimeUnit& unit ) = 0;

        /**
         * Releases the lock.
         * <p>
         * Implementation Considerations
         *
         * A Lock implementation will usually impose restrictions on which thread can release
         * a lock (typically only the holder of the lock can release it) and may throw an
         * exception if the restriction is violated. Any restrictions and the exception
         * type must be documented by that Lock implementation.
         *
         * @throws RuntimeException if an error occurs while acquiring the lock.
         * @throws IllegalMonitorStateException if the current thread is not the owner of the lock.
         */
        virtual void unlock() = 0;

        /**
         * Returns a new Condition instance that is bound to this Lock instance.
         * <p>
         * Before waiting on the condition the lock must be held by the current thread.
         * A call to Condition.await() will atomically release the lock before waiting
         * and re-acquire the lock before the wait returns.
         * <p>
         * Implementation Considerations
         * <p>
         * The exact operation of the Condition instance depends on the Lock implementation
         * and must be documented by that implementation.
         *
         * @returns A new Condition instance for this Lock instance the caller must
         *          delete the returned Condition object when done with it.
         *
         * @throws RuntimeException if an error occurs while creating the Condition.
         * @throws UnsupportedOperationException
         *         if this Lock implementation does not support conditions
         */
        virtual Condition* newCondition() = 0;

    };

}}}}

#endif /*_DECAF_UTIL_CONCURRENT_LOCKS_LOCK_H_ */
