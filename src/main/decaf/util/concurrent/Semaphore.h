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

#ifndef _DECAF_UTIL_CONCURRENT_SEMAPHORE_H_
#define _DECAF_UTIL_CONCURRENT_SEMAPHORE_H_

#include <decaf/util/Config.h>

#include <decaf/lang/exceptions/InterruptedException.h>
#include <decaf/lang/exceptions/RuntimeException.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>

#include <decaf/util/concurrent/TimeUnit.h>

#include <memory>

namespace decaf {
namespace util {
namespace concurrent {

    class SemaphoreHandle;

    /**
     * A counting semaphore. Conceptually, a semaphore maintains a set of permits. Each acquire()
     * blocks if necessary until a permit is available, and then takes it. Each release() adds a
     * permit, potentially releasing a blocking acquirer. However, no actual permit objects are
     * used; the Semaphore just keeps a count of the number available and acts accordingly.
     *
     * Semaphores are often used to restrict the number of threads than can access some (physical
     * or logical) resource.
     *
     *  class Pool {
     *  private:
     *
     *    static const int MAX_AVAILABLE = 100;
     *    Semaphore available;
     *
     *    std::vector<std::string> items;
     *    std::vector<bool> used;
     *
     *    Mutex lock;
     *
     *  public:
     *
     *    Pool() : available( MAX_AVAILABLE, true ) {
     *        used.resize( MAX_AVAILABLE );
     *        items.resize( MAX_AVAILABLE );
     *    }
     *
     *    std::string getItem() throws InterruptedException {
     *      available.acquire();
     *      return getNextAvailableItem();
     *    }
     *
     *    void putItem( std::string x ) {
     *      if( markAsUnused(x) ) {
     *        available.release();
     *      }
     *    }
     *
     *    std::string getNextAvailableItem() {
     *
     *      synchronized( &lock ) {
     *        for( int i = 0; i < MAX_AVAILABLE; ++i ) {
     *          if( !used[i] ) {
     *             used[i] = true;
     *             return items[i];
     *           }
     *       }
     *
     *       return std::string(); // not reached
     *     }
     *
     *     bool markAsUnused( const std::string& item ) {
     *       synchronized( &lock ) {
     *         for( int i = 0; i < MAX_AVAILABLE; ++i ) {
     *           if( item == items[i] ) {
     *             if( used[i] ) {
     *               used[i] = false;
     *               return true;
     *             } else
     *               return false;
     *           }
     *         }
     *       }
     *       return false;
     *     }
     *   };
     *
     * Before obtaining an item each thread must acquire a permit from the semaphore, guaranteeing
     * that an item is available for use. When the thread has finished with the item it is returned
     * back to the pool and a permit is returned to the semaphore, allowing another thread to acquire
     * that item. Note that no synchronization lock is held when acquire() is called as that would
     * prevent an item from being returned to the pool. The semaphore encapsulates the synchronization
     * needed to restrict access to the pool, separately from any synchronization needed to maintain
     * the consistency of the pool itself.
     *
     * A semaphore initialized to one, and which is used such that it only has at most one permit
     * available, can serve as a mutual exclusion lock. This is more commonly known as a binary
     * semaphore, because it only has two states: one permit available, or zero permits available.
     * When used in this way, the binary semaphore has the property (unlike many Lock implementations),
     * that the "lock" can be released by a thread other than the owner (as semaphores have no notion
     * of ownership). This can be useful in some specialized contexts, such as deadlock recovery.
     *
     * The constructor for this class optionally accepts a fairness parameter. When set false, this
     * class makes no guarantees about the order in which threads acquire permits. In particular,
     * barging is permitted, that is, a thread invoking acquire() can be allocated a permit ahead of
     * a thread that has been waiting - logically the new thread places itself at the head of the
     * queue of waiting threads. When fairness is set true, the semaphore guarantees that threads
     * invoking any of the acquire methods are selected to obtain permits in the order in which their
     * invocation of those methods was processed (first-in-first-out; FIFO). Note that FIFO ordering
     * necessarily applies to specific internal points of execution within these methods. So, it is
     * possible for one thread to invoke acquire before another, but reach the ordering point after
     * the other, and similarly upon return from the method. Also note that the untimed tryAcquire
     * methods do not honor the fairness setting, but will take any permits that are available.
     *
     * Generally, semaphores used to control resource access should be initialized as fair, to ensure
     * that no thread is starved out from accessing a resource. When using semaphores for other kinds
     * of synchronization control, the throughput advantages of non-fair ordering often outweigh
     * fairness considerations.
     *
     * This class also provides convenience methods to acquire and release multiple permits at a time.
     * Beware of the increased risk of indefinite postponement when these methods are used without
     * fairness set true.
     *
     * @since 1.0
     */
    class DECAF_API Semaphore {
    private:

