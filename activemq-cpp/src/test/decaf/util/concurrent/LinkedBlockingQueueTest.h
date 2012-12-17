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

#ifndef _DECAF_UTIL_CONCURRENT_LINKEDBLOCKINGQUEUETEST_H_
#define _DECAF_UTIL_CONCURRENT_LINKEDBLOCKINGQUEUETEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <decaf/util/concurrent/ExecutorsTestSupport.h>

namespace decaf {
namespace util {
namespace concurrent {

    class LinkedBlockingQueueTest : public ExecutorsTestSupport {

        CPPUNIT_TEST_SUITE( LinkedBlockingQueueTest );
//        CPPUNIT_TEST( testConstructor1 );
//        CPPUNIT_TEST( testConstructor2 );
//        CPPUNIT_TEST( testConstructor3 );
//        CPPUNIT_TEST( testConstructor4 );
//        CPPUNIT_TEST( testEquals );
//        CPPUNIT_TEST( testEmptyFull );
//        CPPUNIT_TEST( testRemainingCapacity );
//        CPPUNIT_TEST( testOffer );
//        CPPUNIT_TEST( testAdd );
//        CPPUNIT_TEST( testAddAllSelf );
//        CPPUNIT_TEST( testAddAll1 );
//        CPPUNIT_TEST( testAddAll2 );
//        CPPUNIT_TEST( testPut );
//        CPPUNIT_TEST( testTake );
//        CPPUNIT_TEST( testPoll );
//        CPPUNIT_TEST( testTimedPoll1 );
//        CPPUNIT_TEST( testTimedPoll2 );
//        CPPUNIT_TEST( testPeek );
//        CPPUNIT_TEST( testElement );
//        CPPUNIT_TEST( testRemove );
//        CPPUNIT_TEST( testRemoveElement );
//        CPPUNIT_TEST( testRemoveElement2 );
//        CPPUNIT_TEST( testRemoveElementAndAdd );
//        CPPUNIT_TEST( testContains );
//        CPPUNIT_TEST( testClear );
//        CPPUNIT_TEST( testContainsAll );
//        CPPUNIT_TEST( testRetainAll );
//        CPPUNIT_TEST( testRemoveAll );
//        CPPUNIT_TEST( testToArray );
//        CPPUNIT_TEST( testDrainToSelf );
//        CPPUNIT_TEST( testDrainTo );
//        CPPUNIT_TEST( testDrainToSelfN );
//        CPPUNIT_TEST( testDrainToWithActivePut );
//        CPPUNIT_TEST( testDrainToN );
//        CPPUNIT_TEST( testIterator );
//        CPPUNIT_TEST( testIteratorRemove );
//        CPPUNIT_TEST( testIteratorOrdering );
//        CPPUNIT_TEST( testWeaklyConsistentIteration );
//        CPPUNIT_TEST( testConcurrentPut );
//        CPPUNIT_TEST( testConcurrentTake );
//        CPPUNIT_TEST( testConcurrentPutAndTake );
//        CPPUNIT_TEST( testBlockingPut );
//        CPPUNIT_TEST( testTimedOffer );
//        CPPUNIT_TEST( testTakeFromEmpty );
//        CPPUNIT_TEST( testBlockingTake );
//        CPPUNIT_TEST( testInterruptedTimedPoll );
//        CPPUNIT_TEST( testTimedPollWithOffer );
//        CPPUNIT_TEST( testOfferInExecutor );
        CPPUNIT_TEST( testPollInExecutor );
        CPPUNIT_TEST_SUITE_END();

    public:

        static const int SIZE;

    public:

        LinkedBlockingQueueTest();
        virtual ~LinkedBlockingQueueTest();

        void testConstructor1();
        void testConstructor2();
        void testConstructor3();
        void testConstructor4();
        void testEquals();
        void testEmptyFull();
        void testRemainingCapacity();
        void testOffer();
        void testAdd();
        void testAddAllSelf();
        void testAddAll1();
        void testAddAll2();
        void testPut();
        void testTake();
        void testPoll();
        void testTimedPoll1();
        void testTimedPoll2();
        void testPeek();
        void testElement();
        void testRemove();
        void testRemoveElement();
        void testRemoveElement2();
        void testRemoveElementAndAdd();
        void testContains();
        void testClear();
        void testContainsAll();
        void testRetainAll();
        void testRemoveAll();
        void testToArray();
        void testDrainToSelf();
        void testDrainTo();
        void testDrainToSelfN();
        void testDrainToWithActivePut();
        void testDrainToN();
        void testIterator();
        void testIteratorRemove();
        void testIteratorOrdering();
        void testWeaklyConsistentIteration();
        void testConcurrentPut();
        void testConcurrentTake();
        void testConcurrentPutAndTake();
        void testBlockingPut();
        void testTimedOffer();
        void testTakeFromEmpty();
        void testBlockingTake();
        void testInterruptedTimedPoll();
        void testTimedPollWithOffer();
        void testOfferInExecutor();
        void testPollInExecutor();

    };

}}}

#endif /* _DECAF_UTIL_CONCURRENT_LINKEDBLOCKINGQUEUETEST_H_ */
