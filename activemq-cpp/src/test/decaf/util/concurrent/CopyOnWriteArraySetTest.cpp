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

#include "CopyOnWriteArraySetTest.h"

#include <decaf/util/concurrent/CopyOnWriteArraySet.h>
#include <decaf/util/StlList.h>

using namespace decaf;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
namespace {

    void populate( CopyOnWriteArraySet<int>& set, int n ) {

        CPPUNIT_ASSERT( set.isEmpty() );

        for( int i = 0; i < n; ++i ) {
            set.add( i );
        }

        CPPUNIT_ASSERT( !set.isEmpty());
        CPPUNIT_ASSERT_EQUAL( n, set.size() );
    }
}

////////////////////////////////////////////////////////////////////////////////
const int CopyOnWriteArraySetTest::SIZE = 50;

////////////////////////////////////////////////////////////////////////////////
CopyOnWriteArraySetTest::CopyOnWriteArraySetTest() {
}

////////////////////////////////////////////////////////////////////////////////
CopyOnWriteArraySetTest::~CopyOnWriteArraySetTest() {
}

////////////////////////////////////////////////////////////////////////////////
void CopyOnWriteArraySetTest::testConstructor1() {

    CopyOnWriteArraySet<int> set;
    CPPUNIT_ASSERT( set.isEmpty() );
    CPPUNIT_ASSERT( set.size() == 0 );

    CopyOnWriteArraySet<std::string> strSet;
    CPPUNIT_ASSERT( strSet.isEmpty() );
    CPPUNIT_ASSERT( strSet.size() == 0 );
}

////////////////////////////////////////////////////////////////////////////////
void CopyOnWriteArraySetTest::testConstructor2() {

    StlList<int> intsList;

    for( int i = 0; i < SIZE; ++i ) {
        intsList.add( i );
    }

    CopyOnWriteArraySet<int> array( intsList );
    CPPUNIT_ASSERT( !array.isEmpty() );
    CPPUNIT_ASSERT( array.size() == SIZE );

    for( int i = 0; i < SIZE; ++i ) {
        CPPUNIT_ASSERT( array.contains( i ) );
    }
}

////////////////////////////////////////////////////////////////////////////////
void CopyOnWriteArraySetTest::testConstructor3() {

    int ints[SIZE];

    for( int i = 0; i < SIZE; ++i ) {
        ints[i] = i;
    }

    CopyOnWriteArraySet<int> array( ints, SIZE );
    CPPUNIT_ASSERT( !array.isEmpty() );
    CPPUNIT_ASSERT( array.size() == SIZE );

    for( int i = 0; i < SIZE; ++i ) {
        CPPUNIT_ASSERT( array.contains( i ) );
    }
}

////////////////////////////////////////////////////////////////////////////////
void CopyOnWriteArraySetTest::testAddAll1() {

    CopyOnWriteArraySet<int> full;
    populate( full, 3 );

    StlList<int> intsList;
    intsList.add( 3 );
    intsList.add( 4 );
    intsList.add( 5 );

    full.addAll( intsList );
    CPPUNIT_ASSERT_EQUAL( 6, full.size() );
}

////////////////////////////////////////////////////////////////////////////////
void CopyOnWriteArraySetTest::testAddAll2() {

    CopyOnWriteArraySet<int> full;
    populate( full, 3 );

    StlList<int> intsList;
    intsList.add( 3 );
    intsList.add( 4 );
    intsList.add( 1 );

    full.addAll( intsList );
    CPPUNIT_ASSERT_EQUAL( 5, full.size() );
}

////////////////////////////////////////////////////////////////////////////////
void CopyOnWriteArraySetTest::testAdd1() {

    CopyOnWriteArraySet<int> full;
    populate( full, 3 );

    full.add( 1 );
    CPPUNIT_ASSERT_EQUAL( 3, full.size() );
}

////////////////////////////////////////////////////////////////////////////////
void CopyOnWriteArraySetTest::testAdd2() {

    CopyOnWriteArraySet<int> full;
    populate( full, 3 );

    full.add( 3 );
    CPPUNIT_ASSERT_EQUAL( 4, full.size() );
    CPPUNIT_ASSERT( full.contains( 3 ) );
}

////////////////////////////////////////////////////////////////////////////////
void CopyOnWriteArraySetTest::testClear() {

    CopyOnWriteArraySet<int> full;
    populate( full, 3 );

    full.clear();
    CPPUNIT_ASSERT_EQUAL( 0, full.size() );
}

