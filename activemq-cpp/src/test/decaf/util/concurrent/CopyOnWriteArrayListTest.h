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

#ifndef _DECAF_UTIL_CONCURRENT_COPYONWRITEARRAYLISTTEST_H_
#define _DECAF_UTIL_CONCURRENT_COPYONWRITEARRAYLISTTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace decaf {
namespace util {
namespace concurrent {

    class CopyOnWriteArrayListTest : public CppUnit::TestFixture {

        CPPUNIT_TEST_SUITE( CopyOnWriteArrayListTest );
        CPPUNIT_TEST( testConstructor1 );
        CPPUNIT_TEST( testConstructor2 );
        CPPUNIT_TEST( testConstructor3 );
        CPPUNIT_TEST( testAddAll );
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
        CPPUNIT_TEST( testContainsAll );
        CPPUNIT_TEST( testGet );
        CPPUNIT_TEST( testSet );
        CPPUNIT_TEST( testSize );
        CPPUNIT_TEST( testIsEmpty );
        CPPUNIT_TEST( testToArray );
        CPPUNIT_TEST( testIndexOf1 );
        CPPUNIT_TEST( testIndexOf2 );
        CPPUNIT_TEST( testLastIndexOf1 );
        CPPUNIT_TEST( testLastIndexOf2 );
        CPPUNIT_TEST( testAddIndex );
        CPPUNIT_TEST( testAddAllIndex );
        CPPUNIT_TEST( testEquals );
        CPPUNIT_TEST( testRemove );
        CPPUNIT_TEST( testRemoveAt );
        CPPUNIT_TEST( testRemoveAll );
        CPPUNIT_TEST( testRetainAll );
        CPPUNIT_TEST( testAddIfAbsent1 );
        CPPUNIT_TEST( testAddIfAbsent2 );
        CPPUNIT_TEST( testAddAllAbsent );
        CPPUNIT_TEST( testIterator );
        CPPUNIT_TEST( testIteratorRemove );
        CPPUNIT_TEST( testListIterator1 );
        CPPUNIT_TEST( testListIterator2 );
        CPPUNIT_TEST( testAddAll1IndexOutOfBoundsException );
        CPPUNIT_TEST( testAddAll2IndexOutOfBoundsException );
        CPPUNIT_TEST( testListIterator1IndexOutOfBoundsException );
        CPPUNIT_TEST( testListIterator2IndexOutOfBoundsException );
        CPPUNIT_TEST( testAdd1IndexOutOfBoundsException );
        CPPUNIT_TEST( testAdd2IndexOutOfBoundsException );
        CPPUNIT_TEST( testRemoveAt1IndexOutOfBounds );
        CPPUNIT_TEST( testRemoveAt2IndexOutOfBounds );
        CPPUNIT_TEST( testGet1IndexOutOfBoundsException );
        CPPUNIT_TEST( testGet2IndexOutOfBoundsException );
        CPPUNIT_TEST( testSet1IndexOutOfBoundsException );
        CPPUNIT_TEST( testSet2IndexOutOfBoundsException );
//        CPPUNIT_TEST( testConcurrentRandomAddRemoveAndIterate );
        CPPUNIT_TEST_SUITE_END();

    private:

        static const int SIZE;

    public:

        CopyOnWriteArrayListTest();
        virtual ~CopyOnWriteArrayListTest();

        void testConstructor1();
        void testConstructor2();
        void testConstructor3();
        void testAddAll();
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
        void testContainsAll();
        void testGet();
        void testSet();
        void testSize();
        void testIsEmpty();
        void testToArray();
        void testIndexOf1();
        void testIndexOf2();
        void testLastIndexOf1();
        void testLastIndexOf2();
        void testAddIndex();
        void testAddAllIndex();
        void testEquals();
        void testRemove();
        void testRemoveAt();
        void testRemoveAll();
        void testRetainAll();
        void testAddIfAbsent1();
        void testAddIfAbsent2();
        void testAddAllAbsent();
        void testIterator();
        void testIteratorRemove();
        void testListIterator1();
        void testListIterator2();
        void testAddAll1IndexOutOfBoundsException();
        void testAddAll2IndexOutOfBoundsException();
        void testListIterator1IndexOutOfBoundsException();
        void testListIterator2IndexOutOfBoundsException();
        void testAdd1IndexOutOfBoundsException();
        void testAdd2IndexOutOfBoundsException();
        void testRemoveAt1IndexOutOfBounds();
        void testRemoveAt2IndexOutOfBounds();
        void testGet1IndexOutOfBoundsException();
        void testGet2IndexOutOfBoundsException();
        void testSet1IndexOutOfBoundsException();
        void testSet2IndexOutOfBoundsException();
        void testConcurrentRandomAddRemoveAndIterate();

    };

}}}

#endif /* _DECAF_UTIL_CONCURRENT_COPYONWRITEARRAYLISTTEST_H_ */
