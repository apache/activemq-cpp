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

#include "CheckedInputStreamTest.h"

#include <decaf/util/zip/Deflater.h>
#include <decaf/util/zip/CheckedOutputStream.h>
#include <decaf/util/zip/CheckedInputStream.h>
#include <decaf/util/zip/Adler32.h>
#include <decaf/util/zip/CRC32.h>
#include <decaf/io/ByteArrayOutputStream.h>
#include <decaf/io/ByteArrayInputStream.h>

#include <vector>

using namespace std;
using namespace decaf;
using namespace decaf::io;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::util;
using namespace decaf::util::zip;

////////////////////////////////////////////////////////////////////////////////
CheckedInputStreamTest::CheckedInputStreamTest() {
}

////////////////////////////////////////////////////////////////////////////////
CheckedInputStreamTest::~CheckedInputStreamTest() {
}

////////////////////////////////////////////////////////////////////////////////
void CheckedInputStreamTest::testConstructor() {

    std::vector<unsigned char> outPutBuf;
    ByteArrayInputStream baos( outPutBuf );
    CRC32 check;
    CheckedInputStream chkIn( &baos, &check );
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "the checkSum value of the constructor is not 0",
                                  0LL, chkIn.getChecksum()->getValue() );
}

////////////////////////////////////////////////////////////////////////////////
void CheckedInputStreamTest::testGetChecksum() {

    std::vector<unsigned char> emptyBuf;
    ByteArrayInputStream baos( emptyBuf );
    CRC32 check;
    CheckedInputStream checkEmpty( &baos, &check );

    while( checkEmpty.read() >= 0 ) {
    }
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "the checkSum value of an empty file is not zero",
                                  0LL, checkEmpty.getChecksum()->getValue() );

    static const int SIZE = 10;
    unsigned char byteArray[] = { 1, 3, 4, 7, 8, 'e', 'r', 't', 'y', '5' };

    std::vector<unsigned char> outPutBuf( 500 );

    Deflater deflater;
    deflater.setInput( byteArray, SIZE, 0, SIZE );
    deflater.finish();
    while( !deflater.finished() ) {
        deflater.deflate( outPutBuf );
    }

    ByteArrayInputStream bais( outPutBuf );
    Adler32 adler;
    CheckedInputStream checkIn( &bais, &adler );
    while( checkIn.read() >= 0 ) {
    }

    CPPUNIT_ASSERT_MESSAGE( "the checksum value is incorrect",
                            checkIn.getChecksum()->getValue() > 0 );
}

////////////////////////////////////////////////////////////////////////////////
void CheckedInputStreamTest::testSkip() {

    static const int SIZE = 256;
    std::vector<unsigned char> byteArray( SIZE );
    for( int i = 0; i < SIZE; ++i ) {
        byteArray[i] = (unsigned char)i;
    }
    std::vector<unsigned char> outPutBuf( 500 );

    Deflater deflater;
    deflater.setInput( &byteArray[0], SIZE, 0, SIZE );
    deflater.finish();
    while( !deflater.finished() ) {
        deflater.deflate( outPutBuf );
    }

    ByteArrayInputStream bais( outPutBuf );
    Adler32 adler;
    CheckedInputStream checkIn( &bais, &adler );

    long long skipValue = 5;
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "the value returned by skip(n) is not the same as its parameter",
                                  skipValue, checkIn.skip( skipValue ) );
    checkIn.skip( skipValue );

    CPPUNIT_ASSERT_MESSAGE( "checkSum value is not correct", checkIn.getChecksum()->getValue() > 0 );
}

////////////////////////////////////////////////////////////////////////////////
void CheckedInputStreamTest::testRead() {

    static const int SIZE = 256;
    std::vector<unsigned char> byteArray( SIZE );
    for( int i = 0; i < SIZE; ++i ) {
        byteArray[i] = (unsigned char)i;
    }
    std::vector<unsigned char> outPutBuf( 500 );

    Deflater deflater;
    deflater.setInput( &byteArray[0], SIZE, 0, SIZE );
    deflater.finish();
    while( !deflater.finished() ) {
        deflater.deflate( outPutBuf );
    }

    ByteArrayInputStream bais( outPutBuf );
    Adler32 adler;
    CheckedInputStream checkIn( &bais, &adler );

    checkIn.read();
    checkIn.close();

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an IOException",
        checkIn.read(),
        IOException );
}

////////////////////////////////////////////////////////////////////////////////
void CheckedInputStreamTest::testReadBIII() {

    static const int SIZE = 256;
    std::vector<unsigned char> byteArray( SIZE );
    for( int i = 0; i < SIZE; ++i ) {
        byteArray[i] = (unsigned char)i;
    }
    std::vector<unsigned char> outPutBuf( 500 );

    Deflater deflater;
    deflater.setInput( &byteArray[0], SIZE, 0, SIZE );
    deflater.finish();
    while( !deflater.finished() ) {
        deflater.deflate( outPutBuf );
    }

    ByteArrayInputStream bais( outPutBuf );
    Adler32 adler;
    CheckedInputStream checkIn( &bais, &adler );

    unsigned char buff[50];
    checkIn.read( buff, 50, 10, 5 );
    checkIn.close();

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an IOException",
        checkIn.read( buff, 50, 10, 5 ),
        IOException );
}
