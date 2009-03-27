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

#ifndef _DECAF_UTIL_CONCURRENT_THREADPOOLTEST_H_
#define _DECAF_UTIL_CONCURRENT_THREADPOOLTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <decaf/util/concurrent/CountDownLatch.h>
#include <decaf/util/concurrent/Concurrent.h>
#include <decaf/lang/Thread.h>
#include <decaf/util/concurrent/ThreadPool.h>
#include <decaf/util/concurrent/TaskListener.h>
#include <decaf/util/concurrent/Mutex.h>
#include <decaf/util/Config.h>

namespace decaf{
namespace util{
namespace concurrent{

    class ThreadPoolTest :
        public CppUnit::TestFixture,
        public TaskListener
    {
        CPPUNIT_TEST_SUITE( ThreadPoolTest );
        CPPUNIT_TEST( test1 );
        CPPUNIT_TEST( test2 );
        CPPUNIT_TEST_SUITE_END();

        int tasksToComplete;
        int complete;
        bool caughtEx;
        CountDownLatch* latch;

    public:

        ThreadPoolTest() {
            complete = 0;
            tasksToComplete = 0;
            caughtEx = false;
            latch = NULL;
        }

        virtual ~ThreadPoolTest() {}

        virtual void onTaskComplete( lang::Runnable* task DECAF_UNUSED)
        {
            try{

                complete++;

                if( latch != NULL ) {
                    latch->countDown();
                }
            }catch( lang::Exception& ex ){
                ex.setMark( __FILE__, __LINE__ );
            }
        }

        virtual void onTaskException(
            lang::Runnable* task DECAF_UNUSED,
            lang::Exception& ex DECAF_UNUSED) {
            caughtEx = true;
        }

    public:

        class MyTask : public lang::Runnable
        {
        public:

            int value;

            MyTask( int x ) {
                value = x;
            }

            virtual ~MyTask() {};

            virtual void run(void) {
                value += 100;
            }
        };

        class MyWaitingTask : public lang::Runnable
        {
        public:

            Mutex* mutex;
            CountDownLatch* startedLatch;

            MyWaitingTask( Mutex* mutex, CountDownLatch* startedLatch ) {
                this->mutex = mutex;
                this->startedLatch = startedLatch;
            }

            virtual ~MyWaitingTask() {};

            virtual void run(void) {
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

    public:

        virtual void setUp() {
            this->latch = NULL;
        }

        virtual void test1();
        virtual void test2();

    };

}}}

#endif /*_DECAF_UTIL_CONCURRENT_THREADPOOLTEST_H_*/
