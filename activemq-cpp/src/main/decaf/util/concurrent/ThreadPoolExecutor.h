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
#include <decaf/lang/Throwable.h>
#include <decaf/util/concurrent/ThreadFactory.h>
#include <decaf/util/concurrent/BlockingQueue.h>
#include <decaf/util/concurrent/TimeUnit.h>
#include <decaf/util/concurrent/AbstractExecutorService.h>
#include <decaf/util/concurrent/RejectedExecutionHandler.h>
#include <decaf/util/concurrent/RejectedExecutionException.h>
#include <decaf/util/LinkedList.h>
#include <decaf/util/ArrayList.h>
#include <decaf/util/Config.h>

#include <vector>

namespace decaf{
namespace util{
namespace concurrent{

    using decaf::lang::Pointer;

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
    class DECAF_API ThreadPoolExecutor : public AbstractExecutorService {
    private:

        ThreadPoolExecutor( const ThreadPoolExecutor& );
        ThreadPoolExecutor& operator= ( const ThreadPoolExecutor& );

    private:

        friend class ExecutorKernel;
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
         * @param handler
         *      A RejectedExecutionHandler implementation that will be used to handle any
         *      rejected tasks when they are submitted to this executor.  The Executor takes
         *      ownership of the RejectedExecutionHandler instance passed once this method returns.
         *
         * @throws IllegalArguementException if the corePoolSize or keepAliveTime are negative
         *         or the or if maximumPoolSize is less than or equal to zero, or if corePoolSize
         *         is greater than maximumPoolSize.
         * @throws NullPointerException if the workQueue pointer is NULL.
         */
        ThreadPoolExecutor(int corePoolSize, int maxPoolSize,
                           long long keepAliveTime, const TimeUnit& unit,
                           BlockingQueue<decaf::lang::Runnable*>* workQueue,
                           RejectedExecutionHandler* handler);

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
         * @param threadFactory
         *      A ThreadFactory implementation that will be used to create worker threads
         *      that are used by this executor to run the submitted tasks.  The Executor takes
         *      ownership of the ThreadFactory instance passed once this method returns.
         *
         * @throws IllegalArguementException if the corePoolSize or keepAliveTime are negative
         *         or the or if maximumPoolSize is less than or equal to zero, or if corePoolSize
         *         is greater than maximumPoolSize.
         * @throws NullPointerException if the workQueue pointer is NULL.
         */
        ThreadPoolExecutor(int corePoolSize, int maxPoolSize,
                           long long keepAliveTime, const TimeUnit& unit,
                           BlockingQueue<decaf::lang::Runnable*>* workQueue,
                           ThreadFactory* threadFactory);

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
         * @param threadFactory
         *      A ThreadFactory implementation that will be used to create worker threads
         *      that are used by this executor to run the submitted tasks.  The Executor takes
         *      ownership of the ThreadFactory instance passed once this method returns.
         * @param handler
         *      A RejectedExecutionHandler implementation that will be used to handle any
         *      rejected tasks when they are submitted to this executor.  The Executor takes
         *      ownership of the BlockingQueue instance passed once this method returns.
         *
         * @throws IllegalArguementException if the corePoolSize or keepAliveTime are negative
         *         or the or if maximumPoolSize is less than or equal to zero, or if corePoolSize
         *         is greater than maximumPoolSize.
         * @throws NullPointerException if the workQueue pointer is NULL.
         */
        ThreadPoolExecutor(int corePoolSize, int maxPoolSize,
                           long long keepAliveTime, const TimeUnit& unit,
                           BlockingQueue<decaf::lang::Runnable*>* workQueue,
                           ThreadFactory* threadFactory,
                           RejectedExecutionHandler* handler);

        virtual ~ThreadPoolExecutor();

        virtual void execute(decaf::lang::Runnable* task);

        virtual void shutdown();

        virtual ArrayList<decaf::lang::Runnable*> shutdownNow();

        virtual bool awaitTermination(long long timeout, const decaf::util::concurrent::TimeUnit& unit);

