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

#include "ThreadPoolExecutor.h"

#include <decaf/util/Config.h>
#include <decaf/util/LinkedList.h>
#include <decaf/util/Timer.h>
#include <decaf/util/TimerTask.h>
#include <decaf/util/concurrent/Future.h>
#include <decaf/util/concurrent/locks/ReentrantLock.h>
#include <decaf/util/concurrent/locks/AbstractQueuedSynchronizer.h>
#include <decaf/util/concurrent/CountDownLatch.h>
#include <decaf/util/concurrent/ConcurrentStlMap.h>
#include <decaf/util/concurrent/atomic/AtomicInteger.h>
#include <decaf/util/concurrent/atomic/AtomicBoolean.h>
#include <decaf/util/concurrent/RejectedExecutionException.h>
#include <decaf/util/concurrent/RejectedExecutionHandler.h>
#include <decaf/util/concurrent/Executors.h>
#include <decaf/lang/Throwable.h>
#include <decaf/lang/Pointer.h>
#include <decaf/lang/Math.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>
#include <decaf/lang/exceptions/NullPointerException.h>

#include <algorithm>
#include <iostream>

using namespace std;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace decaf::util::concurrent::atomic;
using namespace decaf::util::concurrent::locks;

////////////////////////////////////////////////////////////////////////////////
namespace decaf{
namespace util{
namespace concurrent{

    using decaf::lang::Pointer;

    /**
     * Any task that we don't own we wrap in this Runnable object so that the
     * task deletion logic can remain unchanged and thread safe.
     */
    class UnownedTaskWrapper : public Runnable {
    private:

        Runnable* task;

    private:

        UnownedTaskWrapper(const UnownedTaskWrapper&);
        UnownedTaskWrapper& operator=(const UnownedTaskWrapper&);

    public:

        UnownedTaskWrapper(Runnable* task) : Runnable(), task(task) {
        }

        virtual ~UnownedTaskWrapper() {
        }

        virtual void run() {
            this->task->run();
        }
    };

    /**
     * The main pool control state, ctl, is an atomic integer packing
     * two conceptual fields
     *   workerCount, indicating the effective number of threads
     *   runState,    indicating whether running, shutting down etc
     *
     * In order to pack them into one int, we limit workerCount to
     * (2^29)-1 (about 500 million) threads rather than (2^31)-1 (2
     * billion) otherwise representable. If this is ever an issue in
     * the future, the variable can be changed to be an AtomicLong,
     * and the shift/mask constants below adjusted. But until the need
     * arises, this code is a bit faster and simpler using an int.
     *
     * The workerCount is the number of workers that have been
     * permitted to start and not permitted to stop.  The value may be
     * transiently different from the actual number of live threads,
     * for example when a ThreadFactory fails to create a thread when
     * asked, and when exiting threads are still performing
     * bookkeeping before terminating. The user-visible pool size is
     * reported as the current size of the workers set.
     *
     * The runState provides the main lifecyle control, taking on values:
     *
     *   RUNNING:  Accept new tasks and process queued tasks
     *   SHUTDOWN: Don't accept new tasks, but process queued tasks
     *   STOP:     Don't accept new tasks, don't process queued tasks,
     *             and interrupt in-progress tasks
     *   TIDYING:  All tasks have terminated, workerCount is zero,
     *             the thread transitioning to state TIDYING
     *             will run the terminated() hook method
     *   TERMINATED: terminated() has completed
     *
     * The numerical order among these values matters, to allow
     * ordered comparisons. The runState monotonically increases over
     * time, but need not hit each state. The transitions are:
     *
     * RUNNING -> SHUTDOWN
     *    On invocation of shutdown(), perhaps implicitly in finalize()
     * (RUNNING or SHUTDOWN) -> STOP
     *    On invocation of shutdownNow()
     * SHUTDOWN -> TIDYING
     *    When both queue and pool are empty
     * STOP -> TIDYING
     *    When pool is empty
     * TIDYING -> TERMINATED
     *    When the terminated() hook method has completed
     *
     * Threads waiting in awaitTermination() will return when the
     * state reaches TERMINATED.
     *
     * Detecting the transition from SHUTDOWN to TIDYING is less
     * straightforward than you'd like because the queue may become
     * empty after non-empty and vice versa during SHUTDOWN state, but
     * we can only terminate if, after seeing that it is empty, we see
     * that workerCount is 0 (which sometimes entails a recheck -- see
     * below).
     */
    class ExecutorKernel {
    private:

        /**
         * The worker class does a small amount of Bookkeeping and provides a locking point
         * for the kernel to access the running task.
         */
        class Worker : public AbstractQueuedSynchronizer, public Runnable {
        private:

            Pointer<Thread> thread;
            Runnable* firstTask;
            decaf::util::concurrent::ExecutorKernel* kernel;
            long long completedTasks;

            friend class ExecutorKernel;

        private:

            Worker(const Worker&);
            Worker& operator=(const Worker&);

         public:

            Worker(ExecutorKernel* kernel, Runnable* task) :
                AbstractQueuedSynchronizer(), Runnable(), thread(), firstTask(task), kernel(kernel), completedTasks(0) {

                if( kernel == NULL ) {
                    throw IllegalArgumentException( __FILE__, __LINE__,
                        "ThreadPoolExecutor Worker requires non-NULL pointer to parent ExecutorKernel");
                }

                this->thread.reset(kernel->factory->newThread(this));
            }

            virtual ~Worker() {}

            void run() {
                // Delegate the running of this task to the Kernel so that all the logic
                // for task execution and cleanup is contained in one place.
                this->kernel->runWorker(this);
            }

            virtual void lock() {
                acquire(1);
            }

            virtual bool tryLock() {
                return tryAcquire(1);
            }

            virtual void unlock() {
                release(1);
            }

            virtual bool isLocked() {
                return isHeldExclusively();
            }

         protected:

            virtual bool isHeldExclusively() {
                return getState() == 1;
            }

            virtual bool tryAcquire(int unused DECAF_UNUSED) {
                if (compareAndSetState(0, 1)) {
                    setExclusiveOwnerThread(Thread::currentThread());
                    return true;
                }
                return false;
            }

            virtual bool tryRelease(int unused DECAF_UNUSED) {
                this->setExclusiveOwnerThread(NULL);
                this->setState(0);
                return true;
            }
        };

        /**
         * TimerTask implementation used to clean up Worker objects that have terminated
         * for some reason.  Since they can't delete themselves the cleanup is delegated
         * to the Timer's thread.
         */
        class WorkerKiller : public TimerTask {
        private:

