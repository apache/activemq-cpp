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

#include "DataInputStreamTest.h"

#include <decaf/lang/Integer.h>

using namespace std;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::io;
using namespace decaf::util;

////////////////////////////////////////////////////////////////////////////////
void DataInputStreamTest::testConstructor() {

    try {

        os->writeChar('t');
        os->close();
        openDataInputStream();
    } catch (IOException e) {
        CPPUNIT_FAIL("IOException during constructor test : " + e.getMessage());
    }

    try {
        is->close();
    } catch (IOException e) {
        CPPUNIT_FAIL("IOException during constructor test : " + e.getMessage());
    }
}

////////////////////////////////////////////////////////////////////////////////
void DataInputStreamTest::testRead1() {

    try {

        std::vector<unsigned char> test( testData.begin(), testData.end() );
        os->write( test );
        os->close();
        openDataInputStream();
        std::vector<unsigned char> result;
        result.resize( testData.length() );
        is->read( result );
        CPPUNIT_ASSERT_MESSAGE( "Incorrect data read",
            string( (const char*)&result[0], result.size() ) == testData );
    } catch( IOException &e ) {
        CPPUNIT_FAIL("IOException during read test : " + e.getMessage());
    }
}

////////////////////////////////////////////////////////////////////////////////
void DataInputStreamTest::testRead2() {
    try {

        os->write( std::vector<unsigned char>( testData.begin(), testData.end() ) );
        os->close();
        openDataInputStream();
        unsigned char* result = new unsigned char[ testData.length() ];
        is->read( result, 0, testData.length() );
        CPPUNIT_ASSERT_MESSAGE("Incorrect data read",
            string( (const char*)result, testData.size() ) == testData );
        delete [] result;
    } catch( IOException &e ) {
        CPPUNIT_FAIL("IOException during read test : " + e.getMessage());
    }
}

////////////////////////////////////////////////////////////////////////////////
void DataInputStreamTest::test_readBoolean() {

    try {
        os->writeBoolean(true);
        os->close();
        openDataInputStream();
        CPPUNIT_ASSERT_MESSAGE("Incorrect boolean written", is->readBoolean() );
    } catch( IOException &e ) {
        CPPUNIT_FAIL("readBoolean test failed : " + e.getMessage());
    }
}

////////////////////////////////////////////////////////////////////////////////
void DataInputStreamTest::test_readByte() {
    try {
        os->writeByte( (unsigned char) 127);
        os->close();
        openDataInputStream();
        CPPUNIT_ASSERT_MESSAGE("Incorrect byte read", is->readByte() == (unsigned char) 127);
    } catch( IOException &e ) {
        CPPUNIT_FAIL("IOException during readByte test : " + e.getMessage());
    }
}

////////////////////////////////////////////////////////////////////////////////
void DataInputStreamTest::test_readChar() {
    try {
        os->writeChar('t');
        os->close();
        openDataInputStream();
        CPPUNIT_ASSERT_MESSAGE("Incorrect char read", 't' == is->readChar());
    } catch( IOException &e ) {
        CPPUNIT_FAIL("IOException during readChar test : " + e.getMessage());
    }
}

////////////////////////////////////////////////////////////////////////////////
void DataInputStreamTest::test_readDouble() {
    try {
        os->writeDouble(2345.76834720202);
        os->close();
        openDataInputStream();
        CPPUNIT_ASSERT_MESSAGE("Incorrect double read",
                2345.76834720202 == is->readDouble());
    } catch( IOException &e ) {
        CPPUNIT_FAIL("IOException during readDouble test" + e.getMessage());
    }
}

////////////////////////////////////////////////////////////////////////////////
void DataInputStreamTest::test_readFloat() {
    try {
        os->writeFloat(29.08764f);
        os->close();
        openDataInputStream();
        CPPUNIT_ASSERT_MESSAGE("Incorrect float read", is->readFloat() == 29.08764f);
    } catch( IOException &e ) {
        CPPUNIT_FAIL("readFloat test failed : " + e.getMessage());
    }
}

////////////////////////////////////////////////////////////////////////////////
void DataInputStreamTest::test_readFully1() {
    try {
        os->write( std::vector<unsigned char>( testData.begin(), testData.end() ) );
        os->close();
        openDataInputStream();
        std::vector<unsigned char> result;
        result.resize( testData.length() );
        is->readFully( result );
        CPPUNIT_ASSERT_MESSAGE( "Incorrect data read",
            string( (const char*)&result[0], 0, result.size() ) == testData );
    } catch( IOException &e ) {
        CPPUNIT_FAIL("IOException during readFully test : " + e.getMessage());
    }
}

