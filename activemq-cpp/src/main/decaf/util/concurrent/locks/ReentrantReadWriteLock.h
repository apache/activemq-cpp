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

#ifndef _DECAF_UTIL_CONCURRENT_LOCKS_REENTRANTREADWRITELOCK_H_
#define _DECAF_UTIL_CONCURRENT_LOCKS_REENTRANTREADWRITELOCK_H_

#include <decaf/util/Config.h>

#include <decaf/lang/Thread.h>
#include <decaf/util/Collection.h>
#include <decaf/util/concurrent/locks/ReadWriteLock.h>

namespace decaf {
namespace util {
namespace concurrent {
namespace locks {

    class ReentrantReadWriteLockImpl;

    /**
     *
     * @since 1.0
     */
    class DECAF_API ReentrantReadWriteLock : public ReadWriteLock {
    private:

        ReentrantReadWriteLockImpl* impl;

    private:

        ReentrantReadWriteLock(const ReentrantReadWriteLock&);
        ReentrantReadWriteLock& operator=(const ReentrantReadWriteLock&);

    public:

        /**
         * Creates a new ReentrantReadWriteLock with the default ordering property of Not-Fair.
         */
        ReentrantReadWriteLock();

        /**
         * Creates a new ReentrantReadWriteLock with the given fairness policy.
         *
         * @param fair
         *      Boolean value indicating whether this lock uses a fair or non-fair policy.
         */
        ReentrantReadWriteLock(bool fair);

        virtual ~ReentrantReadWriteLock();

    public:

        /**
         * {@inheritDoc}
         */
        virtual decaf::util::concurrent::locks::Lock& readLock();

        /**
         * {@inheritDoc}
         */
        virtual decaf::util::concurrent::locks::Lock& writeLock();

        /**
         * Returns true if this lock has fairness set true.
         *
         * @returns true if the Lock uses a fair policy otherwise false.
         */
        bool isFair() const;

        /**
         * Queries the number of read locks held for this lock. This method is designed
         * for use in monitoring system state, not for synchronization control.
         *
         * @return the number of read locks held.
         */
        int getReadLockCount() const;

        /**
         * Queries if the write lock is held by any thread. This method is designed for
         * use in monitoring system state, not for synchronization control.
         *
         * @return true if any thread holds the write lock and false otherwise
         */
        bool isWriteLocked() const;

        /**
         * Queries if the write lock is held by the current thread.
         *
         * @return true if the current thread holds the write lock and false otherwise
         */
        bool isWriteLockedByCurrentThread() const;

        /**
         * Queries the number of reentrant write holds on this lock by the current thread.
         * A writer thread has a hold on a lock for each lock action that is not matched
         * by an unlock action.
         *
         * @return the number of holds on the write lock by the current thread,
         *         or zero if the write lock is not held by the current thread
         */
        int getWriteHoldCount() const;

        /**
         * Queries the number of reentrant read holds on this lock by the current thread.
         * A reader thread has a hold on a lock for each lock action that is not matched
         * by an unlock action.
         *
         * @return the number of holds on the read lock by the current thread,
         *         or zero if the read lock is not held by the current thread
         */
        int getReadHoldCount() const;

        /**
         * Queries whether any threads are waiting on the given condition associated with
         * the write lock. Note that because timeouts and interrupts may occur at any time,
         * a true return does not guarantee that a future signal will awaken any threads.
         * This method is designed primarily for use in monitoring of the system state.
         *
         * @param condition
         *      The condition to be queried for waiters.
         *
         * @return true if there are any waiting threads
         *
         * @throws NullPointerException if the ConditionObject pointer is NULL.
         * @throws IllegalArgumentException if the ConditionObject is not associated with this Lock.
         * @throws IllegalMonitorStateException if the caller does not hold exclusive synchronization.
         */
        bool hasWaiters(Condition* condition) const;

        /**
         * Gets an estimated count of the number of threads that are currently waiting on the given
         * Condition object, this value changes dynamically so the result of this method can be invalid
         * immediately after it is called.  The Condition object must be associated with this Lock
         * or an exception will be thrown.
         *
         * @returns an estimate of the number of waiting threads.
         *
         * @throws NullPointerException if the ConditionObject pointer is NULL.
         * @throws IllegalArgumentException if the ConditionObject is not associated with this Synchronizer.
         * @throws IllegalMonitorStateException if the caller does not hold exclusive synchronization.
         */
        int getWaitQueueLength(Condition* condition) const;

