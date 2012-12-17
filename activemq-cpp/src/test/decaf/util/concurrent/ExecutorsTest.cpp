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

#include "ExecutorsTest.h"

#include <decaf/lang/Pointer.h>
#include <decaf/util/concurrent/ThreadPoolExecutor.h>
#include <decaf/util/concurrent/Executors.h>
#include <decaf/util/concurrent/CountDownLatch.h>

using namespace std;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::util;
using namespace decaf::util::concurrent;

////////////////////////////////////////////////////////////////////////////////
namespace {

    class DefaultThreadFactoryRunnable : public Runnable {
    private:

        CountDownLatch* shutdown;

    private:

        DefaultThreadFactoryRunnable(const DefaultThreadFactoryRunnable&);
        DefaultThreadFactoryRunnable operator= (const DefaultThreadFactoryRunnable&);

    public:

        DefaultThreadFactoryRunnable(CountDownLatch* shutdown) : Runnable(), shutdown(shutdown) {
        }

        virtual ~DefaultThreadFactoryRunnable() {}

        virtual void run() {
            this->shutdown->await();
        }

        void signalDone() {
            this->shutdown->countDown();
        }
    };

}

////////////////////////////////////////////////////////////////////////////////
ExecutorsTest::ExecutorsTest() {
}

////////////////////////////////////////////////////////////////////////////////
ExecutorsTest::~ExecutorsTest() {
}

////////////////////////////////////////////////////////////////////////////////
void ExecutorsTest::testNewSingleThreadExecutor1() {

    Pointer<ExecutorService> e(Executors::newSingleThreadExecutor());

    e->execute(new NoOpRunnable());
    e->execute(new NoOpRunnable());
    e->execute(new NoOpRunnable());

    joinPool(e.get());
}

////////////////////////////////////////////////////////////////////////////////
void ExecutorsTest::testNewSingleThreadExecutor2() {

    Pointer<ExecutorService> e(Executors::newSingleThreadExecutor(new SimpleThreadFactory()));

    e->execute(new NoOpRunnable());
    e->execute(new NoOpRunnable());
    e->execute(new NoOpRunnable());

    joinPool(e.get());
}

////////////////////////////////////////////////////////////////////////////////
void ExecutorsTest::testNewSingleThreadExecutor3() {

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException",
        Executors::newSingleThreadExecutor(NULL),
        NullPointerException);
}

////////////////////////////////////////////////////////////////////////////////
void ExecutorsTest::testCastNewSingleThreadExecutor() {

    Pointer<ExecutorService> e(Executors::newSingleThreadExecutor());

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a ClassCastException",
        e.dynamicCast<ThreadPoolExecutor>(),
        ClassCastException);

    joinPool(e.get());
}

////////////////////////////////////////////////////////////////////////////////
void ExecutorsTest::testDefaultThreadFactory() {

    CountDownLatch shutdown(1);
    Pointer<ThreadFactory> defaultFactory;
    DefaultThreadFactoryRunnable* runner = new DefaultThreadFactoryRunnable(&shutdown);

    defaultFactory.reset(Executors::getDefaultThreadFactory());

    Thread* theThread = defaultFactory->newThread(runner);

    CPPUNIT_ASSERT(theThread != NULL);
    const int expected = Thread::NORM_PRIORITY;
    CPPUNIT_ASSERT_EQUAL(expected, theThread->getPriority());

    theThread->start();

    shutdown.countDown();
    theThread->join();

    delete theThread;
    delete runner;
}

////////////////////////////////////////////////////////////////////////////////
void ExecutorsTest::testNewFixedThreadPool1() {
    Pointer<ExecutorService> e(Executors::newFixedThreadPool(2));

    e->execute(new NoOpRunnable());
    e->execute(new NoOpRunnable());
    e->execute(new NoOpRunnable());

    joinPool(e.get());
}

////////////////////////////////////////////////////////////////////////////////
void ExecutorsTest::testNewFixedThreadPool2() {

    Pointer<ExecutorService> e(Executors::newFixedThreadPool(2, new SimpleThreadFactory()));

    e->execute(new NoOpRunnable());
    e->execute(new NoOpRunnable());
    e->execute(new NoOpRunnable());

    joinPool(e.get());
}

////////////////////////////////////////////////////////////////////////////////
void ExecutorsTest::testNewFixedThreadPool3() {

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException",
        Executors::newFixedThreadPool(2, NULL),
        NullPointerException);
}

////////////////////////////////////////////////////////////////////////////////
void ExecutorsTest::testNewFixedThreadPool4() {

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a IllegalArgumentException",
        Executors::newFixedThreadPool(0),
        IllegalArgumentException);
}

////////////////////////////////////////////////////////////////////////////////
void ExecutorsTest::testUnconfigurableExecutorService() {

    Pointer<ExecutorService> e(Executors::unconfigurableExecutorService(Executors::newFixedThreadPool(2)));

    e->execute(new NoOpRunnable());
    e->execute(new NoOpRunnable());
    e->execute(new NoOpRunnable());

    joinPool(e.get());
}

////////////////////////////////////////////////////////////////////////////////
void ExecutorsTest::testUnconfigurableExecutorServiceNPE() {

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException",
        Executors::unconfigurableExecutorService(NULL),
        NullPointerException);
}

////////////////////////////////////////////////////////////////////////////////
void ExecutorsTest::testCallable1() {

    Pointer< Callable<int> > c(Executors::callable<int>(new NoOpRunnable()));
    CPPUNIT_ASSERT_EQUAL(0, c->call());
}

////////////////////////////////////////////////////////////////////////////////
void ExecutorsTest::testCallable2() {

    Pointer< Callable<int> > c(Executors::callable<int>(new NoOpRunnable(), 42));
    CPPUNIT_ASSERT_EQUAL(42, c->call());
}

////////////////////////////////////////////////////////////////////////////////
void ExecutorsTest::testCallableNPE1() {

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException",
        Executors::callable<int>(NULL),
        NullPointerException);
}

////////////////////////////////////////////////////////////////////////////////
void ExecutorsTest::testCallableNPE2() {

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException",
        Executors::callable<int>(NULL, 42),
        NullPointerException);
}