            ExecutorKernel* kernel;

        private:

            WorkerKiller(const WorkerKiller&);
            WorkerKiller& operator=(const WorkerKiller&);

        public:

            WorkerKiller(ExecutorKernel* kernel) : kernel(kernel) {
            }

            virtual ~WorkerKiller() {}

            virtual void run() {
                kernel->mainLock.lock();

                LinkedList<Worker*> toDeleteList;

                try {
                    if (!kernel->isTerminated()) {
                        toDeleteList.copy(kernel->deadWorkers);
                        kernel->deadWorkers.clear();
                    }
                } catch(...) {
                }

                kernel->mainLock.unlock();

                try {
                    Pointer< Iterator<Worker*> > iter(toDeleteList.iterator());
                    while(iter->hasNext()) {
                        delete iter->next();
                        iter->remove();
                    }
                } catch(...) {}
            }
        };

    private:

        ExecutorKernel(const ExecutorKernel&);
        ExecutorKernel& operator= (const ExecutorKernel&);

    public:

        static const int COUNT_BITS;
        static const int CAPACITY;

        // runState is stored in the high-order bits
        static const int RUNNING;
        static const int SHUTDOWN;
        static const int STOP;
        static const int TIDYING;
        static const int TERMINATED;

        static const bool ONLY_ONE;

        AtomicInteger ctl;

        ThreadPoolExecutor* parent;

        /**
         * List containing all worker threads in pool. Accessed only when holding mainLock.
         */
        LinkedList<Worker*> workers;

        /**
         * List to hold Worker object that have terminated for some reason. Usually this is
         * because of a call to setMaximumPoolSize or setCorePoolSize but can also occur
         * because of an exception from a task that the worker was running.
         */
        LinkedList<Worker*> deadWorkers;

        /**
         * Timer used to periodically clean up the dead worker objects.  They must be cleaned
         * up on a separate thread because the Worker generally adds itself to the deadWorkers
         * list from the context of its run method and cannot delete itself.
         */
        Timer cleanupTimer;

        int maxPoolSize;
        int corePoolSize;
        long long keepAliveTime;
        bool coreThreadsCanTimeout;

        /**
         * The queue used for holding tasks and handing off to worker threads.
         * We do not require that workQueue.poll() returning NULL necessarily
         * means that workQueue.isEmpty(), so rely solely on isEmpty to see if
         * the queue is empty (which we must do for example when deciding whether
         * to transition from SHUTDOWN to TIDYING).  This accommodates special-
         * purpose queues such as DelayQueues for which poll() is allowed to
         * return NULL even if it may later return non-NULL when delays expire.
         */
        Pointer< BlockingQueue<decaf::lang::Runnable*> > workQueue;

        /**
         * Lock held on access to workers set and related bookkeeping. While we could
         * use a concurrent set of some sort, it turns out to be generally preferable
         * to use a lock. Among the reasons is that this serializes interruptIdleWorkers,
         * which avoids unnecessary interrupt storms, especially during shutdown.
         * Otherwise exiting threads would concurrently interrupt those that have not
         * yet interrupted. It also simplifies some of the associated statistics
         * bookkeeping of largestPoolSize etc. We also hold mainLock on shutdown and
         * shutdownNow, for the sake of ensuring workers set is stable while separately
         * interrupting.
         */
        ReentrantLock mainLock;

        /**
         * Wait condition to support awaitTermination
         */
        Pointer<Condition> termination;

        long long completedTasks;
        int largestPoolSize;

        Pointer<ThreadFactory> factory;
        Pointer<RejectedExecutionHandler> rejectionHandler;

    public:

        ExecutorKernel(ThreadPoolExecutor* parent,
                       int corePoolSize, int maxPoolSize, long long keepAliveTime,
                       BlockingQueue<decaf::lang::Runnable*>* workQueue,
                       ThreadFactory* threadFactory, RejectedExecutionHandler* handler) :
           ctl(ctlOf(RUNNING, 0)),
           parent(parent),
           workers(),
           deadWorkers(),
           cleanupTimer(),
           maxPoolSize(maxPoolSize),
           corePoolSize(corePoolSize),
           keepAliveTime(keepAliveTime),
           coreThreadsCanTimeout(false),
           workQueue(),
           mainLock(),
           termination(),
           completedTasks(0),
           largestPoolSize(0),
           factory(),
           rejectionHandler() {

           if(corePoolSize < 0 || maxPoolSize <= 0 ||
              maxPoolSize < corePoolSize || keepAliveTime < 0) {

               throw IllegalArgumentException(__FILE__, __LINE__, "Argument out of range.");
           }

           if(workQueue == NULL || threadFactory == NULL || handler == NULL) {
               throw NullPointerException(__FILE__, __LINE__, "Required parameter was NULL");
           }

           this->cleanupTimer.scheduleAtFixedRate(
               new WorkerKiller(this), TimeUnit::SECONDS.toMillis(10), TimeUnit::SECONDS.toMillis(10));

           this->workQueue.reset(workQueue);
           this->factory.reset(threadFactory);
           this->rejectionHandler.reset(handler);
           this->termination.reset(this->mainLock.newCondition());
        }

        ~ExecutorKernel() {
            try {

                // Turn off the cleanup timer first so that it doesn't fire while
                // we transition all the remaining workers into the dead workers
                // queue while can lead to lock contention.  Its run method holds
                // the mainLock so we need to wait for its release before moving on.
                try {
                    this->mainLock.lock();
                    this->cleanupTimer.cancel();
                    this->cleanupTimer.purge();
                    this->mainLock.unlock();
                } catch(Exception& ex) {
                    this->mainLock.unlock();
                }

                this->shutdown();
                this->awaitTermination();

                // We need to wait for the worker cleanup timer to shutdown, otherwise
                // it could segfault if it's still running when the destructor finishes.
                this->cleanupTimer.awaitTermination(10, TimeUnit::MINUTES);

                // Ensure dead Worker Threads are destroyed, the Timer might not have
                // run recently.
                Pointer< Iterator<Worker*> > workers(this->deadWorkers.iterator());
                while(workers->hasNext()) {
                    Worker* worker = workers->next();
                    worker->thread->join();
                    delete worker;
                }

                Pointer< Iterator<Runnable*> > tasks(this->workQueue->iterator());
                while(tasks->hasNext()) {
                    delete tasks->next();
                }

                this->workQueue->clear();
            }
            DECAF_CATCH_NOTHROW(Exception)
            DECAF_CATCHALL_NOTHROW()
        }

