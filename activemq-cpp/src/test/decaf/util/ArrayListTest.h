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

#ifndef _DECAF_UTIL_ARRAYLISTTEST_H_
#define _DECAF_UTIL_ARRAYLISTTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace decaf {
namespace util {

    class ArrayListTest : public CppUnit::TestFixture {

        CPPUNIT_TEST_SUITE( ArrayListTest );
        CPPUNIT_TEST( testConstructor1 );
        CPPUNIT_TEST( testConstructor2 );
        CPPUNIT_TEST( testConstructor3 );
        CPPUNIT_TEST( testAdd1 );
        CPPUNIT_TEST( testAdd2 );
        CPPUNIT_TEST( testAdd3 );
        CPPUNIT_TEST( testAddAll1 );
        CPPUNIT_TEST( testAddAll2 );
        CPPUNIT_TEST( testAddAll3 );
        CPPUNIT_TEST( testAddAll4 );
        CPPUNIT_TEST( testAddAll5 );
        CPPUNIT_TEST( testAddAll6 );
        CPPUNIT_TEST( testAddAll7 );
        CPPUNIT_TEST( testAddAll8 );
        CPPUNIT_TEST( testAddAll9 );
        CPPUNIT_TEST( testClear );
        CPPUNIT_TEST( testContains );
        CPPUNIT_TEST( testEnsureCapacity );
        CPPUNIT_TEST( testGet );
        CPPUNIT_TEST( testSet );
        CPPUNIT_TEST( testIndexOf );
        CPPUNIT_TEST( testIsEmpty );
        CPPUNIT_TEST( testLastIndexOf );
        CPPUNIT_TEST( testRemove );
        CPPUNIT_TEST( testRemoveAt );
        CPPUNIT_TEST( testSize );
        CPPUNIT_TEST( testToString );
        CPPUNIT_TEST( testToArray );
        CPPUNIT_TEST( testTrimToSize );
        CPPUNIT_TEST( testOverrideSize );
        CPPUNIT_TEST( testRemoveAll );
        CPPUNIT_TEST( testRetainAll );
        CPPUNIT_TEST( testListIterator1IndexOutOfBoundsException );
        CPPUNIT_TEST( testListIterator2IndexOutOfBoundsException );
        CPPUNIT_TEST_SUITE_END();

    private:

        static const int SIZE;

    public:

        ArrayListTest();
        virtual ~ArrayListTest();

        void testConstructor1();
        void testConstructor2();
        void testConstructor3();
        void testAdd1();
        void testAdd2();
        void testAdd3();
        void testAddAll1();
        void testAddAll2();
        void testAddAll3();
        void testAddAll4();
        void testAddAll5();
        void testAddAll6();
        void testAddAll7();
        void testAddAll8();
        void testAddAll9();
        void testClear();
        void testContains();
        void testEnsureCapacity();
        void testGet();
        void testSet();
        void testIndexOf();
        void testIsEmpty();
        void testLastIndexOf();
        void testRemove();
        void testRemoveAt();
        void testSize();
        void testToString();
        void testToArray();
        void testTrimToSize();
        void testOverrideSize();
        void testRemoveAll();
        void testRetainAll();
        void testListIterator1IndexOutOfBoundsException();
        void testListIterator2IndexOutOfBoundsException();

    };

}}

#endif /* _DECAF_UTIL_ARRAYLISTTEST_H_ */
