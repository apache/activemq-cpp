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

#include "TimerTest.h"

#include <string>

#include <decaf/lang/Integer.h>
#include <decaf/lang/Long.h>
#include <decaf/lang/System.h>
#include <decaf/lang/Thread.h>
#include <decaf/lang/exceptions/InterruptedException.h>
#include <decaf/util/Timer.h>
#include <decaf/util/TimerTask.h>
#include <decaf/util/concurrent/Mutex.h>
#include <decaf/util/concurrent/atomic/AtomicInteger.h>

using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace decaf::util::concurrent::atomic;

////////////////////////////////////////////////////////////////////////////////
namespace decaf{
namespace util{

    class TimerTaskReport{
    public:

        decaf::util::concurrent::atomic::AtomicInteger wasRun;
        long long startedAt;
        long long lastDelta;

        TimerTaskReport() : wasRun(), startedAt(0), lastDelta(0) {}

        void reset() {
            this->wasRun.set(0);
            this->startedAt = 0;
            this->lastDelta = 0;
        }
    };

    class TimerTestTask : public TimerTask {
    private:

        decaf::util::concurrent::Mutex* gsync;

        decaf::util::concurrent::atomic::AtomicInteger* timerCounter;

        // Pointer to a Report object to log run data
        TimerTaskReport* report;

        // Should we sleep for 200 ms each run()?
        bool sleepInRun;

        // Should we increment the timerCounter?
        bool incrementCount;

        // Should we terminate the timer at a specific timerCounter?
        int terminateCount;

        // The timer we belong to
        Timer* timer;

    private:

        TimerTestTask(const TimerTestTask&);
        TimerTestTask& operator= (const TimerTestTask&);

    public:

        TimerTestTask(AtomicInteger* counter, Mutex* gsync) : gsync(gsync),
                                                              timerCounter(counter),
                                                              report(NULL),
                                                              sleepInRun(false),
                                                              incrementCount(false),
                                                              terminateCount(-1),
                                                              timer(NULL) {
        }

        TimerTestTask(TimerTaskReport* report, AtomicInteger* counter, Mutex* gsync) :
                gsync(gsync),
                timerCounter(counter),
                report(report),
                sleepInRun(false),
                incrementCount(false),
                terminateCount(-1),
                timer(NULL) {
        }

        TimerTestTask(Timer* t, TimerTaskReport* report, AtomicInteger* counter, Mutex* gsync) :
            gsync(gsync),
            timerCounter(counter),
            report(report),
            sleepInRun(false),
            incrementCount(false),
            terminateCount(-1),
            timer(t) {
        }

        virtual ~TimerTestTask() {}

        void run() {

            if( report != NULL ) {
                this->report->wasRun.incrementAndGet();
            }

            if( incrementCount ) {
                timerCounter->incrementAndGet();
            }

            if( terminateCount == timerCounter->get() && timer != NULL ) {
                timer->cancel();
            }

            if( sleepInRun ) {
                try {
                    Thread::sleep( 200 );
                } catch( InterruptedException& e ) {}
            }

            synchronized( gsync ) {
                gsync->notify();
            }
        }

        void setSleepInRun( bool sleepInRun ) {
            this->sleepInRun = sleepInRun;
        }

        void setIncrementCount( bool incrementCount ) {
            this->incrementCount = incrementCount;
        }

        void setTerminateCount( int terminateCount ) {
            this->terminateCount = terminateCount;
        }
    };

    class SlowThenFastTask : public TimerTask {
    private:

        TimerTaskReport* report;

    private:

        SlowThenFastTask(const SlowThenFastTask&);
        SlowThenFastTask& operator= (const SlowThenFastTask&);

    public:

        SlowThenFastTask( TimerTaskReport* report ) : report(report) {
        }

        virtual ~SlowThenFastTask() {}

        void run() {

            if( report->wasRun.get() == 0 ) {
                report->startedAt = System::currentTimeMillis();
            }

            report->lastDelta = System::currentTimeMillis() -
                                ( report->startedAt + ( 100 * report->wasRun.get() ) );

            if( report->wasRun.incrementAndGet() == 2 ) {
                try {
                    Thread::sleep( 200 );
                } catch( InterruptedException& e ) {
                }
            }
        }

    };

}}

////////////////////////////////////////////////////////////////////////////////
void TimerTest::setUp() {
    timerCounter.set( 0 );
}

////////////////////////////////////////////////////////////////////////////////
void TimerTest::testConstructor() {

    std::auto_ptr<Timer> t;
    TimerTaskReport report;

    // Ensure a task is run
    t.reset( new Timer() );

    TimerTestTask* testTask = new TimerTestTask( &report, &this->timerCounter, &this->gsync );
    t->schedule( testTask, 200 );

    synchronized( &this->gsync ) {
        try {
            this->gsync.wait( 2000000 );
        } catch( InterruptedException& e ) {}
    }

    CPPUNIT_ASSERT_MESSAGE( "TimerTask.run() method not called after 200ms",
                            1 == report.wasRun.get() );

    t->cancel();
}

////////////////////////////////////////////////////////////////////////////////
void TimerTest::testCancel() {

    std::auto_ptr<Timer> t;
    TimerTaskReport report;

    // Ensure a task throws an IllegalStateException after cancelled
    t.reset( new Timer() );

    TimerTestTask* testTask = new TimerTestTask( &report, &this->timerCounter, &this->gsync );
    t->cancel();

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Scheduling a task after Timer.cancel() should throw IllegalStateException",
        t->schedule( testTask, 100, 200 ),
        IllegalStateException );

    // unscheduled tasks must be deleted
    delete testTask;

    // Ensure a task is run but not after cancel
    t.reset( new Timer() );
    testTask = new TimerTestTask( &report, &this->timerCounter, &this->gsync );
    t->schedule( testTask, 100, 500 );
    synchronized( &this->gsync ) {
        try {
            this->gsync.wait( 1000 );
        } catch( InterruptedException& e ) {}
    }

    CPPUNIT_ASSERT_MESSAGE( "TimerTask.run() method not called after 200ms",
                            1 == report.wasRun.get() );

    t->cancel();
    synchronized( &this->gsync ) {
        try {
            this->gsync.wait( 500 );
        } catch( InterruptedException& e ) {}
    }

    CPPUNIT_ASSERT_MESSAGE( "TimerTask.run() method should not have been run more than once",
                            1 == report.wasRun.get() );

    // Ensure you can call cancel more than once
    report.wasRun.set( 0 );
    t.reset( new Timer() );
    testTask = new TimerTestTask( &report, &this->timerCounter, &this->gsync );
    t->schedule( testTask, 100, 500 );
    synchronized( &this->gsync ) {
        try {
            this->gsync.wait( 500 );
        } catch( InterruptedException& e ) {}
    }

    CPPUNIT_ASSERT_MESSAGE( "TimerTask.run() method not called after 200ms",
                            1 == report.wasRun.get() );

    t->cancel();
    t->cancel();
    t->cancel();

    synchronized( &this->gsync ) {
        try {
            this->gsync.wait( 500 );
        } catch( InterruptedException& e ) {}
    }

    CPPUNIT_ASSERT_MESSAGE( "TimerTask.run() method should not have been called after cancel",
                            1 == report.wasRun.get() );

    // Ensure that a call to cancel from within a timer ensures no more
    // run
    report.wasRun.set( 0 );
    t.reset( new Timer() );
    testTask = new TimerTestTask( t.get(), &report, &this->timerCounter, &this->gsync );
    testTask->setIncrementCount( true );
    testTask->setTerminateCount( 5 ); // Terminate after 5 runs
    t->schedule( testTask, 100, 100 );
    synchronized( &this->gsync ) {
        try {
            this->gsync.wait( 200 );
            this->gsync.wait( 200 );
            this->gsync.wait( 200 );
            this->gsync.wait( 200 );
            this->gsync.wait( 200 );
            this->gsync.wait( 200 );
        } catch( InterruptedException& e ) {}
    }

    CPPUNIT_ASSERT_MESSAGE( std::string( "TimerTask.run() method should be called 5 times not " ) +
                            Integer::toString( report.wasRun.get() ),
                            5 == report.wasRun.get() );

    t->cancel();
    try {
        Thread::sleep( 200 );
    } catch( InterruptedException& e ) {}

}

