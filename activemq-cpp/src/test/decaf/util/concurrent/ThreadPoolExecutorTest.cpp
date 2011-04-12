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

#include "ThreadPoolExecutorTest.h"

#include <decaf/util/concurrent/ThreadPoolExecutor.h>
#include <decaf/util/concurrent/LinkedBlockingQueue.h>

#include <decaf/lang/exceptions/RuntimeException.h>

using namespace std;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::util;
using namespace decaf::util::concurrent;

///////////////////////////////////////////////////////////////////////////////
namespace {

    class MyTask : public lang::Runnable {
    public:

        CountDownLatch* latch;
        int* value;

        MyTask(CountDownLatch* latch, int* x) : Runnable(), latch(latch), value(x) {
        }

        virtual ~MyTask() {}

        virtual void run() {
            *value += 100;
            Thread::sleep(10);
            latch->countDown();
        }
    };

    class MyExceptionTask : public lang::Runnable {
    public:

        int value;

        MyExceptionTask() : Runnable() {
        }

        virtual ~MyExceptionTask() {}

        virtual void run() {
            throw RuntimeException();
        }
    };

    class MyWaitingTask : public lang::Runnable {
    public:

        Mutex* mutex;
        CountDownLatch* startedLatch;

        MyWaitingTask( Mutex* mutex, CountDownLatch* startedLatch ) {
            this->mutex = mutex;
            this->startedLatch = startedLatch;
        }

        virtual ~MyWaitingTask() {};

        virtual void run() {
            try
            {
                synchronized(mutex) {
                    startedLatch->countDown();
                    mutex->wait();
                }
            }
            catch( lang::Exception& ex ) {
                ex.setMark( __FILE__, __LINE__ );
            }
        }
    };

    class DefaultThreadFactoryRunnable : public Runnable {
    private:

        CountDownLatch* shutdown;

    public:

        DefaultThreadFactoryRunnable(CountDownLatch* shutdown) : Runnable(), shutdown(shutdown) {
        }

        virtual ~DefaultThreadFactoryRunnable() {}

        virtual void run() {
            this->shutdown->await();
        }

        void signalDone() {
            this->shutdown->countDown();
        }
    };

    class SimpleThreadFactory : public ThreadFactory{
    public:

