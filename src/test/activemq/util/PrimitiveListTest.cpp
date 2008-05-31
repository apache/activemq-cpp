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

#include "PrimitiveListTest.h"

#include <activemq/util/PrimitiveValueNode.h>
#include <activemq/util/PrimitiveMap.h>

using namespace activemq;
using namespace activemq::util;

void PrimitiveListTest::testSetGet(){

    PrimitiveList plist;

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should Throw IndexOutOfBoundsException",
        plist.getBool( 0 ),
        decaf::lang::exceptions::IndexOutOfBoundsException );

    plist.add( true );
    CPPUNIT_ASSERT( plist.getBool(0) == true );
    plist.add( false );
    CPPUNIT_ASSERT( plist.getBool(1) == false );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should Throw NoSuchElementException",
        plist.getByte( 0 ),
        decaf::lang::exceptions::NoSuchElementException );

    plist.setByte( 0, 1 );
    CPPUNIT_ASSERT( plist.getByte(0) == 1 );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should Throw NoSuchElementException",
        plist.getChar( 0 ),
        decaf::lang::exceptions::NoSuchElementException );

    plist.setChar( 0, 'a' );
    CPPUNIT_ASSERT( plist.getChar(0) == 'a' );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should Throw NoSuchElementException",
        plist.getShort( 0 ),
        decaf::lang::exceptions::NoSuchElementException );

    plist.setShort( 0, 2 );
    CPPUNIT_ASSERT( plist.getShort(0) == 2 );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should Throw NoSuchElementException",
        plist.getInt( 0 ),
        decaf::lang::exceptions::NoSuchElementException );

    plist.setInt( 0, 3 );
    CPPUNIT_ASSERT( plist.getInt(0) == 3 );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should Throw NoSuchElementException",
        plist.getLong( 0 ),
        decaf::lang::exceptions::NoSuchElementException );

    plist.setLong( 0, 4L );
    CPPUNIT_ASSERT( plist.getLong(0) == 4L );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should Throw NoSuchElementException",
        plist.getDouble( 0 ),
        decaf::lang::exceptions::NoSuchElementException );

    plist.setDouble( 0, 2.3 );
    CPPUNIT_ASSERT( plist.getDouble(0) == 2.3 );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should Throw NoSuchElementException",
        plist.getFloat( 0 ),
        decaf::lang::exceptions::NoSuchElementException );

    plist.setFloat( 0, 3.2f );
    CPPUNIT_ASSERT( plist.getFloat(0) == 3.2f );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should Throw NoSuchElementException",
        plist.getString( 0 ),
        decaf::lang::exceptions::NoSuchElementException );

    plist.setString( 0, "hello" );
    CPPUNIT_ASSERT( plist.getString(0) == "hello" );

    std::vector<unsigned char> byteArray;
    byteArray.push_back( 'a' );
    byteArray.push_back( 'b' );
    byteArray.push_back( 'c' );
    byteArray.push_back( 'd' );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should Throw NoSuchElementException",
        plist.getByteArray( 0 ),
        decaf::lang::exceptions::NoSuchElementException );

    plist.setByteArray( 0, byteArray );
    CPPUNIT_ASSERT( plist.getByteArray(0) == byteArray );
}

void PrimitiveListTest::testAdd(){

    bool boolValue = true;
    unsigned char byteValue = 65;
    char charValue = 'A';
    short shortValue = 32767;
    int intValue = 65540;
    long long longValue = 0xFFFFFFFFFFFFFFFFLL;
    float floatValue = 5.55f;
    double doubleValue = 687.021654;
    std::string stringValue = "TEST";
    std::vector<unsigned char> byteArrayValue;
    byteArrayValue.push_back( 'a' );
    byteArrayValue.push_back( 'b' );
    byteArrayValue.push_back( 'c' );
    byteArrayValue.push_back( 'd' );

    PrimitiveList plist;
    plist.add( boolValue );
    plist.add( byteValue );
    plist.add( charValue );
    plist.add( shortValue );
    plist.add( intValue );
    plist.add( longValue );
    plist.add( floatValue );
    plist.add( doubleValue );
    plist.add( stringValue );
    plist.add( byteArrayValue );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should Throw NoSuchElementException",
        plist.getInt( 0 ),
        decaf::lang::exceptions::NoSuchElementException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should Throw IndexOutOfBoundsException",
        plist.getInt( plist.size() ),
        decaf::lang::exceptions::IndexOutOfBoundsException );

    CPPUNIT_ASSERT( plist.get(0).getValueType() == PrimitiveValueNode::BOOLEAN_TYPE );
    CPPUNIT_ASSERT( plist.get(1).getValueType() == PrimitiveValueNode::BYTE_TYPE );
    CPPUNIT_ASSERT( plist.get(2).getValueType() == PrimitiveValueNode::CHAR_TYPE );
    CPPUNIT_ASSERT( plist.get(3).getValueType() == PrimitiveValueNode::SHORT_TYPE );
    CPPUNIT_ASSERT( plist.get(4).getValueType() == PrimitiveValueNode::INTEGER_TYPE );
    CPPUNIT_ASSERT( plist.get(5).getValueType() == PrimitiveValueNode::LONG_TYPE );
    CPPUNIT_ASSERT( plist.get(6).getValueType() == PrimitiveValueNode::FLOAT_TYPE );
    CPPUNIT_ASSERT( plist.get(7).getValueType() == PrimitiveValueNode::DOUBLE_TYPE );
    CPPUNIT_ASSERT( plist.get(8).getValueType() == PrimitiveValueNode::STRING_TYPE );
    CPPUNIT_ASSERT( plist.get(9).getValueType() == PrimitiveValueNode::BYTE_ARRAY_TYPE );
}