////////////////////////////////////////////////////////////////////////////////
void TimerTest::testPurge() {

    std::auto_ptr<Timer> t;

    t.reset( new Timer() );
    CPPUNIT_ASSERT( 0 == t->purge() );

    std::vector< TimerTestTask* > tasks;
    tasks.resize( 100 );
    int delayTime[] = { 50, 80, 20, 70, 40, 10, 90, 30, 60 };

    int j = 0;
    for( int i = 0; i < 100; i++ ) {
        tasks[i] = new TimerTestTask(&this->timerCounter, &this->gsync);
        t->schedule( tasks[i], delayTime[j++], 200 );
        if( j == 9 ) {
            j = 0;
        }
    }

    for( int i = 0; i < 50; i++ ) {
        tasks[i]->cancel();
    }

    CPPUNIT_ASSERT( t->purge() <= 50 );
    CPPUNIT_ASSERT( 0 == t->purge() );
}

////////////////////////////////////////////////////////////////////////////////
void TimerTest::testSchedule_TimerTask_Date() {

    std::auto_ptr<Timer> t;
    TimerTaskReport report;

    // Ensure a Timer throws an IllegalStateException after cancelled
    t.reset( new Timer() );
    TimerTestTask* testTask = new TimerTestTask(&this->timerCounter, &this->gsync);
    Date d( System::currentTimeMillis() + 100 );
    t->cancel();
    bool exception = false;

    try {
        t->schedule( testTask, d );
    } catch( IllegalStateException& e ) {
        exception = true;
        // unscheduled tasks must be deleted
        delete testTask;
    }

    CPPUNIT_ASSERT_MESSAGE( "Scheduling a task after Timer.cancel() should throw exception",
                            exception == true );

    // Ensure a Timer throws an IllegalStateException if task already cancelled
    t.reset( new Timer() );
    testTask = new TimerTestTask(&this->timerCounter, &this->gsync);
    d.setTime( System::currentTimeMillis() + 100 );
    testTask->cancel();
    exception = false;
    try {
        t->schedule( testTask, d );
    } catch( IllegalStateException& e ) {
        exception = true;
        // unscheduled tasks must be deleted
        delete testTask;
    }

    CPPUNIT_ASSERT_MESSAGE( "Scheduling a task after cancelling it should throw exception",
                            exception == true );
    t->cancel();

    // Ensure a Timer throws an IllegalArgumentException if delay is negative
    t.reset( new Timer() );
    testTask = new TimerTestTask(&this->timerCounter, &this->gsync);
    d.setTime( -100 );
    exception = false;
    try {
        t->schedule( testTask, d );
    } catch( IllegalArgumentException& e ) {
        exception = true;
        // unscheduled tasks must be deleted
        delete testTask;
    }
    CPPUNIT_ASSERT_MESSAGE( "Scheduling a task with negative date should throw IllegalArgumentException",
                            exception == true );
    t->cancel();

    // Ensure a Timer throws a NullPointerException if the task is null
    t.reset( new Timer() );
    exception = false;
    d.setTime( System::currentTimeMillis() + 100 );
    try {
        t->schedule( NULL, d );
    } catch( NullPointerException& e ) {
        exception = true;
    }
    CPPUNIT_ASSERT_MESSAGE( "Scheduling a null task should throw NullPointerException",
                            exception == true );
    t->cancel();

    // Ensure proper sequence of exceptions
    t.reset( new Timer() );
    d.setTime( -100 );
    exception = false;
    try {
        t->schedule( NULL, d );
    } catch( NullPointerException& e ) {
    } catch( IllegalArgumentException& e ) {
        exception = true;
    }
    CPPUNIT_ASSERT_MESSAGE( "Scheduling a null task with negative date should throw IllegalArgumentException first",
                            exception == true );
    t->cancel();

    // Ensure a task is run
    t.reset( new Timer() );
    testTask = new TimerTestTask( &report, &this->timerCounter, &this->gsync );
    d.setTime( System::currentTimeMillis() + 200 );
    t->schedule( testTask, d );
    try {
        Thread::sleep( 400 );
    } catch( InterruptedException& e ) {}

    CPPUNIT_ASSERT_MESSAGE( "TimerTask.run() method not called after 200ms",
                            1 == report.wasRun.get() );
    t->cancel();

    // Ensure multiple tasks are run
    t.reset( new Timer() );
    testTask = new TimerTestTask(&this->timerCounter, &this->gsync);
    testTask->setIncrementCount( true );
    d.setTime( System::currentTimeMillis() + 100 );
    t->schedule( testTask, d );
    testTask = new TimerTestTask(&this->timerCounter, &this->gsync);
    testTask->setIncrementCount( true );
    d.setTime( System::currentTimeMillis() + 150 );
    t->schedule( testTask, d );
    testTask = new TimerTestTask(&this->timerCounter, &this->gsync);
    testTask->setIncrementCount( true );
    d.setTime( System::currentTimeMillis() + 70 );
    t->schedule( testTask, d );
    testTask = new TimerTestTask(&this->timerCounter, &this->gsync);
    testTask->setIncrementCount( true );
    d.setTime( System::currentTimeMillis() + 10 );
    t->schedule( testTask, d );
    try {
        Thread::sleep( 400 );
    } catch( InterruptedException& e ) {}

    CPPUNIT_ASSERT_MESSAGE( std::string( "Multiple tasks should have incremented counter 4 times not " ) +
                            Integer::toString( timerCounter.get() ), timerCounter.get() == 4 );
    t->cancel();
}

