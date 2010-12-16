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

#include "PriorityQueueTest.h"

#include <decaf/util/PriorityQueue.h>
#include <decaf/util/Comparator.h>
#include <decaf/util/LinkedList.h>

#include <algorithm>
#include <memory>

using namespace std;
using namespace decaf;
using namespace decaf::util;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
namespace decaf {
namespace util {

    class MockComparatorStringByLength : public decaf::util::Comparator<std::string> {

        virtual bool operator() ( const std::string& left, const std::string& right ) const {
            return left.size() == right.size();
        }

        virtual int compare( const std::string& o1, const std::string& o2 ) const {
            return o1.size() < o2.size() ? -1 : o1.size() > o2.size() ? 1 : 0;
        }

    };

}}

////////////////////////////////////////////////////////////////////////////////
void PriorityQueueTest::testConstructor_1() {

    PriorityQueue<int> pqueue;

    CPPUNIT_ASSERT( pqueue.isEmpty() );
    CPPUNIT_ASSERT( pqueue.size() == 0 );
    CPPUNIT_ASSERT( pqueue.comparator() != NULL );
}

////////////////////////////////////////////////////////////////////////////////
void PriorityQueueTest::testConstructor_2() {

    PriorityQueue<int> pqueue( 1024 );

    CPPUNIT_ASSERT( pqueue.isEmpty() );
    CPPUNIT_ASSERT( pqueue.size() == 0 );
    CPPUNIT_ASSERT( pqueue.comparator() != NULL );
}

////////////////////////////////////////////////////////////////////////////////
void PriorityQueueTest::testConstructor_3() {

    PriorityQueue<int> intQueue;
    LinkedList<int> collection;

    int array[] = { 12, 2, 456, -11, 99, 111, 456 };

    for( std::size_t ix = 0; ix < sizeof(array)/sizeof(int); ++ix ) {
        intQueue.offer( array[ix] );
        collection.add( array[ix] );
    }

    PriorityQueue<int> copy( collection );

    CPPUNIT_ASSERT( copy.size() == intQueue.size() );

    std::auto_ptr< Iterator<int> > q_iter( intQueue.iterator() );
    std::auto_ptr< Iterator<int> > c_iter( copy.iterator() );

    while( q_iter->hasNext() && c_iter->hasNext() ) {
        CPPUNIT_ASSERT( q_iter->next() == c_iter->next() );
    }
}

////////////////////////////////////////////////////////////////////////////////
void PriorityQueueTest::testAssignment() {

    PriorityQueue<int> intQueue;
    LinkedList<int> collection;

    int array[] = { 12, 2, 456, -11, 99, 111, 456 };

    for( std::size_t ix = 0; ix < sizeof(array)/sizeof(int); ++ix ) {
        intQueue.offer( array[ix] );
        collection.add( array[ix] );
    }

    PriorityQueue<int> copy = collection;

    CPPUNIT_ASSERT( copy.size() == intQueue.size() );

    std::auto_ptr< Iterator<int> > q_iter( intQueue.iterator() );
    std::auto_ptr< Iterator<int> > c_iter( copy.iterator() );

    while( q_iter->hasNext() && c_iter->hasNext() ) {
        CPPUNIT_ASSERT( q_iter->next() == c_iter->next() );
    }

    PriorityQueue<int> assigned = copy;

    std::auto_ptr< Iterator<int> > a1_iter( copy.iterator() );
    std::auto_ptr< Iterator<int> > a2_iter( assigned.iterator() );

    while( a1_iter->hasNext() && a2_iter->hasNext() ) {
        CPPUNIT_ASSERT( a1_iter->next() == a2_iter->next() );
    }
}

////////////////////////////////////////////////////////////////////////////////
void PriorityQueueTest::testSize() {

    PriorityQueue<int> intQueue;

    CPPUNIT_ASSERT( 0 == intQueue.size() );
    int array[] = { 2, 45, 7, -12, 9 };
    for( int i = 0; i < 5; i++ ) {
        intQueue.offer( array[i] );
    }

    CPPUNIT_ASSERT( sizeof(array)/sizeof(int) == intQueue.size() );
}

////////////////////////////////////////////////////////////////////////////////
void PriorityQueueTest::testOfferString() {

    PriorityQueue<std::string> queue( 10, new MockComparatorStringByLength() );

    std::string array[] = { "AAAAA", "AA", "AAAA", "AAAAAAAA" };
    for( int i = 0; i < 4; i++ ) {
        queue.offer( array[i] );
    }

    std::string sortedArray[] = { "AA", "AAAA", "AAAAA", "AAAAAAAA" };
    for( int i = 0; i < 4; i++ ) {
        CPPUNIT_ASSERT( sortedArray[i] == queue.remove() );
    }

    std::string result;
    CPPUNIT_ASSERT( 0 == queue.size() );
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should Throw a NoSuchElementException",
        queue.remove(),
        decaf::util::NoSuchElementException );
}