////////////////////////////////////////////////////////////////////////////////
void DataInputStreamTest::test_readFully2() {

    try {
        os->write( std::vector<unsigned char>( testData.begin(), testData.end() ) );
        os->close();
        openDataInputStream();
        unsigned char* rbytes = new unsigned char[ testData.length() ];
        is->readFully( rbytes, 0, testData.length() );
        CPPUNIT_ASSERT_MESSAGE("Incorrect data read",
            string( (const char*)rbytes, 0, testData.length() ) == testData );
        delete [] rbytes;
    } catch( IOException &e ) {
        CPPUNIT_FAIL("IOException during readFully test : " + e.getMessage());
    }
}

////////////////////////////////////////////////////////////////////////////////
void DataInputStreamTest::test_readFullyNullArray() {
    std::vector<unsigned char> test( 5000 );
    DataInputStream is(
        new ByteArrayInputStream( test ), true );

    unsigned char* nullByteArray = NULL;

    is.readFully( nullByteArray, 0, 0);
    is.readFully( nullByteArray, 1, 0);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "should throw NullPointerException",
        is.readFully( nullByteArray, 0, 1),
        NullPointerException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "should throw NullPointerException",
        is.readFully( nullByteArray, 1, 1),
        NullPointerException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "should throw NullPointerException",
        is.readFully( nullByteArray, 1, Integer::MAX_VALUE),
        NullPointerException );
}

////////////////////////////////////////////////////////////////////////////////
void DataInputStreamTest::test_readFullyNullStream() {

    DataInputStream is(NULL);
    unsigned char* byteArray = new unsigned char[testData.length()];

    is.readFully( byteArray, 0, 0 );
    is.readFully( byteArray, 1, 0 );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "should throw NullPointerException",
        is.readFully( byteArray, 1, 1 ),
        NullPointerException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "should throw NullPointerException",
        is.readFully( byteArray, 0, 1 ),
        NullPointerException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "should throw NullPointerException",
        is.readFully( byteArray, 0, Integer::MAX_VALUE ),
        NullPointerException );

    delete [] byteArray;
}

////////////////////////////////////////////////////////////////////////////////
void DataInputStreamTest::test_readFullyNullStreamNullArray() {

    DataInputStream is(NULL);
    unsigned char* nullByteArray = NULL;

    is.readFully( nullByteArray, 0, 0 );
    is.readFully( nullByteArray, 1, 0 );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "should throw NullPointerException",
        is.readFully( nullByteArray, 0, 1),
        NullPointerException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "should throw NullPointerException",
        is.readFully( nullByteArray, 1, 1),
        NullPointerException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "should throw NullPointerException",
        is.readFully( nullByteArray, 1, Integer::MAX_VALUE),
        NullPointerException );
}

////////////////////////////////////////////////////////////////////////////////
void DataInputStreamTest::test_readInt() {
    try {
        os->writeInt(768347202);
        os->close();
        openDataInputStream();
        CPPUNIT_ASSERT_MESSAGE("Incorrect int read", 768347202 == is->readInt());
    } catch( IOException &e ) {
        CPPUNIT_FAIL("IOException during readInt test : " + e.getMessage());
    }
}

////////////////////////////////////////////////////////////////////////////////
void DataInputStreamTest::test_readLong() {
    try {
        os->writeLong(9875645283333LL);
        os->close();
        openDataInputStream();
        CPPUNIT_ASSERT_MESSAGE("Incorrect long read", 9875645283333LL == is->readLong());
    } catch( IOException &e ) {
        CPPUNIT_FAIL("read long test failed : " + e.getMessage());
    }
}

////////////////////////////////////////////////////////////////////////////////
void DataInputStreamTest::test_readShort() {
    try {
        os->writeShort(9875);
        os->close();
        openDataInputStream();
        CPPUNIT_ASSERT_MESSAGE("Incorrect short read", is->readShort() == (short) 9875);
    } catch( IOException &e ) {
        CPPUNIT_FAIL("Exception during read short test : " + e.getMessage());
    }
}

