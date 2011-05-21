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

#ifndef _DECAF_UTIL_CONCURRENT_LOCKS_ABSTRACTQUEUEDSYNCHRONIZER_H_
#define _DECAF_UTIL_CONCURRENT_LOCKS_ABSTRACTQUEUEDSYNCHRONIZER_H_

#include <decaf/util/Config.h>

#include <decaf/util/concurrent/locks/AbstractOwnableSynchronizer.h>
#include <decaf/util/concurrent/locks/Condition.h>
#include <decaf/util/Collection.h>
#include <decaf/lang/Thread.h>

namespace decaf {
namespace util {
namespace concurrent {
namespace locks {

    class SynchronizerState;

    class DECAF_API AbstractQueuedSynchronizer : public AbstractOwnableSynchronizer {
    private:

        SynchronizerState* impl;

    private:

        AbstractQueuedSynchronizer(const AbstractQueuedSynchronizer&);
        AbstractQueuedSynchronizer& operator= (const AbstractQueuedSynchronizer&);

    protected:

        AbstractQueuedSynchronizer();

    public:

        /**
         * Condition object for this Synchronizer, which serves as the basis for other
         * Lock objects.
         */
        class ConditionObject : public Condition {
        private:

            friend class AbstractQueuedSynchronizer;

        private:

            ConditionObject(const ConditionObject&);
            ConditionObject& operator= (const ConditionObject&);

        public:

            ConditionObject() : Condition() {}
            virtual ~ConditionObject() {}

        protected:

            /**
             * Used to check on the ownership status of this ConditionObject
             *
             * @returns true if the ConditionObject is owned by the given AbstractQueuedSynchronizer
             */
            virtual bool isOwnedBy(const AbstractQueuedSynchronizer* sync) const = 0;

            /**
             * Returns true if there are any waiters on this Condition object at the time
             * of its calling.
             *
             * @returns true if there are currently waiters false otherwise.
             */
            virtual bool hasWaiters() const = 0;

            /**
             * Calculates and returns an estimate of the number of Threads that are waiting
             * on this Condition object.
             *
             * @return count of the estimated number of waiting threads.
             */
            virtual int getWaitQueueLength() const = 0;

            /**
             * Retrieves a new Collection object that contains Threads that may be waiting
             * on this Condition object.
             *
             * @returns new Collection object that holds possible waiters. Caller owns.
             */
            virtual Collection<decaf::lang::Thread*>* getWaitingThreads() const = 0;
        };

    public:

        virtual ~AbstractQueuedSynchronizer();

        /**
         * Acquire the lock exclusively, ignoring interrupts.  This method will call tryAcquire at least
         * once and return if that succeeds, otherwise it can block and possibly spin until the lock
         * is acquired.  This method can serve as the basis for a Lock.lock() implementation.
         *
         * @param arg
         *      Argument passed to tryAcquire, value is not interpreted by this class.
         */
        void acquire(int arg);

        /**
         * Acquire the lock in shared mode, ignoring interrupts.  This method will call tryAcquireShared
         * at least once and return if that succeeds, otherwise it can block and possibly spin until the
         * lock is acquired.
         *
         * @param arg
         *      Argument passed to tryAcquireShared, value is not interpreted by this class.
         */
        void acquireShared(int arg);

        /**
         * Acquire the lock exclusively, allowing for interrupts.  If the interrupt state is not already
         * set this method will call tryAcquire at least once and return if that succeeds, otherwise it
         * can block and possibly spin until the lock is acquired or the Thread is interrupted.  This
         * method can serve as the basis for a Lock.lockInterruptibly() implementation.
         *
         * @param arg
         *      Argument passed to tryAcquire, value is not interpreted by this class.
         *
         * @throws InterruptedException if the calling Thread is interrupted.
         */
        void acquireInterruptibly(int arg);

        /**
         * Acquire the lock in shared mode, allowing interruption.  If the interrupt state is not already
         * set this method will call tryAcquireShared at least once and return if that succeeds, otherwise
         * it can block and possibly spin until the lock is acquired or the Thread is interrupted.
         *
         * @param arg
         *      Argument passed to tryAcquireShared, value is not interpreted by this class.
         *
         * @throws InterruptedException if the calling Thread is interrupted.
         */
        void acquireSharedInterruptibly(int arg);

