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

#include "DataOutputStreamTest.h"

using namespace std;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::io;
using namespace decaf::util;

////////////////////////////////////////////////////////////////////////////////
void DataOutputStreamTest::testFlush() {

    try {
        os->writeInt(9087589);
        os->flush();
        openDataInputStream();
        int c = is->readInt();
        is->close();
        CPPUNIT_ASSERT_MESSAGE("Failed to flush correctly", 9087589 == c);
    } catch( IOException &e ) {
        CPPUNIT_FAIL("Exception during flush test : " + e.getMessage());
    }
}

////////////////////////////////////////////////////////////////////////////////
void DataOutputStreamTest::testSize() {

    try {
        os->write( (unsigned char*)&testData[0], 0, 150 );
        os->close();
        openDataInputStream();
        unsigned char rbuf[150];
        is->read( rbuf, 0, 150 );
        is->close();
        CPPUNIT_ASSERT_MESSAGE("Incorrect size returned", 150 == os->size());
    } catch( IOException &e ) {
        CPPUNIT_FAIL("Exception during write test : " + e.getMessage());
    }
}

////////////////////////////////////////////////////////////////////////////////
void DataOutputStreamTest::testWrite1() {

    try {
        os->write( (unsigned char*)&testData[0], 0, 150 );
        os->close();
        openDataInputStream();
        unsigned char* rbuf = new unsigned char[150];
        is->read(rbuf, 0, 150);
        is->close();
        CPPUNIT_ASSERT_MESSAGE("Incorrect bytes written",
            string( (const char*)rbuf, 150 ) == testData.substr( 0, 150 ) );
        delete [] rbuf;
    } catch( IOException &e ) {
        CPPUNIT_FAIL("Exception during write test : " + e.getMessage());
    }
}

////////////////////////////////////////////////////////////////////////////////
void DataOutputStreamTest::testWrite2() {

    try {
        os->write( 't' );
        os->close();
        openDataInputStream();
        char c = is->readChar();
        is->close();
        CPPUNIT_ASSERT_MESSAGE("Incorrect int written", 't' == c);
    } catch( IOException &e ) {
        CPPUNIT_FAIL("Exception during write test : " + e.getMessage());
    }
}

////////////////////////////////////////////////////////////////////////////////
void DataOutputStreamTest::testWriteBoolean() {

    try {
        os->writeBoolean(true);
        os->close();
        openDataInputStream();
        bool c = is->readBoolean();
        is->close();
        CPPUNIT_ASSERT_MESSAGE("Incorrect boolean written", c );
    } catch( IOException &e ) {
        CPPUNIT_FAIL("Exception during writeBoolean test : " + e.getMessage());
    }
}

////////////////////////////////////////////////////////////////////////////////
void DataOutputStreamTest::testWriteByte() {
    try {
        os->writeByte( (unsigned char) 127 );
        os->close();
        openDataInputStream();
        unsigned char c = is->readByte();
        is->close();
        CPPUNIT_ASSERT_MESSAGE("Incorrect unsigned char written", c == (unsigned char) 127);
    } catch( IOException &e ) {
        CPPUNIT_FAIL("Exception during writeByte test : " + e.getMessage());
    }
}

////////////////////////////////////////////////////////////////////////////////
void DataOutputStreamTest::testWriteBytes() {

    try {
        os->writeBytes( testData );
        os->close();
        openDataInputStream();
        std::vector<unsigned char> result( testData.size() );
        is->read( result );
        is->close();
        CPPUNIT_ASSERT_MESSAGE("Incorrect bytes written",
            string( (const char*)&result[0], result.size() ) == testData );
    } catch( IOException &e ) {
        CPPUNIT_FAIL("Exception during writeBytes test : " + e.getMessage());
    }

    // regression test for HARMONY-1101
    DataOutputStream tester(NULL);
    tester.writeBytes("");
}

////////////////////////////////////////////////////////////////////////////////
void DataOutputStreamTest::testWriteChar() {
    try {
        os->writeChar('T');
        os->close();
        openDataInputStream();
        char c = is->readChar();
        is->close();
        CPPUNIT_ASSERT_MESSAGE("Incorrect char written", 'T' == c );
    } catch( IOException &e ) {
        CPPUNIT_FAIL("Exception during writeChar test : " + e.getMessage());
    }
}

////////////////////////////////////////////////////////////////////////////////
void DataOutputStreamTest::testWriteChars() {

    try {
        os->writeChars( testData );
        os->close();
        openDataInputStream();
        std::vector<unsigned char> result( testData.size() );
        is->read( result );
        is->close();
        CPPUNIT_ASSERT_MESSAGE("Incorrect bytes written",
            string( (const char*)&result[0], result.size() ) == testData );
    } catch( IOException &e ) {
        CPPUNIT_FAIL("Exception during writeChars test : " + e.getMessage());
    }
}

