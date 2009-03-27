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

#include "MemoryUsageTest.h"
#include <activemq/util/MemoryUsage.h>

#include <decaf/lang/Runnable.h>
#include <decaf/lang/System.h>
#include <decaf/lang/Thread.h>

using namespace activemq;
using namespace activemq::util;
using namespace decaf::lang;

class UsageRunner : public decaf::lang::Runnable {
private:

    MemoryUsage* usage;

public:

    UsageRunner( MemoryUsage* usage ) {
        this->usage = usage;
    }

    virtual void run(){
        Thread::sleep( 50 );
        this->usage->decreaseUsage( this->usage->getUsage() );
    }
};

void MemoryUsageTest::testCTors() {

    MemoryUsage usage1;
    MemoryUsage usage2( 1024 );

    CPPUNIT_ASSERT( usage1.getLimit() == 0 );
    CPPUNIT_ASSERT( usage2.getLimit() == 1024 );

    CPPUNIT_ASSERT( usage1.getUsage() == 0 );
    CPPUNIT_ASSERT( usage2.getUsage() == 0 );
}

void MemoryUsageTest::testUsage() {

    MemoryUsage usage1( 2048 );

    CPPUNIT_ASSERT( !usage1.isFull() );
    CPPUNIT_ASSERT( usage1.getUsage() == 0 );

    usage1.increaseUsage( 1024 );

    CPPUNIT_ASSERT( !usage1.isFull() );
    CPPUNIT_ASSERT( usage1.getUsage() == 1024 );

    usage1.decreaseUsage( 512 );

    CPPUNIT_ASSERT( !usage1.isFull() );
    CPPUNIT_ASSERT( usage1.getUsage() == 512 );

    usage1.setUsage( 2048 );

    CPPUNIT_ASSERT( usage1.isFull() );
    CPPUNIT_ASSERT( usage1.getUsage() == 2048 );

    usage1.increaseUsage( 1024 );
    CPPUNIT_ASSERT( usage1.isFull() );
    CPPUNIT_ASSERT( usage1.getUsage() == 3072 );
}

void MemoryUsageTest::testTimedWait() {

    MemoryUsage usage( 2048 );
    usage.increaseUsage( 5072 );

    unsigned long long startTime = System::currentTimeMillis();

    usage.waitForSpace( 100 );

    unsigned long long endTime = System::currentTimeMillis();

    CPPUNIT_ASSERT( endTime - startTime >= 100 );
}

void MemoryUsageTest::testWait() {

    MemoryUsage usage( 2048 );
    usage.increaseUsage( 5072 );
    UsageRunner runner( &usage );

    Thread myThread( &runner );
    myThread.start();

    usage.waitForSpace();
    CPPUNIT_ASSERT( usage.getUsage() == 0 );

    myThread.join();

}