////////////////////////////////////////////////////////////////////////////////
void TimerTest::testSchedule_TimerTask_Date2() {

    std::auto_ptr<Timer> t;
    TimerTaskReport report;

    // Ensure a Timer throws an IllegalStateException after cancelled
    t.reset( new Timer() );
    Pointer<TimerTestTask> testTask( new TimerTestTask(&this->timerCounter, &this->gsync) );
    Date d( System::currentTimeMillis() + 100 );
    t->cancel();
    bool exception = false;

    try {
        t->schedule( testTask, d );
    } catch( IllegalStateException& e ) {
        exception = true;
    }

    CPPUNIT_ASSERT_MESSAGE( "Scheduling a task after Timer.cancel() should throw exception",
                            exception == true );

    // Ensure a Timer throws an IllegalStateException if task already cancelled
    t.reset( new Timer() );
    testTask.reset( new TimerTestTask(&this->timerCounter, &this->gsync));
    d.setTime( System::currentTimeMillis() + 100 );
    testTask->cancel();
    exception = false;
    try {
        t->schedule( testTask, d );
    } catch( IllegalStateException& e ) {
        exception = true;
    }

    CPPUNIT_ASSERT_MESSAGE( "Scheduling a task after cancelling it should throw exception",
                            exception == true );
    t->cancel();

    // Ensure a Timer throws an IllegalArgumentException if delay is negative
    t.reset( new Timer() );
    testTask.reset( new TimerTestTask(&this->timerCounter, &this->gsync));
    d.setTime( -100 );
    exception = false;
    try {
        t->schedule( testTask, d );
    } catch( IllegalArgumentException& e ) {
        exception = true;
    }
    CPPUNIT_ASSERT_MESSAGE( "Scheduling a task with negative date should throw IllegalArgumentException",
                            exception == true );
    t->cancel();

    // Ensure a Timer throws a NullPointerException if the task is null
    t.reset( new Timer() );
    exception = false;
    d.setTime( System::currentTimeMillis() + 100 );
    try {
        t->schedule( NULL, d );
    } catch( NullPointerException& e ) {
        exception = true;
    }
    CPPUNIT_ASSERT_MESSAGE( "Scheduling a null task should throw NullPointerException",
                            exception == true );
    t->cancel();

    // Ensure proper sequence of exceptions
    t.reset( new Timer() );
    d.setTime( -100 );
    exception = false;
    try {
        t->schedule( NULL, d );
    } catch( NullPointerException& e ) {
    } catch( IllegalArgumentException& e ) {
        exception = true;
    }
    CPPUNIT_ASSERT_MESSAGE( "Scheduling a null task with negative date should throw IllegalArgumentException first",
                            exception == true );
    t->cancel();

    // Ensure a task is run
    t.reset( new Timer() );
    testTask.reset( new TimerTestTask( &report, &this->timerCounter, &this->gsync ) );
    d.setTime( System::currentTimeMillis() + 200 );
    t->schedule( testTask, d );
    try {
        Thread::sleep( 500 );
    } catch( InterruptedException& e ) {}

    CPPUNIT_ASSERT_MESSAGE( "TimerTask.run() method not called after 200ms",
                            1 == report.wasRun.get() );
    t->cancel();

    // Ensure multiple tasks are run
    t.reset( new Timer() );
    testTask.reset( new TimerTestTask(&this->timerCounter, &this->gsync));
    testTask->setIncrementCount( true );
    d.setTime( System::currentTimeMillis() + 100 );
    t->schedule( testTask, d );
    testTask.reset( new TimerTestTask(&this->timerCounter, &this->gsync));
    testTask->setIncrementCount( true );
    d.setTime( System::currentTimeMillis() + 150 );
    t->schedule( testTask, d );
    testTask.reset( new TimerTestTask(&this->timerCounter, &this->gsync));
    testTask->setIncrementCount( true );
    d.setTime( System::currentTimeMillis() + 70 );
    t->schedule( testTask, d );
    testTask.reset( new TimerTestTask(&this->timerCounter, &this->gsync));
    testTask->setIncrementCount( true );
    d.setTime( System::currentTimeMillis() + 10 );
    t->schedule( testTask, d );
    try {
        Thread::sleep( 400 );
    } catch( InterruptedException& e ) {}

    CPPUNIT_ASSERT_MESSAGE( std::string( "Multiple tasks should have incremented counter 4 times not " ) +
                            Integer::toString( timerCounter.get() ), timerCounter.get() == 4 );
    t->cancel();
}

////////////////////////////////////////////////////////////////////////////////
void TimerTest::testSchedule_TimerTask_Long() {

    std::auto_ptr<Timer> t;
    TimerTaskReport report;

    // Ensure a Timer throws an IllegalStateException after cancelled
    t.reset( new Timer() );
    TimerTestTask* testTask = new TimerTestTask(&this->timerCounter, &this->gsync);
    t->cancel();
    bool exception = false;
    try {
        t->schedule( testTask, 100 );
    } catch( IllegalStateException& e ) {
        exception = true;
        // unscheduled tasks must be deleted
        delete testTask;
    }
    CPPUNIT_ASSERT_MESSAGE( "Scheduling a task after Timer.cancel() should throw exception",
                            exception == true );

    // Ensure a Timer throws an IllegalStateException if task already cancelled
    t.reset( new Timer() );
    testTask = new TimerTestTask(&this->timerCounter, &this->gsync);
    testTask->cancel();
    exception = false;
    try {
        t->schedule( testTask, 100 );
    } catch( IllegalStateException& e ) {
        exception = true;
        // unscheduled tasks must be deleted
        delete testTask;
    }
    CPPUNIT_ASSERT_MESSAGE( "Scheduling a task after cancelling it should throw exception",
                            exception == true );
    t->cancel();

    // Ensure a Timer throws an IllegalArgumentException if delay is negative
    t.reset( new Timer() );
    testTask = new TimerTestTask(&this->timerCounter, &this->gsync);
    exception = false;
    try {
        t->schedule( testTask, -100 );
    } catch( IllegalArgumentException e ) {
        exception = true;
        // unscheduled tasks must be deleted
        delete testTask;
    }
    CPPUNIT_ASSERT_MESSAGE( "Scheduling a task with negative delay should throw IllegalArgumentException",
                            exception == true );
    t->cancel();

    // Ensure a Timer throws a NullPointerException if the task is null
    t.reset( new Timer() );
    exception = false;
    try {
        t->schedule( NULL, 10 );
    } catch( NullPointerException e ) {
        exception = true;
    }
    CPPUNIT_ASSERT_MESSAGE( "Scheduling a null task should throw NullPointerException",
                            exception == true );
    t->cancel();

    // Ensure proper sequence of exceptions
    t.reset( new Timer() );
    exception = false;
    try {
        t->schedule( NULL, -10 );
    } catch( NullPointerException& e ) {
    } catch( IllegalArgumentException& e ) {
        exception = true;
    }
    CPPUNIT_ASSERT_MESSAGE( "Scheduling a null task with negative delays should throw IllegalArgumentException first",
                            exception == true );
    t->cancel();

    // Ensure a task is run
    t.reset( new Timer() );
    testTask = new TimerTestTask( &report, &this->timerCounter, &this->gsync );
    t->schedule( testTask, 200 );
    try {
        Thread::sleep( 400 );
    } catch( InterruptedException& e ) {
    }
    CPPUNIT_ASSERT_MESSAGE( "TimerTask.run() method not called after 200ms",
                            1 == report.wasRun.get() );
    t->cancel();

    // Ensure multiple tasks are run
    t.reset( new Timer() );
    testTask = new TimerTestTask(&this->timerCounter, &this->gsync);
    testTask->setIncrementCount( true );
    t->schedule( testTask, 100 );
    testTask = new TimerTestTask(&this->timerCounter, &this->gsync);
    testTask->setIncrementCount( true );
    t->schedule( testTask, 150 );
    testTask = new TimerTestTask(&this->timerCounter, &this->gsync);
    testTask->setIncrementCount( true );
    t->schedule( testTask, 70 );
    testTask = new TimerTestTask(&this->timerCounter, &this->gsync);
    testTask->setIncrementCount( true );
    t->schedule( testTask, 10 );
    try {
        Thread::sleep( 400 );
    } catch( InterruptedException& e ) {
    }

    CPPUNIT_ASSERT_MESSAGE( std::string( "Multiple tasks should have incremented counter 4 times not " ) +
                            Integer::toString( timerCounter.get() ), timerCounter.get() == 4 );
    t->cancel();
}