////////////////////////////////////////////////////////////////////////////////
void CopyOnWriteArraySetTest::testContains() {

    CopyOnWriteArraySet<int> full;
    populate( full, 3 );

    CPPUNIT_ASSERT( full.contains( 1 ) );
    CPPUNIT_ASSERT( !full.contains( 5 ) );
}

////////////////////////////////////////////////////////////////////////////////
void CopyOnWriteArraySetTest::testEquals() {

    CopyOnWriteArraySet<int> a;
    populate( a, 3 );
    CopyOnWriteArraySet<int> b;
    populate( b, 3 );

    CPPUNIT_ASSERT( a.equals( b ) );
    CPPUNIT_ASSERT( b.equals( a ) );
    a.add( 42 );

    CPPUNIT_ASSERT( !a.equals( b ) );
    CPPUNIT_ASSERT( !b.equals( a ) );
    b.add( 42 );

    CPPUNIT_ASSERT( a.equals( b ) );
    CPPUNIT_ASSERT( b.equals( a ) );
}

////////////////////////////////////////////////////////////////////////////////
void CopyOnWriteArraySetTest::testContainsAll() {

    CopyOnWriteArraySet<int> full;
    populate( full, 3 );

    StlList<int> intsList;
    intsList.add( 1 );
    intsList.add( 2 );

    CPPUNIT_ASSERT( full.containsAll( intsList ) );

    intsList.add( 6 );
    CPPUNIT_ASSERT( !full.containsAll( intsList ) );
}

////////////////////////////////////////////////////////////////////////////////
void CopyOnWriteArraySetTest::testIsEmpty() {

    CopyOnWriteArraySet<int> empty;
    CopyOnWriteArraySet<int> full;
    populate( full, 3 );

    CPPUNIT_ASSERT( empty.isEmpty() );
    CPPUNIT_ASSERT( !full.isEmpty() );
}

////////////////////////////////////////////////////////////////////////////////
void CopyOnWriteArraySetTest::testIterator() {

    CopyOnWriteArraySet<int> full;
    populate( full, 3 );

    std::auto_ptr< Iterator<int> > iter( full.iterator() );
    int j;
    for( j = 0; iter->hasNext(); j++ ) {
        CPPUNIT_ASSERT_EQUAL( j, iter->next() );
    }

    CPPUNIT_ASSERT_EQUAL( 3, j );
}

////////////////////////////////////////////////////////////////////////////////
void CopyOnWriteArraySetTest::testIteratorRemove() {

    CopyOnWriteArraySet<int> full;
    populate( full, 3 );

    std::auto_ptr< Iterator<int> > iter( full.iterator() );
    iter->next();

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a UnsupportedOperationException",
        iter->remove(),
        UnsupportedOperationException );
}

////////////////////////////////////////////////////////////////////////////////
void CopyOnWriteArraySetTest::testRemoveAll() {

    CopyOnWriteArraySet<int> full;
    populate( full, 3 );

    StlList<int> intsList;
    intsList.add( 1 );
    intsList.add( 2 );

    full.removeAll( intsList );
    CPPUNIT_ASSERT_EQUAL( 1, full.size() );
}

////////////////////////////////////////////////////////////////////////////////
void CopyOnWriteArraySetTest::testRemove() {

    CopyOnWriteArraySet<int> full;
    populate( full, 3 );

    full.remove( 1 );

    CPPUNIT_ASSERT( !full.contains( 1 ) );
    CPPUNIT_ASSERT_EQUAL( 2, full.size() );
}

////////////////////////////////////////////////////////////////////////////////
void CopyOnWriteArraySetTest::testSize() {

    CopyOnWriteArraySet<int> empty;
    CopyOnWriteArraySet<int> full;
    populate( full, 3 );

    CPPUNIT_ASSERT_EQUAL( 3, full.size() );
    CPPUNIT_ASSERT_EQUAL( 0, empty.size() );
}

////////////////////////////////////////////////////////////////////////////////
void CopyOnWriteArraySetTest::testToArray() {

    CopyOnWriteArraySet<int> full;
    populate( full, 3 );

    std::vector<int> array = full.toArray();

    CPPUNIT_ASSERT_EQUAL( 3, (int)array.size() );
    CPPUNIT_ASSERT_EQUAL( 0, array[0] );
    CPPUNIT_ASSERT_EQUAL( 1, array[1] );
    CPPUNIT_ASSERT_EQUAL( 2, array[2] );
}
