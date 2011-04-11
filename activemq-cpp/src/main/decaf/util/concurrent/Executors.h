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

#ifndef _DECAF_UTIL_CONCURRENT_EXECUTORS_H_
#define _DECAF_UTIL_CONCURRENT_EXECUTORS_H_

#include <decaf/util/Config.h>

#include <decaf/lang/Thread.h>
#include <decaf/util/concurrent/ExecutorService.h>
#include <decaf/util/concurrent/ThreadFactory.h>

namespace decaf {
namespace util {
namespace concurrent {

    /**
     * Implements a set of utilities for use with Executors, ExecutorService, ThreadFactory,
     * and Callable types, as well as providing factory methods for instance of these
     * types configured for the most common use cases.
     *
     * @since 1.0
     */
    class DECAF_API Executors {
    private:

        Executors();
        Executors(const Executors&);
        Executors& operator= (const Executors&);

    public:

        virtual ~Executors();

        /**
         * Creates and returns a new ThreadFactory that expresses the default behavior for
         * ThreadFactories used in Executor classes.  The default factory create a new
         * non-daemon thread with normal priority and a name whose value is equal to
         * pool-N-thread-M, where N is the sequence number of this factory, and M is the
         * sequence number of the thread created by this factory.
         *
         * @returns a new instance of the default thread factory used in Executors, the
         *          caller takes ownership of the returned pointer.
         */
        static ThreadFactory* getDefaultThreadFactory();

        /**
         * Creates a new ThreadPoolExecutor with a fixed number of threads to process incoming
         * tasks.  The thread pool will use an unbounded queue to store pending tasks.  At any
         * given time the maximum threads in the pool will be equal to the number given to this
         * factory method.  If a thread in the pool dies a new one will be spawned to take its
         * place in the pool.  Tasks that are submitted when all pooled threads are busy will
         * be held until a thread is freed if the pool has allocated its assigned number of
         * threads already.
         *
         * @param nThreads
         *      The number of threads to assign as the max for the new ExecutorService.
         *
         * @returns pointer to a new ExecutorService that is owned by the caller.
         *
         * @throws IllegalArgumentException if nThreads is less than or equal to zero.
         */
        static ExecutorService* newFixedThreadPool(int nThreads);

        /**
         * Creates a new ThreadPoolExecutor with a fixed number of threads to process incoming
         * tasks.  The thread pool will use an unbounded queue to store pending tasks.  At any
         * given time the maximum threads in the pool will be equal to the number given to this
         * factory method.  If a thread in the pool dies a new one will be spawned to take its
         * place in the pool.  Tasks that are submitted when all pooled threads are busy will
         * be held until a thread is freed if the pool has allocated its assigned number of
         * threads already.
         *
         * @param nThreads
         *      The number of threads to assign as the max for the new ExecutorService.
         * @param threadFactory
         *      Instance of a ThreadFactory that will be used by the Executor to spawn new
         *      worker threads.  This parameter cannot be NULL.
         *
         * @returns pointer to a new ExecutorService that is owned by the caller.
         *
         * @throws NullPointerException if threadFactory is NULL.
         * @throws IllegalArgumentException if nThreads is less than or equal to zero.
         */
        static ExecutorService* newFixedThreadPool(int nThreads, ThreadFactory* threadFactory);

    private:

        static void initialize();
        static void shutdown();

        friend class decaf::lang::Thread;

    };

}}}

#endif /* _DECAF_UTIL_CONCURRENT_EXECUTORS_H_ */