////////////////////////////////////////////////////////////////////////////////
void TimerTest::testSchedule_TimerTask_Long2() {

    std::auto_ptr<Timer> t;
    TimerTaskReport report;

    // Ensure a Timer throws an IllegalStateException after cancelled
    t.reset( new Timer() );
    Pointer<TimerTestTask> testTask( new TimerTestTask(&this->timerCounter, &this->gsync) );
    t->cancel();
    bool exception = false;
    try {
        t->schedule( testTask, 100 );
    } catch( IllegalStateException& e ) {
        exception = true;
    }
    CPPUNIT_ASSERT_MESSAGE( "Scheduling a task after Timer.cancel() should throw exception",
                            exception == true );

    // Ensure a Timer throws an IllegalStateException if task already cancelled
    t.reset( new Timer() );
    testTask.reset( new TimerTestTask(&this->timerCounter, &this->gsync) );
    testTask->cancel();
    exception = false;
    try {
        t->schedule( testTask, 100 );
    } catch( IllegalStateException& e ) {
        exception = true;
    }
    CPPUNIT_ASSERT_MESSAGE( "Scheduling a task after cancelling it should throw exception",
                            exception == true );
    t->cancel();

    // Ensure a Timer throws an IllegalArgumentException if delay is negative
    t.reset( new Timer() );
    testTask.reset( new TimerTestTask(&this->timerCounter, &this->gsync) );
    exception = false;
    try {
        t->schedule( testTask, -100 );
    } catch( IllegalArgumentException e ) {
        exception = true;
    }
    CPPUNIT_ASSERT_MESSAGE( "Scheduling a task with negative delay should throw IllegalArgumentException",
                            exception == true );
    t->cancel();

    // Ensure a Timer throws a NullPointerException if the task is null
    t.reset( new Timer() );
    exception = false;
    try {
        t->schedule( NULL, 10 );
    } catch( NullPointerException e ) {
        exception = true;
    }
    CPPUNIT_ASSERT_MESSAGE( "Scheduling a null task should throw NullPointerException",
                            exception == true );
    t->cancel();

    // Ensure proper sequence of exceptions
    t.reset( new Timer() );
    exception = false;
    try {
        t->schedule( NULL, -10 );
    } catch( NullPointerException& e ) {
    } catch( IllegalArgumentException& e ) {
        exception = true;
    }
    CPPUNIT_ASSERT_MESSAGE( "Scheduling a null task with negative delays should throw IllegalArgumentException first",
                            exception == true );
    t->cancel();

    // Ensure a task is run
    t.reset( new Timer() );
    testTask.reset( new TimerTestTask( &report, &this->timerCounter, &this->gsync ) );
    t->schedule( testTask, 200 );
    try {
        Thread::sleep( 400 );
    } catch( InterruptedException& e ) {
    }
    CPPUNIT_ASSERT_MESSAGE( "TimerTask.run() method not called after 200ms",
                            1 == report.wasRun.get() );
    t->cancel();

    // Ensure multiple tasks are run
    t.reset( new Timer() );
    testTask.reset( new TimerTestTask(&this->timerCounter, &this->gsync));
    testTask->setIncrementCount( true );
    t->schedule( testTask, 100 );
    testTask.reset( new TimerTestTask(&this->timerCounter, &this->gsync));
    testTask->setIncrementCount( true );
    t->schedule( testTask, 150 );
    testTask.reset( new TimerTestTask(&this->timerCounter, &this->gsync));
    testTask->setIncrementCount( true );
    t->schedule( testTask, 70 );
    testTask.reset( new TimerTestTask(&this->timerCounter, &this->gsync));
    testTask->setIncrementCount( true );
    t->schedule( testTask, 10 );
    try {
        Thread::sleep( 400 );
    } catch( InterruptedException& e ) {
    }

    CPPUNIT_ASSERT_MESSAGE( std::string( "Multiple tasks should have incremented counter 4 times not " ) +
                            Integer::toString( timerCounter.get() ), timerCounter.get() == 4 );
    t->cancel();
}

////////////////////////////////////////////////////////////////////////////////
void TimerTest::testSchedule_TimerTask_Long_Long() {

    std::auto_ptr<Timer> t;
    TimerTaskReport report;

    // Ensure a Timer throws an IllegalStateException after cancelled
    t.reset( new Timer() );
    TimerTestTask* testTask = new TimerTestTask(&this->timerCounter, &this->gsync);
    t->cancel();
    bool exception = false;
    try {
        t->schedule( testTask, 100, 100 );
    } catch( IllegalStateException& e ) {
        exception = true;
        // unscheduled tasks must be deleted
        delete testTask;
    }
    CPPUNIT_ASSERT_MESSAGE( "Scheduling a task after Timer.cancel() should throw exception", exception );

    // Ensure a Timer throws an IllegalStateException if task already cancelled
    t.reset( new Timer() );
    testTask = new TimerTestTask(&this->timerCounter, &this->gsync);
    testTask->cancel();
    exception = false;
    try {
        t->schedule( testTask, 100, 100 );
    } catch( IllegalStateException& e ) {
        exception = true;
        // unscheduled tasks must be deleted
        delete testTask;
    }
    CPPUNIT_ASSERT_MESSAGE( "Scheduling a task after cancelling it should throw exception", exception );
    t->cancel();

    // Ensure a Timer throws an IllegalArgumentException if delay is
    // negative
    t.reset( new Timer() );
    testTask = new TimerTestTask(&this->timerCounter, &this->gsync);
    exception = false;
    try {
        t->schedule( testTask, -100, 100 );
    } catch( IllegalArgumentException& e ) {
        exception = true;
        // unscheduled tasks must be deleted
        delete testTask;
    }
    CPPUNIT_ASSERT_MESSAGE( "Scheduling a task with negative delay should throw IllegalArgumentException", exception );
    t->cancel();

    // Ensure a Timer throws an IllegalArgumentException if period is negative
    t.reset( new Timer() );
    testTask = new TimerTestTask(&this->timerCounter, &this->gsync);
    exception = false;
    try {
        t->schedule( testTask, 100, -100 );
    } catch( IllegalArgumentException& e ) {
        exception = true;
        // unscheduled tasks must be deleted
        delete testTask;
    }
    CPPUNIT_ASSERT_MESSAGE( "Scheduling a task with negative period should throw IllegalArgumentException", exception );
    t->cancel();

    // Ensure a Timer throws an IllegalArgumentException if period is zero
    t.reset( new Timer() );
    testTask = new TimerTestTask(&this->timerCounter, &this->gsync);
    exception = false;
    try {
        t->schedule( testTask, 100, 0 );
    } catch( IllegalArgumentException& e ) {
        exception = true;
        // unscheduled tasks must be deleted
        delete testTask;
    }
    CPPUNIT_ASSERT_MESSAGE( "Scheduling a task with 0 period should throw IllegalArgumentException", exception );
    t->cancel();

    // Ensure a Timer throws a NullPointerException if the task is null
    t.reset( new Timer() );
    exception = false;
    try {
        t->schedule( NULL, 10, 10 );
    } catch( NullPointerException& e ) {
        exception = true;
    }
    CPPUNIT_ASSERT_MESSAGE( "Scheduling a null task should throw NullPointerException", exception );
    t->cancel();

    // Ensure proper sequence of exceptions
    t.reset( new Timer() );
    exception = false;
    try {
        t->schedule( NULL, -10, -10 );
    } catch( NullPointerException& e ) {
    } catch( IllegalArgumentException& e ) {
        exception = true;
    }
    CPPUNIT_ASSERT_MESSAGE( "Scheduling a null task with negative delays should throw IllegalArgumentException first", exception );
    t->cancel();

    // Ensure a task is run at least twice
    t.reset( new Timer() );
    testTask = new TimerTestTask( &report, &this->timerCounter, &this->gsync );
    t->schedule( testTask, 100, 100 );
    try {
        Thread::sleep( 400 );
    } catch( InterruptedException& e ) {
    }
    CPPUNIT_ASSERT_MESSAGE( "TimerTask.run() method should have been called at least twice (" +
                            Integer::toString( report.wasRun.get() ) + ")",
                            report.wasRun.get() >= 2 );
    t->cancel();

    // Ensure multiple tasks are run
    t.reset( new Timer() );
    testTask = new TimerTestTask(&this->timerCounter, &this->gsync);
    testTask->setIncrementCount( true );
    t->schedule( testTask, 100, 100 ); // at least 9 times
    testTask = new TimerTestTask(&this->timerCounter, &this->gsync);
    testTask->setIncrementCount( true );
    t->schedule( testTask, 200, 100 ); // at least 7 times
    testTask = new TimerTestTask(&this->timerCounter, &this->gsync);
    testTask->setIncrementCount( true );
    t->schedule( testTask, 300, 200 ); // at least 4 times
    testTask = new TimerTestTask(&this->timerCounter, &this->gsync);
    testTask->setIncrementCount( true );
    t->schedule( testTask, 100, 200 ); // at least 4 times
    try {
        Thread::sleep( 1200 ); // Allowed more room for error
    } catch( InterruptedException& e ) {
    }
    CPPUNIT_ASSERT_MESSAGE( std::string( "Multiple tasks should have incremented counter 24 times not " ) +
                            Integer::toString( timerCounter.get() ), timerCounter.get() >= 24 );
    t->cancel();
}

