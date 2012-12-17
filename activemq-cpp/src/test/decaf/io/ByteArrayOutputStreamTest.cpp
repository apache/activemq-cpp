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

#include "ByteArrayOutputStreamTest.h"

#ifdef HAVE_STRING_H
#include <string.h>
#endif
#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif

using namespace std;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::io;
using namespace decaf::util;

////////////////////////////////////////////////////////////////////////////////
void ByteArrayOutputStreamTest::testConstructor1() {
    ByteArrayOutputStream baos( 500 );
    CPPUNIT_ASSERT_MESSAGE("Failed to create stream", 0 == baos.size() );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayOutputStreamTest::testConstructor2() {
    ByteArrayOutputStream baos;
    CPPUNIT_ASSERT_MESSAGE("Failed to create stream", 0 == baos.size() );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayOutputStreamTest::testClose() {

    CPPUNIT_ASSERT_MESSAGE(
        "close() does nothing for this implementation of OutputSteam",
        true );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayOutputStreamTest::testReset() {
    ByteArrayOutputStream baos;
    baos.write( (unsigned char*)&testString[0], (int)testString.size(), 0, 100 );
    baos.reset();
    CPPUNIT_ASSERT_MESSAGE("reset failed", 0 == baos.size() );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayOutputStreamTest::testSize() {
    ByteArrayOutputStream baos;
    baos.write( (unsigned char*)&testString[0], (int)testString.size(), 0, 100 );
    CPPUNIT_ASSERT_MESSAGE("size test failed", 100 == baos.size());
    baos.reset();
    CPPUNIT_ASSERT_MESSAGE("size test failed", 0 == baos.size());
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayOutputStreamTest::testToByteArray() {
    ByteArrayOutputStream baos;
    baos.write( (unsigned char*)&testString[0], (int)testString.size(), 0, (int)testString.length() );
    std::pair<const unsigned char*, int> array = baos.toByteArray();
    for( std::size_t i = 0; i < testString.length(); i++) {
        CPPUNIT_ASSERT_MESSAGE("Error in byte array", array.first[i] == testString.at(i) );
    }
    delete [] array.first;
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayOutputStreamTest::testToString() {

    ByteArrayOutputStream baos;
    baos.write( (unsigned char*)&testString[0], (int)testString.size(), 0, (int)testString.length() );
    CPPUNIT_ASSERT_MESSAGE( "Returned incorrect String",
                            baos.toString() == testString );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayOutputStreamTest::testWrite1() {

    ByteArrayOutputStream baos;
    baos.write('t');
    std::pair<const unsigned char*, int> array = baos.toByteArray();
    CPPUNIT_ASSERT_MESSAGE( "Wrote incorrect bytes",
                            string("t") == string( (const char*)array.first, array.second ) );
    delete [] array.first;
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayOutputStreamTest::testWrite2() {
    ByteArrayOutputStream baos;
    baos.write( (unsigned char*)&testString[0], (int)testString.size(), 0, 100 );
    std::pair<const unsigned char*, int> array = baos.toByteArray();
    CPPUNIT_ASSERT_MESSAGE("Wrote incorrect bytes",
            string((const char*)array.first, array.second ) == testString.substr(0, 100) );
    delete [] array.first;
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayOutputStreamTest::testWrite3() {
    ByteArrayOutputStream baos;
    baos.write( (unsigned char*)&testString[0], (int)testString.size(), 50, 100 );
    std::pair<const unsigned char*, int> array = baos.toByteArray();
    CPPUNIT_ASSERT_MESSAGE("Wrote incorrect bytes",
            string((const char*)array.first, array.second ) == testString.substr(50, 100) );
    delete [] array.first;
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayOutputStreamTest::testWriteToDecaf_io_OutputStream() {
    ByteArrayOutputStream baos1;
    ByteArrayOutputStream baos2;
    baos1.write( (unsigned char*)&testString[0], (int)testString.size(), 0, 100 );
    baos1.writeTo( &baos2 );
    CPPUNIT_ASSERT_MESSAGE( "Returned incorrect String",
                            baos2.toString() == testString.substr(0, 100) );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayOutputStreamTest::testStream()
{
    ByteArrayOutputStream stream_a;

    stream_a.write('a');
    stream_a.write(60);
    stream_a.write('c');

    CPPUNIT_ASSERT( stream_a.size() == 3 );

    stream_a.reset();

    CPPUNIT_ASSERT( stream_a.size() == 0 );

    stream_a.write((const unsigned char*)("abc"), 3, 0, 3);

    CPPUNIT_ASSERT( stream_a.size() == 3 );

    stream_a.reset();

    CPPUNIT_ASSERT( stream_a.size() == 0 );

    stream_a.write((const unsigned char*)("abc"), 3, 0, 3);

    unsigned char buffer[4];

    memset(buffer, 0, 4);
    std::pair<const unsigned char*, int> array = stream_a.toByteArray();
    memcpy(buffer, array.first, array.second );
    delete [] array.first;

    CPPUNIT_ASSERT( std::string((const char*)buffer) == std::string("abc") );
}