////////////////////////////////////////////////////////////////////////////////
void DataOutputStreamTest::testWriteDouble() {
    try {
        os->writeDouble(908755555456.98);
        os->close();
        openDataInputStream();
        double c = is->readDouble();
        is->close();
        CPPUNIT_ASSERT_MESSAGE("Incorrect double written", 908755555456.98 == c);
    } catch( IOException &e ) {
        CPPUNIT_FAIL("Exception during writeDouble test : " + e.getMessage());
    }
}

////////////////////////////////////////////////////////////////////////////////
void DataOutputStreamTest::testWriteFloat() {
    try {
        os->writeFloat(9087.456f);
        os->close();
        openDataInputStream();
        float c = is->readFloat();
        is->close();
        CPPUNIT_ASSERT_MESSAGE("Incorrect float written", c == 9087.456f);
    } catch( IOException &e ) {
        CPPUNIT_FAIL("Exception during writeFloattest : " + e.getMessage());
    }
}

////////////////////////////////////////////////////////////////////////////////
void DataOutputStreamTest::testWriteInt() {
    try {
        os->writeInt(9087589);
        os->close();
        openDataInputStream();
        int c = is->readInt();
        is->close();
        CPPUNIT_ASSERT_MESSAGE("Incorrect int written", 9087589 == c);
    } catch( IOException &e ) {
        CPPUNIT_FAIL("Exception during writeInt test : " + e.getMessage());
    }
}

////////////////////////////////////////////////////////////////////////////////
void DataOutputStreamTest::testWriteLong() {
    try {
        os->writeLong(908755555456LL);
        os->close();
        openDataInputStream();
        long long c = is->readLong();
        is->close();
        CPPUNIT_ASSERT_MESSAGE("Incorrect long written", 908755555456LL == c);
    } catch( IOException &e ) {
        CPPUNIT_FAIL("Exception during writeLong test" + e.getMessage());
    }
}

////////////////////////////////////////////////////////////////////////////////
void DataOutputStreamTest::testWriteShort() {
    try {
        os->writeShort((short) 9087);
        os->close();
        openDataInputStream();
        short c = is->readShort();
        is->close();
        CPPUNIT_ASSERT_MESSAGE("Incorrect short written", 9087 == c);
    } catch( IOException &e ) {
        CPPUNIT_FAIL("Exception during writeShort test : " + e.getMessage());
    }
}

////////////////////////////////////////////////////////////////////////////////
void DataOutputStreamTest::testWriteUTF() {
    string testString = "test string one";
    os->writeUTF( testString );
    os->close();
    openDataInputStream();
    CPPUNIT_ASSERT_MESSAGE("Failed to write string in UTF format",
        is->available() == testString.length() + 2 );
    CPPUNIT_ASSERT_MESSAGE("Incorrect string returned",
        is->readUTF() == testString );
}

////////////////////////////////////////////////////////////////////////////////
void DataOutputStreamTest::testWriteUTFStringLength() {

    // String of length 65536 of Null Characters.
    // Expect: UTFDataFormatException.
    std::string testStr( 65536, char('a') );
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a UTFDataFormatException",
        os->writeUTF( testStr ),
        UTFDataFormatException );

    baos->reset();
    // String of length 65535 of non Null Characters since Null encodes as UTF-8.
    // Expect: Success.
    testStr.resize( 65535 );
    CPPUNIT_ASSERT_NO_THROW_MESSAGE(
        "String of 65535 Non-Null chars should not throw.",
        os->writeUTF( testStr ) );

    baos->reset();
    // Set one of the 65535 bytes to a value that will result in a 2 byte UTF8 encoded sequence.
    // This will cause the string of length 65535 to have a utf length of 65536.
    // Expect: UTFDataFormatException.
    testStr[0] = char( 255 );
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an UTFDataFormatException",
        os->writeUTF( testStr ),
        UTFDataFormatException );

    // Test that a zero length string write the zero size marker.
    ByteArrayInputStream byteIn;
    ByteArrayOutputStream byteOut;
    DataInputStream dataIn( &byteIn );
    DataOutputStream dataOut( &byteOut );
    dataOut.writeUTF( "" );
    CPPUNIT_ASSERT( dataOut.size() == 2 );
    byteIn.setByteArray( byteOut.toByteArray(), byteOut.size() );
    CPPUNIT_ASSERT( dataIn.readUnsignedShort() == 0 );
}