////////////////////////////////////////////////////////////////////////////////
void DataInputStreamTest::test_readUnsignedByte() {
    try {
        os->writeByte((unsigned char) -127);
        os->close();
        openDataInputStream();
        CPPUNIT_ASSERT_MESSAGE("Incorrect byte read", 129 == is->readUnsignedByte());
    } catch( IOException &e ) {
        CPPUNIT_FAIL("IOException during readUnsignedByte test : " + e.getMessage());
    }
}

////////////////////////////////////////////////////////////////////////////////
void DataInputStreamTest::test_readUnsignedShort() {
    os->writeShort(9875);
    os->close();
    openDataInputStream();
    CPPUNIT_ASSERT_MESSAGE("Incorrect short read", 9875 == is->readUnsignedShort());
}

////////////////////////////////////////////////////////////////////////////////
void DataInputStreamTest::test_skipBytes() {
    try {
        os->write( std::vector<unsigned char>( testData.begin(), testData.end() ) );
        os->close();
        openDataInputStream();
        is->skip( 100 );
        std::vector<unsigned char> result( 50 );
        is->read( result );
        is->close();
        CPPUNIT_ASSERT_MESSAGE("Incorrect data read",
            string( (const char*)&result[0], 50) == testData.substr( 100, 50) );
    } catch( IOException &e ) {
        CPPUNIT_FAIL("IOException during skipBytes test 1 : " + e.getMessage());
    }
    try {

        std::size_t skipped = 0;
        openDataInputStream();

        CPPUNIT_ASSERT_NO_THROW_MESSAGE(
            "Should throw an EOFException",
            skipped = is->skip( 500000 ) );

        CPPUNIT_ASSERT_MESSAGE(
            "Skipped should report " +
            Integer::toString( (int)testData.length() ) + " not " +
            Integer::toString( (int)skipped ),
            skipped == testData.length() );

    } catch( IOException &e ) {
        CPPUNIT_FAIL("IOException during skipBytes test 2 : " + e.getMessage());
    }
}

////////////////////////////////////////////////////////////////////////////////
void DataInputStreamTest::test(){

    unsigned char buffer[30];
    int ix = 0;

    unsigned char byteVal = (unsigned char)'T';
    unsigned short shortVal = 5;
    unsigned int intVal = 10000;
    unsigned long long longVal = 1000000000;
    float floatVal = 10.0f;
    double doubleVal = 100.0;
    unsigned char arrayVal[3] = {
        'a', 'b', 'c'
    };

    int size = sizeof(char);
    memcpy( (char*)(buffer+ix), (char*)&byteVal, size );
    ix += size;

    size = sizeof(unsigned short);
    unsigned short tempShort = util::Endian::byteSwap(shortVal);
    memcpy( (char*)(buffer+ix), (char*)&tempShort, size );
    ix += size;

    size = sizeof(unsigned int);
    unsigned int tempInt = util::Endian::byteSwap(intVal);
    memcpy( (char*)(buffer+ix), (char*)&tempInt, size );
    ix += size;

    size = sizeof(unsigned long long);
    unsigned long long tempLong = util::Endian::byteSwap(longVal);
    memcpy( (char*)(buffer+ix), (char*)&tempLong, size );
    ix += size;

    size = sizeof(float);
    float tempFloat = util::Endian::byteSwap(floatVal);
    memcpy( (char*)(buffer+ix), (char*)&tempFloat, size );
    ix += size;

    size = sizeof(double);
    double tempDouble = util::Endian::byteSwap(doubleVal);
    memcpy( (char*)(buffer+ix), (char*)&tempDouble, size );
    ix += size;

    size = 3;
    memcpy( (char*)(buffer+ix), (char*)&arrayVal, size );
    ix += size;

    // Create the stream with the buffer we just wrote to.
    ByteArrayInputStream myStream( buffer, 30 );
    DataInputStream reader( &myStream );

    byteVal = reader.readByte();
    //std::cout << "Byte Value = " << byteVal << std::endl;
    CPPUNIT_ASSERT( byteVal == (unsigned char)'T' );

    shortVal = reader.readShort();
    //std::cout << "short Value = " << shortVal << std::endl;
    CPPUNIT_ASSERT( shortVal == 5 );

    intVal = reader.readInt();
    //std::cout << "int Value = " << intVal << std::endl;
    CPPUNIT_ASSERT( intVal == 10000 );

    longVal = reader.readLong();
    //std::cout << "long long Value = " << longVal << std::endl;
    CPPUNIT_ASSERT( longVal == 1000000000 );

    floatVal = reader.readFloat();
    //std::cout << "float Value = " << floatVal << std::endl;
    CPPUNIT_ASSERT( floatVal == 10.0f );

    doubleVal = reader.readDouble();
    //std::cout << "double Value = " << doubleVal << std::endl;
    CPPUNIT_ASSERT( doubleVal == 100.0 );

    reader.read( arrayVal, 0, 3 );
    //std::cout << "char[0] Value = " << (int)arrayVal[0] << std::endl;
    CPPUNIT_ASSERT( arrayVal[0] == 'a' );
    //std::cout << "char[1] Value = " << (int)arrayVal[1] << std::endl;
    CPPUNIT_ASSERT( arrayVal[1] == 'b' );
    //std::cout << "char[2] Value = " << (int)arrayVal[2] << std::endl;
    CPPUNIT_ASSERT( arrayVal[2] == 'c' );
}

