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

#include "PrimitiveMapTest.h"

#include <activemq/util/PrimitiveValueNode.h>

using namespace activemq;
using namespace activemq::util;

void PrimitiveMapTest::testValueNode(){

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

void PrimitiveMapTest::testSetGet(){

    PrimitiveMap pmap;

    try{
        pmap.getBool( "bool" );
        CPPUNIT_ASSERT( false );
    } catch( decaf::lang::exceptions::NoSuchElementException& e ){}
    pmap.setBool( "bool", true );
    CPPUNIT_ASSERT( pmap.getBool("bool") == true );
    pmap.setBool( "bool", false );
    CPPUNIT_ASSERT( pmap.getBool("bool") == false );

    try{
        pmap.getByte( "byte" );
        CPPUNIT_ASSERT( false );
    } catch( decaf::lang::exceptions::NoSuchElementException& e ){}
    pmap.setByte( "byte", 1 );
    CPPUNIT_ASSERT( pmap.getByte("byte") == 1 );

    try{
        pmap.getChar( "char" );
        CPPUNIT_ASSERT( false );
    } catch( decaf::lang::exceptions::NoSuchElementException& e ){}
    pmap.setChar( "char", 'a' );
    CPPUNIT_ASSERT( pmap.getChar("char") == 'a' );

    try{
        pmap.getShort( "short" );
        CPPUNIT_ASSERT( false );
    } catch( decaf::lang::exceptions::NoSuchElementException& e ){}
    pmap.setShort( "short", 2 );
    CPPUNIT_ASSERT( pmap.getShort("short") == 2 );

    try{
        pmap.getInt( "int" );
        CPPUNIT_ASSERT( false );
    } catch( decaf::lang::exceptions::NoSuchElementException& e ){}
    pmap.setInt( "int", 3 );
    CPPUNIT_ASSERT( pmap.getInt("int") == 3 );

    try{
        pmap.getLong( "long" );
        CPPUNIT_ASSERT( false );
    } catch( decaf::lang::exceptions::NoSuchElementException& e ){}
    pmap.setLong( "long", 4L );
    CPPUNIT_ASSERT( pmap.getLong("long") == 4L );

    try{
        pmap.getDouble( "double" );
        CPPUNIT_ASSERT( false );
    } catch( decaf::lang::exceptions::NoSuchElementException& e ){}
    pmap.setDouble( "double", 2.3 );
    CPPUNIT_ASSERT( pmap.getDouble("double") == 2.3 );

    try{
        pmap.getFloat( "float" );
        CPPUNIT_ASSERT( false );
    } catch( decaf::lang::exceptions::NoSuchElementException& e ){}
    pmap.setFloat( "float", 3.2f );
    CPPUNIT_ASSERT( pmap.getFloat("float") == 3.2f );

    try{
        pmap.getString( "string" );
        CPPUNIT_ASSERT( false );
    } catch( decaf::lang::exceptions::NoSuchElementException& e ){}
    pmap.setString( "string", "hello" );
    CPPUNIT_ASSERT( pmap.getString("string") == "hello" );

    std::vector<unsigned char> byteArray;
    byteArray.push_back( 'a' );
    byteArray.push_back( 'b' );
    byteArray.push_back( 'c' );
    byteArray.push_back( 'd' );

    try{
        pmap.getByteArray( "byteArray" );
        CPPUNIT_ASSERT( false );
    } catch( decaf::lang::exceptions::NoSuchElementException& e ){}
    pmap.setByteArray( "byteArray", byteArray );
    CPPUNIT_ASSERT( pmap.getByteArray("byteArray") == byteArray );

}

void PrimitiveMapTest::testRemove(){

    PrimitiveMap pmap;
    pmap.setInt("int", 5 );
    pmap.setFloat( "float", 5.5f );
    pmap.setInt("int2", 6 );
    pmap.remove("int");
    try{
        pmap.getInt("int");
        CPPUNIT_ASSERT( false );
    } catch( decaf::lang::exceptions::NoSuchElementException& e ){}
}

void PrimitiveMapTest::testCount(){

    PrimitiveMap pmap;
    CPPUNIT_ASSERT( pmap.size() == 0 );
    pmap.setInt("int", 5 );
    CPPUNIT_ASSERT( pmap.size() == 1);
    pmap.setFloat( "float", 5.5f );
    CPPUNIT_ASSERT( pmap.size() == 2 );
    pmap.setInt("int2", 6 );
    CPPUNIT_ASSERT( pmap.size() == 3 );
    pmap.remove("int");
    CPPUNIT_ASSERT( pmap.size() == 2 );

    CPPUNIT_ASSERT( pmap.toString() != "" );
}

void PrimitiveMapTest::testCopy(){

    PrimitiveMap pmap;
    pmap.setInt("int", 5 );
    pmap.setFloat( "float", 5.5f );
    pmap.setInt("int2", 6 );

    PrimitiveMap copy;
    copy.copy( pmap );
    CPPUNIT_ASSERT( pmap.equals( copy ) );

    PrimitiveMap copy1( pmap );
    CPPUNIT_ASSERT( pmap.equals( copy1 ) );
}

void PrimitiveMapTest::testClear(){

    PrimitiveMap pmap;
    pmap.setInt("int", 5 );
    pmap.setFloat( "float", 5.5f );
    pmap.setInt("int2", 6 );

    pmap.clear();
    CPPUNIT_ASSERT( pmap.size() == 0 );

    try{
        pmap.getInt("int");
        CPPUNIT_ASSERT( false );
    } catch( decaf::lang::exceptions::NoSuchElementException& e ){}

    try{
        pmap.getFloat("float");
        CPPUNIT_ASSERT( false );
    } catch( decaf::lang::exceptions::NoSuchElementException& e ){}

    try{
        pmap.getInt("int2");
        CPPUNIT_ASSERT( false );
    } catch( decaf::lang::exceptions::NoSuchElementException& e ){}
}

void PrimitiveMapTest::testContains(){

    PrimitiveMap pmap;

    CPPUNIT_ASSERT( pmap.containsKey("int") == false );

    pmap.setInt("int", 5 );
    CPPUNIT_ASSERT( pmap.containsKey("int") == true );

    pmap.setFloat( "float", 5.5f );
    CPPUNIT_ASSERT( pmap.containsKey("float") == true );

    pmap.setInt("int2", 6 );
    CPPUNIT_ASSERT( pmap.containsKey("int2") == true );

    pmap.remove("int");
    CPPUNIT_ASSERT( pmap.containsKey("int") == false );
}

void PrimitiveMapTest::testGetKeys(){

    PrimitiveMap pmap;

    pmap.setInt("int", 5 );
    pmap.setFloat( "float", 5.5f );
    pmap.setInt("int2", 6 );
    std::vector<std::string> keys = pmap.getKeys();

    CPPUNIT_ASSERT( keys.size() == 3 );
    CPPUNIT_ASSERT( keys[0] == "int" || keys[0] == "float" || keys[0] == "int2" );
    CPPUNIT_ASSERT( keys[1] == "int" || keys[1] == "float" || keys[1] == "int2" );
    CPPUNIT_ASSERT( keys[2] == "int" || keys[2] == "float" || keys[2] == "int2" );
}