////////////////////////////////////////////////////////////////////////////////
void DataOutputStreamTest::testHelper( unsigned char* input, int inputLength,
                                       unsigned char* expect, int expectLength ) {

    std::string testStr( (char*)input, inputLength );
    os->writeUTF( testStr );

    const unsigned char* result = baos->toByteArray();

    CPPUNIT_ASSERT( result[0] == 0x00 );
    CPPUNIT_ASSERT( result[1] == (unsigned char)( expectLength ) );

    for( std::size_t i = 2; i < baos->size(); ++i ) {
        CPPUNIT_ASSERT( result[i] == expect[i-2] );
    }

    baos->reset();
}

////////////////////////////////////////////////////////////////////////////////
void DataOutputStreamTest::testWriteUTFEncoding() {

    // Test data with 1-byte UTF8 encoding.
    {
        unsigned char input[] = {0x00, 0x0B, 0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x57, 0x6F, 0x72, 0x6C, 0x64};
        unsigned char expect[] = {0xC0, 0x80, 0x0B, 0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x57, 0x6F, 0x72, 0x6C, 0x64};

        testHelper( input, sizeof(input)/sizeof(unsigned char),
                    expect, sizeof(expect)/sizeof(unsigned char) );
    }

    // Test data with 2-byte UT8 encoding.
    {
        unsigned char input[] = {0x00, 0xC2, 0xA9, 0xC3, 0xA6 };
        unsigned char expect[] = {0xC0, 0x80, 0xC3, 0x82, 0xC2, 0xA9, 0xC3, 0x83, 0xC2, 0xA6 };
        testHelper( input, sizeof(input)/sizeof(unsigned char),
                    expect, sizeof(expect)/sizeof(unsigned char)  );
    }

    // Test data with 1-byte and 2-byte encoding with embedded NULL's.
    {
        unsigned char input[] = {0x00, 0x04, 0xC2, 0xA9, 0xC3, 0x00, 0xA6 };
        unsigned char expect[] = {0xC0, 0x80, 0x04, 0xC3, 0x82, 0xC2, 0xA9, 0xC3, 0x83, 0xC0, 0x80, 0xC2, 0xA6 };

        testHelper( input, sizeof(input)/sizeof(unsigned char),
                    expect, sizeof(expect)/sizeof(unsigned char) );
    }
}

////////////////////////////////////////////////////////////////////////////////
void DataOutputStreamTest::test(){

    unsigned char byteVal = (unsigned char)'T';
    unsigned short shortVal = 5;
    unsigned int intVal = 10000;
    unsigned long long longVal = 1000000000;
    float floatVal = 10.0f;
    double doubleVal = 100.0;
    unsigned char arrayVal[3] = {
        'a', 'b', 'c'
    };

    // Create the stream with the buffer we just wrote to.
    ByteArrayOutputStream myStream;
    DataOutputStream writer( &myStream );

    writer.writeByte( byteVal );
    writer.writeShort( shortVal );
    writer.writeInt( intVal );
    writer.writeLong( longVal );
    writer.writeFloat( floatVal );
    writer.writeDouble( doubleVal );
    writer.write( arrayVal, 0, 3 );

    const unsigned char* buffer = myStream.toByteArray();
    int ix = 0;

    unsigned char tempByte = buffer[ix];
    CPPUNIT_ASSERT( tempByte == byteVal );
    ix += sizeof( tempByte );

    unsigned short tempShort = 0;
    memcpy( &tempShort, buffer+ix, sizeof( unsigned short ) );
    tempShort = util::Endian::byteSwap( tempShort );
    CPPUNIT_ASSERT( tempShort == shortVal );
    ix += sizeof( tempShort );

    unsigned int tempInt = 0;
    memcpy( &tempInt, buffer+ix, sizeof( unsigned int ) );
    tempInt = util::Endian::byteSwap( tempInt );
    CPPUNIT_ASSERT( tempInt == intVal );
    ix += sizeof( tempInt );

    unsigned long long tempLong = 0;
    memcpy( &tempLong, buffer+ix, sizeof( unsigned long long ) );
    tempLong = util::Endian::byteSwap( tempLong );
    CPPUNIT_ASSERT( tempLong == longVal );
    ix += sizeof( tempLong );

    float tempFloat = 0;
    memcpy( &tempFloat, buffer+ix, sizeof( float ) );
    tempFloat = util::Endian::byteSwap( tempFloat );
    CPPUNIT_ASSERT( tempFloat == floatVal );
    ix += sizeof( tempFloat );

    double tempDouble = 0;
    memcpy( &tempDouble, buffer+ix, sizeof( double ) );
    tempDouble = util::Endian::byteSwap( tempDouble );
    CPPUNIT_ASSERT( tempDouble == doubleVal );
    ix += sizeof( tempDouble );
}
