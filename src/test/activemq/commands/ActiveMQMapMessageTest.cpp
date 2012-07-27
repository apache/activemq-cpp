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

#include "ActiveMQMapMessageTest.h"

#include <activemq/commands/ActiveMQMapMessage.h>

#include <algorithm>

using namespace cms;
using namespace std;
using namespace activemq;
using namespace activemq::util;
using namespace activemq::commands;

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMapMessageTest::test() {
    ActiveMQMapMessage myMessage;

    CPPUNIT_ASSERT( myMessage.getDataStructureType() == ActiveMQMapMessage::ID_ACTIVEMQMAPMESSAGE );

    CPPUNIT_ASSERT( myMessage.getMapNames().size() == 0 );
    CPPUNIT_ASSERT( myMessage.itemExists( "Something" ) == false );

    std::vector<unsigned char> data;

    data.push_back( 2 );
    data.push_back( 4 );
    data.push_back( 8 );
    data.push_back( 16 );
    data.push_back( 32 );

    myMessage.setBoolean( "boolean", false );
    myMessage.setByte( "byte", 127 );
    myMessage.setChar( "char", 'a' );
    myMessage.setShort( "short", 32000 );
    myMessage.setInt( "int", 6789999 );
    myMessage.setLong( "long", 0xFFFAAA33345LL );
    myMessage.setFloat( "float", 0.000012f );
    myMessage.setDouble( "double", 64.54654 );
    myMessage.setBytes( "bytes", data );

    CPPUNIT_ASSERT( myMessage.getBoolean( "boolean" ) == false );
    CPPUNIT_ASSERT( myMessage.getByte( "byte" ) == 127 );
    CPPUNIT_ASSERT( myMessage.getChar( "char" ) == 'a' );
    CPPUNIT_ASSERT( myMessage.getShort( "short" ) == 32000 );
    CPPUNIT_ASSERT( myMessage.getInt( "int" ) == 6789999 );
    CPPUNIT_ASSERT( myMessage.getLong( "long" ) == 0xFFFAAA33345LL );
    CPPUNIT_ASSERT( myMessage.getFloat( "float" ) == 0.000012f );
    CPPUNIT_ASSERT( myMessage.getDouble( "double" ) == 64.54654 );
    CPPUNIT_ASSERT( myMessage.getBytes( "bytes" ) == data );
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMapMessageTest::testBytesConversion() {

    ActiveMQMapMessage msg;

    std::vector<unsigned char> buffer( 1 );

    msg.setBoolean( "boolean", true );
    msg.setByte( "byte", (unsigned char)1 );
    msg.setBytes( "bytes", buffer );
    msg.setChar( "char", 'a' );
    msg.setDouble( "double", 1.5 );
    msg.setFloat( "float", 1.5f );
    msg.setInt( "int", 1 );
    msg.setLong( "long", 1 );
    msg.setShort( "short", (short)1 );
    msg.setString( "string", "string" );

    // Test with a 1Meg String
    std::string bigString;

    bigString.reserve( 1024 * 1024 );
    for( int i = 0; i < 1024 * 1024; i++ ) {
        bigString += (char)( (int)'a' + i % 26 );
    }

    msg.setString( "bigString", bigString );

    ActiveMQMapMessage msg2;
    msg2.copyDataStructure( &msg );

    CPPUNIT_ASSERT_EQUAL( msg2.getBoolean("boolean"), true);
    CPPUNIT_ASSERT_EQUAL( msg2.getByte( "byte" ), (unsigned char)1 );
    CPPUNIT_ASSERT_EQUAL( msg2.getBytes( "bytes" ).size(), (std::size_t)1 );
    CPPUNIT_ASSERT_EQUAL( msg2.getChar( "char" ), 'a' );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( msg2.getDouble( "double" ), 1.5, 0.01 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( msg2.getFloat( "float" ), 1.5f, 0.01 );
    CPPUNIT_ASSERT_EQUAL( msg2.getInt( "int" ), 1 );
    CPPUNIT_ASSERT_EQUAL( msg2.getLong( "long" ), 1LL );
    CPPUNIT_ASSERT_EQUAL( msg2.getShort( "short" ), (short)1 );
    CPPUNIT_ASSERT_EQUAL( msg2.getString( "string" ), std::string( "string" ) );
    CPPUNIT_ASSERT_EQUAL( msg2.getString( "bigString" ), bigString );
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMapMessageTest::testGetBoolean() {

    ActiveMQMapMessage msg;
    msg.setBoolean( name, true );
    msg.setReadOnlyBody( true );
    CPPUNIT_ASSERT( msg.getBoolean( name ) );
    msg.clearBody();
    msg.setString( name, "true" );

    ActiveMQMapMessage msg2;
    msg2.copyDataStructure( &msg );

    CPPUNIT_ASSERT( msg2.getBoolean( name ) );
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMapMessageTest::testGetByte() {
    ActiveMQMapMessage msg;
    msg.setByte( name, (unsigned char)1 );

    ActiveMQMapMessage msg2;
    msg2.copyDataStructure( &msg );

    CPPUNIT_ASSERT( msg2.getByte( name ) == (unsigned char)1 );
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMapMessageTest::testGetShort() {
    ActiveMQMapMessage msg;
    try {
        msg.setShort( name, (short)1 );

        ActiveMQMapMessage msg2;
        msg2.copyDataStructure( &msg );

        CPPUNIT_ASSERT( msg2.getShort( name ) == (short)1 );

    } catch( CMSException& ex ) {
        ex.printStackTrace();
        CPPUNIT_ASSERT( false );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMapMessageTest::testGetChar() {
    ActiveMQMapMessage msg;
    try {
        msg.setChar( name, 'a' );

        ActiveMQMapMessage msg2;
        msg2.copyDataStructure( &msg );

        CPPUNIT_ASSERT( msg2.getChar( name ) == 'a' );

    } catch( CMSException& ex ) {
        ex.printStackTrace();
        CPPUNIT_ASSERT( false );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMapMessageTest::testGetInt() {
    ActiveMQMapMessage msg;
    try {
        msg.setInt( name, 1 );

        ActiveMQMapMessage msg2;
        msg2.copyDataStructure( &msg );

        CPPUNIT_ASSERT( msg2.getInt( name ) == 1 );

    } catch( CMSException& ex ) {
        ex.printStackTrace();
        CPPUNIT_ASSERT( false );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMapMessageTest::testGetLong() {
    ActiveMQMapMessage msg;
    try {
        msg.setLong( name, 1 );

        ActiveMQMapMessage msg2;
        msg2.copyDataStructure( &msg );

        CPPUNIT_ASSERT( msg2.getLong( name ) == 1 );

    } catch( CMSException& ex ) {
        ex.printStackTrace();
        CPPUNIT_ASSERT( false );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMapMessageTest::testGetFloat() {
    ActiveMQMapMessage msg;
    try {
        msg.setFloat( name, 1.5f );

        ActiveMQMapMessage msg2;
        msg2.copyDataStructure( &msg );

        CPPUNIT_ASSERT( msg2.getFloat( name ) == 1.5f );

    } catch( CMSException& ex ) {
        ex.printStackTrace();
        CPPUNIT_ASSERT( false );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMapMessageTest::testGetDouble() {
    ActiveMQMapMessage msg;
    try {
        msg.setDouble( name, 1.5 );

        ActiveMQMapMessage msg2;
        msg2.copyDataStructure( &msg );

        CPPUNIT_ASSERT( msg2.getDouble( name ) == 1.5 );

    } catch( CMSException& ex ) {
        ex.printStackTrace();
        CPPUNIT_ASSERT( false );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMapMessageTest::testGetString() {
    ActiveMQMapMessage msg;
    try {
        std::string str = "test";
        msg.setString( name, str );

        ActiveMQMapMessage msg2;
        msg2.copyDataStructure( &msg );

        CPPUNIT_ASSERT( msg2.getString( name ) == str );

    } catch( CMSException& ex ) {
        ex.printStackTrace();
        CPPUNIT_ASSERT( false );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMapMessageTest::testGetBytes() {
    ActiveMQMapMessage msg;
    try {

        std::vector<unsigned char> bytes1( 3, 'a' );
        std::vector<unsigned char> bytes2( 2, 'b' );

        msg.setBytes( name, bytes1 );
        msg.setBytes( name + "2", bytes2 );

        ActiveMQMapMessage msg2;
        msg2.copyDataStructure( &msg );

        CPPUNIT_ASSERT( msg2.getBytes( name ) == bytes1 );
        CPPUNIT_ASSERT_EQUAL( msg2.getBytes( name + "2" ).size(), bytes2.size() );

    } catch( CMSException& ex ) {
        ex.printStackTrace();
        CPPUNIT_ASSERT( false );
    }

    ActiveMQMapMessage msg3;
    msg3.setBytes( "empty", std::vector<unsigned char>() );
    CPPUNIT_ASSERT_NO_THROW( msg3.getBytes( "empty" ) );
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMapMessageTest::testGetMapNames() {

    ActiveMQMapMessage msg;

    std::vector<unsigned char> bytes1( 3, 'a' );
    std::vector<unsigned char> bytes2( 2, 'b' );

    msg.setBoolean( "boolean", true );
    msg.setByte( "byte", (unsigned char)1 );
    msg.setBytes( "bytes1", bytes1 );
    msg.setBytes( "bytes2", bytes2 );
    msg.setChar( "char", 'a' );
    msg.setDouble( "double", 1.5 );
    msg.setFloat( "float", 1.5f );
    msg.setInt( "int", 1 );
    msg.setLong( "long", 1 );
    msg.setShort( "short", (short)1 );
    msg.setString( "string", "string" );

    ActiveMQMapMessage msg2;
    msg2.copyDataStructure( &msg );

    std::vector<std::string> mapNamesList = msg2.getMapNames();

    CPPUNIT_ASSERT_EQUAL( (std::size_t)11, mapNamesList.size() );
    CPPUNIT_ASSERT( std::find( mapNamesList.begin(), mapNamesList.end(), "boolean" ) != mapNamesList.end() );
    CPPUNIT_ASSERT( std::find( mapNamesList.begin(), mapNamesList.end(), "byte" ) != mapNamesList.end() );
    CPPUNIT_ASSERT( std::find( mapNamesList.begin(), mapNamesList.end(), "bytes1" ) != mapNamesList.end() );
    CPPUNIT_ASSERT( std::find( mapNamesList.begin(), mapNamesList.end(), "bytes2" ) != mapNamesList.end() );
    CPPUNIT_ASSERT( std::find( mapNamesList.begin(), mapNamesList.end(), "char" ) != mapNamesList.end() );
    CPPUNIT_ASSERT( std::find( mapNamesList.begin(), mapNamesList.end(), "double" ) != mapNamesList.end() );
    CPPUNIT_ASSERT( std::find( mapNamesList.begin(), mapNamesList.end(), "float" ) != mapNamesList.end() );
    CPPUNIT_ASSERT( std::find( mapNamesList.begin(), mapNamesList.end(), "int" ) != mapNamesList.end() );
    CPPUNIT_ASSERT( std::find( mapNamesList.begin(), mapNamesList.end(), "long" ) != mapNamesList.end() );
    CPPUNIT_ASSERT( std::find( mapNamesList.begin(), mapNamesList.end(), "short" ) != mapNamesList.end() );
    CPPUNIT_ASSERT( std::find( mapNamesList.begin(), mapNamesList.end(), "string" ) != mapNamesList.end() );
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMapMessageTest::testItemExists() {
    ActiveMQMapMessage mapMessage;

    mapMessage.setString( "exists", "test" );

    ActiveMQMapMessage mapMessage2;
    mapMessage2.copyDataStructure( &mapMessage );

    CPPUNIT_ASSERT( mapMessage2.itemExists( "exists" ) );
    CPPUNIT_ASSERT( !mapMessage2.itemExists( "doesntExist" ) );
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMapMessageTest::testClearBody() {

    ActiveMQMapMessage mapMessage;
    mapMessage.setString( "String", "String" );
    mapMessage.clearBody();
    CPPUNIT_ASSERT( !mapMessage.isReadOnlyBody() );

    mapMessage.onSend();
    mapMessage.setContent( mapMessage.getContent() );
    CPPUNIT_ASSERT( mapMessage.itemExists( "String" ) == false );
    mapMessage.clearBody();
    mapMessage.setString( "String", "String" );

    ActiveMQMapMessage mapMessage2;
    mapMessage2.copyDataStructure( &mapMessage );

    CPPUNIT_ASSERT( mapMessage2.itemExists( "String" ) );
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMapMessageTest::testReadOnlyBody() {

    ActiveMQMapMessage msg;
    std::vector<unsigned char> buffer(2);

    msg.setBoolean( "boolean", true );
    msg.setByte( "byte", (unsigned char)1 );
    msg.setBytes( "bytes", buffer );
    msg.setChar( "char", 'a' );
    msg.setDouble( "double", 1.5 );
    msg.setFloat( "float", 1.5f );
    msg.setInt( "int", 1 );
    msg.setLong( "long", 1 );
    msg.setShort( "short", (short)1 );
    msg.setString( "string", "string" );

    msg.setReadOnlyBody( true );

    try {
        msg.getBoolean( "boolean" );
        msg.getByte( "byte" );
        msg.getBytes( "bytes" );
        msg.getChar( "char" );
        msg.getDouble( "double" );
        msg.getFloat( "float" );
        msg.getInt( "int" );
        msg.getLong( "long" );
        msg.getShort( "short" );
        msg.getString( "string" );
    } catch( MessageNotReadableException& mnre ) {
        CPPUNIT_FAIL( "should be readable" );
    }
    try {
        msg.setBoolean( "boolean", true );
        CPPUNIT_FAIL( "should throw exception" );
    } catch( MessageNotWriteableException& mnwe ) {
    }
    try {
        msg.setByte( "byte", (unsigned char)1 );
        CPPUNIT_FAIL( "should throw exception" );
    } catch( MessageNotWriteableException& mnwe ) {
    }
    try {
        msg.setBytes( "bytes", buffer );
        CPPUNIT_FAIL( "should throw exception" );
    } catch( MessageNotWriteableException& mnwe ) {
    }
    try {
        msg.setChar( "char", 'a' );
        CPPUNIT_FAIL( "should throw exception" );
    } catch( MessageNotWriteableException& mnwe ) {
    }
    try {
        msg.setDouble( "double", 1.5 );
        CPPUNIT_FAIL( "should throw exception" );
    } catch( MessageNotWriteableException& mnwe ) {
    }
    try {
        msg.setFloat( "float", 1.5f );
        CPPUNIT_FAIL( "should throw exception" );
    } catch( MessageNotWriteableException& mnwe ) {
    }
    try {
        msg.setInt( "int", 1 );
        CPPUNIT_FAIL( "should throw exception" );
    } catch( MessageNotWriteableException& mnwe ) {
    }
    try {
        msg.setLong( "long", 1 );
        CPPUNIT_FAIL( "should throw exception" );
    } catch( MessageNotWriteableException& mnwe ) {
    }
    try {
        msg.setShort( "short", (short)1 );
        CPPUNIT_FAIL( "should throw exception" );
    } catch( MessageNotWriteableException& mnwe ) {
    }
    try {
        msg.setString( "string", "string" );
        CPPUNIT_FAIL( "should throw exception" );
    } catch( MessageNotWriteableException& mnwe ) {
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMapMessageTest::testWriteOnlyBody() {

    ActiveMQMapMessage msg;

    std::vector<unsigned char> buffer1(1);
    std::vector<unsigned char> buffer2(2);

    msg.setReadOnlyBody( false );

    msg.setBoolean( "boolean", true );
    msg.setByte( "byte", (unsigned char)1 );
    msg.setBytes( "bytes", buffer1 );
    msg.setBytes( "bytes2", buffer2 );
    msg.setChar( "char", 'a' );
    msg.setDouble( "double", 1.5 );
    msg.setFloat( "float", 1.5f );
    msg.setInt( "int", 1 );
    msg.setLong( "long", 1 );
    msg.setShort( "short", (short)1 );
    msg.setString( "string", "string" );

    msg.setReadOnlyBody( true );

    msg.getBoolean( "boolean" );
    msg.getByte( "byte" );
    msg.getBytes( "bytes" );
    msg.getChar( "char" );
    msg.getDouble( "double" );
    msg.getFloat( "float" );
    msg.getInt( "int" );
    msg.getLong( "long" );
    msg.getShort( "short" );
    msg.getString( "string" );
}
