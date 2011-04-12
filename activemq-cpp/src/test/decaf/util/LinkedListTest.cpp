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

#include "LinkedListTest.h"

#include <decaf/util/ArrayList.h>
#include <decaf/util/LinkedList.h>
#include <decaf/lang/Integer.h>

using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::util;

////////////////////////////////////////////////////////////////////////////////
const int LinkedListTest::SIZE = 256;

////////////////////////////////////////////////////////////////////////////////
namespace {

    void populate( LinkedList<int>& list, int n ) {

        CPPUNIT_ASSERT( list.isEmpty() );

        for( int i = 0; i < n; ++i ) {
            list.add( i );
        }

        CPPUNIT_ASSERT( !list.isEmpty());
        CPPUNIT_ASSERT_EQUAL( n, list.size() );
    }

    void populate( LinkedList<std::string>& list, int n ) {

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
LinkedListTest::LinkedListTest() {
}

////////////////////////////////////////////////////////////////////////////////
LinkedListTest::~LinkedListTest() {
}

////////////////////////////////////////////////////////////////////////////////
void LinkedListTest::testConstructor1() {

    LinkedList<int> list;

    CPPUNIT_ASSERT( list.size() == 0 );
    CPPUNIT_ASSERT( list.isEmpty() == true );
}

////////////////////////////////////////////////////////////////////////////////
void LinkedListTest::testConstructor2() {

    LinkedList<int> mylist;
    populate( mylist, SIZE );
}

////////////////////////////////////////////////////////////////////////////////
void LinkedListTest::testConstructor3() {
}

//////////////////////////////////////////////////////////////////////////////
void LinkedListTest::testEquals() {

    LinkedList<int> list1;
    populate( list1, 7 );
    LinkedList<int> list2;
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

////////////////////////////////////////////////////////////////////////////////
void LinkedListTest::testGet() {

    LinkedList<int> list;
    populate( list, SIZE );

    CPPUNIT_ASSERT_EQUAL( 0, list.get(0) );
    CPPUNIT_ASSERT_EQUAL( SIZE/2, list.get(SIZE/2) );
    CPPUNIT_ASSERT_EQUAL( SIZE-1, list.get(SIZE-1) );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an IndexOutOfBoundsException",
        list.get(-1),
        IndexOutOfBoundsException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an IndexOutOfBoundsException",
        list.get(SIZE),
        IndexOutOfBoundsException );
}

////////////////////////////////////////////////////////////////////////////////
void LinkedListTest::testSet() {

    LinkedList<int> list;
    populate( list, SIZE );

    CPPUNIT_ASSERT_EQUAL( 0, list.get(0) );
    CPPUNIT_ASSERT_EQUAL( 0, list.set(0, 42) );
    CPPUNIT_ASSERT_EQUAL( 42, list.get(0) );
    CPPUNIT_ASSERT_EQUAL( SIZE/2, list.get(SIZE/2) );
    CPPUNIT_ASSERT_EQUAL( SIZE/2, list.set(SIZE/2, 42) );
    CPPUNIT_ASSERT_EQUAL( 42, list.get(SIZE/2) );
    CPPUNIT_ASSERT_EQUAL( SIZE-1, list.get(SIZE-1) );
    CPPUNIT_ASSERT_EQUAL( SIZE-1, list.set(SIZE-1, 42) );
    CPPUNIT_ASSERT_EQUAL( 42, list.get(SIZE-1) );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an IndexOutOfBoundsException",
        list.set(-1, 42),
        IndexOutOfBoundsException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an IndexOutOfBoundsException",
        list.set(SIZE, 42),
        IndexOutOfBoundsException );
}

////////////////////////////////////////////////////////////////////////////////
void LinkedListTest::testOffer() {

    LinkedList<int> list;
    CPPUNIT_ASSERT_EQUAL( true, list.offer( 42 ) );
    CPPUNIT_ASSERT( list.size() == 1 );
    CPPUNIT_ASSERT( list.isEmpty() == false );
    CPPUNIT_ASSERT_EQUAL( 42, list.getLast() );
    CPPUNIT_ASSERT_EQUAL( 42, list.getFirst() );
    CPPUNIT_ASSERT_EQUAL( true, list.offer( 84 ) );
    CPPUNIT_ASSERT( list.size() == 2 );
    CPPUNIT_ASSERT( list.isEmpty() == false );
    CPPUNIT_ASSERT_EQUAL( 42, list.getFirst() );
    CPPUNIT_ASSERT_EQUAL( 84, list.getLast() );
}

////////////////////////////////////////////////////////////////////////////////
void LinkedListTest::testAddFirst() {

    LinkedList<int> list;
    list.addFirst( 42 );
    CPPUNIT_ASSERT( list.size() == 1 );
    CPPUNIT_ASSERT( list.isEmpty() == false );
    CPPUNIT_ASSERT_EQUAL( 42, list.getLast() );
    CPPUNIT_ASSERT_EQUAL( 42, list.getFirst() );
    list.addFirst( 84 );
    CPPUNIT_ASSERT( list.size() == 2 );
    CPPUNIT_ASSERT( list.isEmpty() == false );
    CPPUNIT_ASSERT_EQUAL( 84, list.getFirst() );
    CPPUNIT_ASSERT_EQUAL( 42, list.getLast() );
}

////////////////////////////////////////////////////////////////////////////////
void LinkedListTest::testAddLast() {

    LinkedList<int> list;
    list.addLast( 42 );
    CPPUNIT_ASSERT( list.size() == 1 );
    CPPUNIT_ASSERT( list.isEmpty() == false );
    CPPUNIT_ASSERT_EQUAL( 42, list.getLast() );
    CPPUNIT_ASSERT_EQUAL( 42, list.getFirst() );
    list.addLast( 84 );
    CPPUNIT_ASSERT( list.size() == 2 );
    CPPUNIT_ASSERT( list.isEmpty() == false );
    CPPUNIT_ASSERT_EQUAL( 42, list.getFirst() );
    CPPUNIT_ASSERT_EQUAL( 84, list.getLast() );
}

////////////////////////////////////////////////////////////////////////////////
void LinkedListTest::testGetFirst() {

    LinkedList<int> list;

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an NoSuchElementException",
        list.getFirst(),
        NoSuchElementException );