////////////////////////////////////////////////////////////////////////////////
void TimerTest::testSchedule_TimerTask_Long_Long2() {

    std::auto_ptr<Timer> t;
    TimerTaskReport report;

    // Ensure a Timer throws an IllegalStateException after cancelled
    t.reset( new Timer() );
    Pointer<TimerTestTask> testTask( new TimerTestTask(&this->timerCounter, &this->gsync) );
    t->cancel();
    bool exception = false;
    try {
        t->schedule( testTask, 100, 100 );
    } catch( IllegalStateException& e ) {
        exception = true;
    }
    CPPUNIT_ASSERT_MESSAGE( "Scheduling a task after Timer.cancel() should throw exception", exception );

    // Ensure a Timer throws an IllegalStateException if task already cancelled
    t.reset( new Timer() );
    testTask.reset( new TimerTestTask(&this->timerCounter, &this->gsync));
    testTask->cancel();
    exception = false;
    try {
        t->schedule( testTask, 100, 100 );
    } catch( IllegalStateException& e ) {
        exception = true;
    }
    CPPUNIT_ASSERT_MESSAGE( "Scheduling a task after cancelling it should throw exception", exception );
    t->cancel();

    // Ensure a Timer throws an IllegalArgumentException if delay is
    // negative
    t.reset( new Timer() );
    testTask.reset( new TimerTestTask(&this->timerCounter, &this->gsync) );
    exception = false;
    try {
        t->schedule( testTask, -100, 100 );
    } catch( IllegalArgumentException& e ) {
        exception = true;
    }
    CPPUNIT_ASSERT_MESSAGE( "Scheduling a task with negative delay should throw IllegalArgumentException", exception );
    t->cancel();

    // Ensure a Timer throws an IllegalArgumentException if period is negative
    t.reset( new Timer() );
    testTask.reset( new TimerTestTask(&this->timerCounter, &this->gsync) );
    exception = false;
    try {
        t->schedule( testTask, 100, -100 );
    } catch( IllegalArgumentException& e ) {
        exception = true;
    }
    CPPUNIT_ASSERT_MESSAGE( "Scheduling a task with negative period should throw IllegalArgumentException", exception );
    t->cancel();

    // Ensure a Timer throws an IllegalArgumentException if period is zero
    t.reset( new Timer() );
    testTask.reset( new TimerTestTask(&this->timerCounter, &this->gsync) );
    exception = false;
    try {
        t->schedule( testTask, 100, 0 );
    } catch( IllegalArgumentException& e ) {
        exception = true;
    }
    CPPUNIT_ASSERT_MESSAGE( "Scheduling a task with 0 period should throw IllegalArgumentException", exception );
    t->cancel();

    // Ensure a Timer throws a NullPointerException if the task is null
    t.reset( new Timer() );
    exception = false;
    try {
        t->schedule( NULL, 10, 10 );
    } catch( NullPointerException& e ) {
        exception = true;
    }
    CPPUNIT_ASSERT_MESSAGE( "Scheduling a null task should throw NullPointerException", exception );
    t->cancel();

    // Ensure proper sequence of exceptions
    t.reset( new Timer() );
    exception = false;
    try {
        t->schedule( NULL, -10, -10 );
    } catch( NullPointerException& e ) {
    } catch( IllegalArgumentException& e ) {
        exception = true;
    }
    CPPUNIT_ASSERT_MESSAGE( "Scheduling a null task with negative delays should throw IllegalArgumentException first", exception );
    t->cancel();

    // Ensure a task is run at least twice
    t.reset( new Timer() );
    testTask.reset( new TimerTestTask( &report, &this->timerCounter, &this->gsync ) );
    t->schedule( testTask, 100, 100 );
    try {
        Thread::sleep( 400 );
    } catch( InterruptedException& e ) {
    }
    CPPUNIT_ASSERT_MESSAGE( "TimerTask.run() method should have been called at least twice (" +
                            Integer::toString( report.wasRun.get() ) + ")",
                            report.wasRun.get() >= 2 );
    t->cancel();

    // Ensure multiple tasks are run
    t.reset( new Timer() );
    testTask.reset( new TimerTestTask(&this->timerCounter, &this->gsync));
    testTask->setIncrementCount( true );
    t->schedule( testTask, 100, 100 ); // at least 9 times
    testTask.reset( new TimerTestTask(&this->timerCounter, &this->gsync));
    testTask->setIncrementCount( true );
    t->schedule( testTask, 200, 100 ); // at least 7 times
    testTask.reset( new TimerTestTask(&this->timerCounter, &this->gsync));
    testTask->setIncrementCount( true );
    t->schedule( testTask, 300, 200 ); // at least 4 times
    testTask.reset( new TimerTestTask(&this->timerCounter, &this->gsync));
    testTask->setIncrementCount( true );
    t->schedule( testTask, 100, 200 ); // at least 4 times
    try {
        Thread::sleep( 1200 ); // Allowed more room for error
    } catch( InterruptedException& e ) {
    }
    CPPUNIT_ASSERT_MESSAGE( std::string( "Multiple tasks should have incremented counter 24 times not " ) +
                            Integer::toString( timerCounter.get() ), timerCounter.get() >= 24 );
    t->cancel();
}