void PrimitiveListTest::testRemove(){

    PrimitiveList plist;
    plist.add( 5 );
    plist.add( 5.5f );
    plist.add( 6 );
    plist.remove( 0 );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should Throw NoSuchElementException",
        plist.getInt( 0 ),
        decaf::lang::exceptions::NoSuchElementException );

    plist.remove( 0 );
    plist.remove( 0 );

    CPPUNIT_ASSERT( plist.isEmpty() );
}

void PrimitiveListTest::testCount(){

    PrimitiveList plist;
    CPPUNIT_ASSERT( plist.size() == 0 );
    plist.add( 5 );
    CPPUNIT_ASSERT( plist.size() == 1);
    plist.add( 5.5f );
    CPPUNIT_ASSERT( plist.size() == 2 );
    plist.add( 6 );
    CPPUNIT_ASSERT( plist.size() == 3 );
    plist.remove( 0 );
    CPPUNIT_ASSERT( plist.size() == 2 );

    CPPUNIT_ASSERT( plist.toString() != "" );
}

void PrimitiveListTest::testCopy(){

    PrimitiveList plist;
    plist.add( 5 );
    plist.add( 5.5f );
    plist.add( 6 );

    PrimitiveList copy;
    copy.copy( plist );
    CPPUNIT_ASSERT( plist.equals( copy ) );

    PrimitiveList copy1( plist );
    CPPUNIT_ASSERT( plist.equals( copy1 ) );
}

void PrimitiveListTest::testClear(){

    PrimitiveList plist;
    plist.add( 5 );
    plist.add( 5.5f );
    plist.add( 6 );

    plist.clear();
    CPPUNIT_ASSERT( plist.size() == 0 );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should Throw IndexOutOfBoundsException",
        plist.getInt( 0 ),
        decaf::lang::exceptions::IndexOutOfBoundsException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should Throw IndexOutOfBoundsException",
        plist.getFloat( 1 ),
        decaf::lang::exceptions::IndexOutOfBoundsException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should Throw IndexOutOfBoundsException",
        plist.getInt( 2 ),
        decaf::lang::exceptions::IndexOutOfBoundsException );
}

void PrimitiveListTest::testContains(){

    PrimitiveList plist;

    CPPUNIT_ASSERT( plist.contains( 255 ) == false );

    plist.add( 5 );
    CPPUNIT_ASSERT( plist.contains( 5 ) == true );

    plist.add( 5.5f );
    CPPUNIT_ASSERT( plist.contains( 5.5f ) == true );

    plist.add( 6 );
    CPPUNIT_ASSERT( plist.contains( 6 ) == true );

    plist.remove( PrimitiveValueNode(5) );
    CPPUNIT_ASSERT( plist.contains( 5 ) == false );
}

void PrimitiveListTest::testListOfLists() {

    PrimitiveList list;
    PrimitiveList sublist1;
    PrimitiveList sublist2;
    PrimitiveList sublist3;

    sublist1.add( 1 );
    sublist2.add( 2 );
    sublist3.add( 3 );

    list.add( sublist1 );
    list.add( sublist2 );
    list.add( sublist3 );

    CPPUNIT_ASSERT( list.get(0).getList().get(0).getInt() == 1 );
    CPPUNIT_ASSERT( list.get(1).getList().get(0).getInt() == 2 );
    CPPUNIT_ASSERT( list.get(2).getList().get(0).getInt() == 3 );

}

void PrimitiveListTest::testListOfMaps() {

    PrimitiveList list;
    PrimitiveMap map1;
    PrimitiveMap map2;
    PrimitiveMap map3;

    map1.setInt( "1", 1 );
    map2.setInt( "2", 2 );
    map3.setInt( "3", 3 );

    list.add( map1 );
    list.add( map2 );
    list.add( map3 );

    CPPUNIT_ASSERT( list.get(0).getMap().getValue("1").getInt() == 1 );
    CPPUNIT_ASSERT( list.get(1).getMap().getValue("2").getInt() == 2 );
    CPPUNIT_ASSERT( list.get(2).getMap().getValue("3").getInt() == 3 );

}
