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

#include "CopyOnWriteArrayListTest.h"

#include <decaf/util/concurrent/CopyOnWriteArrayList.h>
#include <decaf/util/concurrent/ThreadPoolExecutor.h>
#include <decaf/util/concurrent/LinkedBlockingQueue.h>
#include <decaf/util/StlList.h>
#include <decaf/util/Random.h>
#include <decaf/lang/Integer.h>

using namespace decaf;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
const int CopyOnWriteArrayListTest::SIZE = 256;

////////////////////////////////////////////////////////////////////////////////
namespace {

    void populate( CopyOnWriteArrayList<int>& list, int n ) {

        CPPUNIT_ASSERT( list.isEmpty() );

        for( int i = 0; i < n; ++i ) {
            list.add( i );
        }

        CPPUNIT_ASSERT( !list.isEmpty());
        CPPUNIT_ASSERT_EQUAL( n, list.size() );
    }

    void populate( CopyOnWriteArrayList<std::string>& list, int n ) {

        CPPUNIT_ASSERT( list.isEmpty() );

        for( int i = 0; i < n; ++i ) {
            list.add( Integer::toString( i ) );
        }

        CPPUNIT_ASSERT( !list.isEmpty());
        CPPUNIT_ASSERT_EQUAL( n, list.size() );
    }

    void populate( std::vector<int>& list, int n ) {

        CPPUNIT_ASSERT( list.empty() );

        for( int i = 0; i < n; ++i ) {
            list.push_back( i );
        }

        CPPUNIT_ASSERT( !list.empty());
        CPPUNIT_ASSERT_EQUAL( n, (int)list.size() );
    }
}

////////////////////////////////////////////////////////////////////////////////
CopyOnWriteArrayListTest::CopyOnWriteArrayListTest() {
}

////////////////////////////////////////////////////////////////////////////////
CopyOnWriteArrayListTest::~CopyOnWriteArrayListTest() {
}

////////////////////////////////////////////////////////////////////////////////
void CopyOnWriteArrayListTest::testConstructor1() {

    CopyOnWriteArrayList<int> array;
    CPPUNIT_ASSERT( array.isEmpty() );
    CPPUNIT_ASSERT( array.size() == 0 );

    CopyOnWriteArrayList<std::string> strArray;
    CPPUNIT_ASSERT( strArray.isEmpty() );
    CPPUNIT_ASSERT( strArray.size() == 0 );
}

////////////////////////////////////////////////////////////////////////////////
void CopyOnWriteArrayListTest::testConstructor2() {

    StlList<int> intsList;

    for( int i = 0; i < SIZE; ++i ) {
        intsList.add( i );
    }

    CopyOnWriteArrayList<int> array( intsList );
    CPPUNIT_ASSERT( !array.isEmpty() );
    CPPUNIT_ASSERT( array.size() == SIZE );

    for( int i = 0; i < SIZE; ++i ) {
        CPPUNIT_ASSERT_EQUAL( intsList.get( i ), array.get( i ) );
    }
}

////////////////////////////////////////////////////////////////////////////////
void CopyOnWriteArrayListTest::testConstructor3() {

    int ints[SIZE];

    for( int i = 0; i < SIZE; ++i ) {
        ints[i] = i;
    }

    CopyOnWriteArrayList<int> array( ints, SIZE );
    CPPUNIT_ASSERT( !array.isEmpty() );
    CPPUNIT_ASSERT( array.size() == SIZE );

    for( int i = 0; i < SIZE; ++i ) {
        CPPUNIT_ASSERT_EQUAL( ints[i], array.get( i ) );
    }
}

////////////////////////////////////////////////////////////////////////////////
void CopyOnWriteArrayListTest::testAddAll() {

    CopyOnWriteArrayList<int> list;
    for( int i = 0; i < 3; ++i ) {
        list.add( i );
    }

    StlList<int> collection;
    for( int i = 3; i < 6; ++i ) {
        collection.add( i );
    }

    list.addAll( collection );
    CPPUNIT_ASSERT_EQUAL( 6, list.size() );
}