////////////////////////////////////////////////////////////////////////////////
void DataInputStreamTest::testString() {

    std::string data1 = "This is a Test";
    std::string data2 = "of the readString method";
    std::string data3 = "This one should fail";

    std::vector<unsigned char> buffer;

    buffer.insert( buffer.begin(), data1.begin(), data1.end() );
    buffer.push_back( '\0' );
    buffer.insert( buffer.end(), data2.begin(), data2.end() );
    buffer.push_back( '\0' );
    buffer.insert( buffer.end(), data3.begin(), data3.end() );

    // Create the stream with the buffer we just wrote to.
    ByteArrayInputStream myStream( buffer );
    DataInputStream reader( &myStream );

    std::string result1 = reader.readString();
    std::string result2 = reader.readString();

    CPPUNIT_ASSERT( result1 == data1 );
    CPPUNIT_ASSERT( result2 == data2 );

    try{
        std::string result3 = reader.readString();
        CPPUNIT_ASSERT( false );
    } catch(...){
        CPPUNIT_ASSERT( true );
    }

    try{
        std::vector<unsigned char> buffer2;
        reader.readFully( buffer2 );
    } catch(...){
        CPPUNIT_ASSERT( false );
    }
}

////////////////////////////////////////////////////////////////////////////////
void DataInputStreamTest::testUTF() {

    std::string data1 = "This is a Test";
    std::string data2 = "of the readString method";
    std::string data3 = "This one should fail";

    char sizeData[sizeof(short)] = {0};
    short tempShort = 0;

    std::vector<unsigned char> buffer;

    tempShort = util::Endian::byteSwap( ((unsigned short)data1.size()) );
    memcpy( sizeData, (char*)&tempShort, sizeof( short ) );
    buffer.insert( buffer.end(), sizeData, sizeData + sizeof(short) );
    buffer.insert( buffer.end(), data1.begin(), data1.end() );

    tempShort = util::Endian::byteSwap( ((unsigned short)data2.size()) );
    memcpy( sizeData, (char*)&tempShort, sizeof( short ) );
    buffer.insert( buffer.end(), sizeData, sizeData + sizeof(short) );
    buffer.insert( buffer.end(), data2.begin(), data2.end() );

    tempShort = util::Endian::byteSwap( (unsigned short)(data3.size() + 10 ) );
    memcpy( sizeData, (char*)&tempShort, sizeof( short ) );
    buffer.insert( buffer.end(), sizeData, sizeData + sizeof(short) );
    buffer.insert( buffer.end(), data3.begin(), data3.end() );

    // Create the stream with the buffer we just wrote to.
    ByteArrayInputStream myStream( buffer );
    DataInputStream reader( &myStream );

    std::string result1 = reader.readUTF();
    std::string result2 = reader.readUTF();

    CPPUNIT_ASSERT( result1 == data1 );
    CPPUNIT_ASSERT( result2 == data2 );

    try{
        std::string result3 = reader.readUTF();
        CPPUNIT_ASSERT( false );
    } catch(...){
        CPPUNIT_ASSERT( true );
    }
}

////////////////////////////////////////////////////////////////////////////////
void DataInputStreamTest::testHelper( unsigned char* input, int inputLength,
                                      unsigned char* expect, int expectLength ) {

    ByteArrayInputStream myStream( input, inputLength );
    DataInputStream reader( &myStream );

    std::string result = reader.readUTF();

    for( std::size_t i = 0; i < result.length(); ++i ) {
        CPPUNIT_ASSERT( (unsigned char)result[i] == expect[i] );
    }
}

