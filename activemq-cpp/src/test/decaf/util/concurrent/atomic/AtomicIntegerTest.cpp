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

#include "AtomicIntegerTest.h"

#include <decaf/util/concurrent/atomic/AtomicInteger.h>
#include <decaf/lang/Integer.h>
#include <decaf/lang/Thread.h>

using namespace decaf;
using namespace decaf::lang;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace decaf::util::concurrent::atomic;

////////////////////////////////////////////////////////////////////////////////
void AtomicIntegerTest::testConstructor() {
    AtomicInteger ai;
    CPPUNIT_ASSERT( ai.get() == 0 );
}

////////////////////////////////////////////////////////////////////////////////
void AtomicIntegerTest::testConstructor2() {
    AtomicInteger ai( 999 );
    CPPUNIT_ASSERT( ai.get() == 999 );
}

////////////////////////////////////////////////////////////////////////////////
void AtomicIntegerTest::testGetSet() {
    AtomicInteger ai( 2 );
    CPPUNIT_ASSERT( 2 == ai.get() );
    ai.set( 5 );
    CPPUNIT_ASSERT( 5 == ai.get() );
    ai.set( 6 );
    CPPUNIT_ASSERT( 6 == ai.get() );
}

////////////////////////////////////////////////////////////////////////////////
void AtomicIntegerTest::testCompareAndSet() {
    AtomicInteger ai( 25 );
    CPPUNIT_ASSERT( ai.compareAndSet( 25, 50 ) );
    CPPUNIT_ASSERT( 50 == ai.get() );
    CPPUNIT_ASSERT( ai.compareAndSet( 50, 25 ) );
    CPPUNIT_ASSERT( 25 == ai.get() );
    CPPUNIT_ASSERT( !ai.compareAndSet( 50, 75 ) );
    CPPUNIT_ASSERT( ai.get() != 75 );
    CPPUNIT_ASSERT( ai.compareAndSet( 25, 50 ) );
    CPPUNIT_ASSERT( 50 == ai.get() );

    AtomicInteger ai2( 1 );
    CPPUNIT_ASSERT( ai2.compareAndSet( 1, 2 ) );
    CPPUNIT_ASSERT( ai2.compareAndSet( 2, -4 ) );
    CPPUNIT_ASSERT( -4 == ai2.get() );
    CPPUNIT_ASSERT( !ai2.compareAndSet( -5, 7 ) );
    CPPUNIT_ASSERT( 7 != ai2.get() );
    CPPUNIT_ASSERT( ai2.compareAndSet( -4, 7 ) );
    CPPUNIT_ASSERT( 7 == ai2.get() );
}

////////////////////////////////////////////////////////////////////////////////
class MyIntRunnable: public Runnable {
private:

    AtomicInteger* aip;

public:

    MyIntRunnable( AtomicInteger* ai ) :
        aip( ai ) {
    }

    virtual void run() {
        while( !aip->compareAndSet( 2, 3 ) ) {
            Thread::yield();
        }
    }

};

////////////////////////////////////////////////////////////////////////////////
void AtomicIntegerTest::testCompareAndSetInMultipleThreads() {
    AtomicInteger ai( 1 );

    MyIntRunnable runnable( &ai );
    Thread t( &runnable );

    try {

        t.start();
        CPPUNIT_ASSERT( ai.compareAndSet( 1, 2 ) );
        t.join();
        CPPUNIT_ASSERT( ai.get() == 3 );

    } catch( Exception& e ) {
        CPPUNIT_FAIL( "Should Not Throw" );
    }
}

////////////////////////////////////////////////////////////////////////////////
void AtomicIntegerTest::testGetAndSet() {
    AtomicInteger ai( 50 );
    CPPUNIT_ASSERT( 50 == ai.getAndSet( 75 ) );
    CPPUNIT_ASSERT( 75 == ai.getAndSet( 25 ) );
    CPPUNIT_ASSERT( 25 == ai.getAndSet( 100 ) );
    CPPUNIT_ASSERT( 100 == ai.get() );
}

////////////////////////////////////////////////////////////////////////////////
void AtomicIntegerTest::testToString() {
    AtomicInteger ai;
    CPPUNIT_ASSERT( ai.toString() == Integer::toString( 0 ) );
    ai.set( 999 );
    CPPUNIT_ASSERT( ai.toString() == Integer::toString( 999 ) );
}