////////////////////////////////////////////////////////////////////////////////
void PriorityQueueTest::testPoll() {

    PriorityQueue<int> intQueue;
    int array[] = { 52, 12, 42, 7, 111 };
    int sorted[] = { 7, 12, 42, 52, 111 };

    for( int i = 0; i < 5; i++ ) {
        intQueue.offer( array[i] );
    }

    int result = 0;
    for( int i = 0; i < 5; i++ ) {
        CPPUNIT_ASSERT( intQueue.poll( result ) );
        CPPUNIT_ASSERT( sorted[i] == result );
    }

    CPPUNIT_ASSERT( 0 == intQueue.size() );
    CPPUNIT_ASSERT( intQueue.poll( result ) == false );
}

////////////////////////////////////////////////////////////////////////////////
void PriorityQueueTest::testPollEmpty() {

    double result;
    PriorityQueue<double> queue;
    CPPUNIT_ASSERT( 0 == queue.size() );
    CPPUNIT_ASSERT( queue.poll( result ) == false );
}

////////////////////////////////////////////////////////////////////////////////
void PriorityQueueTest::testPeek() {
    PriorityQueue<int> integerQueue;

    int array[] = { 2, 45, 7, -12, 9 };
    int sorted[] = { -12, 2, 7, 9, 45 };

    for( int i = 0; i < 5; i++ ) {
        integerQueue.add( array[i] );
    }

    int result = 0;

    CPPUNIT_ASSERT( integerQueue.peek( result ) == true );
    CPPUNIT_ASSERT( sorted[0] == result );

    CPPUNIT_ASSERT( integerQueue.peek( result ) == true );
    CPPUNIT_ASSERT( sorted[0] == result );
}

////////////////////////////////////////////////////////////////////////////////
void PriorityQueueTest::testPeekEmpty() {

    float result;
    PriorityQueue<float> queue;
    CPPUNIT_ASSERT( 0 == queue.size() );
    CPPUNIT_ASSERT( queue.peek( result ) == false );
}

////////////////////////////////////////////////////////////////////////////////
void PriorityQueueTest::testClear() {
    PriorityQueue<int> integerQueue;

    int array[] = {2, 45, 7, -12, 9};

    for( int i = 0; i < 5; i++ ) {
        integerQueue.offer( array[i] );
    }

    integerQueue.clear();
    CPPUNIT_ASSERT( integerQueue.isEmpty() );
}

////////////////////////////////////////////////////////////////////////////////
void PriorityQueueTest::testAdd() {
    PriorityQueue<int> integerQueue;

    int array[] = {2, 45, 7, -12, 9};
    int sorted[] = { -12, 2, 7, 9, 45 };

    for( int i = 0; i < 5; i++ ) {
        integerQueue.add( array[i] );
    }

    CPPUNIT_ASSERT( 5 == integerQueue.size() );

    for( int i = 0; i < 5; i++ ) {
        CPPUNIT_ASSERT( sorted[i] == integerQueue.remove() );
    }

    CPPUNIT_ASSERT( 0 == integerQueue.size() );
}

////////////////////////////////////////////////////////////////////////////////
void PriorityQueueTest::testAddAll() {
    PriorityQueue<int> integerQueue;

    LinkedList<int> list;
    list.add( 2 );
    list.add( 45 );
    list.add( 7 );
    list.add( -12 );
    list.add( 9 );

    int sorted[] = { -12, 2, 7, 9, 45 };

    integerQueue.addAll( list );

    CPPUNIT_ASSERT( 5 == integerQueue.size() );

    for( int i = 0; i < 5; i++ ) {
        CPPUNIT_ASSERT( sorted[i] == integerQueue.remove() );
    }

    CPPUNIT_ASSERT( 0 == integerQueue.size() );
}

////////////////////////////////////////////////////////////////////////////////
void PriorityQueueTest::testRemove() {

    int array[] = { 2, 45, 7, -12, 9, 23, 17, 1118, 10, 16, 39 };

    PriorityQueue<int> integerQueue;

    for( int i = 0; i < 11; i++ ) {
        integerQueue.add( array[i] );
    }

    CPPUNIT_ASSERT( integerQueue.remove( 16 ) );

    int sorted[] = { -12, 2, 7, 9, 10, 17, 23, 39, 45, 1118 };

    int result = 0;
    for( int i = 0; i < 10; i++ ) {
        CPPUNIT_ASSERT( integerQueue.poll( result ) );
        CPPUNIT_ASSERT( sorted[i] == result );
    }

    CPPUNIT_ASSERT( 0 == integerQueue.size() );
}

////////////////////////////////////////////////////////////////////////////////
void PriorityQueueTest::testRemoveUsingComparator() {

    PriorityQueue<std::string> queue( 10, new MockComparatorStringByLength() );
    std::string array[] = {"AAAAA", "AA", "AAAA", "AAAAAAAA"};

    for( int i = 0; i < 4; i++ ) {
        queue.offer( array[i] );
    }

    // Prove that the comparator overrides the equality tests for remove, the Queue
    // doesn't contains BB but it should contain a string of length two.
    CPPUNIT_ASSERT( !queue.contains( "BB" ) );
    CPPUNIT_ASSERT( queue.remove( "BB" ) );
}