////////////////////////////////////////////////////////////////////////////////
void DataInputStreamTest::testUTFDecoding() {

    // Test data with 1-byte UTF8 encoding.
    {
        unsigned char expect[] = {0x00, 0x0B, 0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x57, 0x6F, 0x72, 0x6C, 0x64};
        unsigned char input[] = { 0x00, 0x0E ,0xC0, 0x80, 0x0B, 0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x57, 0x6F, 0x72, 0x6C, 0x64};

        testHelper( input, sizeof(input)/sizeof(unsigned char),
                    expect, sizeof(expect)/sizeof(unsigned char) );
    }

    // Test data with 2-byte UT8 encoding.
    {
        unsigned char expect[] = {0x00, 0xC2, 0xA9, 0xC3, 0xA6 };
        unsigned char input[] = { 0x00, 0x0A, 0xC0, 0x80, 0xC3, 0x82, 0xC2, 0xA9, 0xC3, 0x83, 0xC2, 0xA6 };
        testHelper( input, sizeof(input)/sizeof(unsigned char),
                    expect, sizeof(expect)/sizeof(unsigned char)  );
    }

    // Test data with 1-byte and 2-byte encoding with embedded NULL's.
    {
        unsigned char expect[] = {0x00, 0x04, 0xC2, 0xA9, 0xC3, 0x00, 0xA6 };
        unsigned char input[] = { 0x00, 0x0D, 0xC0, 0x80, 0x04, 0xC3, 0x82, 0xC2, 0xA9, 0xC3, 0x83, 0xC0, 0x80, 0xC2, 0xA6 };

        testHelper( input, sizeof(input)/sizeof(unsigned char),
                    expect, sizeof(expect)/sizeof(unsigned char) );
    }

    // Test with bad UTF-8 encoding, missing 2nd byte of two byte value
    {
        unsigned char input[] = { 0x00, 0x0D, 0xC0, 0x80, 0x04, 0xC3, 0x82, 0xC2, 0xC2, 0xC3, 0x83, 0xC0, 0x80, 0xC2, 0xA6 };

        ByteArrayInputStream myStream( input, sizeof(input)/sizeof(unsigned char) );
        DataInputStream reader( &myStream );

        CPPUNIT_ASSERT_THROW_MESSAGE(
            "Should throw a UTFDataFormatException",
            reader.readUTF(),
            UTFDataFormatException );
    }

    // Test with bad UTF-8 encoding, encoded value greater than 255
    {
        unsigned char input[] = { 0x00, 0x0D, 0xC0, 0x80, 0x04, 0xC3, 0x82, 0xC2, 0xC2, 0xC3, 0x83, 0xC0, 0x80, 0xC2, 0xA6 };

        ByteArrayInputStream myStream( input, sizeof(input)/sizeof(unsigned char) );
        DataInputStream reader( &myStream );

        CPPUNIT_ASSERT_THROW_MESSAGE(
            "Should throw a UTFDataFormatException",
            reader.readUTF(),
            UTFDataFormatException );
    }

    // Test data with value greater than 255 in 2-byte encoding.
    {
        unsigned char input[] = {0x00, 0x04, 0xC8, 0xA9, 0xC3, 0xA6};
        ByteArrayInputStream myStream( input, sizeof(input)/sizeof(unsigned char) );
        DataInputStream reader( &myStream );

        CPPUNIT_ASSERT_THROW_MESSAGE(
            "Should throw a UTFDataFormatException",
            reader.readUTF(),
            UTFDataFormatException );
    }

    // Test data with value greater than 255 in 3-byte encoding.
    {
        unsigned char input[] = {0x00, 0x05, 0xE8, 0xA8, 0xA9, 0xC3, 0xA6};
        ByteArrayInputStream myStream( input, sizeof(input)/sizeof(unsigned char) );
        DataInputStream reader( &myStream );

        CPPUNIT_ASSERT_THROW_MESSAGE(
            "Should throw a UTFDataFormatException",
            reader.readUTF(),
            UTFDataFormatException );
    }

    // Test with three byte encode that's missing a last byte.
    {
        unsigned char input[] = {0x00, 0x02, 0xE8, 0xA8};
        ByteArrayInputStream myStream( input, sizeof(input)/sizeof(unsigned char) );
        DataInputStream reader( &myStream );

        CPPUNIT_ASSERT_THROW_MESSAGE(
            "Should throw a UTFDataFormatException",
            reader.readUTF(),
            UTFDataFormatException );
    }

}
