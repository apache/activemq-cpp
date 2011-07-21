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

#ifndef _DECAF_UTIL_CONCURRENT_FUTURETASK_H_
#define _DECAF_UTIL_CONCURRENT_FUTURETASK_H_

#include <decaf/util/Config.h>

#include <decaf/lang/Thread.h>
#include <decaf/lang/Pointer.h>
#include <decaf/lang/exceptions/NullPointerException.h>

#include <decaf/util/concurrent/RunnableFuture.h>
#include <decaf/util/concurrent/Callable.h>
#include <decaf/util/concurrent/Executors.h>
#include <decaf/util/concurrent/CancellationException.h>
#include <decaf/util/concurrent/ExecutionException.h>
#include <decaf/util/concurrent/TimeoutException.h>
#include <decaf/util/concurrent/TimeUnit.h>
#include <decaf/util/concurrent/locks/AbstractQueuedSynchronizer.h>

namespace decaf {
namespace util {
namespace concurrent {

    using decaf::lang::Pointer;

    /**
     * A cancellable asynchronous computation. This class provides a base implementation of
     * Future, with methods to start and cancel a computation, query to see if the computation
     * is complete, and retrieve the result of the computation. The result can only be retrieved
     * when the computation has completed; the get method will block if the computation has not
     * yet completed. Once the computation has completed, the computation cannot be restarted
     * or canceled.
     *
     * A FutureTask can be used to wrap a Callable or Runnable object. Because FutureTask
     * implements Runnable, a FutureTask can be submitted to an Executor for execution.
     *
     * In addition to serving as a standalone class, this class provides protected functionality
     * that may be useful when creating customized task classes.
     *
     * @since 1.0
     */
    template<typename T>
    class FutureTask : public RunnableFuture<T> {
    private:

        /**
         * Synchronization control for FutureTask.
         *
         * Uses AQS sync state to represent run status
         */
        class FutureTaskSync : public locks::AbstractQueuedSynchronizer {
        private:

            enum SyncState {
                /** State value representing that task is ready to run */
                READY = 0,
                /** State value representing that task is running */
                RUNNING = 1,
                /** State value representing that task ran */
                RAN = 2,
                /** State value representing that task was canceled */
                CANCELLED = 4
            };

            /** The underlying callable */
            Pointer< Callable<T> > callable;

            /** The result to return from get() */
            T result;

            /** The exception to throw from get() */
            Pointer<decaf::lang::Exception> exception;

            // The FutureTask parent of the Sync object.
            FutureTask* parent;

            /**
             * The thread running task. When nulled after set/cancel, this indicates that
             * the results are accessible.
             */
            decaf::lang::Thread* runner;

        public:

            FutureTaskSync(FutureTask* parent, Callable<T>* callable) :
                AbstractQueuedSynchronizer(), callable(callable), result(), exception(), parent(parent), runner(NULL) {
            }

            virtual ~FutureTaskSync() {

            }

            bool innerIsCancelled() const {
                return getState() == CANCELLED;
            }

            bool innerIsDone() const {
                return ranOrCancelled(getState()) && this->runner == NULL;
            }

            T innerGet() {
                this->acquireSharedInterruptibly(0);
                if (getState() == CANCELLED) {
                    throw CancellationException();
                }
                if (exception != NULL) {
                    throw ExecutionException(exception.get());
                }
                return result;
            }

            T innerGet(long long nanosTimeout) {
                if (!tryAcquireSharedNanos(0, nanosTimeout)) {
                    throw TimeoutException();
                }
                if (getState() == CANCELLED) {
                    throw CancellationException();
                }
                if (exception != NULL) {
                    throw ExecutionException(exception.get());
                }
                return result;
            }

            void innerSet(const T& result) {
                for (;;) {
                    int s = getState();
                    if (s == RAN) {
                        return;
                    }
                    if (s == CANCELLED) {
                        // aggressively release to set runner to null,
                        // in case we are racing with a cancel request
                        // that will try to interrupt runner
                        releaseShared(0);
                        return;
                    }
                    if (compareAndSetState(s, RAN)) {
                        this->result = result;
                        releaseShared(0);
                        this->parent->done();
                        return;
                    }
                }
            }

            void innerSetException(const decaf::lang::Exception& t) {
                for (;;) {
                    int s = getState();
                    if (s == RAN) {
                        return;
                    }
                    if (s == CANCELLED) {
                        // aggressively release to set runner to null,
                        // in case we are racing with a cancel request
                        // that will try to interrupt runner
                        releaseShared(0);
                        return;
                    }
                    if (compareAndSetState(s, RAN)) {
                        exception.reset(t.clone());
                        releaseShared(0);
                        this->parent->done();
                        return;
                    }
                }
            }

            bool innerCancel(bool mayInterruptIfRunning) {
                for (;;) {
                    int s = getState();
                    if (ranOrCancelled(s)) {
                        return false;
                    }
                    if (compareAndSetState(s, CANCELLED)) {
                        break;
                    }
                }

                if (mayInterruptIfRunning) {
                    decaf::lang::Thread* r = runner;
                    if (r != NULL) {
                        r->interrupt();
                    }
                }

                releaseShared(0);
                this->parent->done();
                return true;
            }