////////////////////////////////////////////////////////////////////////////////
void CopyOnWriteArrayListTest::testAddAll1() {

    CopyOnWriteArrayList<int> array;
    populate( array, 100 );
    std::vector<int> mirror;
    populate( mirror, 100 );

    array.addAll( 50, array );
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "Returned incorrect size after adding to existing list", 200, array.size() );

    for( int i = 0; i < 50; i++ ) {
        CPPUNIT_ASSERT_MESSAGE( "Manipulated elements < index", array.get( i ) == mirror[i] );
    }

    for( int i = 0; i >= 50 && ( i < 150 ); i++ ) {
        CPPUNIT_ASSERT_MESSAGE( "Failed to ad elements properly", array.get( i ) == mirror[i - 50] );
    }

    for( int i = 0; i >= 150 && ( i < 200 ); i++ ) {
        CPPUNIT_ASSERT_MESSAGE( "Failed to ad elements properly", array.get( i ) == mirror[i - 100] );
    }
}

////////////////////////////////////////////////////////////////////////////////
void CopyOnWriteArrayListTest::testAddAll2() {

    StlList<int> emptyCollection;
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IndexOutOfBoundsException",
        CopyOnWriteArrayList<int>().addAll( -1, emptyCollection ),
        IndexOutOfBoundsException );

    {
        std::string data[] = { "1", "2", "3", "4", "5", "6", "7", "8" };
        CopyOnWriteArrayList<std::string> list1;
        CopyOnWriteArrayList<std::string> list2;
        for( int i = 0; i < 8; ++i ) {
            list1.add( data[i] );
            list2.add( data[i] );
            list2.add( data[i] );
        }

        while( list1.size() > 0 ) {
            list1.removeAt( 0 );
        }
        list1.addAll( list2 );
        CPPUNIT_ASSERT_MESSAGE( "The object list is not the same as original list",
                                list1.containsAll( list2 ) && list2.containsAll( list1 ) );

        CopyOnWriteArrayList<std::string> list3;
        for( int i = 0; i < 100; i++ ) {
            if( list1.size() > 0 ) {
                list3.removeAll( list1 );
                list3.addAll( list1 );
            }
        }
        CPPUNIT_ASSERT_MESSAGE( "The object list is not the same as original list",
                                list3.containsAll( list1 ) && list1.containsAll( list3 ) );
    }
    {
        CopyOnWriteArrayList<std::string> list1;
        CopyOnWriteArrayList<std::string> list2;
        int location = 2;

        std::string data1[] = { "1", "2", "3", "4", "5", "6" };
        std::string data2[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
        for( int i = 0; i < 6; i++ ) {
            list1.add( data1[i] );
        }
        for( int i = 0; i < 8; i++ ) {
            list2.add( data2[i] );
        }

        list1.removeAt( location );
        list1.addAll( location, list2 );

        // Inserted elements should be equal to second array
        for( int i = 0; i < 8; i++ ) {
            CPPUNIT_ASSERT_EQUAL( data2[i], list1.get( location + i ) );
        }
        // Elements after inserted location should
        // be equals to related elements in first array
        for( int i = location + 1; i < 6; i++ ) {
            CPPUNIT_ASSERT_EQUAL( data1[i], list1.get( i + 8 - 1 ) );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void CopyOnWriteArrayListTest::testAddAll3() {

    CopyOnWriteArrayList<int> list;
    list.addAll( 0, list );
    list.addAll( list.size(), list );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IndexOutOfBoundsException",
        list.addAll( -1, list ),
        IndexOutOfBoundsException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IndexOutOfBoundsException",
        list.addAll( list.size() + 1, list ),
        IndexOutOfBoundsException );
}

////////////////////////////////////////////////////////////////////////////////
void CopyOnWriteArrayListTest::testAddAll4() {

    CopyOnWriteArrayList<std::string> array;
    CopyOnWriteArrayList<std::string> blist;

    array.add( "a" );
    array.add( "b" );
    blist.add( "c" );
    blist.add( "d" );
    blist.removeAt( 0 );
    blist.addAll( 0, array );

    CPPUNIT_ASSERT_EQUAL( std::string("a"), blist.get(0) );
    CPPUNIT_ASSERT_EQUAL( std::string("b"), blist.get(1) );
    CPPUNIT_ASSERT_EQUAL( std::string("d"), blist.get(2) );
}

////////////////////////////////////////////////////////////////////////////////
void CopyOnWriteArrayListTest::testAddAll5() {

    CopyOnWriteArrayList<std::string> array;
    populate( array, 100 );

    CopyOnWriteArrayList<std::string> l;
    l.addAll( array );
    for( int i = 0; i < array.size(); i++ ) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "Failed to add elements properly",
                                      l.get(i), array.get( i ) );
    }
    array.addAll( array );
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "Returned incorrect size after adding to existing list",
                                  200, array.size());

    for( int i = 0; i < 100; i++ ) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "Added to list in incorrect order",
                                      array.get(i), l.get(i) );
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "Failed to add to existing list",
                                      array.get(i + 100), l.get(i) );
    }

    CopyOnWriteArrayList<int> originalList;
    for( int j = 0; j < 12; j++ ) {
        originalList.add( j );
    }

    originalList.removeAt( 0 );
    originalList.removeAt( 0 );

    CopyOnWriteArrayList<int> additionalList;
    for( int j = 0; j < 11; j++ ) {
        additionalList.add( j );
    }
    CPPUNIT_ASSERT( originalList.addAll( additionalList ) );
    CPPUNIT_ASSERT_EQUAL( 21, originalList.size() );
}

