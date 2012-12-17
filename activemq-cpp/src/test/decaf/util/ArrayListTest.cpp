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

#include "ArrayListTest.h"

#include <decaf/lang/exceptions/UnsupportedOperationException.h>
#include <decaf/util/ArrayList.h>
#include <decaf/util/StlList.h>
#include <decaf/lang/Integer.h>

using namespace std;
using namespace decaf;
using namespace decaf::util;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
const int ArrayListTest::SIZE = 256;

////////////////////////////////////////////////////////////////////////////////
namespace {

    void populate( ArrayList<int>& list, int n ) {

        CPPUNIT_ASSERT( list.isEmpty() );

        for( int i = 0; i < n; ++i ) {
            list.add( i );
        }

        CPPUNIT_ASSERT( !list.isEmpty());
        CPPUNIT_ASSERT_EQUAL( n, list.size() );
    }

    void populate( ArrayList<std::string>& list, int n ) {

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

    template< typename E>
    class MockArrayList : public ArrayList<E> {
    public:

        MockArrayList() : ArrayList<E>() {
        }

        virtual ~MockArrayList() {}

        virtual int size() const {
            return 0;
        }
    };

}

////////////////////////////////////////////////////////////////////////////////
ArrayListTest::ArrayListTest() {
}

////////////////////////////////////////////////////////////////////////////////
ArrayListTest::~ArrayListTest() {
}

////////////////////////////////////////////////////////////////////////////////
void ArrayListTest::testConstructor1() {

    ArrayList<int> array;
    CPPUNIT_ASSERT( array.isEmpty() );
    CPPUNIT_ASSERT( array.size() == 0 );

    ArrayList<std::string> strArray;
    CPPUNIT_ASSERT( strArray.isEmpty() );
    CPPUNIT_ASSERT( strArray.size() == 0 );
}

////////////////////////////////////////////////////////////////////////////////
void ArrayListTest::testConstructor2() {

    StlList<int> intsList;

    for( int i = 0; i < SIZE; ++i ) {
        intsList.add( i );
    }

    ArrayList<int> array( intsList );
    CPPUNIT_ASSERT( !array.isEmpty() );
    CPPUNIT_ASSERT( array.size() == SIZE );

    for( int i = 0; i < SIZE; ++i ) {
        CPPUNIT_ASSERT_EQUAL( intsList.get( i ), array.get( i ) );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ArrayListTest::testClear() {

    ArrayList<int> array( 100 );
    CPPUNIT_ASSERT_MESSAGE( "List size should be zero", 0 == array.size() );
    CPPUNIT_ASSERT_MESSAGE( "List should indicate its empty", array.isEmpty() );
    array.add( 25 );
    array.clear();
    CPPUNIT_ASSERT_MESSAGE( "List size should be zero", 0 == array.size() );
    CPPUNIT_ASSERT_MESSAGE( "List should indicate its empty", array.isEmpty() );

    ArrayList<int> array1;
    CPPUNIT_ASSERT_MESSAGE( "List size should be zero", 0 == array1.size() );
    CPPUNIT_ASSERT_MESSAGE( "List should indicate its empty", array1.isEmpty() );
    array1.add( 25 );
    array1.clear();
    CPPUNIT_ASSERT_MESSAGE( "List size should be zero", 0 == array1.size() );
    CPPUNIT_ASSERT_MESSAGE( "List should indicate its empty", array1.isEmpty() );
}

////////////////////////////////////////////////////////////////////////////////
void ArrayListTest::testConstructor3() {

    ArrayList<int> array( 100 );
    CPPUNIT_ASSERT( array.isEmpty() );
    CPPUNIT_ASSERT( array.size() == 0 );

    ArrayList<std::string> strArray( 42 );
    CPPUNIT_ASSERT( strArray.isEmpty() );
    CPPUNIT_ASSERT( strArray.size() == 0 );
}

////////////////////////////////////////////////////////////////////////////////
void ArrayListTest::testAdd1() {

    ArrayList<int> array( 100 );
    array.add( 25 );
    CPPUNIT_ASSERT_MESSAGE( "Failed to add Object", array.get( 0 ) == 25 );

    ArrayList<int> array1;
    array1.add( 25 );
    CPPUNIT_ASSERT_MESSAGE( "Failed to add Object", array1.get( 0 ) == 25 );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IllegalArgumentException",
        ArrayList<int>( -1 ),
        IllegalArgumentException );
}

////////////////////////////////////////////////////////////////////////////////
void ArrayListTest::testAdd2() {

    ArrayList<int> array;
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
void ArrayListTest::testAdd3() {

    ArrayList<int> array;
    populate( array, SIZE );
    std::vector<int> mirror;
    populate( mirror, SIZE );

    int size = array.size();
    array.add( size, 42 );
    CPPUNIT_ASSERT( array.size() == SIZE + 1 );

    CPPUNIT_ASSERT_EQUAL_MESSAGE( "Failed to add Object", array.get( size ), 42 );
    CPPUNIT_ASSERT_EQUAL( array.get( size - 2 ), mirror[size - 2] );
    CPPUNIT_ASSERT_EQUAL( array.get( size - 1 ), mirror[size - 1] );

    array.removeAt( size );
    CPPUNIT_ASSERT( array.size() == SIZE );

    size = array.size();
    array.add( size, 0 );
    CPPUNIT_ASSERT( array.size() == SIZE + 1 );

    CPPUNIT_ASSERT_EQUAL_MESSAGE( "Should have returned 0", 0, array.get( size ) );
    CPPUNIT_ASSERT_EQUAL( array.get( size - 2 ), mirror[size - 2] );
    CPPUNIT_ASSERT_EQUAL( array.get( size - 1 ), mirror[size - 1] );
}

////////////////////////////////////////////////////////////////////////////////
void ArrayListTest::testAddAll1() {

    ArrayList<int> array;
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
void ArrayListTest::testAddAll2() {

    StlList<int> emptyCollection;
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IndexOutOfBoundsException",
        ArrayList<int>().addAll( -1, emptyCollection ),
        IndexOutOfBoundsException );

    {
        std::string data[] = { "1", "2", "3", "4", "5", "6", "7", "8" };
        ArrayList<std::string> list1;
        ArrayList<std::string> list2;
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

        ArrayList<std::string> list3;
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
        ArrayList<std::string> list1;
        ArrayList<std::string> list2;
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
void ArrayListTest::testAddAll3() {

    ArrayList<int> list;
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
void ArrayListTest::testAddAll4() {

    ArrayList<std::string> array;
    ArrayList<std::string> blist;

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
void ArrayListTest::testAddAll5() {

    ArrayList<std::string> array;
    populate( array, 100 );

    ArrayList<std::string> l;
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

    ArrayList<int> originalList( 12 );
    for( int j = 0; j < 12; j++ ) {
        originalList.add( j );
    }

    originalList.removeAt( 0 );
    originalList.removeAt( 0 );

    ArrayList<int> additionalList( 11 );
    for( int j = 0; j < 11; j++ ) {
        additionalList.add( j );
    }
    CPPUNIT_ASSERT( originalList.addAll( additionalList ) );
    CPPUNIT_ASSERT_EQUAL( 21, originalList.size() );
}

////////////////////////////////////////////////////////////////////////////////
void ArrayListTest::testAddAll6() {

    ArrayList<int> arrayListA;
    arrayListA.add( 1 );
    ArrayList<int> arrayListB;
    arrayListB.add( 1 );
    arrayListA.addAll( 1, arrayListB );
    int size = arrayListA.size();
    CPPUNIT_ASSERT_EQUAL( 2, size );
    for( int index = 0; index < size; index++ ) {
        CPPUNIT_ASSERT_EQUAL( 1, arrayListA.get( index ) );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ArrayListTest::testAddAll7() {

    ArrayList<int> arrayList;
    arrayList.add( 1 );
    arrayList.addAll( 1, arrayList );
    int size = arrayList.size();
    CPPUNIT_ASSERT_EQUAL( 2, size );
    for( int index = 0; index < size; index++ ) {
        CPPUNIT_ASSERT_EQUAL( 1, arrayList.get( index ) );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ArrayListTest::testAddAll8() {

    ArrayList<std::string> arrayList;
    arrayList.add( "1" );
    arrayList.add( "2" );

    ArrayList<std::string> list;
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
void ArrayListTest::testAddAll9() {

    ArrayList<std::string> list;
    list.add( "one" );
    list.add( "two" );
    CPPUNIT_ASSERT_EQUAL( 2, list.size() );

    list.removeAt( 0 );
    CPPUNIT_ASSERT_EQUAL( 1, list.size() );

    ArrayList<std::string> collection;
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
void ArrayListTest::testContains() {

    ArrayList<int> array;
    populate( array, SIZE );
    std::vector<int> mirror;
    populate( mirror, SIZE );

    CPPUNIT_ASSERT_MESSAGE( "Returned false for valid element",
                            array.contains( mirror[ 99 ] ) );
    CPPUNIT_ASSERT_MESSAGE( "Returned false for equal element",
                            array.contains( 8 ) );
    CPPUNIT_ASSERT_MESSAGE( "Returned true for invalid element",
                            !array.contains( 9999 ) );
}

////////////////////////////////////////////////////////////////////////////////
void ArrayListTest::testEnsureCapacity() {

    int capacity = 20;
    int testValue = 2048;
    ArrayList<int> array( capacity );
    int i;

    for( i = 0; i < capacity / 2; i++ ) {
        array.add( i, i + 44 );
    }

    array.add( i, testValue );
    int location = array.indexOf( testValue );
    array.ensureCapacity( capacity );
    CPPUNIT_ASSERT_MESSAGE( "EnsureCapacity moved objects around in array1.",
                            location == array.indexOf( testValue ) );
    array.removeAt( 0 );
    array.ensureCapacity( capacity );
    CPPUNIT_ASSERT_MESSAGE( "EnsureCapacity moved objects around in array2.",
                            --location == array.indexOf(testValue));
    array.ensureCapacity( capacity + 2 );
    CPPUNIT_ASSERT_MESSAGE( "EnsureCapacity did not change location.",
                            location == array.indexOf(testValue));

    ArrayList<std::string> list( 1 );
    list.add( "hello" );
    list.ensureCapacity( Integer::MIN_VALUE );
}

////////////////////////////////////////////////////////////////////////////////
void ArrayListTest::testGet() {

    ArrayList<int> array;
    populate( array, SIZE );
    std::vector<int> mirror;
    populate( mirror, SIZE );

    CPPUNIT_ASSERT_MESSAGE( "Returned incorrect element", array.get(22) == mirror[22] );
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IndexOutOfBoundsException",
        array.get( 9999 ),
        IndexOutOfBoundsException );
}

////////////////////////////////////////////////////////////////////////////////
void ArrayListTest::testIndexOf() {

    ArrayList<int> array;
    populate( array, SIZE );
    std::vector<int> mirror;
    populate( mirror, SIZE );

    CPPUNIT_ASSERT_EQUAL_MESSAGE( "Returned incorrect index", 87, array.indexOf( mirror[87] ) );
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "Returned index for invalid Object", -1, array.indexOf( SIZE + 10 ) );
}

////////////////////////////////////////////////////////////////////////////////
void ArrayListTest::testIsEmpty() {

    ArrayList<int> array;
    populate( array, SIZE );

    CPPUNIT_ASSERT_MESSAGE( "isEmpty returned false for new list", ArrayList<int>( 10 ).isEmpty() );
    CPPUNIT_ASSERT_MESSAGE("Returned true for existing list with elements", !array.isEmpty() );
}

////////////////////////////////////////////////////////////////////////////////
void ArrayListTest::testLastIndexOf() {

    ArrayList<int> array;
    populate( array, SIZE );

    array.add( 99 );

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Returned incorrect index", SIZE, array.lastIndexOf( 99 ) );
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Returned index for invalid Object", -1, array.lastIndexOf( 2048 ) );

    array.trimToSize();

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Returned incorrect index", SIZE, array.lastIndexOf( 99 ) );
}

//////////////////////////////////////////////////////////////////////////////
void ArrayListTest::testRemove() {

    ArrayList<int> list1;
    populate( list1, SIZE );
    ArrayList<int> list2;
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
void ArrayListTest::testRemoveAt() {

    ArrayList<int> array;
    populate( array, SIZE );

    array.removeAt( 10 );
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Failed to remove element", -1, array.indexOf( 10 ) );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IndexOutOfBoundsException",
        array.removeAt( 9999 ),
        IndexOutOfBoundsException );

    ArrayList<int> myArray( array );
    array.add( 25, 9999 );
    array.add( 50, 9999 );
    array.removeAt( 50 );
    array.removeAt( 25 );
    CPPUNIT_ASSERT_MESSAGE("Removing index did not work", array.equals( myArray ) );

    std::string data[] = { "a", "b", "c", "d", "e", "f", "g" };
    ArrayList<std::string> list;
    for( int i = 0; i < 7; ++i ) {
        list.add( data[i] );
    }

    CPPUNIT_ASSERT_MESSAGE( "Removed wrong element 1", list.removeAt(0) == "a" );
    CPPUNIT_ASSERT_MESSAGE( "Removed wrong element 2", list.removeAt(4) == "f" );

    ArrayList<int> l( 0 );
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
void ArrayListTest::testSet() {

    ArrayList<int> array;
    populate( array, SIZE );

    array.set( 65, 42 );

    CPPUNIT_ASSERT_MESSAGE( "Failed to set object", array.get( 65 ) == 42 );
    array.set( 50, 0 );
    CPPUNIT_ASSERT_MESSAGE( "Setting to null did not work", 0 == array.get( 50 ) );
    CPPUNIT_ASSERT_MESSAGE( std::string("") + "Setting increased the list's size to: " +
                            Integer::toString( array.size() ), array.size() == SIZE );

    array.set( 0, 1 );
    CPPUNIT_ASSERT_MESSAGE( "Failed to set object", array.get( 0 ) == 1 );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IndexOutOfBoundsException",
        array.set( -1, 10 ),
        IndexOutOfBoundsException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IndexOutOfBoundsException",
        array.set( array.size(), 10 ),
        IndexOutOfBoundsException );
}

////////////////////////////////////////////////////////////////////////////////
void ArrayListTest::testSize() {

    ArrayList<int> array;
    populate( array, SIZE );

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Returned incorrect size for exiting list", SIZE, array.size() );
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Returned incorrect size for new list", 0, ArrayList<int>().size());
}

////////////////////////////////////////////////////////////////////////////////
void ArrayListTest::testToString() {

    ArrayList<int> l(1);
    l.add( 5 );
    std::string result = l.toString();
    CPPUNIT_ASSERT_MESSAGE( "should produce a non-empty string", !result.empty() );
}

////////////////////////////////////////////////////////////////////////////////
void ArrayListTest::testToArray() {

    ArrayList<int> array;
    populate( array, SIZE );

    array.set( 25, 0 );
    array.set( 75, 0 );

    std::vector<int> stlvec = array.toArray();
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "Returned array of incorrect size", SIZE, (int)stlvec.size() );

    for( int i = 0; i < (int)stlvec.size(); i++ ) {
        if( ( i == 25 ) || ( i == 75 ) ) {
            CPPUNIT_ASSERT_EQUAL_MESSAGE( "Should be zero but instead got: ", 0, stlvec[i] );
        } else {
            CPPUNIT_ASSERT_EQUAL_MESSAGE( "Returned incorrect array: ", array.get(i), stlvec[i] );
        }
    }
}

//////////////////////////////////////////////////////////////////////////////
void ArrayListTest::testTrimToSize() {

    ArrayList<int> array;
    populate( array, SIZE );
    std::vector<int> mirror;
    populate( mirror, SIZE );

    for( int i = SIZE - 1; i > 24; i-- ) {
        array.removeAt( i );
    }

    array.trimToSize();

    CPPUNIT_ASSERT_EQUAL_MESSAGE( "Returned incorrect size after trim", 25, array.size() );
    for( int i = 0; i < array.size(); i++ ) {
        CPPUNIT_ASSERT_MESSAGE( "Trimmed list contained incorrect elements", array.get(i) == mirror[i] );
    }

    StlList<std::string> list;
    list.add( "a" );

    ArrayList<std::string> strArray( list );
    std::auto_ptr< Iterator<int> > iter( array.iterator() );
    array.trimToSize();

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an ConcurrentModificationException",
        iter->next(),
        ConcurrentModificationException );
}

////////////////////////////////////////////////////////////////////////////////
void ArrayListTest::testOverrideSize() {

    MockArrayList<std::string> testlist;

    // though size is overriden, it should passed without exception
    testlist.add( "test_0" );
    testlist.add( "test_1" );
    testlist.add( "test_2" );
    testlist.add( 1, "test_3" );
    testlist.get( 1 );
    testlist.removeAt( 2 );
    testlist.set( 1, "test_4" );
}

//////////////////////////////////////////////////////////////////////////////
void ArrayListTest::testRemoveAll() {

    ArrayList<int> list;
    populate( list, 3 );

    StlList<int> collection;
    collection.add( 1 );
    collection.add( 2 );

    list.removeAll( collection );

    CPPUNIT_ASSERT_EQUAL( 1, list.size() );

    ArrayList<int> list2;
    list2.removeAll( collection );
    CPPUNIT_ASSERT_EQUAL( 0, list2.size() );

    ArrayList<int> list3;
    populate( list3, 3 );
    collection.clear();

    list3.removeAll( collection );
    CPPUNIT_ASSERT_EQUAL( 3, list3.size() );
}

//////////////////////////////////////////////////////////////////////////////
void ArrayListTest::testRetainAll() {

    ArrayList<int> list;
    populate( list, 3 );

    StlList<int> collection;
    collection.add( 1 );
    collection.add( 2 );

    list.retainAll( collection );

    CPPUNIT_ASSERT_EQUAL( 2, list.size() );

    ArrayList<int> list2;
    list2.retainAll( collection );
    CPPUNIT_ASSERT_EQUAL( 0, list2.size() );

    ArrayList<int> list3;
    populate( list3, 3 );
    collection.clear();

    list3.retainAll( collection );
    CPPUNIT_ASSERT_EQUAL( 0, list3.size() );
}

//////////////////////////////////////////////////////////////////////////////
void ArrayListTest::testListIterator1IndexOutOfBoundsException() {

    ArrayList<int> list;

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IndexOutOfBoundsException",
        std::auto_ptr< ListIterator<int> > it( list.listIterator( -1 ) ),
        IndexOutOfBoundsException );
}

//////////////////////////////////////////////////////////////////////////////
void ArrayListTest::testListIterator2IndexOutOfBoundsException() {

    ArrayList<int> list;
    list.add( 1 );
    list.add( 2 );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IndexOutOfBoundsException",
        std::auto_ptr< ListIterator<int> > it( list.listIterator( 100 ) ),
        IndexOutOfBoundsException );
}
