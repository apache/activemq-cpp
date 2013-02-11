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

#ifndef _DECAF_LANG_THREADTEST_H_
#define _DECAF_LANG_THREADTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace decaf{
namespace lang{

    class ThreadTest : public CppUnit::TestFixture {

      CPPUNIT_TEST_SUITE( ThreadTest );
      CPPUNIT_TEST( testConstructor );
      CPPUNIT_TEST( testConstructor_1 );
      CPPUNIT_TEST( testConstructor_2 );
      CPPUNIT_TEST( testConstructor_3 );
      CPPUNIT_TEST( testRun );
      CPPUNIT_TEST( testDelegate );
      CPPUNIT_TEST( testDerived );
      CPPUNIT_TEST( testJoin1 );
      CPPUNIT_TEST( testJoin2 );
      CPPUNIT_TEST( testJoin3 );
      CPPUNIT_TEST( testJoin4 );
      CPPUNIT_TEST( testSetPriority );
      CPPUNIT_TEST( testIsAlive );
      CPPUNIT_TEST( testGetId );
      CPPUNIT_TEST( testGetState );
      CPPUNIT_TEST( testSleep );
      CPPUNIT_TEST( testSleep2Arg );
      CPPUNIT_TEST( testUncaughtExceptionHandler );
      CPPUNIT_TEST( testCurrentThread );
      CPPUNIT_TEST( testInterrupt );
      CPPUNIT_TEST( testInterrupted );
      CPPUNIT_TEST( testIsInterrupted );
      CPPUNIT_TEST( testSetName );
      CPPUNIT_TEST( testInterruptSleep );
      CPPUNIT_TEST( testInterruptJoin );
      CPPUNIT_TEST( testInterruptWait );
      CPPUNIT_TEST( testRapidCreateAndDestroy );
      CPPUNIT_TEST( testConcurrentRapidCreateAndDestroy );
      CPPUNIT_TEST( testCreatedButNotStarted );
      CPPUNIT_TEST_SUITE_END();

    public:

        virtual ~ThreadTest(){}

        virtual void setUp(){}
        virtual void tearDown(){}

        void testConstructor();
        void testConstructor_1();
        void testConstructor_2();
        void testConstructor_3();
        void testRun();
        void testDelegate();
        void testDerived();
        void testJoin1();
        void testJoin2();
        void testJoin3();
        void testJoin4();
        void testSetPriority();
        void testIsAlive();
        void testGetId();
        void testGetState();
        void testSleep();
        void testSleep2Arg();
        void testUncaughtExceptionHandler();
        void testCurrentThread();
        void testInterrupt();
        void testInterrupted();
        void testIsInterrupted();
        void testSetName();
        void testInterruptSleep();
        void testInterruptJoin();
        void testInterruptWait();
        void testRapidCreateAndDestroy();
        void testConcurrentRapidCreateAndDestroy();
        void testCreatedButNotStarted();

    };

}}

#endif /*_DECAF_LANG_THREADTEST_H_*/
