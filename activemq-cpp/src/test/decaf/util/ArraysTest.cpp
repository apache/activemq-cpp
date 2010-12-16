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

#include "ArraysTest.h"

#include <decaf/lang/exceptions/UnsupportedOperationException.h>
#include <decaf/util/Arrays.h>
#include <decaf/lang/Integer.h>

using namespace std;
using namespace decaf;
using namespace decaf::util;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
const int ArraysTest::SIZE = 256;

////////////////////////////////////////////////////////////////////////////////
ArraysTest::ArraysTest() {
}

////////////////////////////////////////////////////////////////////////////////
ArraysTest::~ArraysTest() {
}

////////////////////////////////////////////////////////////////////////////////
void ArraysTest::testFill1() {

    bool boolArray[SIZE];
    Arrays::fill( boolArray, SIZE, true );
    for( int i = 0; i < SIZE; ++i ) {
        CPPUNIT_ASSERT_EQUAL( true, boolArray[i] );
    }
    Arrays::fill( boolArray, SIZE, false );
    for( int i = 0; i < SIZE; ++i ) {
        CPPUNIT_ASSERT_EQUAL( false, boolArray[i] );
    }

    std::string stringArray[SIZE];
    Arrays::fill( stringArray, SIZE, std::string("A") );
    for( int i = 0; i < SIZE; ++i ) {
        CPPUNIT_ASSERT_EQUAL( std::string("A"), stringArray[i] );
    }
    Arrays::fill( stringArray, SIZE, std::string("B") );
    for( int i = 0; i < SIZE; ++i ) {
        CPPUNIT_ASSERT_EQUAL( std::string("B"), stringArray[i] );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException",
        Arrays::fill( (bool*)NULL, -1, false ),
        NullPointerException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a IllegalArgumentException",
        Arrays::fill( boolArray, -1, false ),
        IllegalArgumentException );
}

////////////////////////////////////////////////////////////////////////////////
void ArraysTest::testFill2() {

    bool boolArray[SIZE];
    Arrays::fill( boolArray, SIZE, 0, SIZE, true );
    for( int i = 0; i < SIZE; ++i ) {
        CPPUNIT_ASSERT_EQUAL( true, boolArray[i] );
    }
    Arrays::fill( boolArray, SIZE, 0, SIZE, false );
    for( int i = 0; i < SIZE; ++i ) {
        CPPUNIT_ASSERT_EQUAL( false, boolArray[i] );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException",
        Arrays::fill( (bool*)NULL, -1, 0, 1, false ),
        NullPointerException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a IllegalArgumentException",
        Arrays::fill( boolArray, -1, 0, 1, false ),
        IllegalArgumentException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a IllegalArgumentException",
        Arrays::fill( boolArray, SIZE, SIZE + 1, SIZE, false ),
        IllegalArgumentException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a IndexOutOfBoundsException",
        Arrays::fill( boolArray, SIZE, -1, SIZE, false ),
        IndexOutOfBoundsException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a IndexOutOfBoundsException",
        Arrays::fill( boolArray, SIZE, 0, SIZE + 10, false ),
        IndexOutOfBoundsException );
}
