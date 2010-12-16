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

#include "ListTest.h"

#include <decaf/util/List.h>
#include <decaf/util/StlList.h>
#include <decaf/util/StlSet.h>
#include <decaf/util/Iterator.h>
#include <decaf/lang/Integer.h>

using namespace std;
using namespace decaf;
using namespace decaf::util;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
const int ListTest::SIZE = 256;

////////////////////////////////////////////////////////////////////////////////
namespace {

    void populate( StlList<int>& list, int n ) {

        CPPUNIT_ASSERT( list.isEmpty() );

        for( int i = 0; i < n; ++i ) {
            list.add( i );
        }

        CPPUNIT_ASSERT( !list.isEmpty());
        CPPUNIT_ASSERT_EQUAL( n, list.size() );
    }

    void populate( StlList<std::string>& list, int n ) {

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
ListTest::ListTest(){
}

////////////////////////////////////////////////////////////////////////////////
void ListTest::testConstructor1(){

    StlList<int> list;

    CPPUNIT_ASSERT( list.size() == 0 );
    CPPUNIT_ASSERT( list.isEmpty() );

    list.add( 1 );

    CPPUNIT_ASSERT( list.size() == 1 );
    CPPUNIT_ASSERT( !list.isEmpty() );

    list.add( 1 );

    CPPUNIT_ASSERT( list.size() == 2 );
    CPPUNIT_ASSERT( !list.isEmpty() );
}

////////////////////////////////////////////////////////////////////////////////
void ListTest::testConstructor2(){

    StlList<int> list1;

    for( int i = 0; i < 50; ++i ) {
        list1.add( i );
    }

    StlList<int> list2( list1 );

    CPPUNIT_ASSERT( list1.size() == list2.size() );

    for( int i = 0; i < 50; ++i ) {
        CPPUNIT_ASSERT( list2.contains( i ) );
    }

    CPPUNIT_ASSERT( list2.equals( list1 ) );
}

////////////////////////////////////////////////////////////////////////////////
void ListTest::testConstructor3(){

    StlSet<int> collection;

    for( int i = 0; i < 50; ++i ) {
        collection.add( i );
    }

    StlList<int> list( collection );

    CPPUNIT_ASSERT( collection.size() == list.size() );

    for( int i = 0; i < 50; ++i ) {
        CPPUNIT_ASSERT( list.contains( i ) );
    }

    CPPUNIT_ASSERT( list.equals( collection ) );
}

//////////////////////////////////////////////////////////////////////////////
void ListTest::testEquals() {

    StlList<int> list1;
    populate( list1, 7 );
    StlList<int> list2;
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
void ListTest::testCopy1() {

    StlList<int> list1;

    for( int i = 0; i < 50; ++i ) {
        list1.add( i );
    }

    StlList<int> list2;

    list2.copy( list1 );

    CPPUNIT_ASSERT( list1.size() == list2.size() );

    for( int i = 0; i < 50; ++i ) {
        CPPUNIT_ASSERT( list2.contains( i ) );
    }

    CPPUNIT_ASSERT( list2.equals( list1 ) );
}

////////////////////////////////////////////////////////////////////////////////
void ListTest::testCopy2() {

    StlSet<int> collection;

    for( int i = 0; i < 50; ++i ) {
        collection.add( i );
    }

    StlList<int> list;

    list.copy( collection );

    CPPUNIT_ASSERT( collection.size() == list.size() );

    for( int i = 0; i < 50; ++i ) {
        CPPUNIT_ASSERT( list.contains( i ) );
    }

    CPPUNIT_ASSERT( list.equals( collection ) );
}

////////////////////////////////////////////////////////////////////////////////
void ListTest::testContains(){

    StlList<string> list;
    CPPUNIT_ASSERT( list.contains( "bob" ) == false);

    list.add( "bob" );

    CPPUNIT_ASSERT(list.contains( "bob" ) == true );
    CPPUNIT_ASSERT(list.contains( "fred" ) == false );
}

////////////////////////////////////////////////////////////////////////////////
void ListTest::testIndexOf(){

    StlList<string> list;

    list.add( "bob" );    // 0
    list.add( "fred" );   // 1
    list.add( "george" ); // 2
    list.add( "steve" );  // 3

    CPPUNIT_ASSERT( list.indexOf( "bob" ) == 0 );
    CPPUNIT_ASSERT( list.indexOf( "fred" ) == 1 );
    CPPUNIT_ASSERT( list.indexOf( "george" ) == 2 );
    CPPUNIT_ASSERT( list.indexOf( "steve" ) == 3 );

    list.remove( "fred" );

    CPPUNIT_ASSERT( list.indexOf( "bob" ) == 0 );
    CPPUNIT_ASSERT( list.indexOf( "fred" ) == -1 );
    CPPUNIT_ASSERT( list.indexOf( "george" ) == 1 );
    CPPUNIT_ASSERT( list.indexOf( "steve" ) == 2 );
}

////////////////////////////////////////////////////////////////////////////////
void ListTest::testLastIndexOf(){

    StlList<string> list;

    list.add( "bob" );    // 0
    list.add( "fred" );   // 1
    list.add( "george" ); // 2
    list.add( "bob" );    // 3

    CPPUNIT_ASSERT_MESSAGE( "indexOf 'bob' before remove failed", list.indexOf( "bob" ) == 0 );
    CPPUNIT_ASSERT_MESSAGE( "lastIndexOf 'bob' before remove failed", list.lastIndexOf( "bob" ) == 3 );

    list.remove( "fred" );

    CPPUNIT_ASSERT_MESSAGE( "indexOf 'bob' after remove failed", list.indexOf( "bob" ) == 0 );
    CPPUNIT_ASSERT_MESSAGE( "lastIndexOf 'bob' after remove failed", list.lastIndexOf( "bob" ) == 2 );

    list.remove( "bob" );

    CPPUNIT_ASSERT( list.indexOf( "bob" ) == -1 );
    CPPUNIT_ASSERT( list.lastIndexOf( "bob" ) == -1 );
}

////////////////////////////////////////////////////////////////////////////////
void ListTest::testClear(){

    StlList<string> list;
    list.add( "bob" );
    list.add( "fred" );

    CPPUNIT_ASSERT( list.size() == 2 );
    list.clear();
    CPPUNIT_ASSERT( list.size() == 0 );
}

////////////////////////////////////////////////////////////////////////////////
void ListTest::testIsEmpty(){

    StlList<string> list;
    list.add( "bob" );
    list.add( "fred" );

    CPPUNIT_ASSERT(list.isEmpty() == false );
    list.clear();
    CPPUNIT_ASSERT(list.isEmpty() == true );
}

////////////////////////////////////////////////////////////////////////////////
void ListTest::testSize(){

    StlList<string> list;

    CPPUNIT_ASSERT( list.size() == 0 );
    list.add( "bob" );
    CPPUNIT_ASSERT( list.size() == 1 );
    list.add( "fred" );
    CPPUNIT_ASSERT( list.size() == 2 );
}

////////////////////////////////////////////////////////////////////////////////
void ListTest::testGet(){
    StlList<string> list;

    list.add( "fred" );
    list.add( "fred" );
    list.add( "fred" );
    list.add( "bob" );

    CPPUNIT_ASSERT( list.get(0) == "fred" );
    CPPUNIT_ASSERT( list.get(1) == "fred" );
    CPPUNIT_ASSERT( list.get(2) == "fred" );
    CPPUNIT_ASSERT( list.get(3) == "bob" );
    list.remove( "fred" );
    CPPUNIT_ASSERT( list.get(0) == "bob" );
}

////////////////////////////////////////////////////////////////////////////////
void ListTest::testSet(){
    StlList<string> list;

    list.add( "fred" );
    list.add( "fred" );
    list.add( "fred" );
    list.add( "bob" );

    CPPUNIT_ASSERT( list.get(0) == "fred" );
    CPPUNIT_ASSERT( list.get(1) == "fred" );
    CPPUNIT_ASSERT( list.get(2) == "fred" );
    CPPUNIT_ASSERT( list.get(3) == "bob" );

    list.set( 0, "steve" );
    list.set( 1, "joe" );

    CPPUNIT_ASSERT( list.get(0) == "steve" );
    CPPUNIT_ASSERT( list.get(1) == "joe" );
    CPPUNIT_ASSERT( list.get(2) == "fred" );
    CPPUNIT_ASSERT( list.get(3) == "bob" );
}

////////////////////////////////////////////////////////////////////////////////
void ListTest::testAdd(){
    StlList<string> list;

    list.add( "fred" );
    list.add( "fred" );
    list.add( "fred" );
    CPPUNIT_ASSERT( list.contains("fred") == true );
    CPPUNIT_ASSERT( list.size() == 3 );
    list.remove( "fred" );
    CPPUNIT_ASSERT( list.contains("fred") == false );
    CPPUNIT_ASSERT( list.isEmpty() );
}

////////////////////////////////////////////////////////////////////////////////
void ListTest::testAdd2(){
    StlList<string> list;

    list.add( "fred" );
    list.add( "fred" );
    list.add( "fred" );

    CPPUNIT_ASSERT( list.get(0) == "fred" );
    CPPUNIT_ASSERT( list.get(1) == "fred" );
    CPPUNIT_ASSERT( list.get(2) == "fred" );

    list.add( 1, "bob" );

    CPPUNIT_ASSERT( list.get(0) == "fred" );
    CPPUNIT_ASSERT( list.get(1) == "bob" );
    CPPUNIT_ASSERT( list.get(2) == "fred" );
    CPPUNIT_ASSERT( list.get(3) == "fred" );

    list.add( 3, "bob" );

    CPPUNIT_ASSERT( list.get(3) == "bob" );
    CPPUNIT_ASSERT( list.get(4) == "fred" );

    list.add( 5, "bob" );

    CPPUNIT_ASSERT( list.get(4) == "fred" );
    CPPUNIT_ASSERT( list.get(5) == "bob" );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IndexOutOfBoundsException",
        list.add( list.size() + 1, "bob" ),
        decaf::lang::exceptions::IndexOutOfBoundsException );
}

////////////////////////////////////////////////////////////////////////////////
void ListTest::testAdd3() {

    StlList<int> array;
    populate( array, SIZE );
    std::vector<int> mirror;
    populate( mirror, SIZE );

    array.add( 50, 42 );
    CPPUNIT_ASSERT_MESSAGE( "Failed to add Object", array.get( 50 ) == 42 );
    CPPUNIT_ASSERT_MESSAGE( "Failed to fix up list after insert",
                            array.get( 51 ) == mirror[50] && ( array.get( 52 ) == mirror[51] ) );
    int oldItem = array.get( 25 );
    array.add( 25, 0 );
    CPPUNIT_ASSERT_MESSAGE( "Should have returned zero", array.get( 25 ) == 0 );
    CPPUNIT_ASSERT_MESSAGE( "Should have returned the old item from slot 25", array.get( 26 ) == oldItem );

    array.add( 0, 84 );
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "Failed to add Object", array.get( 0 ), 84 );
    CPPUNIT_ASSERT_EQUAL( array.get( 1 ), mirror[0] );
    CPPUNIT_ASSERT_EQUAL( array.get( 2 ), mirror[1] );

    oldItem = array.get( 0 );
    array.add( 0, 0 );
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "Should have returned null", 0, array.get( 0 ) );
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "Should have returned the old item from slot 0", array.get( 1 ), oldItem );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IndexOutOfBoundsException",
        array.add( -1, 0 ),
        IndexOutOfBoundsException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IndexOutOfBoundsException",
        array.add( array.size() + 1, 0 ),
        IndexOutOfBoundsException );
}