////////////////////////////////////////////////////////////////////////////////
void AtomicIntegerTest::testGetAndAdd() {
    AtomicInteger ai( 1 );
    CPPUNIT_ASSERT( 1 == ai.getAndAdd(2) );
    CPPUNIT_ASSERT( 3 == ai.get() );
    CPPUNIT_ASSERT( 3 == ai.getAndAdd(-4) );
    CPPUNIT_ASSERT( -1 == ai.get() );
}

////////////////////////////////////////////////////////////////////////////////
void AtomicIntegerTest::testGetAndDecrement() {
    AtomicInteger ai( 1 );
    CPPUNIT_ASSERT( 1 == ai.getAndDecrement() );
    CPPUNIT_ASSERT( 0 == ai.getAndDecrement() );
    CPPUNIT_ASSERT( -1 == ai.getAndDecrement() );
}

////////////////////////////////////////////////////////////////////////////////
void AtomicIntegerTest::testGetAndIncrement() {
    AtomicInteger ai( 1 );
    CPPUNIT_ASSERT( 1 == ai.getAndIncrement() );
    CPPUNIT_ASSERT( 2 == ai.get() );
    ai.set( -2 );
    CPPUNIT_ASSERT( -2 == ai.getAndIncrement() );
    CPPUNIT_ASSERT( -1 == ai.getAndIncrement() );
    CPPUNIT_ASSERT( 0 == ai.getAndIncrement() );
    CPPUNIT_ASSERT( 1 == ai.get() );
}

////////////////////////////////////////////////////////////////////////////////
void AtomicIntegerTest::testAddAndGet() {
    AtomicInteger ai( 1 );
    CPPUNIT_ASSERT( 3 == ai.addAndGet(2) );
    CPPUNIT_ASSERT( 3 == ai.get() );
    CPPUNIT_ASSERT( -1 == ai.addAndGet(-4) );
    CPPUNIT_ASSERT( -1 == ai.get() );
}

////////////////////////////////////////////////////////////////////////////////
void AtomicIntegerTest::testDecrementAndGet() {
    AtomicInteger ai( 1 );
    CPPUNIT_ASSERT( 0 == ai.decrementAndGet() );
    CPPUNIT_ASSERT( -1 == ai.decrementAndGet() );
    CPPUNIT_ASSERT( -2 == ai.decrementAndGet() );
    CPPUNIT_ASSERT( -2 == ai.get() );
}

////////////////////////////////////////////////////////////////////////////////
void AtomicIntegerTest::testIncrementAndGet() {
    AtomicInteger ai( 1 );
    CPPUNIT_ASSERT( 2 == ai.incrementAndGet() );
    CPPUNIT_ASSERT( 2 == ai.get() );
    ai.set( -2 );
    CPPUNIT_ASSERT( -1 == ai.incrementAndGet() );
    CPPUNIT_ASSERT( 0 == ai.incrementAndGet() );
    CPPUNIT_ASSERT( 1 == ai.incrementAndGet() );
    CPPUNIT_ASSERT( 1 == ai.get() );
}

////////////////////////////////////////////////////////////////////////////////
void AtomicIntegerTest::testIntValue() {
    AtomicInteger ai;
    for( int i = -12; i < 6; ++i ) {
        ai.set( i );
        CPPUNIT_ASSERT( i == ai.intValue() );
    }
}

////////////////////////////////////////////////////////////////////////////////
void AtomicIntegerTest::testLongValue() {
    AtomicInteger ai;
    for( int i = -12; i < 6; ++i ) {
        ai.set( i );
        CPPUNIT_ASSERT( (long long)i == ai.longValue() );
    }
}

////////////////////////////////////////////////////////////////////////////////
void AtomicIntegerTest::testFloatValue() {
    AtomicInteger ai;
    for( int i = -12; i < 6; ++i ) {
        ai.set( i );
        CPPUNIT_ASSERT( (float)i == ai.floatValue() );
    }
}

////////////////////////////////////////////////////////////////////////////////
void AtomicIntegerTest::testDoubleValue() {
    AtomicInteger ai;
    for( int i = -12; i < 6; ++i ) {
        ai.set( i );
        CPPUNIT_ASSERT( (double)i == ai.doubleValue() );
    }
}