////////////////////////////////////////////////////////////////////////////////
void TimerTest::testSchedule_TimerTask_Date_Long() {

    std::auto_ptr<Timer> t;
    TimerTaskReport report;

    // Ensure a Timer throws an IllegalStateException after cancelled
    t.reset( new Timer() );
    TimerTestTask* testTask = new TimerTestTask(&this->timerCounter, &this->gsync);
    Date d( System::currentTimeMillis() + 100 );
    t->cancel();
    bool exception = false;
    try {
        t->schedule( testTask, d, 100 );
    } catch( IllegalStateException& e ) {
        exception = true;
        // unscheduled tasks must be deleted
        delete testTask;
    }
    CPPUNIT_ASSERT_MESSAGE( "Scheduling a task after Timer.cancel() should throw exception", exception );

    // Ensure a Timer throws an IllegalStateException if task already cancelled
    t.reset( new Timer() );
    d.setTime( System::currentTimeMillis() + 100 );
    testTask = new TimerTestTask(&this->timerCounter, &this->gsync);
    testTask->cancel();
    exception = false;
    try {
        t->schedule( testTask, d, 100 );
    } catch( IllegalStateException& e ) {
        exception = true;
        // unscheduled tasks must be deleted
        delete testTask;
    }
    CPPUNIT_ASSERT_MESSAGE( "Scheduling a task after cancelling it should throw exception", exception );
    t->cancel();

    // Ensure a Timer throws an IllegalArgumentException if delay is
    // negative
    t.reset( new Timer() );
    d.setTime( -100 );
    testTask = new TimerTestTask(&this->timerCounter, &this->gsync);
    exception = false;
    try {
        t->schedule( testTask, d, 100 );
    } catch( IllegalArgumentException& e ) {
        exception = true;
        // unscheduled tasks must be deleted
        delete testTask;
    }
    CPPUNIT_ASSERT_MESSAGE( "Scheduling a task with negative delay should throw IllegalArgumentException", exception );
    t->cancel();

    // Ensure a Timer throws an IllegalArgumentException if period is
    // negative
    t.reset( new Timer() );
    d.setTime( System::currentTimeMillis() + 100 );
    testTask = new TimerTestTask(&this->timerCounter, &this->gsync);
    exception = false;
    try {
        t->schedule( testTask, d, -100 );
    } catch( IllegalArgumentException& e ) {
        exception = true;
        // unscheduled tasks must be deleted
        delete testTask;
    }
    CPPUNIT_ASSERT_MESSAGE( "Scheduling a task with negative period should throw IllegalArgumentException", exception );
    t->cancel();

    // Ensure a Timer throws a NullPointerException if the task is null
    t.reset( new Timer() );
    d.setTime( System::currentTimeMillis() + 100 );
    exception = false;
    try {
        t->schedule( NULL, d, 10 );
    } catch( NullPointerException& e ) {
        exception = true;
    }
    CPPUNIT_ASSERT_MESSAGE( "Scheduling a null task should throw NullPointerException", exception );
    t->cancel();

    // Ensure proper sequence of exceptions
    t.reset( new Timer() );
    d.setTime( -100 );
    exception = false;
    try {
        t->schedule( NULL, d, 10 );
    } catch( NullPointerException& e ) {
    } catch( IllegalArgumentException& e ) {
        exception = true;
    }
    CPPUNIT_ASSERT_MESSAGE( "Scheduling a null task with negative dates should throw IllegalArgumentException first", exception );
    t->cancel();

    // Ensure a task is run at least twice
    t.reset( new Timer() );
    d.setTime( System::currentTimeMillis() + 100 );
    testTask = new TimerTestTask( &report, &this->timerCounter, &this->gsync );
    t->schedule( testTask, d, 100 );
    try {
        Thread::sleep( 800 );
    } catch( InterruptedException& e ) {
    }
    CPPUNIT_ASSERT_MESSAGE( std::string( "TimerTask.run() method should have been called at least twice (" ) +
                            Integer::toString( report.wasRun.get() ) + ")",
                            report.wasRun.get() >= 2 );
    t->cancel();

    // Ensure multiple tasks are run
    t.reset( new Timer() );
    testTask = new TimerTestTask(&this->timerCounter, &this->gsync);
    testTask->setIncrementCount( true );
    d.setTime( System::currentTimeMillis() + 100 );
    t->schedule( testTask, d, 100 ); // at least 9 times
    testTask = new TimerTestTask(&this->timerCounter, &this->gsync);
    testTask->setIncrementCount( true );
    d.setTime( System::currentTimeMillis() + 200 );
    t->schedule( testTask, d, 100 ); // at least 7 times
    testTask = new TimerTestTask(&this->timerCounter, &this->gsync);
    testTask->setIncrementCount( true );
    d.setTime( System::currentTimeMillis() + 300 );
    t->schedule( testTask, d, 200 ); // at least 4 times
    testTask = new TimerTestTask(&this->timerCounter, &this->gsync);
    testTask->setIncrementCount( true );
    d.setTime( System::currentTimeMillis() + 100 );
    t->schedule( testTask, d, 200 ); // at least 4 times
    try {
        Thread::sleep( 3000 );
    } catch( InterruptedException& e ) {
    }
    CPPUNIT_ASSERT_MESSAGE( std::string( "Multiple tasks should have incremented counter 24 times not " ) +
                            Integer::toString( timerCounter.get() ), timerCounter.get() >= 24 );
    t->cancel();
}

////////////////////////////////////////////////////////////////////////////////
void TimerTest::testSchedule_TimerTask_Date_Long2() {

    std::auto_ptr<Timer> t;
    TimerTaskReport report;

    // Ensure a Timer throws an IllegalStateException after cancelled
    t.reset( new Timer() );
    Pointer<TimerTestTask> testTask( new TimerTestTask(&this->timerCounter, &this->gsync) );
    Date d( System::currentTimeMillis() + 100 );
    t->cancel();
    bool exception = false;
    try {
        t->schedule( testTask, d, 100 );
    } catch( IllegalStateException& e ) {
        exception = true;
    }
    CPPUNIT_ASSERT_MESSAGE( "Scheduling a task after Timer.cancel() should throw exception", exception );

    // Ensure a Timer throws an IllegalStateException if task already cancelled
    t.reset( new Timer() );
    d.setTime( System::currentTimeMillis() + 100 );
    testTask.reset( new TimerTestTask(&this->timerCounter, &this->gsync));
    testTask->cancel();
    exception = false;
    try {
        t->schedule( testTask, d, 100 );
    } catch( IllegalStateException& e ) {
        exception = true;
    }
    CPPUNIT_ASSERT_MESSAGE( "Scheduling a task after cancelling it should throw exception", exception );
    t->cancel();

    // Ensure a Timer throws an IllegalArgumentException if delay is
    // negative
    t.reset( new Timer() );
    d.setTime( -100 );
    testTask.reset( new TimerTestTask(&this->timerCounter, &this->gsync) );
    exception = false;
    try {
        t->schedule( testTask, d, 100 );
    } catch( IllegalArgumentException& e ) {
        exception = true;
    }
    CPPUNIT_ASSERT_MESSAGE( "Scheduling a task with negative delay should throw IllegalArgumentException", exception );
    t->cancel();

    // Ensure a Timer throws an IllegalArgumentException if period is
    // negative
    t.reset( new Timer() );
    d.setTime( System::currentTimeMillis() + 100 );
    testTask.reset( new TimerTestTask(&this->timerCounter, &this->gsync) );
    exception = false;
    try {
        t->schedule( testTask, d, -100 );
    } catch( IllegalArgumentException& e ) {
        exception = true;
    }
    CPPUNIT_ASSERT_MESSAGE( "Scheduling a task with negative period should throw IllegalArgumentException", exception );
    t->cancel();

    // Ensure a Timer throws a NullPointerException if the task is null
    t.reset( new Timer() );
    d.setTime( System::currentTimeMillis() + 100 );
    exception = false;
    try {
        t->schedule( NULL, d, 10 );
    } catch( NullPointerException& e ) {
        exception = true;
    }
    CPPUNIT_ASSERT_MESSAGE( "Scheduling a null task should throw NullPointerException", exception );
    t->cancel();

    // Ensure proper sequence of exceptions
    t.reset( new Timer() );
    d.setTime( -100 );
    exception = false;
    try {
        t->schedule( NULL, d, 10 );
    } catch( NullPointerException& e ) {
    } catch( IllegalArgumentException& e ) {
        exception = true;
    }
    CPPUNIT_ASSERT_MESSAGE( "Scheduling a null task with negative dates should throw IllegalArgumentException first", exception );
    t->cancel();

    // Ensure a task is run at least twice
    t.reset( new Timer() );
    d.setTime( System::currentTimeMillis() + 100 );
    testTask.reset( new TimerTestTask( &report, &this->timerCounter, &this->gsync ) );
    t->schedule( testTask, d, 100 );
    try {
        Thread::sleep( 800 );
    } catch( InterruptedException& e ) {
    }
    CPPUNIT_ASSERT_MESSAGE( std::string( "TimerTask.run() method should have been called at least twice (" ) +
                            Integer::toString( report.wasRun.get() ) + ")",
                            report.wasRun.get() >= 2 );
    t->cancel();

    // Ensure multiple tasks are run
    t.reset( new Timer() );
    testTask.reset( new TimerTestTask(&this->timerCounter, &this->gsync));
    testTask->setIncrementCount( true );
    d.setTime( System::currentTimeMillis() + 100 );
    t->schedule( testTask, d, 100 ); // at least 9 times
    testTask.reset( new TimerTestTask(&this->timerCounter, &this->gsync));
    testTask->setIncrementCount( true );
    d.setTime( System::currentTimeMillis() + 200 );
    t->schedule( testTask, d, 100 ); // at least 7 times
    testTask.reset( new TimerTestTask(&this->timerCounter, &this->gsync));
    testTask->setIncrementCount( true );
    d.setTime( System::currentTimeMillis() + 300 );
    t->schedule( testTask, d, 200 ); // at least 4 times
    testTask.reset( new TimerTestTask(&this->timerCounter, &this->gsync));
    testTask->setIncrementCount( true );
    d.setTime( System::currentTimeMillis() + 100 );
    t->schedule( testTask, d, 200 ); // at least 4 times
    try {
        Thread::sleep( 3000 );
    } catch( InterruptedException& e ) {
    }
    CPPUNIT_ASSERT_MESSAGE( std::string( "Multiple tasks should have incremented counter 24 times not " ) +
                            Integer::toString( timerCounter.get() ), timerCounter.get() >= 24 );
    t->cancel();
}

