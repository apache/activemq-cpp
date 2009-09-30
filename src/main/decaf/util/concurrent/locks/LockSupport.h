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

#ifndef _DECAF_UTIL_CONCURRENT_LOCKSUPPORT_H_
#define _DECAF_UTIL_CONCURRENT_LOCKSUPPORT_H_

#include <decaf/util/Config.h>

namespace decaf {
namespace lang{
    class Thread;
}
namespace util {
namespace concurrent {
namespace locks {

    /**
     * Basic thread blocking primitives for creating locks and other synchronization classes.
     *
     * This class associates, with each thread that uses it, a permit (in the sense of the
     * Semaphore class). A call to park will return immediately if the permit is available,
     * consuming it in the process; otherwise it may block. A call to unpark makes the permit
     * available, if it was not already available. (Unlike with Semaphores though, permits do
     * not accumulate. There is at most one.)
     *
     * Methods park and unpark provide efficient means of blocking and unblocking threads
     * Races between one thread invoking park and another thread trying to unpark it will
     * preserve liveness, due to the permit. Additionally, park will return if the caller's
     * thread was interrupted, and timeout versions are supported. The park method may also
     * return at any other time, for "no reason", so in general must be invoked within a loop
     * that rechecks conditions upon return. In this sense park serves as an optimization of a
     * "busy wait" that does not waste as much time spinning, but must be paired with an unpark
     * to be effective.
     *
     * These methods are designed to be used as tools for creating higher-level synchronization
     * utilities, and are not in themselves useful for most concurrency control applications.
     * The park method is designed for use only in constructions of the form:
     *
     *   while (!canProceed()) { ... LockSupport.park(this); }
     *
     * where neither canProceed nor any other actions prior to the call to park entail locking or
     * blocking. Because only one permit is associated with each thread, any intermediary uses of
     * park could interfere with its intended effects.
     *
     * Sample Usage. Here is a sketch of a first-in-first-out non-reentrant lock class:
     *
     *   class FIFOMutex {
     *   private:
     *
     *      AtomicBoolean locked;
     *      ConcurrentLinkedQueue<Thread*> waiters;
     *
     *   public:
     *
     *      void lock() {
     *
     *         bool wasInterrupted = false;
     *         Thread* current = Thread::currentThread();
     *         waiters.add( current );
     *
     *         // Block while not first in queue or cannot acquire lock
     *         while( waiters.peek() != current ||
     *                !locked.compareAndSet( false, true ) ) {
     *
     *            LockSupport.park(this);
     *            if( Thread::interrupted() ) // ignore interrupts while waiting
     *               wasInterrupted = true;
     *            }
     *
     *            waiters.remove();
     *            if( wasInterrupted )          // reassert interrupt status on exit
     *               current.interrupt();
     *      }
     *
     *      void unlock() {
     *         locked.set( false );
     *         LockSupport.unpark( waiters.peek() );
     *      }
     *   };
     *
     * @since 1.0
     */
    class DECAF_API LockSupport {
    private:

        LockSupport();

    public:

        ~LockSupport();

        /**
         * Makes available the permit for the given thread, if it was not already available. If
         * the thread was blocked on park then it will unblock. Otherwise, its next call to park
         * is guaranteed not to block. This operation is not guaranteed to have any effect at all
         * if the given thread has not been started.
         *
         * @param thread the thread to unport, or NULL in which case the method has no effect.
         */
        static void unpark( decaf::lang::Thread* thread ) throw();

        /**
         * Disables the current thread for thread scheduling purposes unless the permit is available.
         *
         * If the permit is available then it is consumed and the call returns immediately; otherwise
         * the current thread becomes disabled for thread scheduling purposes and lies dormant until
         * one of three things happens:
         *
         *   * Some other thread invokes unpark with the current thread as the target; or
         *   * Some other thread interrupts the current thread; or
         *   * The call spuriously (that is, for no reason) returns.
         *
         * This method does not report which of these caused the method to return. Callers should
         * re-check the conditions which caused the thread to park in the first place. Callers may
         * also determine, for example, the interrupt status of the thread upon return.
         */
        static void park() throw();

        /**
         * Disables the current thread for thread scheduling purposes, for up to the specified
         * waiting time, unless the permit is available.
         *
         * If the permit is available then it is consumed and the call returns immediately;
         * otherwise the current thread becomes disabled for thread scheduling purposes and lies
         * dormant until one of four things happens:
         *
         *   * Some other thread invokes unpark with the current thread as the target; or
         *   * Some other thread interrupts the current thread; or
         *   * The specified waiting time elapses; or
         *   * The call spuriously (that is, for no reason) returns.
         *
         * This method does not report which of these caused the method to return. Callers should
         * re-check the conditions which caused the thread to park in the first place. Callers may
         * also determine, for example, the interrupt status of the thread, or the elapsed time
         * upon return.
         *
         * @param nanos the maximum number of nanoseconds to wait
         */
        static void parkNanos( long long nanos ) throw();

        /**
         * Disables the current thread for thread scheduling purposes, until the specified deadline,
         * unless the permit is available.
         *
         * If the permit is available then it is consumed and the call returns immediately; otherwise
         * the current thread becomes disabled for thread scheduling purposes and lies dormant until
         * one of four things happens:
         *
         *   * Some other thread invokes unpark with the current thread as the target; or
         *   * Some other thread interrupts the current thread; or
         *   * The specified deadline passes; or
         *   * The call spuriously (that is, for no reason) returns.
         *
         * This method does not report which of these caused the method to return. Callers should
         * re-check the conditions which caused the thread to park in the first place. Callers may
         * also determine, for example, the interrupt status of the thread, or the current time
         * upon return.
         *
         * @param deadline the absolute time, in milliseconds from the Epoch, to wait until
         */
        static void parkUntil( long long deadline ) throw();

    };

}}}}

#endif /* _DECAF_UTIL_CONCURRENT_LOCKSUPPORT_H_ */
