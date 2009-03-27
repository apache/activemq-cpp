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

#include "PrimitiveValueNodeTest.h"

#include <activemq/util/PrimitiveValueNode.h>

using namespace activemq;
using namespace activemq::util;

void PrimitiveValueNodeTest::testValueNode(){

    PrimitiveValueNode node;

    node.setBool( true );
    CPPUNIT_ASSERT( node.getValueType() == PrimitiveValueNode::BOOLEAN_TYPE );
    CPPUNIT_ASSERT( node.getBool() == true );
    node.setBool( false );
    CPPUNIT_ASSERT( node.getValueType() == PrimitiveValueNode::BOOLEAN_TYPE );
    CPPUNIT_ASSERT( node.getBool() == false );

    node.setByte( 5 );
    CPPUNIT_ASSERT( node.getValueType() == PrimitiveValueNode::BYTE_TYPE );
    CPPUNIT_ASSERT( node.getByte() == 5 );

    node.setChar( 'a' );
    CPPUNIT_ASSERT( node.getValueType() == PrimitiveValueNode::CHAR_TYPE );
    CPPUNIT_ASSERT( node.getChar() == 'a' );

    node.setShort( 10 );
    CPPUNIT_ASSERT( node.getValueType() == PrimitiveValueNode::SHORT_TYPE );
    CPPUNIT_ASSERT( node.getShort() == 10 );

    node.setInt( 10000 );
    CPPUNIT_ASSERT( node.getValueType() == PrimitiveValueNode::INTEGER_TYPE );
    CPPUNIT_ASSERT( node.getInt() == 10000 );

    node.setLong( 100000L );
    CPPUNIT_ASSERT( node.getValueType() == PrimitiveValueNode::LONG_TYPE );
    CPPUNIT_ASSERT( node.getLong() == 100000L );

    node.setDouble( 2.3 );
    CPPUNIT_ASSERT( node.getValueType() == PrimitiveValueNode::DOUBLE_TYPE );
    CPPUNIT_ASSERT( node.getDouble() == 2.3 );

    node.setFloat( 3.2f );
    CPPUNIT_ASSERT( node.getValueType() == PrimitiveValueNode::FLOAT_TYPE );
    CPPUNIT_ASSERT( node.getFloat() == 3.2f );

    node.setString( "hello" );
    CPPUNIT_ASSERT( node.getValueType() == PrimitiveValueNode::STRING_TYPE );
    CPPUNIT_ASSERT( node.getString() == "hello" );

    std::vector<unsigned char> byteArray;
    byteArray.push_back( 'a' );
    byteArray.push_back( 'b' );
    byteArray.push_back( 'c' );
    byteArray.push_back( 'd' );

    node.setByteArray( byteArray );
    CPPUNIT_ASSERT( node.getValueType() == PrimitiveValueNode::BYTE_ARRAY_TYPE );
    CPPUNIT_ASSERT( node.getByteArray() == byteArray );

    try{
        node.getFloat();
        CPPUNIT_ASSERT( false );
    } catch( decaf::lang::exceptions::NoSuchElementException& e ){
    }

    node.clear();
    CPPUNIT_ASSERT( node.getValueType() == PrimitiveValueNode::NULL_TYPE );
}

void PrimitiveValueNodeTest::testValueNodeCtors(){

    PrimitiveValueNode tfvalue = true;
    PrimitiveValueNode bvalue = (unsigned char)60;
    PrimitiveValueNode cvalue = (char)60;
    PrimitiveValueNode svalue = (short)32767;
    PrimitiveValueNode ivalue = (int)4096;
    PrimitiveValueNode lvalue = 555666777888LL;
    PrimitiveValueNode fvalue = 0.125f;
    PrimitiveValueNode dvalue = 10.056;
    PrimitiveValueNode strValue = "TEST";
    PrimitiveValueNode bArrayValue = std::vector<unsigned char>();

    CPPUNIT_ASSERT( tfvalue.getBool() == true );
    CPPUNIT_ASSERT( tfvalue.getValueType() == PrimitiveValueNode::BOOLEAN_TYPE );
    CPPUNIT_ASSERT( bvalue.getByte() == 60 );
    CPPUNIT_ASSERT( bvalue.getValueType() == PrimitiveValueNode::BYTE_TYPE );
    CPPUNIT_ASSERT( cvalue.getChar() == (char)60 );
    CPPUNIT_ASSERT( cvalue.getValueType() == PrimitiveValueNode::CHAR_TYPE );
    CPPUNIT_ASSERT( svalue.getShort() == 32767 );
    CPPUNIT_ASSERT( svalue.getValueType() == PrimitiveValueNode::SHORT_TYPE );
    CPPUNIT_ASSERT( ivalue.getInt() == 4096 );
    CPPUNIT_ASSERT( ivalue.getValueType() == PrimitiveValueNode::INTEGER_TYPE );
    CPPUNIT_ASSERT( lvalue.getLong() == 555666777888LL );
    CPPUNIT_ASSERT( lvalue.getValueType() == PrimitiveValueNode::LONG_TYPE );
    CPPUNIT_ASSERT( fvalue.getFloat() == 0.125f );
    CPPUNIT_ASSERT( fvalue.getValueType() == PrimitiveValueNode::FLOAT_TYPE );
    CPPUNIT_ASSERT( dvalue.getDouble() == 10.056 );
    CPPUNIT_ASSERT( dvalue.getValueType() == PrimitiveValueNode::DOUBLE_TYPE );
    CPPUNIT_ASSERT( strValue.getString() == "TEST" );
    CPPUNIT_ASSERT( strValue.getValueType() == PrimitiveValueNode::STRING_TYPE );
    CPPUNIT_ASSERT( bArrayValue.getValueType() == PrimitiveValueNode::BYTE_ARRAY_TYPE );
}