        virtual bool isShutdown() const;

        virtual bool isTerminated() const;

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
         * Set the number of threads that this executor treats as its core threads, this value
         * will override the value set in the constructor.  If the value given is less than the
         * current value then the core threads will shrink to the new value over time.  If the
         * value is larger than the current value then new threads may be started to process
         * currently pending tasks, otherwise they will be started as needed when new tasks
         * arrive.
         *
         * @param poolSize
         *      The new core pool size for this executor.
         *
         * @throws IllegalArgumentException if the pool size value is less than zero.
         */
        virtual void setCorePoolSize(int poolSize);

        /**
         * Returns the configured maximum number of threads for this Executor.
         *
         * @return the configured maximum number of Threads.
         */
        virtual int getMaximumPoolSize() const;

        /**
         * Sets the maximum number of workers this Executor is allowed to have at any given
         * time above the core pool size.  This new value overrides any set in the constructor
         * and if smaller than the current value worker threads will terminate as they complete
         * their current tasks and become idle.
         *
         * @param maxSize
         *      The new maximum allowed worker pool size.
         *
         * @throws IllegalArgumentException if maxSize is negative or less than core pool size.
         */
        virtual void setMaximumPoolSize(int maxSize);

        /**
         * Returns the current number of pending tasks in the work queue.  This is
         * an approximation as the number of pending tasks can quickly changes as
         * tasks complete and new tasks are started.
         *
         * @return number of outstanding tasks, approximate.
         */
        virtual long long getTaskCount() const;

        /**
         * Returns an approximation of the number of threads that are currently running
         * tasks for this executor.  This value can change rapidly.
         *
         * @return the number of currently active threads.
         */
        virtual int getActiveCount() const;

        /**
         * Returns the approximate number of Tasks that have been completed by this
         * Executor, this value never decreases.
         *
         * @return the number of completed tasks since creation of the Executor.
         */
        virtual long long getCompletedTaskCount() const;

        /**
         * Returns the most Threads that have ever been active at one time within this
         * Executors Thread pool.
         *
         * @return the largest number of threads ever to coexist in this executor.
         */
        virtual int getLargestPoolSize() const;

        /**
         * Provides access to the Task Queue used by this Executor.  This method is meant mainly
         * for debugging and monitoring, care should be taken when using this method.  The executor
         * continues to execute tasks from the Queue.
         *
         * @returns a pointer to the blocking queue that this executor stores future tasks in.
         */
        virtual BlockingQueue<decaf::lang::Runnable*>* getQueue();

        /**
         * Returns true if the executor has begin the process of terminating but has not yet
         * completed the process of shutting down all worker threads.  If the Executor does
         * not transition from this state to terminated after some time its generally an
         * indication that one of the submitted tasks will not complete and the executor is
         * locked in a terminating state.
         *
         * @return true if all tasks have completed after a request to shut down was made.
         */
        virtual bool isTerminating() const;

        /**
         * When true this setting allows the threads in the core pool to terminate if they
         * sit idle longer than the set keep alive time.  Core threads that terminate are
         * replaced as needed by new ones on demand.  This settings requires that the set
         * keep alive time be greater than zero and will throw an IllegalArguementException
         * if that is not the case.
         *
         * @param value
         *      Boolean value indicating if core threads are allowed to time out when idle.
         *
         * @throws IllegalArgumentException if the keep alive time is set to zero.
         */
        virtual void allowCoreThreadTimeout(bool value);

        /**
         * Returns whether this executor has been configured to allow core threads to
         * terminate if they sit idle longer than the configured keep alive time. Threads
         * that are not core threads continue to time out using the set keep alive value
         * regardless of whether this option is enabled.
         *
         * @returns true if core threads can timeout when idle.
         */
        virtual bool allowsCoreThreadTimeout() const;

