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

#include "PrimitiveMapMarshallerTest.h"

#include <activemq/util/PrimitiveMap.h>
#include <activemq/util/PrimitiveList.h>
#include <activemq/connector/openwire/marshal/PrimitiveMapMarshaller.h>

using namespace std;
using namespace activemq;
using namespace activemq::util;
using namespace decaf::io;
using namespace decaf::lang::exceptions;
using namespace activemq::connector;
using namespace activemq::connector::openwire;
using namespace activemq::connector::openwire::marshal;

////////////////////////////////////////////////////////////////////////////////
void PrimitiveMapMarshallerTest::test() {

    PrimitiveMap myMap;

    unsigned char byteValue = 'A';
    char charValue = 'B';
    bool booleanValue = true;
    short shortValue = 2048;
    int intValue = 655369;
    long long longValue = 0xFFFFFFFF00000000ULL;
    float floatValue = 45.6545f;
    double doubleValue = 654564.654654;
    std::string stringValue = "The test string";

    myMap.setString( "stringKey", stringValue );
    myMap.setBool( "boolKey", booleanValue );
    myMap.setByte( "byteKey", byteValue );
    myMap.setChar( "charKey", charValue );
    myMap.setShort( "shortKey", shortValue );
    myMap.setInt( "intKey", intValue );
    myMap.setLong( "longKey", longValue );
    myMap.setFloat( "floatKey", floatValue );
    myMap.setDouble( "doubleKey", doubleValue );

    std::vector<unsigned char> bytes;
    bytes.push_back( 65 );
    bytes.push_back( 66 );
    bytes.push_back( 67 );
    bytes.push_back( 68 );
    bytes.push_back( 69 );
    myMap.setByteArray( "bytesKey", bytes );

    std::vector<unsigned char> marshaled;

    // Turn it into some bytes
    PrimitiveMapMarshaller::marshal( &myMap, marshaled );

    try {
        this->unmarshaledMap = PrimitiveMapMarshaller::unmarshal( marshaled );
    } catch(...) {
        CPPUNIT_ASSERT( false );
    }

    CPPUNIT_ASSERT( this->unmarshaledMap != NULL );

    CPPUNIT_ASSERT( this->unmarshaledMap->getString( "stringKey" ) == stringValue );
    CPPUNIT_ASSERT( this->unmarshaledMap->getBool( "boolKey" ) == booleanValue );
    CPPUNIT_ASSERT( this->unmarshaledMap->getByte( "byteKey" ) == byteValue );
    CPPUNIT_ASSERT( this->unmarshaledMap->getChar( "charKey" ) == charValue );
    CPPUNIT_ASSERT( this->unmarshaledMap->getShort( "shortKey" ) == shortValue );
    CPPUNIT_ASSERT( this->unmarshaledMap->getInt( "intKey" ) == intValue );
    CPPUNIT_ASSERT( this->unmarshaledMap->getLong( "longKey" ) == longValue );
    CPPUNIT_ASSERT( this->unmarshaledMap->getFloat( "floatKey" ) == floatValue );
    CPPUNIT_ASSERT( this->unmarshaledMap->getDouble( "doubleKey" ) == doubleValue );
    CPPUNIT_ASSERT( this->unmarshaledMap->getByteArray( "bytesKey" ) == bytes );
}

////////////////////////////////////////////////////////////////////////////////
void PrimitiveMapMarshallerTest::testLists() {

    PrimitiveMap myMap;

    PrimitiveList list1;
    PrimitiveList list2;
    PrimitiveList list3;

    list1.add( 1 );
    list2.add( 2 );
    list3.add( 3 );

    myMap.setValue( "1", list1 );
    myMap.setValue( "2", list2 );
    myMap.setValue( "3", list3 );

    std::vector<unsigned char> marshaled;

    // Turn it into some bytes
    PrimitiveMapMarshaller::marshal( &myMap, marshaled );

    // Try and get it back from those bytes.
    PrimitiveMap* newMap = NULL;

    try {
        newMap = PrimitiveMapMarshaller::unmarshal( marshaled );
    } catch(...) {
        CPPUNIT_ASSERT( false );
    }

    CPPUNIT_ASSERT( newMap != NULL );
    CPPUNIT_ASSERT( newMap->size() == 3 );

    delete newMap;
}

////////////////////////////////////////////////////////////////////////////////
void PrimitiveMapMarshallerTest::testMaps() {

    PrimitiveMap myMap;

    PrimitiveMap map1;
    PrimitiveMap map2;
    PrimitiveMap map3;

    map1.setValue( "1", 1 );
    map2.setValue( "2", 2 );
    map3.setValue( "3", 3 );

    myMap.setValue( "1", map1 );
    myMap.setValue( "2", map2 );
    myMap.setValue( "3", map3 );

    std::vector<unsigned char> marshaled;

    // Turn it into some bytes
    PrimitiveMapMarshaller::marshal( &myMap, marshaled );

    // Try and get it back from those bytes.
    PrimitiveMap* newMap = NULL;

    try {
        newMap = PrimitiveMapMarshaller::unmarshal( marshaled );
    } catch(...) {
        CPPUNIT_ASSERT( false );
    }

    CPPUNIT_ASSERT( newMap != NULL );
    CPPUNIT_ASSERT( newMap->size() == 3 );

    delete newMap;
}
