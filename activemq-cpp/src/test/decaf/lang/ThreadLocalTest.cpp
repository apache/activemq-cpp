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

#include "ThreadLocalTest.h"

#include <decaf/util/concurrent/Mutex.h>
#include <decaf/lang/System.h>
#include <decaf/lang/ThreadLocal.h>
#include <decaf/lang/exceptions/InterruptedException.h>
#include <decaf/lang/exceptions/RuntimeException.h>

#include <memory>

using namespace std;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::util;
using namespace decaf::util::concurrent;

////////////////////////////////////////////////////////////////////////////////
ThreadLocalTest::ThreadLocalTest() {
}

////////////////////////////////////////////////////////////////////////////////
ThreadLocalTest::~ThreadLocalTest() {
}

////////////////////////////////////////////////////////////////////////////////
void ThreadLocalTest::testConstructor() {
    ThreadLocal<int> local;
    CPPUNIT_ASSERT(local.get() == 0);
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class StringThreadLocal : public ThreadLocal<std::string> {
    public:

        StringThreadLocal() : ThreadLocal<std::string>() {}
        virtual ~StringThreadLocal() {}

    protected:

        virtual std::string initialValue() const {
            return "initial";
        }
    };

}

////////////////////////////////////////////////////////////////////////////////
void ThreadLocalTest::testRemove() {

    StringThreadLocal tl;

    CPPUNIT_ASSERT_EQUAL(std::string("initial"), tl.get());
    tl.set("fixture");
    CPPUNIT_ASSERT_EQUAL(std::string("fixture"), tl.get());
    tl.remove();
    CPPUNIT_ASSERT_EQUAL(std::string("initial"), tl.get());
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class TestGetRunnable : public Runnable {
    private:

        std::string* result;
        StringThreadLocal* local;

    private:

        TestGetRunnable(const TestGetRunnable&);
        TestGetRunnable& operator= (const TestGetRunnable&);

    public:

        TestGetRunnable(StringThreadLocal* local, std::string* result) : Runnable(), result(result), local(local) {}
        virtual ~TestGetRunnable() {}

        virtual void run() {
            *result = local->get();
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
void ThreadLocalTest::testGet() {

    ThreadLocal<long long> l;
    CPPUNIT_ASSERT_MESSAGE("ThreadLocal's initial value is 0", l.get() == 0);

    // The ThreadLocal has to run once for each thread that touches the
    // ThreadLocal
    StringThreadLocal local;

    CPPUNIT_ASSERT_MESSAGE(std::string("ThreadLocal's initial value should be 'initial'")
                           + " but is " + local.get(), local.get() == "initial");

    std::string result;
    TestGetRunnable runnable(&local, &result);
    Thread t(&runnable);

    // Alter the ThreadLocal here and then check that another thread still gets the default
    // initial value when it calls get.
    local.set("updated");

    t.start();
    try {
        t.join();
    } catch (InterruptedException& ie) {
        CPPUNIT_FAIL("Interrupted!!");
    }

    CPPUNIT_ASSERT_MESSAGE("ThreadLocal's initial value in other Thread should be 'initial'",
                           result == "initial");
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class TestSetRunnable : public Runnable {
    private:

        StringThreadLocal* local;

    private:

        TestSetRunnable(const TestSetRunnable&);
        TestSetRunnable& operator= (const TestSetRunnable&);

    public:

        TestSetRunnable(StringThreadLocal* local) : Runnable(), local(local) {}
        virtual ~TestSetRunnable() {}

        virtual void run() {
            local->set("some other value");
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
void ThreadLocalTest::testSet() {

    StringThreadLocal local;
    CPPUNIT_ASSERT_MESSAGE(std::string("ThreadLocal's initial value should be 'initial'")
                           + " but is " + local.get(), local.get() == "initial");

    TestSetRunnable runnable(&local);
    Thread t(&runnable);

    // Alter the ThreadLocal here and then check that another thread still gets the default
    // initial value when it calls get.
    local.set("updated");

    t.start();
    try {
        t.join();
    } catch (InterruptedException& ie) {
        CPPUNIT_FAIL("Interrupted!!");
    }

    CPPUNIT_ASSERT_MESSAGE("ThreadLocal's value in this Thread should be 'updated'",
                           local.get() == "updated");
}