    populate( list, SIZE );
    CPPUNIT_ASSERT_EQUAL( 0, list.getFirst() );
}

////////////////////////////////////////////////////////////////////////////////
void LinkedListTest::testGetLast() {

    LinkedList<int> list;

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an NoSuchElementException",
        list.getLast(),
        NoSuchElementException );

    populate( list, SIZE );
    CPPUNIT_ASSERT_EQUAL( SIZE-1, list.getLast() );
}

////////////////////////////////////////////////////////////////////////////////
void LinkedListTest::testAdd1() {

    LinkedList<int> list;

    list.add( 42 );

    CPPUNIT_ASSERT( list.size() == 1 );
    CPPUNIT_ASSERT( list.isEmpty() == false );
    CPPUNIT_ASSERT_EQUAL( 42, list.getLast() );
    CPPUNIT_ASSERT_EQUAL( 42, list.getFirst() );

    list.add( 49 );
    list.add( 50 );
    list.add( 51 );
    list.add( 60 );
    list.add( 84 );

    CPPUNIT_ASSERT( list.indexOf( 42 ) == 0 );
    CPPUNIT_ASSERT( list.size() == 6 );
    CPPUNIT_ASSERT( list.isEmpty() == false );
    CPPUNIT_ASSERT_EQUAL( 84, list.getLast() );
    CPPUNIT_ASSERT_EQUAL( 42, list.getFirst() );

    LinkedList<int> mylist;
    populate( mylist, SIZE );
    CPPUNIT_ASSERT_EQUAL( 0, mylist.get( 0 ) );
    CPPUNIT_ASSERT_EQUAL( SIZE-1, mylist.get( SIZE-1 ) );
}

////////////////////////////////////////////////////////////////////////////////
void LinkedListTest::testAdd2() {

    LinkedList<int> list;

    list.add( 0, 42 );

    CPPUNIT_ASSERT( list.size() == 1 );
    CPPUNIT_ASSERT( list.isEmpty() == false );
    CPPUNIT_ASSERT_EQUAL( 42, list.getLast() );
    CPPUNIT_ASSERT_EQUAL( 42, list.getFirst() );

    list.add( 1, 84 );

    CPPUNIT_ASSERT( list.size() == 2 );
    CPPUNIT_ASSERT( list.isEmpty() == false );
    CPPUNIT_ASSERT_EQUAL( 84, list.getLast() );
    CPPUNIT_ASSERT_EQUAL( 42, list.getFirst() );

    list.add( 0, 21 );

    CPPUNIT_ASSERT( list.size() == 3 );
    CPPUNIT_ASSERT( list.isEmpty() == false );
    CPPUNIT_ASSERT_EQUAL( 84, list.getLast() );
    CPPUNIT_ASSERT_EQUAL( 21, list.getFirst() );

    list.add( 1, 22 );
    list.add( 4, 168 );

    CPPUNIT_ASSERT( list.size() == 5 );
    CPPUNIT_ASSERT( list.isEmpty() == false );
    CPPUNIT_ASSERT_EQUAL( 168, list.getLast() );
    CPPUNIT_ASSERT_EQUAL( 21, list.getFirst() );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an IndexOutOfBoundsException",
        list.add( -1, 12 ),
        IndexOutOfBoundsException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an IndexOutOfBoundsException",
        list.add( 100, 12 ),
        IndexOutOfBoundsException );
}

////////////////////////////////////////////////////////////////////////////////
void LinkedListTest::testAddAll1() {

    LinkedList<int> list;
    populate( list, SIZE );
    ArrayList<int> listCopy( list );

    list.addAll( 50, listCopy );
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "Returned incorrect size after adding to existing list",
                                  SIZE * 2, list.size() );
    for( int i = 0; i < 50; i++ ) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "Manipulated elements < index", i, list.get( i ) );
    }
    for( int i = 50; i < SIZE + 50; i++ ) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "Failed to add elements properly", i - 50, list.get( i ) );
    }
    for( int i = SIZE+50; i < SIZE * 2; i++ ) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "Failed to add elements properly", i - SIZE, list.get( i ) );
    }

    LinkedList<int> list2;

    list2.add(4);
    list2.add(3);
    list2.add(2);
    list2.add(1);
    list2.add(0);

    list.addAll( 50, list2 );
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "smaller list not added correctly", 4, list.get(50) );
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "smaller list not added correctly", 3, list.get(51) );
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "smaller list not added correctly", 2, list.get(52) );
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "smaller list not added correctly", 1, list.get(53) );
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "smaller list not added correctly", 0, list.get(54) );
}

