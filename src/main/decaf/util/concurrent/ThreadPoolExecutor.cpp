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
#include <decaf/util/concurrent/ThreadPoolExecutor.h>
#include <decaf/util/concurrent/Mutex.h>
#include <decaf/util/concurrent/CountDownLatch.h>
#include <decaf/util/Timer.h>
#include <decaf/util/TimerTask.h>
#include <decaf/util/concurrent/atomic/AtomicInteger.h>
#include <decaf/util/concurrent/atomic/AtomicBoolean.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/util/concurrent/RejectedExecutionException.h>
#include <decaf/util/Config.h>
#include <decaf/util/LinkedList.h>
#include <decaf/lang/Pointer.h>

#include <algorithm>
#include <iostream>

using namespace std;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace decaf::util::concurrent::atomic;

////////////////////////////////////////////////////////////////////////////////
namespace decaf{
namespace util{
namespace concurrent{

    class Worker;

    class ExecutorKernel {
    public:

        ThreadPoolExecutor* parent;

        LinkedList<Worker*> workers;
        LinkedList<Worker*> deadWorkers;
        Timer cleanupTimer;

        AtomicBoolean stopping;
        AtomicBoolean stopped;
        AtomicBoolean terminated;
        AtomicInteger freeThreads;

        int maxPoolSize;
        int corePoolSize;
        long long keepAliveTime;
        Pointer< BlockingQueue<decaf::lang::Runnable*> > workQueue;
        Mutex mainLock;
        CountDownLatch termination;

        long long completedTasks;
        int largestPoolSize;

    public:

        ExecutorKernel(ThreadPoolExecutor* parent,
                       int corePoolSize, int maxPoolSize, long long keepAliveTime,
                       BlockingQueue<decaf::lang::Runnable*>* workQueue);

        ~ExecutorKernel();

        void onTaskStarted(Worker* thread);

        void onTaskCompleted(Worker* thread);

        void onTaskException(Worker* thread, lang::Exception& ex);

        void enQueueTask(Runnable* task);

        Runnable* deQueueTask();

        void AllocateThread();

        bool isStoppedOrStopping();

        void shutdown();

        bool awaitTermination(long long timeout, const TimeUnit& unit);

        void handleWorkerExit(Worker* worker);

        void tryTerminate();

    };

    class Worker : public lang::Thread {
    private:

        bool busy;
        bool done;
        decaf::util::concurrent::ExecutorKernel* kernel;

    private:

        Worker( const Worker& );
        Worker& operator= ( const Worker& );

     public:

        Worker(decaf::util::concurrent::ExecutorKernel* kernel) :
            Thread(), busy(false), done(false), kernel(kernel) {

            if( kernel == NULL ) {
                throw IllegalArgumentException( __FILE__, __LINE__,
                    "ThreadPoolExecutor Worker requires non-NULL pointer to parent ExecutorKernel");
            }
        }

        ~Worker() {}

        void run() {

            try {

                while(!this->done) {

                    // Blocks until there something to be done
                    Runnable* task = this->kernel->deQueueTask();

                    if(this->done) {
                        break;
                    }

                    if(!task) {
                        throw Exception( __FILE__, __LINE__,
                            "Worker - Retrieved NULL task from Kernel.");
                    }

                    this->busy = true;
                    this->kernel->onTaskStarted(this);

                    try{
                        task->run();
                    } catch(...) {}

                    delete task;

                    this->kernel->onTaskCompleted(this);
                    this->busy = false;
                }
            }
            catch( Exception& ex )
            {
                ex.setMark( __FILE__, __LINE__ );
                this->busy = false;
                this->kernel->onTaskException(this, ex);
            }
            catch(...)
            {
                Exception ex(__FILE__, __LINE__, "Worker - Caught Unknown Exception");
                this->busy = false;
                this->kernel->onTaskException(this, ex);
            }

            this->kernel->handleWorkerExit(this);
        }

        void stop() {
            this->done = true;
        }

        bool isBusy() {
            return this->busy;
        }

    };

    class WorkerKiller : public TimerTask {
    private:

        ExecutorKernel* kernel;

    public:

        WorkerKiller(ExecutorKernel* kernel) : kernel(kernel) {
        }

        virtual ~WorkerKiller() {}

        virtual void run() {
            try{
                synchronized(&kernel->mainLock) {
                    Pointer< Iterator<Worker*> > iter( kernel->deadWorkers.iterator() );
                    while(iter->hasNext()) {
                        delete iter->next();
                        iter->remove();
                    }
                }
            } catch(...) {}
        }

    };

}}}

////////////////////////////////////////////////////////////////////////////////
ThreadPoolExecutor::ThreadPoolExecutor(int corePoolSize, int maxPoolSize,
                                       long long keepAliveTime, const TimeUnit& unit,
                                       BlockingQueue<decaf::lang::Runnable*>* workQueue) :
    kernel(new ExecutorKernel(this, corePoolSize, maxPoolSize, unit.toMillis(keepAliveTime), workQueue)) {
}

