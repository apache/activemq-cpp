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

#include "LockSupportTest.h"

#include <decaf/lang/System.h>
#include <decaf/lang/Thread.h>
#include <decaf/lang/Runnable.h>
#include <decaf/util/Date.h>
#include <decaf/util/concurrent/TimeUnit.h>
#include <decaf/util/concurrent/locks/LockSupport.h>

using namespace std;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace decaf::util::concurrent::locks;

////////////////////////////////////////////////////////////////////////////////
LockSupportTest::LockSupportTest() {
}

////////////////////////////////////////////////////////////////////////////////
LockSupportTest::~LockSupportTest() {
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class ParkTestThread : public Thread {
    private:

        LockSupportTest* parent;

    private:

        ParkTestThread(const ParkTestThread&);
        ParkTestThread operator= (const ParkTestThread&);

    public:

        ParkTestThread(LockSupportTest* parent) : Thread(), parent(parent) {}
        virtual ~ParkTestThread() {}

        virtual void run() {
            try{
                LockSupport::park();
            } catch(...) {
                parent->threadUnexpectedException();
            }
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
void LockSupportTest::testPark1() {

    ParkTestThread t(this);

    try {

        t.start();
        Thread::sleep( 1000 );
        LockSupport::unpark( &t );
        t.join();

    } catch(...) {
        CPPUNIT_FAIL("Caught an unexpected exception");
    }
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class ParkTest2Thread : public Thread {
    private:

        LockSupportTest* parent;

    private:

        ParkTest2Thread(const ParkTest2Thread&);
        ParkTest2Thread operator= (const ParkTest2Thread&);

    public:

        ParkTest2Thread(LockSupportTest* parent) : Thread(), parent(parent) {}
        virtual ~ParkTest2Thread() {}

        virtual void run() {
            try{
                Thread::sleep( 1000 );
                LockSupport::park();
            } catch(...) {
                parent->threadUnexpectedException();
            }
        }

    };
}

////////////////////////////////////////////////////////////////////////////////
void LockSupportTest::testPark2() {

    ParkTest2Thread t(this);

    try {

        long long before = System::currentTimeMillis();
        t.start();
        LockSupport::unpark( &t );
        t.join();
        long long after = System::currentTimeMillis();

        long long delta = after - before;

        CPPUNIT_ASSERT( delta > 800 && delta < 1500 );

    } catch(...) {
        CPPUNIT_FAIL("Caught an unexpected exception");
    }
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class Park3TestThread : public Thread {
    private:

        LockSupportTest* parent;

    private:

        Park3TestThread(const Park3TestThread&);
        Park3TestThread operator= (const Park3TestThread&);

    public:

        Park3TestThread(LockSupportTest* parent) : Thread(), parent(parent) {}
        virtual ~Park3TestThread() {}

        virtual void run() {
            try{
                LockSupport::park();
            } catch(...) {
                parent->threadUnexpectedException();
            }
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
void LockSupportTest::testPark3() {

    Park3TestThread t(this);

    try {
        t.start();
        Thread::sleep(SHORT_DELAY_MS);
        t.interrupt();
        t.join();
    }
    catch(Exception& e) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class Park4TestThread : public Thread {
    private:

        LockSupportTest* parent;
        Mutex* lock;

    private:

        Park4TestThread(const Park4TestThread&);
        Park4TestThread operator= (const Park4TestThread&);

    public:

        Park4TestThread(LockSupportTest* parent, Mutex* lock) : Thread(), parent(parent), lock(lock) {}
        virtual ~Park4TestThread() {}

        virtual void run() {
            try{
                lock->lock();
                LockSupport::park();
                lock->unlock();
                parent->threadAssertTrue(Thread::currentThread()->isInterrupted());
            } catch(...) {
                parent->threadUnexpectedException();
            }
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
void LockSupportTest::testPark4() {

    Mutex lock;
    Park4TestThread t(this, &lock);
    lock.lock();

    try {
        t.start();
        t.interrupt();
        lock.unlock();
        t.join();
    }
    catch(Exception e) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class ParkNanosTestThread : public Thread {
    private:

        LockSupportTest* parent;

    private:

        ParkNanosTestThread(const ParkNanosTestThread&);
        ParkNanosTestThread operator= (const ParkNanosTestThread&);

    public:

        ParkNanosTestThread(LockSupportTest* parent) : Thread(), parent(parent) {}
        virtual ~ParkNanosTestThread() {}

        virtual void run() {
            try{
                LockSupport::parkNanos( TimeUnit::SECONDS.toNanos( 2 ) );
            } catch(...) {
                parent->threadUnexpectedException();
            }
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
void LockSupportTest::testParkNanos() {

    ParkNanosTestThread t(this);

    long long before = System::currentTimeMillis();
    t.start();
    t.join();
    long long after = System::currentTimeMillis();
    long long delta = after - before;

    CPPUNIT_ASSERT( delta > 1500 && delta < 3000 );
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class ParkUntilTestThread : public Thread {
    private:

        LockSupportTest* parent;

    private:

        ParkUntilTestThread(const ParkUntilTestThread&);
        ParkUntilTestThread operator= (const ParkUntilTestThread&);

    public:

        ParkUntilTestThread(LockSupportTest* parent) : Thread(), parent(parent) {}
        virtual ~ParkUntilTestThread() {}

        virtual void run() {
            try{
                long long deadline = Date().getTime() + 100;
                LockSupport::parkUntil( deadline );
            } catch(...) {
                parent->threadUnexpectedException();
            }
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
void LockSupportTest::testParkUntil() {

    ParkUntilTestThread t(this);

    try {
        t.start();
        t.join();
    } catch(...) {
        CPPUNIT_FAIL("Caught an unexpected exception");
    }
}
