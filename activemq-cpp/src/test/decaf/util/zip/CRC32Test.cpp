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

#include "CRC32Test.h"

#include <decaf/util/zip/CRC32.h>
#include <decaf/lang/Integer.h>
#include <decaf/lang/Long.h>

#include <vector>

using namespace std;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::util;
using namespace decaf::util::zip;

////////////////////////////////////////////////////////////////////////////////
CRC32Test::CRC32Test() {
}

////////////////////////////////////////////////////////////////////////////////
CRC32Test::~CRC32Test() {
}

////////////////////////////////////////////////////////////////////////////////
void CRC32Test::testConstructor() {

    CRC32 crc;
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "Constructor of CRC32 failed", 0LL, crc.getValue() );
}

////////////////////////////////////////////////////////////////////////////////
void CRC32Test::testGetValue() {

    // test methods of java.util.zip.crc32.getValue()
    CRC32 crc;
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "getValue() should return a zero as a result of constructing a CRC32 instance",
                                  0LL, crc.getValue() );

    crc.reset();
    crc.update( Integer::MAX_VALUE );

    // Ran JDK and discovered that the value of the CRC should be
    // 4278190080
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "update(max) failed to update the checksum to the correct value ",
                                  4278190080LL, crc.getValue() );

    crc.reset();
    std::vector<unsigned char> byteEmpty( 10000, 0 );
    crc.update( byteEmpty );

    // Ran JDK and discovered that the value of the CRC should be
    // 1295764014
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "update(byte[]) failed to update the checksum to the correct value ",
                                  1295764014LL, crc.getValue() );

    crc.reset();
    crc.update( 1 );

    // Ran JDK and discovered that the value of the CRC should be
    // 2768625435
    // CPPUNIT_ASSERT_EQUAL_MESSAGE( "update(int) failed to update the checksum to the correct
    // value ",2768625435L, crc.getValue());
    crc.reset();
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "reset failed to reset the checksum value to zero",
                                  0LL, crc.getValue());
}

////////////////////////////////////////////////////////////////////////////////
void CRC32Test::testReset() {

    CRC32 crc;
    crc.update( 1 );

    // Ran JDK and discovered that the value of the CRC should be
    // 2768625435
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "update(int) failed to update the checksum to the correct value ",
                                  2768625435LL, crc.getValue() );
    crc.reset();
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "reset failed to reset the checksum value to zero",
                                  0LL, crc.getValue() );
}

////////////////////////////////////////////////////////////////////////////////
void CRC32Test::testUpdateI() {

    CRC32 crc;
    crc.update( 1 );

    // Ran JDK and discovered that the value of the CRC should be
    // 2768625435
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "update(1) failed to update the checksum to the correct value ",
                                  2768625435LL, crc.getValue() );

    crc.reset();
    crc.update( Integer::MAX_VALUE );

    // Ran JDK and discovered that the value of the CRC should be
    // 4278190080
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "update(max) failed to update the checksum to the correct value ",
                                  4278190080LL, crc.getValue() );

    crc.reset();
    crc.update( Integer::MIN_VALUE );

    // Ran JDK and discovered that the value of the CRC should be
    // 3523407757
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "update(min) failed to update the checksum to the correct value ",
                                  3523407757LL, crc.getValue() );
}

////////////////////////////////////////////////////////////////////////////////
void CRC32Test::testUpdateArray() {

    unsigned char byteArray[] = { 1, 2 };
    CRC32 crc;
    crc.update( byteArray, 2, 0, 2 );

    // Ran JDK and discovered that the value of the CRC should be
    // 3066839698
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "update(unsigned char[]) failed to update the checksum to the correct value ",
                                  3066839698LL, crc.getValue() );

    crc.reset();
    std::vector<unsigned char> byteEmpty( 10000, 0 );
    crc.update( byteEmpty );

    // Ran JDK and discovered that the value of the CRC should be
    // 1295764014
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "update(unsigned char[]) failed to update the checksum to the correct value ",
                                  1295764014LL, crc.getValue() );
}

////////////////////////////////////////////////////////////////////////////////
void CRC32Test::testUpdateArrayIndexed() {

    static const int SIZE = 3;
    unsigned char byteArray[] = {1, 2, 3};
    CRC32 crc;

    int off = 2;// accessing the 2nd element of byteArray
    int len = 1;
    int lenError = 3;
    int offError = 4;
    crc.update( byteArray, SIZE, off, len );

    // Ran JDK and discovered that the value of the CRC should be
    // 1259060791
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "update(unsigned char[],int,int) failed to update the checksum to the correct value ",
                                  1259060791LL, crc.getValue() );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an IndexOutOfBoundsException for lenError",
        crc.update( byteArray, SIZE, off, lenError ),
        IndexOutOfBoundsException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an IndexOutOfBoundsException for offError",
        crc.update( byteArray, SIZE, offError, len ),
        IndexOutOfBoundsException );
}
