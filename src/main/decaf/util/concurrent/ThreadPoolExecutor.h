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
#ifndef _DECAF_UTIL_CONCURRENT_THREADPOOLEXECUTOR_H_
#define _DECAF_UTIL_CONCURRENT_THREADPOOLEXECUTOR_H_

#include <decaf/lang/Runnable.h>
#include <decaf/util/concurrent/atomic/AtomicBoolean.h>
#include <decaf/util/concurrent/ThreadFactory.h>
#include <decaf/util/concurrent/Mutex.h>
#include <decaf/util/concurrent/BlockingQueue.h>
#include <decaf/util/LinkedList.h>
#include <decaf/util/Config.h>

#include <vector>

namespace decaf{
namespace util{
namespace concurrent{

    using decaf::lang::Pointer;
    using decaf::util::concurrent::atomic::AtomicBoolean;

    class ExecutorKernel;

    /**
     * Defines a Thread Pool object that implements the functionality
     * of pooling threads to perform user tasks.  The Thread Poll has
     * max size that it will grow to.  The thread pool allocates threads
     * in blocks.  When there are no waiting worker threads and a task
     * is queued then a new batch is allocated.  The user can specify
     * the size of the blocks, otherwise a default value is used.
     * <P>
     * When the user queues a task they must also queue a listener to
     * be notified when the task has completed, this provides the user
     * with a mechanism to know when a task object can be freed.
     * <P>
     * To have the Thread Pool perform a task, the user enqueue's an
     * object that implements the <code>Runnable</code> interface and
     * one of the worker threads will executing it in its thread context.
     */
    class DECAF_API ThreadPoolExecutor {
    private:

        ThreadPoolExecutor( const ThreadPoolExecutor& );
        ThreadPoolExecutor& operator= ( const ThreadPoolExecutor& );

    private:

        ExecutorKernel* kernel;

    public:

        /**
         * Creates a new instance of a ThreadPoolExecutor.
         *
         * The executor instance is configured with the passed in parameters and a
         * default thread Factory is used along with a default rejected execution
         * handler.
         *
         * @param corePoolSize
         *      The number of threads to pool regardless of their idle state.
         * @param maxPoolSize
         *      The maximum number of threads that will ever exist at one time in the pool.
         * @param keepAliveTime
         *      The maximum time to keep a thread in the pool for if the number of current
         *      threads exceeds to core pool size.
         * @param unit
         *      The units that the keepAliveTime is specified in.
         * @param workQueue
         *      A BlockingQueue implementation that will be used to hold Runnable tasks
         *      that are awaiting execution within this executor.  The Executor takes
         *      ownership of the BlockingQueue instance passed once this method returns.
         *
         * @throws IllegalArguementException if the corePoolSize or keepAliveTime are negative
         *         or the or if maximumPoolSize is less than or equal to zero, or if corePoolSize
         *         is greater than maximumPoolSize.
         * @throws NullPointerException if the workQueue pointer is NULL.
         */
        ThreadPoolExecutor(int corePoolSize, int maxPoolSize,
                           long long keepAliveTime, const TimeUnit& unit,
                           BlockingQueue<decaf::lang::Runnable*>* workQueue);

        virtual ~ThreadPoolExecutor();

        /**
         * Queue a task to be completed by one of the Pooled Threads at some point in the
         * future.  The task can be rejected by this executor if it has been shut down or
         * if the workQueue is full, rejected Runnables are not deleted by this executor.
         * Upon successful return from this method the given Runnable pointer is considered
         * to be owned by this Executor and will be deleted upon completion or shut down.
         *
         * @param task
         *      The Runnable object that is to be executed.
         *
         * @throws RejectedExecutionException based on instruction from RejectedExecutionHandler
         *         if the given task cannot be accepted for execution at this time.
         * @throws NullPointerException - if command is null
         */
        virtual void execute(decaf::lang::Runnable* task);

        /**
         * Performs an orderly shutdown of this Executor.  Previously queued tasks are allowed
         * to complete but no new tasks are accepted for execution.  Calling this method more
         * than once has no affect on this executor.
         */
        virtual void shutdown();

        /**
         * Returns the number of threads that currently exists for this Executor.
         *
         * @return the configured number of Threads in the Pool.
         */
        virtual int getPoolSize() const;

        /**
         * Returns the configured number of core threads for this Executor.
         *
         * @return the configured number of core Threads.
         */
        virtual int getCorePoolSize() const;

        /**
         * Returns the configured maximum number of threads for this Executor.
         *
         * @return the configured maximum number of Threads.
         */
        virtual int getMaximumPoolSize() const;

        /**
         * Returns the current number of pending tasks in the work queue.  This is
         * an approximation as the number of pending tasks can quickly changes as
         * tasks complete and new tasks are started.
         *
         * @return number of outstanding tasks, approximate.
         */
        virtual long long getTaskCount() const;

    };

}}}

#endif /*_DECAF_UTIL_CONCURRENT_THREADPOOLEXECUTOR_H_*/
