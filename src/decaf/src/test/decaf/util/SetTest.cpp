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

using namespace std;
using namespace decaf;
using namespace decaf::util;

////////////////////////////////////////////////////////////////////////////////
SetTest::SetTest(){
}

////////////////////////////////////////////////////////////////////////////////
void SetTest::testContains(){

    Set<string> set;
    CPPUNIT_ASSERT( set.contains( "bob" ) == false);

    set.add( "bob" );

    CPPUNIT_ASSERT(set.contains( "bob" ) == true );
    CPPUNIT_ASSERT(set.contains( "fred" ) == false );
}

////////////////////////////////////////////////////////////////////////////////
void SetTest::testClear(){

    Set<string> set;
    set.add( "bob" );
    set.add( "fred" );

    CPPUNIT_ASSERT( set.size() == 2 );
    set.clear();
    CPPUNIT_ASSERT( set.size() == 0 );
}

////////////////////////////////////////////////////////////////////////////////
void SetTest::testIsEmpty(){

    Set<string> set;
    set.add( "bob" );
    set.add( "fred" );

    CPPUNIT_ASSERT(set.isEmpty() == false );
    set.clear();
    CPPUNIT_ASSERT(set.isEmpty() == true );
}

////////////////////////////////////////////////////////////////////////////////
void SetTest::testSize(){

    Set<string> set;

    CPPUNIT_ASSERT( set.size() == 0 );
    set.add( "bob" );
    CPPUNIT_ASSERT( set.size() == 1 );
    set.add( "fred" );
    CPPUNIT_ASSERT( set.size() == 2 );
}

////////////////////////////////////////////////////////////////////////////////
void SetTest::testAdd(){
    Set<string> set;

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
    Set<string> set;

    set.add( "fred" );
    CPPUNIT_ASSERT( set.contains( "fred" ) == true );
    set.remove( "fred" );
    CPPUNIT_ASSERT( set.contains( "fred" ) == false );
}

////////////////////////////////////////////////////////////////////////////////
void SetTest::testToArray(){

    Set<string> set;

    set.add( "fred1" );
    set.add( "fred2" );
    set.add( "fred3" );
    CPPUNIT_ASSERT( set.size() == 3 );

    std::vector<std::string> array = set.toArray();

    CPPUNIT_ASSERT( array.size() == 3 );
}

////////////////////////////////////////////////////////////////////////////////
void SetTest::testIterator(){

    Set<string> set;

    set.add( "fred1" );
    set.add( "fred2" );
    set.add( "fred3" );

    Iterator<string>* iterator1 = set.iterator();
    CPPUNIT_ASSERT( iterator1 != NULL );
    CPPUNIT_ASSERT( iterator1->hasNext() == true );

    size_t count = 0;
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
