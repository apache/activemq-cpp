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

#ifndef THREADPOOLTEST_H_
#define THREADPOOLTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <activemq/concurrent/CountDownLatch.h>
#include <activemq/concurrent/Concurrent.h>
#include <activemq/concurrent/Thread.h>
#include <activemq/concurrent/ThreadPool.h>
#include <activemq/concurrent/TaskListener.h>
#include <activemq/concurrent/Mutex.h>
#include <activemq/util/Config.h>

namespace activemq{
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
        Mutex mutex;
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

        virtual void onTaskComplete(Runnable* task AMQCPP_UNUSED)
        {
            try{

                complete++;

                if( latch != NULL ) {
                    latch->countDown();
                }
            }catch( exceptions::ActiveMQException& ex ){
                ex.setMark( __FILE__, __LINE__ );
            }
        }

        virtual void onTaskException(Runnable* task AMQCPP_UNUSED,
            exceptions::ActiveMQException& ex AMQCPP_UNUSED) {
            caughtEx = true;
        }

    public:

        class MyTask : public Runnable
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

        class MyWaitingTask : public Runnable
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
                catch( exceptions::ActiveMQException& ex ) {
                    ex.setMark( __FILE__, __LINE__ );
                }
            }
        };

    public:

        virtual void test1();
        virtual void test2();

    };

}}

#endif /*THREADPOOLTEST_H_*/
