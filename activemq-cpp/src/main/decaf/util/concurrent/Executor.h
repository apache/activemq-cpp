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

#ifndef _DECAF_UTIL_CONCURRENT_EXECUTOR_H_
#define _DECAF_UTIL_CONCURRENT_EXECUTOR_H_

#include <decaf/util/Config.h>

#include <decaf/lang/Runnable.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/util/concurrent/RejectedExecutionException.h>

namespace decaf {
namespace util {
namespace concurrent {

    /**
     * An object that executes submitted {@link decaf.lang.Runnable} tasks. This
     * interface provides a way of decoupling task submission from the
     * mechanics of how each task will be run, including details of thread
     * use, scheduling, etc.  An <tt>Executor</tt> is normally used
     * instead of explicitly creating threads. For example, rather than
     * invoking <tt>new Thread(new(RunnableTask())).start()</tt> for each
     * of a set of tasks, you might use:
     *
     * <pre>
     * Executor executor = <em>anExecutor</em>;
     * executor->execute( new RunnableTask1() );
     * executor->execute( new RunnableTask2() );
     * ...
     * </pre>
     *
     * However, the <tt>Executor</tt> interface does not strictly
     * require that execution be asynchronous. In the simplest case, an
     * executor can run the submitted task immediately in the caller's
     * thread:
     *
     * <pre>
     * class DirectExecutor : public Executor {
     * public:
     *
     *     void execute( Runnable* r ) {
     *         r->run();
     *     }
     *
     * }</pre>
     *
     * More typically, tasks are executed in some thread other
     * than the caller's thread.  The executor below spawns a new thread
     * for each task.
     *
     * <pre>
     * class ThreadPerTaskExecutor : public Executor {
     * public:
     *     std::vector&lt;Thread*gt; threads;
     *
     *     void execute( Runnable* r ) {
     *         threads.push_back( new Thread( r ) );
     *         threads.rbegin()->start();
     *     }
     *
     * }</pre>
     *
     * The <tt>Executor</tt> implementations provided in this package
     * implement {@link decaf.util.concurrent.ExecutorService}, which is a more
     * extensive interface.  The {@link decaf.util.concurrent.ThreadPoolExecutor}
     * class provides an extensible thread pool implementation. The
     * {@link decaf.util.concurrentExecutor} class provides convenient factory
     * methods for these Executors.
     *
     * @since 1.0
     */
    class Executor {
    public:

        virtual ~Executor() {}

        /**
         * Executes the given command at some time in the future.  The command
         * may execute in a new thread, in a pooled thread, or in the calling
         * thread, at the discretion of the <tt>Executor</tt> implementation.
         *
         * @param command the runnable task
         *
         * @throws RejectedExecutionException if this task cannot be
         *         accepted for execution.
         *
         * @throws NullPointerException if command is null
         */
        virtual void execute( decaf::lang::Runnable* command ) = 0;

    };

}}}

#endif /* _DECAF_UTIL_CONCURRENT_EXECUTOR_H_ */
