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

#include "BufferedInputStreamTest.h"

using namespace std;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::io;
using namespace decaf::util;

void BufferedInputStreamTest::testSmallerBuffer(){

    std::string testStr = "TEST12345678910";
    MyInputStream myStream( testStr );
    BufferedInputStream bufStream( &myStream, (std::size_t)1 );

    std::size_t available = bufStream.available();
    CPPUNIT_ASSERT( available == testStr.length() );

    unsigned char dummy = bufStream.read();
    CPPUNIT_ASSERT( dummy == 'T' );

    available = bufStream.available();
    CPPUNIT_ASSERT( available == (testStr.length() - 1) );

    dummy = bufStream.read();
    CPPUNIT_ASSERT( dummy == 'E' );

    available = bufStream.available();
    CPPUNIT_ASSERT( available == (testStr.length() - 2 ) );

    dummy = bufStream.read();
    CPPUNIT_ASSERT( dummy == 'S' );

    available = bufStream.available();
    CPPUNIT_ASSERT( available == (testStr.length() - 3 ) );

    dummy = bufStream.read();
    CPPUNIT_ASSERT( dummy == 'T' );

    unsigned char dummyBuf[20];
    memset( dummyBuf, 0, 20 );
    std::size_t numRead = bufStream.read( dummyBuf, 10 );
    CPPUNIT_ASSERT( numRead == 10 );
    CPPUNIT_ASSERT( strcmp( (char*)dummyBuf, "1234567891" ) == 0 );

    available = bufStream.available();
    CPPUNIT_ASSERT( available == 1 );
}

void BufferedInputStreamTest::testBiggerBuffer(){

    std::string testStr = "TEST12345678910";
    MyInputStream myStream( testStr );
    BufferedInputStream bufStream( &myStream, (std::size_t)10 );

    std::size_t available = bufStream.available();
    CPPUNIT_ASSERT( available == testStr.length() );

    unsigned char dummy = bufStream.read();
    CPPUNIT_ASSERT( dummy == 'T' );

    available = bufStream.available();
    CPPUNIT_ASSERT( available == (testStr.length() - 1 ) );

    dummy = bufStream.read();
    CPPUNIT_ASSERT( dummy == 'E' );

    available = bufStream.available();
    CPPUNIT_ASSERT( available == (testStr.length() - 2 ) );

    dummy = bufStream.read();
    CPPUNIT_ASSERT( dummy == 'S' );

    available = bufStream.available();
    CPPUNIT_ASSERT( available == (testStr.length() - 3 ) );

    dummy = bufStream.read();
    CPPUNIT_ASSERT( dummy == 'T' );

    unsigned char dummyBuf[20];
    memset( dummyBuf, 0, 20 );
    std::size_t numRead = bufStream.read( dummyBuf, 10 );
    CPPUNIT_ASSERT( numRead == 10 );
    CPPUNIT_ASSERT( strcmp( (char*)dummyBuf, "1234567891" ) == 0 );

    available = bufStream.available();
    CPPUNIT_ASSERT( available == 1 );
}