        std::auto_ptr<SemaphoreHandle> handle;

    private:

        Semaphore( const Semaphore& );
        Semaphore& operator= ( const Semaphore& );

    public:

        /**
         * Creates a Semaphore with the given number of permits and nonfair fairness setting.
         *
         * @param permits the initial number of permits available. This value may be negative, in
         *        which case releases must occur before any acquires will be granted.
         */
        Semaphore( int permits );

        /**
         * Creates a Semaphore with the given number of permits and the given fairness setting.
         *
         * @param permits the initial number of permits available. This value may be negative, in
         *        which case releases must occur before any acquires will be granted.
         * @param fair  true if this semaphore will guarantee first-in first-out granting of
         *        permits under contention, else false
         */
        Semaphore( int permits, bool fair );

        virtual ~Semaphore();

        /**
         * Acquires a permit from this semaphore, blocking until one is available, or the thread
         * is interrupted.
         *
         * Acquires a permit, if one is available and returns immediately, reducing the number of
         * available permits by one.
         *
         * If no permit is available then the current thread becomes disabled for thread scheduling
         * purposes and lies dormant until one of two things happens:
         *
         *   * Some other thread invokes the release() method for this semaphore and the current
         *     thread is next to be assigned a permit; or
         *   * Some other thread interrupts the current thread.
         *
         * If the current thread:
         *
         *   * has its interrupted status set on entry to this method; or
         *   * is interrupted while waiting for a permit,
         *
         * then InterruptedException is thrown and the current thread's interrupted status is cleared.
         *
         * @throw InterruptedException - if the current thread is interrupted.
         * @throw RuntimeException if an unexpected error occurs while acquiring the Semaphore.
         */
        void acquire();

        /**
         * Acquires a permit from this semaphore, blocking until one is available.
         *
         * Acquires a permit, if one is available and returns immediately, reducing the number of
         * available permits by one.
         *
         * If no permit is available then the current thread becomes disabled for thread scheduling
         * purposes and lies dormant until some other thread invokes the release() method for this
         * semaphore and the current thread is next to be assigned a permit.
         *
         * If the current thread is interrupted while waiting for a permit then it will continue to
         * wait, but the time at which the thread is assigned a permit may change compared to the
         * time it would have received the permit had no interruption occurred. When the thread does
         * return from this method its interrupt status will be set.
         *
         * @throw RuntimeException if an unexpected error occurs while acquiring the Semaphore.
         */
        void acquireUninterruptibly();

        /**
         * Acquires a permit from this semaphore, only if one is available at the time of invocation.
         *
         * Acquires a permit, if one is available and returns immediately, with the value true, reducing
         * the number of available permits by one.
         *
         * If no permit is available then this method will return immediately with the value false.
         *
         * Even when this semaphore has been set to use a fair ordering policy, a call to tryAcquire()
         * will immediately acquire a permit if one is available, whether or not other threads are
         * currently waiting. This "barging" behavior can be useful in certain circumstances, even
         * though it breaks fairness. If you want to honor the fairness setting, then use
         * tryAcquire(0, TimeUnit.SECONDS) which is almost equivalent (it also detects interruption).
         *
         * @returns true if a permit was acquired and false otherwise
         *
         * @throw RuntimeException if an unexpected error occurs while acquiring the Semaphore.
         */
        bool tryAcquire();

