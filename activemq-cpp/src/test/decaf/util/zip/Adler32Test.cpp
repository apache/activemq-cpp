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

#include "Adler32Test.h"

#include <decaf/util/zip/Adler32.h>

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
Adler32Test::Adler32Test() {

}

////////////////////////////////////////////////////////////////////////////////
Adler32Test::~Adler32Test() {
}

////////////////////////////////////////////////////////////////////////////////
void Adler32Test::testConstructor() {
    Adler32 adl;
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "Constructor of adl32 failed", 1LL, adl.getValue() );
}

////////////////////////////////////////////////////////////////////////////////
void Adler32Test::testGetValue() {

    Adler32 adl;
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "GetValue should return a zero as a result of construction an object of Adler32",
                                  1LL, adl.getValue() );

    adl.reset();
    adl.update( 1 );

    // The value of the adl should be 131074
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "update(int) failed to update the checksum to the correct value ",
                                  131074LL, adl.getValue());
    adl.reset();
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "reset failed to reset the checksum value to zero",
                                  1LL, adl.getValue() );

    adl.reset();
    adl.update( Integer::MIN_VALUE );

    // The value of the adl should be 65537
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "update(min) failed to update the checksum to the correct value ",
                                  65537LL, adl.getValue() );
}

////////////////////////////////////////////////////////////////////////////////
void Adler32Test::testReset() {

    Adler32 adl;
    adl.update( 1 );

    // The value of the adl should be 131074
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "update(int) failed to update the checksum to the correct value ",
                                  131074LL, adl.getValue() );
    adl.reset();
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "reset failed to reset the checksum value to zero",
                                  1LL, adl.getValue() );
}

////////////////////////////////////////////////////////////////////////////////
void Adler32Test::testUpdateI() {

    Adler32 adl;
    adl.update( 1 );
    // The value of the adl should be 131074
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "update(int) failed to update the checksum to the correct value ",
                                  131074LL, adl.getValue() );

    adl.reset();
    adl.update( Integer::MAX_VALUE );

    // The value of the adl should be 16777472
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "update(max) failed to update the checksum to the correct value ",
                                  16777472LL, adl.getValue() );

    adl.reset();
    adl.update( Integer::MIN_VALUE );

    // The value of the adl should be 65537
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "update(min) failed to update the checksum to the correct value ",
                                  65537LL, adl.getValue() );
}

////////////////////////////////////////////////////////////////////////////////
void Adler32Test::testUpdateArray() {

    unsigned char byteArray[] = { 1, 2 };
    Adler32 adl;
    adl.update( byteArray, 2, 0, 2 );

    // The value of the adl should be 393220
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "update(unsigned char[]) failed to update the checksum to the correct value ",
                                  393220LL, adl.getValue() );

    adl.reset();
    std::vector<unsigned char> byteEmpty( 10000, 0 );
    adl.update( byteEmpty );

    // The value of the adl should be 655360001
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "update(unsigned char[]) failed to update the checksum to the correct value ",
                                  655360001LL, adl.getValue() );
}

////////////////////////////////////////////////////////////////////////////////
void Adler32Test::testUpdateArrayIndexed() {

    static const int SIZE = 3;
    unsigned char byteArray[] = { 1, 2, 3 };

    Adler32 adl;
    int off = 2;// accessing the 2nd element of byteArray
    int len = 1;
    int lenError = 3;
    int offError = 4;
    adl.update( byteArray, SIZE, off, len );

    // The value of the adl should be 262148
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "update(unsigned char[],int,int) failed to update the checksum to the correct value ",
                                  262148LL, adl.getValue() );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an IndexOutOfBoundsException for lenError",
        adl.update( byteArray, SIZE, off, lenError ),
        IndexOutOfBoundsException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an IndexOutOfBoundsException for offError",
        adl.update( byteArray, SIZE, offError, len ),
        IndexOutOfBoundsException );
}