////////////////////////////////////////////////////////////////////////////////
void CopyOnWriteArrayListTest::testAddAll6() {

    CopyOnWriteArrayList<int> arrayListA;
    arrayListA.add( 1 );
    CopyOnWriteArrayList<int> arrayListB;
    arrayListB.add( 1 );
    arrayListA.addAll( 1, arrayListB );
    int size = arrayListA.size();
    CPPUNIT_ASSERT_EQUAL( 2, size );
    for( int index = 0; index < size; index++ ) {
        CPPUNIT_ASSERT_EQUAL( 1, arrayListA.get( index ) );
    }
}

////////////////////////////////////////////////////////////////////////////////
void CopyOnWriteArrayListTest::testAddAll7() {

    CopyOnWriteArrayList<int> arrayList;
    arrayList.add( 1 );
    arrayList.addAll( 1, arrayList );
    int size = arrayList.size();
    CPPUNIT_ASSERT_EQUAL( 2, size );
    for( int index = 0; index < size; index++ ) {
        CPPUNIT_ASSERT_EQUAL( 1, arrayList.get( index ) );
    }
}

////////////////////////////////////////////////////////////////////////////////
void CopyOnWriteArrayListTest::testAddAll8() {

    CopyOnWriteArrayList<std::string> arrayList;
    arrayList.add( "1" );
    arrayList.add( "2" );

    CopyOnWriteArrayList<std::string> list;
    list.add( "a" );
    list.add( 0, "b" );
    list.add( 0, "c" );
    list.add( 0, "d" );
    list.add( 0, "e" );
    list.add( 0, "f" );
    list.add( 0, "g" );
    list.add( 0, "h" );
    list.add( 0, "i" );

    list.addAll( 6, arrayList );

    CPPUNIT_ASSERT_EQUAL( 11, list.size() );
    CPPUNIT_ASSERT( !list.contains( "q" ) );
}

////////////////////////////////////////////////////////////////////////////////
void CopyOnWriteArrayListTest::testAddAll9() {

    CopyOnWriteArrayList<std::string> list;
    list.add( "one" );
    list.add( "two" );
    CPPUNIT_ASSERT_EQUAL( 2, list.size() );

    list.removeAt( 0 );
    CPPUNIT_ASSERT_EQUAL( 1, list.size() );

    CopyOnWriteArrayList<std::string> collection;
    collection.add( "1" );
    collection.add( "2" );
    collection.add( "3" );
    CPPUNIT_ASSERT_EQUAL( 3, collection.size() );

    list.addAll( 0, collection );
    CPPUNIT_ASSERT_EQUAL( 4, list.size() );

    list.removeAt( 0 );
    list.removeAt( 0 );
    CPPUNIT_ASSERT_EQUAL( 2, list.size() );

    collection.add( "4" );
    collection.add( "5" );
    collection.add( "6" );
    collection.add( "7" );
    collection.add( "8" );
    collection.add( "9" );
    collection.add( "10" );
    collection.add( "11" );
    collection.add( "12" );

    CPPUNIT_ASSERT_EQUAL( 12, collection.size() );

    list.addAll( 0, collection );
    CPPUNIT_ASSERT_EQUAL( 14, list.size() );
}

