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

#include "ThreadTest.h"

#include <decaf/util/concurrent/Mutex.h>
#include <decaf/lang/System.h>
#include <decaf/util/Random.h>
#include <decaf/lang/exceptions/InterruptedException.h>
#include <decaf/lang/exceptions/RuntimeException.h>

#include <memory>

using namespace std;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::util;
using namespace decaf::util::concurrent;

////////////////////////////////////////////////////////////////////////////////
namespace decaf{
namespace lang{

    class SimpleThread : public Runnable {
    private:

        int delay;

    public:

        decaf::util::concurrent::Mutex lock;

        virtual void run() {
            try {

                synchronized( &lock ) {
                    lock.notify();
                    lock.wait( delay );
                }

            } catch( decaf::lang::exceptions::InterruptedException& e ) {
                return;
            }
        }

        SimpleThread( int d ) : delay( 0 ) {
            if( d >= 0 ) {
                delay = d;
            }
        }
    };

    class YieldThread : public Runnable {
    private:

        volatile int delay;

    public:

        virtual void run() {
            int x = 0;
            while( true ) {
                ++x;
            }
        }

        YieldThread( int d ) : delay( 0 ) {
            if( d >= 0 ) {
                delay = d;
            }
        }
    };

    class Delegate : public Runnable{
    private:

        int stuff;

    public:

        Delegate(){ stuff = 0; }
        virtual ~Delegate(){}

        int getStuff(){
            return stuff;
        }

        virtual void run(){
            stuff = 1;
        }

    };

    class Derived : public Thread{
    private:

        int stuff;

    public:

        Derived(){ stuff = 0; }
        virtual ~Derived(){}

        int getStuff(){
            return stuff;
        }

        virtual void run(){
            stuff = 1;
        }

    };

    class JoinTest : public Thread{
    public:

        JoinTest(){}
        virtual ~JoinTest(){}

        virtual void run(){

            // Sleep for 2 seconds.
            Thread::sleep( 2000 );
        }

    };

    class RandomSleepRun : public Thread{
    private:

        static Random rand;

    public:

        RandomSleepRun() {}
        virtual ~RandomSleepRun(){}

        virtual void run(){

            // Sleep for Random time.
            Thread::sleep( rand.nextInt( 2000 ) );
        }

    };

    Random RandomSleepRun::rand( System::currentTimeMillis() );

    class BadRunnable : public Runnable {
    public:

        BadRunnable(){}
        virtual ~BadRunnable(){}

        virtual void run(){

            Thread::sleep( 100 );
            throw RuntimeException( __FILE__, __LINE__, "Planned" );
        }

    };

    class Handler : public Thread::UncaughtExceptionHandler {
    public:

        bool executed;

        Handler() : executed( false ) {
        }

        virtual void uncaughtException( const Thread* t, const Throwable& error ) throw() {
            this->executed = true;
        }

    };

}}

////////////////////////////////////////////////////////////////////////////////
void ThreadTest::testConstructor() {

    Thread ct;
    ct.start();
    ct.join();
}

////////////////////////////////////////////////////////////////////////////////
void ThreadTest::testConstructor_1() {

    std::auto_ptr<Runnable> runnable( new SimpleThread( 10 ) );
    Thread ct( runnable.get() );
    ct.start();
    ct.join();
}

////////////////////////////////////////////////////////////////////////////////
void ThreadTest::testConstructor_2() {

    std::auto_ptr<Runnable> runnable( new SimpleThread( 10 ) );
    Thread ct( runnable.get(), "SimpleThread_1" );
    CPPUNIT_ASSERT( ct.getName() == "SimpleThread_1" );
    ct.start();
    ct.join();
}

////////////////////////////////////////////////////////////////////////////////
void ThreadTest::testConstructor_3() {

    Thread ct( "SimpleThread_1" );
    CPPUNIT_ASSERT( ct.getName() == "SimpleThread_1" );
    ct.start();
    ct.join();
}

////////////////////////////////////////////////////////////////////////////////
void ThreadTest::testDelegate(){

    Delegate test;
    int initialValue = test.getStuff();

    Thread thread( &test );
    thread.start();
    thread.join();

    int finalValue = test.getStuff();

    // The values should be different - this proves
    // that the runnable was run.
    CPPUNIT_ASSERT( finalValue != initialValue );
}

////////////////////////////////////////////////////////////////////////////////
void ThreadTest::testDerived() {

    Derived test;
    int initialValue = test.getStuff();

    test.start();
    test.join();

    int finalValue = test.getStuff();

    // The values should be different - this proves
    // that the runnable was run.
    CPPUNIT_ASSERT( finalValue != initialValue );
}