        /**
         * Acquires a permit from this semaphore, if one becomes available within the given waiting
         * time and the current thread has not been interrupted.
         *
         * Acquires a permit, if one is available and returns immediately, with the value true,
         * reducing the number of available permits by one.
         *
         * If no permit is available then the current thread becomes disabled for thread scheduling
         * purposes and lies dormant until one of three things happens:
         *
         *   * Some other thread invokes the release() method for this semaphore and the current
         *     thread is next to be assigned a permit; or
         *   * Some other thread interrupts the current thread; or
         *   * The specified waiting time elapses.
         *
         * If a permit is acquired then the value true is returned.
         *
         * If the current thread:
         *
         *   * has its interrupted status set on entry to this method; or
         *   * is interrupted while waiting to acquire a permit,
         *
         * then InterruptedException is thrown and the current thread's interrupted status is cleared.
         *
         * If the specified waiting time elapses then the value false is returned. If the time is less
         * than or equal to zero, the method will not wait at all.
         *
         * @param timeout the maximum time to wait for a permit
         * @param unit the time unit of the timeout argument
         *
         * @returns true if a permit was acquired and false if the waiting time elapsed before a permit
         *          was acquired
         *
         * @throw InterruptedException if the current thread is interrupted.
         * @throw RuntimeException if an unexpected error occurs while acquiring the Semaphore.
         */
        bool tryAcquire( long long timeout, const TimeUnit& unit );

        /**
         * Releases a permit, returning it to the semaphore.
         *
         * Releases a permit, increasing the number of available permits by one. If any threads are
         * trying to acquire a permit, then one is selected and given the permit that was just
         * released. That thread is (re)enabled for thread scheduling purposes.
         *
         * There is no requirement that a thread that releases a permit must have acquired that permit
         * by calling acquire(). Correct usage of a semaphore is established by programming convention
         * in the application.
         *
         * @throw RuntimeException if an unexpected error occurs while releasing the Semaphore.
         */
        void release();

        /**
         * Acquires the given number of permits from this semaphore, blocking until all are available,
         * or the thread is interrupted.
         *
         * Acquires the given number of permits, if they are available, and returns immediately,
         * reducing the number of available permits by the given amount.
         *
         * If insufficient permits are available then the current thread becomes disabled for thread
         * scheduling purposes and lies dormant until one of two things happens:
         *
         *   * Some other thread invokes one of the release methods for this semaphore, the current
         *     thread is next to be assigned permits and the number of available permits satisfies
         *     this request; or
         *   * Some other thread interrupts the current thread.
         *
         * If the current thread:
         *
         *   * has its interrupted status set on entry to this method; or
         *   * is interrupted while waiting for a permit,
         *
         * then InterruptedException is thrown and the current thread's interrupted status is cleared.
         * Any permits that were to be assigned to this thread are instead assigned to other threads
         * trying to acquire permits, as if permits had been made available by a call to release().
         *
         * @param permits the number of permits to acquire.
         *
         * @throw InterruptedException if the current thread is interrupted.
         * @throw IllegalArgumentException if the permits argument is negative.
         * @throw RuntimeException if an unexpected error occurs while acquiring the Semaphore.
         */
        void acquire( int permits );

        /**
         * Acquires the given number of permits from this semaphore, blocking until all are available.
         *
         * Acquires the given number of permits, if they are available, and returns immediately,
         * reducing the number of available permits by the given amount.
         *
         * If insufficient permits are available then the current thread becomes disabled for thread
         * scheduling purposes and lies dormant until some other thread invokes one of the release
         * methods for this semaphore, the current thread is next to be assigned permits and the number
         * of available permits satisfies this request.
         *
         * If the current thread is interrupted while waiting for permits then it will continue to wait
         * and its position in the queue is not affected. When the thread does return from this method
         * its interrupt status will be set.
         *
         * @param permits the number of permits to acquire.
         *
         * @throw IllegalArgumentException if the permits argument is negative.
         * @throw RuntimeException if an unexpected error occurs while acquiring the Semaphore.
         */
        void acquireUninterruptibly( int permits );