        /**
         * Returns the currently set value for the maximum amount of time a worker Thread
         * that is not part of the core threads is allowed to sit idle before it terminates.
         *
         * @param unit
         *      The unit of time to return the results in.
         *
         * @returns the configure keep alive time in the requested time units.
         */
        virtual long long getKeepAliveTime(const TimeUnit& unit) const;

        /**
         * Configures the amount of time a non core Thread will remain alive after it has
         * completed its assigned task.  This value can also be applied to core threads if
         * the allowCoreThreadsTimeout option is enabled.
         *
         * @param timeout
         *      The amount of time an idle worker will live before terminating.
         * @param unit
         *      The units that the timeout is given in.
         *
         * @throws IllegalArgumentException if allowCoreThreadsTimeout is enabled and the
         *         the timeout value given is zero, or the timeout given is negative.
         */
        virtual void setKeepAliveTime(long long timeout, const TimeUnit& unit);

        /**
         * Sets the ThreadFactory instance used to create new Threads for this Executor.
         *
         * This class takes ownership of the given ThreadFactory and will destroy it
         * upon termination or when a new ThreadFactory is set using this method.
         *
         * @param factory
         *      A ThreadFactory instance used by this Executor to create new Threads.
         *
         * @throws NullPointerException if the given factory pointer is NULL.
         */
        virtual void setThreadFactory(ThreadFactory* factory);

        /**
         * Gets the currently configured ThreadFactory.  It is considered a programming
         * error to delete the pointer returned by this method.
         *
         * @returns the currently configured ThreadFactory instance used by this object.
         */
        virtual ThreadFactory* getThreadFactory() const;

        /**
         * Gets the currently configured RejectedExecutionHandler for this Executor.
         *
         * @returns a pointer to the current RejectedExecutionHandler.
         */
        virtual RejectedExecutionHandler* getRejectedExecutionHandler() const;

        /**
         * Sets the new RejectedExecutionHandler that this executor should use to process any
         * rejected Runnable tasks.  This executor takes ownership of the supplied pointer and
         * will desotroy it upon termination, any previous handler is destroyed by this call.
         *
         * @param handler
         *      The new RejectedExecutionHandler instance to use.
         *
         * @throws NullPointerException if the handler is NULL.
         */
        virtual void setRejectedExecutionHandler(RejectedExecutionHandler* handler);

        /**
         * By default a Core thread is only created once the first task is queued, this method
         * forces the creation of core thread that waits in an idle mode for new work to be
         * enqueued.  If the limit on core threads has already been reached then this method
         * returns false.
         *
         * @return true if a new core thread was added, false otherwise.
         */
        virtual bool prestartCoreThread();

        /**
         * This method will create and start new core threads running in an idle state waiting for
         * new tasks up to the set core thread limit.  When the limit is reached this method returns
         * zero to indicate no more core threads can be created.
         *
         * @returns the number of core threads created, or zero if the limit has already been met.
         */
        virtual int prestartAllCoreThreads();

        /**
         * Attempts to remove the Runnable from the work queue, if successful then the caller
         * now owns the Runnable and is responsible for deleting it.
         *
         * @param task
         *      The task that is to be removed from the work queue.
         *
         * @returns true if the task was removed from the Queue.
         */
        bool remove(decaf::lang::Runnable* task);

        /**
         * Attempts to remove any Future derived tasks from the pending work queue if they have
         * been canceled.  This method can be used to more quickly remove and reclaim space as
         * canceled tasks are not run but must await a worker thread to be removed normally.
         * Since there are multiple threads in operation its possible for this method to not
         * remove all canceled tasks from the work queue.
         */
        virtual void purge();

    protected:

        /**
         * Method called before a task is executed by the given thread.  The default implementation
         * of this method does nothing, however a subclass can override this method to add some
         * new functionality.
         *
         * It is recommended that a subclass call this method on its base class to ensure that
         * all base classes have a chance to process this event.
         *
         * @param thread
         *      The thread that will be executing the given task.
         * @param task
         *      The task that will be executed by the given thread.
         */
        virtual void beforeExecute(decaf::lang::Thread* thread, decaf::lang::Runnable* task);

