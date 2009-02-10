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

#include "MapTest.h"
#include <string>
#include <decaf/util/STLMap.h>

using namespace std;
using namespace decaf;
using namespace decaf::util;

////////////////////////////////////////////////////////////////////////////////
MapTest::MapTest(){
}

////////////////////////////////////////////////////////////////////////////////
MapTest::~MapTest(){
}

////////////////////////////////////////////////////////////////////////////////
void MapTest::testContainsKey(){

    STLMap<string, bool> boolMap;
    CPPUNIT_ASSERT(boolMap.containsKey("bob") == false);

    boolMap.put( "bob", true );

    CPPUNIT_ASSERT(boolMap.containsKey("bob") == true );
    CPPUNIT_ASSERT(boolMap.containsKey("fred") == false );
}

////////////////////////////////////////////////////////////////////////////////
void MapTest::testClear(){

    STLMap<string, bool> boolMap;
    boolMap.put( "bob", true );
    boolMap.put( "fred", true );

    CPPUNIT_ASSERT(boolMap.size() == 2 );
    boolMap.clear();
    CPPUNIT_ASSERT(boolMap.size() == 0 );
}

////////////////////////////////////////////////////////////////////////////////
void MapTest::testIsEmpty(){

    STLMap<string, bool> boolMap;
    boolMap.put( "bob", true );
    boolMap.put( "fred", true );

    CPPUNIT_ASSERT(boolMap.isEmpty() == false );
    boolMap.clear();
    CPPUNIT_ASSERT(boolMap.isEmpty() == true );
}

////////////////////////////////////////////////////////////////////////////////
void MapTest::testSize(){

    STLMap<string, bool> boolMap;

    CPPUNIT_ASSERT(boolMap.size() == 0 );
    boolMap.put( "bob", true );
    CPPUNIT_ASSERT(boolMap.size() == 1 );
    boolMap.put( "fred", true );
    CPPUNIT_ASSERT(boolMap.size() == 2 );
}

////////////////////////////////////////////////////////////////////////////////
void MapTest::testValue(){

    STLMap<string, bool> boolMap;

    boolMap.put( "fred", true );
    CPPUNIT_ASSERT( boolMap.get("fred") == true );

    boolMap.put( "bob", false );
    CPPUNIT_ASSERT( boolMap.get("bob") == false );
    CPPUNIT_ASSERT( boolMap.get("fred") == true );

    try{
        boolMap.get( "mike" );
        CPPUNIT_ASSERT(false);
    } catch( decaf::lang::exceptions::NoSuchElementException& e ){
    }
}

////////////////////////////////////////////////////////////////////////////////
void MapTest::testRemove(){
    STLMap<string, bool> boolMap;

    boolMap.put( "fred", true );
    CPPUNIT_ASSERT( boolMap.containsKey("fred") == true );
    boolMap.remove( "fred" );
    CPPUNIT_ASSERT( boolMap.containsKey("fred") == false );
}

////////////////////////////////////////////////////////////////////////////////
void MapTest::testContiansValue(){
    STLMap<string, bool> boolMap;

    boolMap.put( "fred", true );
    boolMap.put( "fred1", false );
    CPPUNIT_ASSERT( boolMap.containsValue(true) == true );
    boolMap.remove( "fred" );
    CPPUNIT_ASSERT( boolMap.containsValue(true) == false );
}