////////////////////////////////////////////////////////////////////////////////
void ThreadTest::testJoin1() {

    JoinTest test;

    // Joining a non-started thread should just return.
    CPPUNIT_ASSERT_NO_THROW( test.join() );
    CPPUNIT_ASSERT_NO_THROW( test.join( 10 ) );
    CPPUNIT_ASSERT_NO_THROW( test.join( 10, 10 ) );

    CPPUNIT_ASSERT_MESSAGE( "Thread is alive", !test.isAlive() );
    time_t startTime = time( NULL );
    test.start();
    test.join();
    time_t endTime = time( NULL );
    CPPUNIT_ASSERT_MESSAGE( "Joined Thread is still alive", !test.isAlive() );

    time_t delta = endTime - startTime;

    // Should be about 2 seconds that elapsed.
    CPPUNIT_ASSERT( delta >= 1 && delta <= 3 );

    // Thread should be able to join itself, use a timeout so we don't freeze
    Thread::currentThread()->join( 100 );
}

////////////////////////////////////////////////////////////////////////////////
void ThreadTest::testJoin2() {

    JoinTest test;

    // Joining a non-started thread should just return.
    CPPUNIT_ASSERT_NO_THROW( test.join() );
    CPPUNIT_ASSERT_NO_THROW( test.join( 10 ) );
    CPPUNIT_ASSERT_NO_THROW( test.join( 10, 10 ) );

    CPPUNIT_ASSERT_MESSAGE( "Thread is alive", !test.isAlive() );
    time_t startTime = time( NULL );
    test.start();
    test.join( 3500, 999999 );
    time_t endTime = time( NULL );
    CPPUNIT_ASSERT_MESSAGE( "Joined Thread is still alive", !test.isAlive() );

    time_t delta = endTime - startTime;

    // Should be about 2 seconds that elapsed.
    CPPUNIT_ASSERT( delta >= 1 && delta <= 3 );

    // Thread should be able to join itself, use a timeout so we don't freeze
    Thread::currentThread()->join( 100 );
}

////////////////////////////////////////////////////////////////////////////////
void ThreadTest::testJoin3() {

    JoinTest test;

    // Joining a non-started thread should just return.
    CPPUNIT_ASSERT_NO_THROW( test.join() );
    CPPUNIT_ASSERT_NO_THROW( test.join( 10 ) );
    CPPUNIT_ASSERT_NO_THROW( test.join( 10, 10 ) );

    CPPUNIT_ASSERT_MESSAGE( "Thread is alive", !test.isAlive() );
    test.start();
    test.join( 0, 999999 );
    CPPUNIT_ASSERT_MESSAGE( "Joined Thread is not still alive", test.isAlive() );
    test.join( 3500, 999999 );
    CPPUNIT_ASSERT_MESSAGE( "Joined Thread is still alive", !test.isAlive() );

    // Thread should be able to join itself, use a timeout so we don't freeze
    Thread::currentThread()->join( 100 );
}

////////////////////////////////////////////////////////////////////////////////
void ThreadTest::testJoin4() {

    // Start all the threads.
    const unsigned int numThreads = 200;
    RandomSleepRun threads[numThreads];
    for( unsigned int ix = 0; ix < numThreads; ++ix ){
        threads[ix].start();
    }

    // Join them all to ensure they complete as expected
    for( unsigned int ix = 0; ix < numThreads; ++ix ){
        threads[ix].join();
    }

    // Thread should be able to join itself, use a timeout so we don't freeze
    Thread::currentThread()->join( 100 );
}

////////////////////////////////////////////////////////////////////////////////
void ThreadTest::testSetPriority() {

    std::auto_ptr<Runnable> runnable( new SimpleThread( 10 ) );
    Thread ct( runnable.get() );
    CPPUNIT_ASSERT( ct.getPriority() == Thread::NORM_PRIORITY );
    ct.setPriority( Thread::MAX_PRIORITY );
    CPPUNIT_ASSERT( ct.getPriority() == Thread::MAX_PRIORITY );
    ct.start();
    ct.join();
}

