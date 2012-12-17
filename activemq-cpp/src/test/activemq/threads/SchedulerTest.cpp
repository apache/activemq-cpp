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

#include "SchedulerTest.h"

#include <activemq/threads/Scheduler.h>
#include <decaf/lang/Runnable.h>
#include <decaf/lang/Thread.h>
#include <decaf/lang/exceptions/NullPointerException.h>

using namespace std;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::util;
using namespace activemq;
using namespace activemq::threads;

////////////////////////////////////////////////////////////////////////////////
namespace {

    class CounterTask : public Runnable {
    private:

        int count;

    public:

        CounterTask() : count(0) {

        }

        virtual ~CounterTask() {}

        int getCount() const {
            return count;
        }

        virtual void run() {
            count++;
        }

    };
}

////////////////////////////////////////////////////////////////////////////////
SchedulerTest::SchedulerTest() {
}

////////////////////////////////////////////////////////////////////////////////
SchedulerTest::~SchedulerTest() {
}

////////////////////////////////////////////////////////////////////////////////
void SchedulerTest::testConstructor() {

    Scheduler scheduler("testExecutePeriodically");
    CPPUNIT_ASSERT_EQUAL(false, scheduler.isStarted());
    CPPUNIT_ASSERT_EQUAL(false, scheduler.isStopping());
    CPPUNIT_ASSERT_EQUAL(true, scheduler.isStopped());
    scheduler.start();
    CPPUNIT_ASSERT_EQUAL(true, scheduler.isStarted());
    CPPUNIT_ASSERT_EQUAL(false, scheduler.isStopping());
    CPPUNIT_ASSERT_EQUAL(false, scheduler.isStopped());
    scheduler.shutdown();
    CPPUNIT_ASSERT_EQUAL(true, scheduler.isStarted());
    CPPUNIT_ASSERT_EQUAL(false, scheduler.isStopping());
    CPPUNIT_ASSERT_EQUAL(false, scheduler.isStopped());
    scheduler.stop();
    CPPUNIT_ASSERT_EQUAL(false, scheduler.isStarted());
    CPPUNIT_ASSERT_EQUAL(false, scheduler.isStopping());
    CPPUNIT_ASSERT_EQUAL(true, scheduler.isStopped());
}

////////////////////////////////////////////////////////////////////////////////
void SchedulerTest::testScheduleNullRunnableThrows() {

    Scheduler scheduler("testSchedualPeriodically");
    scheduler.start();

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a NullPointerException",
        scheduler.schedualPeriodically(NULL, 400),
        NullPointerException);
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a NullPointerException",
        scheduler.executePeriodically(NULL, 400),
        NullPointerException);
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a NullPointerException",
        scheduler.executeAfterDelay(NULL, 400),
        NullPointerException);

}

////////////////////////////////////////////////////////////////////////////////
void SchedulerTest::testExecutePeriodically() {

    {
        Scheduler scheduler("testExecutePeriodically");
        scheduler.start();
        CounterTask* task = new CounterTask();
        scheduler.executePeriodically(task, 500);
        CPPUNIT_ASSERT(task->getCount() == 0);
        Thread::sleep(600);
        CPPUNIT_ASSERT(task->getCount() >= 1);
        Thread::sleep(600);
        CPPUNIT_ASSERT(task->getCount() >= 2);
        CPPUNIT_ASSERT(task->getCount() < 5);
    }

    {
        Scheduler scheduler("testSchedualPeriodically");
        scheduler.start();
        CounterTask* task = new CounterTask();
        scheduler.schedualPeriodically(task, 1000);
        CPPUNIT_ASSERT(task->getCount() == 0);
        scheduler.cancel(task);

        try{
            scheduler.cancel(task);
            CPPUNIT_FAIL("Should have thrown an exception");
        } catch(...) {
        }

        scheduler.shutdown();
    }
}