        virtual Thread* newThread(Runnable* task) {
            return new Thread(task);
        }
    };

}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testConstructor1() {

    ThreadPoolExecutor pool(1, 3, 5, TimeUnit::SECONDS, new LinkedBlockingQueue<Runnable*>());

    CPPUNIT_ASSERT_EQUAL(1, pool.getCorePoolSize());
    CPPUNIT_ASSERT_EQUAL(3, pool.getMaximumPoolSize());
    CPPUNIT_ASSERT_EQUAL(false, pool.isShutdown());
    CPPUNIT_ASSERT_EQUAL(false, pool.isTerminated());
}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testSimpleTasks()
{
    CountDownLatch myLatch( 3 );

    int taskValue1 = 1;
    int taskValue2 = 2;
    int taskValue3 = 3;

    MyTask* task1 = new MyTask(&myLatch, &taskValue1);
    MyTask* task2 = new MyTask(&myLatch, &taskValue2);
    MyTask* task3 = new MyTask(&myLatch, &taskValue3);

    ThreadPoolExecutor pool(1, 3, 5, TimeUnit::SECONDS, new LinkedBlockingQueue<Runnable*>());

    pool.execute(task1);
    pool.execute(task2);
    pool.execute(task3);

    // Wait for them to finish, if we can't do this in 30 seconds then
    // there's probably something really wrong.
    CPPUNIT_ASSERT( myLatch.await( 30000 ) );

    CPPUNIT_ASSERT( taskValue1 == 101 );
    CPPUNIT_ASSERT( taskValue2 == 102 );
    CPPUNIT_ASSERT( taskValue3 == 103 );

    CPPUNIT_ASSERT( pool.getMaximumPoolSize() == 3 );

    pool.shutdown();
}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testAwaitTermination()
{
    CountDownLatch myLatch( 3 );

    int taskValue1 = 1;
    int taskValue2 = 2;
    int taskValue3 = 3;

    MyTask* task1 = new MyTask(&myLatch, &taskValue1);
    MyTask* task2 = new MyTask(&myLatch, &taskValue2);
    MyTask* task3 = new MyTask(&myLatch, &taskValue3);

    ThreadPoolExecutor pool(1, 3, 5, TimeUnit::SECONDS, new LinkedBlockingQueue<Runnable*>());

    pool.execute(task1);
    pool.execute(task2);
    pool.execute(task3);

    // Wait for them to finish, if we can't do this in 30 seconds then
    // there's probably something really wrong.
    CPPUNIT_ASSERT( myLatch.await( 30000 ) );

    CPPUNIT_ASSERT( taskValue1 == 101 );
    CPPUNIT_ASSERT( taskValue2 == 102 );
    CPPUNIT_ASSERT( taskValue3 == 103 );

    CPPUNIT_ASSERT( pool.getMaximumPoolSize() == 3 );

    CPPUNIT_ASSERT_EQUAL(false, pool.isShutdown());
    CPPUNIT_ASSERT_EQUAL(false, pool.isTerminated());

    pool.shutdown();
    CPPUNIT_ASSERT_EQUAL(true, pool.isShutdown());
    CPPUNIT_ASSERT(pool.awaitTermination(30, TimeUnit::SECONDS));

    CPPUNIT_ASSERT_EQUAL(true, pool.isShutdown());
    CPPUNIT_ASSERT_EQUAL(true, pool.isTerminated());
}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testMoreTasksThanMaxPoolSize() {

    ThreadPoolExecutor pool(1, 3, 5, TimeUnit::SECONDS, new LinkedBlockingQueue<Runnable*>());

    CPPUNIT_ASSERT( pool.getMaximumPoolSize() == 3);

    CountDownLatch startedLatch1(3);  // First three should go right away
    CountDownLatch startedLatch2(1);  // The fourth one goes after others finish

    MyWaitingTask* task1 = new MyWaitingTask( &myMutex, &startedLatch1 );
    MyWaitingTask* task2 = new MyWaitingTask( &myMutex, &startedLatch1 );
    MyWaitingTask* task3 = new MyWaitingTask( &myMutex, &startedLatch1 );
    MyWaitingTask* task4 = new MyWaitingTask( &myMutex, &startedLatch2 );

    pool.execute(task1);
    pool.execute(task2);
    pool.execute(task3);
    pool.execute(task4);

    // Wait 30 seconds, then we let it fail because something is
    // probably very wrong.
    CPPUNIT_ASSERT( startedLatch1.await( 30000 ) );

    Thread::sleep(10);

    // Wake up the tasks.
    synchronized(&myMutex) {
        myMutex.notifyAll();
    }

    // Wait 30 seconds, then we let it fail because something is
    // probably very wrong.
    CPPUNIT_ASSERT( startedLatch2.await( 30000 ) );

    Thread::sleep(10);

    // Wake up the last task.
    synchronized(&myMutex) {
        myMutex.notifyAll();
    }

    // Wait for them to finish, if it takes longer than 30 seconds
    // something is not right.
    CPPUNIT_ASSERT( startedLatch2.await( 30000 ) );
}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testTasksThatThrow()
{
    CountDownLatch myLatch( 3 );

    int taskValue1 = 1;
    int taskValue2 = 2;
    int taskValue3 = 3;

    MyTask* task1 = new MyTask(&myLatch, &taskValue1);
    MyTask* task2 = new MyTask(&myLatch, &taskValue2);
    MyTask* task3 = new MyTask(&myLatch, &taskValue3);

    MyExceptionTask* exTask1 = new MyExceptionTask;
    MyExceptionTask* exTask2 = new MyExceptionTask;
    MyExceptionTask* exTask3 = new MyExceptionTask;

    ThreadPoolExecutor pool(1, 3, 5, TimeUnit::SECONDS, new LinkedBlockingQueue<Runnable*>());

    pool.execute(exTask1);
    pool.execute(task2);
    pool.execute(exTask2);
    pool.execute(task1);
    pool.execute(exTask3);
    pool.execute(task3);

    // Wait for them to finish, if we can't do this in 30 seconds then
    // there's probably something really wrong.
    CPPUNIT_ASSERT( myLatch.await( 30000 ) );

    CPPUNIT_ASSERT( taskValue1 == 101 );
    CPPUNIT_ASSERT( taskValue2 == 102 );
    CPPUNIT_ASSERT( taskValue3 == 103 );

    CPPUNIT_ASSERT( pool.getMaximumPoolSize() == 3 );

    pool.shutdown();
}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testPrestartCoreThread() {

    ThreadPoolExecutor p2(2, 2, LONG_DELAY_MS, TimeUnit::MILLISECONDS, new LinkedBlockingQueue<Runnable*>());

    CPPUNIT_ASSERT_EQUAL(0, p2.getPoolSize());
    CPPUNIT_ASSERT(p2.prestartCoreThread());
    CPPUNIT_ASSERT_EQUAL(1, p2.getPoolSize());
    CPPUNIT_ASSERT(p2.prestartCoreThread());
    CPPUNIT_ASSERT_EQUAL(2, p2.getPoolSize());
    CPPUNIT_ASSERT(!p2.prestartCoreThread());
    CPPUNIT_ASSERT_EQUAL(2, p2.getPoolSize());

    joinPool(&p2);
}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testPrestartAllCoreThreads() {

    ThreadPoolExecutor p2(2, 2, LONG_DELAY_MS, TimeUnit::MILLISECONDS, new LinkedBlockingQueue<Runnable*>());
    CPPUNIT_ASSERT_EQUAL(0, p2.getPoolSize());
    p2.prestartAllCoreThreads();
    CPPUNIT_ASSERT_EQUAL(2, p2.getPoolSize());
    p2.prestartAllCoreThreads();
    CPPUNIT_ASSERT_EQUAL(2, p2.getPoolSize());

    joinPool(&p2);
}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testGetCompletedTaskCount() {

    ThreadPoolExecutor p2(2, 2, LONG_DELAY_MS, TimeUnit::MILLISECONDS, new LinkedBlockingQueue<Runnable*>());
    CPPUNIT_ASSERT_EQUAL(0LL, p2.getCompletedTaskCount());
    p2.execute(new ShortRunnable(this));

    try {
        Thread::sleep(SMALL_DELAY_MS);
    } catch(Exception& e){
        CPPUNIT_FAIL("Caught unknown exception");
    }

    CPPUNIT_ASSERT_EQUAL(1LL, p2.getCompletedTaskCount());
    p2.shutdown();

    joinPool(&p2);
}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testGetCorePoolSize() {

    ThreadPoolExecutor p1(1, 1, LONG_DELAY_MS, TimeUnit::MILLISECONDS, new LinkedBlockingQueue<Runnable*>());
    CPPUNIT_ASSERT_EQUAL(1, p1.getCorePoolSize());
    joinPool(&p1);
}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testGetKeepAliveTime() {

    ThreadPoolExecutor p2(2, 2, 1000, TimeUnit::MILLISECONDS, new LinkedBlockingQueue<Runnable*>());
    CPPUNIT_ASSERT_EQUAL(1LL, p2.getKeepAliveTime(TimeUnit::SECONDS));
    joinPool(&p2);
}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testGetThreadFactory() {

    ThreadFactory* tf = new SimpleThreadFactory();
    ThreadPoolExecutor p(1,2,LONG_DELAY_MS, TimeUnit::MILLISECONDS, new LinkedBlockingQueue<Runnable*>(), tf, new NoOpREHandler());
    CPPUNIT_ASSERT_EQUAL(tf, p.getThreadFactory());
    joinPool(&p);
}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testSetThreadFactory() {

    ThreadPoolExecutor p(1,2,LONG_DELAY_MS, TimeUnit::MILLISECONDS, new LinkedBlockingQueue<Runnable*>());
    ThreadFactory* tf = new SimpleThreadFactory();
    p.setThreadFactory(tf);
    CPPUNIT_ASSERT_EQUAL(tf, p.getThreadFactory());
    joinPool(&p);
}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testSetThreadFactoryNull() {

    ThreadPoolExecutor p(1,2,LONG_DELAY_MS, TimeUnit::MILLISECONDS, new LinkedBlockingQueue<Runnable*>());
    try {
        p.setThreadFactory(NULL);
        shouldThrow();
    } catch(...) {
    }

    joinPool(&p);
}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testGetRejectedExecutionHandler() {

    RejectedExecutionHandler* h = new NoOpREHandler();
    ThreadPoolExecutor p(1,2,LONG_DELAY_MS, TimeUnit::MILLISECONDS, new LinkedBlockingQueue<Runnable*>(), h);
    CPPUNIT_ASSERT_EQUAL(h, p.getRejectedExecutionHandler());
    joinPool(p);
}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testSetRejectedExecutionHandler() {

    ThreadPoolExecutor p(1,2,LONG_DELAY_MS, TimeUnit::MILLISECONDS, new LinkedBlockingQueue<Runnable*>());
    RejectedExecutionHandler* h = new NoOpREHandler();
    p.setRejectedExecutionHandler(h);
    CPPUNIT_ASSERT_EQUAL(h, p.getRejectedExecutionHandler());
    joinPool(p);
}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testSetRejectedExecutionHandlerNull() {

    ThreadPoolExecutor p(1,2,LONG_DELAY_MS, TimeUnit::MILLISECONDS, new LinkedBlockingQueue<Runnable*>());

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a NullPointerException",
        p.setRejectedExecutionHandler(NULL),
        NullPointerException );

    joinPool(p);
}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testGetLargestPoolSize() {

    ThreadPoolExecutor p2(2, 2, LONG_DELAY_MS, TimeUnit::MILLISECONDS, new LinkedBlockingQueue<Runnable*>());
    try {
        CPPUNIT_ASSERT_EQUAL(0, p2.getLargestPoolSize());
        p2.execute(new MediumRunnable(this));
        p2.execute(new MediumRunnable(this));
        Thread::sleep(SHORT_DELAY_MS);
        CPPUNIT_ASSERT_EQUAL(2, p2.getLargestPoolSize());
    } catch(Exception& e){
        unexpectedException();
    }

    joinPool(p2);
}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testGetMaximumPoolSize() {

    ThreadPoolExecutor p2(2, 2, LONG_DELAY_MS, TimeUnit::MILLISECONDS, new LinkedBlockingQueue<Runnable*>());
    CPPUNIT_ASSERT_EQUAL(2, p2.getMaximumPoolSize());
    joinPool(p2);
}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testGetPoolSize() {

    ThreadPoolExecutor p1(1, 1, LONG_DELAY_MS, TimeUnit::MILLISECONDS, new LinkedBlockingQueue<Runnable*>());
    CPPUNIT_ASSERT_EQUAL(0, p1.getPoolSize());
    p1.execute(new MediumRunnable(this));
    CPPUNIT_ASSERT_EQUAL(1, p1.getPoolSize());
    joinPool(p1);
}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testGetTaskCount() {

    ThreadPoolExecutor p1(1, 1, LONG_DELAY_MS, TimeUnit::MILLISECONDS, new LinkedBlockingQueue<Runnable*>());
    try {
        CPPUNIT_ASSERT_EQUAL(0LL, p1.getTaskCount());
        p1.execute(new MediumRunnable(this));
        Thread::sleep(SHORT_DELAY_MS);
        CPPUNIT_ASSERT_EQUAL(1LL, p1.getTaskCount());
    } catch(Exception& e){
        unexpectedException();
    }
    joinPool(p1);
}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testIsShutdown() {

    ThreadPoolExecutor p1(1, 1, LONG_DELAY_MS, TimeUnit::MILLISECONDS, new LinkedBlockingQueue<Runnable*>());
    CPPUNIT_ASSERT(!p1.isShutdown());
    p1.shutdown();
    CPPUNIT_ASSERT(p1.isShutdown());
    joinPool(p1);
}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testIsTerminated() {

    ThreadPoolExecutor p1(1, 1, LONG_DELAY_MS, TimeUnit::MILLISECONDS, new LinkedBlockingQueue<Runnable*>());
    CPPUNIT_ASSERT(!p1.isTerminated());
    try {
        p1.execute(new MediumRunnable(this));
    } catch(...) {
    }

    p1.shutdown();

    try {
        CPPUNIT_ASSERT(p1.awaitTermination(LONG_DELAY_MS, TimeUnit::MILLISECONDS));
        CPPUNIT_ASSERT(p1.isTerminated());
    } catch(Exception& e){
        unexpectedException();
    }
}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testIsTerminating() {

    ThreadPoolExecutor p1(1, 1, LONG_DELAY_MS, TimeUnit::MILLISECONDS, new LinkedBlockingQueue<Runnable*>());
    CPPUNIT_ASSERT(!p1.isTerminating());
    try {
        p1.execute(new SmallRunnable(this));
        CPPUNIT_ASSERT(!p1.isTerminating());
    } catch(...) {
    }

    p1.shutdown();

    try {
        CPPUNIT_ASSERT(p1.awaitTermination(LONG_DELAY_MS, TimeUnit::MILLISECONDS));
        CPPUNIT_ASSERT(p1.isTerminated());
        CPPUNIT_ASSERT(!p1.isTerminating());
    } catch(Exception& e){
        unexpectedException();
    }
}
