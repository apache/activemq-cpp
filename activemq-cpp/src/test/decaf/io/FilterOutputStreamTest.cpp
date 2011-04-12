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

#include "FilterOutputStreamTest.h"
#include <decaf/io/ByteArrayOutputStream.h>
#include <decaf/io/ByteArrayInputStream.h>

using namespace std;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::io;
using namespace decaf::util;

////////////////////////////////////////////////////////////////////////////////
void FilterOutputStreamTest::testConstructor() {

    try {
        ByteArrayOutputStream baos;
        FilterOutputStream os( &baos );
        os.write( 't' );
    } catch( IOException& e ) {
        CPPUNIT_FAIL("Constructor test failed : " + e.getMessage());
    }
}

////////////////////////////////////////////////////////////////////////////////
void FilterOutputStreamTest::testClose() {

    try {
        ByteArrayOutputStream baos;
        FilterOutputStream os( &baos );
        os.write( (unsigned char*)&testString[0], (int)testString.size(), 0, 500 );
        os.flush();
        CPPUNIT_ASSERT_MESSAGE( "Bytes not written after flush",
                                500 == baos.size() );
        os.close();
    } catch( IOException& e ) {
        CPPUNIT_FAIL("Close test failed : " + e.getMessage());
    }
}

////////////////////////////////////////////////////////////////////////////////
void FilterOutputStreamTest::testFlush() {

    try {
        ByteArrayOutputStream baos;
        FilterOutputStream os( &baos );
        os.write( (unsigned char*)&testString[0], (int)testString.size(), 0, 500 );
        os.flush();
        CPPUNIT_ASSERT_MESSAGE( "Bytes not written after flush",
                                500 == baos.size() );
        os.close();
    } catch( IOException& e ) {
        CPPUNIT_FAIL("Flush test failed : " + e.getMessage());
    }
}

////////////////////////////////////////////////////////////////////////////////
void FilterOutputStreamTest::testWrite1() {

    try {
        ByteArrayOutputStream baos;
        FilterOutputStream os( &baos );
        os.write( (unsigned char*)&testString[0], (int)testString.size(), 0, (int)testString.size() );

        std::pair<const unsigned char*, int> array = baos.toByteArray();
        ByteArrayInputStream bais( array.first, array.second, true );
        os.flush();
        CPPUNIT_ASSERT_MESSAGE( "Bytes not written after flush",
                                bais.available() == (int)testString.length() );
        unsigned char* wbytes = new unsigned char[ testString.length() ];
        bais.read( wbytes, (int)testString.length(), 0, (int)testString.length() );
        CPPUNIT_ASSERT_MESSAGE("Incorrect bytes written",
            testString == string( (const char*)wbytes, testString.length() ) );

        delete [] wbytes;
    } catch( IOException& e ) {
        CPPUNIT_FAIL("Write test failed : " + e.getMessage());
    }
}

////////////////////////////////////////////////////////////////////////////////
void FilterOutputStreamTest::testWrite2() {

    try {
        ByteArrayOutputStream baos;
        FilterOutputStream os( &baos );
        os.write('t');
        std::pair<const unsigned char*, int> array = baos.toByteArray();
        ByteArrayInputStream bais( array.first, array.second, true );
        os.flush();
        CPPUNIT_ASSERT_MESSAGE( "Byte not written after flush", 1 == bais.available() );
        unsigned char wbytes[1];
        bais.read( wbytes, 1, 0, 1 );
        CPPUNIT_ASSERT_MESSAGE("Incorrect byte written", 't' == wbytes[0] );
    } catch( IOException& e ) {
        CPPUNIT_FAIL("Write test failed : " + e.getMessage());
    }
}

////////////////////////////////////////////////////////////////////////////////
void FilterOutputStreamTest::testWriteBIIIExceptions() {

    ByteArrayOutputStream baos;
    FilterOutputStream os( &baos );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an NullPointerException",
        os.write( NULL, 1000, 0, 1001 ),
        NullPointerException );

    unsigned char buffer[1000];

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an IndexOutOfBoundsException",
        os.write( buffer, 1000, 0, 1001 ),
        IndexOutOfBoundsException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an IndexOutOfBoundsException",
        os.write( buffer, 1000, 1001, 0 ),
        IndexOutOfBoundsException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an IndexOutOfBoundsException",
        os.write( buffer, 1000, 500, 501 ),
        IndexOutOfBoundsException );

    os.close();

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an IOException",
        os.write( buffer, 1000, 0, 100 ),
        IOException );
}