        // Packing and unpacking ctl
        static int runStateOf(int c) {
            return c & ~CAPACITY;
        }

        static int workerCountOf(int c) {
            return c & CAPACITY;
        }

        static int ctlOf(int rs, int wc) {
            return rs | wc;
        }

        int getPoolSize() {
            mainLock.lock();
            try {
                // Remove rare and surprising possibility of
                // isTerminated() && getPoolSize() > 0
                mainLock.unlock();
                return runStateAtLeast(ctl.get(), TIDYING) ? 0 : workers.size();
            } catch(Exception& ex) {
                mainLock.unlock();
                throw;
            }
        }

        int getActiveCount() {
            mainLock.lock();
            try {
                int n = 0;
                Pointer< Iterator<Worker*> > iter(workers.iterator());
                while(iter->hasNext()) {
                    Worker* worker = iter->next();
                    if (worker->isLocked()) {
                        ++n;
                    }
                }
                mainLock.unlock();
                return n;
            } catch(Exception& ex) {
                mainLock.unlock();
                throw;
            }
        }

        int getLargestPoolSize() {
            mainLock.lock();
            try {
                mainLock.unlock();
                return largestPoolSize;
            } catch(Exception& ex) {
                mainLock.unlock();
                throw;
            }
        }

        long long getTaskCount() {
            mainLock.lock();
            try {
                long long n = completedTasks;
                Pointer< Iterator<Worker*> > iter(workers.iterator());
                while(iter->hasNext()) {
                    Worker* worker = iter->next();
                    n += worker->completedTasks;
                    if (worker->isLocked()) {
                        ++n;
                    }
                }

                mainLock.unlock();
                return n + workQueue->size();
            } catch(Exception& ex) {
                mainLock.unlock();
                throw;
            }
        }

        long long getCompletedTaskCount() {
            mainLock.lock();
            try {
                long long n = completedTasks;
                Pointer< Iterator<Worker*> > iter(workers.iterator());
                while(iter->hasNext()) {
                    Worker* worker = iter->next();
                    n += worker->completedTasks;
                }

                mainLock.unlock();
                return n;
            } catch(Exception& ex) {
                mainLock.unlock();
                throw;
            }
        }

        /**
         * Transitions to TERMINATED state if either (SHUTDOWN and pool
         * and queue empty) or (STOP and pool empty).  If otherwise
         * eligible to terminate but workerCount is nonzero, interrupts an
         * idle worker to ensure that shutdown signals propagate. This
         * method must be called following any action that might make
         * termination possible -- reducing worker count or removing tasks
         * from the queue during shutdown. The method is non-private to
         * allow access from ScheduledThreadPoolExecutor.
         *
         * @returns true if the termination succeeded.
         */
        bool tryTerminate() {
            for (;;) {
                int c = ctl.get();
                if (isRunning(c) || runStateAtLeast(c, TIDYING) ||
                    (runStateOf(c) == SHUTDOWN && !workQueue->isEmpty())) {
                    return false;
                }

                if (workerCountOf(c) != 0) { // Eligible to terminate
                    interruptIdleWorkers(ONLY_ONE);
                    return false;
                }

                mainLock.lock();
                try {
                    if (ctl.compareAndSet(c, ctlOf(TIDYING, 0))) {
                        try {
                            this->parent->terminated();
                        } catch(Exception& ex) {
                            ctl.set(ctlOf(TERMINATED, 0));
                            termination->signalAll();
                            mainLock.unlock();
                            throw;
                        }

                        ctl.set(ctlOf(TERMINATED, 0));
                        termination->signalAll();
                        mainLock.unlock();
                        return true;
                    }
                } catch(Exception& ex) {
                    mainLock.unlock();
                    throw;
                }
                mainLock.unlock();
                // else retry on failed CAS
            }

            return false;
        }

        /**
         * Force an interrupt of all threads even if they are currently active.
         */
        void interruptWorkers() {
            mainLock.lock();
            try {
                Pointer< Iterator<Worker*> > iter(this->workers.iterator());
                while(iter->hasNext()) {
                    iter->next()->thread->interrupt();
                }
            } catch(Exception& ex) {
                mainLock.unlock();
                throw;
            }
            mainLock.unlock();
        }

        /**
         * Interrupts threads that might be waiting for tasks (as indicated by not
         * being locked) so they can check for termination or configuration changes.
         *
         * @param onlyOne
         *      If true, interrupt at most one worker. This is called only from
         *      tryTerminate when termination is otherwise enabled but there are
         *      still other workers.  In this case, at most one waiting worker is
         *      interrupted to propagate shutdown signals in case all threads are
         *      currently waiting.  Interrupting any arbitrary thread ensures that
         *      newly arriving workers since shutdown began will also eventually exit.
         *      To guarantee eventual termination, it suffices to always interrupt
         *      only one idle worker, but shutdown() interrupts all idle workers so
         *      that redundant workers exit promptly, not waiting for a straggler
         *      task to finish.
         */
        void interruptIdleWorkers(bool onlyOne) {
            mainLock.lock();
            try {
                Pointer< Iterator<Worker*> > iter(this->workers.iterator());
                while(iter->hasNext()) {
                    Worker* worker = iter->next();
                    Pointer<Thread> thread = worker->thread;
                    if (!thread->isInterrupted() && worker->tryLock()) {
                        try {
                            thread->interrupt();
                        } catch(Exception& ex) {
                            worker->unlock();
                        }

                        worker->unlock();
                    }

                    if (onlyOne) {
                        break;
                    }
                }
            } catch(Exception& ex) {
                mainLock.unlock();
                throw;
            }

            mainLock.unlock();
        }

        /**
         * Common form of interruptIdleWorkers, to avoid having to remember what
         * the boolean argument means.
         */
        void interruptIdleWorkers() {
            this->interruptIdleWorkers(false);
        }

        /**
         * Ensures that unless the pool is stopping, the current thread does not have
         * its interrupt set. This requires a double-check of state in case the interrupt
         * was cleared concurrently with a shutdownNow -- if so, the interrupt is re-enabled.
         */
        void clearInterruptsForTaskRun() {
            if (this->runStateLessThan(ctl.get(), STOP) && Thread::interrupted() &&
                this->runStateAtLeast(ctl.get(), STOP)) {

                Thread::currentThread()->interrupt();
            }
        }

