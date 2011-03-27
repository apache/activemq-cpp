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
#include <decaf/util/concurrent/Concurrent.h>
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

////////////////////////////////////////////////////////////////////////////////
namespace decaf{
namespace util{
namespace concurrent{

    class Worker;

    class ExecutorKernel {
    public:

        LinkedList<Worker*> workers;
        AtomicBoolean stopping;
        AtomicBoolean stopped;
        std::size_t freeThreads;

        int maxPoolSize;
        int corePoolSize;
        long long keepAliveTime;
        Pointer< BlockingQueue<decaf::lang::Runnable*> > workQueue;
        Mutex mainLock;

    public:

        ExecutorKernel(int corePoolSize, int maxPoolSize, long long keepAliveTime,
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
        }

        void stop() {
            this->done = true;
        }

        bool isBusy() {
            return this->busy;
        }

    };

}}}

////////////////////////////////////////////////////////////////////////////////
ThreadPoolExecutor::ThreadPoolExecutor(int corePoolSize, int maxPoolSize,
                                       long long keepAliveTime, const TimeUnit& unit,
                                       BlockingQueue<decaf::lang::Runnable*>* workQueue) :
    kernel(new ExecutorKernel(corePoolSize, maxPoolSize, unit.toMillis(keepAliveTime), workQueue)) {
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
int ThreadPoolExecutor::getPoolSize() const {
    return (int)this->kernel->workers.size();
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
ExecutorKernel::ExecutorKernel(int corePoolSize, int maxPoolSize, long long keepAliveTime,
                               BlockingQueue<decaf::lang::Runnable*>* workQueue) :
    workers(),
    stopping(false),
    stopped(false),
    freeThreads(0),
    maxPoolSize(maxPoolSize),
    corePoolSize(corePoolSize),
    keepAliveTime(keepAliveTime),
    workQueue(workQueue),
    mainLock() {

    if(corePoolSize < 0 || maxPoolSize <= 0 ||
       maxPoolSize < corePoolSize || keepAliveTime < 0) {

        throw IllegalArgumentException(__FILE__, __LINE__, "Argument out of range.");
    }

    if(workQueue == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "BlockingQueue pointer was null");
    }
}

////////////////////////////////////////////////////////////////////////////////
ExecutorKernel::~ExecutorKernel() {
    try{
        this->shutdown();
    }
    DECAF_CATCH_NOTHROW(Exception)
    DECAF_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void ExecutorKernel::onTaskStarted(Worker* thread DECAF_UNUSED) {

    try{

        synchronized( &mainLock ) {

            freeThreads--;

            // Now that this callback has decremented the free threads counter
            // lets check if there is any outstanding work to be done and no
            // threads to handle it.  This could happen if the QueueTask
            // method was called successively without any of the PooledThreads
            // having a chance to wake up and service the queue.  This would
            // cause the number of Task to exceed the number of free threads
            // once the Threads got a chance to wake up and service the queue
            if( freeThreads == 0 && !workQueue->isEmpty() ) {
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
        synchronized( &mainLock ) {
            freeThreads++;
        }
    }
    DECAF_CATCH_RETHROW( lang::Exception )
    DECAF_CATCHALL_THROW( lang::Exception )
}

////////////////////////////////////////////////////////////////////////////////
void ExecutorKernel::onTaskException(Worker* thread, lang::Exception& ex DECAF_UNUSED) {

    try{

        synchronized( &mainLock ) {

        }
    }
    DECAF_CATCH_RETHROW( Exception )
    DECAF_CATCHALL_THROW( Exception )
}

////////////////////////////////////////////////////////////////////////////////
void ExecutorKernel::enQueueTask(Runnable* task) {

    try{

        synchronized( &this->mainLock ) {

            // If there's nobody open to do work, then create some more
            // threads to handle the work.
            if( this->freeThreads == 0 ) {
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
            freeThreads++;
            newWorker->start();
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

            iter.reset(this->workers.iterator());
            while(iter->hasNext()) {
                Worker* worker = iter->next();
                worker->join();
                delete worker;
            }

            this->workers.clear();
            this->stopped.set(true);
        }
    }
}