////////////////////////////////////////////////////////////////////////////////
void TimerTest::testScheduleAtFixedRate_TimerTask_Long_Long() {

    std::auto_ptr<Timer> t;
    TimerTaskReport report;

    // Ensure a Timer throws an IllegalStateException after cancelled
    t.reset( new Timer() );
    TimerTestTask* testTask = new TimerTestTask(&this->timerCounter, &this->gsync);
    t->cancel();
    bool exception = false;
    try {
        t->scheduleAtFixedRate( testTask, 100, 100 );
    } catch( IllegalStateException& e ) {
        exception = true;
        // unscheduled tasks must be deleted
        delete testTask;
    }
    CPPUNIT_ASSERT_MESSAGE( "scheduleAtFixedRate after Timer.cancel() should throw exception", exception );

    // Ensure a Timer throws an IllegalArgumentException if delay is negative
    t.reset( new Timer() );
    testTask = new TimerTestTask(&this->timerCounter, &this->gsync);
    exception = false;
    try {
        t->scheduleAtFixedRate( testTask, -100, 100 );
    } catch( IllegalArgumentException& e ) {
        exception = true;
        // unscheduled tasks must be deleted
        delete testTask;
    }
    CPPUNIT_ASSERT_MESSAGE( "scheduleAtFixedRate with negative delay should throw IllegalArgumentException",
                            exception );
    t->cancel();

    // Ensure a Timer throws an IllegalArgumentException if period is negative
    t.reset( new Timer() );
    testTask = new TimerTestTask(&this->timerCounter, &this->gsync);
    exception = false;
    try {
        t->scheduleAtFixedRate( testTask, 100, -100 );
    } catch( IllegalArgumentException& e ) {
        exception = true;
        // unscheduled tasks must be deleted
        delete testTask;
    }
    CPPUNIT_ASSERT_MESSAGE( "scheduleAtFixedRate with negative period should throw IllegalArgumentException",
                            exception );
    t->cancel();

    // Ensure a task is run at least twice
    t.reset( new Timer() );
    testTask = new TimerTestTask( &report, &this->timerCounter, &this->gsync );
    t->scheduleAtFixedRate( testTask, 100, 100 );
    try {
        Thread::sleep( 400 );
    } catch( InterruptedException& e ) {
    }
    CPPUNIT_ASSERT_MESSAGE( std::string( "TimerTask.run() method should have been called at least twice (" ) +
                            Integer::toString( report.wasRun.get() ) + ")",
                            report.wasRun.get() >= 2 );
    t->cancel();

    // Ensure multiple tasks are run
    t.reset( new Timer() );
    report.reset();
    SlowThenFastTask* slowThenFastTask = new SlowThenFastTask( &report );

    // at least 9 times even when asleep
    t->scheduleAtFixedRate( slowThenFastTask, 100, 100 );
    try {
        Thread::sleep( 1000 );
    } catch( InterruptedException& e ) {
    }
    long long lastDelta = report.lastDelta;
    CPPUNIT_ASSERT_MESSAGE( "Fixed Rate Schedule should catch up, but is off by " +
                            Long::toString( lastDelta ) + " ms",
                            lastDelta < 300 );
    t->cancel();
}

////////////////////////////////////////////////////////////////////////////////
void TimerTest::testScheduleAtFixedRate_TimerTask_Long_Long2() {

    std::auto_ptr<Timer> t;
    TimerTaskReport report;

    // Ensure a Timer throws an IllegalStateException after cancelled
    t.reset( new Timer() );
    Pointer<TimerTestTask> testTask( new TimerTestTask(&this->timerCounter, &this->gsync) );
    t->cancel();
    bool exception = false;
    try {
        t->scheduleAtFixedRate( testTask, 100, 100 );
    } catch( IllegalStateException& e ) {
        exception = true;
    }
    CPPUNIT_ASSERT_MESSAGE( "scheduleAtFixedRate after Timer.cancel() should throw exception", exception );

    // Ensure a Timer throws an IllegalArgumentException if delay is negative
    t.reset( new Timer() );
    testTask.reset( new TimerTestTask(&this->timerCounter, &this->gsync) );
    exception = false;
    try {
        t->scheduleAtFixedRate( testTask, -100, 100 );
    } catch( IllegalArgumentException& e ) {
        exception = true;
    }
    CPPUNIT_ASSERT_MESSAGE( "scheduleAtFixedRate with negative delay should throw IllegalArgumentException",
                            exception );
    t->cancel();

    // Ensure a Timer throws an IllegalArgumentException if period is negative
    t.reset( new Timer() );
    testTask.reset( new TimerTestTask(&this->timerCounter, &this->gsync) );
    exception = false;
    try {
        t->scheduleAtFixedRate( testTask, 100, -100 );
    } catch( IllegalArgumentException& e ) {
        exception = true;
    }
    CPPUNIT_ASSERT_MESSAGE( "scheduleAtFixedRate with negative period should throw IllegalArgumentException",
                            exception );
    t->cancel();

    // Ensure a task is run at least twice
    t.reset( new Timer() );
    testTask.reset( new TimerTestTask( &report, &this->timerCounter, &this->gsync ) );
    t->scheduleAtFixedRate( testTask, 100, 100 );
    try {
        Thread::sleep( 400 );
    } catch( InterruptedException& e ) {
    }
    CPPUNIT_ASSERT_MESSAGE( std::string( "TimerTask.run() method should have been called at least twice (" ) +
                            Integer::toString( report.wasRun.get() ) + ")",
                            report.wasRun.get() >= 2 );
    t->cancel();

    // Ensure multiple tasks are run
    t.reset( new Timer() );
    report.reset();
    Pointer<SlowThenFastTask> slowThenFastTask( new SlowThenFastTask( &report ) );

    // at least 9 times even when asleep
    t->scheduleAtFixedRate( slowThenFastTask, 100, 100 );
    try {
        Thread::sleep( 1000 );
    } catch( InterruptedException& e ) {
    }
    long long lastDelta = report.lastDelta;
    CPPUNIT_ASSERT_MESSAGE( "Fixed Rate Schedule should catch up, but is off by " +
                            Long::toString( lastDelta ) + " ms",
                            lastDelta < 300 );
    t->cancel();
}

