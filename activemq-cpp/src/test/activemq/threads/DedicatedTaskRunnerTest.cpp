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

#include "DedicatedTaskRunnerTest.h"

#include <memory>

#include <activemq/threads/Task.h>
#include <activemq/threads/DedicatedTaskRunner.h>

#include <decaf/lang/Thread.h>
#include <decaf/lang/exceptions/NullPointerException.h>

using namespace activemq;
using namespace activemq::threads;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
namespace {

    class SimpleCountingTask : public Task {
    private:

        unsigned int count;

    public:

        SimpleCountingTask() : count(0) {}
        virtual ~SimpleCountingTask() {}

        virtual bool iterate() {

            count++;
            return false;
        }

        unsigned int getCount() const { return count; }
    };

    class InfiniteCountingTask : public Task {
    private:

        unsigned int count;

    public:

        InfiniteCountingTask() : count(0) {}
        virtual ~InfiniteCountingTask() {}

        virtual bool iterate() {

            count++;
            return true;
        }

        unsigned int getCount() const { return count; }
    };
}

////////////////////////////////////////////////////////////////////////////////
void DedicatedTaskRunnerTest::testSimple() {

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException",
        std::auto_ptr<TaskRunner>( new DedicatedTaskRunner( NULL ) ),
        NullPointerException );

    SimpleCountingTask simpleTask;
    CPPUNIT_ASSERT( simpleTask.getCount() == 0 );
    DedicatedTaskRunner simpleTaskRunner( &simpleTask );

    simpleTaskRunner.start();

    simpleTaskRunner.wakeup();
    Thread::sleep( 250 );
    CPPUNIT_ASSERT( simpleTask.getCount() >= 1 );
    simpleTaskRunner.wakeup();
    Thread::sleep( 250 );
    CPPUNIT_ASSERT( simpleTask.getCount() >= 2 );

    InfiniteCountingTask infiniteTask;
    CPPUNIT_ASSERT( infiniteTask.getCount() == 0 );
    DedicatedTaskRunner infiniteTaskRunner( &infiniteTask );
    infiniteTaskRunner.start();
    Thread::sleep( 500 );
    CPPUNIT_ASSERT( infiniteTask.getCount() != 0 );
    infiniteTaskRunner.shutdown();
    unsigned int count = infiniteTask.getCount();
    Thread::sleep( 250 );
    CPPUNIT_ASSERT( infiniteTask.getCount() == count );
}
