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
#include <decaf/util/concurrent/Mutex.h>
#include <decaf/util/concurrent/CountDownLatch.h>
#include <decaf/util/concurrent/atomic/AtomicInteger.h>
#include <decaf/util/concurrent/atomic/AtomicBoolean.h>
#include <decaf/util/concurrent/RejectedExecutionException.h>
#include <decaf/util/concurrent/RejectedExecutionHandler.h>
#include <decaf/util/concurrent/Executors.h>
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
        bool coreThreadsCanTimeout;
        Pointer< BlockingQueue<decaf::lang::Runnable*> > workQueue;
        Mutex mainLock;
        CountDownLatch termination;

        long long completedTasks;
        int largestPoolSize;

        Pointer<ThreadFactory> factory;
        Pointer<RejectedExecutionHandler> rejectionHandler;

    public:

        ExecutorKernel(ThreadPoolExecutor* parent,
                       int corePoolSize, int maxPoolSize, long long keepAliveTime,
                       BlockingQueue<decaf::lang::Runnable*>* workQueue,
                       ThreadFactory* threadFactory, RejectedExecutionHandler* handler);

        ~ExecutorKernel();

        void onTaskStarted(Worker* thread);

        void onTaskCompleted(Worker* thread);

        void onTaskException(Worker* thread, lang::Exception& ex);

        void enQueueTask(Runnable* task);

        Runnable* deQueueTask();

        bool addWorker();

        int addAllWorkers();

        bool isStoppedOrStopping();

        void shutdown();

        void shutdownNow(ArrayList<Runnable*>& unexecutedTasks);

        bool awaitTermination(long long timeout, const TimeUnit& unit);

        void handleWorkerExit(Worker* worker);

        void tryTerminate();

        void drainQueue(ArrayList<Runnable*>& unexecutedTasks);

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

                        if(task != NULL) {
                            delete task;
                        }

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
    AbstractExecutorService(),
    kernel(NULL) {

    try{

        if(workQueue == NULL) {
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
ArrayList<Runnable*> ThreadPoolExecutor::shutdownNow() {

    ArrayList<Runnable*> result;

    try{
        this->kernel->shutdownNow(result);
        return result;
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
void ThreadPoolExecutor::setCorePoolSize(int poolSize) {

    if (poolSize < 0) {
        throw IllegalArgumentException(__FILE__, __LINE__, "Pool size given was negative.");
    }

    synchronized(&this->kernel->mainLock) {

        //int delta = poolSize - this->kernel->corePoolSize;
        this->kernel->corePoolSize = poolSize;

        if (this->kernel->workers.size() > poolSize) {
            // TODO - Once Threads are interruptible wake them up so some can terminate.
        } else {

            // TODO - Create new threads up to the new pool size, unless we are out
            //        of work or run out while creating.
//            int target = Math::min(delta, this->kernel->workQueue->size());
//            while (target-- > 0 && addWorker(NULL, true)) {
//                if (this->kernel->workQueue->isEmpty()) {
//                    break;
//                }
//            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
int ThreadPoolExecutor::getMaximumPoolSize() const {
    return this->kernel->maxPoolSize;
}

////////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutor::setMaximumPoolSize(int maxSize) {

    if (maxSize < 0 || maxSize < this->kernel->corePoolSize) {
        throw IllegalArgumentException(__FILE__, __LINE__, "Size given was invalid.");
    }

    this->kernel->maxPoolSize = maxSize;

    if (this->kernel->workers.size() > maxSize) {
        // TODO - Wake idle worker threads when able to.
    }
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
        throw NullPointerException(__FILE__, __LINE__, "Cannot assign a NULL ThreadFactory.");
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
    return this->kernel->stopped.get();
}

////////////////////////////////////////////////////////////////////////////////
bool ThreadPoolExecutor::isTerminated() const {
    return this->kernel->terminated.get();
}

////////////////////////////////////////////////////////////////////////////////
bool ThreadPoolExecutor::isTerminating() const {
    return this->kernel->isStoppedOrStopping() && !this->kernel->terminated.get();
}

////////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutor::allowCoreThreadTimeout(bool value) {

    if (value == true && this->kernel->keepAliveTime == 0) {
        throw IllegalArgumentException(__FILE__, __LINE__,
            "Keep Alive Time must be set to a non-zero value to enable this option.");
    }

    if (value != this->kernel->coreThreadsCanTimeout) {
        this->kernel->coreThreadsCanTimeout = value;
        if (value == true) {
            // TODO - When Threads are interruptible wake works so they can check timeout.
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
long long ThreadPoolExecutor::getKeepAliveTime(const TimeUnit& unit) const {
    return unit.convert(this->kernel->keepAliveTime, TimeUnit::MILLISECONDS);
}

////////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutor::setKeepAliveTime(long long timeout, const TimeUnit& unit) {

    if (timeout < 0) {
        throw IllegalArgumentException(__FILE__, __LINE__, "Timeout value cannot be negative.");
    }

    if (this->kernel->coreThreadsCanTimeout == true && unit.toMillis(timeout) == 0) {
        throw IllegalArgumentException(__FILE__, __LINE__,
            "Keep Alive Time must be set to a non-zero value when allowCoreThreadsTimeout is enabled.");
    }

    long long keepAliveTime = unit.toMillis(timeout);
    long long delta = keepAliveTime - this->kernel->keepAliveTime;
    this->kernel->keepAliveTime = keepAliveTime;
    if (delta < 0) {
        // TODO - When Threads are interruptible wake works so they can check timeout.
    }
}

////////////////////////////////////////////////////////////////////////////////
bool ThreadPoolExecutor::allowsCoreThreadTimeout() const {
    return this->kernel->coreThreadsCanTimeout;
}

////////////////////////////////////////////////////////////////////////////////
bool ThreadPoolExecutor::prestartCoreThread() {
    return this->kernel->addWorker();
}

////////////////////////////////////////////////////////////////////////////////
int ThreadPoolExecutor::prestartAllCoreThreads() {
    return this->kernel->addAllWorkers();
}

////////////////////////////////////////////////////////////////////////////////
bool ThreadPoolExecutor::remove(decaf::lang::Runnable* task) {
    bool removed = this->kernel->workQueue->remove(task);
    this->kernel->tryTerminate();
    return removed;
}

////////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutor::purge() {
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
ExecutorKernel::ExecutorKernel(ThreadPoolExecutor* parent, int corePoolSize,
                               int maxPoolSize, long long keepAliveTime,
                               BlockingQueue<decaf::lang::Runnable*>* workQueue,
                               ThreadFactory* threadFactory, RejectedExecutionHandler* handler) :
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
    coreThreadsCanTimeout(false),
    workQueue(),
    mainLock(),
    termination(1),
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
                addWorker();
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
                addWorker();
            }

            // queue the new work.
            if(isStoppedOrStopping() || !this->workQueue->offer(task)) {
                this->rejectionHandler->rejectedExecution(task, this->parent);
            }
        }
    }
    DECAF_CATCH_RETHROW( Exception )
    DECAF_CATCHALL_THROW( Exception )
}

////////////////////////////////////////////////////////////////////////////////
Runnable* ExecutorKernel::deQueueTask() {

    try{

        Runnable* task = NULL;

        while(true) {

            // TODO - Threads aren't interruptible yet, so spin wait.
            if(workQueue->poll(task, 10, TimeUnit::MILLISECONDS)) {
                break;
            }

            if(isStoppedOrStopping() && workQueue->isEmpty()) {
                break;
            }
        }

        if(isStoppedOrStopping() && task == NULL) {
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
bool ExecutorKernel::addWorker() {

    try{

        if( this->workers.size() >= this->maxPoolSize ) {
            return false;
        }

        synchronized( &mainLock ) {
            Worker* newWorker = new Worker(this);
            this->workers.add(newWorker);
            freeThreads.incrementAndGet();
            newWorker->start();
            this->largestPoolSize++;
        }

        return true;
    }
    DECAF_CATCH_RETHROW( lang::Exception )
    DECAF_CATCHALL_THROW( lang::Exception )
}

////////////////////////////////////////////////////////////////////////////////
int ExecutorKernel::addAllWorkers() {

    try{

        if( this->workers.size() >= this->maxPoolSize ) {
            return 0;
        }

        int delta = 0;

        synchronized( &mainLock ) {

            delta = this->maxPoolSize - this->workers.size();

            for(int i = 0; i < delta; ++i) {
                Worker* newWorker = new Worker(this);
                this->workers.add(newWorker);
                freeThreads.incrementAndGet();
                newWorker->start();
                this->largestPoolSize++;
            }
        }

        return delta;
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

            // TODO - When threads are interruptible, we need to interrupt the Queue.
            //synchronized( workQueue.get() ) {
            //    // Signal the Queue so that all waiters are notified
            //    workQueue->notifyAll();
            //}
        }

        this->tryTerminate();
        this->stopped.set(true);
    }
}

////////////////////////////////////////////////////////////////////////////////
void ExecutorKernel::shutdownNow(ArrayList<Runnable*>& unexecutedTasks) {

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

            this->drainQueue(unexecutedTasks);
        }

        this->tryTerminate();
        this->stopped.set(true);
    }
}

////////////////////////////////////////////////////////////////////////////////
void ExecutorKernel::drainQueue(ArrayList<Runnable*>& unexecutedTasks) {

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
