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
#include <decaf/lang/Runnable.h>
#include <decaf/util/concurrent/Callable.h>

#include <decaf/lang/exceptions/NullPointerException.h>

namespace decaf {
namespace util {
namespace concurrent {

    class ThreadFactory;
    class ExecutorService;

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

    private:

        /**
         * A Callable subclass that runs given task and returns given result
         */
        template<typename E>
        class RunnableAdapter : public decaf::util::concurrent::Callable<E> {
        private:

            decaf::lang::Runnable* task;
            bool owns;
            E result;

        private:

            RunnableAdapter(const RunnableAdapter&);
            RunnableAdapter operator= (const RunnableAdapter&);

        public:

            RunnableAdapter(decaf::lang::Runnable* task, bool owns, const E& result) :
                decaf::util::concurrent::Callable<E>(), task(task), owns(owns), result(result) {
            }

            virtual ~RunnableAdapter() {
                try{
                    if (owns) {
                        delete this->task;
                    }
                }
                DECAF_CATCHALL_NOTHROW()
            }

            virtual E call() {
                this->task->run();
                return result;
            }
        };

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

        /**
         * Creates an Executor that uses a single worker thread operating off an unbounded queue
         * owned by the executor.  If the Executor's single thread should terminate for some reason
         * such as failure during the execution of a task, a new Thread will be created if the Executor
         * has not been shutdown and there are more tasks in the queue.  The Executor returned from this
         * method is owned by the caller but unlike the Executor returned from the method
         * newFixedThreadPool(1) this one cannot be reconfigurable to use more threads later on.
         *
         * @returns a new Executor pointer that is owned by the caller.
         */
        static ExecutorService* newSingleThreadExecutor();

        /**
         * Creates an Executor that uses a single worker thread operating off an unbounded queue
         * owned by the executor.  If the Executor's single thread should terminate for some reason
         * such as failure during the execution of a task, a new Thread will be created if the Executor
         * has not been shutdown and there are more tasks in the queue.  The Executor returned from this
         * method is owned by the caller but unlike the Executor returned from the method
         * newFixedThreadPool(1) this one cannot be reconfigurable to use more threads later on.
         *
         * @param threadFactory
         *      Instance of a ThreadFactory that will be used by the Executor to spawn new
         *      worker threads.  This parameter cannot be NULL and ownership passes to the Executor.
         *
         * @returns a new Executor pointer that is owned by the caller.
         *
         * @throws NullPointerException if threadFactory is NULL.
         */
        static ExecutorService* newSingleThreadExecutor(ThreadFactory* threadFactory);

        /**
         * Returns a new ExecutorService derived instance that wraps and takes ownership of the given
         * ExecutorService pointer.  The returned ExecutorService delegates all calls to the wrapped
         * ExecutorService instance but does not allow any configuration changes.  This method provides
         * a means of locking an ExecutorService instance configuration and prevents changes that might
         * be accomplished with casting.
         *
         * @param executor
         *      The ExecutorService pointer to wrap and take ownership of.
         *
         * @returns a new ExecutorService pointer that is owned by the caller.
         *
         * @throws NullPointerException if ExecutorService is NULL.
         */
        static ExecutorService* unconfigurableExecutorService(ExecutorService* executor);

    public:

        /**
         * Returns a Callable object that, when called, runs the given task and returns the default
         * value of the template type E (or E()).
         *
         * @param task
         *      The Runnable task that is to be executed.
         * @param owns
         *      Does the callable instance own the given Runnable task pointer, default is true.
         *
         * @returns a new Callable<E> pointer that is owned by the caller.
         *
         * @throws NullPointerException if the Runnable task is NULL
         */
        template<typename E>
        static Callable<E>* callable(decaf::lang::Runnable* task, bool owns = true) {

            if (task == NULL) {
                throw decaf::lang::exceptions::NullPointerException(__FILE__, __LINE__,
                    "The Runnable task argument cannot be NULL");
            }

            return new RunnableAdapter<E>(task, owns, E());
        }

        /**
         * Returns a Callable object that, when called, runs the given task and returns the default
         * value of the template type E (or E()).
         *
         * @param task
         *      The Runnable task that is to be executed.
         * @param result
         *      The value that is returned from the callable upon completion.
         * @param owns
         *      Does the callable instance own the given Runnable task pointer, default is true.
         *
         * @returns a new Callable<E> pointer that is owned by the caller.
         *
         * @throws NullPointerException if the Runnable task is NULL
         */
        template<typename E>
        static Callable<E>* callable(decaf::lang::Runnable* task, const E& result, bool owns = true) {

            if (task == NULL) {
                throw decaf::lang::exceptions::NullPointerException(__FILE__, __LINE__,
                    "The Runnable task argument cannot be NULL");
            }

            return new RunnableAdapter<E>(task, owns, result);
        }

    private:

        static void initialize();
        static void shutdown();

        friend class decaf::internal::util::concurrent::Threading;

    };

}}}

#endif /* _DECAF_UTIL_CONCURRENT_EXECUTORS_H_ */
