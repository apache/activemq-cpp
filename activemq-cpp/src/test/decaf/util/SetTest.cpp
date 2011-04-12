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

#include "SetTest.h"

#include <decaf/util/StlSet.h>
#include <decaf/util/LinkedList.h>
#include <decaf/util/Iterator.h>

using namespace std;
using namespace decaf;
using namespace decaf::util;

////////////////////////////////////////////////////////////////////////////////
SetTest::SetTest(){
}

////////////////////////////////////////////////////////////////////////////////
void SetTest::testConstructor1(){

    StlSet<int> set;

    CPPUNIT_ASSERT( set.size() == 0 );
    CPPUNIT_ASSERT( set.isEmpty() );

    set.add( 1 );

    CPPUNIT_ASSERT( set.size() == 1 );
    CPPUNIT_ASSERT( !set.isEmpty() );

    set.add( 1 );

    CPPUNIT_ASSERT( set.size() == 1 );
    CPPUNIT_ASSERT( !set.isEmpty() );
}

////////////////////////////////////////////////////////////////////////////////
void SetTest::testConstructor2(){

    StlSet<int> set1;

    for( int i = 0; i < 50; ++i ) {
        set1.add( i );
    }

    StlSet<int> set2( set1 );

    CPPUNIT_ASSERT( set1.size() == set2.size() );

    for( int i = 0; i < 50; ++i ) {
        CPPUNIT_ASSERT( set2.contains( i ) );
    }

    CPPUNIT_ASSERT( set2.equals( set1 ) );
}

////////////////////////////////////////////////////////////////////////////////
void SetTest::testConstructor3(){

    LinkedList<int> collection;

    for( int i = 0; i < 50; ++i ) {
        collection.add( i );
    }

    StlSet<int> set( collection );

    CPPUNIT_ASSERT( collection.size() == set.size() );

    for( int i = 0; i < 50; ++i ) {
        CPPUNIT_ASSERT( set.contains( i ) );
    }

    CPPUNIT_ASSERT( set.equals( collection ) );
}

////////////////////////////////////////////////////////////////////////////////
void SetTest::testCopy1() {

    StlSet<int> set1;

    for( int i = 0; i < 50; ++i ) {
        set1.add( i );
    }

    StlSet<int> set2;

    set2.copy( set1 );

    CPPUNIT_ASSERT( set1.size() == set2.size() );

    for( int i = 0; i < 50; ++i ) {
        CPPUNIT_ASSERT( set2.contains( i ) );
    }

    CPPUNIT_ASSERT( set2.equals( set1 ) );
}

////////////////////////////////////////////////////////////////////////////////
void SetTest::testCopy2() {

    LinkedList<int> collection;

    for( int i = 0; i < 50; ++i ) {
        collection.add( i );
    }

    StlSet<int> set;

    set.copy( collection );

    CPPUNIT_ASSERT( collection.size() == set.size() );

    for( int i = 0; i < 50; ++i ) {
        CPPUNIT_ASSERT( set.contains( i ) );
    }

    CPPUNIT_ASSERT( set.equals( collection ) );
}

////////////////////////////////////////////////////////////////////////////////
void SetTest::testContains(){

    StlSet<string> set;
    CPPUNIT_ASSERT( set.contains( "bob" ) == false);

    set.add( "bob" );

    CPPUNIT_ASSERT( set.contains( "bob" ) == true );
    CPPUNIT_ASSERT( set.contains( "fred" ) == false );
}

////////////////////////////////////////////////////////////////////////////////
void SetTest::testClear(){

    StlSet<string> set;
    set.add( "bob" );
    set.add( "fred" );

    CPPUNIT_ASSERT( set.size() == 2 );
    set.clear();
    CPPUNIT_ASSERT( set.size() == 0 );
}

////////////////////////////////////////////////////////////////////////////////
void SetTest::testIsEmpty(){

    StlSet<string> set;
    set.add( "bob" );
    set.add( "fred" );

    CPPUNIT_ASSERT(set.isEmpty() == false );
    set.clear();
    CPPUNIT_ASSERT(set.isEmpty() == true );
}

////////////////////////////////////////////////////////////////////////////////
void SetTest::testSize(){

    StlSet<string> set;

    CPPUNIT_ASSERT( set.size() == 0 );
    set.add( "bob" );
    CPPUNIT_ASSERT( set.size() == 1 );
    set.add( "fred" );
    CPPUNIT_ASSERT( set.size() == 2 );
}

////////////////////////////////////////////////////////////////////////////////
void SetTest::testAdd(){
    StlSet<string> set;

    set.add( "fred" );
    set.add( "fred" );
    set.add( "fred" );
    CPPUNIT_ASSERT( set.contains("fred") == true );
    CPPUNIT_ASSERT( set.size() == 1 );
    set.remove( "fred" );
    CPPUNIT_ASSERT( set.contains("fred") == false );
    CPPUNIT_ASSERT( set.isEmpty() );
}

////////////////////////////////////////////////////////////////////////////////
void SetTest::testRemove(){
    StlSet<string> set;

    set.add( "fred" );
    CPPUNIT_ASSERT( set.contains( "fred" ) == true );
    set.remove( "fred" );
    CPPUNIT_ASSERT( set.contains( "fred" ) == false );
}

////////////////////////////////////////////////////////////////////////////////
void SetTest::testToArray(){

    StlSet<string> set;

    set.add( "fred1" );
    set.add( "fred2" );
    set.add( "fred3" );
    CPPUNIT_ASSERT( set.size() == 3 );

    std::vector<std::string> array = set.toArray();

    CPPUNIT_ASSERT( array.size() == 3 );
}

////////////////////////////////////////////////////////////////////////////////
void SetTest::testIterator(){

    StlSet<string> set;

    set.add( "fred1" );
    set.add( "fred2" );
    set.add( "fred3" );

    Iterator<string>* iterator1 = set.iterator();
    CPPUNIT_ASSERT( iterator1 != NULL );
    CPPUNIT_ASSERT( iterator1->hasNext() == true );

    int count = 0;
    while( iterator1->hasNext() ) {
        iterator1->next();
        ++count;
    }

    CPPUNIT_ASSERT( count == set.size() );

    Iterator<string>* iterator2 = set.iterator();

    while( iterator2->hasNext() ) {
        iterator2->next();
        iterator2->remove();
    }

    CPPUNIT_ASSERT( set.isEmpty() );

    delete iterator1;
    delete iterator2;
}
