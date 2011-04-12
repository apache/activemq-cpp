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

#include "ActiveMQBytesMessageTest.h"

#include <decaf/util/UUID.h>
#include <decaf/lang/Exception.h>
#include <activemq/commands/ActiveMQBytesMessage.h>

using namespace std;
using namespace cms;
using namespace activemq;
using namespace activemq::util;
using namespace activemq::commands;
using namespace decaf;
using namespace decaf::lang;

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessageTest::testGetBodyLength() {
    ActiveMQBytesMessage msg;
    int len = 10;

    try {

        for( int i = 0; i < len; i++ ) {
            msg.writeLong( 5LL );
        }

    } catch( CMSException& ex ) {
        ex.printStackTrace();
    }

    try {

        msg.reset();
        CPPUNIT_ASSERT( msg.getBodyLength() == ( len * 8 ) );

    } catch( Exception& e ) {
        e.printStackTrace();
        CPPUNIT_ASSERT( false );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessageTest::testReadBoolean() {

    ActiveMQBytesMessage msg;
    try {
        msg.writeBoolean( true );
        msg.reset();
        CPPUNIT_ASSERT( msg.readBoolean() );
    } catch( CMSException& ex ) {
        ex.printStackTrace();
        CPPUNIT_ASSERT( false );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessageTest::testReadByte() {

    ActiveMQBytesMessage msg;
    try {
        msg.writeByte( (unsigned char)2 );
        msg.reset();
        CPPUNIT_ASSERT( msg.readByte() == 2 );
    } catch( CMSException& ex ) {
        ex.printStackTrace();
        CPPUNIT_ASSERT( false );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessageTest::testReadShort() {

    ActiveMQBytesMessage msg;
    try {
        msg.writeShort( (short)3000 );
        msg.reset();
        CPPUNIT_ASSERT( msg.readShort() == 3000 );
    } catch( CMSException& ex ) {
        ex.printStackTrace();
        CPPUNIT_ASSERT( false );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessageTest::testReadUnsignedShort() {

    ActiveMQBytesMessage msg;
    try {
        msg.writeShort( (short)3000 );
        msg.reset();
        CPPUNIT_ASSERT( msg.readUnsignedShort() == 3000 );
    } catch( CMSException& ex ) {
        ex.printStackTrace();
        CPPUNIT_ASSERT( false );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessageTest::testReadChar() {

    ActiveMQBytesMessage msg;
    try {
        msg.writeChar( 'a' );
        msg.reset();
        CPPUNIT_ASSERT( msg.readChar() == 'a' );
    } catch( CMSException& ex ) {
        ex.printStackTrace();
        CPPUNIT_ASSERT( false );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessageTest::testReadInt() {

    ActiveMQBytesMessage msg;
    try {
        msg.writeInt( 3000 );
        msg.reset();
        CPPUNIT_ASSERT( msg.readInt() == 3000 );
    } catch( CMSException& ex ) {
        ex.printStackTrace();
        CPPUNIT_ASSERT( false );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessageTest::testReadLong() {

    ActiveMQBytesMessage msg;
    try {
        msg.writeLong( 3000 );
        msg.reset();
        CPPUNIT_ASSERT( msg.readLong() == 3000 );
    } catch( CMSException& ex ) {
        ex.printStackTrace();
        CPPUNIT_ASSERT( false );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessageTest::testReadFloat() {

    ActiveMQBytesMessage msg;
    try {
        msg.writeFloat( 3.3f );
        msg.reset();
        CPPUNIT_ASSERT( msg.readFloat() == 3.3f );
    } catch( CMSException& ex ) {
        ex.printStackTrace();
        CPPUNIT_ASSERT( false );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessageTest::testReadDouble() {

    ActiveMQBytesMessage msg;
    try {
        msg.writeDouble( 3.3 );
        msg.reset();
        CPPUNIT_ASSERT( msg.readDouble() == 3.3 );
    } catch( CMSException& ex ) {
        ex.printStackTrace();
        CPPUNIT_ASSERT( false );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessageTest::testReadUTF() {

    ActiveMQBytesMessage msg;
    try {
        std::string str = "this is a test";
        msg.writeUTF( str );
        msg.reset();
        CPPUNIT_ASSERT( msg.readUTF() == str );
    } catch( CMSException& ex ) {
        ex.printStackTrace();
        CPPUNIT_ASSERT( false );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessageTest::testReadBytesbyteArray() {

    ActiveMQBytesMessage msg;
    try {
        unsigned char data[50];
        for( int i = 0; i < 50; i++ ) {
            data[i] = (unsigned char)i;
        }
        msg.writeBytes( &data[0], 0, 50 );
        msg.reset();
        unsigned char test[50];
        msg.readBytes( test, 50 );
        for( int i = 0; i < 50; i++ ) {
            CPPUNIT_ASSERT( test[i] == i );
        }

        CPPUNIT_ASSERT_THROW_MESSAGE(
            "Should have thrown a CMSException",
            msg.readBytes( test, -1 ),
            CMSException);

    } catch( CMSException& ex ) {
        ex.printStackTrace();
        CPPUNIT_ASSERT( false );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessageTest::testClearBody() {
    ActiveMQBytesMessage bytesMessage;
    try {
        bytesMessage.writeInt( 1 );
        bytesMessage.clearBody();
        CPPUNIT_ASSERT( !bytesMessage.isReadOnlyBody() );
        bytesMessage.writeInt( 1 );
        bytesMessage.readInt();
    } catch( MessageNotReadableException& mnwe ) {
    } catch( MessageNotWriteableException& mnwe ) {
        CPPUNIT_ASSERT( false );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessageTest::testReset() {
    ActiveMQBytesMessage message;
    try {
        message.writeDouble( 24.5 );
        message.writeLong( 311 );
    } catch( MessageNotWriteableException& mnwe ) {
        CPPUNIT_FAIL( "should be writeable" );
    }
    message.reset();
    try {
        CPPUNIT_ASSERT( message.isReadOnlyBody() );
        CPPUNIT_ASSERT_DOUBLES_EQUAL( message.readDouble(), 24.5, 0 );
        CPPUNIT_ASSERT_EQUAL( message.readLong(), 311LL );
    } catch( MessageNotReadableException& mnre ) {
        CPPUNIT_FAIL( "should be readable" );
    }
    try {
        message.writeInt( 33 );
        CPPUNIT_FAIL( "should throw exception" );
    } catch( MessageNotWriteableException& mnwe ) {
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessageTest::testReadOnlyBody() {

    ActiveMQBytesMessage message;
    std::vector<unsigned char> buffer(3);
    unsigned char array[2];

    try {
        message.writeBoolean( true );
        message.writeByte( (unsigned char)1 );
        message.writeByte( (unsigned char)1 );
        message.writeBytes( buffer );
        message.writeBytes( &array[0], 0, 2 );
        message.writeChar( 'a' );
        message.writeDouble( 1.5 );
        message.writeFloat( (float)1.5 );
        message.writeInt( 1 );
        message.writeLong( 1 );
        message.writeString( "stringobj" );
        message.writeShort( (short)1 );
        message.writeShort( (short)1 );
        message.writeUTF( "utfstring" );
    } catch( MessageNotWriteableException& mnwe ) {
        CPPUNIT_FAIL( "Should be writeable" );
    }
    message.reset();
    try {
        message.readBoolean();
        message.readByte();
        message.readByte();
        message.readBytes( buffer );
        message.readBytes( &array[0], 2 );
        message.readChar();
        message.readDouble();
        message.readFloat();
        message.readInt();
        message.readLong();
        message.readString();
        message.readShort();
        message.readUnsignedShort();
        message.readUTF();
    } catch( MessageNotReadableException& mnwe ) {
        CPPUNIT_FAIL( "Should be readable" );
    }
    try {
        message.writeBoolean( true );
        CPPUNIT_FAIL( "Should have thrown exception" );
    } catch( MessageNotWriteableException& mnwe ) {
    }
    try {
        message.writeByte( (unsigned char)1 );
        CPPUNIT_FAIL( "Should have thrown exception" );
    } catch( MessageNotWriteableException& mnwe ) {
    }
    try {
        message.writeBytes( buffer );
        CPPUNIT_FAIL( "Should have thrown exception" );
    } catch( MessageNotWriteableException& mnwe ) {
    }
    try {
        message.writeBytes( &array[0], 0, 2 );
        CPPUNIT_FAIL( "Should have thrown exception" );
    } catch( MessageNotWriteableException& mnwe ) {
    }
    try {
        message.writeChar( 'a' );
        CPPUNIT_FAIL( "Should have thrown exception" );
    } catch( MessageNotWriteableException& mnwe ) {
    }
    try {
        message.writeDouble( 1.5 );
        CPPUNIT_FAIL( "Should have thrown exception" );
    } catch( MessageNotWriteableException& mnwe ) {
    }
    try {
        message.writeFloat( (float)1.5 );
        CPPUNIT_FAIL( "Should have thrown exception" );
    } catch( MessageNotWriteableException& mnwe ) {
    }
    try {
        message.writeInt( 1 );
        CPPUNIT_FAIL( "Should have thrown exception" );
    } catch( MessageNotWriteableException& mnwe ) {
    }
    try {
        message.writeLong( 1 );
        CPPUNIT_FAIL( "Should have thrown exception" );
    } catch( MessageNotWriteableException& mnwe ) {
    }
    try {
        message.writeString( "stringobj" );
        CPPUNIT_FAIL( "Should have thrown exception" );
    } catch( MessageNotWriteableException& mnwe ) {
    }
    try {
        message.writeShort( (short)1 );
        CPPUNIT_FAIL( "Should have thrown exception" );
    } catch( MessageNotWriteableException& mnwe ) {
    }
    try {
        message.writeUTF( "utfstring" );
        CPPUNIT_FAIL( "Should have thrown exception" );
    } catch( MessageNotWriteableException& mnwe ) {
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessageTest::testWriteOnlyBody() {
    ActiveMQBytesMessage message;
    message.clearBody();
    std::vector<unsigned char> buffer(3);
    unsigned char array[2];
    try {
        message.writeBoolean( true );
        message.writeByte( (unsigned char)1 );
        message.writeByte( (unsigned char)1 );
        message.writeBytes( buffer );
        message.writeBytes( &array[0], 0, 2 );
        message.writeChar( 'a' );
        message.writeDouble( 1.5 );
        message.writeFloat( (float)1.5 );
        message.writeInt( 1 );
        message.writeLong( 1LL );
        message.writeString( "stringobj" );
        message.writeShort( (short)1 );
        message.writeShort( (short)1 );
        message.writeUTF( "utfstring" );
    } catch( MessageNotWriteableException& mnwe ) {
        CPPUNIT_FAIL( "Should be writeable" );
    }
    try {
        message.readBoolean();
        CPPUNIT_FAIL( "Should have thrown exception" );
    } catch( MessageNotReadableException& mnwe ) {
    }
    try {
        message.readByte();
        CPPUNIT_FAIL( "Should have thrown exception" );
    } catch( MessageNotReadableException& e ) {
    }
    try {
        message.readBytes( buffer );
        CPPUNIT_FAIL( "Should have thrown exception" );
    } catch( MessageNotReadableException& e ) {
    }
    try {
        message.readBytes( &array[0], 2 );
        CPPUNIT_FAIL( "Should have thrown exception" );
    } catch( MessageNotReadableException& e ) {
    }
    try {
        message.readChar();
        CPPUNIT_FAIL( "Should have thrown exception" );
    } catch( MessageNotReadableException& e ) {
    }
    try {
        message.readDouble();
        CPPUNIT_FAIL( "Should have thrown exception" );
    } catch( MessageNotReadableException& e ) {
    }
    try {
        message.readFloat();
        CPPUNIT_FAIL( "Should have thrown exception" );
    } catch( MessageNotReadableException& e ) {
    }
    try {
        message.readInt();
        CPPUNIT_FAIL( "Should have thrown exception" );
    } catch( MessageNotReadableException& e ) {
    }
    try {
        message.readLong();
        CPPUNIT_FAIL( "Should have thrown exception" );
    } catch( MessageNotReadableException& e ) {
    }
    try {
        message.readString();
        CPPUNIT_FAIL( "Should have thrown exception" );
    } catch( MessageNotReadableException& e ) {
    }
    try {
        message.readShort();
        CPPUNIT_FAIL( "Should have thrown exception" );
    } catch( MessageNotReadableException& e ) {
    }
    try {
        message.readUnsignedShort();
        CPPUNIT_FAIL( "Should have thrown exception" );
    } catch( MessageNotReadableException& e ) {
    }
    try {
        message.readUTF();
        CPPUNIT_FAIL( "Should have thrown exception" );
    } catch( MessageNotReadableException& e ) {
    }
}