        /**
         * Creates and returns a new Collection object that contains only those threads that may
         * be waiting to acquire this Synchronization in exclusive mode.
         *
         * @returns a Collection pointer that contains waiting threads for exclusive acquisition.
         *          The caller owns the returned pointer.
         */
        Collection<decaf::lang::Thread*>* getExclusiveQueuedThreads() const;

        /**
         * Creates and returns a new Collection object that contains only those threads that may
         * be waiting to acquire this Synchronization in shared mode.
         *
         * @returns a Collection pointer that contains waiting threads for shared acquisition.
         *          The caller owns the returned pointer.
         */
        Collection<decaf::lang::Thread*>* getSharedQueuedThreads() const;

        /**
         * Returns the first thread queue (the thread that's been waiting the longest) if there
         * are currently no queued threads this method returns NULL.
         *
         * @returns the first thread in the queue or NULL if none are currently waiting.
         */
        decaf::lang::Thread* getFirstQueuedThread() const;

        /**
         * Creates and returns a new Collection object that contains a best effort snapshot of the
         * threads that are currently waiting to acquire.
         *
         * @returns a Collection pointer that contains waiting threads for lock acquisition.
         *          The caller owns the returned pointer.
         */
        Collection<decaf::lang::Thread*>* getQueuedThreads() const;

        /**
         * Gets an estimated count of the number of threads that are currently waiting to acquire, this
         * value changes dynamically so the result of this method can be invalid immediately after it
         * is called.
         *
         * @returns an estimate of the number of waiting threads.
         */
        int getQueueLength() const;

        /**
         * Creates and returns a new Collection object that contains all the threads that may be waiting
         * on the given ConditionOject instance at the time this method is called.
         *
         * @returns a Collection pointer that contains waiting threads on given ConditionObject.
         *          The caller owns the returned pointer.
         *
         * @throws NullPointerException if the ConditionObject pointer is NULL.
         * @throws IllegalArgumentException if the ConditionObject is not associated with this Synchronizer.
         * @throws IllegalMonitorStateException if the caller does not hold exclusive synchronization.
         */
        Collection<decaf::lang::Thread*>* getWaitingThreads(const AbstractQueuedSynchronizer::ConditionObject* condition) const;

        /**
         * Gets an estimated count of the number of threads that are currently waiting on the given
         * ConditionObject, this value changes dynamically so the result of this method can be invalid
         * immediately after it is called.  The ConditionObject must be associated with this
         * AbstractQueuedSynchronizer or an exception will be thrown.
         *
         * @returns an estimate of the number of waiting threads.
         *
         * @throws NullPointerException if the ConditionObject pointer is NULL.
         * @throws IllegalArgumentException if the ConditionObject is not associated with this Synchronizer.
         * @throws IllegalMonitorStateException if the caller does not hold exclusive synchronization.
         */
        int getWaitQueueLength(const AbstractQueuedSynchronizer::ConditionObject* condition) const;

        /**
         * @returns true if there has ever been the need for the acquire method to block.
         */
        bool hasContended() const;

        /**
         * @returns true if there are threads that are currently waiting to acquire.
         */
        bool hasQueuedThreads() const;

        /**
         * Returns true if there are any threads that are currently waiting on the given ConditionObject,
         * the condition must be associated with this synchronizer instance.
         *
         * @returns true if the condition object has waiting threads.
         *
         * @throws NullPointerException if the ConditionObject pointer is NULL.
         * @throws IllegalArgumentException if the ConditionObject is not associated with this Synchronizer.
         * @throws IllegalMonitorStateException if the caller does not hold exclusive synchronization.
         */
        bool hasWaiters(const AbstractQueuedSynchronizer::ConditionObject* condition) const;

        /**
         * Traverse the Queue if waiting threads to see if the given thread is present.
         *
         * @returns true if the given thread is in the wait Queue.
         *
         * @throws NullPointerException if the thread pointer is NULL.
         */
        bool isQueued(decaf::lang::Thread* thread) const;

