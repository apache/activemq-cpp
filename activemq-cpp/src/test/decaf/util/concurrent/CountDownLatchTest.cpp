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

#include "CountDownLatchTest.h"

using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::util;
using namespace decaf::util::concurrent;

////////////////////////////////////////////////////////////////////////////////
void CountDownLatchTest::test()
{
    CountDownLatch latch( 50 );

    CPPUNIT_ASSERT( latch.getCount() == 50 );

    MyThread thread;
    thread.latch = &latch;
    thread.start();

    latch.await();

    CPPUNIT_ASSERT( latch.getCount() == 0 );

    thread.join();

    CPPUNIT_ASSERT( true );
}

////////////////////////////////////////////////////////////////////////////////
void CountDownLatchTest::test2()
{
    CountDownLatch latch( 75 );

    CPPUNIT_ASSERT( latch.getCount() == 75 );

    MyThread thread;
    thread.latch = &latch;
    thread.start();

    CPPUNIT_ASSERT( latch.await( 2 ) == false );

    latch.await();

    CPPUNIT_ASSERT( latch.getCount() == 0 );

    thread.join();

    CPPUNIT_ASSERT( true );
}

////////////////////////////////////////////////////////////////////////////////
void CountDownLatchTest::testConstructor() {
    try {
        CountDownLatch l(-1);
        shouldThrow();
    } catch(IllegalArgumentException& success){}
}

////////////////////////////////////////////////////////////////////////////////
void CountDownLatchTest::testGetCount() {
    CountDownLatch l(2);
    CPPUNIT_ASSERT_EQUAL(2, l.getCount());
    l.countDown();
    CPPUNIT_ASSERT_EQUAL(1, l.getCount());
}

