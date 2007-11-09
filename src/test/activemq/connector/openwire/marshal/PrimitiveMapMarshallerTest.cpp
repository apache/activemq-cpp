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

CPPUNIT_TEST_SUITE_REGISTRATION( activemq::connector::openwire::marshal::PrimitiveMapMarshallerTest );

#include <activemq/util/PrimitiveMap.h>
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
void PrimitiveMapMarshallerTest::test()
{
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

    // Try and get it back from those bytes.
    PrimitiveMap* newMap = NULL;

    try {
        newMap = PrimitiveMapMarshaller::unmarshal( marshaled );
    } catch(...) {
        CPPUNIT_ASSERT( false );
    }

    CPPUNIT_ASSERT( newMap != NULL );

    CPPUNIT_ASSERT( myMap.getString( "stringKey" ) == stringValue );
    CPPUNIT_ASSERT( myMap.getBool( "boolKey" ) == booleanValue );
    CPPUNIT_ASSERT( myMap.getByte( "byteKey" ) == byteValue );
    CPPUNIT_ASSERT( myMap.getChar( "charKey" ) == charValue );
    CPPUNIT_ASSERT( myMap.getShort( "shortKey" ) == shortValue );
    CPPUNIT_ASSERT( myMap.getInt( "intKey" ) == intValue );
    CPPUNIT_ASSERT( myMap.getLong( "longKey" ) == longValue );
    CPPUNIT_ASSERT( myMap.getFloat( "floatKey" ) == floatValue );
    CPPUNIT_ASSERT( myMap.getDouble( "doubleKey" ) == doubleValue );
    CPPUNIT_ASSERT( myMap.getByteArray( "bytesKey" ) == bytes );

    delete newMap;
}