        /**
         * Checks whether the given ConditionObject uses this Synchronizer as its lock object.
         *
         * @returns true if the ConditionObject uses this Synchronizer as its lock.
         *
         * @throws NullPointerException if the condition pointer is NULL.
         */
        bool owns(const AbstractQueuedSynchronizer::ConditionObject* condition) const;

        /**
         * When held in exclusive mode this method releases the Synchronizer.  This method calls
         * tryRelease(int) and if one or more threads is unblocked it returns true.  This method
         * forms the basis of Lock.unlock.
         *
         * @param arg
         *      A value used to release, it is passed to tryRelease and not interpreted by this class.
         *
         * @return the result that is returned from a call to tryRelease(int).
         */
        bool release(int arg);

        /**
         * When held in shared mode this method releases the Synchronizer.  This method calls
         * tryReleaseShared(int) and if one or more threads is unblocked it returns true.
         *
         * @param arg
         *      A value used to release, it is passed to tryReleaseShared and not interpreted
         *      by this class.
         *
         * @return the result that is returned from a call to tryReleaseShared(int).
         */
        bool releaseShared(int arg);

        /**
         * Gets a string that identifies this Synchronizer along with its present state.  The string
         * contains the state in a bracketed form that contains "State =" and the result of getState()
         * and also contains the indicators "nonempty" or "empty" based on whether the thread queue
         * is empty or not.
         *
         * @returns a string value that identifies this AbstractQueuedSynchronizer.
         */
        std::string toString() const;

        /**
         * Acquires in exclusive mode if possible, first checking if the calling thread has already been
         * interrupted or not, then calling tryAcquire(int) at least one time and possibly more up to
         * the given timeout, or until the calling thread is interrupted.
         *
         * @param arg
         *      Value to be passed to tryAcquire(int) its meaning is uninterpreted here.
         * @param nanos
         *      Time in nanoseconds to wait before reporting the acquisition as failed.
         *
         * @returns true if the acquire succeeded, false otherwise.
         *
         * @throws InterruptedException if the calling thread is interrupted.
         */
        bool tryAcquireNanos(int arg, long long nanos);

        /**
         * Acquires in shared mode if possible, first checking if the calling thread has already been
         * interrupted or not, then calling tryAcquireShared(int) at least one time and possibly more up
         * to the given timeout, or until the calling thread is interrupted.
         *
         * @param arg
         *      Value to be passed to tryAcquireShared(int) its meaning is uninterpreted here.
         * @param nanos
         *      Time in nanoseconds to wait before reporting the acquisition as failed.
         *
         * @returns true if the acquire succeeded, false otherwise.
         *
         * @throws InterruptedException if the calling thread is interrupted.
         */
        bool tryAcquireSharedNanos(int arg, long long nanos);

    protected:

        /**
         * Gets and returns the currently set value of this object Synchronization sate.
         *
         * @returns the value of the synchronization sate.
         */
        virtual int getState() const;

        /**
         * Sets the synchronization state to the given value.
         *
         * @param value
         *      The new value to assign to the synchronization state.
         */
        virtual void setState(int value);

        /**
         * Sets the synchronization state to the specified value if the current value is equal to the
         * expected value given, otherwise no change is made.  This method is Atomic.
         *
         * @param expect
         *      The value that state must have if the update is made.
         * @param update
         *      The new value to assign the state if the current value matches the expected.
         *
         * @returns true if a change is made, false otherwise.
         */
        virtual bool compareAndSetState(int expect, int update);

        /**
         * If the calling thread hold an exclusive lock on this synchronization then this method returns
         * true, false otherwise.  The default behavior is to throw an UnsupportedOperation exception as
         * this method is only needed when ConditionObject is supported.
         *
         * @return true if this synchronization is held exclusively by the current thread.
         *
         * @throws UnsupportedOperationException if Condition objects are not supported.
         */
        virtual bool isHeldExclusively() const;

        /**
         * Performs the actual work of attempting to acquire the lock in exclusive mode.  The
         * implementation should acquire the lock in exclusive mode based on its current state or
         * the capabilities of the lock being implemented.
         *
         * Whenever a thread calls acquire this method is invoked.  If the method fails then the
         * acquire method can decide to block the calling thread until signaled that another attempt
         * to acquire should be made.
         *
         * The default implementation always throws UnsupportedOperationException.
         *
         * @param arg
         *      The value passed to the acquire method.
         *
         * @returns true if the acquire succeeded, false otherwise.
         *
         * @throws IllegalMonitorStateException if the acquire places the object in an invalid state.
         * @throws UnsupportedOperationException if exclusive mode is not supported.
         */
        virtual bool tryAcquire(int arg);