        /**
         * State check needed by ScheduledThreadPoolExecutor to enable running
         * tasks during shutdown.
         *
         * @param shutdownOK
         *      true if should return true if SHUTDOWN
         */
        bool isRunningOrShutdown(bool shutdownOK) {
            int rs = this->runStateOf(ctl.get());
            return rs == RUNNING || (rs == SHUTDOWN && shutdownOK);
        }

        /**
         * Main worker run loop.  Repeatedly gets tasks from queue and
         * executes them, while coping with a number of issues:
         *
         * 1. We may start out with an initial task, in which case we
         * don't need to get the first one. Otherwise, as long as pool is
         * running, we get tasks from getTask. If it returns null then the
         * worker exits due to changed pool state or configuration
         * parameters.  Other exits result from exception throws in
         * external code, in which case completedAbruptly holds, which
         * usually leads processWorkerExit to replace this thread.
         *
         * 2. Before running any task, the lock is acquired to prevent
         * other pool interrupts while the task is executing, and
         * clearInterruptsForTaskRun called to ensure that unless pool is
         * stopping, this thread does not have its interrupt set.
         *
         * 3. Each task run is preceded by a call to beforeExecute, which
         * might throw an exception, in which case we cause thread to die
         * (breaking loop with completedAbruptly true) without processing
         * the task.
         *
         * 4. Assuming beforeExecute completes normally, we run the task,
         * gathering any of its thrown exceptions to send to
         * afterExecute. We separately handle RuntimeException, Error
         * (both of which the specs guarantee that we trap) and arbitrary
         * Throwables.  Because we cannot rethrow Throwables within
         * Runnable.run, we wrap them within Errors on the way out (to the
         * thread's UncaughtExceptionHandler).  Any thrown exception also
         * conservatively causes thread to die.
         *
         * 5. After task.run completes, we call afterExecute, which may
         * also throw an exception, which will also cause thread to
         * die. According to JLS Sec 14.20, this exception is the one that
         * will be in effect even if task.run throws.
         *
         * The net effect of the exception mechanics is that afterExecute
         * and the thread's UncaughtExceptionHandler have as accurate
         * information as we can provide about any problems encountered by
         * user code.
         *
         * @param w the worker
         */
        void runWorker(Worker* w) {
            Runnable* task = w->firstTask;
            w->firstTask = NULL;
            bool completedAbruptly = true;
            try {
                while (task != NULL || (task = getTask()) != NULL) {
                    w->lock();
                    clearInterruptsForTaskRun();
                    try {
                        this->parent->beforeExecute(w->thread.get(), task);
                        try {
                            task->run();
                        } catch (RuntimeException& re) {
                            this->parent->afterExecute(task, &re);
                            throw;
                        } catch (Exception& e) {
                            this->parent->afterExecute(task, &e);
                            throw;
                        } catch (std::exception& stdex) {
                            Exception ex(__FILE__, __LINE__, new std::exception(stdex),
                                "Caught unknown exception while executing task.");
                            this->parent->afterExecute(task, &ex);
                            throw ex;
                        } catch (...) {
                            Exception ex(__FILE__, __LINE__, "Caught unknown exception while executing task.");
                            this->parent->afterExecute(task, &ex);
                            throw ex;
                        }

                        this->parent->afterExecute(task, NULL);

                    } catch(Exception& ex) {
                        delete task;
                        task = NULL;
                        w->completedTasks++;
                        w->unlock();
                        throw;
                    }

                    delete task;
                    task = NULL;
                    w->completedTasks++;
                    w->unlock();
                }

                completedAbruptly = false;
            } catch(Exception& ex) {
                completedAbruptly = true;
            }

            processWorkerExit(w, completedAbruptly);
        }

        void execute(Runnable* task, bool takeOwnership) {

            if (task == NULL) {
                throw NullPointerException(__FILE__, __LINE__, "Runnable task cannot be NULL");
            }

            Runnable* target = task;

            /**
             * If we don't own it then wrap it so that our deletion logic is
             * still valid.
             */
            if (!takeOwnership) {
                target = new UnownedTaskWrapper(task);
            }

            /*
             * Proceed in 3 steps:
             *
             * 1. If fewer than corePoolSize threads are running, try to
             * start a new thread with the given command as its first
             * task.  The call to addWorker atomically checks runState and
             * workerCount, and so prevents false alarms that would add
             * threads when it shouldn't, by returning false.
             *
             * 2. If a task can be successfully queued, then we still need
             * to double-check whether we should have added a thread
             * (because existing ones died since last checking) or that
             * the pool shut down since entry into this method. So we
             * recheck state and if necessary roll back the enqueuing if
             * stopped, or start a new thread if there are none.
             *
             * 3. If we cannot queue task, then we try to add a new
             * thread.  If it fails, we know we are shut down or saturated
             * and so reject the task.
             */
            int c = ctl.get();
            if (workerCountOf(c) < corePoolSize) {
                if (addWorker(target, true)) {
                    return;
                }
                c = ctl.get();
            }

            if (isRunning(c) && workQueue->offer(target)) {
                int recheck = ctl.get();
                if (!isRunning(recheck) && this->remove(target)) {
                    this->rejectionHandler->rejectedExecution(target, this->parent);
                } else if (workerCountOf(recheck) == 0) {
                    addWorker(NULL, false);
                }
            } else if (!addWorker(target, false)) {
                this->rejectionHandler->rejectedExecution(target, this->parent);
            }
        }

        void shutdown() {
            mainLock.lock();
            try {
                advanceRunState(SHUTDOWN);
                interruptIdleWorkers();
                this->parent->onShutdown();
            } catch(Exception& ex) {
                mainLock.unlock();
                throw;
            }
            mainLock.unlock();
            tryTerminate();
        }

        void shutdownNow(ArrayList<Runnable*>& unexecutedTasks) {
            mainLock.lock();
            try {
                advanceRunState(STOP);
                interruptWorkers();
                drainQueue(unexecutedTasks);
            } catch(Exception& ex) {
                mainLock.unlock();
                throw;
            }
            mainLock.unlock();
            tryTerminate();
        }

        bool isShutdown() {
            return !isRunning(ctl.get());
        }

        bool isTerminating() {
            int c = ctl.get();
            return !isRunning(c) && runStateLessThan(c, TERMINATED);
        }

        bool isTerminated() {
            return runStateAtLeast(ctl.get(), TERMINATED);
        }

        bool awaitTermination() {
            mainLock.lock();
            try {

                for (;;) {

                    if (runStateAtLeast(ctl.get(), TERMINATED)) {
                        mainLock.unlock();
                        return true;
                    }

                    this->termination->await();
                }

            } catch(Exception& ex) {
                mainLock.unlock();
                throw;
            }
            mainLock.unlock();
            return false;
        }

