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

#include "AbstractExecutorServiceTest.h"

#include <string>

#include <decaf/lang/Runnable.h>
#include <decaf/lang/Pointer.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/util/ArrayList.h>
#include <decaf/util/concurrent/LinkedBlockingQueue.h>
#include <decaf/util/concurrent/TimeUnit.h>
#include <decaf/util/concurrent/Future.h>
#include <decaf/util/concurrent/AbstractExecutorService.h>
#include <decaf/util/concurrent/ThreadPoolExecutor.h>

using namespace std;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::util;
using namespace decaf::util::concurrent;

////////////////////////////////////////////////////////////////////////////////
namespace {

    /**
     * A no-frills implementation of AbstractExecutorService, designed
     * to test the submit methods only.
     */
    class DirectExecutorService : public AbstractExecutorService {
    private:

        volatile bool isshutdown;

    public:

        DirectExecutorService() : AbstractExecutorService(), isshutdown(false) {
        }

        virtual ~DirectExecutorService() {}

        virtual void execute(Runnable* r) {
            this->execute(r, true);
        }

        virtual void execute(Runnable* r, bool own) {
            if (r == NULL) {
                throw NullPointerException(__FILE__, __LINE__, "Runnable was NULL");
            }

            try {
                r->run();
            } catch(Exception& e) {
                if (own) {
                    delete r;
                }

                throw;
            }

            if (own) {
                delete r;
            }
        }

        virtual void shutdown() {
            isshutdown = true;
        }

        virtual ArrayList<Runnable*> shutdownNow() {
            isshutdown = true;

            return ArrayList<Runnable*>();
        }

        virtual bool isShutdown() const {
            return isshutdown;
        }

        virtual bool isTerminated() const {
            return isShutdown();
        }