////////////////////////////////////////////////////////////////////////////////
void PriorityQueueTest::testRemoveNotExists() {

    int array[] = {2, 45, 7, -12, 9, 23, 17, 1118, 10, 16, 39};

    PriorityQueue<int> integerQueue;

    for( int i = 0; i < 11; i++ ) {
        integerQueue.offer( array[i] );
    }

    CPPUNIT_ASSERT( !integerQueue.remove( 111 ) );
}

////////////////////////////////////////////////////////////////////////////////
void PriorityQueueTest::testComparator() {

    PriorityQueue<std::string> queue1;
    CPPUNIT_ASSERT( queue1.comparator() != NULL );

    MockComparatorStringByLength* comparator = new MockComparatorStringByLength();
    PriorityQueue<std::string> queue2( 100, comparator );
    CPPUNIT_ASSERT( comparator == queue2.comparator().get() );
}

////////////////////////////////////////////////////////////////////////////////
void PriorityQueueTest::testIterator() {

    PriorityQueue<int> integerQueue;

    int array[] = {2, 45, 7, -12, 9};
    int sorted[] = {-12, 2, 7, 9, 45};

    for( int i = 0; i < 5; i++ ) {
        integerQueue.offer( array[i] );
    }

    std::auto_ptr< Iterator<int> > iter( integerQueue.iterator() );
    CPPUNIT_ASSERT( iter.get() != NULL );

    std::vector<int> result;

    while( iter->hasNext() ) {
        result.push_back( iter->next() );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should Throw a NoSuchElementException",
        iter->next(),
        NoSuchElementException );

    std::sort( result.begin(), result.end() );

    for( int i = 0; i < 5; i++ ) {
        CPPUNIT_ASSERT( result[i] == sorted[i] );
    }
}

////////////////////////////////////////////////////////////////////////////////
void PriorityQueueTest::testIteratorEmpty() {

    PriorityQueue<int> intQueue;
    std::auto_ptr< Iterator<int> > iter( intQueue.iterator() );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should Throw a NoSuchElementException",
        iter->next(),
        NoSuchElementException );

    iter.reset( intQueue.iterator() );
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should Throw a IllegalStateException",
        iter->remove(),
        IllegalStateException );
}

////////////////////////////////////////////////////////////////////////////////
void PriorityQueueTest::testIteratorOutOfBounds() {

    PriorityQueue<int> intQueue;
    intQueue.offer( 0 );
    std::auto_ptr< Iterator<int> > iter( intQueue.iterator() );
    iter->next();
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should Throw a NoSuchElementException",
        iter->next(),
        NoSuchElementException );

    iter.reset( intQueue.iterator() );
    iter->next();
    iter->remove();
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should Throw a NoSuchElementException",
        iter->next(),
        NoSuchElementException );
}

////////////////////////////////////////////////////////////////////////////////
void PriorityQueueTest::testIteratorRemove() {

    PriorityQueue<int> intQueue;
    int array[] = {2, 45, 7, -12, 9};
    for( int i = 0; i < 5; i++ ) {
        intQueue.offer( array[i] );
    }
    std::auto_ptr< Iterator<int> > iter( intQueue.iterator() );
    CPPUNIT_ASSERT( iter.get() != NULL );
    for( int i = 0; i < 5; i++ ) {
        iter->next();
        if( 2 == i ) {
            iter->remove();
        }
    }
    CPPUNIT_ASSERT( 4 == intQueue.size() );

    iter.reset( intQueue.iterator() );
    std::vector<int> newArray;
    for( int i = 0; i < 4; i++ ) {
        newArray.push_back( iter->next() );
    }

    int result;
    std::sort( newArray.begin(), newArray.end() );
    for( int i = 0; i < intQueue.size(); i++ ) {
        CPPUNIT_ASSERT( intQueue.poll( result ) );
        CPPUNIT_ASSERT( newArray[i] == result );
    }

    const PriorityQueue<int> constQueue( intQueue );
    CPPUNIT_ASSERT( !constQueue.isEmpty() );
    CPPUNIT_ASSERT( constQueue.size() == intQueue.size() );

    std::auto_ptr< Iterator<int> > const_iter( constQueue.iterator() );
    const_iter->next();
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should Throw a UnsupportedOperationException",
        const_iter->remove(),
        UnsupportedOperationException );
}

////////////////////////////////////////////////////////////////////////////////
void PriorityQueueTest::testIteratorRemoveIllegalState() {

    PriorityQueue<int> intQueue;
    int array[] = {2, 45, 7, -12, 9};
    for( int i = 0; i < 5; i++ ) {
        intQueue.offer( array[i] );
    }
    std::auto_ptr< Iterator<int> > iter( intQueue.iterator() );
    CPPUNIT_ASSERT( iter.get() != NULL );
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should Throw a IllegalStateException",
        iter->remove(),
        IllegalStateException );

    iter->next();
    iter->remove();
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should Throw a IllegalStateException",
        iter->remove(),
        IllegalStateException );

}