////////////////////////////////////////////////////////////////////////////////
void CopyOnWriteArrayListTest::testClear() {

    CopyOnWriteArrayList<int> list;

    populate( list, SIZE );

    CPPUNIT_ASSERT( !list.isEmpty() );

    list.clear();

    CPPUNIT_ASSERT( list.isEmpty() );
    CPPUNIT_ASSERT_EQUAL( 0, list.size() );
}

//////////////////////////////////////////////////////////////////////////////
void CopyOnWriteArrayListTest::testContains() {

    CopyOnWriteArrayList<int> list;
    populate( list, 3 );

    CPPUNIT_ASSERT( list.contains( 1 ) );
    CPPUNIT_ASSERT( !list.contains( 5 ) );
}

//////////////////////////////////////////////////////////////////////////////
void CopyOnWriteArrayListTest::testGet() {

    CopyOnWriteArrayList<int> list;
    populate( list, 3 );

    CPPUNIT_ASSERT_EQUAL( 0, list.get( 0 ) );
}

//////////////////////////////////////////////////////////////////////////////
void CopyOnWriteArrayListTest::testIsEmpty() {

    CopyOnWriteArrayList<int> list;
    populate( list, SIZE );

    CopyOnWriteArrayList<int> empty;

    CPPUNIT_ASSERT( empty.isEmpty() );
    CPPUNIT_ASSERT( !list.isEmpty() );
}

//////////////////////////////////////////////////////////////////////////////
void CopyOnWriteArrayListTest::testIndexOf1() {

    CopyOnWriteArrayList<int> list;
    populate( list, 3 );

    CPPUNIT_ASSERT_EQUAL( 1, list.indexOf( 1 ) );
    CPPUNIT_ASSERT_EQUAL( -1, list.indexOf( 99 ) );
}

//////////////////////////////////////////////////////////////////////////////
void CopyOnWriteArrayListTest::testLastIndexOf1() {

    CopyOnWriteArrayList<int> list;
    populate( list, 3 );
    list.add( 1 );
    list.add( 3 );
    CPPUNIT_ASSERT_EQUAL( 3, list.lastIndexOf( 1 ) );
    CPPUNIT_ASSERT_EQUAL( -1, list.lastIndexOf( 6 ) );
}

//////////////////////////////////////////////////////////////////////////////
void CopyOnWriteArrayListTest::testAddIndex() {

    CopyOnWriteArrayList<int> list;
    populate( list, 3 );

    list.add( 0, 4 );
    CPPUNIT_ASSERT_EQUAL( 4, list.size() );
    CPPUNIT_ASSERT_EQUAL( 4, list.get( 0 ) );
    CPPUNIT_ASSERT_EQUAL( 0, list.get( 1 ) );

    list.add( 2, 6 );
    CPPUNIT_ASSERT_EQUAL( 5, list.size() );
    CPPUNIT_ASSERT_EQUAL( 6, list.get( 2 ) );
    CPPUNIT_ASSERT_EQUAL( 2, list.get( 4 ) );

    CopyOnWriteArrayList<int> list2;
    list2.add( 0, 42 );
    CPPUNIT_ASSERT_EQUAL( 1, list2.size() );
}

//////////////////////////////////////////////////////////////////////////////
void CopyOnWriteArrayListTest::testAddAllIndex() {

    CopyOnWriteArrayList<int> list;
    populate( list, 3 );

    StlList<int> collection;
    for( int i = 0; i < 6; ++i ) {
        collection.add( i + 10 );
    }

    list.addAll( 0, collection );
    CPPUNIT_ASSERT_EQUAL( 9, list.size() );
    CPPUNIT_ASSERT_EQUAL( 10, list.get( 0 ) );
    CPPUNIT_ASSERT_EQUAL( 0, list.get( 6 ) );

    list.addAll( 6, collection );
    CPPUNIT_ASSERT_EQUAL( 15, list.size() );
    CPPUNIT_ASSERT_EQUAL( 10, list.get( 6 ) );
    CPPUNIT_ASSERT_EQUAL( 0, list.get( 12 ) );

    CopyOnWriteArrayList<int> list2;
    list2.addAll( 0, collection );
    CPPUNIT_ASSERT_EQUAL( 6, list2.size() );
}

//////////////////////////////////////////////////////////////////////////////
void CopyOnWriteArrayListTest::testContainsAll() {

    CopyOnWriteArrayList<int> list;
    populate( list, 7 );

    StlList<int> collection;
    for( int i = 0; i < 6; ++i ) {
        collection.add( i );
    }

    CPPUNIT_ASSERT( list.containsAll( collection ) );
    collection.add( 42 );
    CPPUNIT_ASSERT( !list.containsAll( collection ) );
}

