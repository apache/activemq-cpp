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

#ifndef _DECAF_UTIL_LINKEDLISTTEST_H_
#define _DECAF_UTIL_LINKEDLISTTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace decaf {
namespace util {

    class LinkedListTest : public CppUnit::TestFixture
    {
        CPPUNIT_TEST_SUITE( LinkedListTest );
        CPPUNIT_TEST( testConstructor1 );
        CPPUNIT_TEST( testConstructor2 );
        CPPUNIT_TEST( testConstructor3 );
        CPPUNIT_TEST( testEquals );
        CPPUNIT_TEST( testGet );
        CPPUNIT_TEST( testSet );
        CPPUNIT_TEST( testAdd1 );
        CPPUNIT_TEST( testAdd2 );
        CPPUNIT_TEST( testAddAll1 );
        CPPUNIT_TEST( testAddAll2 );
        CPPUNIT_TEST( testAddAll3 );
        CPPUNIT_TEST( testAddAll4 );
        CPPUNIT_TEST( testAddAllSelfAsCollection1 );
        CPPUNIT_TEST( testAddAllSelfAsCollection2 );
        CPPUNIT_TEST( testAddFirst );
        CPPUNIT_TEST( testAddLast );
        CPPUNIT_TEST( testRemoveAtIndex );
        CPPUNIT_TEST( testRemoveByValue );
        CPPUNIT_TEST( testRemoveAll );
        CPPUNIT_TEST( testRetainAll );
        CPPUNIT_TEST( testGetFirst );
        CPPUNIT_TEST( testGetLast );
        CPPUNIT_TEST( testClear );
        CPPUNIT_TEST( testIndexOf );
        CPPUNIT_TEST( testLastIndexOf );
        CPPUNIT_TEST( testContains );
        CPPUNIT_TEST( testContainsAll );
        CPPUNIT_TEST( testToArray );
        CPPUNIT_TEST( testOffer );
        CPPUNIT_TEST( testPoll );
        CPPUNIT_TEST( testPeek );
        CPPUNIT_TEST( testElement );
        CPPUNIT_TEST( testQRemove );
        CPPUNIT_TEST( testOfferFirst );
        CPPUNIT_TEST( testOfferLast );
        CPPUNIT_TEST( testRemoveFirst );
        CPPUNIT_TEST( testRemoveLast );
        CPPUNIT_TEST( testPollFirst );
        CPPUNIT_TEST( testPollLast );
        CPPUNIT_TEST( testPeekFirst );
        CPPUNIT_TEST( testPeekLast );
        CPPUNIT_TEST( testPop );
        CPPUNIT_TEST( testPush );
        CPPUNIT_TEST( testIterator1 );
        CPPUNIT_TEST( testIterator2 );
        CPPUNIT_TEST( testListIterator1 );
        CPPUNIT_TEST( testListIterator2 );
        CPPUNIT_TEST( testListIterator3 );
        CPPUNIT_TEST( testListIterator4 );
        CPPUNIT_TEST( testListIterator1IndexOutOfBoundsException );
        CPPUNIT_TEST( testListIterator2IndexOutOfBoundsException );
        CPPUNIT_TEST( testDescendingIterator );
        CPPUNIT_TEST( testRemoveFirstOccurrence );
        CPPUNIT_TEST( testRemoveLastOccurrence );
        CPPUNIT_TEST_SUITE_END();

    private:

        static const int SIZE;

    public:

        LinkedListTest();
        virtual ~LinkedListTest();

        void testConstructor1();
        void testConstructor2();
        void testConstructor3();
        void testEquals();
        void testGet();
        void testSet();
        void testAdd1();
        void testAdd2();
        void testAddAll1();
        void testAddAll2();
        void testAddAll3();
        void testAddAll4();
        void testAddAllSelfAsCollection1();
        void testAddAllSelfAsCollection2();
        void testAddFirst();
        void testAddLast();
        void testRemoveAtIndex();
        void testRemoveByValue();
        void testRemoveAll();
        void testRetainAll();
        void testGetFirst();
        void testGetLast();
        void testClear();
        void testIndexOf();
        void testLastIndexOf();
        void testContains();
        void testContainsAll();
        void testToArray();
        void testOffer();
        void testPoll();
        void testPeek();
        void testElement();
        void testQRemove();
        void testOfferFirst();
        void testOfferLast();
        void testRemoveFirst();
        void testRemoveLast();
        void testPollFirst();
        void testPollLast();
        void testPeekFirst();
        void testPeekLast();
        void testPop();
        void testPush();
        void testIterator1();
        void testIterator2();
        void testListIterator1();
        void testListIterator2();
        void testListIterator3();
        void testListIterator4();
        void testListIterator1IndexOutOfBoundsException();
        void testListIterator2IndexOutOfBoundsException();
        void testDescendingIterator();
        void testRemoveFirstOccurrence();
        void testRemoveLastOccurrence();

    };

}}

#endif /* _DECAF_UTIL_LINKEDLISTTEST_H_ */
