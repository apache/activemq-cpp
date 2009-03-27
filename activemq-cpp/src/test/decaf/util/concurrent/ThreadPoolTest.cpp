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

#include "ThreadPoolTest.h"

using namespace std;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::util;
using namespace decaf::util::concurrent;

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolTest::test1()
{
    CountDownLatch myLatch( 3 );
    this->latch = &myLatch;

    MyTask task1( 1 );
    MyTask task2( 2 );
    MyTask task3( 3 );

    this->complete = 0;
    this->tasksToComplete = 3;

    ThreadPool* pool = ThreadPool::getInstance();

    pool->setMaxThreads( ThreadPool::DEFAULT_MAX_POOL_SIZE );
    pool->setBlockSize( ThreadPool::DEFAULT_MAX_BLOCK_SIZE );

    pool->queueTask( ThreadPool::Task( &task1, this ) );
    pool->queueTask( ThreadPool::Task( &task2, this ) );
    pool->queueTask( ThreadPool::Task( &task3, this ) );

    // Wait for them to finish, if we can't do this in 30 seconds then
    // there's probably something really wrong.
    myLatch.await( 30000 );

    CPPUNIT_ASSERT( this->complete == this->tasksToComplete );

    CPPUNIT_ASSERT( task1.value == 101 );
    CPPUNIT_ASSERT( task2.value == 102 );
    CPPUNIT_ASSERT( task3.value == 103 );

    CPPUNIT_ASSERT( pool->getPoolSize() > 0 );
    CPPUNIT_ASSERT( pool->getBacklog() == 0 );

    CPPUNIT_ASSERT( pool->getMaxThreads() == ThreadPool::DEFAULT_MAX_POOL_SIZE );
    CPPUNIT_ASSERT( pool->getBlockSize() == ThreadPool::DEFAULT_MAX_BLOCK_SIZE );

    pool->setMaxThreads(50);
    pool->setBlockSize(50);

    CPPUNIT_ASSERT( pool->getMaxThreads() == 50 );
    CPPUNIT_ASSERT( pool->getBlockSize() == 50 );

    // Give it a little time to create all those threads.
    for( int i = 0; i < 1000; ++i ) {
        if( pool->getFreeThreadCount() == pool->getPoolSize() ) {
            break;
        }

        Thread::sleep( 100 );
    }

    CPPUNIT_ASSERT( pool->getFreeThreadCount() == pool->getPoolSize() );
    CPPUNIT_ASSERT( this->caughtEx == false );
}

///////////////////////////////////////////////////////////////////////////////
void ThreadPoolTest::test2() {

    try
    {
        ThreadPool pool;
        Mutex myMutex;

        CPPUNIT_ASSERT( pool.getMaxThreads() == ThreadPool::DEFAULT_MAX_POOL_SIZE );
        CPPUNIT_ASSERT( pool.getBlockSize() == ThreadPool::DEFAULT_MAX_BLOCK_SIZE );
        pool.setMaxThreads(3);
        pool.setBlockSize(1);
        CPPUNIT_ASSERT( pool.getMaxThreads() == 3 );
        CPPUNIT_ASSERT( pool.getBlockSize() == 1 );
        CPPUNIT_ASSERT( pool.getPoolSize() == 0 );
        pool.reserve( 4 );
        CPPUNIT_ASSERT( pool.getPoolSize() == 3 );
        CPPUNIT_ASSERT( pool.getFreeThreadCount() == 3 );

        CountDownLatch startedLatch1(3);  // First three should go right away
        CountDownLatch startedLatch2(1);  // The fourth one goes after others finish
        CountDownLatch doneLatch(4);      // All should be done when we are at the end.

        this->latch = &doneLatch;

        MyWaitingTask task1( &myMutex, &startedLatch1 );
        MyWaitingTask task2( &myMutex, &startedLatch1 );
        MyWaitingTask task3( &myMutex, &startedLatch1 );
        MyWaitingTask task4( &myMutex, &startedLatch2 );

        this->complete = 0;
        this->tasksToComplete = 4;

        pool.queueTask( ThreadPool::Task( &task1, this ) );
        pool.queueTask( ThreadPool::Task( &task2, this ) );
        pool.queueTask( ThreadPool::Task( &task3, this ) );
        pool.queueTask( ThreadPool::Task( &task4, this ) );

        // Wait 30 seconds, then we let it fail because something is
        // probably very wrong.
        startedLatch1.await( 30000 );

        CPPUNIT_ASSERT( pool.getFreeThreadCount() == 0 );
        CPPUNIT_ASSERT( pool.getBacklog() == 1 );

        // Wake up the tasks.
        synchronized(&myMutex) {
            myMutex.notifyAll();
        }

        // Wait 30 seconds, then we let it fail because something is
        // probably very wrong.
        startedLatch2.await( 30000 );

        // Wake up the last task.
        synchronized(&myMutex) {
            myMutex.notifyAll();
        }

        // Wait for them to finish, if it takes longer than 30 seconds
        // something is not right.
        doneLatch.await( 30000 );

        CPPUNIT_ASSERT( this->complete == this->tasksToComplete );
        CPPUNIT_ASSERT( this->caughtEx == false );
    }
    catch( lang::Exception& ex ) {
        ex.setMark( __FILE__, __LINE__ );
    }
}