////////////////////////////////////////////////////////////////////////////////
void LinkedListTest::testAddAll2() {

    LinkedList<int> list;
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an IndexOutOfBoundsException",
        list.addAll( -1, ArrayList<int>() ),
        IndexOutOfBoundsException );
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an IndexOutOfBoundsException",
        list.addAll( 99, ArrayList<int>() ),
        IndexOutOfBoundsException );
}

////////////////////////////////////////////////////////////////////////////////
void LinkedListTest::testAddAll3() {

    LinkedList<int> list;
    populate( list, SIZE );
    ArrayList<int> listCopy( list );

    list.addAll( listCopy );
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "Returned incorrect size after adding to existing list",
                                  SIZE * 2, list.size() );
    for( int i = 0; i < SIZE; i++ ) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "Failed to add elements properly", i, list.get( i ) );
    }
    for( int i = SIZE; i < SIZE * 2; i++ ) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "Failed to add elements properly", i - SIZE, list.get( i ) );
    }

    int newSize = list.size();

    LinkedList<int> list2;

    list2.add(4);
    list2.add(3);
    list2.add(2);
    list2.add(1);
    list2.add(0);

    list.addAll( list2 );
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "smaller list not added correctly", 4, list.get(newSize) );
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "smaller list not added correctly", 3, list.get(newSize+1) );
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "smaller list not added correctly", 2, list.get(newSize+2) );
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "smaller list not added correctly", 1, list.get(newSize+3) );
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "smaller list not added correctly", 0, list.get(newSize+4) );
}

////////////////////////////////////////////////////////////////////////////////
void LinkedListTest::testAddAll4() {

    ArrayList<int> emptyCollection;
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IndexOutOfBoundsException",
        ArrayList<int>().addAll( -1, emptyCollection ),
        IndexOutOfBoundsException );

    {
        std::string data[] = { "1", "2", "3", "4", "5", "6", "7", "8" };
        LinkedList<std::string> list1;
        LinkedList<std::string> list2;
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

        LinkedList<std::string> list3;
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
        LinkedList<std::string> list1;
        LinkedList<std::string> list2;
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
void LinkedListTest::testAddAllSelfAsCollection1() {

    LinkedList<int> list;
    populate( list, SIZE );

    CPPUNIT_ASSERT_EQUAL( SIZE, list.size() );
    CPPUNIT_ASSERT_EQUAL( true, list.addAll( list ) );
    CPPUNIT_ASSERT_EQUAL( SIZE+SIZE, list.size() );
}

////////////////////////////////////////////////////////////////////////////////
void LinkedListTest::testAddAllSelfAsCollection2() {

    LinkedList<int> list;
    populate( list, SIZE );

    CPPUNIT_ASSERT_EQUAL( SIZE, list.size() );
    CPPUNIT_ASSERT_EQUAL( true, list.addAll( 1, list ) );
    CPPUNIT_ASSERT_EQUAL( SIZE+SIZE, list.size() );
}

////////////////////////////////////////////////////////////////////////////////
void LinkedListTest::testRemoveAtIndex() {

    LinkedList<int> list;
    populate( list, SIZE );
    list.removeAt(10);

    CPPUNIT_ASSERT_EQUAL_MESSAGE( "Failed to remove element", -1, list.indexOf(10) );
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an IndexOutOfBoundsException",
        list.removeAt( 999 ),
        IndexOutOfBoundsException );

    list.add( 20, 0 );
    list.removeAt(20);
    CPPUNIT_ASSERT_MESSAGE( "Should not have removed 0", list.get(20) );
}

////////////////////////////////////////////////////////////////////////////////
void LinkedListTest::testRemoveByValue() {

    LinkedList<int> list;
    populate( list, SIZE );

    CPPUNIT_ASSERT_MESSAGE( "Failed to remove valid Object", list.remove(42) );
    CPPUNIT_ASSERT_MESSAGE( "Removed invalid object", !list.remove(999) );
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "Found Object after removal", -1, list.indexOf(42) );
    list.add(SIZE+1);
    list.remove(SIZE+1);
    CPPUNIT_ASSERT_MESSAGE( "Should not contain null afrer removal", !list.contains(SIZE+1) );
}

//////////////////////////////////////////////////////////////////////////////
void LinkedListTest::testRemoveAll() {

    LinkedList<int> list;
    populate( list, 3 );

    ArrayList<int> collection;
    collection.add( 1 );
    collection.add( 2 );

    list.removeAll( collection );

    CPPUNIT_ASSERT_EQUAL( 1, list.size() );

    LinkedList<int> list2;
    list2.removeAll( collection );
    CPPUNIT_ASSERT_EQUAL( 0, list2.size() );

    LinkedList<int> list3;
    populate( list3, 3 );
    collection.clear();

    list3.removeAll( collection );
    CPPUNIT_ASSERT_EQUAL( 3, list3.size() );
}

