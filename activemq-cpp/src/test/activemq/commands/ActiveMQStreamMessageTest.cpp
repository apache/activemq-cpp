/**
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "ActiveMQStreamMessageTest.h"

#include <activemq/commands/ActiveMQStreamMessage.h>

#include <cms/MessageFormatException.h>
#include <cms/MessageEOFException.h>
#include <cms/MessageNotReadableException.h>
#include <cms/MessageNotWriteableException.h>

#include <decaf/lang/Boolean.h>
#include <decaf/lang/Byte.h>
#include <decaf/lang/Character.h>
#include <decaf/lang/Short.h>
#include <decaf/lang/Integer.h>
#include <decaf/lang/Long.h>
#include <decaf/lang/Float.h>
#include <decaf/lang/Double.h>

using namespace cms;
using namespace std;
using namespace activemq;
using namespace activemq::commands;
using namespace decaf;
using namespace decaf::lang;

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessageTest::setUp() {
    this->buffer.resize( 100 );
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessageTest::tearDown() {
    this->buffer.clear();
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessageTest::testSetAndGet() {

    ActiveMQStreamMessage myMessage;

    CPPUNIT_ASSERT( myMessage.getDataStructureType() == ActiveMQStreamMessage::ID_ACTIVEMQSTREAMMESSAGE );

    std::vector<unsigned char> data;
    data.push_back( 2 );
    data.push_back( 4 );
    data.push_back( 8 );
    data.push_back( 16 );
    data.push_back( 32 );
    std::vector<unsigned char> readData( data.size() );

    myMessage.writeBoolean( false );
    myMessage.writeByte( 127 );
    myMessage.writeChar( 'a' );
    myMessage.writeShort( 32000 );
    myMessage.writeInt( 6789999 );
    myMessage.writeLong( 0xFFFAAA33345LL );
    myMessage.writeFloat( 0.000012f );
    myMessage.writeDouble( 64.54654 );
    myMessage.writeBytes( data );

    myMessage.reset();

    CPPUNIT_ASSERT( myMessage.getNextValueType() == cms::Message::BOOLEAN_TYPE );
    CPPUNIT_ASSERT( myMessage.readBoolean() == false );
    CPPUNIT_ASSERT( myMessage.getNextValueType() == cms::Message::BYTE_TYPE );
    CPPUNIT_ASSERT( myMessage.readByte() == 127 );
    CPPUNIT_ASSERT( myMessage.getNextValueType() == cms::Message::CHAR_TYPE );
    CPPUNIT_ASSERT( myMessage.readChar() == 'a' );
    CPPUNIT_ASSERT( myMessage.getNextValueType() == cms::Message::SHORT_TYPE );
    CPPUNIT_ASSERT( myMessage.readShort() == 32000 );
    CPPUNIT_ASSERT( myMessage.getNextValueType() == cms::Message::INTEGER_TYPE );
    CPPUNIT_ASSERT( myMessage.readInt() == 6789999 );
    CPPUNIT_ASSERT( myMessage.getNextValueType() == cms::Message::LONG_TYPE );
    CPPUNIT_ASSERT( myMessage.readLong() == 0xFFFAAA33345LL );
    CPPUNIT_ASSERT( myMessage.getNextValueType() == cms::Message::FLOAT_TYPE );
    CPPUNIT_ASSERT( myMessage.readFloat() == 0.000012f );
    CPPUNIT_ASSERT( myMessage.getNextValueType() == cms::Message::DOUBLE_TYPE );
    CPPUNIT_ASSERT( myMessage.readDouble() == 64.54654 );
    CPPUNIT_ASSERT( myMessage.getNextValueType() == cms::Message::BYTE_ARRAY_TYPE );
    CPPUNIT_ASSERT( myMessage.readBytes( readData ) == (int)data.size() );
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessageTest::testReadBoolean() {

    ActiveMQStreamMessage msg;

    try {

        msg.writeBoolean( true );
        msg.reset();
        CPPUNIT_ASSERT( msg.readBoolean() );
        msg.reset();
        CPPUNIT_ASSERT( msg.readString() == "true" );
        msg.reset();

        try {
            msg.readByte();
            CPPUNIT_FAIL("Should have thrown exception");
        } catch( MessageFormatException& ex ) {
        }
        msg.reset();
        try {
            msg.readShort();
            CPPUNIT_FAIL("Should have thrown exception");
        } catch( MessageFormatException& ex ) {
        }
        msg.reset();
        try {
            msg.readInt();
            CPPUNIT_FAIL("Should have thrown exception");
        } catch( MessageFormatException& ex ) {
        }
        msg.reset();
        try {
            msg.readLong();
            CPPUNIT_FAIL("Should have thrown exception");
        } catch( MessageFormatException& ex ) {
        }
        msg.reset();
        try {
            msg.readFloat();
            CPPUNIT_FAIL("Should have thrown exception");
        } catch( MessageFormatException& ex ) {
        }
        msg.reset();
        try {
            msg.readDouble();
            CPPUNIT_FAIL("Should have thrown exception");
        } catch( MessageFormatException& ex ) {
        }
        msg.reset();
        try {
            msg.readChar();
            CPPUNIT_FAIL("Should have thrown exception");
        } catch( MessageFormatException& ex ) {
        }
        msg.reset();
        try {
            msg.readBytes( buffer );
            CPPUNIT_FAIL("Should have thrown exception");
        } catch( MessageFormatException& ex ) {
        }

    } catch( CMSException& ex ) {
        ex.printStackTrace();
        CPPUNIT_ASSERT(false);
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessageTest::testReadByte() {

    ActiveMQStreamMessage msg;
    try {
        unsigned char test = (unsigned char)4;
        msg.writeByte( test );
        msg.reset();
        CPPUNIT_ASSERT(msg.readByte() == test);
        msg.reset();
        CPPUNIT_ASSERT(msg.readShort() == test);
        msg.reset();
        CPPUNIT_ASSERT(msg.readInt() == test);
        msg.reset();
        CPPUNIT_ASSERT(msg.readLong() == test);
        msg.reset();
        CPPUNIT_ASSERT(msg.readString() == Byte(test).toString());
        msg.reset();

        try {
            msg.readBoolean();
            CPPUNIT_FAIL("Should have thrown exception");
        } catch( MessageFormatException& ex ) {
        }
        msg.reset();
        try {
            msg.readFloat();
            CPPUNIT_FAIL("Should have thrown exception");
        } catch( MessageFormatException& ex ) {
        }
        msg.reset();
        try {
            msg.readDouble();
            CPPUNIT_FAIL("Should have thrown exception");
        } catch( MessageFormatException& ex ) {
        }
        msg.reset();
        try {
            msg.readChar();
            CPPUNIT_FAIL("Should have thrown exception");
        } catch( MessageFormatException& ex ) {
        }
        msg.reset();
        try {
            msg.readBytes( buffer );
            CPPUNIT_FAIL("Should have thrown exception");
        } catch( MessageFormatException& ex ) {
        }

    } catch( CMSException& ex ) {
        ex.printStackTrace();
        CPPUNIT_ASSERT(false);
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessageTest::testReadShort() {
    ActiveMQStreamMessage msg;
    try {

        short test = (short)4;
        msg.writeShort( test );
        msg.reset();
        CPPUNIT_ASSERT(msg.readShort() == test);
        msg.reset();
        CPPUNIT_ASSERT(msg.readInt() == test);
        msg.reset();
        CPPUNIT_ASSERT(msg.readLong() == test);
        msg.reset();
        CPPUNIT_ASSERT(msg.readString() == Short(test).toString());
        msg.reset();
        try {
            msg.readBoolean();
            CPPUNIT_FAIL("Should have thrown exception");
        } catch( MessageFormatException& ex ) {
        }
        msg.reset();
        try {
            msg.readByte();
            CPPUNIT_FAIL("Should have thrown exception");
        } catch( MessageFormatException& ex ) {
        }
        msg.reset();
        try {
            msg.readFloat();
            CPPUNIT_FAIL("Should have thrown exception");
        } catch( MessageFormatException& ex ) {
        }
        msg.reset();
        try {
            msg.readDouble();
            CPPUNIT_FAIL("Should have thrown exception");
        } catch( MessageFormatException& ex ) {
        }
        msg.reset();
        try {
            msg.readChar();
            CPPUNIT_FAIL("Should have thrown exception");
        } catch( MessageFormatException& ex ) {
        }
        msg.reset();
        try {
            msg.readBytes( buffer );
            CPPUNIT_FAIL("Should have thrown exception");
        } catch( MessageFormatException& ex ) {
        }

    } catch( CMSException& ex ) {
        ex.printStackTrace();
        CPPUNIT_ASSERT(false);
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessageTest::testReadChar() {
    ActiveMQStreamMessage msg;
    try {
        char test = 'z';
        msg.writeChar( test );
        msg.reset();
        CPPUNIT_ASSERT(msg.readChar() == test);
        msg.reset();
        CPPUNIT_ASSERT(msg.readString() == Character(test).toString());
        msg.reset();

        try {
            msg.readBoolean();
            CPPUNIT_FAIL("Should have thrown exception");
        } catch( MessageFormatException& ex ) {
        }
        msg.reset();
        try {
            msg.readByte();
            CPPUNIT_FAIL("Should have thrown exception");
        } catch( MessageFormatException& ex ) {
        }
        msg.reset();
        try {
            msg.readShort();
            CPPUNIT_FAIL("Should have thrown exception");
        } catch( MessageFormatException& ex ) {
        }
        msg.reset();
        try {
            msg.readInt();
            CPPUNIT_FAIL("Should have thrown exception");
        } catch( MessageFormatException& ex ) {
        }
        msg.reset();
        try {
            msg.readLong();
            CPPUNIT_FAIL("Should have thrown exception");
        } catch( MessageFormatException& ex ) {
        }
        msg.reset();
        try {
            msg.readFloat();
            CPPUNIT_FAIL("Should have thrown exception");
        } catch( MessageFormatException& ex ) {
        }
        msg.reset();
        try {
            msg.readDouble();
            CPPUNIT_FAIL("Should have thrown exception");
        } catch( MessageFormatException& ex ) {
        }
        msg.reset();
        try {
            msg.readBytes( buffer );
            CPPUNIT_FAIL("Should have thrown exception");
        } catch( MessageFormatException& ex ) {
        }

    } catch( CMSException& ex ) {
        ex.printStackTrace();
        CPPUNIT_ASSERT(false);
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessageTest::testReadInt() {

    ActiveMQStreamMessage msg;

    try {
        int test = 4;
        msg.writeInt( test );
        msg.reset();
        CPPUNIT_ASSERT(msg.readInt() == test);
        msg.reset();
        CPPUNIT_ASSERT(msg.readLong() == test);
        msg.reset();
        CPPUNIT_ASSERT(msg.readString() == Integer(test).toString());
        msg.reset();
        try {
            msg.readBoolean();
            CPPUNIT_FAIL("Should have thrown exception");
        } catch( MessageFormatException& ex ) {
        }
        msg.reset();
        try {
            msg.readByte();
            CPPUNIT_FAIL("Should have thrown exception");
        } catch( MessageFormatException& ex ) {
        }
        msg.reset();
        try {
            msg.readShort();
            CPPUNIT_FAIL("Should have thrown exception");
        } catch( MessageFormatException& ex ) {
        }
        msg.reset();
        try {
            msg.readFloat();
            CPPUNIT_FAIL("Should have thrown exception");
        } catch( MessageFormatException& ex ) {
        }
        msg.reset();
        try {
            msg.readDouble();
            CPPUNIT_FAIL("Should have thrown exception");
        } catch( MessageFormatException& ex ) {
        }
        msg.reset();
        try {
            msg.readChar();
            CPPUNIT_FAIL("Should have thrown exception");
        } catch( MessageFormatException& ex ) {
        }
        msg.reset();
        try {
            msg.readBytes( buffer );
            CPPUNIT_FAIL("Should have thrown exception");
        } catch( MessageFormatException& ex ) {
        }

    } catch( CMSException& ex ) {
        ex.printStackTrace();
        CPPUNIT_ASSERT(false);
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessageTest::testReadLong() {

    ActiveMQStreamMessage msg;

    try {
        long test = 4L;
        msg.writeLong( test );
        msg.reset();
        CPPUNIT_ASSERT(msg.readLong() == test);
        msg.reset();
        CPPUNIT_ASSERT(msg.readString() == Long::valueOf(test).toString());
        msg.reset();

        try {
            msg.readBoolean();
            CPPUNIT_FAIL("Should have thrown exception");
        } catch( MessageFormatException& ex ) {
        }
        msg.reset();
        try {
            msg.readByte();
            CPPUNIT_FAIL("Should have thrown exception");
        } catch( MessageFormatException& ex ) {
        }
        msg.reset();
        try {
            msg.readShort();
            CPPUNIT_FAIL("Should have thrown exception");
        } catch( MessageFormatException& ex ) {
        }
        msg.reset();
        try {
            msg.readInt();
            CPPUNIT_FAIL("Should have thrown exception");
        } catch( MessageFormatException& ex ) {
        }
        msg.reset();
        try {
            msg.readFloat();
            CPPUNIT_FAIL("Should have thrown exception");
        } catch( MessageFormatException& ex ) {
        }
        msg.reset();
        try {
            msg.readDouble();
            CPPUNIT_FAIL("Should have thrown exception");
        } catch( MessageFormatException& ex ) {
        }
        msg.reset();
        try {
            msg.readChar();
            CPPUNIT_FAIL("Should have thrown exception");
        } catch( MessageFormatException& ex ) {
        }
        msg.reset();
        try {
            msg.readBytes( buffer );
            CPPUNIT_FAIL("Should have thrown exception");
        } catch( MessageFormatException& ex ) {
        }

    } catch( CMSException& ex ) {
        ex.printStackTrace();
        CPPUNIT_ASSERT(false);
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessageTest::testReadFloat() {
    ActiveMQStreamMessage msg;
    try {
        float test = 4.4f;
        msg.writeFloat( test );
        msg.reset();
        CPPUNIT_ASSERT(msg.readFloat() == test);
        msg.reset();
        CPPUNIT_ASSERT(msg.readDouble() == test);
        msg.reset();
        CPPUNIT_ASSERT(msg.readString() == Float(test).toString());
        msg.reset();

        try {
            msg.readBoolean();
            CPPUNIT_FAIL("Should have thrown exception");
        } catch( MessageFormatException& ex ) {
        }
        msg.reset();
        try {
            msg.readByte();
            CPPUNIT_FAIL("Should have thrown exception");
        } catch( MessageFormatException& ex ) {
        }
        msg.reset();
        try {
            msg.readShort();
            CPPUNIT_FAIL("Should have thrown exception");
        } catch( MessageFormatException& ex ) {
        }
        msg.reset();
        try {
            msg.readInt();
            CPPUNIT_FAIL("Should have thrown exception");
        } catch( MessageFormatException& ex ) {
        }
        msg.reset();
        try {
            msg.readLong();
            CPPUNIT_FAIL("Should have thrown exception");
        } catch( MessageFormatException& ex ) {
        }
        msg.reset();
        try {
            msg.readChar();
            CPPUNIT_FAIL("Should have thrown exception");
        } catch( MessageFormatException& ex ) {
        }
        msg.reset();
        try {
            msg.readBytes( buffer );
            CPPUNIT_FAIL("Should have thrown exception");
        } catch( MessageFormatException& ex ) {
        }

    } catch( CMSException& ex ) {
        ex.printStackTrace();
        CPPUNIT_ASSERT(false);
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessageTest::testReadDouble() {
    ActiveMQStreamMessage msg;
    try {
        double test = 4.4;
        msg.writeDouble( test );
        msg.reset();
        CPPUNIT_ASSERT(msg.readDouble() == test);
        msg.reset();
        CPPUNIT_ASSERT(msg.readString() == Double(test).toString());
        msg.reset();

        try {
            msg.readBoolean();
            CPPUNIT_FAIL("Should have thrown exception");
        } catch( MessageFormatException& ex ) {
        }
        msg.reset();
        try {
            msg.readByte();
            CPPUNIT_FAIL("Should have thrown exception");
        } catch( MessageFormatException& ex ) {
        }
        msg.reset();
        try {
            msg.readShort();
            CPPUNIT_FAIL("Should have thrown exception");
        } catch( MessageFormatException& ex ) {
        }
        msg.reset();
        try {
            msg.readInt();
            CPPUNIT_FAIL("Should have thrown exception");
        } catch( MessageFormatException& ex ) {
        }
        msg.reset();
        try {
            msg.readLong();
            CPPUNIT_FAIL("Should have thrown exception");
        } catch( MessageFormatException& ex ) {
        }
        msg.reset();
        try {
            msg.readFloat();
            CPPUNIT_FAIL("Should have thrown exception");
        } catch( MessageFormatException& ex ) {
        }
        msg.reset();
        try {
            msg.readChar();
            CPPUNIT_FAIL("Should have thrown exception");
        } catch( MessageFormatException& ex ) {
        }
        msg.reset();
        try {
            msg.readBytes( buffer );
            CPPUNIT_FAIL("Should have thrown exception");
        } catch( MessageFormatException& ex ) {
        }

    } catch( CMSException& ex ) {
        ex.printStackTrace();
        CPPUNIT_ASSERT(false);
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessageTest::testReadString() {
    ActiveMQStreamMessage msg;
    try {
        unsigned char testByte = (unsigned char)2;
        msg.writeString( Byte( testByte ).toString() );
        msg.reset();
        CPPUNIT_ASSERT(msg.readByte() == testByte);
        msg.clearBody();
        short testShort = 3;
        msg.writeString( Short( testShort ).toString() );
        msg.reset();
        CPPUNIT_ASSERT(msg.readShort() == testShort);
        msg.clearBody();
        int testInt = 4;
        msg.writeString( Integer( testInt ).toString() );
        msg.reset();
        CPPUNIT_ASSERT(msg.readInt() == testInt);
        msg.clearBody();
        long testLong = 6L;
        msg.writeString( Long( testLong ).toString() );
        msg.reset();
        CPPUNIT_ASSERT(msg.readLong() == testLong);
        msg.clearBody();
        float testFloat = 6.6f;
        msg.writeString( Float( testFloat ).toString() );
        msg.reset();
        CPPUNIT_ASSERT(msg.readFloat() == testFloat);
        msg.clearBody();
        double testDouble = 7.7;
        msg.writeString( Double( testDouble ).toString() );
        msg.reset();
        CPPUNIT_ASSERT_DOUBLES_EQUAL( testDouble, msg.readDouble(), 0.05 );
        msg.clearBody();
        msg.writeString( "true" );
        msg.reset();
        CPPUNIT_ASSERT(msg.readBoolean());
        msg.clearBody();
        msg.writeString( "a" );
        msg.reset();
        try {
            msg.readChar();
            CPPUNIT_FAIL("Should have thrown exception");
        } catch( MessageFormatException& e ) {
        }
        msg.clearBody();
        msg.writeString( "777" );
        msg.reset();
        try {
            msg.readBytes( buffer );
            CPPUNIT_FAIL("Should have thrown exception");
        } catch( MessageFormatException& e ) {
        }

    } catch( CMSException& ex ) {
        ex.printStackTrace();
        CPPUNIT_ASSERT(false);
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessageTest::testReadBigString() {
    ActiveMQStreamMessage msg;
    try {
        // Test with a 1Meg String
        std::string bigString;
        bigString.reserve( 1024 * 1024 );
        for( int i = 0; i < 1024 * 1024; i++ ) {
            bigString.append( 1, (char)'a' + i % 26 );
        }

        msg.writeString( bigString );
        msg.reset();
        CPPUNIT_ASSERT_EQUAL( bigString, msg.readString() );

    } catch( CMSException& ex ) {
        ex.printStackTrace();
        CPPUNIT_ASSERT(false);
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessageTest::testReadBytes() {

    ActiveMQStreamMessage msg;
    try {

        unsigned char test[50];
        for( int i = 0; i < 50; i++ ) {
            test[i] = (unsigned char)i;
        }
        msg.writeBytes( test, 0, 50 );
        msg.reset();

        unsigned char valid[50];
        msg.readBytes( valid, 50 );
        for( int i = 0; i < 50; i++ ) {
            CPPUNIT_ASSERT(valid[i] == test[i]);
        }

        msg.reset();
        try {
            msg.readByte();
            CPPUNIT_FAIL("Should have thrown exception");
        } catch( MessageFormatException& ex ) {
        }
        msg.reset();
        try {
            msg.readShort();
            CPPUNIT_FAIL("Should have thrown exception");
        } catch( MessageFormatException& ex ) {
        }
        msg.reset();
        try {
            msg.readInt();
            CPPUNIT_FAIL("Should have thrown exception");
        } catch( MessageFormatException& ex ) {
        }
        msg.reset();
        try {
            msg.readLong();
            CPPUNIT_FAIL("Should have thrown exception");
        } catch( MessageFormatException& ex ) {
        }
        msg.reset();
        try {
            msg.readFloat();
            CPPUNIT_FAIL("Should have thrown exception");
        } catch( MessageFormatException& ex ) {
        }
        msg.reset();
        try {
            msg.readChar();
            CPPUNIT_FAIL("Should have thrown exception");
        } catch( MessageFormatException& ex ) {
        }
        msg.reset();
        try {
            msg.readString();
            CPPUNIT_FAIL("Should have thrown exception");
        } catch( MessageFormatException& ex ) {
        }

    } catch( CMSException& ex ) {
        ex.printStackTrace();
        CPPUNIT_ASSERT(false);
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessageTest::testClearBody() {

    ActiveMQStreamMessage streamMessage;
    try {

        streamMessage.writeLong( 2LL );
        streamMessage.clearBody();
        CPPUNIT_ASSERT( !streamMessage.isReadOnlyBody() );
        streamMessage.writeLong(  2LL );
        streamMessage.readLong();
        CPPUNIT_FAIL("should throw exception");

    } catch( MessageNotReadableException& mnwe ) {
    } catch( MessageNotWriteableException& mnwe ) {
        CPPUNIT_FAIL("should be writeable");
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessageTest::testReset() {

    ActiveMQStreamMessage streamMessage;

    try {
        streamMessage.writeDouble( 24.5 );
        streamMessage.writeLong( 311LL );
    } catch( MessageNotWriteableException& mnwe ) {
        CPPUNIT_FAIL("should be writeable");
    }

    streamMessage.reset();

    try {
        CPPUNIT_ASSERT(streamMessage.isReadOnlyBody());
        CPPUNIT_ASSERT_DOUBLES_EQUAL( streamMessage.readDouble(), 24.5, 0.01 );
        CPPUNIT_ASSERT_EQUAL( streamMessage.readLong(), 311LL );
    } catch( MessageNotReadableException& mnre ) {
        CPPUNIT_FAIL("should be readable");
    }

    try {
        streamMessage.writeInt( 33 );
        CPPUNIT_FAIL("should throw exception");
    } catch( MessageNotWriteableException& mnwe ) {
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessageTest::testReadOnlyBody() {
    ActiveMQStreamMessage message;
    try {
        message.writeBoolean( true );
        message.writeByte( (unsigned char)1 );
        message.writeChar('a');
        message.writeDouble( 121.5 );
        message.writeFloat( (float)1.5 );
        message.writeInt( 1 );
        message.writeLong( 1 );
        message.writeShort( (short)1 );
        message.writeString( "string" );
    } catch( MessageNotWriteableException& mnwe ) {
        CPPUNIT_FAIL("Should be writeable");
    }
    message.reset();
    try {
        message.readBoolean();
        message.readByte();
        message.readChar();
        message.readDouble();
        message.readFloat();
        message.readInt();
        message.readLong();
        message.readShort();
        message.readString();
    } catch( MessageNotReadableException& mnwe ) {
        CPPUNIT_FAIL("Should be readable");
    }
    try {
        message.writeBoolean( true );
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageNotWriteableException& mnwe ) {
    }
    try {
        message.writeByte( (unsigned char)1 );
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageNotWriteableException& mnwe ) {
    }
    try {
        message.writeBytes( buffer );
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageNotWriteableException& mnwe ) {
    }
    try {
        unsigned char test[3];
        message.writeBytes( test, 0, 2 );
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageNotWriteableException& mnwe ) {
    }
    try {
        message.writeChar( 'a' );
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageNotWriteableException& mnwe ) {
    }
    try {
        message.writeDouble( 1.5 );
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageNotWriteableException& mnwe ) {
    }
    try {
        message.writeFloat( (float)1.5 );
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageNotWriteableException& mnwe ) {
    }
    try {
        message.writeInt( 1 );
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageNotWriteableException& mnwe ) {
    }
    try {
        message.writeLong( 1 );
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageNotWriteableException& mnwe ) {
    }
    try {
        message.writeShort( (short)1 );
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageNotWriteableException& mnwe ) {
    }
    try {
        message.writeString( "string" );
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageNotWriteableException& mnwe ) {
    }
}

//////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessageTest::testWriteOnlyBody() {
    ActiveMQStreamMessage message;
    message.clearBody();
    try {
        message.writeBoolean( true );
        message.writeByte( (unsigned char)1 );
        message.writeBytes( buffer );
        message.writeChar( 'a' );
        message.writeDouble( 1.5 );
        message.writeFloat( (float)1.5 );
        message.writeInt( 1 );
        message.writeLong( 1 );
        message.writeShort( (short)1 );
        message.writeString( "string" );
    } catch( MessageNotWriteableException& mnwe ) {
        CPPUNIT_FAIL("Should be writeable");
    }
    try {
        message.getNextValueType();
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageNotReadableException& mnwe ) {
    }
    try {
        message.readBoolean();
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageNotReadableException& mnwe ) {
    }
    try {
        message.readByte();
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageNotReadableException& e ) {
    }
    try {
        message.readBytes( buffer );
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageNotReadableException& e ) {
    }
    try {
        unsigned char test[50];
        message.readBytes( test, 50 );
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageNotReadableException& e ) {
    }
    try {
        message.readChar();
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageNotReadableException& e ) {
    }
    try {
        message.readDouble();
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageNotReadableException& e ) {
    }
    try {
        message.readFloat();
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageNotReadableException& e ) {
    }
    try {
        message.readInt();
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageNotReadableException& e ) {
    }
    try {
        message.readLong();
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageNotReadableException& e ) {
    }
    try {
        message.readString();
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageNotReadableException& e ) {
    }
    try {
        message.readShort();
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageNotReadableException& e ) {
    }
    try {
        message.readString();
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageNotReadableException& e ) {
    }
}