//////////////////////////////////////////////////////////////////////////////
void CopyOnWriteArrayListTest::testEquals() {

    CopyOnWriteArrayList<int> list1;
    populate( list1, 7 );
    CopyOnWriteArrayList<int> list2;
    populate( list2, 7 );

    CPPUNIT_ASSERT( list1.equals( list2 ) );
    CPPUNIT_ASSERT( list2.equals( list1 ) );

    list1.add( 42 );
    CPPUNIT_ASSERT( !list1.equals( list2 ) );
    CPPUNIT_ASSERT( !list2.equals( list1 ) );
    list2.add( 42 );
    CPPUNIT_ASSERT( list1.equals( list2 ) );
    CPPUNIT_ASSERT( list2.equals( list1 ) );
}

//////////////////////////////////////////////////////////////////////////////
void CopyOnWriteArrayListTest::testRemove() {

    CopyOnWriteArrayList<int> list1;
    populate( list1, SIZE );
    CopyOnWriteArrayList<int> list2;
    populate( list2, SIZE );

    list1.remove( 42 );
    list2.remove( 42 );

    CPPUNIT_ASSERT_MESSAGE( "Lists should be equal", list1.equals( list2 ) );
    list1.remove( 42 );
    CPPUNIT_ASSERT_MESSAGE( "Lists should be equal", list1.equals( list2 ) );

    CPPUNIT_ASSERT( list1.remove( 0 ) );
    CPPUNIT_ASSERT_MESSAGE( "Lists should not be equal", !list1.equals( list2 ) );

    list1.clear();
    populate( list1, SIZE );

    for( int i = 0; i < SIZE; i++ ) {
        CPPUNIT_ASSERT( list1.remove( i ) );
    }
}

//////////////////////////////////////////////////////////////////////////////
void CopyOnWriteArrayListTest::testRemoveAt() {

    CopyOnWriteArrayList<int> list1;
    populate( list1, 7 );
    CopyOnWriteArrayList<int> list2;
    populate( list2, 7 );

    CPPUNIT_ASSERT_EQUAL( 2, list1.removeAt( 2 ) );
    CPPUNIT_ASSERT_EQUAL( 6, list1.size() );
    CPPUNIT_ASSERT_EQUAL( 3, list1.removeAt( 2 ) );
    CPPUNIT_ASSERT_EQUAL( 5, list1.size() );

    CPPUNIT_ASSERT_EQUAL( 6, list2.removeAt( 6 ) );
    CPPUNIT_ASSERT_EQUAL( 6, list2.size() );
    CPPUNIT_ASSERT_EQUAL( 0, list2.removeAt( 0 ) );
    CPPUNIT_ASSERT_EQUAL( 5, list2.size() );
    CPPUNIT_ASSERT_EQUAL( 5, list2.removeAt( 4 ) );
    CPPUNIT_ASSERT_EQUAL( 4, list2.size() );
    CPPUNIT_ASSERT_EQUAL( 1, list2.removeAt( 0 ) );
    CPPUNIT_ASSERT_EQUAL( 3, list2.size() );
    CPPUNIT_ASSERT_EQUAL( 4, list2.removeAt( 2 ) );
    CPPUNIT_ASSERT_EQUAL( 2, list2.size() );
    CPPUNIT_ASSERT_EQUAL( 2, list2.removeAt( 0 ) );
    CPPUNIT_ASSERT_EQUAL( 1, list2.size() );
    CPPUNIT_ASSERT_EQUAL( 3, list2.removeAt( 0 ) );
    CPPUNIT_ASSERT_EQUAL( 0, list2.size() );
}

//////////////////////////////////////////////////////////////////////////////
void CopyOnWriteArrayListTest::testAddIfAbsent1() {
    CopyOnWriteArrayList<int> list;
    populate( list, SIZE );

    list.addIfAbsent( 1 );

    CPPUNIT_ASSERT_EQUAL( SIZE, list.size() );
}

//////////////////////////////////////////////////////////////////////////////
void CopyOnWriteArrayListTest::testAddIfAbsent2() {

    CopyOnWriteArrayList<int> list;
    populate( list, SIZE );

    CPPUNIT_ASSERT( !list.contains(SIZE) );
    list.addIfAbsent( SIZE );
    CPPUNIT_ASSERT( list.contains(SIZE) );
}