////////////////////////////////////////////////////////////////////////////////
void CountDownLatchTest::testCountDown() {
    CountDownLatch l(1);
    CPPUNIT_ASSERT_EQUAL(1, l.getCount());
    l.countDown();
    CPPUNIT_ASSERT_EQUAL(0, l.getCount());
    l.countDown();
    CPPUNIT_ASSERT_EQUAL(0, l.getCount());
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class TestAwaitRunnable : public Runnable {
    private:

        CountDownLatch* latch;
        CountDownLatchTest* parent;

    private:

        TestAwaitRunnable(const TestAwaitRunnable&);
        TestAwaitRunnable operator= (const TestAwaitRunnable&);

    public:

        TestAwaitRunnable(CountDownLatch* latch, CountDownLatchTest* parent) :
            Runnable(), latch(latch), parent(parent) {}
        virtual ~TestAwaitRunnable() {}

        virtual void run() {
            try {
                parent->threadAssertTrue(latch->getCount() > 0);
                latch->await();
                parent->threadAssertTrue(latch->getCount() == 0);
            } catch(InterruptedException& e) {
                parent->threadUnexpectedException();
            }
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
void CountDownLatchTest::testAwait() {
    CountDownLatch l(2);
    TestAwaitRunnable runnable(&l, this);
    Thread t(&runnable);

    t.start();
    try {
        CPPUNIT_ASSERT_EQUAL(l.getCount(), 2);
        Thread::sleep(SHORT_DELAY_MS);
        l.countDown();
        CPPUNIT_ASSERT_EQUAL(l.getCount(), 1);
        l.countDown();
        CPPUNIT_ASSERT_EQUAL(l.getCount(), 0);
        t.join();
    } catch(InterruptedException& e) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class TestTimedAwaitRunnable : public Runnable {
    private:

        CountDownLatch* latch;
        CountDownLatchTest* parent;

    private:

        TestTimedAwaitRunnable(const TestTimedAwaitRunnable&);
        TestTimedAwaitRunnable operator= (const TestTimedAwaitRunnable&);

    public:

        TestTimedAwaitRunnable(CountDownLatch* latch, CountDownLatchTest* parent) :
            Runnable(), latch(latch), parent(parent) {}
        virtual ~TestTimedAwaitRunnable() {}

        virtual void run() {
            try {
                parent->threadAssertTrue(latch->getCount() > 0);
                parent->threadAssertTrue(latch->await(CountDownLatchTest::SMALL_DELAY_MS, TimeUnit::MILLISECONDS));
            } catch(InterruptedException& e) {
                parent->threadUnexpectedException();
            }
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
void CountDownLatchTest::testTimedAwait() {
    CountDownLatch l(2);
    TestTimedAwaitRunnable runnable(&l, this);
    Thread t(&runnable);

    t.start();
    try {
        CPPUNIT_ASSERT_EQUAL(l.getCount(), 2);
        Thread::sleep(SHORT_DELAY_MS);
        l.countDown();
        CPPUNIT_ASSERT_EQUAL(l.getCount(), 1);
        l.countDown();
        CPPUNIT_ASSERT_EQUAL(l.getCount(), 0);
        t.join();
    } catch(InterruptedException& e) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class TestAwaitInterruptedExceptionRunnable : public Runnable {
    private:

        CountDownLatch* latch;
        CountDownLatchTest* parent;

    private:

        TestAwaitInterruptedExceptionRunnable(const TestAwaitInterruptedExceptionRunnable&);
        TestAwaitInterruptedExceptionRunnable operator= (const TestAwaitInterruptedExceptionRunnable&);

    public:

        TestAwaitInterruptedExceptionRunnable(CountDownLatch* latch, CountDownLatchTest* parent) :
            Runnable(), latch(latch), parent(parent) {}
        virtual ~TestAwaitInterruptedExceptionRunnable() {}

        virtual void run() {
            try {
                parent->threadAssertTrue(latch->getCount() > 0);
                latch->await();
                parent->threadShouldThrow();
            } catch(InterruptedException& success) {
            }
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
void CountDownLatchTest::testAwaitInterruptedException() {
    CountDownLatch l(1);
    TestAwaitInterruptedExceptionRunnable runnable(&l, this);
    Thread t(&runnable);

    t.start();
    try {
        CPPUNIT_ASSERT_EQUAL(l.getCount(), 1);
        t.interrupt();
        t.join();
    } catch(InterruptedException& e) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class TestTimedAwaitInterruptedExceptionRunnable : public Runnable {
    private:

        CountDownLatch* latch;
        CountDownLatchTest* parent;

    private:

        TestTimedAwaitInterruptedExceptionRunnable(const TestTimedAwaitInterruptedExceptionRunnable&);
        TestTimedAwaitInterruptedExceptionRunnable operator= (const TestTimedAwaitInterruptedExceptionRunnable&);

    public:

        TestTimedAwaitInterruptedExceptionRunnable(CountDownLatch* latch, CountDownLatchTest* parent) :
            Runnable(), latch(latch), parent(parent) {}
        virtual ~TestTimedAwaitInterruptedExceptionRunnable() {}

        virtual void run() {
            try {
                parent->threadAssertTrue(latch->getCount() > 0);
                latch->await(CountDownLatchTest::MEDIUM_DELAY_MS, TimeUnit::MILLISECONDS);
                parent->threadShouldThrow();
            } catch(InterruptedException& success) {
            }
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
void CountDownLatchTest::testTimedAwaitInterruptedException() {
    CountDownLatch l(1);
    TestTimedAwaitInterruptedExceptionRunnable runnable(&l, this);
    Thread t(&runnable);

    t.start();
    try {
        Thread::sleep(SHORT_DELAY_MS);
        CPPUNIT_ASSERT_EQUAL(l.getCount(), 1);
        t.interrupt();
        t.join();
    } catch(InterruptedException& e) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class TestAwaitTimeoutRunnable : public Runnable {
    private:

        CountDownLatch* latch;
        CountDownLatchTest* parent;

    private:

        TestAwaitTimeoutRunnable(const TestAwaitTimeoutRunnable&);
        TestAwaitTimeoutRunnable operator= (const TestAwaitTimeoutRunnable&);

    public:

        TestAwaitTimeoutRunnable(CountDownLatch* latch, CountDownLatchTest* parent) :
            Runnable(), latch(latch), parent(parent) {}
        virtual ~TestAwaitTimeoutRunnable() {}

        virtual void run() {
            try {
                parent->threadAssertTrue(latch->getCount() > 0);
                parent->threadAssertFalse(latch->await(CountDownLatchTest::SHORT_DELAY_MS, TimeUnit::MILLISECONDS));
                parent->threadAssertTrue(latch->getCount() > 0);
            } catch(InterruptedException& ie) {
                parent->threadUnexpectedException();
            }
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
void CountDownLatchTest::testAwaitTimeout() {
    CountDownLatch l(1);
    TestAwaitTimeoutRunnable runnable(&l, this);
    Thread t(&runnable);

    t.start();
    try {
        CPPUNIT_ASSERT_EQUAL(l.getCount(), 1);
        t.join();
    } catch(InterruptedException& e) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
void CountDownLatchTest::testToString() {
    CountDownLatch s(2);
    std::string us = s.toString();
    CPPUNIT_ASSERT((int)us.find_first_of("Count = 2") >= 0);
    s.countDown();
    std::string s1 = s.toString();
    CPPUNIT_ASSERT((int)s1.find_first_of("Count = 1") >= 0);
    s.countDown();
    std::string s2 = s.toString();
    CPPUNIT_ASSERT((int)s2.find_first_of("Count = 0") >= 0);
}