        /**
         * Returns a string identifying this lock, as well as its lock state. The state,
         * in brackets, includes the String "Write locks =" followed by the number of
         * reentrantly held write locks, and the String "Read locks =" followed by the
         * number of held read locks.
         *
         * @return a string identifying this lock, as well as its lock state
         */
        std::string toString() const;

        /**
         * Queries whether any threads are waiting to acquire the read or write lock.
         * Note that because cancellations may occur at any time, a true return does
         * not guarantee that any other thread will ever acquire a lock.  This method
         * is designed primarily for use in monitoring of the system state.
         *
         * @return if there may be other threads waiting to acquire the lock
         */
        bool hasQueuedThreads() const;

        /**
         * Queries whether the given thread is waiting to acquire either the read or
         * write lock. Note that because cancellations may occur at any time, a true
         * return does not guarantee that this thread will ever acquire a lock.  This
         * method is designed primarily for use in monitoring of the system state.
         *
         * @param thread
         *      The thread that will be queried for.
         *
         * @return true if the given thread is queued waiting for this lock
         *
         * @throws NullPointerException if the thread is NULL.
         */
        bool hasQueuedThread(decaf::lang::Thread* thread) const;

        /**
         * Returns an estimate of the number of threads waiting to acquire either the
         * read or write lock.  The value is only an estimate because the number of
         * threads may change dynamically while this method traverses internal data
         * structures.  This method is designed for use in monitoring of the system
         * state, not for synchronization control.
         *
         * @return the estimated number of threads waiting for this lock
         */
        int getQueueLength() const;

    protected:

        /**
         * Creates and returns a new Collection object that contains all the threads that may be waiting
         * on the given Condition object instance at the time this method is called.
         *
         * @returns a Collection pointer that contains waiting threads on given Condition object.
         *          The caller owns the returned pointer.
         *
         * @throws NullPointerException if the ConditionObject pointer is NULL.
         * @throws IllegalArgumentException if the ConditionObject is not associated with this Synchronizer.
         * @throws IllegalMonitorStateException if the caller does not hold exclusive synchronization.
         */
        decaf::util::Collection<decaf::lang::Thread*>* getWaitingThreads(Condition* condition) const;

        /**
         * Returns a collection containing threads that may be waiting to acquire either
         * the read or write lock.  Because the actual set of threads may change dynamically
         * while constructing this result, the returned collection is only a best-effort
         * estimate.  The elements of the returned collection are in no particular order.
         * This method is designed to facilitate construction of subclasses that provide
         * more extensive monitoring facilities.
         *
         * @return the collection of threads
         */
        decaf::util::Collection<decaf::lang::Thread*>* getQueuedThreads() const;

        /**
         * Returns a collection containing threads that may be waiting to acquire the
         * write lock.  Because the actual set of threads may change dynamically
         * while constructing this result, the returned collection is only a best-effort
         * estimate.  The elements of the returned collection are in no particular order.
         * This method is designed to facilitate construction of subclasses that provide
         * more extensive lock monitoring facilities.
         *
         * @return the collection of threads
         */
        decaf::util::Collection<decaf::lang::Thread*>* getQueuedWriterThreads() const;

        /**
         * Returns a collection containing threads that may be waiting to acquire the
         * read lock.  Because the actual set of threads may change dynamically while
         * constructing this result, the returned collection is only a best-effort estimate.
         * The elements of the returned collection are in no particular order.  This method
         * is designed to facilitate construction of subclasses that provide more extensive
         * lock monitoring facilities.
         *
         * @return the collection of threads
         */
        decaf::util::Collection<decaf::lang::Thread*>* getQueuedReaderThreads() const;

        /**
         * Returns the thread that currently owns the write lock, or NULL if not
         * owned. When this method is called by a thread that is not the owner, the
         * return value reflects a best-effort approximation of current lock status.
         * For example, the owner may be momentarily NULL even if there are threads
         * trying to acquire the lock but have not yet done so.  This method is
         * designed to facilitate construction of subclasses that provide more
         * extensive lock monitoring facilities.
         *
         * @return the owner thread pointer, or NULL if not currently owned.
         */
        decaf::lang::Thread* getOwner() const;

    };

}}}}

#endif /* _DECAF_UTIL_CONCURRENT_LOCKS_REENTRANTREADWRITELOCK_H_ */
