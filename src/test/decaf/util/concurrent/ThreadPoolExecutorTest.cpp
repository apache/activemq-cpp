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

#include <decaf/util/Random.h>
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
    private:

        MyTask(const MyTask&);
        MyTask operator= (const MyTask&);

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
    private:

        MyExceptionTask(const MyExceptionTask&);
        MyExceptionTask operator= (const MyExceptionTask&);

    public:

        int value;

        MyExceptionTask() : Runnable(), value() {
        }

        virtual ~MyExceptionTask() {}

        virtual void run() {
            throw RuntimeException();
        }
    };

    class MyWaitingTask : public lang::Runnable {
    private:

        MyWaitingTask(const MyWaitingTask&);
        MyWaitingTask operator= (const MyWaitingTask&);

    public:

        Mutex* mutex;
        CountDownLatch* startedLatch;

        MyWaitingTask( Mutex* mutex, CountDownLatch* startedLatch ) : mutex(mutex), startedLatch(startedLatch) {
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

        DefaultThreadFactoryRunnable(const DefaultThreadFactoryRunnable&);
        DefaultThreadFactoryRunnable operator= (const DefaultThreadFactoryRunnable&);

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

    class MyThreadPoolExecutor : public ThreadPoolExecutor {
    public:

        volatile bool beforeCalled;
        volatile bool afterCalled;
        volatile bool terminatedCalled;

    public:

        MyThreadPoolExecutor() : ThreadPoolExecutor(1, 1, ThreadPoolExecutorTest::LONG_DELAY_MS, TimeUnit::MILLISECONDS, new LinkedBlockingQueue<Runnable*>()),
                                 beforeCalled(false), afterCalled(false), terminatedCalled(false) {
        }

        virtual ~MyThreadPoolExecutor() {}

    protected:

        void beforeExecute(Thread* t, Runnable* r) {
            beforeCalled = true;
        }

        void afterExecute(Runnable* r, Throwable* t) {
            afterCalled = true;
        }

        void terminated() {
            terminatedCalled = true;
        }
    };

}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testConstructor() {

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
void ThreadPoolExecutorTest::testSimpleTasksCallerOwns()
{
    CountDownLatch myLatch( 3 );

    int taskValue1 = 1;
    int taskValue2 = 2;
    int taskValue3 = 3;

    MyTask task1(&myLatch, &taskValue1);
    MyTask task2(&myLatch, &taskValue2);
    MyTask task3(&myLatch, &taskValue3);

    ThreadPoolExecutor pool(1, 3, 5, TimeUnit::SECONDS, new LinkedBlockingQueue<Runnable*>());

    pool.execute(&task1, false);
    pool.execute(&task2, false);
    pool.execute(&task3, false);

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

    ThreadPoolExecutor pool(3, 3, 5, TimeUnit::SECONDS, new LinkedBlockingQueue<Runnable*>());

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

    pool.shutdown();
    joinPool(&pool);
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

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testGetQueue() {

    BlockingQueue<Runnable*>* q = new LinkedBlockingQueue<Runnable*>();
    ThreadPoolExecutor p1(1, 1, LONG_DELAY_MS, TimeUnit::MILLISECONDS, q);
    Runnable* tasks[5];
    for (int i = 0; i < 5; i++){
        tasks[i] = new MediumPossiblyInterruptedRunnable(this);
        p1.execute(tasks[i]);
    }
    try {
        Thread::sleep(SHORT_DELAY_MS);
        BlockingQueue<Runnable*>* wq = p1.getQueue();
        CPPUNIT_ASSERT_EQUAL(q, wq);
        CPPUNIT_ASSERT(!wq->contains(tasks[0]));
        CPPUNIT_ASSERT(wq->contains(tasks[4]));
        p1.shutdown();
    } catch(Exception& e) {
        unexpectedException();
    }

    joinPool(p1);
}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testRemove() {

    BlockingQueue<Runnable*>* q = new LinkedBlockingQueue<Runnable*>();
    ThreadPoolExecutor p1(1, 1, LONG_DELAY_MS, TimeUnit::MILLISECONDS, q);
    Runnable* tasks[5];
    for (int i = 0; i < 5; i++){
        tasks[i] = new MediumPossiblyInterruptedRunnable(this);
        p1.execute(tasks[i]);
    }

    try {

        Thread::sleep(SHORT_DELAY_MS);
        CPPUNIT_ASSERT(!p1.remove(tasks[0]));
        CPPUNIT_ASSERT(q->contains(tasks[4]));
        CPPUNIT_ASSERT(q->contains(tasks[3]));
        CPPUNIT_ASSERT(p1.remove(tasks[4]));
        CPPUNIT_ASSERT(!p1.remove(tasks[4]));
        CPPUNIT_ASSERT(!q->contains(tasks[4]));
        CPPUNIT_ASSERT(q->contains(tasks[3]));
        CPPUNIT_ASSERT(p1.remove(tasks[3]));
        CPPUNIT_ASSERT(!q->contains(tasks[3]));

        delete tasks[3];
        delete tasks[4];

    } catch(Exception& e) {
        unexpectedException();
    }

    joinPool(p1);
}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testShutDownNow() {

    ThreadPoolExecutor p1(1, 1, LONG_DELAY_MS, TimeUnit::MILLISECONDS, new LinkedBlockingQueue<Runnable*>());
    ArrayList<Runnable*> list;

    try {

        for (int i = 0; i < 5; i++) {
            p1.execute(new MediumPossiblyInterruptedRunnable(this));
        }
    }
    catch(...) {
    }

    Thread::sleep(SHORT_DELAY_MS);
    list = p1.shutdownNow();

    CPPUNIT_ASSERT(p1.isShutdown());
    CPPUNIT_ASSERT(list.size() <= 4);

    Pointer< Iterator<Runnable*> > iter(list.iterator());
    while(iter->hasNext()) {
        delete iter->next();
    }
}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testConstructor1() {

    Pointer< BlockingQueue<Runnable*> > queue(new LinkedBlockingQueue<Runnable*>());

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a IllegalArgumentException",
        new ThreadPoolExecutor(-1, 1, LONG_DELAY_MS, TimeUnit::MILLISECONDS, queue.get()),
        IllegalArgumentException );
}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testConstructor2() {

    Pointer< BlockingQueue<Runnable*> > queue(new LinkedBlockingQueue<Runnable*>());

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a IllegalArgumentException",
        new ThreadPoolExecutor(1, -1, LONG_DELAY_MS, TimeUnit::MILLISECONDS, queue.get()),
        IllegalArgumentException );
}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testConstructor3() {

    Pointer< BlockingQueue<Runnable*> > queue(new LinkedBlockingQueue<Runnable*>());

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a IllegalArgumentException",
        new ThreadPoolExecutor(1, 0, LONG_DELAY_MS, TimeUnit::MILLISECONDS, queue.get()),
        IllegalArgumentException );
}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testConstructor4() {

    Pointer< BlockingQueue<Runnable*> > queue(new LinkedBlockingQueue<Runnable*>());

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a IllegalArgumentException",
        new ThreadPoolExecutor(1, 2, -1LL, TimeUnit::MILLISECONDS, queue.get()),
        IllegalArgumentException );
}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testConstructor5() {

    Pointer< BlockingQueue<Runnable*> > queue(new LinkedBlockingQueue<Runnable*>());

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a IllegalArgumentException",
        new ThreadPoolExecutor(2, 1, LONG_DELAY_MS, TimeUnit::MILLISECONDS, queue.get()),
        IllegalArgumentException );
}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testConstructor6() {

    Pointer< BlockingQueue<Runnable*> > queue(new LinkedBlockingQueue<Runnable*>());
    Pointer<ThreadFactory> factory(new SimpleThreadFactory());

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a IllegalArgumentException",
        new ThreadPoolExecutor(-1, 1, LONG_DELAY_MS, TimeUnit::MILLISECONDS, queue.get(), factory.get()),
        IllegalArgumentException );
}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testConstructor7() {

    Pointer< BlockingQueue<Runnable*> > queue(new LinkedBlockingQueue<Runnable*>());
    Pointer<ThreadFactory> factory(new SimpleThreadFactory());

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a IllegalArgumentException",
        new ThreadPoolExecutor(1, -1, LONG_DELAY_MS, TimeUnit::MILLISECONDS, queue.get(), factory.get()),
        IllegalArgumentException );
}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testConstructor8() {

    Pointer< BlockingQueue<Runnable*> > queue(new LinkedBlockingQueue<Runnable*>());
    Pointer<ThreadFactory> factory(new SimpleThreadFactory());

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a IllegalArgumentException",
        new ThreadPoolExecutor(1, 0, LONG_DELAY_MS, TimeUnit::MILLISECONDS, queue.get(), factory.get()),
        IllegalArgumentException );
}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testConstructor9() {

    Pointer< BlockingQueue<Runnable*> > queue(new LinkedBlockingQueue<Runnable*>());
    Pointer<ThreadFactory> factory(new SimpleThreadFactory());

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a IllegalArgumentException",
        new ThreadPoolExecutor(1, 2, -1LL, TimeUnit::MILLISECONDS, queue.get(), factory.get()),
        IllegalArgumentException );
}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testConstructor10() {

    Pointer< BlockingQueue<Runnable*> > queue(new LinkedBlockingQueue<Runnable*>());
    Pointer<ThreadFactory> factory(new SimpleThreadFactory());

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a IllegalArgumentException",
        new ThreadPoolExecutor(2, 1, LONG_DELAY_MS, TimeUnit::MILLISECONDS, queue.get(), factory.get()),
        IllegalArgumentException );
}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testConstructor11() {

    Pointer< BlockingQueue<Runnable*> > queue(new LinkedBlockingQueue<Runnable*>());
    Pointer<RejectedExecutionHandler> handler(new NoOpREHandler());

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a IllegalArgumentException",
        new ThreadPoolExecutor(-1, 1, LONG_DELAY_MS, TimeUnit::MILLISECONDS, queue.get(), handler.get()),
        IllegalArgumentException );
}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testConstructor12() {

    Pointer< BlockingQueue<Runnable*> > queue(new LinkedBlockingQueue<Runnable*>());
    Pointer<RejectedExecutionHandler> handler(new NoOpREHandler());

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a IllegalArgumentException",
        new ThreadPoolExecutor(1, -1, LONG_DELAY_MS, TimeUnit::MILLISECONDS, queue.get(), handler.get()),
        IllegalArgumentException );
}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testConstructor13() {

    Pointer< BlockingQueue<Runnable*> > queue(new LinkedBlockingQueue<Runnable*>());
    Pointer<RejectedExecutionHandler> handler(new NoOpREHandler());

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a IllegalArgumentException",
        new ThreadPoolExecutor(1, 0, LONG_DELAY_MS, TimeUnit::MILLISECONDS, queue.get(), handler.get()),
        IllegalArgumentException );
}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testConstructor14() {

    Pointer< BlockingQueue<Runnable*> > queue(new LinkedBlockingQueue<Runnable*>());
    Pointer<RejectedExecutionHandler> handler(new NoOpREHandler());

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a IllegalArgumentException",
        new ThreadPoolExecutor(1, 2, -1LL, TimeUnit::MILLISECONDS, queue.get(), handler.get()),
        IllegalArgumentException );
}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testConstructor15() {

    Pointer< BlockingQueue<Runnable*> > queue(new LinkedBlockingQueue<Runnable*>());
    Pointer<RejectedExecutionHandler> handler(new NoOpREHandler());

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a IllegalArgumentException",
        new ThreadPoolExecutor(2, 1, LONG_DELAY_MS, TimeUnit::MILLISECONDS, queue.get(), handler.get()),
        IllegalArgumentException );
}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testConstructor16() {

    Pointer< BlockingQueue<Runnable*> > queue(new LinkedBlockingQueue<Runnable*>());
    Pointer<ThreadFactory> factory(new SimpleThreadFactory());
    Pointer<RejectedExecutionHandler> handler(new NoOpREHandler());

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a IllegalArgumentException",
        new ThreadPoolExecutor(-1, 1, LONG_DELAY_MS, TimeUnit::MILLISECONDS, queue.get(), factory.get(), handler.get()),
        IllegalArgumentException );
}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testConstructor17() {

    Pointer< BlockingQueue<Runnable*> > queue(new LinkedBlockingQueue<Runnable*>());
    Pointer<ThreadFactory> factory(new SimpleThreadFactory());
    Pointer<RejectedExecutionHandler> handler(new NoOpREHandler());

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a IllegalArgumentException",
        new ThreadPoolExecutor(1, -1, LONG_DELAY_MS, TimeUnit::MILLISECONDS, queue.get(), factory.get(), handler.get()),
        IllegalArgumentException );
}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testConstructor18() {

    Pointer< BlockingQueue<Runnable*> > queue(new LinkedBlockingQueue<Runnable*>());
    Pointer<ThreadFactory> factory(new SimpleThreadFactory());
    Pointer<RejectedExecutionHandler> handler(new NoOpREHandler());

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a IllegalArgumentException",
        new ThreadPoolExecutor(1, 0, LONG_DELAY_MS, TimeUnit::MILLISECONDS, queue.get(), factory.get(), handler.get()),
        IllegalArgumentException );
}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testConstructor19() {

    Pointer< BlockingQueue<Runnable*> > queue(new LinkedBlockingQueue<Runnable*>());
    Pointer<ThreadFactory> factory(new SimpleThreadFactory());
    Pointer<RejectedExecutionHandler> handler(new NoOpREHandler());

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a IllegalArgumentException",
        new ThreadPoolExecutor(1, 2, -1LL, TimeUnit::MILLISECONDS, queue.get(), factory.get(), handler.get()),
        IllegalArgumentException );
}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testConstructor20() {

    Pointer< BlockingQueue<Runnable*> > queue(new LinkedBlockingQueue<Runnable*>());
    Pointer<ThreadFactory> factory(new SimpleThreadFactory());
    Pointer<RejectedExecutionHandler> handler(new NoOpREHandler());

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a IllegalArgumentException",
        new ThreadPoolExecutor(2, 1, LONG_DELAY_MS, TimeUnit::MILLISECONDS, queue.get(), factory.get(), handler.get()),
        IllegalArgumentException );
}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testConstructorNullPointerException1() {

    Pointer< BlockingQueue<Runnable*> > queue(new LinkedBlockingQueue<Runnable*>());

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a NullPointerException",
        new ThreadPoolExecutor(1, 2, LONG_DELAY_MS, TimeUnit::MILLISECONDS, NULL),
        NullPointerException );
}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testConstructorNullPointerException2() {

    Pointer< BlockingQueue<Runnable*> > queue(new LinkedBlockingQueue<Runnable*>());
    Pointer<ThreadFactory> factory(new SimpleThreadFactory());

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a NullPointerException",
        new ThreadPoolExecutor(1, 2, LONG_DELAY_MS, TimeUnit::MILLISECONDS, NULL, factory.get()),
        NullPointerException );
}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testConstructorNullPointerException3() {

    Pointer< BlockingQueue<Runnable*> > queue(new LinkedBlockingQueue<Runnable*>());

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a NullPointerException",
        new ThreadPoolExecutor(1, 2, LONG_DELAY_MS, TimeUnit::MILLISECONDS, queue.get(), (ThreadFactory*)NULL),
        NullPointerException );
}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testConstructorNullPointerException4() {

    Pointer<RejectedExecutionHandler> handler(new NoOpREHandler());

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a NullPointerException",
        new ThreadPoolExecutor(1, 2, LONG_DELAY_MS, TimeUnit::MILLISECONDS, NULL, handler.get()),
        NullPointerException );
}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testConstructorNullPointerException5() {

    Pointer< BlockingQueue<Runnable*> > queue(new LinkedBlockingQueue<Runnable*>());

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a NullPointerException",
        new ThreadPoolExecutor(1, 2, LONG_DELAY_MS, TimeUnit::MILLISECONDS, queue.get(), (RejectedExecutionHandler*)NULL),
        NullPointerException );
}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testConstructorNullPointerException6() {

    Pointer<ThreadFactory> factory(new SimpleThreadFactory());
    Pointer<RejectedExecutionHandler> handler(new NoOpREHandler());

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a NullPointerException",
        new ThreadPoolExecutor(1, 2, LONG_DELAY_MS, TimeUnit::MILLISECONDS, NULL, factory.get(), handler.get()),
        NullPointerException );
}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testConstructorNullPointerException7() {

    Pointer< BlockingQueue<Runnable*> > queue(new LinkedBlockingQueue<Runnable*>());
    Pointer<ThreadFactory> factory(new SimpleThreadFactory());

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a NullPointerException",
        new ThreadPoolExecutor(1, 2, LONG_DELAY_MS, TimeUnit::MILLISECONDS, queue.get(), factory.get(), (RejectedExecutionHandler*)NULL),
        NullPointerException );
}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testConstructorNullPointerException8() {

    Pointer< BlockingQueue<Runnable*> > queue(new LinkedBlockingQueue<Runnable*>());
    Pointer<RejectedExecutionHandler> handler(new NoOpREHandler());

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a NullPointerException",
        new ThreadPoolExecutor(1, 2, LONG_DELAY_MS, TimeUnit::MILLISECONDS, queue.get(), (ThreadFactory*)NULL, handler.get()),
        NullPointerException );
}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testSaturatedExecute1() {

    ThreadPoolExecutor p(1, 1, LONG_DELAY_MS, TimeUnit::MILLISECONDS, new LinkedBlockingQueue<Runnable*>(1));
    try {

        for(int i = 0; i < 5; ++i) {
            p.execute(new MediumRunnable(this));
        }

        shouldThrow();
    } catch(RejectedExecutionException& success) {
    }

    joinPool(p);
}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testSaturatedExecute2() {

    RejectedExecutionHandler* h = new ThreadPoolExecutor::CallerRunsPolicy();
    ThreadPoolExecutor p(1, 1, LONG_DELAY_MS, TimeUnit::MILLISECONDS, new LinkedBlockingQueue<Runnable*>(1), h);
    try {

        bool trackTokens[5] = {};
        TrackedNoOpRunnable* tasks[5];

        for(int i = 0; i < 5; ++i){
            tasks[i] = new TrackedNoOpRunnable(&trackTokens[i]);
        }

        bool longTrackedToken = false;
        TrackedLongRunnable* mr = new TrackedLongRunnable(&longTrackedToken);
        p.execute(mr);

        for(int i = 0; i < 5; ++i) {
            p.execute(tasks[i]);
        }

        for(int i = 1; i < 5; ++i) {
            CPPUNIT_ASSERT(trackTokens[i]);
        }

        destroyRemaining(p.shutdownNow());

    } catch(RejectedExecutionException& ex){
        unexpectedException();
    }

    joinPool(p);
}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testSaturatedExecute3() {

    RejectedExecutionHandler* h = new ThreadPoolExecutor::DiscardPolicy;
    ThreadPoolExecutor p(1,1, LONG_DELAY_MS, TimeUnit::MILLISECONDS, new LinkedBlockingQueue<Runnable*>(1), h);
    try {

        bool trackTokens[5] = {};
        TrackedNoOpRunnable* tasks[5];
        for(int i = 0; i < 5; ++i){
            tasks[i] = new TrackedNoOpRunnable(&trackTokens[i]);
        }

        bool longTrackedToken = false;
        p.execute(new TrackedLongRunnable(&longTrackedToken));

        for(int i = 0; i < 5; ++i) {
            p.execute(tasks[i]);
        }

        for(int i = 0; i < 5; ++i) {
            CPPUNIT_ASSERT(!trackTokens[i]);
        }

        destroyRemaining(p.shutdownNow());

    } catch(RejectedExecutionException& ex){
        unexpectedException();
    }

    joinPool(p);
}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testSaturatedExecute4() {

    RejectedExecutionHandler* h = new ThreadPoolExecutor::DiscardOldestPolicy();
    ThreadPoolExecutor p(1, 1, LONG_DELAY_MS, TimeUnit::MILLISECONDS, new LinkedBlockingQueue<Runnable*>(1), h);

    try {

        bool longTrackedToken1 = false;
        bool longTrackedToken2 = false;
        bool longTrackedToken3 = false;

        p.execute(new TrackedLongRunnable(&longTrackedToken1));
        Thread::sleep(SHORT_DELAY_MS);

        TrackedLongRunnable* r2 = new TrackedLongRunnable(&longTrackedToken2);
        p.execute(r2);

        CPPUNIT_ASSERT(p.getQueue()->contains(r2));
        TrackedNoOpRunnable* r3 = new TrackedNoOpRunnable(&longTrackedToken3);
        p.execute(r3);

        CPPUNIT_ASSERT(!p.getQueue()->contains(r2));
        CPPUNIT_ASSERT(p.getQueue()->contains(r3));

        destroyRemaining(p.shutdownNow());

    } catch(RejectedExecutionException& ex){
        unexpectedException();
    }

    joinPool(p);
}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testRejectedExecutionExceptionOnShutdown() {

    ThreadPoolExecutor tpe(1, 1, LONG_DELAY_MS, TimeUnit::MILLISECONDS, new LinkedBlockingQueue<Runnable*>(1));
    tpe.shutdown();

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a RejectedExecutionException",
        tpe.execute(new NoOpRunnable()),
        RejectedExecutionException );

    joinPool(tpe);
}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testCallerRunsOnShutdown() {

    RejectedExecutionHandler* h = new ThreadPoolExecutor::CallerRunsPolicy();
    ThreadPoolExecutor p(1, 1, LONG_DELAY_MS, TimeUnit::MILLISECONDS, new LinkedBlockingQueue<Runnable*>(1), h);

    p.shutdown();

    try {
        bool tracker = false;
        TrackedNoOpRunnable* r = new TrackedNoOpRunnable(&tracker);
        p.execute(r);
        CPPUNIT_ASSERT(!tracker);
    } catch(RejectedExecutionException& success) {
        unexpectedException();
    }

    joinPool(p);
}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testDiscardOnShutdown() {

    RejectedExecutionHandler* h = new ThreadPoolExecutor::DiscardPolicy();
    ThreadPoolExecutor p(1, 1, LONG_DELAY_MS, TimeUnit::MILLISECONDS, new LinkedBlockingQueue<Runnable*>(1), h);

    p.shutdown();

    try {
        bool tracker = false;
        TrackedNoOpRunnable* r = new TrackedNoOpRunnable(&tracker);
        p.execute(r);
        CPPUNIT_ASSERT(!tracker);
    } catch(RejectedExecutionException& success) {
        unexpectedException();
    }

    joinPool(p);
}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testDiscardOldestOnShutdown() {

    RejectedExecutionHandler* h = new ThreadPoolExecutor::DiscardOldestPolicy();
    ThreadPoolExecutor p(1, 1, LONG_DELAY_MS, TimeUnit::MILLISECONDS, new LinkedBlockingQueue<Runnable*>(1), h);

    p.shutdown();

    try {
        bool tracker = false;
        TrackedNoOpRunnable* r = new TrackedNoOpRunnable(&tracker);
        p.execute(r);
        CPPUNIT_ASSERT(!tracker);
    } catch(RejectedExecutionException& success) {
        unexpectedException();
    }

    joinPool(p);
}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testExecuteNull() {

    ThreadPoolExecutor tpe(1, 2, LONG_DELAY_MS, TimeUnit::MILLISECONDS, new LinkedBlockingQueue<Runnable*>());

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a NullPointerException",
        tpe.execute(NULL),
        NullPointerException );

    tpe.shutdown();

    joinPool(tpe);
}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testCorePoolSizeIllegalArgumentException() {

    ThreadPoolExecutor tpe(1, 2, LONG_DELAY_MS, TimeUnit::MILLISECONDS, new LinkedBlockingQueue<Runnable*>());

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a IllegalArgumentException",
        tpe.setCorePoolSize(-1),
        IllegalArgumentException );

    tpe.shutdown();

    joinPool(tpe);
}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testMaximumPoolSizeIllegalArgumentException1() {

    ThreadPoolExecutor tpe(2, 3, LONG_DELAY_MS, TimeUnit::MILLISECONDS, new LinkedBlockingQueue<Runnable*>());

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a IllegalArgumentException",
        tpe.setMaximumPoolSize(1),
        IllegalArgumentException );

    tpe.shutdown();

    joinPool(tpe);
}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testMaximumPoolSizeIllegalArgumentException2() {

    ThreadPoolExecutor tpe(2, 3, LONG_DELAY_MS, TimeUnit::MILLISECONDS, new LinkedBlockingQueue<Runnable*>());

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a IllegalArgumentException",
        tpe.setMaximumPoolSize(-1),
        IllegalArgumentException );

    tpe.shutdown();

    joinPool(tpe);
}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testKeepAliveTimeIllegalArgumentException() {

    ThreadPoolExecutor tpe(2, 3, LONG_DELAY_MS, TimeUnit::MILLISECONDS, new LinkedBlockingQueue<Runnable*>());

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a IllegalArgumentException",
        tpe.setKeepAliveTime(-1,TimeUnit::MILLISECONDS),
        IllegalArgumentException );

    tpe.shutdown();

    joinPool(tpe);
}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testTerminated() {

    MyThreadPoolExecutor tpe;

    tpe.shutdown();
    CPPUNIT_ASSERT(tpe.terminatedCalled);

    joinPool(tpe);
}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testBeforeAfter() {

    MyThreadPoolExecutor tpe;

    try {

        bool tracker = false;
        TrackedNoOpRunnable* r = new TrackedNoOpRunnable(&tracker);
        tpe.execute(r);
        Thread::sleep(SHORT_DELAY_MS);
        CPPUNIT_ASSERT(tracker);
        CPPUNIT_ASSERT(tpe.beforeCalled);
        CPPUNIT_ASSERT(tpe.afterCalled);
        tpe.shutdown();

    } catch(Exception& ex) {
        unexpectedException();
    }

    joinPool(tpe);
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class CreateSessionRunnable : public Runnable {
    private:

        Random rand;

    public:

        CreateSessionRunnable() : Runnable(), rand() {
        }

        virtual ~CreateSessionRunnable() {}

        virtual void run() {
            TimeUnit::MILLISECONDS.sleep(rand.nextInt(75));
        }
    };

    class StartStopRunnable : public Runnable {
    private:

        Random rand;

    public:

        StartStopRunnable() : Runnable(), rand() {
        }

        virtual ~StartStopRunnable() {}

        virtual void run() {
            TimeUnit::MILLISECONDS.sleep(rand.nextInt(100));
        }
    };

}

////////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testConcurrentRandomDelayedThreads() {

    ThreadPoolExecutor executor(50, Integer::MAX_VALUE, 60LL, TimeUnit::SECONDS, new LinkedBlockingQueue<Runnable*>());

    Random rand;

    for (int i = 0; i < 3000; i++) {
        executor.execute(new CreateSessionRunnable());
        executor.execute(new StartStopRunnable());
    }

    executor.shutdown();
    CPPUNIT_ASSERT_MESSAGE("executor terminated", executor.awaitTermination(45, TimeUnit::SECONDS));
}

////////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testRapidCreateAndDestroyExecutor() {

    for (int i = 0; i < 100; i++) {
        ThreadPoolExecutor executor(10, Integer::MAX_VALUE, 60LL, TimeUnit::SECONDS, new LinkedBlockingQueue<Runnable*>());
        executor.shutdown();
        CPPUNIT_ASSERT_MESSAGE("executor terminated", executor.awaitTermination(45, TimeUnit::SECONDS));
    }
}
