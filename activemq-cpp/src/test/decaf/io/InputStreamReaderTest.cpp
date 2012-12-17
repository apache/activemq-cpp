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

#include "InputStreamReaderTest.h"

#include <decaf/io/Reader.h>
#include <decaf/lang/exceptions/IndexOutOfBoundsException.h>

#include <vector>

using namespace std;
using namespace decaf;
using namespace decaf::io;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
const std::string InputStreamReaderTest::TEST_STRING = "This is a test message with some simple text in it.";

////////////////////////////////////////////////////////////////////////////////
InputStreamReaderTest::InputStreamReaderTest() : buffer1(), reader1() {
}

////////////////////////////////////////////////////////////////////////////////
InputStreamReaderTest::~InputStreamReaderTest() {
}

////////////////////////////////////////////////////////////////////////////////
void InputStreamReaderTest::setUp() {

    this->buffer1 = new ByteArrayInputStream( (unsigned char*)TEST_STRING.c_str(), (int)TEST_STRING.length() );
    this->reader1 = new InputStreamReader( this->buffer1 );
}

////////////////////////////////////////////////////////////////////////////////
void InputStreamReaderTest::tearDown() {

    try{
        delete this->reader1;
        delete this->buffer1;
    } catch(...) {}
}

////////////////////////////////////////////////////////////////////////////////
void InputStreamReaderTest::testClose() {

    this->reader1->close();

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IOException",
        this->reader1->read(),
        IOException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IOException",
        this->reader1->ready(),
        IOException );

    CPPUNIT_ASSERT_NO_THROW( this->reader1->close() );
}

////////////////////////////////////////////////////////////////////////////////
void InputStreamReaderTest::testConstructorInputStream() {

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an NullPointerException",
        InputStreamReader( NULL ),
        NullPointerException );

    InputStreamReader* reader2 = new InputStreamReader( this->buffer1 );
    reader2->close();
    delete reader2;
}

////////////////////////////////////////////////////////////////////////////////
void InputStreamReaderTest::testRead() {

    CPPUNIT_ASSERT_EQUAL( 'T', (char) this->reader1->read() );
    CPPUNIT_ASSERT_EQUAL( 'h', (char) this->reader1->read() );
    CPPUNIT_ASSERT_EQUAL( 'i', (char) this->reader1->read() );
    CPPUNIT_ASSERT_EQUAL( 's', (char) this->reader1->read() );
    CPPUNIT_ASSERT_EQUAL( ' ', (char) this->reader1->read() );

    std::vector<char> buffer( TEST_STRING.length() - 5 );
    this->reader1->read( &buffer[0], (int)buffer.size(), 0, (int)TEST_STRING.length() - 5 );
    CPPUNIT_ASSERT_EQUAL( -1, this->reader1->read() );

    this->reader1->close();

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IOException",
        this->reader1->read(),
        IOException );
}

////////////////////////////////////////////////////////////////////////////////
void InputStreamReaderTest::testReady() {

    CPPUNIT_ASSERT_MESSAGE( "Ready test failed", this->reader1->ready() );
    this->reader1->read();
    CPPUNIT_ASSERT_MESSAGE("More chars, but not ready", this->reader1->ready());

    CPPUNIT_ASSERT( this->reader1->ready() );
    std::vector<char> buffer( TEST_STRING.length() - 1 );
    this->reader1->read( buffer );
    CPPUNIT_ASSERT( !this->reader1->ready() );
}
