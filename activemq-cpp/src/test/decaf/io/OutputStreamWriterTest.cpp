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

#include "OutputStreamWriterTest.h"

#include <decaf/io/Writer.h>
#include <decaf/lang/exceptions/IndexOutOfBoundsException.h>
#include <decaf/io/ByteArrayInputStream.h>

using namespace std;
using namespace decaf;
using namespace decaf::io;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
const int OutputStreamWriterTest::BUFFER_SIZE = 10000;
const std::string OutputStreamWriterTest::TEST_STRING =
    "Test_All_Tests\nTest_decaf_io_BufferedInputStream\nTest_decaf_io_BufferedOutputStream\nTest_decaf_io_ByteArrayInputStream\nTest_decaf_io_ByteArrayOutputStream\nTest_decaf_io_DataInputStream\n";

////////////////////////////////////////////////////////////////////////////////
OutputStreamWriterTest::OutputStreamWriterTest() : writer1(), buffer1(), reader() {
}

////////////////////////////////////////////////////////////////////////////////
OutputStreamWriterTest::~OutputStreamWriterTest() {
}

////////////////////////////////////////////////////////////////////////////////
void OutputStreamWriterTest::setUp() {

    this->buffer1 = new ByteArrayOutputStream();
    this->writer1 = new OutputStreamWriter( this->buffer1 );
    this->reader = NULL;
}

////////////////////////////////////////////////////////////////////////////////
void OutputStreamWriterTest::tearDown() {

    try{

        delete this->writer1;
        delete this->buffer1;
        delete this->reader;

    } catch(...) {}
}

////////////////////////////////////////////////////////////////////////////////
void OutputStreamWriterTest::testClose() {

    this->writer1->flush();
    this->writer1->close();

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IOException",
        this->writer1->flush(),
        IOException );
}

////////////////////////////////////////////////////////////////////////////////
void OutputStreamWriterTest::testFlush() {

    this->writer1->write( TEST_STRING );
    this->writer1->flush();

    std::string result = this->buffer1->toString();
    CPPUNIT_ASSERT_EQUAL( TEST_STRING, result );
}

////////////////////////////////////////////////////////////////////////////////
void OutputStreamWriterTest::testWriteCharArrayIntIntInt() {

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an NullPointerException",
        this->writer1->write( NULL, 0, 1, 1 ),
        NullPointerException );

    this->writer1->write( TEST_STRING.c_str(), 1, 2 );
    this->writer1->flush();

    CPPUNIT_ASSERT_EQUAL( std::string("es"), this->buffer1->toString() );

    this->writer1->write( TEST_STRING.c_str(), 0, (int)TEST_STRING.length() );
    this->writer1->flush();

    CPPUNIT_ASSERT_EQUAL( std::string("es") + TEST_STRING, this->buffer1->toString() );

    this->writer1->close();

    // After the stream is closed, should throw IOException first
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IOException",
        this->writer1->write( NULL, 0, 0, 10 ),
        IOException );
}

////////////////////////////////////////////////////////////////////////////////
void OutputStreamWriterTest::testWriteChar() {

    this->writer1->write( 'a' );
    this->writer1->flush();
    CPPUNIT_ASSERT_EQUAL( std::string( "a" ), this->buffer1->toString() );

    this->writer1->write( 'b' );
    this->writer1->flush();
    CPPUNIT_ASSERT_EQUAL( std::string( "ab" ), this->buffer1->toString() );

    this->writer1->write( 'c' );
    this->writer1->flush();
    CPPUNIT_ASSERT_EQUAL( std::string( "abc" ), this->buffer1->toString() );

    this->writer1->close();

    // After the stream is closed, should throw IOException first
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IOException",
        this->writer1->write( 'd' ),
        IOException );
}

////////////////////////////////////////////////////////////////////////////////
void OutputStreamWriterTest::testWriteStringIntInt() {

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IndexOutOfBoundsException",
        this->writer1->write( string( "" ), 0, 1 ),
        IndexOutOfBoundsException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IndexOutOfBoundsException",
        this->writer1->write( string( "abc" ), 1, 3 ),
        IndexOutOfBoundsException );

    this->writer1->write( "abc", 1, 2 );
    this->writer1->flush();
    CPPUNIT_ASSERT_EQUAL( std::string( "bc" ), this->buffer1->toString() );

    this->writer1->write( TEST_STRING, 0, (int)TEST_STRING.length() );
    this->writer1->flush();
    CPPUNIT_ASSERT_EQUAL( std::string( "bc" ) + TEST_STRING, this->buffer1->toString() );

    this->writer1->close();

    // After the stream is closed, should throw IOException first
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IOException",
        this->writer1->write( "abcdefg", 0, 3 ),
        IOException );
}

////////////////////////////////////////////////////////////////////////////////
void OutputStreamWriterTest::testOutputStreamWriterOutputStream() {

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an NullPointerException",
        OutputStreamWriter( NULL ),
        NullPointerException );

    OutputStreamWriter* writer2 = new OutputStreamWriter( this->buffer1 );
    writer2->close();
    delete writer2;
}

////////////////////////////////////////////////////////////////////////////////
void OutputStreamWriterTest::testWriteString() {

    this->writer1->write( "abc" );
    this->writer1->flush();
    CPPUNIT_ASSERT_EQUAL( std::string( "abc" ), this->buffer1->toString() );

    this->writer1->write( TEST_STRING, 0, (int)TEST_STRING.length() );
    this->writer1->flush();
    CPPUNIT_ASSERT_EQUAL( std::string( "abc" ) + TEST_STRING, this->buffer1->toString() );

    this->writer1->close();

    // After the stream is closed, should throw IOException first
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IOException",
        this->writer1->write( TEST_STRING ),
        IOException );
}

////////////////////////////////////////////////////////////////////////////////
void OutputStreamWriterTest::openInputStream() {
    std::pair<const unsigned char*, int> array = this->buffer1->toByteArray();
    this->reader = new InputStreamReader(
        new ByteArrayInputStream( array.first, array.second, true ), true );
}
