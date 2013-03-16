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

#ifndef _DECAF_UTIL_UUIDTEST_H_
#define _DECAF_UTIL_UUIDTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace decaf{
namespace util{

    class UUIDTest : public CppUnit::TestFixture {

        CPPUNIT_TEST_SUITE( UUIDTest );
        CPPUNIT_TEST( testConstructor1 );
        CPPUNIT_TEST( testGetLeastSignificantBits );
        CPPUNIT_TEST( testGetMostSignificantBits );
        CPPUNIT_TEST( testVersion );
        CPPUNIT_TEST( testVariant );
        CPPUNIT_TEST( testTimestamp );
        CPPUNIT_TEST( testClockSequence );
        CPPUNIT_TEST( testNode );
        CPPUNIT_TEST( testCompareTo );
        CPPUNIT_TEST( testHashCode );
        CPPUNIT_TEST( testEquals );
        CPPUNIT_TEST( testToString );
        CPPUNIT_TEST( testRandomUUID );
        CPPUNIT_TEST( testNameUUIDFromBytes );
        CPPUNIT_TEST( testFromString );
        CPPUNIT_TEST( testFromStringStringException );
        CPPUNIT_TEST_SUITE_END();

    public:

        UUIDTest();
        virtual ~UUIDTest() {}

        void testConstructor1();
        void testGetLeastSignificantBits();
        void testGetMostSignificantBits();
        void testVersion();
        void testVariant();
        void testTimestamp();
        void testClockSequence();
        void testNode();
        void testCompareTo();
        void testHashCode();
        void testEquals();
        void testToString();
        void testRandomUUID();
        void testNameUUIDFromBytes();
        void testFromString();
        void testFromStringStringException();

    };

}}

#endif /*_DECAF_UTIL_UUIDTEST_H_*/