//////////////////////////////////////////////////////////////////////////////
void LinkedListTest::testRetainAll() {

    LinkedList<int> list;
    populate( list, 3 );

    ArrayList<int> collection;
    collection.add( 1 );
    collection.add( 2 );

    list.retainAll( collection );

    CPPUNIT_ASSERT_EQUAL( 2, list.size() );

    LinkedList<int> list2;
    list2.retainAll( collection );
    CPPUNIT_ASSERT_EQUAL( 0, list2.size() );

    LinkedList<int> list3;
    populate( list3, 3 );
    collection.clear();

    list3.retainAll( collection );
    CPPUNIT_ASSERT_EQUAL( 0, list3.size() );
}

////////////////////////////////////////////////////////////////////////////////
void LinkedListTest::testClear() {

    LinkedList<int> list;

    list.clear();

    CPPUNIT_ASSERT( list.size() == 0 );
    CPPUNIT_ASSERT( list.isEmpty() == true );

    list.add( 42 );
    list.add( 42 );
    list.add( 1 );

    CPPUNIT_ASSERT( list.size() == 3 );
    CPPUNIT_ASSERT( list.isEmpty() == false );

    list.clear();

    CPPUNIT_ASSERT( list.size() == 0 );
    CPPUNIT_ASSERT( list.isEmpty() == true );
}

////////////////////////////////////////////////////////////////////////////////
void LinkedListTest::testIndexOf() {

    LinkedList<int> list;

    CPPUNIT_ASSERT_EQUAL( -1, list.indexOf(42) );

    list.add( 21 );

    CPPUNIT_ASSERT_EQUAL( 0, list.indexOf(21) );

    list.add( 42 );
    list.add( 84 );
    list.add( 168 );

    CPPUNIT_ASSERT_EQUAL( 168, list.getLast() );

    CPPUNIT_ASSERT_EQUAL( 1, list.indexOf(42) );
    CPPUNIT_ASSERT_EQUAL( 0, list.indexOf(21) );
    CPPUNIT_ASSERT_EQUAL( 2, list.indexOf(84) );
    CPPUNIT_ASSERT_EQUAL( 3, list.indexOf(168) );
    CPPUNIT_ASSERT_EQUAL( -1, list.indexOf(336) );
}

////////////////////////////////////////////////////////////////////////////////
void LinkedListTest::testLastIndexOf() {

    LinkedList<int> list;

    CPPUNIT_ASSERT_EQUAL( -1, list.lastIndexOf(42) );

    list.add( 21 );

    CPPUNIT_ASSERT_EQUAL( 0, list.lastIndexOf(21) );

    list.add( 42 );
    list.add( 84 );
    list.add( 168 );

    CPPUNIT_ASSERT_EQUAL( 168, list.getLast() );

    CPPUNIT_ASSERT_EQUAL( 1, list.lastIndexOf(42) );
    CPPUNIT_ASSERT_EQUAL( 0, list.lastIndexOf(21) );
    CPPUNIT_ASSERT_EQUAL( 2, list.lastIndexOf(84) );
    CPPUNIT_ASSERT_EQUAL( 3, list.lastIndexOf(168) );
    CPPUNIT_ASSERT_EQUAL( -1, list.lastIndexOf(336) );

    list.add( 42 );
    CPPUNIT_ASSERT_EQUAL( list.size() - 1, list.lastIndexOf(42) );
}

////////////////////////////////////////////////////////////////////////////////
void LinkedListTest::testContains() {

    LinkedList<int> list;
    populate( list, SIZE );

    CPPUNIT_ASSERT( list.contains( 42 ) );
    CPPUNIT_ASSERT( list.contains( 0 ) );
    CPPUNIT_ASSERT( !list.contains( -1 ) );
    CPPUNIT_ASSERT( !list.contains( SIZE ) );
}

////////////////////////////////////////////////////////////////////////////////
void LinkedListTest::testContainsAll() {

    LinkedList<int> list;
    populate( list, SIZE );

    LinkedList<int> list2;
    populate( list2, SIZE*2 );

    CPPUNIT_ASSERT( list2.containsAll( list ) );
    CPPUNIT_ASSERT( !list.containsAll( list2 ) );
    CPPUNIT_ASSERT( list2.containsAll( list2 ) );
}

////////////////////////////////////////////////////////////////////////////////
void LinkedListTest::testToArray() {

    LinkedList<int> list;
    populate( list, SIZE );

    std::vector<int> array = list.toArray();
    CPPUNIT_ASSERT_EQUAL( SIZE, (int)array.size() );
    CPPUNIT_ASSERT_EQUAL( 0, array[0] );
    CPPUNIT_ASSERT_EQUAL( SIZE/2, array[SIZE/2] );
    CPPUNIT_ASSERT_EQUAL( SIZE-1, array[SIZE-1] );

    list.clear();
    array = list.toArray();
    CPPUNIT_ASSERT( array.empty() );
}