        /**
         * Acquires the given number of permits from this semaphore, only if all are available at the
         * time of invocation.
         *
         * Acquires the given number of permits, if they are available, and returns immediately, with
         * the value true, reducing the number of available permits by the given amount.
         *
         * If insufficient permits are available then this method will return immediately with the value
         * false and the number of available permits is unchanged.
         *
         * Even when this semaphore has been set to use a fair ordering policy, a call to tryAcquire
         * will immediately acquire a permit if one is available, whether or not other threads are
         * currently waiting. This "barging" behavior can be useful in certain circumstances, even
         * though it breaks fairness. If you want to honor the fairness setting, then use
         * tryAcquire(permits, 0, TimeUnit.SECONDS) which is almost equivalent (it also detects
         * interruption).
         *
         * @param permits the number of permits to acquire
         *
         * @returns true if the permits were acquired and false otherwise.
         *
         * @throw IllegalArgumentException if the permits argument is negative.
         * @throw RuntimeException if an unexpected error occurs while acquiring the Semaphore.
         */
        bool tryAcquire( int permits );

        /**
         * Acquires the given number of permits from this semaphore, if all become available within
         * the given waiting time and the current thread has not been interrupted.
         *
         * Acquires the given number of permits, if they are available and returns immediately,
         * with the value true, reducing the number of available permits by the given amount.
         *
         * If insufficient permits are available then the current thread becomes disabled for thread
         * scheduling purposes and lies dormant until one of three things happens:
         *
         *   * Some other thread invokes one of the release methods for this semaphore, the current
         *     thread is next to be assigned permits and the number of available permits satisfies
         *     this request; or
         *   * Some other thread interrupts the current thread; or
         *   * The specified waiting time elapses.
         *
         * If the permits are acquired then the value true is returned.
         *
         * If the current thread:
         *
         *   * has its interrupted status set on entry to this method; or
         *   * is interrupted while waiting to acquire the permits,
         *
         * then InterruptedException is thrown and the current thread's interrupted status is cleared.
         * Any permits that were to be assigned to this thread, are instead assigned to other threads
         * trying to acquire permits, as if the permits had been made available by a call to release().
         *
         * If the specified waiting time elapses then the value false is returned. If the time is less
         * than or equal to zero, the method will not wait at all. Any permits that were to be assigned
         * to this thread, are instead assigned to other threads trying to acquire permits, as if the
         * permits had been made available by a call to release().
         *
         * @param permits the number of permits to acquire
         * @param timeout the maximum amount of time to wait to acquire the permits.
         * @param unit the units that the timeout param represents.
         *
         * @returns true if all permits were acquired and false if the waiting time elapsed before
         *          all permits were acquired
         *
         * @throw IllegalArgumentException if the permits argument is negative.
         * @throw RuntimeException if an unexpected error occurs while acquiring the Semaphore.
         */
        bool tryAcquire( int permits, long long timeout, const TimeUnit& unit );

        /**
         * Releases the given number of permits, returning them to the semaphore.
         *
         * Releases the given number of permits, increasing the number of available permits by that
         * amount. If any threads are trying to acquire permits, then one is selected and given the
         * permits that were just released. If the number of available permits satisfies that thread's
         * request then that thread is (re)enabled for thread scheduling purposes; otherwise the
         * thread will wait until sufficient permits are available. If there are still permits
         * available after this thread's request has been satisfied, then those permits are assigned
         * in turn to other threads trying to acquire permits.
         *
         * @param permits the number of permits to release
         *
         * @throw IllegalArgumentException if the permits argument is negative.
         * @throw RuntimeException if an unexpected error occurs while releasing the Semaphore.
         */
        void release( int permits );

        /**
         * Returns the current number of permits available in this semaphore.
         *
         * This method is typically used for debugging and testing purposes.
         *
         * @returns the number of permits available in this semaphore
         */
        int availablePermits() const;

        /**
         * Acquires and returns all permits that are immediately available.
         *
         * @returns the number of permits acquired
         *
         * @throw RuntimeException if an unexpected error occurs while draining the Semaphore.
         */
        int drainPermits();

        /**
         * @returns true if this semaphore has fairness set true
         */
        bool isFair() const;

        /**
         * Returns a string identifying this semaphore, as well as its state. The state, in
         * brackets, includes the String "Permits =" followed by the number of permits.
         *
         * @returns a string identifying this semaphore, as well as its state
         */
        std::string toString() const;

    };

}}}

#endif /* _DECAF_UTIL_CONCURRENT_SEMAPHORE_H_ */
