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

#ifndef _DECAF_UTIL_CONCURRENT_TIMEUNITTEST_H_
#define _DECAF_UTIL_CONCURRENT_TIMEUNITTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace decaf {
namespace util {
namespace concurrent {

    class TimeUnitTest : public CppUnit::TestFixture {

        CPPUNIT_TEST_SUITE( TimeUnitTest );
        CPPUNIT_TEST( testConvert1 );
        CPPUNIT_TEST( testConvert2 );
        CPPUNIT_TEST( testToNanos );
        CPPUNIT_TEST( testToMicros );
        CPPUNIT_TEST( testToMillis );
        CPPUNIT_TEST( testToSeconds );
        CPPUNIT_TEST( testConvertSaturate );
        CPPUNIT_TEST( testToNanosSaturate );
        CPPUNIT_TEST( testToString );
        CPPUNIT_TEST( testTimedWait );
        CPPUNIT_TEST( testSleep );
        CPPUNIT_TEST( testToMinutes );
        CPPUNIT_TEST( testToHours );
        CPPUNIT_TEST( testToDays );
        CPPUNIT_TEST( testValueOf );
        CPPUNIT_TEST_SUITE_END();

    public:

        TimeUnitTest() {}
        virtual ~TimeUnitTest() {}

        void testConvert1();
        void testConvert2();
        void testToNanos();
        void testToMicros();
        void testToMillis();
        void testToSeconds();
        void testToMinutes();
        void testToHours();
        void testToDays();
        void testConvertSaturate();
        void testToNanosSaturate();
        void testToString();
        void testTimedWait();
        void testSleep();
        void testValueOf();

    };

}}}

#endif /*_DECAF_UTIL_CONCURRENT_TIMEUNITTEST_H_ */