////////////////////////////////////////////////////////////////////////////////
void ThreadTest::testIsAlive() {

    std::auto_ptr<SimpleThread> runnable( new SimpleThread( 500 ) );
    Thread ct( runnable.get() );

    CPPUNIT_ASSERT_MESSAGE( "A thread that wasn't started is alive.", !ct.isAlive() );

    synchronized( &( runnable->lock ) ) {
        ct.start();
        try {
            runnable->lock.wait();
        } catch( InterruptedException& e ) {
        }
    }

    CPPUNIT_ASSERT_MESSAGE( "Started thread returned false", ct.isAlive() );

    try {
        ct.join();
    } catch( InterruptedException& e ) {
        CPPUNIT_FAIL( "Thread did not die" );
    }

    CPPUNIT_ASSERT_MESSAGE( "Stopped thread returned true", !ct.isAlive() );
}

////////////////////////////////////////////////////////////////////////////////
void ThreadTest::testGetId() {
    CPPUNIT_ASSERT( Thread::getId() > 0 );
}

////////////////////////////////////////////////////////////////////////////////
void ThreadTest::testSleep() {

    long long startTime = 0LL;
    long long endTime = 0LL;

    try {
        startTime = System::currentTimeMillis();
        Thread::sleep( 1000 );
        endTime = System::currentTimeMillis();
    } catch( InterruptedException& e ) {
        CPPUNIT_FAIL( "Unexpected interrupt received" );
    }

    CPPUNIT_ASSERT_MESSAGE( "Failed to sleep long enough", ( endTime - startTime ) >= 800 );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IllegalArgumentException",
        Thread::sleep( -1 ),
        IllegalArgumentException );
}

////////////////////////////////////////////////////////////////////////////////
void ThreadTest::testSleep2Arg() {

    long long startTime = 0LL;
    long long endTime = 0LL;

    try {
        startTime = System::currentTimeMillis();
        Thread::sleep( 1000, 10 );
        endTime = System::currentTimeMillis();
    } catch( InterruptedException& e ) {
        CPPUNIT_FAIL( "Unexpected interrupt received" );
    }

    CPPUNIT_ASSERT_MESSAGE( "Failed to sleep long enough", ( endTime - startTime ) >= 800 );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IllegalArgumentException",
        Thread::sleep( -1, 0 ),
        IllegalArgumentException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IllegalArgumentException",
        Thread::sleep( 1000, 10000000 ),
        IllegalArgumentException );
}

////////////////////////////////////////////////////////////////////////////////
void ThreadTest::testGetState() {
    std::auto_ptr<SimpleThread> runnable( new SimpleThread( 500 ) );
    Thread ct( runnable.get() );

    CPPUNIT_ASSERT_MESSAGE( "A thread that wasn't started is alive.", !ct.isAlive() );

    synchronized( &( runnable->lock ) ) {
        ct.start();
        try {
            runnable->lock.wait();
        } catch( InterruptedException& e ) {
        }
    }

    CPPUNIT_ASSERT_MESSAGE( "Started thread returned false", ct.isAlive() );
    CPPUNIT_ASSERT( ct.getState() == Thread::RUNNABLE );

    try {
        ct.join();
    } catch( InterruptedException& e ) {
        CPPUNIT_FAIL( "Thread did not die" );
    }

    CPPUNIT_ASSERT_MESSAGE( "Stopped thread returned true", !ct.isAlive() );
    CPPUNIT_ASSERT( ct.getState() == Thread::TERMINATED );
}

////////////////////////////////////////////////////////////////////////////////
void ThreadTest::testUncaughtExceptionHandler() {

    std::auto_ptr<BadRunnable> runnable( new BadRunnable() );
    Thread t1( runnable.get() );
    Handler myHandler;

    t1.start();
    t1.join();

    Thread t2( runnable.get() );

    CPPUNIT_ASSERT( myHandler.executed == false );
    CPPUNIT_ASSERT( t2.getUncaughtExceptionHandler() == NULL );
    t2.setUncaughtExceptionHandler( &myHandler );
    CPPUNIT_ASSERT( t2.getUncaughtExceptionHandler() == &myHandler );

    t2.start();
    t2.join();

    CPPUNIT_ASSERT( myHandler.executed == true );
}

////////////////////////////////////////////////////////////////////////////////
void ThreadTest::testCurrentThread() {

    CPPUNIT_ASSERT( Thread::currentThread() != NULL );
    CPPUNIT_ASSERT( Thread::currentThread()->getName() == "Main Thread" );
    CPPUNIT_ASSERT( Thread::currentThread()->getPriority() == Thread::NORM_PRIORITY );
    CPPUNIT_ASSERT( Thread::currentThread()->getState() == Thread::RUNNABLE );

    CPPUNIT_ASSERT( Thread::currentThread() == Thread::currentThread() );
}