//////////////////////////////////////////////////////////////////////////////
void CopyOnWriteArrayListTest::testIterator() {

    CopyOnWriteArrayList<int> list;
    populate( list, SIZE );

    std::auto_ptr< Iterator<int> > i( list.iterator() );
    int j;
    for( j = 0; i->hasNext(); j++ ) {
        CPPUNIT_ASSERT_EQUAL( j, i->next() );
    }

    CPPUNIT_ASSERT_EQUAL( SIZE, j );
}

//////////////////////////////////////////////////////////////////////////////
void CopyOnWriteArrayListTest::testIteratorRemove() {

    CopyOnWriteArrayList<int> list;
    populate( list, SIZE );

    std::auto_ptr< Iterator<int> > it( list.iterator() );

    it->next();

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an UnsupportedOperationException",
        it->remove(),
        UnsupportedOperationException );
}

//////////////////////////////////////////////////////////////////////////////
void CopyOnWriteArrayListTest::testListIterator1() {

    CopyOnWriteArrayList<int> list;
    populate( list, SIZE );

    std::auto_ptr< ListIterator<int> > i( list.listIterator() );
    int j;
    for( j = 0; i->hasNext(); j++ ) {
        CPPUNIT_ASSERT_EQUAL( j, i->next() );
    }

    CPPUNIT_ASSERT_EQUAL( SIZE, j );
}

//////////////////////////////////////////////////////////////////////////////
void CopyOnWriteArrayListTest::testListIterator2() {

    CopyOnWriteArrayList<int> list;
    populate( list, SIZE );

    std::auto_ptr< ListIterator<int> > i( list.listIterator( 1 ) );
    int j;
    for( j = 0; i->hasNext(); j++ ) {
        CPPUNIT_ASSERT_EQUAL( j+1, i->next() );
    }

    CPPUNIT_ASSERT_EQUAL( SIZE - 1, j );
}

//////////////////////////////////////////////////////////////////////////////
void CopyOnWriteArrayListTest::testSet() {

    CopyOnWriteArrayList<int> list;
    populate( list, SIZE );

    CPPUNIT_ASSERT_EQUAL( 2, list.set( 2, 4 ) );
    CPPUNIT_ASSERT_EQUAL( 4, list.get( 2 ) );
}

//////////////////////////////////////////////////////////////////////////////
void CopyOnWriteArrayListTest::testSize() {

    CopyOnWriteArrayList<int> empty;
    CopyOnWriteArrayList<int> list;
    populate( list, SIZE );

    CPPUNIT_ASSERT_EQUAL( SIZE, list.size() );
    CPPUNIT_ASSERT_EQUAL( 0, empty.size() );
}

//////////////////////////////////////////////////////////////////////////////
void CopyOnWriteArrayListTest::testAddAll1IndexOutOfBoundsException() {

    CopyOnWriteArrayList<int> list;
    StlList<int> collection;

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IndexOutOfBoundsException",
        list.addAll( -1, collection ),
        IndexOutOfBoundsException );
}

//////////////////////////////////////////////////////////////////////////////
void CopyOnWriteArrayListTest::testAddAll2IndexOutOfBoundsException() {

    CopyOnWriteArrayList<int> list;
    list.add( 1 );
    list.add( 2 );

    StlList<int> collection;

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IndexOutOfBoundsException",
        list.addAll( 100, collection ),
        IndexOutOfBoundsException );
}

//////////////////////////////////////////////////////////////////////////////
void CopyOnWriteArrayListTest::testListIterator1IndexOutOfBoundsException() {

    CopyOnWriteArrayList<int> list;

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IndexOutOfBoundsException",
        std::auto_ptr< ListIterator<int> > it( list.listIterator( -1 ) ),
        IndexOutOfBoundsException );
}

//////////////////////////////////////////////////////////////////////////////
void CopyOnWriteArrayListTest::testListIterator2IndexOutOfBoundsException() {

    CopyOnWriteArrayList<int> list;
    list.add( 1 );
    list.add( 2 );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IndexOutOfBoundsException",
        std::auto_ptr< ListIterator<int> > it( list.listIterator( 100 ) ),
        IndexOutOfBoundsException );
}