        bool awaitTermination(long long timeout, const TimeUnit& unit) {
            long long nanos = unit.toNanos(timeout);
            mainLock.lock();
            try {

                for (;;) {

                    if (runStateAtLeast(ctl.get(), TERMINATED)) {
                        mainLock.unlock();
                        return true;
                    }

                    if (nanos <= 0) {
                        mainLock.unlock();
                        return false;
                    }

                    nanos = this->termination->awaitNanos(nanos);
                }

            } catch(Exception& ex) {
                mainLock.unlock();
                throw;
            }
            mainLock.unlock();
            return false;
        }

        void setCorePoolSize(int corePoolSize) {

            int delta = corePoolSize - this->corePoolSize;

            this->corePoolSize = corePoolSize;

            if (workerCountOf(ctl.get()) > corePoolSize) {
                interruptIdleWorkers();
            } else if (delta > 0) {
                // We don't really know how many new threads are "needed".
                // As a heuristic, prestart enough new workers (up to new
                // core size) to handle the current number of tasks in
                // queue, but stop if queue becomes empty while doing so.
                int k = Math::min(delta, workQueue->size());
                while (k-- > 0 && addWorker(NULL, true)) {
                    if (workQueue->isEmpty()) {
                        break;
                    }
                }
            }
        }

        void setMaximumPoolSize(int maximumPoolSize) {
            if (maximumPoolSize <= 0 || maximumPoolSize < corePoolSize) {
                throw IllegalArgumentException();
            }

            this->maxPoolSize = maximumPoolSize;

            if (workerCountOf(ctl.get()) > maximumPoolSize) {
                interruptIdleWorkers();
            }
        }

        bool prestartCoreThread() {
            return workerCountOf(ctl.get()) < corePoolSize && addWorker(NULL, true);
        }

        int prestartAllCoreThreads() {
            int n = 0;
            while (addWorker(NULL, true)) {
                ++n;
            }
            return n;
        }

        void allowCoreThreadTimeOut(bool value) {
            if (value && keepAliveTime <= 0) {
                throw IllegalArgumentException(__FILE__, __LINE__,
                    "Core threads must have nonzero keep alive times");
            }

            if (value != this->coreThreadsCanTimeout) {
                this->coreThreadsCanTimeout = value;
                if (value) {
                    interruptIdleWorkers();
                }
            }
        }

        void setKeepAliveTime(long long time, const TimeUnit& unit) {
            if (time < 0) {
                throw IllegalArgumentException();
            }

            if (time == 0 && this->coreThreadsCanTimeout) {
                throw IllegalArgumentException(__FILE__, __LINE__,
                    "Core threads must have nonzero keep alive times");
            }

            long long keepAliveTime = unit.toNanos(time);
            long long delta = keepAliveTime - this->keepAliveTime;

            this->keepAliveTime = keepAliveTime;

            if (delta < 0) {
                interruptIdleWorkers();
            }
        }

        void purge() {
            Pointer< BlockingQueue<Runnable*> > q = workQueue;
            try {

                Pointer< Iterator<Runnable*> > iter(q->iterator());
                while (iter->hasNext()) {
                    Runnable* r = iter->next();
                    FutureType* future = dynamic_cast<FutureType*>(r);
                    if (r != NULL && future->isCancelled()) {
                        iter->remove();
                    }
                }

            } catch (ConcurrentModificationException& ex) {
                // Take slow path if we encounter interference during traversal.
                // Make copy for traversal and call remove for cancelled entries.
                // The slow path is more likely to be O(N*N).
                std::vector<Runnable*> array = q->toArray();
                std::vector<Runnable*>::const_iterator iter = array.begin();
                for(; iter != array.end(); ++iter) {
                    Runnable* r = *iter;
                    FutureType* future = dynamic_cast<FutureType*>(r);
                    if (r != NULL && future->isCancelled()) {
                        q->remove(r);
                    }
                }
            }

            tryTerminate(); // In case SHUTDOWN and now empty
        }

        bool remove(Runnable* task) {
            bool result = this->workQueue->remove(task);
            this->tryTerminate();
            return result;
        }

    private:

        static bool runStateLessThan(int c, int s) {
            return c < s;
        }

        static bool runStateAtLeast(int c, int s) {
            return c >= s;
        }

        static bool isRunning(int c) {
            return c < SHUTDOWN;
        }

    private:

        /**
         * Drains the task queue into a new list, normally using drainTo. But if
         * the queue is a DelayQueue or any other kind of queue for which poll or
         * drainTo may fail to remove some elements, it deletes them one by one.
         *
         * @param unexecutedTasks
         *      Reference to an ArrayList where the tasks are to be moved.
         */
        void drainQueue(ArrayList<Runnable*>& unexecutedTasks) {

            // Some Queue implementations can fail in poll and drainTo so we check
            // after attempting to drain the Queue and if its not empty we remove
            // the tasks one by one.

            this->workQueue->drainTo(unexecutedTasks);
            if (!this->workQueue->isEmpty()) {

                std::vector<Runnable*> tasks = this->workQueue->toArray();
                std::vector<Runnable*>::iterator iter = tasks.begin();

                for (; iter != tasks.end(); ++iter) {

                    if (this->workQueue->remove(*iter)) {
                        unexecutedTasks.add(*iter);
                    }
                }
            }
        }

        /**
         * Transitions runState to given target, or leaves it alone if already at
         * least the given target.
         *
         * @param targetState the desired state, either SHUTDOWN or STOP
         *        (but not TIDYING or TERMINATED -- use tryTerminate for that)
         */
        void advanceRunState(int targetState) {
            for (;;) {
                int c = ctl.get();
                if (runStateAtLeast(c, targetState) || ctl.compareAndSet(c, ctlOf(targetState, workerCountOf(c))))
                    break;
            }
        }

