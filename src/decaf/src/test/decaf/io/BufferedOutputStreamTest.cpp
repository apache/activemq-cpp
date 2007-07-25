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

#include "BufferedOutputStreamTest.h"

using namespace std;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::io;
using namespace decaf::util;

void BufferedOutputStreamTest::testSmallerBuffer(){

    MyOutputStream myStream;
    BufferedOutputStream bufStream( &myStream, (std::size_t)1 );

    const char* buffer = myStream.getBuffer();

    bufStream.write( (unsigned char)'T' );
    // Should not be written yet.
    CPPUNIT_ASSERT( strcmp( buffer, "" ) == 0 );

    bufStream.write( (unsigned char)'E' );
    // This time the T should have been written.
    CPPUNIT_ASSERT( strcmp( buffer, "T" ) == 0 );

    bufStream.write( (unsigned char*)"ST", 2 );
    // This time the ES should have been written.
    CPPUNIT_ASSERT( strcmp( buffer, "TES" ) == 0 );

    bufStream.flush();
    CPPUNIT_ASSERT( strcmp( buffer, "TEST" ) == 0 );
}

void BufferedOutputStreamTest::testBiggerBuffer(){

    MyOutputStream myStream;
    BufferedOutputStream bufStream( &myStream, (std::size_t)10 );

    const char* buffer = myStream.getBuffer();

    bufStream.write( (unsigned char*)"TEST", 4 );

    // Should not be written yet.
    CPPUNIT_ASSERT( strcmp( buffer, "" ) == 0 );

    bufStream.flush();
    CPPUNIT_ASSERT( strcmp( buffer, "TEST" ) == 0 );

    bufStream.write( (unsigned char*)"TEST", 4 );
    bufStream.write( (unsigned char*)"12345678910", 11);

    CPPUNIT_ASSERT( strcmp( buffer, "TESTTEST123456" ) == 0 );

    bufStream.flush();
    CPPUNIT_ASSERT( strcmp( buffer, "TESTTEST12345678910" ) == 0 );

}