////////////////////////////////////////////////////////////////////////////////
void ListTest::testAddAll1() {

    StlList<int> array;
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
void ListTest::testAddAll2() {

    StlList<int> emptyCollection;
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IndexOutOfBoundsException",
        StlList<int>().addAll( -1, emptyCollection ),
        IndexOutOfBoundsException );

    {
        std::string data[] = { "1", "2", "3", "4", "5", "6", "7", "8" };
        StlList<std::string> list1;
        StlList<std::string> list2;
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

        StlList<std::string> list3;
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
        StlList<std::string> list1;
        StlList<std::string> list2;
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
void ListTest::testAddAll3() {

    StlList<int> list;
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
void ListTest::testAddAll4() {

    StlList<std::string> array;
    StlList<std::string> blist;

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
void ListTest::testAddAll5() {

    StlList<std::string> array;
    populate( array, 100 );

    StlList<std::string> l;
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

    StlList<int> originalList;
    for( int j = 0; j < 12; j++ ) {
        originalList.add( j );
    }

    originalList.removeAt( 0 );
    originalList.removeAt( 0 );

    StlList<int> additionalList;
    for( int j = 0; j < 11; j++ ) {
        additionalList.add( j );
    }
    CPPUNIT_ASSERT( originalList.addAll( additionalList ) );
    CPPUNIT_ASSERT_EQUAL( 21, originalList.size() );
}

////////////////////////////////////////////////////////////////////////////////
void ListTest::testAddAll6() {

    StlList<int> arrayListA;
    arrayListA.add( 1 );
    StlList<int> arrayListB;
    arrayListB.add( 1 );
    arrayListA.addAll( 1, arrayListB );
    int size = arrayListA.size();
    CPPUNIT_ASSERT_EQUAL( 2, size );
    for( int index = 0; index < size; index++ ) {
        CPPUNIT_ASSERT_EQUAL( 1, arrayListA.get( index ) );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ListTest::testAddAll7() {

    StlList<int> arrayList;
    arrayList.add( 1 );
    arrayList.addAll( 1, arrayList );
    int size = arrayList.size();
    CPPUNIT_ASSERT_EQUAL( 2, size );
    for( int index = 0; index < size; index++ ) {
        CPPUNIT_ASSERT_EQUAL( 1, arrayList.get( index ) );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ListTest::testAddAll8() {

    StlList<std::string> arrayList;
    arrayList.add( "1" );
    arrayList.add( "2" );

    StlList<std::string> list;
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
void ListTest::testAddAll9() {

    StlList<std::string> list;
    list.add( "one" );
    list.add( "two" );
    CPPUNIT_ASSERT_EQUAL( 2, list.size() );

    list.removeAt( 0 );
    CPPUNIT_ASSERT_EQUAL( 1, list.size() );

    StlList<std::string> collection;
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
void ListTest::testRemove(){
    StlList<string> list;

    list.add( "fred" );
    CPPUNIT_ASSERT( list.contains( "fred" ) == true );
    list.remove( "fred" );
    CPPUNIT_ASSERT( list.contains( "fred" ) == false );
}

////////////////////////////////////////////////////////////////////////////////
void ListTest::testRemove2(){
    StlList<string> list;

    list.add( "fred" );
    list.add( "bob" );
    list.add( "steve" );
    list.add( "mike" );
    list.add( "larry" );

    CPPUNIT_ASSERT( list.removeAt(0) == "fred" );
    CPPUNIT_ASSERT( list.get(0) == "bob" );

    CPPUNIT_ASSERT( list.removeAt(2) == "mike" );
    CPPUNIT_ASSERT( list.get(2) == "larry" );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IndexOutOfBoundsException",
        list.removeAt( list.size() + 1 ),
        decaf::lang::exceptions::IndexOutOfBoundsException );
}

//////////////////////////////////////////////////////////////////////////////
void ListTest::testRemove3() {

    StlList<int> list1;
    populate( list1, SIZE );
    StlList<int> list2;
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

////////////////////////////////////////////////////////////////////////////////
void ListTest::testRemoveAt() {

    StlList<int> array;
    populate( array, SIZE );

    array.removeAt( 10 );
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Failed to remove element", -1, array.indexOf( 10 ) );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IndexOutOfBoundsException",
        array.removeAt( 9999 ),
        IndexOutOfBoundsException );

    StlList<int> myArray( array );
    array.add( 25, 9999 );
    array.add( 50, 9999 );
    array.removeAt( 50 );
    array.removeAt( 25 );
    CPPUNIT_ASSERT_MESSAGE("Removing index did not work", array.equals( myArray ) );

    std::string data[] = { "a", "b", "c", "d", "e", "f", "g" };
    StlList<std::string> list;
    for( int i = 0; i < 7; ++i ) {
        list.add( data[i] );
    }

    CPPUNIT_ASSERT_MESSAGE( "Removed wrong element 1", list.removeAt(0) == "a" );
    CPPUNIT_ASSERT_MESSAGE( "Removed wrong element 2", list.removeAt(4) == "f" );

    StlList<int> l;
    l.add( 5 );
    l.add( 6 );
    l.removeAt( 0 );
    l.removeAt( 0 );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IndexOutOfBoundsException",
        l.removeAt( -1 ),
        IndexOutOfBoundsException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IndexOutOfBoundsException",
        l.removeAt( 0 ),
        IndexOutOfBoundsException );
}

////////////////////////////////////////////////////////////////////////////////
void ListTest::testToArray(){

    StlList<string> list;

    list.add( "fred1" );
    list.add( "fred2" );
    list.add( "fred3" );
    CPPUNIT_ASSERT( list.size() == 3 );

    std::vector<std::string> array = list.toArray();

    CPPUNIT_ASSERT( array.size() == 3 );
}

////////////////////////////////////////////////////////////////////////////////
void ListTest::testIterator(){

    StlList<string> list;

    list.add( "fred1" );
    list.add( "fred2" );
    list.add( "fred3" );

    Iterator<string>* iterator1 = list.iterator();
    CPPUNIT_ASSERT( iterator1 != NULL );
    CPPUNIT_ASSERT( iterator1->hasNext() == true );

    int count = 0;
    while( iterator1->hasNext() ) {
        iterator1->next();
        ++count;
    }

    CPPUNIT_ASSERT( count == list.size() );

    Iterator<string>* iterator2 = list.iterator();

    while( iterator2->hasNext() ) {
        iterator2->next();
        iterator2->remove();
    }

    CPPUNIT_ASSERT( list.isEmpty() );

    delete iterator1;
    delete iterator2;
}

//////////////////////////////////////////////////////////////////////////////
void ListTest::testListIterator1IndexOutOfBoundsException() {

    StlList<int> list;

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IndexOutOfBoundsException",
        std::auto_ptr< ListIterator<int> > it( list.listIterator( -1 ) ),
        IndexOutOfBoundsException );
}

//////////////////////////////////////////////////////////////////////////////
void ListTest::testListIterator2IndexOutOfBoundsException() {

    StlList<int> list;
    list.add( 1 );
    list.add( 2 );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IndexOutOfBoundsException",
        std::auto_ptr< ListIterator<int> > it( list.listIterator( 100 ) ),
        IndexOutOfBoundsException );
}
