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

#ifndef _DECAF_UTIL_CONCURRENT_EXECUTORSERVICE_H_
#define _DECAF_UTIL_CONCURRENT_EXECUTORSERVICE_H_

#include <decaf/util/Config.h>

#include <decaf/lang/Runnable.h>
#include <decaf/util/ArrayList.h>
#include <decaf/util/concurrent/Executor.h>
#include <decaf/util/concurrent/TimeUnit.h>
#include <decaf/lang/exceptions/InterruptedException.h>

namespace decaf {
namespace util {
namespace concurrent {

    /**
     * An Executor that provides methods to manage termination and methods that can produce a Future for
     * tracking progress of one or more asynchronous tasks.
     *
     * An ExecutorService can be shut down, which will cause it to reject new tasks. Two different methods
     * are provided for shutting down an ExecutorService. The shutdown() method will allow previously
     * submitted tasks to execute before terminating, while the shutdownNow() method prevents waiting tasks
     * from starting and attempts to stop currently executing tasks. Upon termination, an executor has no
     * tasks actively executing, no tasks awaiting execution, and no new tasks can be submitted. An unused
     * ExecutorService should be shut down to allow reclamation of its resources.
     *
     * Method submit extends base method Executor.execute(decaf.lang.Runnable) by creating and returning a
     * Future that can be used to cancel execution and/or wait for completion. Methods invokeAny and invokeAll
     * perform the most commonly useful forms of bulk execution, executing a collection of tasks and then
     * waiting for at least one, or all, to complete. (Class ExecutorCompletionService can be used to write
     * customized variants of these methods.)
     *
     * The Executors class provides factory methods for the executor services provided in this package.
     *
     * @since 1.0
     */
    class ExecutorService : public Executor {
    public:

        virtual ~ExecutorService() {}

        /**
         * The caller will block until the executor has completed termination meaning all tasks
         * that where scheduled before shutdown have now completed and the executor is ready for
         * deletion.  If the timeout period elapses before the executor reaches the terminated
         * state then this method return false to indicate it has not terminated.
         *
         * @param timeout
         *      The amount of time to wait before abandoning the wait for termination.
         * @param unit
         *      The unit of time that the timeout value represents.
         *
         * @return true if the executor terminated or false if the timeout expired.
         *
         * @throws InterruptedException if this call is interrupted while awaiting termination.
         */
        virtual bool awaitTermination( long long timeout, const TimeUnit& unit ) = 0;

        /**
         * Performs an orderly shutdown of this Executor.  Previously queued tasks are allowed
         * to complete but no new tasks are accepted for execution.  Calling this method more
         * than once has no affect on this executor.
         */
        virtual void shutdown() = 0;

        /**
         * Attempts to stop all currently executing tasks and returns an ArrayList containing the
         * Runnables that did not get executed, these object become the property of the caller and
         * are not deleted by this class, they are removed from the work queue and forgotten about.
         *
         * There is no guarantee that this method will halt execution of currently executing tasks.
         *
         * @return an ArrayList containing all Runnable instance that were still waiting to be
         *         executed by this class, call now owns those pointers.
         */
        virtual ArrayList<decaf::lang::Runnable*> shutdownNow() = 0;

        /**
         * Returns whether this executor has been shutdown or not.
         *
         * @return true if this executor has been shutdown.
         */
        virtual bool isShutdown() const = 0;

        /**
         * Returns whether all tasks have completed after this executor was shut down.
         *
         * @return true if all tasks have completed after a request to shut down was made.
         */
        virtual bool isTerminated() const = 0;

    };

}}}

#endif /* _DECAF_UTIL_CONCURRENT_EXECUTORSERVICE_H_ */