        /**
         * Called upon completion of execution of a given task.  This method is called
         * from the Thread that executed the given Runnable.  If the Throwable pointer is
         * not NULL then its value is the Exception that caused the task to terminate.
         *
         * The base class implementation does nothing, a derived class should call this
         * method on its base class to ensure that all subclasses have a chance to process
         * the afterExecute event.
         *
         * @param task
         *      The Runnable instance that was executed by the calling Thread.
         * @param error
         *      The exception that was thrown from the given Runnable.
         */
        virtual void afterExecute(decaf::lang::Runnable* task, decaf::lang::Throwable* error);

        /**
         * Method invoked when the Executor has terminated, by default this method does
         * nothing.  When overridden the subclass should call superclass::terminated to
         * ensure that all subclasses have their terminated method invoked.
         */
        virtual void terminated();

    public:  // RejectedExecutionHandler implementations.

        /**
         * Handler policy for tasks that are rejected upon a call to ThreadPoolExecutor::execute
         * this class always throws a RejectedExecutionException.
         *
         * @since 1.0
         */
        class AbortPolicy : public RejectedExecutionHandler {
        public:

            AbortPolicy() : RejectedExecutionHandler() {
            }

            virtual ~AbortPolicy() {
            }

            virtual void rejectedExecution(decaf::lang::Runnable* task, ThreadPoolExecutor* executer DECAF_UNUSED) {
                delete task;
                throw RejectedExecutionException(__FILE__, __LINE__, "Unable to execute task.");
            }

        };

        /**
         * Handler policy for tasks that are rejected upon a call to ThreadPoolExecutor::execute
         * this class will attempt to run the task in the Thread that called the execute method
         * unless the executor is shutdown in which case the task is not run and is destroyed..
         *
         * @since 1.0
         */
        class CallerRunsPolicy : public RejectedExecutionHandler {
        public:

            CallerRunsPolicy() : RejectedExecutionHandler() {
            }

            virtual ~CallerRunsPolicy() {
            }

            virtual void rejectedExecution(decaf::lang::Runnable* task, ThreadPoolExecutor* executer DECAF_UNUSED) {

                if (executer->isShutdown()) {
                    delete task;
                    return;
                }

                try{
                    task->run();
                } catch(decaf::lang::Exception& ex) {
                    delete task;
                    throw ex;
                }
            }

            /**
             * Handler policy for tasks that are rejected upon a call to ThreadPoolExecutor::execute
             * this class always destroys the rejected task and returns quietly.
             *
             * @since 1.0
             */
            class DiscardPolicy : public RejectedExecutionHandler {
            public:

                DiscardPolicy() : RejectedExecutionHandler() {
                }

                virtual ~DiscardPolicy() {
                }

                virtual void rejectedExecution(decaf::lang::Runnable* task, ThreadPoolExecutor* executer DECAF_UNUSED) {
                    delete task;
                }

            };

            /**
             * Handler policy for tasks that are rejected upon a call to ThreadPoolExecutor::execute
             * this class always destroys the oldest unexecuted task in the Queue and then attempts
             * to execute the rejected task using the passed in executor..
             *
             * @since 1.0
             */
            class DiscardOldestPolicy : public RejectedExecutionHandler {
            public:

                DiscardOldestPolicy() : RejectedExecutionHandler() {
                }

                virtual ~DiscardOldestPolicy() {
                }

                virtual void rejectedExecution( decaf::lang::Runnable* task, ThreadPoolExecutor* executer ) {

                    if (executer->isShutdown()) {
                        delete task;
                        return;
                    }

                    try{

                        decaf::lang::Runnable* oldest = NULL;
                        executer->getQueue()->poll(oldest);
                        delete oldest;

                        executer->execute(task);
                    } catch(decaf::lang::Exception& ex) {
                        delete task;
                        throw ex;
                    }
                }

            };

        };
    };

}}}

#endif /*_DECAF_UTIL_CONCURRENT_THREADPOOLEXECUTOR_H_*/
