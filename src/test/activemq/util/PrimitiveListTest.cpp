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
