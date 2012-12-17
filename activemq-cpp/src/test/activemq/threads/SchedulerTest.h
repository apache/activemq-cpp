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

#ifndef _ACTIVEMQ_THREADS_SCHEDULERTEST_H_
#define _ACTIVEMQ_THREADS_SCHEDULERTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace activemq {
namespace threads {

    class SchedulerTest : public CppUnit::TestFixture {

        CPPUNIT_TEST_SUITE( SchedulerTest );
        CPPUNIT_TEST( testConstructor );
        CPPUNIT_TEST( testScheduleNullRunnableThrows );
        CPPUNIT_TEST( testExecutePeriodically );
        CPPUNIT_TEST( testSchedualPeriodically );
        CPPUNIT_TEST( testExecuteAfterDelay );
        CPPUNIT_TEST( testCancel );
        CPPUNIT_TEST( testShutdown );
        CPPUNIT_TEST_SUITE_END();

    public:

        SchedulerTest();
        virtual ~SchedulerTest();

        void testConstructor();
        void testScheduleNullRunnableThrows();
        void testExecutePeriodically();
        void testSchedualPeriodically();
        void testExecuteAfterDelay();
        void testCancel();
        void testShutdown();

    };

}}

#endif /* _ACTIVEMQ_THREADS_SCHEDULERTEST_H_ */
