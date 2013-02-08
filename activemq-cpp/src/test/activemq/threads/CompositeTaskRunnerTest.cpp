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

#include "CompositeTaskRunnerTest.h"

#include <activemq/threads/CompositeTask.h>
#include <activemq/threads/CompositeTaskRunner.h>
#include <decaf/lang/Thread.h>

#include <iostream>
#include <iomanip>

using namespace std;
using namespace activemq;
using namespace activemq::threads;
using namespace decaf::lang;

////////////////////////////////////////////////////////////////////////////////
class CountingTask : public CompositeTask {
private:

    int count;
    int goal;
    std::string name;

public:

    CountingTask( const std::string& name, int goal ) : count(0), goal(goal), name(name) {}

    int getCount() const {
        return count;
    }

    virtual bool isPending() const {
        return count != goal;
    }

    virtual bool iterate() {
        return !( ++count == goal );
    }

};

////////////////////////////////////////////////////////////////////////////////
void CompositeTaskRunnerTest::test() {

    int attempts = 0;

    CompositeTaskRunner runner;

    CountingTask task1( "task1", 100);
    CountingTask task2( "task2", 200);

    runner.addTask( &task1 );
    runner.addTask( &task2 );

    runner.start();
    runner.wakeup();

    while( attempts++ != 10 ) {

        Thread::sleep( 1000 );

        if( task1.getCount() == 100 && task2.getCount() == 200 ) {
            break;
        }
    }

    CPPUNIT_ASSERT( task1.getCount() == 100 );
    CPPUNIT_ASSERT( task2.getCount() == 200 );

    runner.removeTask( &task1 );
    runner.removeTask( &task2 );

}