        /**
         * Checks if a new worker can be added with respect to current pool state
         * and the given bound (either core or maximum). If so, the worker count
         * is adjusted accordingly, and, if possible, a new worker is created and
         * started running firstTask as its first task. This method returns false
         * if the pool is stopped or eligible to shut down. It also returns false
         * if the thread factory fails to create a thread when asked, which requires
         * a backout of workerCount, and a recheck for termination, in case the
         * existence of this worker was holding up termination.
         *
         * @param firstTask
         *      The task the new thread should run first (or null if none).
         *      Workers are created with an initial first task (in method execute())
         *      to bypass queuing when there are fewer than corePoolSize threads
         *      (in which case we always start one), or when the queue is full
         *      (in which case we must bypass queue). Initially idle threads are
         *      usually created via prestartCoreThread or to replace other dying workers.
         *
         * @param core
         *      If true use corePoolSize as bound, else maximumPoolSize.
         *
         * @return true if successful
         */
        bool addWorker(Runnable* firstTask, bool core) {
            retry:
            for (;;) {
                int c = ctl.get();
                int rs = this->runStateOf(c);

                // Check if queue empty only if necessary.
                if (rs >= SHUTDOWN && !(rs == SHUTDOWN && firstTask == NULL && !workQueue->isEmpty())) {
                    return false;
                }

                for (;;) {
                    int wc = this->workerCountOf(c);
                    if (wc >= CAPACITY || wc >= (core ? this->corePoolSize : this->maxPoolSize)) {
                        return false;
                    }
                    if (compareAndIncrementWorkerCount(c)) {
                        goto success;
                    }
                    c = ctl.get();  // Re-read ctl
                    if (runStateOf(c) != rs) {
                        goto retry;
                    }
                    // else CAS failed due to workerCount change; retry inner loop
                }
            }

            success:

            Pointer<Worker> w(new Worker(this, firstTask));
            Pointer<Thread> t = w->thread;

            mainLock.lock();
            try {
                // Recheck while holding lock. Back out on ThreadFactory failure or if
                // shut down before lock acquired.
                int c = ctl.get();
                int rs = runStateOf(c);

                if (t == NULL || (rs >= SHUTDOWN && !(rs == SHUTDOWN && firstTask == NULL))) {
                    decrementWorkerCount();
                    tryTerminate();
                    t.reset(NULL);
                    w.reset(NULL);
                    mainLock.unlock();
                    return false;
                }

                workers.add(w.release());

                int s = workers.size();
                if (s > largestPoolSize) {
                    largestPoolSize = s;
                }

            } catch(Exception& ex) {
                mainLock.unlock();
                throw;
            }

            t->start();

            mainLock.unlock();

            // It is possible (but unlikely) for a thread to have been added to
            // workers, but not yet started, during transition to STOP, which
            // could result in a rare missed interrupt, because Thread::interrupt
            // is not guaranteed to have any effect on a non-yet-started Thread
            // (see Thread#interrupt).
            if (runStateOf(ctl.get()) == STOP && !t->isInterrupted()) {
                t->interrupt();
            }

            return true;
        }

        /**
         * Performs cleanup and bookkeeping for a dying worker. Called only from
         * worker threads. Unless completedAbruptly is set, assumes that workerCount
         * has not already been adjusted to account for exit.  This method removes
         * thread from worker set, and possibly terminates the pool or replaces the
         * worker if either it exited due to user task exception or if fewer than
         * corePoolSize workers are running or queue is non-empty but there are no
         * workers.
         *
         * @param w
         *      The worker that has completed or exited.
         * @param completedAbruptly
         *      Indicates if the worker died due to user exception.
         */
        void processWorkerExit(Worker* w, bool completedAbruptly DECAF_UNUSED) {

            mainLock.lock();
            try {
                this->completedTasks += w->completedTasks;
                this->workers.remove(w);
                this->deadWorkers.add(w);
            } catch(...) {
            }
            decrementWorkerCount();
            mainLock.unlock();

            if (tryTerminate()) {
                return;
            }

            int c = ctl.get();
            if (runStateLessThan(c, STOP)) {
                if (!completedAbruptly) {
                    int min = this->coreThreadsCanTimeout ? 0 : corePoolSize;
                    if (min == 0 && ! workQueue->isEmpty()) {
                        min = 1;
                    }
                    if (workerCountOf(c) >= min) {
                        return; // replacement not needed
                    }
                }
                addWorker(NULL, false);
            }
        }

        /**
         * Performs blocking or timed wait for a task, depending on current configuration
         * settings, or returns NULL if this worker must exit because of any of:
         *
         *  1. There are more than maximumPoolSize workers (due to
         *     a call to setMaximumPoolSize).
         *  2. The pool is stopped.
         *  3. The pool is shutdown and the queue is empty.
         *  4. This worker timed out waiting for a task, and timed-out
         *     workers are subject to termination (that is,
         *     {@code allowCoreThreadTimeOut || workerCount > corePoolSize})
         *     both before and after the timed wait.
         *
         * @return task, or NULL if the worker must exit, in which case
         *         workerCount is decremented when the task completes.
         */
        Runnable* getTask() {
            bool timedOut = false; // Did the last poll() time out?

            retry:
            for (;;) {
                int c = ctl.get();
                int rs = runStateOf(c);

                // Check if queue empty only if necessary.
                if (rs >= SHUTDOWN && (rs >= STOP || workQueue->isEmpty())) {
                    return NULL;
                }

                bool timed;

                for (;;) {
                    int wc = workerCountOf(c);
                    timed = this->coreThreadsCanTimeout || wc > this->corePoolSize;

                    if (wc <= this->maxPoolSize && ! (timedOut && timed)) {
                        break;
                    }
                    if (compareAndDecrementWorkerCount(c)) {
                        return NULL;
                    }
                    c = ctl.get();  // Re-read ctl
                    if (runStateOf(c) != rs) {
                        goto retry;
                    }
                    // else CAS failed due to workerCount change; retry inner loop
                }

                try {
                    Runnable* r = NULL;
                    if (timed) {
                        workQueue->poll(r, keepAliveTime, TimeUnit::NANOSECONDS);
                    } else {
                        r = workQueue->take();
                    }

                    if (r != NULL) {
                        return r;
                    }

                    timedOut = true;
                } catch (InterruptedException& retry) {
                    timedOut = false;
                }
            }

            return NULL;
        }

        /**
         * Attempt to CAS-increment the workerCount field of ctl.
         */
        bool compareAndIncrementWorkerCount(int expect) {
            return ctl.compareAndSet(expect, expect + 1);
        }

        /**
         * Attempt to CAS-decrement the workerCount field of ctl.
         */
        bool compareAndDecrementWorkerCount(int expect) {
            return ctl.compareAndSet(expect, expect - 1);
        }

        /**
         * Decrements the workerCount field of ctl. This is called only on
         * abrupt termination of a thread (see processWorkerExit). Other
         * decrements are performed within getTask.
         */
        void decrementWorkerCount() {
            do {} while (!compareAndDecrementWorkerCount(ctl.get()));
        }

    };