////////////////////////////////////////////////////////////////////////////////
void LinkedListTest::testPoll() {

    int result = 0;
    LinkedList<int> list;
    CPPUNIT_ASSERT( list.poll( result ) == false );
    populate( list, SIZE );

    for( int i = 0; i < SIZE; ++i ) {
        CPPUNIT_ASSERT( list.poll( result ) == true );
        CPPUNIT_ASSERT_EQUAL( i, result );
    }

    CPPUNIT_ASSERT( list.size() == 0 );
    CPPUNIT_ASSERT( list.poll( result ) == false );
}

////////////////////////////////////////////////////////////////////////////////
void LinkedListTest::testQRemove() {

    LinkedList<int> list;
    populate( list, SIZE );

    for( int i = 0; i < SIZE; ++i ) {
        CPPUNIT_ASSERT( list.remove() == i );
    }

    CPPUNIT_ASSERT( list.size() == 0 );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an NoSuchElementException",
        list.remove(),
        NoSuchElementException );
}

////////////////////////////////////////////////////////////////////////////////
void LinkedListTest::testPeek() {

    int result = 0;
    LinkedList<int> list;
    CPPUNIT_ASSERT( list.peek( result ) == false );
    populate( list, SIZE );

    for( int i = 0; i < SIZE; ++i ) {
        CPPUNIT_ASSERT( list.peek( result ) == true );
        CPPUNIT_ASSERT_EQUAL( i, result );
        list.remove();
    }

    CPPUNIT_ASSERT( list.size() == 0 );
    CPPUNIT_ASSERT( list.peek( result ) == false );
}

////////////////////////////////////////////////////////////////////////////////
void LinkedListTest::testElement() {

    LinkedList<int> list;
    populate( list, SIZE );

    for( int i = 0; i < SIZE; ++i ) {
        CPPUNIT_ASSERT( list.element() == i );
        list.remove();
    }

    CPPUNIT_ASSERT( list.size() == 0 );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an NoSuchElementException",
        list.element(),
        NoSuchElementException );
}

////////////////////////////////////////////////////////////////////////////////
void LinkedListTest::testOfferFirst() {

    LinkedList<int> list;
    CPPUNIT_ASSERT_EQUAL( true, list.offerFirst( 42 ) );
    CPPUNIT_ASSERT( list.size() == 1 );
    CPPUNIT_ASSERT( list.isEmpty() == false );
    CPPUNIT_ASSERT_EQUAL( 42, list.getLast() );
    CPPUNIT_ASSERT_EQUAL( 42, list.getFirst() );
    CPPUNIT_ASSERT_EQUAL( true, list.offerFirst( 84 ) );
    CPPUNIT_ASSERT( list.size() == 2 );
    CPPUNIT_ASSERT( list.isEmpty() == false );
    CPPUNIT_ASSERT_EQUAL( 84, list.getFirst() );
    CPPUNIT_ASSERT_EQUAL( 42, list.getLast() );
}

////////////////////////////////////////////////////////////////////////////////
void LinkedListTest::testOfferLast() {

    LinkedList<int> list;
    CPPUNIT_ASSERT_EQUAL( true, list.offerLast( 42 ) );
    CPPUNIT_ASSERT( list.size() == 1 );
    CPPUNIT_ASSERT( list.isEmpty() == false );
    CPPUNIT_ASSERT_EQUAL( 42, list.getLast() );
    CPPUNIT_ASSERT_EQUAL( 42, list.getFirst() );
    CPPUNIT_ASSERT_EQUAL( true, list.offerLast( 84 ) );
    CPPUNIT_ASSERT( list.size() == 2 );
    CPPUNIT_ASSERT( list.isEmpty() == false );
    CPPUNIT_ASSERT_EQUAL( 42, list.getFirst() );
    CPPUNIT_ASSERT_EQUAL( 84, list.getLast() );
}

////////////////////////////////////////////////////////////////////////////////
void LinkedListTest::testRemoveFirst() {

    LinkedList<int> list;
    populate( list, SIZE );

    for( int i = 0; i < SIZE; ++i ) {
        CPPUNIT_ASSERT( list.removeFirst() == i );
    }

    CPPUNIT_ASSERT( list.size() == 0 );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an NoSuchElementException",
        list.removeFirst(),
        NoSuchElementException );
}

////////////////////////////////////////////////////////////////////////////////
void LinkedListTest::testRemoveLast() {

    LinkedList<int> list;
    populate( list, SIZE );

    for( int i = 0; i < SIZE; ++i ) {
        CPPUNIT_ASSERT( list.removeLast() == SIZE - i - 1 );
    }

    CPPUNIT_ASSERT( list.size() == 0 );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an NoSuchElementException",
        list.removeLast(),
        NoSuchElementException );
}

////////////////////////////////////////////////////////////////////////////////
void LinkedListTest::testPollFirst() {

    int result = 0;
    LinkedList<int> list;
    CPPUNIT_ASSERT( list.pollFirst( result ) == false );
    populate( list, SIZE );

    for( int i = 0; i < SIZE; ++i ) {
        CPPUNIT_ASSERT( list.pollFirst( result ) == true );
        CPPUNIT_ASSERT_EQUAL( i, result );
    }

    CPPUNIT_ASSERT( list.size() == 0 );
    CPPUNIT_ASSERT( list.pollFirst( result ) == false );
}