        /**
         * Performs the actual work of attempting to acquire the lock in shared mode.  The
         * implementation should acquire the lock in exclusive mode based on its current state or
         * the capabilities of the lock being implemented.
         *
         * Whenever a thread calls acquire this method is invoked.  If the method fails then the
         * acquire method can decide to block the calling thread until signaled that another attempt
         * to acquire should be made.
         *
         * The default implementation always throws UnsupportedOperationException.
         *
         * @param arg
         *      The value passed to the acquire method.
         *
         * @return a negative value if the acquire failed, zero if it did succeed but no additional
         *         shared mode acquires can, or a positive number if success and future calls amy
         *         also succeed.
         *
         * @throws IllegalMonitorStateException if the acquire places the object in an invalid state.
         * @throws UnsupportedOperationException if shared mode is not supported.
         */
        virtual int tryAcquireShared(int arg);

        /**
         * Performs a release for the calling thread in exclusive mode.  For any thread that performs
         * a release this method will always be invoked.
         *
         * The default implementation always throws UnsupportedOperationException.
         *
         * @param arg
         *      The value that was passed to the release method.
         *
         * @returns true if the synchronization is now fully released such that waiting threads can
         *          now attempt to acquire it, false if not fully released.
         *
         * @throws IllegalMonitorStateException if the release places the object in an invalid state.
         * @throws UnsupportedOperationException if exclusive mode is not supported.
         */
        virtual bool tryRelease(int arg);

        /**
         * Performs a release for the calling thread in shared mode.  For any thread that performs
         * a release this method will always be invoked.
         *
         * The default implementation always throws UnsupportedOperationException.
         *
         * @param arg
         *      The value that was passed to the release method.
         *
         * @returns true if the synchronization is now fully released such that waiting threads can
         *          now attempt to acquire it, false if not fully released.
         *
         * @throws IllegalMonitorStateException if the release places the object in an invalid state.
         * @throws UnsupportedOperationException if shared mode is not supported.
         */
        virtual bool tryReleaseShared(int arg);

        /**
         * Provides a means for derived classes to create a ConditionObject implemented by the basic
         * logic implemented inside this class.  Can be overridden by derived classes that wish to
         * provide their own implementation of a ConditionObject.
         *
         * @returns a new ConditionObject that is owned by the caller.
         */
        virtual ConditionObject* createDefaultConditionObject();

        /**
         * Queries whether any threads have been waiting to acquire longer than the
         * current thread.
         *
         * Note that because cancellations due to interrupts and timeouts may occur at any
         * time, a true return does not guarantee that some other thread will acquire before
         * the current thread.  Likewise, it is possible for another thread to win a race to
         * enqueue after this method has returned false, due to the queue being empty.
         *
         * This method is designed to be used by a fair synchronizer to avoid barging. Such a
         * synchronizer's tryAcquire method should return false, and its tryAcquireShared method
         * should return a negative value, if this method returns true (unless this is a
         * reentrant acquire).  For example, the tryAcquire method for a fair, reentrant,
         * exclusive mode synchronizer might look like this:
         *
         * <pre> {@code
         * virtual bool tryAcquire(int arg) {
         *   if (isHeldExclusively()) {
         *     // A reentrant acquire; increment hold count
         *     return true;
         *   } else if (hasQueuedPredecessors()) {
         *     return false;
         *   } else {
         *     // try to acquire normally
         *   }
         * }}
         * </pre>
         *
         * @return true if there is a queued thread preceding the current thread,
         *         and false if the current thread is at the head of the queue
         *         or the queue is empty
         */
        bool hasQueuedPredecessors() const;

        friend class SynchronizerState;
    };

}}}}

#endif /* _DECAF_UTIL_CONCURRENT_LOCKS_ABSTRACTQUEUEDSYNCHRONIZER_H_ */
