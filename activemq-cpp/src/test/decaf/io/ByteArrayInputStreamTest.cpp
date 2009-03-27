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

#include "ByteArrayInputStreamTest.h"

using namespace std;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::io;
using namespace decaf::util;

////////////////////////////////////////////////////////////////////////////////
void ByteArrayInputStreamTest::testConstructor() {

    std::vector<unsigned char> testBuffer;

    testBuffer.push_back('t');
    testBuffer.push_back('e');
    testBuffer.push_back('s');
    testBuffer.push_back('t');

    ByteArrayInputStream stream_a( &testBuffer[0], testBuffer.size());
    ByteArrayInputStream stream_b( testBuffer );

    CPPUNIT_ASSERT_MESSAGE( "Unable to create ByteArrayInputStream",
                            stream_a.available() == testBuffer.size() );
    CPPUNIT_ASSERT_MESSAGE( "Unable to create ByteArrayInputStream",
                            stream_b.available() == testBuffer.size() );

    ByteArrayInputStream nullStream;
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an IO Exception",
        nullStream.read(),
        IOException );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayInputStreamTest::testConstructor2() {

    // Test for method ByteArrayInputStream(byte [], int, int)
    std::vector<unsigned char> testBuffer;
    for( int i = 0; i < 128; i++ ) {
        testBuffer.push_back( (char)i );
    }

    ByteArrayInputStream bis( &testBuffer[0], 100 );

    CPPUNIT_ASSERT_MESSAGE( "Unable to create ByteArrayInputStream",
                            100 == bis.available() );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayInputStreamTest::testAvailable() {

    // Test for method ByteArrayInputStream(byte [], int, int)
    std::vector<unsigned char> testBuffer;
    for( int i = 0; i < 128; i++ ) {
        testBuffer.push_back( (char)i );
    }

    ByteArrayInputStream bis( &testBuffer[0], 128 );

    CPPUNIT_ASSERT_MESSAGE( "Unable to create ByteArrayInputStream",
                            128 == bis.available() );
    for( int j = 0; j < 10; j++ ) {
        bis.read();
    }

    // Test for method int ByteArrayInputStream.available()
    CPPUNIT_ASSERT_MESSAGE( "Returned incorrect number of available bytes",
                            bis.available() == ( testBuffer.size() - 10 ) );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayInputStreamTest::testClose() {

    std::vector<unsigned char> testBuffer;
    for( int i = 0; i < 128; i++ ) {
        testBuffer.push_back( (char)i );
    }

    ByteArrayInputStream is( testBuffer );

    // Test for method void ByteArrayInputStream.close()
    try {
        is.read();
    } catch( IOException& e ) {
        CPPUNIT_FAIL("Failed reading from input stream");
    }

    try {
        is.close();
    } catch( IOException& e ) {
        CPPUNIT_FAIL("Failed closing input stream");
    }

    try {
        is.read();
    } catch( Exception& e ) {
        CPPUNIT_FAIL("Should be able to read from closed stream");
    }
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayInputStreamTest::testRead() {

    try{

        std::vector<unsigned char> testBuffer;
        for( int i = 0; i < 128; i++ ) {
            testBuffer.push_back( (char)i );
        }

        ByteArrayInputStream is( testBuffer );

        // Test for method int ByteArrayInputStream.read()
        int c = is.read();
        is.reset();
        CPPUNIT_ASSERT_MESSAGE( "read returned incorrect char",
                                c == testBuffer.at(0) );
    } catch(...) {
        CPPUNIT_FAIL( "Shouldn't get any exceptions in this test." );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayInputStreamTest::testRead2() {

    std::vector<unsigned char> testBuffer;
    for( int i = 0; i < 128; i++ ) {
        testBuffer.push_back( (char)i );
    }

    ByteArrayInputStream is( testBuffer );

    unsigned char buf1[20];
    is.skip(50);
    is.read( buf1, 0, 20 );
    CPPUNIT_ASSERT_MESSAGE(
        "Failed to read correct data",
        string( (const char*)buf1, 20 ) == string( (const char*)&testBuffer[50], 20) );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayInputStreamTest::testRead3() {

    std::vector<unsigned char> testBuffer;
    testBuffer.insert( testBuffer.end(), 10, 'a' );
    ByteArrayInputStream is( testBuffer );

    unsigned char buf[10];
    memset( buf, 'b', 10 );
    is.read( buf, 5, 5 );

    CPPUNIT_ASSERT_MESSAGE(
        "Failed to read correct data",
        string( (const char*)buf, 10 ) == "bbbbbaaaaa" );

    // Try for an EOF
    is.skip( 5 );
    CPPUNIT_ASSERT( is.read( buf, 5, 5 ) == -1 );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayInputStreamTest::testSkip() {

    std::vector<unsigned char> testBuffer;
    for( int i = 0; i < 128; i++ ) {
        testBuffer.push_back( (char)i );
    }

    ByteArrayInputStream is( testBuffer );

    unsigned char buf1[10];
    is.skip(100);
    is.read( buf1, 0, 10 );

    CPPUNIT_ASSERT_MESSAGE(
        "Failed to skip to correct position",
        string( (const char*)buf1, 10 ) == string( (const char*)&testBuffer[100], 10) );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayInputStreamTest::testStream()
{
    std::vector<unsigned char> testBuffer;

    testBuffer.push_back('t');
    testBuffer.push_back('e');
    testBuffer.push_back('s');
    testBuffer.push_back('t');

    ByteArrayInputStream stream_a(&testBuffer[0], testBuffer.size());

    CPPUNIT_ASSERT( stream_a.available() == 4 );

    char a = stream_a.read();
    char b = stream_a.read();
    char c = stream_a.read();
    char d = stream_a.read();

    CPPUNIT_ASSERT( a == 't' && b == 'e' && c == 's' && d == 't' );
    CPPUNIT_ASSERT( stream_a.available() == 0 );

    testBuffer.push_back('e');

    stream_a.setByteArray(&testBuffer[0], testBuffer.size());

    CPPUNIT_ASSERT( stream_a.available() == 5 );

    unsigned char* buffer = new unsigned char[6];

    buffer[5] = '\0';

    CPPUNIT_ASSERT( stream_a.read(buffer, 0, 5) == 5 );
    CPPUNIT_ASSERT( std::string((const char*)buffer) == std::string("teste") );
    CPPUNIT_ASSERT( stream_a.available() == 0 );

    stream_a.setByteArray(&testBuffer[0], testBuffer.size());

    memset(buffer, 0, 6);

    CPPUNIT_ASSERT( stream_a.read(buffer, 0, 3) == 3 );
    CPPUNIT_ASSERT( stream_a.read(&buffer[3], 0, 2) == 2 );
    CPPUNIT_ASSERT( std::string((const char*)buffer) == std::string("teste") );

    stream_a.close();

    delete [] buffer;
}
