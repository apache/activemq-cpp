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

#ifndef _DECAF_UTIL_CONCURRENT_FUTURE_H_
#define _DECAF_UTIL_CONCURRENT_FUTURE_H_

#include <decaf/util/concurrent/TimeUnit.h>

namespace decaf {
namespace util {
namespace concurrent {

    /**
     * A Future represents the result of an asynchronous computation. Methods
     * are provided to check if the computation is complete, to wait for its
     * completion, and to retrieve the result of the computation. The result
     * can only be retrieved using method get when the computation has
     * completed, blocking if necessary until it is ready. Cancellation is
     * performed by the cancel method. Additional methods are provided to
     * determine if the task completed normally or was canceled. Once a
     * computation has completed, the computation cannot be canceled. If you
     * would like to use a Future for the sake of cancellability but not
     * provide a usable result, you can declare types of the form Future<void*>
     * and return null as a result of the underlying task.
     */
    template<typename V>
    class Future {
    public:

        virtual ~Future() {}

        /**
         * Attempts to cancel execution of this task. This attempt will fail if the
         * task has already completed, has already been canceled, or could not be
         * canceled for some other reason. If successful, and this task has not
         * started when cancel is called, this task should never run. If the task
         * has already started, then the mayInterruptIfRunning parameter determines
         * whether the thread executing this task should be interrupted in an attempt
         * to stop the task.
         * <p>
         * After this method returns, subsequent calls to isDone() will always return
         * true. Subsequent calls to isCancelled() will always return true if this
         * method returned true.
         * <p>
         * @param mayInterruptIfRunning - true if the thread executing this task should
         * be interrupted; otherwise, in-progress tasks are allowed to complete.
         * @returns false if the task could not be canceled, typically because it has
         * already completed normally; true otherwise
         */
        virtual bool cancel( bool mayInterruptIfRunning ) = 0;

        /**
         * Returns true if this task was canceled before it completed normally.
         * @returns true if this task was canceled before it completed
         */
        virtual bool isCancelled() const = 0;

        /**
         * Returns true if this task completed. Completion may be due to normal termination,
         * an exception, or cancellation -- in all of these cases, this method will return
         * true.
         * @returns true if this task completed
         */
        virtual bool isDone() const = 0;

        /**
         * Waits if necessary for the computation to complete, and then retrieves its result.
         * @returns the computed result.
         * @throws CancellationException - if the computation was canceled
         * @throws ExecutionException - if the computation threw an exception
         * @throws InterruptedException - if the current thread was interrupted while waiting
         */
        virtual V get() = 0;

        /**
         * Waits if necessary for at most the given time for the computation to complete, and
         * then retrieves its result, if available.
         * @param timeout - the maximum time to wait
         * @param unit - the time unit of the timeout argument
         * @returns the computed result
         * @throws CancellationException - if the computation was canceled
         * @throws ExecutionException - if the computation threw an exception
         * @throws InterruptedException - if the current thread was interrupted while waiting
         * @throws TimeoutException - if the wait timed out
         */
        virtual V get( long long timeout, const TimeUnit& unit ) = 0;

    };

}}}

#endif /*_DECAF_UTIL_CONCURRENT_FUTURE_H_*/
