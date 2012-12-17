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

#include "SecureRandomTest.h"

#include <decaf/security/SecureRandom.h>

#include <decaf/lang/exceptions/IllegalArgumentException.h>
#include <decaf/lang/exceptions/NullPointerException.h>

using namespace std;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::security;

////////////////////////////////////////////////////////////////////////////////
SecureRandomTest::SecureRandomTest() {
}

////////////////////////////////////////////////////////////////////////////////
SecureRandomTest::~SecureRandomTest() {
}

////////////////////////////////////////////////////////////////////////////////
void SecureRandomTest::testConstructor1() {
    SecureRandom srng;
}

////////////////////////////////////////////////////////////////////////////////
void SecureRandomTest::testConstructor2() {
    std::vector<unsigned char> seed;

    seed.push_back( 65 );
    seed.push_back( 120 );
    seed.push_back( 6 );
    seed.push_back( 15 );
    seed.push_back( 15 );
    seed.push_back( 24 );
    seed.push_back( 221 );

    SecureRandom srng( seed );
}

////////////////////////////////////////////////////////////////////////////////
void SecureRandomTest::testConstructor3() {
    unsigned char seed[] = { 51, 12, 99, 18, 125, 127, 21, 99 };

    SecureRandom srng( seed, 8 );
}

////////////////////////////////////////////////////////////////////////////////
void SecureRandomTest::testNextBytes1() {

    std::vector<unsigned char> buffer1( 255, 0 );
    std::vector<unsigned char> buffer2( 255, 0 );
    std::vector<unsigned char> buffer3( 255, 0 );

    long long buffer1Sum = 0LL;
    long long buffer2Sum = 0LL;
    long long buffer3Sum = 0LL;

    SecureRandom srng;
    srng.nextBytes( buffer1 );
    srng.nextBytes( buffer2 );
    srng.nextBytes( buffer3 );

    for( std::size_t i = 0; i < buffer1.size(); ++i ) {
        buffer1Sum += buffer1[i];
    }
    for( std::size_t i = 0; i < buffer2.size(); ++i ) {
        buffer2Sum += buffer2[i];
    }
    for( std::size_t i = 0; i < buffer3.size(); ++i ) {
        buffer3Sum += buffer3[i];
    }

    CPPUNIT_ASSERT( buffer1Sum != buffer2Sum );
    CPPUNIT_ASSERT( buffer1Sum != buffer3Sum );
    CPPUNIT_ASSERT( buffer2Sum != buffer3Sum );

    std::vector<unsigned char> emptyBuffer;
    CPPUNIT_ASSERT_NO_THROW( srng.nextBytes( emptyBuffer ) );
}

////////////////////////////////////////////////////////////////////////////////
void SecureRandomTest::testNextBytes2() {

    unsigned char buffer1[255] = {0};
    unsigned char buffer2[255] = {0};
    unsigned char buffer3[255] = {0};

    long long buffer1Sum = 0LL;
    long long buffer2Sum = 0LL;
    long long buffer3Sum = 0LL;

    SecureRandom srng;
    srng.nextBytes( buffer1, 255 );
    srng.nextBytes( buffer2, 255 );
    srng.nextBytes( buffer3, 255 );

    for( std::size_t i = 0; i < 255; ++i ) {
        buffer1Sum += buffer1[i];
    }
    for( std::size_t i = 0; i < 255; ++i ) {
        buffer2Sum += buffer2[i];
    }
    for( std::size_t i = 0; i < 255; ++i ) {
        buffer3Sum += buffer3[i];
    }

    CPPUNIT_ASSERT( buffer1Sum != buffer2Sum );
    CPPUNIT_ASSERT( buffer1Sum != buffer3Sum );
    CPPUNIT_ASSERT( buffer2Sum != buffer3Sum );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a NullPointerException",
        srng.nextBytes( NULL, 255 ),
        NullPointerException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a IllegalArgumentException",
        srng.nextBytes( buffer1, -1 ),
        IllegalArgumentException );
}

////////////////////////////////////////////////////////////////////////////////
void SecureRandomTest::testSetSeed1() {

    std::vector<unsigned char> seed;

    seed.push_back( 65 );
    seed.push_back( 120 );
    seed.push_back( 6 );
    seed.push_back( 15 );
    seed.push_back( 15 );
    seed.push_back( 24 );
    seed.push_back( 221 );

    SecureRandom srng;
    CPPUNIT_ASSERT_NO_THROW( srng.setSeed( seed ) );

    std::vector<unsigned char> emptyBuffer;
    CPPUNIT_ASSERT_NO_THROW( srng.setSeed( emptyBuffer ) );
}

////////////////////////////////////////////////////////////////////////////////
void SecureRandomTest::testSetSeed2() {
    unsigned char seed[] = { 51, 12, 99, 18, 125, 127, 21, 99 };

    SecureRandom srng;
    CPPUNIT_ASSERT_NO_THROW( srng.setSeed( seed, 8 ) );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a NullPointerException",
        srng.setSeed( NULL, 255 ),
        NullPointerException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a IllegalArgumentException",
        srng.setSeed( seed, -1 ),
        IllegalArgumentException );
}

////////////////////////////////////////////////////////////////////////////////
void SecureRandomTest::testSetSeed3() {

    long long seed = 4565143165LL;
    SecureRandom srng;
    CPPUNIT_ASSERT_NO_THROW( srng.setSeed( seed ) );
}
