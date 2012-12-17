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

#ifndef _DECAF_UTIL_PRIORITYQUEUETEST_H_
#define _DECAF_UTIL_PRIORITYQUEUETEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace decaf {
namespace util {

    class PriorityQueueTest : public CppUnit::TestFixture {

        CPPUNIT_TEST_SUITE( PriorityQueueTest );
        CPPUNIT_TEST( testConstructor_1 );
        CPPUNIT_TEST( testConstructor_2 );
        CPPUNIT_TEST( testConstructor_3 );
        CPPUNIT_TEST( testAssignment );
        CPPUNIT_TEST( testSize );
        CPPUNIT_TEST( testOfferString );
        CPPUNIT_TEST( testPoll );
        CPPUNIT_TEST( testPollEmpty );
        CPPUNIT_TEST( testPeek );
        CPPUNIT_TEST( testPeekEmpty );
        CPPUNIT_TEST( testClear );
        CPPUNIT_TEST( testAdd );
        CPPUNIT_TEST( testAddAll );
        CPPUNIT_TEST( testRemove );
        CPPUNIT_TEST( testRemoveUsingComparator );
        CPPUNIT_TEST( testRemoveNotExists );
        CPPUNIT_TEST( testComparator );
        CPPUNIT_TEST( testIterator );
        CPPUNIT_TEST( testIteratorEmpty );
        CPPUNIT_TEST( testIteratorOutOfBounds );
        CPPUNIT_TEST( testIteratorRemove );
        CPPUNIT_TEST( testIteratorRemoveIllegalState );
        CPPUNIT_TEST_SUITE_END();

    public:

        PriorityQueueTest() {}
        virtual ~PriorityQueueTest() {}

        void testConstructor_1();
        void testConstructor_2();
        void testConstructor_3();
        void testAssignment();
        void testSize();
        void testOfferString();
        void testPoll();
        void testPollEmpty();
        void testPeek();
        void testPeekEmpty();
        void testClear();
        void testAdd();
        void testAddAll();
        void testRemove();
        void testRemoveUsingComparator();
        void testRemoveNotExists();
        void testComparator();
        void testIterator();
        void testIteratorEmpty();
        void testIteratorOutOfBounds();
        void testIteratorRemove();
        void testIteratorRemoveIllegalState();

    };

}}

#endif /* _DECAF_UTIL_PRIORITYQUEUETEST_H_ */