            void innerRun() {
                if (!compareAndSetState(READY, RUNNING)) {
                    return;
                }

                runner = decaf::lang::Thread::currentThread();
                if (getState() == RUNNING) { // recheck after setting thread
                    T result;
                    try {
                        result = this->callable->call();
                    } catch(decaf::lang::Exception& ex) {
                        this->parent->setException(ex);
                        return;
                    }
                    this->parent->set(result);
                } else {
                    releaseShared(0); // cancel
                }
            }

            bool innerRunAndReset() {
                if (!compareAndSetState(READY, RUNNING)) {
                    return false;
                }

                try {
                    this->runner = decaf::lang::Thread::currentThread();
                    if (getState() == RUNNING) {
                        this->callable->call(); // don't set result
                    }
                    this->runner = NULL;
                    return compareAndSetState(RUNNING, READY);
                } catch(decaf::lang::Exception& ex) {
                    this->parent->setException(ex);
                    return false;
                }
            }

        protected:

            /**
             * Implements AQS base acquire to succeed if ran or cancelled
             */
            virtual int tryAcquireShared(int ignore) {
                return innerIsDone() ? 1 : -1;
            }

            /**
             * Implements AQS base release to always signal after setting
             * final done status by nulling runner thread.
             */
            virtual bool tryReleaseShared(int ignore) {
                runner = NULL;
                return true;
            }

        private:

            bool ranOrCancelled(int state) const {
                return (state & (RAN | CANCELLED)) != 0;
            }
        };

    private:

        FutureTaskSync* sync;

    public:

        /**
         * Creates a FutureTask instance that will, upon running, execute the
         * given Callable.
         *
         * @param callable
         *      The callable task that will be invoked when run.
         *
         * @throws NullPointerException if callable pointer is NULL
         */
        FutureTask(Callable<T>* callable) : sync(NULL) {
            if (callable == NULL ) {
                throw decaf::lang::exceptions::NullPointerException(__FILE__, __LINE__,
                    "The Callable pointer passed to the constructor was NULL");
            }

            this->sync = new FutureTaskSync(this, callable);
        }

        /**
         * Creates a FutureTask that will, upon running, execute the given Runnable,
         * and arrange that the get method will return the given result on successful
         * completion.
         *
         * @param runnable
         *      The runnable task that the future will execute.
         * @param result
         *      The result to return on successful completion.
         *
         * @throws NullPointerException if runnable is NULL.
         */
        FutureTask(decaf::lang::Runnable* runnable, const T& result) {
            if (runnable == NULL ) {
                throw decaf::lang::exceptions::NullPointerException(__FILE__, __LINE__,
                    "The Runnable pointer passed to the constructor was NULL");
            }

            sync = new FutureTaskSync(this, Executors::callable<T>(runnable, result));
        }

        virtual ~FutureTask() {
            try{
                delete this->sync;
            }
            DECAF_CATCHALL_NOTHROW()
        }

        virtual bool isCancelled() const {
            return this->sync->innerIsCancelled();
        }

        virtual bool isDone() const {
            return this->sync->innerIsDone();
        }

        virtual bool cancel(bool mayInterruptIfRunning) {
            return this->sync->innerCancel(mayInterruptIfRunning);
        }

        virtual T get() {
            return this->sync->innerGet();
        }

        virtual T get(long long timeout, const TimeUnit& unit) {
            return this->sync->innerGet(unit.toNanos(timeout));
        }

    public:

        /**
         * Protected method invoked when this task transitions to state isDone
         * (whether normally or via cancellation). The default implementation
         * does nothing.  Subclasses may override this method to invoke completion
         * callbacks or perform bookkeeping. Note that you can query status inside
         * the implementation of this method to determine whether this task has
         * been canceled.
         */
        virtual void done() {}

        /**
         * Sets the result of this Future to the given value unless this future
         * has already been set or has been cancelled.  This method is invoked
         * internally by the <tt>run</tt> method upon successful completion of
         * the computation.
         *
         * @param v
         *      The value to return as the result of this Future.
         */
        virtual void set(const T& result) {
            this->sync->innerSet(result);
        }

        /**
         * Causes this future to report an ExecutionException with the given
         * Exception as its cause, unless this Future has already been set or
         * has been canceled.  This method is invoked internally by the run
         * method upon failure of the computation.
         *
         * @param error
         *      The cause of failure that is thrown from run.
         */
        virtual void setException(const decaf::lang::Exception& error) {
            this->sync->innerSetException(error);
        }

        virtual void run() {
            this->sync->innerRun();
        }

        /**
         * Executes the computation without setting its result, and then resets
         * this Future to initial state, failing to do so if the computation
         * encounters an exception or is canceled.  This is designed for use
         * with tasks that intrinsically execute more than once.
         *
         * @return true if successfully run and reset
         */
        virtual bool runAndReset() {
            return this->sync->innerRunAndReset();
        }

    };

}}}

#endif /* _DECAF_UTIL_CONCURRENT_FUTURETASK_H_ */