////////////////////////////////////////////////////////////////////////////////
void SchedulerTest::testSchedualPeriodically() {

    {
        Scheduler scheduler("testSchedualPeriodically");
        scheduler.start();
        CounterTask* task = new CounterTask();
        scheduler.schedualPeriodically(task, 400);
        CPPUNIT_ASSERT(task->getCount() == 0);
        Thread::sleep(600);
        CPPUNIT_ASSERT(task->getCount() >= 1);
        Thread::sleep(600);
        CPPUNIT_ASSERT(task->getCount() >= 2);
        CPPUNIT_ASSERT(task->getCount() < 5);
    }

    {
        Scheduler scheduler("testSchedualPeriodically");
        scheduler.start();
        CounterTask* task = new CounterTask();
        scheduler.schedualPeriodically(task, 1000);
        CPPUNIT_ASSERT(task->getCount() == 0);
        scheduler.cancel(task);

        try{
            scheduler.cancel(task);
            CPPUNIT_FAIL("Should have thrown an exception");
        } catch(...) {
        }

        scheduler.shutdown();
    }
}

////////////////////////////////////////////////////////////////////////////////
void SchedulerTest::testExecuteAfterDelay() {

    {
        Scheduler scheduler("testExecuteAfterDelay");
        scheduler.start();
        CounterTask task;
        scheduler.executeAfterDelay(&task, 500, false);
        CPPUNIT_ASSERT(task.getCount() == 0);
        Thread::sleep(600);
        CPPUNIT_ASSERT_EQUAL(1, task.getCount());
        Thread::sleep(600);
        CPPUNIT_ASSERT_EQUAL(1, task.getCount());
    }

    // Should not be any cancelable tasks
    {
        Scheduler scheduler("testExecuteAfterDelay");
        scheduler.start();
        CounterTask* task = new CounterTask();
        scheduler.executeAfterDelay(task, 1000);
        CPPUNIT_ASSERT(task->getCount() == 0);

        try{
            scheduler.cancel(task);
            CPPUNIT_FAIL("Should have thrown an exception");
        } catch(...) {
        }

        scheduler.shutdown();
    }

    // This block is for testing memory leaks, no actual Tests.
    {
        Scheduler scheduler("testExecuteAfterDelay");
        scheduler.start();
        CounterTask* task = new CounterTask();
        scheduler.executeAfterDelay(task, 300);
        CPPUNIT_ASSERT(task->getCount() == 0);
        Thread::sleep(600);
    }
}

////////////////////////////////////////////////////////////////////////////////
void SchedulerTest::testCancel() {

    Scheduler scheduler("testCancel");
    scheduler.start();
    CounterTask task;
    scheduler.executePeriodically(&task, 500, false);
    scheduler.cancel(&task);
    CPPUNIT_ASSERT(task.getCount() == 0);
    Thread::sleep(600);
    CPPUNIT_ASSERT_EQUAL(0, task.getCount());
    Thread::sleep(600);
    CPPUNIT_ASSERT_EQUAL(0, task.getCount());
    scheduler.shutdown();
}

////////////////////////////////////////////////////////////////////////////////
void SchedulerTest::testShutdown() {

    {
        Scheduler scheduler("testShutdown");
        scheduler.start();
        CounterTask task;
        scheduler.executePeriodically(&task, 500, false);
        scheduler.shutdown();
        CPPUNIT_ASSERT(task.getCount() == 0);
        Thread::sleep(600);
        CPPUNIT_ASSERT_EQUAL(0, task.getCount());
        Thread::sleep(600);
        CPPUNIT_ASSERT_EQUAL(0, task.getCount());
    }

    {
        Scheduler scheduler("testShutdown");
        scheduler.start();
        CounterTask* task = new CounterTask();
        scheduler.executeAfterDelay(task, 1000);
        CPPUNIT_ASSERT(task->getCount() == 0);
        scheduler.shutdown();
        scheduler.stop();
        CPPUNIT_ASSERT(scheduler.isStopped());
    }
}