////////////////////////////////////////////////////////////////////////////////
void LinkedListTest::testPollLast() {

    int result = 0;
    LinkedList<int> list;
    CPPUNIT_ASSERT( list.pollLast( result ) == false );
    populate( list, SIZE );

    for( int i = 0; i < SIZE; ++i ) {
        CPPUNIT_ASSERT( list.pollLast( result ) == true );
        CPPUNIT_ASSERT_EQUAL( SIZE - i - 1, result );
    }

    CPPUNIT_ASSERT( list.size() == 0 );
    CPPUNIT_ASSERT( list.pollLast( result ) == false );
}

////////////////////////////////////////////////////////////////////////////////
void LinkedListTest::testPeekFirst() {

    int result = 0;
    LinkedList<int> list;
    CPPUNIT_ASSERT( list.peekFirst( result ) == false );
    populate( list, SIZE );

    for( int i = 0; i < SIZE; ++i ) {
        CPPUNIT_ASSERT( list.peekFirst( result ) == true );
        CPPUNIT_ASSERT_EQUAL( i, result );
        list.removeFirst();
    }

    CPPUNIT_ASSERT( list.size() == 0 );
    CPPUNIT_ASSERT( list.peekFirst( result ) == false );
}

////////////////////////////////////////////////////////////////////////////////
void LinkedListTest::testPeekLast() {

    int result = 0;
    LinkedList<int> list;
    CPPUNIT_ASSERT( list.peekLast( result ) == false );
    populate( list, SIZE );

    for( int i = 0; i < SIZE; ++i ) {
        CPPUNIT_ASSERT( list.peekLast( result ) == true );
        CPPUNIT_ASSERT_EQUAL( SIZE - i - 1, result );
        list.removeLast();
    }

    CPPUNIT_ASSERT( list.size() == 0 );
    CPPUNIT_ASSERT( list.peekLast( result ) == false );
}

////////////////////////////////////////////////////////////////////////////////
void LinkedListTest::testPop() {

    LinkedList<int> list;
    populate( list, SIZE );

    for( int i = 0; i < SIZE; ++i ) {
        CPPUNIT_ASSERT( list.pop() == i );
    }

    CPPUNIT_ASSERT( list.size() == 0 );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an NoSuchElementException",
        list.pop(),
        NoSuchElementException );
}

////////////////////////////////////////////////////////////////////////////////
void LinkedListTest::testPush() {

    LinkedList<int> list;
    list.push( 42 );
    CPPUNIT_ASSERT( list.size() == 1 );
    CPPUNIT_ASSERT( list.isEmpty() == false );
    CPPUNIT_ASSERT_EQUAL( 42, list.getLast() );
    CPPUNIT_ASSERT_EQUAL( 42, list.getFirst() );
    list.push( 84 );
    CPPUNIT_ASSERT( list.size() == 2 );
    CPPUNIT_ASSERT( list.isEmpty() == false );
    CPPUNIT_ASSERT_EQUAL( 84, list.getFirst() );
    CPPUNIT_ASSERT_EQUAL( 42, list.getLast() );
}

//////////////////////////////////////////////////////////////////////////////
void LinkedListTest::testListIterator1IndexOutOfBoundsException() {

    LinkedList<std::string> list;

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IndexOutOfBoundsException",
        std::auto_ptr< ListIterator<std::string> > it( list.listIterator( -1 ) ),
        IndexOutOfBoundsException );
}

//////////////////////////////////////////////////////////////////////////////
void LinkedListTest::testListIterator2IndexOutOfBoundsException() {

    LinkedList<std::string> list;
    list.add( "1" );
    list.add( "2" );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IndexOutOfBoundsException",
        std::auto_ptr< ListIterator<std::string> > it( list.listIterator( 100 ) ),
        IndexOutOfBoundsException );
}

////////////////////////////////////////////////////////////////////////////////
void LinkedListTest::testIterator1() {

    LinkedList<int> list;
    populate( list, SIZE );

    std::auto_ptr< Iterator<int> > iter( list.iterator() );

    CPPUNIT_ASSERT( iter->hasNext() );

    int count = 0;
    while( iter->hasNext() ) {
        CPPUNIT_ASSERT_EQUAL( count++, iter->next() );
    }

    CPPUNIT_ASSERT_EQUAL( SIZE, count );

    CPPUNIT_ASSERT( !iter->hasNext() );
}

////////////////////////////////////////////////////////////////////////////////
void LinkedListTest::testIterator2() {

    LinkedList<std::string> list;

    list.add( "fred1" );
    list.add( "fred2" );
    list.add( "fred3" );

    std::auto_ptr< Iterator<std::string> > iterator1( list.iterator() );
    CPPUNIT_ASSERT( iterator1.get() != NULL );
    CPPUNIT_ASSERT( iterator1->hasNext() == true );

    int count = 0;
    while( iterator1->hasNext() ) {
        iterator1->next();
        ++count;
    }

    CPPUNIT_ASSERT( count == list.size() );

    std::auto_ptr< Iterator<std::string> > iterator2( list.iterator() );

    while( iterator2->hasNext() ) {
        iterator2->next();
        iterator2->remove();
    }

    CPPUNIT_ASSERT( list.isEmpty() );
}

