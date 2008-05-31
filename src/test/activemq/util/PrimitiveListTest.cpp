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

using namespace activemq;
using namespace activemq::util;

void PrimitiveListTest::testSetGet(){

    PrimitiveList plist;

    try{
        plist.getBool( 0 );
        CPPUNIT_ASSERT( false );
    } catch( decaf::lang::exceptions::NoSuchElementException& e ){}
    plist.setBool( 0, true );
    CPPUNIT_ASSERT( plist.getBool(0) == true );
    plist.setBool( 1, false );
    CPPUNIT_ASSERT( plist.getBool(1) == false );

    try{
        plist.getByte( 0 );
        CPPUNIT_ASSERT( false );
    } catch( decaf::lang::exceptions::NoSuchElementException& e ){}
    plist.setByte( 0, 1 );
    CPPUNIT_ASSERT( plist.getByte(0) == 1 );

    try{
        plist.getChar( 0 );
        CPPUNIT_ASSERT( false );
    } catch( decaf::lang::exceptions::NoSuchElementException& e ){}
    plist.setChar( 0, 'a' );
    CPPUNIT_ASSERT( plist.getChar(0) == 'a' );

    try{
        plist.getShort( 0 );
        CPPUNIT_ASSERT( false );
    } catch( decaf::lang::exceptions::NoSuchElementException& e ){}
    plist.setShort( 0, 2 );
    CPPUNIT_ASSERT( plist.getShort(0) == 2 );

    try{
        plist.getInt( 0 );
        CPPUNIT_ASSERT( false );
    } catch( decaf::lang::exceptions::NoSuchElementException& e ){}
    plist.setInt( 0, 3 );
    CPPUNIT_ASSERT( plist.getInt(0) == 3 );

    try{
        plist.getLong( 0 );
        CPPUNIT_ASSERT( false );
    } catch( decaf::lang::exceptions::NoSuchElementException& e ){}
    plist.setLong( 0, 4L );
    CPPUNIT_ASSERT( plist.getLong(0) == 4L );

    try{
        plist.getDouble( 0 );
        CPPUNIT_ASSERT( false );
    } catch( decaf::lang::exceptions::NoSuchElementException& e ){}
    plist.setDouble( 0, 2.3 );
    CPPUNIT_ASSERT( plist.getDouble(0) == 2.3 );

    try{
        plist.getFloat( 0 );
        CPPUNIT_ASSERT( false );
    } catch( decaf::lang::exceptions::NoSuchElementException& e ){}
    plist.setFloat( 0, 3.2f );
    CPPUNIT_ASSERT( plist.getFloat(0) == 3.2f );

    try{
        plist.getString( 0 );
        CPPUNIT_ASSERT( false );
    } catch( decaf::lang::exceptions::NoSuchElementException& e ){}
    plist.setString( 0, "hello" );
    CPPUNIT_ASSERT( plist.getString(0) == "hello" );

    std::vector<unsigned char> byteArray;
    byteArray.push_back( 'a' );
    byteArray.push_back( 'b' );
    byteArray.push_back( 'c' );
    byteArray.push_back( 'd' );

    try{
        plist.getByteArray( 0 );
        CPPUNIT_ASSERT( false );
    } catch( decaf::lang::exceptions::NoSuchElementException& e ){}
    plist.setByteArray( 0, byteArray );
    CPPUNIT_ASSERT( plist.getByteArray(0) == byteArray );

}

void PrimitiveListTest::testRemove(){

    PrimitiveList plist;
    plist.setInt( 0, 5 );
    plist.setFloat( 1, 5.5f );
    plist.setInt( 2, 6 );
    plist.remove( 0 );
    try{
        plist.getInt(0);
        CPPUNIT_ASSERT( false );
    } catch( decaf::lang::exceptions::NoSuchElementException& e ){}
}

void PrimitiveListTest::testCount(){

    PrimitiveList plist;
    CPPUNIT_ASSERT( plist.size() == 0 );
    plist.setInt( 0, 5 );
    CPPUNIT_ASSERT( plist.size() == 1);
    plist.setFloat( 1, 5.5f );
    CPPUNIT_ASSERT( plist.size() == 2 );
    plist.setInt( 2, 6 );
    CPPUNIT_ASSERT( plist.size() == 3 );
    plist.remove( 0 );
    CPPUNIT_ASSERT( plist.size() == 2 );

    CPPUNIT_ASSERT( plist.toString() != "" );
}

void PrimitiveListTest::testCopy(){

    PrimitiveList plist;
    plist.setInt( 0, 5 );
    plist.setFloat( 1, 5.5f );
    plist.setInt( 2, 6 );

    PrimitiveList copy;
    copy.copy( plist );
    CPPUNIT_ASSERT( plist.equals( copy ) );

    PrimitiveList copy1( plist );
    CPPUNIT_ASSERT( plist.equals( copy1 ) );
}

void PrimitiveListTest::testClear(){

    PrimitiveList plist;
    plist.setInt( 0, 5 );
    plist.setFloat( 1, 5.5f );
    plist.setInt( 2, 6 );

    plist.clear();
    CPPUNIT_ASSERT( plist.size() == 0 );

    try{
        plist.getInt( 0 );
        CPPUNIT_ASSERT( false );
    } catch( decaf::lang::exceptions::NoSuchElementException& e ){}

    try{
        plist.getFloat( 1 );
        CPPUNIT_ASSERT( false );
    } catch( decaf::lang::exceptions::NoSuchElementException& e ){}

    try{
        plist.getInt( 2 );
        CPPUNIT_ASSERT( false );
    } catch( decaf::lang::exceptions::NoSuchElementException& e ){}
}

void PrimitiveListTest::testContains(){

    PrimitiveList plist;

//    CPPUNIT_ASSERT( plist.contains("int") == false );
//
//    plist.setInt("int", 5 );
//    CPPUNIT_ASSERT( plist.contains("int") == true );
//
//    plist.setFloat( "float", 5.5f );
//    CPPUNIT_ASSERT( plist.contains("float") == true );
//
//    plist.setInt("int2", 6 );
//    CPPUNIT_ASSERT( plist.contains("int2") == true );
//
//    plist.remove("int");
//    CPPUNIT_ASSERT( plist.contains("int") == false );
}