        virtual bool awaitTermination(long long timeout, const TimeUnit& unit) {
            return isShutdown();
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
AbstractExecutorServiceTest::AbstractExecutorServiceTest() {
}

////////////////////////////////////////////////////////////////////////////////
AbstractExecutorServiceTest::~AbstractExecutorServiceTest() {
}

////////////////////////////////////////////////////////////////////////////////
void AbstractExecutorServiceTest::testExecuteRunnable() {

    try {
        bool done = false;
        DirectExecutorService e;
        TrackedShortRunnable task(&done);

        CPPUNIT_ASSERT(!done);
        Future<int>* future = e.submit<int>(&task, false);
        future->get();
        CPPUNIT_ASSERT(done);
        delete future;
    } catch (ExecutionException& ex) {
        unexpectedException();
    } catch (InterruptedException& ex) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
void AbstractExecutorServiceTest::testSubmitCallable() {

    try {
        DirectExecutorService e;
        Future<string>* future = e.submit(new StringTask());
        string result = future->get();
        CPPUNIT_ASSERT_EQUAL(TEST_STRING, result);
        delete future;
    } catch (ExecutionException& ex) {
        unexpectedException();
    } catch (InterruptedException& ex) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
void AbstractExecutorServiceTest::testSubmitRunnable() {
    try {
        DirectExecutorService e;
        Future<int>* future = e.submit<int>(new NoOpRunnable());
        future->get();
        CPPUNIT_ASSERT(future->isDone());
        delete future;
    } catch (ExecutionException& ex) {
        unexpectedException();
    } catch (InterruptedException& ex) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
void AbstractExecutorServiceTest::testSubmitRunnable2() {
    try {
        DirectExecutorService e;
        Future<string>* future = e.submit(new NoOpRunnable(), TEST_STRING);
        string result = future->get();
        CPPUNIT_ASSERT_EQUAL(TEST_STRING, result);
        delete future;
    } catch (ExecutionException& ex) {
        unexpectedException();
    } catch (InterruptedException& ex) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
void AbstractExecutorServiceTest::testExecuteNullRunnable() {
    try {
        DirectExecutorService e;
        TrackedShortRunnable* task = NULL;
        e.submit<int>(task);
        shouldThrow();
    } catch (NullPointerException& success) {
    } catch (Exception& ex) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
void AbstractExecutorServiceTest::testSubmitNullCallable() {
    try {
        DirectExecutorService e;
        StringTask* t = NULL;
        e.submit(t);
        shouldThrow();
    } catch (NullPointerException& success) {
    } catch (Exception& ex) {
        unexpectedException();
    }
}

////////////////////////////////////////////////////////////////////////////////
void AbstractExecutorServiceTest::testExecute1() {
    ThreadPoolExecutor p(1, 1, 60, TimeUnit::SECONDS, new LinkedBlockingQueue<Runnable*>(1));
    MediumRunnable task(this);

    try {
        for(int i = 0; i < 5; ++i) {
            Pointer< Future<int> > future(p.submit<int>(&task, false));
        }

        shouldThrow();
    } catch(RejectedExecutionException& success) {
    }

    joinPool(p);
}

////////////////////////////////////////////////////////////////////////////////
void AbstractExecutorServiceTest::testExecute2() {
    ThreadPoolExecutor p(1, 1, 60, TimeUnit::SECONDS, new LinkedBlockingQueue<Runnable*>(1));
    try {

        for(int i = 0; i < 5; ++i) {
            delete p.submit(new SmallCallable<int>(this));
        }

        shouldThrow();
    } catch(RejectedExecutionException& e) {
    }

    joinPool(p);
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    template<typename E>
    class TestInterruptedSubmitCallable : public Callable<E> {
    private:

        AbstractExecutorServiceTest* parent;

    private:

        TestInterruptedSubmitCallable(const TestInterruptedSubmitCallable&);
        TestInterruptedSubmitCallable operator= (const TestInterruptedSubmitCallable&);

    public:

        TestInterruptedSubmitCallable(AbstractExecutorServiceTest* parent) :
            decaf::util::concurrent::Callable<E>(), parent(parent) {
        }

        virtual ~TestInterruptedSubmitCallable() {}

        virtual E call() {
            try {
                Thread::sleep(AbstractExecutorServiceTest::LONG_DELAY_MS);
                parent->threadShouldThrow();
            } catch(InterruptedException& e){
            }

            return E();
        }
    };

    class TestInterruptedSubmitRunnable : public Runnable {
    private:

        AbstractExecutorServiceTest* parent;
        ThreadPoolExecutor* executor;

    private:

        TestInterruptedSubmitRunnable(const TestInterruptedSubmitRunnable&);
        TestInterruptedSubmitRunnable operator= (const TestInterruptedSubmitRunnable&);

    public:

        TestInterruptedSubmitRunnable(AbstractExecutorServiceTest* parent, ThreadPoolExecutor* executor) :
            Runnable(), parent(parent), executor(executor) {
        }

        virtual ~TestInterruptedSubmitRunnable() {}

        virtual void run() {
            try {
                Pointer< Future<int> > future(executor->submit(new TestInterruptedSubmitCallable<int>(parent)));
                future->get();
            } catch(InterruptedException& success){
            } catch(Exception& e) {
                parent->unexpectedException();
            }
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
void AbstractExecutorServiceTest::testInterruptedSubmit() {

    // TODO
//    ThreadPoolExecutor p(1, 1, 60, TimeUnit::SECONDS, new LinkedBlockingQueue<Runnable*>(10));
//    TestInterruptedSubmitRunnable runner(this, &p);
//    Thread t(&runner);
//
//    try {
//
//        t.start();
//        Thread::sleep(SHORT_DELAY_MS);
//        t.interrupt();
//
//    } catch(Exception& e){
//        unexpectedException();
//    }
//
//    joinPool(p);
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    template<typename E>
    class CallingRunnable : public decaf::lang::Runnable {
    private:

        AbstractExecutorServiceTest* parent;
        Callable<E>* target;

    private:

        CallingRunnable(const CallingRunnable&);
        CallingRunnable operator= (const CallingRunnable&);

    public:

        CallingRunnable(AbstractExecutorServiceTest* parent, Callable<E>* target) :
            Runnable(), parent(parent), target(target) {
        }

        virtual ~CallingRunnable() {
            delete target;
        }

        virtual void run() {
            try {
                target->call();
            } catch(Exception& e) {
            }
        }
    };

    template<typename E>
    class TestSubmitIECallable : public Callable<E> {
    private:

        AbstractExecutorServiceTest* parent;
        ThreadPoolExecutor* executor;

    private:

        TestSubmitIECallable(const TestSubmitIECallable&);
        TestSubmitIECallable operator= (const TestSubmitIECallable&);

    public:

        TestSubmitIECallable(AbstractExecutorServiceTest* parent, ThreadPoolExecutor* executor) :
            Callable<E>(), parent(parent), executor(executor) {
        }

        virtual ~TestSubmitIECallable() {}

        virtual E call() {
            try {
                Pointer< Future<E> >(executor->submit(
                    new AbstractExecutorServiceTest::SmallCallable<E>(parent)))->get();
                parent->threadShouldThrow();
            } catch(InterruptedException& e){
            } catch(RejectedExecutionException& e2){
            } catch(ExecutionException& e3){
            }

            return E();
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
void AbstractExecutorServiceTest::testSubmitIE() {

    ThreadPoolExecutor p(1, 1, 60, TimeUnit::SECONDS, new LinkedBlockingQueue<Runnable*>(10));

    TestSubmitIECallable<int>* callable = new TestSubmitIECallable<int>(this, &p);
    CallingRunnable<int> runner(this, callable);
    Thread t(&runner);

    try {
        t.start();
        Thread::sleep(SHORT_DELAY_MS);
        t.interrupt();
        t.join();
    } catch(InterruptedException& e){
        unexpectedException();
    }

    joinPool(p);
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class testSubmitEECallable : public Callable<int> {
    public:

        virtual ~testSubmitEECallable() {}

        virtual int call() {
            throw NumberFormatException(__FILE__, __LINE__, "Throwing a common exception");
            return 1;
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
void AbstractExecutorServiceTest::testSubmitEE() {
    ThreadPoolExecutor p(1, 1, 60, TimeUnit::SECONDS, new LinkedBlockingQueue<Runnable*>(10));

    testSubmitEECallable c;

    try {

        for(int i = 0; i < 5; i++) {
            Pointer< Future<int> >(p.submit(&c, false))->get();
        }

        shouldThrow();
    } catch(ExecutionException& success) {
    } catch(Exception& e) {
        unexpectedException();
    }

    joinPool(p);
}