////////////////////////////////////////////////////////////////////////////////
void LinkedListTest::testListIterator1() {

    LinkedList<int> list;
    populate( list, SIZE );

    std::auto_ptr< ListIterator<int> > iter( list.listIterator() );

    int index = 0;
    while( iter->hasNext() ) {

        if( index == 0 ) {
            CPPUNIT_ASSERT_MESSAGE( "The first element cannot have a previous", !iter->hasPrevious() );
        }
        if( index == SIZE ) {
            CPPUNIT_FAIL( std::string("List index should be capped at ") + Integer::toString(SIZE) );
        }

        int value = iter->next();
        if( index + 1 == SIZE ) {
            CPPUNIT_ASSERT_MESSAGE( "The last element cannot have a next", !iter->hasNext() );
        }

        CPPUNIT_ASSERT_EQUAL( index, value );

        if( index > 0 && index < (SIZE - 1) ) {
            CPPUNIT_ASSERT_MESSAGE( "Next index returned incorrect value",
                                    iter->nextIndex() == index + 1 );
            CPPUNIT_ASSERT_EQUAL_MESSAGE( "previousIndex returned incorrect value :",
                                          iter->previousIndex(), index );
        }

        index++;
    }

    LinkedList<int> myList;
    populate( myList, 5 );
    iter.reset( myList.listIterator() );

    CPPUNIT_ASSERT_MESSAGE( "hasPrevious() should be false", !iter->hasPrevious() );
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "next() should be 0", 0, iter->next() );
    CPPUNIT_ASSERT_MESSAGE( "hasPrevious() should be true", iter->hasPrevious() );
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "prev() should be 0", 0, iter->previous() );
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "next() should be 0", 0, iter->next() );
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "next() should be 1", 1, iter->next() );
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "next() should be 2", 2, iter->next() );
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "next() should be 3", 3, iter->next() );
    CPPUNIT_ASSERT_MESSAGE( "hasNext() should be true", iter->hasNext() );
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "next() should be 4", 4, iter->next() );
    CPPUNIT_ASSERT_MESSAGE( "hasNext() should be false", !iter->hasNext() );
}

////////////////////////////////////////////////////////////////////////////////
void LinkedListTest::testListIterator2() {

    LinkedList<int> list;
    list.add(1);
    list.add(2);

    std::auto_ptr< ListIterator<int> > iter( list.listIterator() );

    while( iter->hasNext() ) {
        iter->next();
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a NoSuchElementException",
        iter->next(),
        NoSuchElementException );
    CPPUNIT_ASSERT_EQUAL( list.size(), iter->nextIndex() );

    list.add(3);
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a ConcurrentModificationException",
        iter->next(),
        ConcurrentModificationException );
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a ConcurrentModificationException",
        iter->add(42),
        ConcurrentModificationException );
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a ConcurrentModificationException",
        iter->remove(),
        ConcurrentModificationException );
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a ConcurrentModificationException",
        iter->previous(),
        ConcurrentModificationException );

    iter.reset( list.listIterator() );
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a IllegalStateException",
        iter->previous(),
        IllegalStateException );
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a IllegalStateException",
        iter->set( 42 ),
        IllegalStateException );

    int value = iter->next();
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "Should have returned first element", 1, value );
}

////////////////////////////////////////////////////////////////////////////////
void LinkedListTest::testListIterator3() {

    LinkedList<int> list;
    std::auto_ptr< ListIterator<int> > iter( list.listIterator() );
    CPPUNIT_ASSERT_EQUAL( -1, iter->previousIndex() );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a NoSuchElementException",
        iter->next(),
        NoSuchElementException );

    CPPUNIT_ASSERT_MESSAGE( "hasNext() should be false", !iter->hasNext() );
    iter->add(42);

    CPPUNIT_ASSERT_MESSAGE( "hasPrevious() should be true", iter->hasPrevious() );
    CPPUNIT_ASSERT_EQUAL( 42, iter->previous() );
    CPPUNIT_ASSERT_MESSAGE( "hasNext() should be true", iter->hasNext() );
    CPPUNIT_ASSERT_EQUAL( 42, iter->next() );
    CPPUNIT_ASSERT_MESSAGE( "hasNext() should be false", !iter->hasNext() );
    iter->set(84);
    CPPUNIT_ASSERT_EQUAL( 84, iter->previous() );
    iter->set(42);
    CPPUNIT_ASSERT_EQUAL( 42, iter->next() );
    CPPUNIT_ASSERT_EQUAL( 1, list.size() );
}

