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
        os.write( (unsigned char*)&testString[0], 0, 500 );
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
        os.write( (unsigned char*)&testString[0], 0, 500 );
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
        os.write( (unsigned char*)&testString[0], 0, testString.size() );
        ByteArrayInputStream bais( baos.toByteArray(), baos.size() );
        os.flush();
        CPPUNIT_ASSERT_MESSAGE( "Bytes not written after flush",
                                bais.available() == testString.length() );
        unsigned char* wbytes = new unsigned char[ testString.length() ];
        bais.read( wbytes, 0, testString.length() );
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
        ByteArrayInputStream bais( baos.toByteArray(), baos.size() );
        os.flush();
        CPPUNIT_ASSERT_MESSAGE( "Byte not written after flush", 1 == bais.available() );
        unsigned char wbytes[1];
        bais.read( wbytes, 0, 1 );
        CPPUNIT_ASSERT_MESSAGE("Incorrect byte written", 't' == wbytes[0] );
    } catch( IOException& e ) {
        CPPUNIT_FAIL("Write test failed : " + e.getMessage());
    }
}