//////////////////////////////////////////////////////////////////////////////
void CopyOnWriteArrayListTest::testAdd1IndexOutOfBoundsException() {

    CopyOnWriteArrayList<int> list;

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IndexOutOfBoundsException",
        list.add( -1, 42 ),
        IndexOutOfBoundsException );
}

//////////////////////////////////////////////////////////////////////////////
void CopyOnWriteArrayListTest::testAdd2IndexOutOfBoundsException() {

    CopyOnWriteArrayList<int> list;
    list.add( 1 );
    list.add( 2 );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IndexOutOfBoundsException",
        list.add( 100, 42 ),
        IndexOutOfBoundsException );
}

//////////////////////////////////////////////////////////////////////////////
void CopyOnWriteArrayListTest::testRemoveAt1IndexOutOfBounds() {

    CopyOnWriteArrayList<int> list;

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IndexOutOfBoundsException",
        list.removeAt( -1 ),
        IndexOutOfBoundsException );
}

//////////////////////////////////////////////////////////////////////////////
void CopyOnWriteArrayListTest::testRemoveAt2IndexOutOfBounds() {

    CopyOnWriteArrayList<int> list;
    list.add( 1 );
    list.add( 2 );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IndexOutOfBoundsException",
        list.removeAt( 100 ),
        IndexOutOfBoundsException );
}

//////////////////////////////////////////////////////////////////////////////
void CopyOnWriteArrayListTest::testGet1IndexOutOfBoundsException() {

    CopyOnWriteArrayList<int> list;

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IndexOutOfBoundsException",
        list.get( -1 ),
        IndexOutOfBoundsException );
}

//////////////////////////////////////////////////////////////////////////////
void CopyOnWriteArrayListTest::testGet2IndexOutOfBoundsException() {

    CopyOnWriteArrayList<int> list;
    list.add( 1 );
    list.add( 2 );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IndexOutOfBoundsException",
        list.get( 100 ),
        IndexOutOfBoundsException );
}

//////////////////////////////////////////////////////////////////////////////
void CopyOnWriteArrayListTest::testSet1IndexOutOfBoundsException() {

    CopyOnWriteArrayList<int> list;

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IndexOutOfBoundsException",
        list.set( -1, 42 ),
        IndexOutOfBoundsException );
}

//////////////////////////////////////////////////////////////////////////////
void CopyOnWriteArrayListTest::testSet2IndexOutOfBoundsException() {

    CopyOnWriteArrayList<int> list;
    list.add( 1 );
    list.add( 2 );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IndexOutOfBoundsException",
        list.set( 100, 42 ),
        IndexOutOfBoundsException );
}

//////////////////////////////////////////////////////////////////////////////
void CopyOnWriteArrayListTest::testToArray() {

    CopyOnWriteArrayList<int> list;
    list.add( 1 );
    list.add( 2 );
    list.add( 3 );

    std::vector<int> result = list.toArray();

    CPPUNIT_ASSERT_EQUAL( 3, (int)result.size() );
    CPPUNIT_ASSERT_EQUAL( 1, result[0] );
    CPPUNIT_ASSERT_EQUAL( 2, result[1] );
    CPPUNIT_ASSERT_EQUAL( 3, result[2] );
}

//////////////////////////////////////////////////////////////////////////////
void CopyOnWriteArrayListTest::testLastIndexOf2() {
    CopyOnWriteArrayList<int> list;
    populate( list, 3 );
    list.add( 1 );
    list.add( 3 );

    CPPUNIT_ASSERT_EQUAL( 3, list.lastIndexOf( 1, 4 ) );
    CPPUNIT_ASSERT_EQUAL( -1, list.lastIndexOf( 3, 3 ) );
}

//////////////////////////////////////////////////////////////////////////////
void CopyOnWriteArrayListTest::testIndexOf2() {

    CopyOnWriteArrayList<int> list;
    populate( list, 3 );

    CPPUNIT_ASSERT_EQUAL( 1, list.indexOf( 1, 0 ) );
    CPPUNIT_ASSERT_EQUAL( -1, list.indexOf( 1, 2 ) );
}

//////////////////////////////////////////////////////////////////////////////
void CopyOnWriteArrayListTest::testAddAllAbsent() {

    CopyOnWriteArrayList<int> list;
    populate( list, 3 );

    StlList<int> collection;
    collection.add( 3 );
    collection.add( 4 );
    collection.add( 1 ); // will not add this element

    list.addAllAbsent( collection );

    CPPUNIT_ASSERT_EQUAL( 5, list.size() );
}

