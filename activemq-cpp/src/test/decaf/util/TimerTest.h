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

#ifndef _DECAF_UTIL_TIMERTEST_H_
#define _DECAF_UTIL_TIMERTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <decaf/util/concurrent/atomic/AtomicInteger.h>
#include <decaf/util/concurrent/Mutex.h>

namespace decaf {
namespace util {

    class TimerTest : public CppUnit::TestFixture {
    private:

        decaf::util::concurrent::atomic::AtomicInteger timerCounter;
        decaf::util::concurrent::Mutex gsync;

    private:

        CPPUNIT_TEST_SUITE( TimerTest );
        CPPUNIT_TEST( testConstructor );
        CPPUNIT_TEST( testCancel );
        CPPUNIT_TEST( testPurge );
        CPPUNIT_TEST( testSchedule_TimerTask_Date );
        CPPUNIT_TEST( testSchedule_TimerTask_Date2 );
        CPPUNIT_TEST( testSchedule_TimerTask_Long );
        CPPUNIT_TEST( testSchedule_TimerTask_Long2 );
        CPPUNIT_TEST( testSchedule_TimerTask_Long_Long );
        CPPUNIT_TEST( testSchedule_TimerTask_Long_Long2 );
        CPPUNIT_TEST( testSchedule_TimerTask_Date_Long );
        CPPUNIT_TEST( testSchedule_TimerTask_Date_Long2 );
        CPPUNIT_TEST( testScheduleAtFixedRate_TimerTask_Long_Long );
        CPPUNIT_TEST( testScheduleAtFixedRate_TimerTask_Long_Long2 );
        CPPUNIT_TEST( testScheduleAtFixedRate_TimerTask_Date_Long );
        CPPUNIT_TEST( testScheduleAtFixedRate_TimerTask_Date_Long2 );
        CPPUNIT_TEST_SUITE_END();

    public:

        TimerTest() : timerCounter(), gsync() {}
        virtual ~TimerTest() {}

        void testConstructor();
        void testCancel();
        void testPurge();
        void testSchedule_TimerTask_Date();
        void testSchedule_TimerTask_Date2();
        void testSchedule_TimerTask_Long();
        void testSchedule_TimerTask_Long2();
        void testSchedule_TimerTask_Long_Long();
        void testSchedule_TimerTask_Long_Long2();
        void testSchedule_TimerTask_Date_Long();
        void testSchedule_TimerTask_Date_Long2();
        void testScheduleAtFixedRate_TimerTask_Long_Long();
        void testScheduleAtFixedRate_TimerTask_Long_Long2();
        void testScheduleAtFixedRate_TimerTask_Date_Long();
        void testScheduleAtFixedRate_TimerTask_Date_Long2();

        virtual void setUp();

    };

}}

#endif /* _DECAF_UTIL_TIMERTEST_H_ */
