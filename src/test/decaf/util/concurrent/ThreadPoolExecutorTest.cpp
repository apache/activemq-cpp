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
        int value;

        MyTask(CountDownLatch* latch, int x) : Runnable(), latch(latch), value(x) {
        }

        virtual ~MyTask() {}

        virtual void run() {
            value += 100;
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

}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testSimpleTasks()
{
    CountDownLatch myLatch( 3 );

    MyTask task1(&myLatch, 1);
    MyTask task2(&myLatch, 2);
    MyTask task3(&myLatch, 3);

    ThreadPoolExecutor pool(1, 3, 5, TimeUnit::SECONDS, new LinkedBlockingQueue<Runnable*>());

    pool.execute(&task1);
    pool.execute(&task2);
    pool.execute(&task3);

    // Wait for them to finish, if we can't do this in 30 seconds then
    // there's probably something really wrong.
    CPPUNIT_ASSERT( myLatch.await( 30000 ) );

    CPPUNIT_ASSERT( task1.value == 101 );
    CPPUNIT_ASSERT( task2.value == 102 );
    CPPUNIT_ASSERT( task3.value == 103 );

    CPPUNIT_ASSERT( pool.getMaximumPoolSize() == 3 );

    pool.shutdown();
}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolExecutorTest::testMoreTasksThanMaxPoolSize() {

    ThreadPoolExecutor pool(1, 3, 5, TimeUnit::SECONDS, new LinkedBlockingQueue<Runnable*>());
    Mutex myMutex;

    CPPUNIT_ASSERT( pool.getMaximumPoolSize() == 3);

    CountDownLatch startedLatch1(3);  // First three should go right away
    CountDownLatch startedLatch2(1);  // The fourth one goes after others finish

    MyWaitingTask task1( &myMutex, &startedLatch1 );
    MyWaitingTask task2( &myMutex, &startedLatch1 );
    MyWaitingTask task3( &myMutex, &startedLatch1 );
    MyWaitingTask task4( &myMutex, &startedLatch2 );

    pool.execute( &task1 );
    pool.execute( &task2 );
    pool.execute( &task3 );
    pool.execute( &task4 );

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

    MyTask task1(&myLatch, 1);
    MyTask task2(&myLatch, 2);
    MyTask task3(&myLatch, 3);

    MyExceptionTask exTask1;
    MyExceptionTask exTask2;
    MyExceptionTask exTask3;

    ThreadPoolExecutor pool(1, 3, 5, TimeUnit::SECONDS, new LinkedBlockingQueue<Runnable*>());

    pool.execute(&exTask1);
    pool.execute(&task2);
    pool.execute(&exTask2);
    pool.execute(&task1);
    pool.execute(&exTask3);
    pool.execute(&task3);

    // Wait for them to finish, if we can't do this in 30 seconds then
    // there's probably something really wrong.
    CPPUNIT_ASSERT( myLatch.await( 30000 ) );

    CPPUNIT_ASSERT( task1.value == 101 );
    CPPUNIT_ASSERT( task2.value == 102 );
    CPPUNIT_ASSERT( task3.value == 103 );

    CPPUNIT_ASSERT( pool.getMaximumPoolSize() == 3 );

    pool.shutdown();
}