////////////////////////////////////////////////////////////////////////////////
void LinkedListTest::testListIterator4() {

    LinkedList<int> list;
    std::auto_ptr< ListIterator<int> > iter( list.listIterator( 0 ) );
    CPPUNIT_ASSERT_EQUAL( 0, iter->nextIndex() );
    CPPUNIT_ASSERT_EQUAL( -1, iter->previousIndex() );

    populate( list, 3 );
    iter.reset( list.listIterator( 0 ) );
    CPPUNIT_ASSERT_EQUAL( 0, iter->nextIndex() );
    CPPUNIT_ASSERT_EQUAL( -1, iter->previousIndex() );

    iter.reset( list.listIterator( 1 ) );
    CPPUNIT_ASSERT_EQUAL( 1, iter->nextIndex() );
    CPPUNIT_ASSERT_EQUAL( 0, iter->previousIndex() );

    iter.reset( list.listIterator( 2 ) );
    CPPUNIT_ASSERT_EQUAL( 2, iter->nextIndex() );
    CPPUNIT_ASSERT_EQUAL( 1, iter->previousIndex() );
}

////////////////////////////////////////////////////////////////////////////////
void LinkedListTest::testDescendingIterator() {

    LinkedList<int> list;
    std::auto_ptr< Iterator<int> > iter( list.descendingIterator() );

    CPPUNIT_ASSERT( !iter->hasNext() );

    populate( list, 5 );

    iter.reset( list.descendingIterator() );
    CPPUNIT_ASSERT_EQUAL( 5, list.size() );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a IllegalStateException",
        iter->remove(),
        IllegalStateException );

    list.add( 5 );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a ConcurrentModificationException",
        iter->remove(),
        ConcurrentModificationException );

    iter.reset( list.descendingIterator() );
    CPPUNIT_ASSERT_EQUAL( 5, iter->next() );
    CPPUNIT_ASSERT_EQUAL( 4, iter->next() );
    CPPUNIT_ASSERT_EQUAL( 3, iter->next() );
    CPPUNIT_ASSERT_EQUAL( 2, iter->next() );
    CPPUNIT_ASSERT_EQUAL( 1, iter->next() );

    CPPUNIT_ASSERT( iter->hasNext() );
    iter->remove();
    CPPUNIT_ASSERT_EQUAL( 0, iter->next() );
    CPPUNIT_ASSERT( !iter->hasNext() );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a NoSuchElementException",
        iter->next(),
        NoSuchElementException );
}

////////////////////////////////////////////////////////////////////////////////
void LinkedListTest::testRemoveFirstOccurrence() {

    LinkedList<int> list;
    std::auto_ptr< Iterator<int> > iter( list.descendingIterator() );

    CPPUNIT_ASSERT( list.offerLast(1) );
    CPPUNIT_ASSERT( list.offerLast(2) );
    CPPUNIT_ASSERT( list.offerLast(1) );
    CPPUNIT_ASSERT( list.offerLast(3) );
    CPPUNIT_ASSERT( list.offerLast(1) );

    CPPUNIT_ASSERT_EQUAL( 5, list.size() );

    CPPUNIT_ASSERT( list.removeFirstOccurrence(1) );
    CPPUNIT_ASSERT( !list.removeFirstOccurrence(4) );
    CPPUNIT_ASSERT_EQUAL( 2, list.getFirst() );
    CPPUNIT_ASSERT_EQUAL( 1, list.getLast() );
    CPPUNIT_ASSERT_EQUAL( 4, list.size() );
    CPPUNIT_ASSERT( list.removeFirstOccurrence(1) );
    CPPUNIT_ASSERT_EQUAL( 3, list.size() );
    CPPUNIT_ASSERT_EQUAL( 1, list.getLast() );
    CPPUNIT_ASSERT( list.removeFirstOccurrence(1) );
    CPPUNIT_ASSERT_EQUAL( 2, list.size() );
    CPPUNIT_ASSERT_EQUAL( 3, list.getLast() );
    CPPUNIT_ASSERT( !list.removeFirstOccurrence(1) );
}

////////////////////////////////////////////////////////////////////////////////
void LinkedListTest::testRemoveLastOccurrence() {

    LinkedList<int> list;
    std::auto_ptr< Iterator<int> > iter( list.descendingIterator() );

    CPPUNIT_ASSERT( list.offerLast(1) );
    CPPUNIT_ASSERT( list.offerLast(2) );
    CPPUNIT_ASSERT( list.offerLast(1) );
    CPPUNIT_ASSERT( list.offerLast(3) );
    CPPUNIT_ASSERT( list.offerLast(1) );

    CPPUNIT_ASSERT_EQUAL( 5, list.size() );

    CPPUNIT_ASSERT( list.removeLastOccurrence(1) );
    CPPUNIT_ASSERT( !list.removeLastOccurrence(4) );
    CPPUNIT_ASSERT_EQUAL( 1, list.getFirst() );
    CPPUNIT_ASSERT_EQUAL( 3, list.getLast() );
    CPPUNIT_ASSERT_EQUAL( 4, list.size() );
    CPPUNIT_ASSERT( list.removeLastOccurrence(1) );
    CPPUNIT_ASSERT_EQUAL( 3, list.size() );
    CPPUNIT_ASSERT_EQUAL( 3, list.getLast() );
    CPPUNIT_ASSERT( list.removeLastOccurrence(1) );
    CPPUNIT_ASSERT_EQUAL( 2, list.size() );
    CPPUNIT_ASSERT_EQUAL( 3, list.getLast() );
    CPPUNIT_ASSERT( !list.removeLastOccurrence(1) );
}