//////////////////////////////////////////////////////////////////////////////
void CopyOnWriteArrayListTest::testRemoveAll() {

    CopyOnWriteArrayList<int> list;
    populate( list, 3 );

    StlList<int> collection;
    collection.add( 1 );
    collection.add( 2 );

    list.removeAll( collection );

    CPPUNIT_ASSERT_EQUAL( 1, list.size() );

    CopyOnWriteArrayList<int> list2;
    list2.removeAll( collection );
    CPPUNIT_ASSERT_EQUAL( 0, list2.size() );

    CopyOnWriteArrayList<int> list3;
    populate( list3, 3 );
    collection.clear();

    list3.removeAll( collection );
    CPPUNIT_ASSERT_EQUAL( 3, list3.size() );
}

//////////////////////////////////////////////////////////////////////////////
void CopyOnWriteArrayListTest::testRetainAll() {

    CopyOnWriteArrayList<int> list;
    populate( list, 3 );

    StlList<int> collection;
    collection.add( 1 );
    collection.add( 2 );

    list.retainAll( collection );

    CPPUNIT_ASSERT_EQUAL( 2, list.size() );

    CopyOnWriteArrayList<int> list2;
    list2.retainAll( collection );
    CPPUNIT_ASSERT_EQUAL( 0, list2.size() );

    CopyOnWriteArrayList<int> list3;
    populate( list3, 3 );
    collection.clear();

    list3.retainAll( collection );
    CPPUNIT_ASSERT_EQUAL( 0, list3.size() );
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class Target {
    private:

        int counter;

    public:

        Target() : counter(0) {
        }

        void increment() {
            this->counter++;
        }
    };

    class AddRemoveItemRunnable : public Runnable {
    private:

        Random rand;
        CopyOnWriteArrayList<Pointer<Target> >* list;

    private:

        AddRemoveItemRunnable(const AddRemoveItemRunnable&);
        AddRemoveItemRunnable operator= (const AddRemoveItemRunnable&);

    public:

        AddRemoveItemRunnable(CopyOnWriteArrayList<Pointer<Target> >* list) :
            Runnable(), rand(), list(list) {
        }

        virtual ~AddRemoveItemRunnable() {}

        virtual void run() {
            TimeUnit::MILLISECONDS.sleep(rand.nextInt(10));
            Pointer<Target> target(new Target());
            list->add(target);
            TimeUnit::MILLISECONDS.sleep(rand.nextInt(10));
            list->remove(target);
        }
    };

    class IterateAndExecuteMethodRunnable : public Runnable {
    private:

        Random rand;
        CopyOnWriteArrayList<Pointer<Target> >* list;

    private:

        IterateAndExecuteMethodRunnable(const IterateAndExecuteMethodRunnable&);
        IterateAndExecuteMethodRunnable operator= (const IterateAndExecuteMethodRunnable&);

    public:

        IterateAndExecuteMethodRunnable(CopyOnWriteArrayList<Pointer<Target> >* list) :
            Runnable(), rand(), list(list) {
        }

        virtual ~IterateAndExecuteMethodRunnable() {}

        virtual void run() {
            TimeUnit::MILLISECONDS.sleep(rand.nextInt(15));
            Pointer< Iterator<Pointer<Target> > > iter(list->iterator());
            while(iter->hasNext()) {
                iter->next()->increment();
            }
        }
    };

}

////////////////////////////////////////////////////////////////////////////////
void CopyOnWriteArrayListTest::testConcurrentRandomAddRemoveAndIterate() {

    ThreadPoolExecutor executor(50, Integer::MAX_VALUE, 60LL, TimeUnit::SECONDS, new LinkedBlockingQueue<Runnable*>());
    CopyOnWriteArrayList<Pointer<Target> > list;

    Random rand;

    for (int i = 0; i < 3000; i++) {
        executor.execute(new AddRemoveItemRunnable(&list));
        executor.execute(new IterateAndExecuteMethodRunnable(&list));
    }

    executor.shutdown();
    CPPUNIT_ASSERT_MESSAGE("executor terminated", executor.awaitTermination(45, TimeUnit::SECONDS));
}