////////////////////////////////////////////////////////////////////////////////
void TimerTest::testScheduleAtFixedRate_TimerTask_Date_Long() {

    std::auto_ptr<Timer> t;
    TimerTaskReport report;

    // Ensure a Timer throws an IllegalStateException after cancelled
    t.reset( new Timer() );
    TimerTestTask* testTask = new TimerTestTask(&this->timerCounter, &this->gsync);
    t->cancel();
    bool exception = false;
    Date d( System::currentTimeMillis() + 100 );
    try {
        t->scheduleAtFixedRate( testTask, d, 100 );
    } catch( IllegalStateException& e ) {
        exception = true;
        // unscheduled tasks must be deleted
        delete testTask;
    }
    CPPUNIT_ASSERT_MESSAGE( "scheduleAtFixedRate after Timer.cancel() should throw exception", exception );

    // Ensure a Timer throws an IllegalArgumentException if delay is negative
    t.reset( new Timer() );
    testTask = new TimerTestTask(&this->timerCounter, &this->gsync);
    exception = false;
    d.setTime( -100 );
    try {
        t->scheduleAtFixedRate( testTask, d, 100 );
    } catch( IllegalArgumentException& e ) {
        exception = true;
        // unscheduled tasks must be deleted
        delete testTask;
    }
    CPPUNIT_ASSERT_MESSAGE( "scheduleAtFixedRate with negative Date should throw IllegalArgumentException",
                            exception );
    t->cancel();

    // Ensure a Timer throws an IllegalArgumentException if period is negative
    t.reset( new Timer() );
    testTask = new TimerTestTask(&this->timerCounter, &this->gsync);
    exception = false;
    try {
        t->scheduleAtFixedRate( testTask, d, -100 );
    } catch( IllegalArgumentException& e ) {
        exception = true;
        // unscheduled tasks must be deleted
        delete testTask;
    }
    CPPUNIT_ASSERT_MESSAGE( "scheduleAtFixedRate with negative period should throw IllegalArgumentException",
                            exception );
    t->cancel();

    // Ensure proper sequence of exceptions
    t.reset( new Timer() );
    exception = false;
    d.setTime( -100 );
    try {
        t->scheduleAtFixedRate( NULL, d, 10 );
    } catch( NullPointerException& e ) {
    } catch( IllegalArgumentException& e ) {
        exception = true;
    }
    CPPUNIT_ASSERT_MESSAGE( "Scheduling a null task with negative date should throw IllegalArgumentException first",
                            exception );
    t->cancel();

    // Ensure proper sequence of exceptions
    t.reset( new Timer() );
    exception = false;
    try {
        t->scheduleAtFixedRate( NULL, d, -10 );
    } catch( NullPointerException& e ) {
    } catch( IllegalArgumentException& e ) {
        exception = true;
    }
    CPPUNIT_ASSERT_MESSAGE( "Scheduling a null task & negative period should throw IllegalArgumentException first",
                            exception );
    t->cancel();

    // Ensure a task is run at least twice
    t.reset( new Timer() );
    testTask = new TimerTestTask( &report, &this->timerCounter, &this->gsync );
    d.setTime( System::currentTimeMillis() + 100 );
    t->scheduleAtFixedRate( testTask, d, 100 );
    try {
        Thread::sleep( 400 );
    } catch( InterruptedException& e ) {
    }
    CPPUNIT_ASSERT_MESSAGE( std::string( "TimerTask.run() method should have been called at least twice (" ) +
                            Integer::toString( report.wasRun.get() ) + ")",
                            report.wasRun.get() >= 2 );
    t->cancel();

    // Ensure multiple tasks are run
    t.reset( new Timer() );
    report.reset();
    SlowThenFastTask* slowThenFastTask = new SlowThenFastTask( &report );
    d.setTime( System::currentTimeMillis() + 100 );

    // at least 9 times even when asleep
    t->scheduleAtFixedRate( slowThenFastTask, d, 100 );
    try {
        Thread::sleep( 1000 );
    } catch( InterruptedException& e ) {
    }
    long long lastDelta = report.lastDelta;
    CPPUNIT_ASSERT_MESSAGE( std::string( "Fixed Rate Schedule should catch up, but is off by " ) +
                            Long::toString( lastDelta ) + " ms",
                            lastDelta < 300 );
    t->cancel();
}

////////////////////////////////////////////////////////////////////////////////
void TimerTest::testScheduleAtFixedRate_TimerTask_Date_Long2() {

    std::auto_ptr<Timer> t;
    TimerTaskReport report;

    // Ensure a Timer throws an IllegalStateException after cancelled
    t.reset( new Timer() );
    Pointer<TimerTestTask> testTask( new TimerTestTask(&this->timerCounter, &this->gsync) );
    t->cancel();
    bool exception = false;
    Date d( System::currentTimeMillis() + 100 );
    try {
        t->scheduleAtFixedRate( testTask, d, 100 );
    } catch( IllegalStateException& e ) {
        exception = true;
    }
    CPPUNIT_ASSERT_MESSAGE( "scheduleAtFixedRate after Timer.cancel() should throw exception", exception );

    // Ensure a Timer throws an IllegalArgumentException if delay is negative
    t.reset( new Timer() );
    testTask.reset( new TimerTestTask(&this->timerCounter, &this->gsync) );
    exception = false;
    d.setTime( -100 );
    try {
        t->scheduleAtFixedRate( testTask, d, 100 );
    } catch( IllegalArgumentException& e ) {
        exception = true;
    }
    CPPUNIT_ASSERT_MESSAGE( "scheduleAtFixedRate with negative Date should throw IllegalArgumentException",
                            exception );
    t->cancel();

    // Ensure a Timer throws an IllegalArgumentException if period is negative
    t.reset( new Timer() );
    testTask.reset( new TimerTestTask(&this->timerCounter, &this->gsync) );
    exception = false;
    try {
        t->scheduleAtFixedRate( testTask, d, -100 );
    } catch( IllegalArgumentException& e ) {
        exception = true;
    }
    CPPUNIT_ASSERT_MESSAGE( "scheduleAtFixedRate with negative period should throw IllegalArgumentException",
                            exception );
    t->cancel();

    // Ensure proper sequence of exceptions
    t.reset( new Timer() );
    exception = false;
    d.setTime( -100 );
    try {
        t->scheduleAtFixedRate( NULL, d, 10 );
    } catch( NullPointerException& e ) {
    } catch( IllegalArgumentException& e ) {
        exception = true;
    }
    CPPUNIT_ASSERT_MESSAGE( "Scheduling a null task with negative date should throw IllegalArgumentException first",
                            exception );
    t->cancel();

    // Ensure proper sequence of exceptions
    t.reset( new Timer() );
    exception = false;
    try {
        t->scheduleAtFixedRate( NULL, d, -10 );
    } catch( NullPointerException& e ) {
    } catch( IllegalArgumentException& e ) {
        exception = true;
    }
    CPPUNIT_ASSERT_MESSAGE( "Scheduling a null task & negative period should throw IllegalArgumentException first",
                            exception );
    t->cancel();

    // Ensure a task is run at least twice
    t.reset( new Timer() );
    testTask.reset( new TimerTestTask( &report, &this->timerCounter, &this->gsync ) );
    d.setTime( System::currentTimeMillis() + 100 );
    t->scheduleAtFixedRate( testTask, d, 100 );
    try {
        Thread::sleep( 400 );
    } catch( InterruptedException& e ) {
    }
    CPPUNIT_ASSERT_MESSAGE( std::string( "TimerTask.run() method should have been called at least twice (" ) +
                            Integer::toString( report.wasRun.get() ) + ")",
                            report.wasRun.get() >= 2 );
    t->cancel();

    // Ensure multiple tasks are run
    t.reset( new Timer() );
    report.reset();
    Pointer<SlowThenFastTask> slowThenFastTask( new SlowThenFastTask( &report ) );
    d.setTime( System::currentTimeMillis() + 100 );

    // at least 9 times even when asleep
    t->scheduleAtFixedRate( slowThenFastTask, d, 100 );
    try {
        Thread::sleep( 1000 );
    } catch( InterruptedException& e ) {
    }
    long long lastDelta = report.lastDelta;
    CPPUNIT_ASSERT_MESSAGE( std::string( "Fixed Rate Schedule should catch up, but is off by " ) +
                            Long::toString( lastDelta ) + " ms",
                            lastDelta < 300 );
    t->cancel();
}
