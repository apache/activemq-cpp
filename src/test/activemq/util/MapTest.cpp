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

CPPUNIT_TEST_SUITE_REGISTRATION( activemq::util::MapTest );

using namespace activemq::util;
using namespace std;

MapTest::MapTest()
{
}

MapTest::~MapTest()
{
}

void MapTest::testContains(){
    
    Map<string, bool> boolMap;
    CPPUNIT_ASSERT(boolMap.contains("bob") == false);
    
    boolMap.setValue( "bob", true );
    
    CPPUNIT_ASSERT(boolMap.contains("bob") == true );
    CPPUNIT_ASSERT(boolMap.contains("fred") == false );
}

void MapTest::testClear(){
    
    Map<string, bool> boolMap;
    boolMap.setValue( "bob", true );
    boolMap.setValue( "fred", true );
    
    CPPUNIT_ASSERT(boolMap.count() == 2 );
    
    boolMap.clear();
    
    CPPUNIT_ASSERT(boolMap.count() == 0 );
}

void MapTest::testCount(){
    
    Map<string, bool> boolMap;
    
    CPPUNIT_ASSERT(boolMap.count() == 0 );
    
    boolMap.setValue( "bob", true );
    
    CPPUNIT_ASSERT(boolMap.count() == 1 );
    
    boolMap.setValue( "fred", true );
    
    CPPUNIT_ASSERT(boolMap.count() == 2 );
}

void MapTest::testValue(){
    
    Map<string, bool> boolMap;
    
    boolMap.setValue( "fred", true );    
    CPPUNIT_ASSERT( boolMap.getValue("fred") == true );
    
    boolMap.setValue( "bob", false );
    CPPUNIT_ASSERT( boolMap.getValue("bob") == false );
    CPPUNIT_ASSERT( boolMap.getValue("fred") == true );
    
    try{
        boolMap.getValue( "mike" );
        CPPUNIT_ASSERT(false);
    } catch( activemq::exceptions::NoSuchElementException& e ){        
    }
}

void MapTest::testRemove(){
    Map<string, bool> boolMap;
    
    boolMap.setValue( "fred", true );    
    CPPUNIT_ASSERT( boolMap.contains("fred") == true );
    boolMap.remove( "fred" );
    CPPUNIT_ASSERT( boolMap.contains("fred") == false );    
}

