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

using namespace std;
using namespace decaf;
using namespace decaf::util;

////////////////////////////////////////////////////////////////////////////////
ListTest::ListTest(){
}

////////////////////////////////////////////////////////////////////////////////
void ListTest::testContains(){

    List<string> list;
    CPPUNIT_ASSERT( list.contains( "bob" ) == false);

    list.add( "bob" );

    CPPUNIT_ASSERT(list.contains( "bob" ) == true );
    CPPUNIT_ASSERT(list.contains( "fred" ) == false );
}

////////////////////////////////////////////////////////////////////////////////
void ListTest::testIndexOf(){

    List<string> list;

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

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an NoSuchElementException",
        list.indexOf( "fred" ),
        decaf::lang::exceptions::NoSuchElementException );

    CPPUNIT_ASSERT( list.indexOf( "george" ) == 1 );
    CPPUNIT_ASSERT( list.indexOf( "steve" ) == 2 );
}

////////////////////////////////////////////////////////////////////////////////
void ListTest::testLastIndexOf(){

    List<string> list;

    list.add( "bob" );    // 0
    list.add( "fred" );   // 1
    list.add( "george" ); // 2
    list.add( "bob" );    // 3

    CPPUNIT_ASSERT( list.indexOf( "bob" ) == 0 );
    CPPUNIT_ASSERT( list.lastIndexOf( "bob" ) == 3 );

    list.remove( "fred" );

    CPPUNIT_ASSERT( list.indexOf( "bob" ) == 0 );
    CPPUNIT_ASSERT( list.lastIndexOf( "bob" ) == 2 );

    list.remove( "bob" );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an NoSuchElementException",
        list.lastIndexOf( "bob" ),
        decaf::lang::exceptions::NoSuchElementException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an NoSuchElementException",
        list.indexOf( "bob" ),
        decaf::lang::exceptions::NoSuchElementException );
}

////////////////////////////////////////////////////////////////////////////////
void ListTest::testClear(){

    List<string> list;
    list.add( "bob" );
    list.add( "fred" );

    CPPUNIT_ASSERT( list.size() == 2 );
    list.clear();
    CPPUNIT_ASSERT( list.size() == 0 );
}

////////////////////////////////////////////////////////////////////////////////
void ListTest::testIsEmpty(){

    List<string> list;
    list.add( "bob" );
    list.add( "fred" );

    CPPUNIT_ASSERT(list.isEmpty() == false );
    list.clear();
    CPPUNIT_ASSERT(list.isEmpty() == true );
}

////////////////////////////////////////////////////////////////////////////////
void ListTest::testSize(){

    List<string> list;

    CPPUNIT_ASSERT( list.size() == 0 );
    list.add( "bob" );
    CPPUNIT_ASSERT( list.size() == 1 );
    list.add( "fred" );
    CPPUNIT_ASSERT( list.size() == 2 );
}

////////////////////////////////////////////////////////////////////////////////
void ListTest::testGet(){
    List<string> list;

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
    List<string> list;

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
    List<string> list;

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
    List<string> list;

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
void ListTest::testRemove(){
    List<string> list;

    list.add( "fred" );
    CPPUNIT_ASSERT( list.contains( "fred" ) == true );
    list.remove( "fred" );
    CPPUNIT_ASSERT( list.contains( "fred" ) == false );
}

////////////////////////////////////////////////////////////////////////////////
void ListTest::testRemove2(){
    List<string> list;

    list.add( "fred" );
    list.add( "bob" );
    list.add( "steve" );
    list.add( "mike" );
    list.add( "larry" );

    CPPUNIT_ASSERT( list.remove(0) == "fred" );
    CPPUNIT_ASSERT( list.get(0) == "bob" );

    CPPUNIT_ASSERT( list.remove(2) == "mike" );
    CPPUNIT_ASSERT( list.get(2) == "larry" );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IndexOutOfBoundsException",
        list.remove( list.size() + 1 ),
        decaf::lang::exceptions::IndexOutOfBoundsException );
}

////////////////////////////////////////////////////////////////////////////////
void ListTest::testToArray(){

    List<string> list;

    list.add( "fred1" );
    list.add( "fred2" );
    list.add( "fred3" );
    CPPUNIT_ASSERT( list.size() == 3 );

    std::vector<std::string> array = list.toArray();

    CPPUNIT_ASSERT( array.size() == 3 );
}

////////////////////////////////////////////////////////////////////////////////
void ListTest::testIterator(){

    List<string> list;

    list.add( "fred1" );
    list.add( "fred2" );
    list.add( "fred3" );

    Iterator<string>* iterator1 = list.iterator();
    CPPUNIT_ASSERT( iterator1 != NULL );
    CPPUNIT_ASSERT( iterator1->hasNext() == true );

    size_t count = 0;
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
