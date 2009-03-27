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

#include "FilterInputStreamTest.h"
#include <decaf/io/ByteArrayInputStream.h>

using namespace std;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::io;
using namespace decaf::util;

////////////////////////////////////////////////////////////////////////////////
void FilterInputStreamTest::testAvailable() {

    std::string testStr = "TEST12345678910";
    MyInputStream myStream( testStr );
    FilterInputStream is( &myStream );

    CPPUNIT_ASSERT_MESSAGE( "Returned incorrect number of available bytes",
                            is.available() == testStr.length() );
}

////////////////////////////////////////////////////////////////////////////////
void FilterInputStreamTest::testClose() {

    std::string testStr = "TEST12345678910";
    MyInputStream myStream( testStr );
    FilterInputStream is( &myStream );

    try {
        is.close();
    } catch( IOException& e ) {
        CPPUNIT_FAIL("Exception attempting to close stream");
    }

    try {
        is.read();
    } catch( IOException& e ) {
        return;
    }

    CPPUNIT_FAIL("Able to read from closed stream");
}

////////////////////////////////////////////////////////////////////////////////
void FilterInputStreamTest::testRead() {

    std::string testStr = "TEST12345678910";
    MyInputStream myStream( testStr );
    FilterInputStream is( &myStream );

    char c = is.read();
    CPPUNIT_ASSERT_MESSAGE( "read returned incorrect char",
                            c == testStr.at(0) );
}

////////////////////////////////////////////////////////////////////////////////
void FilterInputStreamTest::testRead2() {

    std::string testStr = "TEST12345678910ABCDEFGHIJKLMNOPQRSTU";
    MyInputStream myStream( testStr );
    FilterInputStream is( &myStream );

    unsigned char buf[30];
    is.read( buf, 0, 30 );
    CPPUNIT_ASSERT_MESSAGE( "Failed to read correct data",
        string( (const char*)buf, 30 ) == testStr.substr(0, 30) );
}

////////////////////////////////////////////////////////////////////////////////
void FilterInputStreamTest::testRead3() {

    std::string testStr;
    for( int i = 0; i < 4000; ++i ) {
        testStr += (char)i;
    }
    MyInputStream myStream( testStr );
    FilterInputStream is( &myStream );

    unsigned char buf[100];
    is.skip(3000);
    is.read( buf, 0, 100 );
    CPPUNIT_ASSERT_MESSAGE( "Failed to read correct data",
        string( (const char*)buf, 100 ) == testStr.substr( 3000, 100 ) );
}

////////////////////////////////////////////////////////////////////////////////
void FilterInputStreamTest::testSkip() {

    std::string testStr;
    for( int i = 0; i < 4000; ++i ) {
        testStr += (char)i;
    }
    MyInputStream myStream( testStr );
    FilterInputStream is( &myStream );

    unsigned char buf[100];
    is.skip( 1000 );
    is.read( buf, 0, 100 );
    CPPUNIT_ASSERT_MESSAGE( "Failed to skip to correct position",
            string( (const char*)buf, 100 ) == testStr.substr( 1000, 100 ) );
}