    const bool ExecutorKernel::ONLY_ONE = true;

    const int ExecutorKernel::COUNT_BITS = Integer::SIZE - 3;
    const int ExecutorKernel::CAPACITY   = (1 << COUNT_BITS) - 1;

    // runState is stored in the high-order bits
    const int ExecutorKernel::RUNNING    = -1 << ExecutorKernel::COUNT_BITS;
    const int ExecutorKernel::SHUTDOWN   =  0 << ExecutorKernel::COUNT_BITS;
    const int ExecutorKernel::STOP       =  1 << ExecutorKernel::COUNT_BITS;
    const int ExecutorKernel::TIDYING    =  2 << ExecutorKernel::COUNT_BITS;
    const int ExecutorKernel::TERMINATED =  3 << ExecutorKernel::COUNT_BITS;

}}}

////////////////////////////////////////////////////////////////////////////////
ThreadPoolExecutor::ThreadPoolExecutor(int corePoolSize, int maxPoolSize,
                                       long long keepAliveTime, const TimeUnit& unit,
                                       BlockingQueue<decaf::lang::Runnable*>* workQueue) :
    AbstractExecutorService(),
    kernel(NULL) {

    try{

        if (workQueue == NULL) {
            throw NullPointerException(__FILE__, __LINE__, "The BlockingQueue pointer cannot be NULL.");
        }

        Pointer<RejectedExecutionHandler> handler(new ThreadPoolExecutor::AbortPolicy());
        Pointer<ThreadFactory> threadFactory(Executors::getDefaultThreadFactory());

        this->kernel = new ExecutorKernel(
            this, corePoolSize, maxPoolSize, unit.toMillis(keepAliveTime), workQueue,
            threadFactory.get(), handler.get());

        handler.release();
        threadFactory.release();
    }
    DECAF_CATCH_RETHROW(NullPointerException)
    DECAF_CATCH_RETHROW(IllegalArgumentException)
    DECAF_CATCH_RETHROW(Exception)
    DECAF_CATCHALL_THROW(Exception)
}

////////////////////////////////////////////////////////////////////////////////
ThreadPoolExecutor::ThreadPoolExecutor(int corePoolSize, int maxPoolSize,
                                       long long keepAliveTime, const TimeUnit& unit,
                                       BlockingQueue<decaf::lang::Runnable*>* workQueue,
                                       RejectedExecutionHandler* handler) :
    AbstractExecutorService(),
    kernel(NULL) {

    try{

        if(workQueue == NULL) {
            throw NullPointerException(__FILE__, __LINE__, "The BlockingQueue pointer cannot be NULL.");
        }

        if(handler == NULL) {
            throw NullPointerException(__FILE__, __LINE__, "The RejectedExecutionHandler pointer cannot be NULL.");
        }

        Pointer<ThreadFactory> threadFactory(Executors::getDefaultThreadFactory());

        this->kernel = new ExecutorKernel(
            this, corePoolSize, maxPoolSize, unit.toMillis(keepAliveTime), workQueue,
            threadFactory.get(), handler);

        threadFactory.release();
    }
    DECAF_CATCH_RETHROW(NullPointerException)
    DECAF_CATCH_RETHROW(IllegalArgumentException)
    DECAF_CATCH_RETHROW(Exception)
    DECAF_CATCHALL_THROW(Exception)
}

////////////////////////////////////////////////////////////////////////////////
ThreadPoolExecutor::ThreadPoolExecutor(int corePoolSize, int maxPoolSize,
                                       long long keepAliveTime, const TimeUnit& unit,
                                       BlockingQueue<decaf::lang::Runnable*>* workQueue,
                                       ThreadFactory* threadFactory) :
    AbstractExecutorService(),
    kernel(NULL) {

    try{

        if(workQueue == NULL) {
            throw NullPointerException(__FILE__, __LINE__, "The BlockingQueue pointer cannot be NULL.");
        }

        if(threadFactory == NULL) {
            throw NullPointerException(__FILE__, __LINE__, "The ThreadFactory pointer cannot be NULL.");
        }

        Pointer<RejectedExecutionHandler> handler(new ThreadPoolExecutor::AbortPolicy());

        this->kernel = new ExecutorKernel(
            this, corePoolSize, maxPoolSize, unit.toMillis(keepAliveTime), workQueue,
            threadFactory, handler.get());

        handler.release();
    }
    DECAF_CATCH_RETHROW(NullPointerException)
    DECAF_CATCH_RETHROW(IllegalArgumentException)
    DECAF_CATCH_RETHROW(Exception)
    DECAF_CATCHALL_THROW(Exception)
}

////////////////////////////////////////////////////////////////////////////////
ThreadPoolExecutor::ThreadPoolExecutor(int corePoolSize, int maxPoolSize,
                                       long long keepAliveTime, const TimeUnit& unit,
                                       BlockingQueue<decaf::lang::Runnable*>* workQueue,
                                       ThreadFactory* threadFactory, RejectedExecutionHandler* handler) :
    AbstractExecutorService(),
    kernel(NULL) {

    try{

        if(workQueue == NULL) {
            throw NullPointerException(__FILE__, __LINE__, "The BlockingQueue pointer cannot be NULL.");
        }

        if(handler == NULL) {
            throw NullPointerException(__FILE__, __LINE__, "The RejectedExecutionHandler pointer cannot be NULL.");
        }

        if(threadFactory == NULL) {
            throw NullPointerException(__FILE__, __LINE__, "The ThreadFactory pointer cannot be NULL.");
        }

        this->kernel = new ExecutorKernel(
            this, corePoolSize, maxPoolSize, unit.toMillis(keepAliveTime), workQueue,
            threadFactory, handler);
    }
    DECAF_CATCH_RETHROW(NullPointerException)
    DECAF_CATCH_RETHROW(IllegalArgumentException)
    DECAF_CATCH_RETHROW(Exception)
    DECAF_CATCHALL_THROW(Exception)
}

