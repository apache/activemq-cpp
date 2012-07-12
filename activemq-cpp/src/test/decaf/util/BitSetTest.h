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

#ifndef _DECAF_UTIL_BITSETTEST_H_
#define _DECAF_UTIL_BITSETTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace decaf {
namespace util {

    class BitSetTest : public CppUnit::TestFixture {

        CPPUNIT_TEST_SUITE( BitSetTest );
        CPPUNIT_TEST( testConstructor );
        CPPUNIT_TEST( testConstructorI );
        CPPUNIT_TEST( testCopy );
        CPPUNIT_TEST( testCardinality );
        CPPUNIT_TEST( testEquals );
        CPPUNIT_TEST( testClear );
        CPPUNIT_TEST( testClearI );
        CPPUNIT_TEST( testClearII );
        CPPUNIT_TEST( testGetI );
        CPPUNIT_TEST( testGetII );
        CPPUNIT_TEST( testFlipI );
        CPPUNIT_TEST( testFlipII );
        CPPUNIT_TEST( testSetI );
        CPPUNIT_TEST( testSetIB );
        CPPUNIT_TEST( testSetII );
        CPPUNIT_TEST( testSetIIB );
        CPPUNIT_TEST( testIsEmpty );
        CPPUNIT_TEST( testIntersects );
        CPPUNIT_TEST( testAnd );
        CPPUNIT_TEST( testAndNot );
        CPPUNIT_TEST( testOR );
        CPPUNIT_TEST( testXOR );
        CPPUNIT_TEST( testSize );
        CPPUNIT_TEST( testLength );
        CPPUNIT_TEST( testToString );
        CPPUNIT_TEST( testNextSetBitI );
        CPPUNIT_TEST( testNextClearBitI );
        CPPUNIT_TEST( testNotModified );
        CPPUNIT_TEST_SUITE_END();

    public:

        BitSetTest();
        virtual ~BitSetTest();

        virtual void setUp();

        void testConstructor();
        void testConstructorI();
        void testCopy();
        void testEquals();
        void testCardinality();
        void testClear();
        void testClearI();
        void testClearII();
        void testGetI();
        void testGetII();
        void testFlipI();
        void testFlipII();
        void testSetI();
        void testSetIB();
        void testSetII();
        void testSetIIB();
        void testIsEmpty();
        void testIntersects();
        void testAnd();
        void testAndNot();
        void testOR();
        void testXOR();
        void testSize();
        void testToString();
        void testLength();
        void testNextSetBitI();
        void testNextClearBitI();
        void testNotModified();

    };

}}

#endif /* _DECAF_UTIL_BITSETTEST_H_ */