////////////////////////////////////////////////////////////////////////////////
ThreadPoolExecutor::~ThreadPoolExecutor() {

    try{
        delete kernel;
    }
    DECAF_CATCH_NOTHROW( lang::Exception )
    DECAF_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutor::execute(Runnable* task ) {

    try{

        if( task == NULL ) {
            throw NullPointerException(
                __FILE__, __LINE__,
                "ThreadPoolExecutor::execute - Supplied Runnable pointer was NULL.");
        }

        this->kernel->enQueueTask(task);
    }
    DECAF_CATCH_RETHROW( lang::Exception )
    DECAF_CATCHALL_THROW( lang::Exception )
}

////////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutor::shutdown() {

    try{
        this->kernel->shutdown();
    }
    DECAF_CATCH_RETHROW( lang::Exception )
    DECAF_CATCHALL_THROW( lang::Exception )
}

////////////////////////////////////////////////////////////////////////////////
bool ThreadPoolExecutor::awaitTermination(long long timeout, const TimeUnit& unit) {

    try{
        return this->kernel->awaitTermination(timeout, unit);
    }
    DECAF_CATCH_RETHROW( lang::Exception )
    DECAF_CATCHALL_THROW( lang::Exception )
}

////////////////////////////////////////////////////////////////////////////////
int ThreadPoolExecutor::getPoolSize() const {
    int result = 0;
    synchronized(&this->kernel->mainLock) {
        result = this->kernel->workers.size();
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
int ThreadPoolExecutor::getCorePoolSize() const {
    return this->kernel->corePoolSize;
}

////////////////////////////////////////////////////////////////////////////////
int ThreadPoolExecutor::getMaximumPoolSize() const {
    return this->kernel->maxPoolSize;
}

////////////////////////////////////////////////////////////////////////////////
long long ThreadPoolExecutor::getTaskCount() const {
    return this->kernel->workQueue->size();
}

////////////////////////////////////////////////////////////////////////////////
int ThreadPoolExecutor::getActiveCount() const {

    int result = 0;
    synchronized(&this->kernel->mainLock) {
        if(!this->kernel->terminated.get()) {
            result = this->kernel->workers.size() - this->kernel->freeThreads.get();
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
long long ThreadPoolExecutor::getCompletedTaskCount() const {
    long long result = 0;
    synchronized(&this->kernel->mainLock) {
        result = this->kernel->completedTasks;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
int ThreadPoolExecutor::getLargestPoolSize() const {
    int result = 0;
    synchronized(&this->kernel->mainLock) {
        result = this->kernel->largestPoolSize;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
bool ThreadPoolExecutor::isShutdown() const {
    return this->kernel->stopped.get();
}

////////////////////////////////////////////////////////////////////////////////
bool ThreadPoolExecutor::isTerminated() const {
    return this->kernel->terminated.get();
}

////////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutor::terminated() {
}

////////////////////////////////////////////////////////////////////////////////
ExecutorKernel::ExecutorKernel(ThreadPoolExecutor* parent, int corePoolSize,
                               int maxPoolSize, long long keepAliveTime,
                               BlockingQueue<decaf::lang::Runnable*>* workQueue) :
    parent(parent),
    workers(),
    deadWorkers(),
    cleanupTimer(),
    stopping(false),
    stopped(false),
    terminated(false),
    freeThreads(0),
    maxPoolSize(maxPoolSize),
    corePoolSize(corePoolSize),
    keepAliveTime(keepAliveTime),
    workQueue(workQueue),
    mainLock(),
    termination(1),
    completedTasks(0),
    largestPoolSize(0) {

    if(corePoolSize < 0 || maxPoolSize <= 0 ||
       maxPoolSize < corePoolSize || keepAliveTime < 0) {

        throw IllegalArgumentException(__FILE__, __LINE__, "Argument out of range.");
    }

    if(workQueue == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "BlockingQueue pointer was null");
    }

    this->cleanupTimer.scheduleAtFixedRate(
        new WorkerKiller(this), TimeUnit::SECONDS.toMillis(10), TimeUnit::SECONDS.toMillis(10));
}

////////////////////////////////////////////////////////////////////////////////
ExecutorKernel::~ExecutorKernel() {
    try{

        this->shutdown();
        this->tryTerminate();

        this->termination.await();

        this->cleanupTimer.cancel();

        // Ensure dead Worker Threads are destroyed, the Timer might not have
        // run recently.
        Pointer< Iterator<Worker*> > iter(this->deadWorkers.iterator());
        while(iter->hasNext()) {
            Worker* worker = iter->next();
            worker->join();
            delete worker;
        }
    }
    DECAF_CATCH_NOTHROW(Exception)
    DECAF_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void ExecutorKernel::onTaskStarted(Worker* thread DECAF_UNUSED) {

    try{

        synchronized( &mainLock ) {

            freeThreads.decrementAndGet();

            // Now that this callback has decremented the free threads counter
            // lets check if there is any outstanding work to be done and no
            // threads to handle it.  This could happen if the QueueTask
            // method was called successively without any of the PooledThreads
            // having a chance to wake up and service the queue.  This would
            // cause the number of Task to exceed the number of free threads
            // once the Threads got a chance to wake up and service the queue
            if( freeThreads.get() == 0 && !workQueue->isEmpty() ) {
                AllocateThread();
            }
        }
    }
    DECAF_CATCH_RETHROW( lang::Exception )
    DECAF_CATCHALL_THROW( lang::Exception )
}

////////////////////////////////////////////////////////////////////////////////
void ExecutorKernel::onTaskCompleted(Worker* thread DECAF_UNUSED) {

    try {
        synchronized(&mainLock) {
            freeThreads.incrementAndGet();
            completedTasks++;
        }
    }
    DECAF_CATCH_RETHROW( lang::Exception )
    DECAF_CATCHALL_THROW( lang::Exception )
}

////////////////////////////////////////////////////////////////////////////////
void ExecutorKernel::onTaskException(Worker* thread DECAF_UNUSED, lang::Exception& ex DECAF_UNUSED) {

    try{
    }
    DECAF_CATCH_RETHROW( Exception )
    DECAF_CATCHALL_THROW( Exception )
}

////////////////////////////////////////////////////////////////////////////////
void ExecutorKernel::handleWorkerExit(Worker* worker) {

    synchronized( &this->mainLock ) {

        this->workers.remove(worker);
        this->deadWorkers.add(worker);

        if(this->workers.isEmpty()) {
            this->parent->terminated();
            this->terminated = true;
            this->termination.countDown();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void ExecutorKernel::enQueueTask(Runnable* task) {

    try{

        synchronized( &this->mainLock ) {

            // If there's nobody open to do work, then create some more
            // threads to handle the work.
            if( this->freeThreads.get() == 0 ) {
                AllocateThread();
            }
        }

        // queue the new work.
        if(!this->workQueue->offer(task)) {
            throw RejectedExecutionException(__FILE__, __LINE__, "Task Rejected by work Q");
        }
    }
    DECAF_CATCH_RETHROW( Exception )
    DECAF_CATCHALL_THROW( Exception )
}

////////////////////////////////////////////////////////////////////////////////
Runnable* ExecutorKernel::deQueueTask() {

    try{

        Runnable* task = NULL;

        // Wait for work, wait in a while loop since another thread could
        // be waiting for a lock and get the work before we get woken up
        // from our wait.
        while( !isStoppedOrStopping() ) {

            // TODO - Threads aren't interruptible yet.
            if(workQueue->poll(task, 10, TimeUnit::MILLISECONDS)) {
                break;
            }
        }

        // Don't give more work if we are closing down
        if(isStoppedOrStopping()) {

            if(task != NULL) {
                delete task;
            }

            return NULL;
        }

        if( task == NULL ) {
           throw lang::Exception(__FILE__, __LINE__,
               "deQueueTask: Got empty Runnable while not in shutdown.");
        }

        return task;
    }
    DECAF_CATCH_RETHROW( lang::Exception )
    DECAF_CATCHALL_THROW( lang::Exception )
}

////////////////////////////////////////////////////////////////////////////////
void ExecutorKernel::AllocateThread() {

    try{

        if( this->workers.size() >= this->maxPoolSize ) {
            return;
        }

        synchronized( &mainLock ) {
            Worker* newWorker = new Worker(this);
            this->workers.add(newWorker);
            freeThreads.incrementAndGet();
            newWorker->start();
            this->largestPoolSize++;
        }
    }
    DECAF_CATCH_RETHROW( lang::Exception )
    DECAF_CATCHALL_THROW( lang::Exception )
}

////////////////////////////////////////////////////////////////////////////////
bool ExecutorKernel::isStoppedOrStopping() {
    if(this->stopped.get() || this->stopping.get()) {
        return true;
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////
void ExecutorKernel::shutdown() {

    if(isStoppedOrStopping()) {
        return;
    }

    if(this->stopping.compareAndSet(false, true)) {

        synchronized(&mainLock) {

            Pointer< Iterator<Worker*> > iter(this->workers.iterator());

            while(iter->hasNext()) {
                iter->next()->stop();
            }

            // TODO - When threads are interruptible, we need to interrupt the Queue.
            //synchronized( workQueue.get() ) {
            //    // Signal the Queue so that all waiters are notified
            //    workQueue->notifyAll();
            //}
        }

        this->stopped.set(true);
    }
}

////////////////////////////////////////////////////////////////////////////////
bool ExecutorKernel::awaitTermination(long long timeout, const TimeUnit& unit) {

    if (this->terminated.get() == true) {
        return true;
    }

    return this->termination.await(timeout, unit);
}

////////////////////////////////////////////////////////////////////////////////
void ExecutorKernel::tryTerminate() {

    if (!this->isStoppedOrStopping() || (this->isStoppedOrStopping() && !this->workQueue->isEmpty())) {
        return;
    }

    if (this->workers.size() > 0) {
        // TODO - Once they are interruptible wake a worker.
        return;
    }

    synchronized(&this->mainLock) {
        try {
            this->parent->terminated();
        } catch(...) {}

        this->terminated.set(true);
        this->termination.countDown();
    }
}
