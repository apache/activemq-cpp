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

#ifndef _DECAF_CONCURRENT_COUNTDOWNLATCH_H_
#define _DECAF_CONCURRENT_COUNTDOWNLATCH_H_

#include <decaf/util/concurrent/Mutex.h>
#include <decaf/util/concurrent/TimeUnit.h>
#include <decaf/util/Config.h>
#include <decaf/lang/exceptions/InterruptedException.h>
#include <decaf/lang/Exception.h>

namespace decaf{
namespace util{
namespace concurrent{

    class DECAF_API CountDownLatch {
    private:

        /**
         * number to count down to
         */
        int count;

        /**
         * Mutex to protect the counts, and wait on.
         */
        Mutex mutex;

    public:

        /**
         * Constructor
         * @param count - number to count down from.
         */
        CountDownLatch( int count );

        virtual ~CountDownLatch();

        /**
         * Causes the current thread to wait until the latch has counted down to zero, unless
         * the thread is interrupted.
         *
         * If the current count is zero then this method returns immediately.
         *
         * If the current count is greater than zero then the current thread becomes disabled
         * for thread scheduling purposes and lies dormant until one of two things happen:
         *
         *   * The count reaches zero due to invocations of the countDown() method; or
         *   * Some other thread interrupts the current thread.
         *
         * If the current thread:
         *
         *   * has its interrupted status set on entry to this method; or
         *   * is interrupted while waiting,
         *
         * then InterruptedException is thrown and the current thread's interrupted status
         * is cleared.
         *
         * @throws InterruptedException - if the current thread is interrupted while waiting.
         * @throws Exception - if any other error occurs.
         */
        virtual void await();

        /**
         * Causes the current thread to wait until the latch has counted down to zero, unless the
         * thread is interrupted, or the specified waiting time elapses.
         *
         * If the current count is zero then this method returns immediately with the value true.
         *
         * If the current count is greater than zero then the current thread becomes disabled for
         * thread scheduling purposes and lies dormant until one of three things happen:
         *
         *   * The count reaches zero due to invocations of the countDown() method; or
         *   * Some other thread interrupts the current thread; or
         *   * The specified waiting time elapses.
         *
         * If the count reaches zero then the method returns with the value true.
         *
         * If the current thread:
         *
         *   * has its interrupted status set on entry to this method; or
         *   * is interrupted while waiting,
         *
         * then InterruptedException is thrown and the current thread's interrupted status is cleared.
         *
         * If the specified waiting time elapses then the value false is returned. If the time is
         * less than or equal to zero, the method will not wait at all.
         *
         * @param timeout - Time in milliseconds to wait for the count to reach zero.
         *
         * @throws InterruptedException - if the current thread is interrupted while waiting.
         * @throws Exception - if any other error occurs.
         */
        virtual bool await( long long timeOut );

        /**
         * Causes the current thread to wait until the latch has counted down to zero, unless the
         * thread is interrupted, or the specified waiting time elapses.
         *
         * If the current count is zero then this method returns immediately with the value true.
         *
         * If the current count is greater than zero then the current thread becomes disabled for
         * thread scheduling purposes and lies dormant until one of three things happen:
         *
         *   * The count reaches zero due to invocations of the countDown() method; or
         *   * Some other thread interrupts the current thread; or
         *   * The specified waiting time elapses.
         *
         * If the count reaches zero then the method returns with the value true.
         *
         * If the current thread:
         *
         *   * has its interrupted status set on entry to this method; or
         *   * is interrupted while waiting,
         *
         * then InterruptedException is thrown and the current thread's interrupted status is cleared.
         *
         * If the specified waiting time elapses then the value false is returned. If the time is
         * less than or equal to zero, the method will not wait at all.
         *
         * @param timeout - Time to wait for the count to reach zero.
         * @param unit - The units that the timeout specifies.
         *
         * @throws InterruptedException - if the current thread is interrupted while waiting.
         * @throws Exception - if any other error occurs.
         */
        virtual bool await( long long timeout, const TimeUnit& unit );

        /**
         * Counts down the latch, releasing all waiting threads when
         * the count hits zero.
         */
        virtual void countDown();

        /**
         * Gets the current count
         * @returns int count value
         */
        virtual int getCount() const {
            return this->count;
        }

    };

}}}

#endif /*_DECAF_CONCURRENT_COUNTDOWNLATCH_H_*/
