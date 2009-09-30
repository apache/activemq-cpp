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
class ParkTestThread : public Thread {
public:

    virtual void run() {
        try{
            LockSupport::park();
        } catch(...) {
            CPPUNIT_FAIL("Caught an unexpected exception");
        }
    }

};

////////////////////////////////////////////////////////////////////////////////
void LockSupportTest::testPark() {

    ParkTestThread t;

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
class ParkTest2Thread : public Thread {
public:

    virtual void run() {
        try{
            Thread::sleep( 1000 );
            LockSupport::park();
        } catch(...) {
            CPPUNIT_FAIL("Caught an unexpected exception");
        }
    }

};

////////////////////////////////////////////////////////////////////////////////
void LockSupportTest::testPark2() {

    ParkTest2Thread t;

    try {
        t.start();
        long long before = System::currentTimeMillis();
        LockSupport::unpark( &t );
        long long after = System::currentTimeMillis();
        CPPUNIT_ASSERT( after - before < 1000 );
        t.join();
    } catch(...) {
        CPPUNIT_FAIL("Caught an unexpected exception");
    }
}

////////////////////////////////////////////////////////////////////////////////
class ParkNanosTestThread : public Thread {
public:

    virtual void run() {
        try{
            LockSupport::parkNanos( 1111 );
        } catch(...) {
            CPPUNIT_FAIL("Caught an unexpected exception");
        }
    }
};

////////////////////////////////////////////////////////////////////////////////
void LockSupportTest::testParkNanos() {

    ParkNanosTestThread t;

    try {
        t.start();
        t.join();
    } catch(...) {
        CPPUNIT_FAIL("Caught an unexpected exception");
    }
}

////////////////////////////////////////////////////////////////////////////////
class ParkUntilTestThread : public Thread {
public:

    virtual void run() {
        try{
            long long deadline = Date().getTime() + 100;
            LockSupport::parkUntil( deadline );
        } catch(...) {
            CPPUNIT_FAIL("Caught an unexpected exception");
        }
    }
};

////////////////////////////////////////////////////////////////////////////////
void LockSupportTest::testParkUntil() {

    ParkUntilTestThread t;

    try {
        t.start();
        t.join();
    } catch(...) {
        CPPUNIT_FAIL("Caught an unexpected exception");
    }
}
