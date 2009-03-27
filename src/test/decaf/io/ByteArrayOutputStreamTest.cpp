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

using namespace std;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::io;
using namespace decaf::util;

////////////////////////////////////////////////////////////////////////////////
void ByteArrayOutputStreamTest::testConstructor1() {
    std::vector<unsigned char> buffer;
    ByteArrayOutputStream baos( buffer );
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
    baos.write( (unsigned char*)&testString[0], 0, 100 );
    baos.reset();
    CPPUNIT_ASSERT_MESSAGE("reset failed", 0 == baos.size() );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayOutputStreamTest::testSize() {
    ByteArrayOutputStream baos;
    baos.write( (unsigned char*)&testString[0], 0, 100 );
    CPPUNIT_ASSERT_MESSAGE("size test failed", 100 == baos.size());
    baos.reset();
    CPPUNIT_ASSERT_MESSAGE("size test failed", 0 == baos.size());
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayOutputStreamTest::testToByteArray() {
    const unsigned char* bytes = NULL;
    ByteArrayOutputStream baos;
    baos.write( (unsigned char*)&testString[0], 0, testString.length() );
    bytes = baos.toByteArray();
    for( std::size_t i = 0; i < testString.length(); i++) {
        CPPUNIT_ASSERT_MESSAGE("Error in byte array", bytes[i] == testString.at(i) );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayOutputStreamTest::testToString() {

    ByteArrayOutputStream baos;
    baos.write( (unsigned char*)&testString[0], 0, testString.length() );
    CPPUNIT_ASSERT_MESSAGE( "Returned incorrect String",
                            baos.toString() == testString );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayOutputStreamTest::testWrite1() {

    ByteArrayOutputStream baos;
    baos.write('t');
    const unsigned char* bytes = baos.toByteArray();
    CPPUNIT_ASSERT_MESSAGE( "Wrote incorrect bytes",
                            string("t") == string( (const char*)bytes, baos.size() ) );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayOutputStreamTest::testWrite2() {
    ByteArrayOutputStream baos;
    baos.write( (unsigned char*)&testString[0], 0, 100 );
    const unsigned char* bytes = baos.toByteArray();
    CPPUNIT_ASSERT_MESSAGE("Wrote incorrect bytes",
            string((const char*)bytes, baos.size() ) == testString.substr(0, 100) );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayOutputStreamTest::testWrite3() {
    ByteArrayOutputStream baos;
    baos.write( (unsigned char*)&testString[0], 50, 100 );
    const unsigned char* bytes = baos.toByteArray();
    CPPUNIT_ASSERT_MESSAGE("Wrote incorrect bytes",
            string((const char*)bytes, baos.size() ) == testString.substr(50, 100) );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayOutputStreamTest::testWriteToDecaf_io_OutputStream() {
    ByteArrayOutputStream baos1;
    ByteArrayOutputStream baos2;
    baos1.write( (unsigned char*)&testString[0], 0, 100 );
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

    stream_a.write((const unsigned char*)("abc"), 0, 3);

    CPPUNIT_ASSERT( stream_a.size() == 3 );

    stream_a.reset();

    CPPUNIT_ASSERT( stream_a.size() == 0 );

    stream_a.write((const unsigned char*)("abc"), 0, 3);

    unsigned char buffer[4];

    memset(buffer, 0, 4);
    memcpy(buffer, stream_a.toByteArray(), stream_a.size());

    CPPUNIT_ASSERT( std::string((const char*)buffer) == std::string("abc") );
}
