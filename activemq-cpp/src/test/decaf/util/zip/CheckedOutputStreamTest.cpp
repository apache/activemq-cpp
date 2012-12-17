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

#include "CheckedOutputStreamTest.h"

#include <decaf/util/zip/CheckedOutputStream.h>
#include <decaf/util/zip/Adler32.h>
#include <decaf/util/zip/CRC32.h>
#include <decaf/io/ByteArrayOutputStream.h>

using namespace std;
using namespace decaf;
using namespace decaf::io;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::util;
using namespace decaf::util::zip;

////////////////////////////////////////////////////////////////////////////////
CheckedOutputStreamTest::CheckedOutputStreamTest() {
}

////////////////////////////////////////////////////////////////////////////////
CheckedOutputStreamTest::~CheckedOutputStreamTest() {
}

////////////////////////////////////////////////////////////////////////////////
void CheckedOutputStreamTest::testConstructor() {

    ByteArrayOutputStream baos;
    CRC32 check;
    CheckedOutputStream chkOut( &baos, &check );
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "the checkSum value of the constructor is not 0",
                                  0LL, chkOut.getChecksum()->getValue() );
}

////////////////////////////////////////////////////////////////////////////////
void CheckedOutputStreamTest::testGetChecksum() {

    unsigned char byteArray[] = { 1, 2, 3, 'e', 'r', 't', 'g', 3, 6 };

    ByteArrayOutputStream baos;
    Adler32 check;
    CheckedOutputStream chkOut( &baos, &check );

    chkOut.write( byteArray[4] );
    // ran JDK and found that checkSum value is 7536755

    CPPUNIT_ASSERT_EQUAL_MESSAGE( "the checkSum value for writeI is incorrect",
                                  7536755LL, chkOut.getChecksum()->getValue());

    chkOut.getChecksum()->reset();
    chkOut.write( byteArray, 9, 5, 4 );
    // ran JDK and found that checkSum value is 51708133

    CPPUNIT_ASSERT_EQUAL_MESSAGE( "the checkSum value for writeBII is incorrect ",
                                  51708133LL, chkOut.getChecksum()->getValue() );
}

////////////////////////////////////////////////////////////////////////////////
void CheckedOutputStreamTest::testWriteI() {

    static const int SIZE = 9;
    unsigned char byteArray[] = { 1, 2, 3, 'e', 'r', 't', 'g', 3, 6 };

    ByteArrayOutputStream baos;
    CRC32 check;
    CheckedOutputStream chkOut( &baos, &check );

    for( int ix = 0; ix < SIZE; ++ix ) {
        chkOut.write( byteArray[ix] );
    }

    CPPUNIT_ASSERT_MESSAGE(
        "the checkSum value is zero, no bytes are written to the output file",
        chkOut.getChecksum()->getValue() != 0 );
}

////////////////////////////////////////////////////////////////////////////////
void CheckedOutputStreamTest::testWriteBIII() {

    static const int SIZE = 9;
    unsigned char byteArray[] = { 1, 2, 3, 'e', 'r', 't', 'g', 3, 6 };

    ByteArrayOutputStream baos;
    CRC32 check;
    CheckedOutputStream chkOut( &baos, &check );

    chkOut.write( byteArray, SIZE, 4, 5 );
    CPPUNIT_ASSERT_MESSAGE( "the checkSum value is zero, no bytes are written to the output file",
                            chkOut.getChecksum()->getValue() != 0 );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an IndexOutOfBoundsException",
        chkOut.write( byteArray, SIZE, 4, 6 ),
        IndexOutOfBoundsException );
}