////////////////////////////////////////////////////////////////////////////////
ThreadPoolExecutor::~ThreadPoolExecutor() {

    try{
        delete kernel;
    }
    DECAF_CATCH_NOTHROW(Exception)
    DECAF_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutor::execute(Runnable* task) {

    try{

        if( task == NULL ) {
            throw NullPointerException(
                __FILE__, __LINE__,
                "ThreadPoolExecutor::execute - Supplied Runnable pointer was NULL.");
        }

        this->kernel->execute(task, true);
    }
    DECAF_CATCH_RETHROW( RejectedExecutionException )
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCH_RETHROW( Exception )
    DECAF_CATCHALL_THROW( Exception )
}

////////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutor::execute(Runnable* task, bool takeOwnership) {

    try{

        if( task == NULL ) {
            throw NullPointerException(
                __FILE__, __LINE__,
                "ThreadPoolExecutor::execute - Supplied Runnable pointer was NULL.");
        }

        this->kernel->execute(task, takeOwnership);
    }
    DECAF_CATCH_RETHROW( RejectedExecutionException )
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCH_RETHROW( Exception )
    DECAF_CATCHALL_THROW( Exception )
}

////////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutor::shutdown() {

    try{
        this->kernel->shutdown();
    }
    DECAF_CATCH_RETHROW( Exception )
    DECAF_CATCHALL_THROW( Exception )
}

////////////////////////////////////////////////////////////////////////////////
ArrayList<Runnable*> ThreadPoolExecutor::shutdownNow() {

    ArrayList<Runnable*> result;

    try{
        this->kernel->shutdownNow(result);
        return result;
    }
    DECAF_CATCH_RETHROW( Exception )
    DECAF_CATCHALL_THROW( Exception )
}

////////////////////////////////////////////////////////////////////////////////
bool ThreadPoolExecutor::awaitTermination(long long timeout, const TimeUnit& unit) {

    try{
        return this->kernel->awaitTermination(timeout, unit);
    }
    DECAF_CATCH_RETHROW( Exception )
    DECAF_CATCHALL_THROW( Exception )
}

////////////////////////////////////////////////////////////////////////////////
int ThreadPoolExecutor::getPoolSize() const {
    return this->kernel->getPoolSize();
}

////////////////////////////////////////////////////////////////////////////////
int ThreadPoolExecutor::getCorePoolSize() const {
    return this->kernel->corePoolSize;
}

////////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutor::setCorePoolSize(int poolSize) {

    if (poolSize < 0) {
        throw IllegalArgumentException(__FILE__, __LINE__, "Pool size given was negative.");
    }

    this->kernel->setCorePoolSize(poolSize);
}

////////////////////////////////////////////////////////////////////////////////
int ThreadPoolExecutor::getMaximumPoolSize() const {
    return this->kernel->maxPoolSize;
}

////////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutor::setMaximumPoolSize(int maxSize) {

    if (maxSize < 0) {
        throw IllegalArgumentException(__FILE__, __LINE__, "Maximum Pool size given was negative.");
    }

    this->kernel->setMaximumPoolSize(maxSize);
}

////////////////////////////////////////////////////////////////////////////////
long long ThreadPoolExecutor::getTaskCount() const {
    return this->kernel->getTaskCount();
}

////////////////////////////////////////////////////////////////////////////////
int ThreadPoolExecutor::getActiveCount() const {
    return this->kernel->getActiveCount();
}

////////////////////////////////////////////////////////////////////////////////
long long ThreadPoolExecutor::getCompletedTaskCount() const {
    return this->kernel->getCompletedTaskCount();
}

////////////////////////////////////////////////////////////////////////////////
int ThreadPoolExecutor::getLargestPoolSize() const {
    return this->kernel->getLargestPoolSize();
}

////////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutor::setThreadFactory(ThreadFactory* factory) {

    if (factory == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "Cannot assign a NULL ThreadFactory.");
    }

    if (factory != this->kernel->factory) {
        Pointer<ThreadFactory> temp(factory);
        this->kernel->factory.swap(temp);
    }
}

////////////////////////////////////////////////////////////////////////////////
ThreadFactory* ThreadPoolExecutor::getThreadFactory() const {
    return this->kernel->factory.get();
}

////////////////////////////////////////////////////////////////////////////////
RejectedExecutionHandler* ThreadPoolExecutor::getRejectedExecutionHandler() const {
    return this->kernel->rejectionHandler.get();
}

////////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutor::setRejectedExecutionHandler(RejectedExecutionHandler* handler) {

    if (handler == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "Cannot assign a NULL RejectedExecutionHandler.");
    }

    if (handler != this->kernel->rejectionHandler) {
        Pointer<RejectedExecutionHandler> temp(handler);
        this->kernel->rejectionHandler.swap(temp);
    }
}

////////////////////////////////////////////////////////////////////////////////
BlockingQueue<Runnable*>* ThreadPoolExecutor::getQueue() {
    return this->kernel->workQueue.get();
}

////////////////////////////////////////////////////////////////////////////////
bool ThreadPoolExecutor::isShutdown() const {
    return this->kernel->isShutdown();
}

////////////////////////////////////////////////////////////////////////////////
bool ThreadPoolExecutor::isTerminated() const {
    return this->kernel->isTerminated();
}

////////////////////////////////////////////////////////////////////////////////
bool ThreadPoolExecutor::isTerminating() const {
    return this->kernel->isTerminating();
}

////////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutor::allowCoreThreadTimeout(bool value) {
    this->kernel->allowCoreThreadTimeOut(value);
}

////////////////////////////////////////////////////////////////////////////////
long long ThreadPoolExecutor::getKeepAliveTime(const TimeUnit& unit) const {
    return unit.convert(this->kernel->keepAliveTime, TimeUnit::MILLISECONDS);
}

////////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutor::setKeepAliveTime(long long timeout, const TimeUnit& unit) {
    this->kernel->setKeepAliveTime(timeout, unit);
}

////////////////////////////////////////////////////////////////////////////////
bool ThreadPoolExecutor::allowsCoreThreadTimeout() const {
    return this->kernel->coreThreadsCanTimeout;
}

////////////////////////////////////////////////////////////////////////////////
bool ThreadPoolExecutor::prestartCoreThread() {
    return this->kernel->prestartCoreThread();
}

////////////////////////////////////////////////////////////////////////////////
int ThreadPoolExecutor::prestartAllCoreThreads() {
    return this->kernel->prestartAllCoreThreads();
}

////////////////////////////////////////////////////////////////////////////////
bool ThreadPoolExecutor::remove(decaf::lang::Runnable* task) {
    return this->kernel->remove(task);
}

////////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutor::purge() {
    this->kernel->purge();
}

////////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutor::beforeExecute(Thread* thread DECAF_UNUSED, Runnable* task DECAF_UNUSED) {
}

////////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutor::afterExecute(Runnable* task DECAF_UNUSED, decaf::lang::Throwable* error DECAF_UNUSED) {
}

////////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutor::terminated() {
}

////////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutor::onShutdown() {
}
