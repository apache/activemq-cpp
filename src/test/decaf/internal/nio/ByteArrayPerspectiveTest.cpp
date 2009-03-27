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

#include "ByteArrayPerspectiveTest.h"

using namespace decaf;
using namespace decaf::internal;
using namespace decaf::internal::util;
using namespace decaf::internal::nio;
using namespace decaf::nio;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
void ByteArrayPerspectiveTest::testReferences(){

    ByteArrayPerspective array( (std::size_t)256 );
    CPPUNIT_ASSERT( array.getReferences() == 1 );
    array.takeRef();
    CPPUNIT_ASSERT( array.getReferences() == 2 );
    array.returnRef();
    array.returnRef();
    CPPUNIT_ASSERT( array.getReferences() == 0 );

}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayPerspectiveTest::testRead(){

    unsigned char* data = new unsigned char[256];
    for( int i = 0; i < 256; ++i ) {
        data[i] = i;
    }

    ByteArrayPerspective array( data, 256 );

    CPPUNIT_ASSERT( array.getCapacity() == 256 );

    unsigned char* result = new unsigned char[256];
    array.read( result, 0, 256 );

    for( int i = 0; i < 256; ++i ){
        CPPUNIT_ASSERT( data[i] == result[i] );
    }

    delete [] data;
    delete [] result;
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayPerspectiveTest::testWrite(){

    unsigned char* data = new unsigned char[256];
    for( int i = 0; i < 256; ++i ) {
        data[i] = i;
    }

    ByteArrayPerspective array( (std::size_t)256 );

    CPPUNIT_ASSERT( array.getCapacity() == 256 );

    array.write( data, 0, 256 );
    unsigned char* result = new unsigned char[256];
    array.read( result, 0, 256 );

    for( int i = 0; i < 256; ++i ){
        CPPUNIT_ASSERT( data[i] == result[i] );
    }

    delete [] data;
    delete [] result;
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayPerspectiveTest::testCtor1(){

    ByteArrayPerspective array1( (std::size_t)256 );
    CPPUNIT_ASSERT( array1.getCapacity() == 256 );
    ByteArrayPerspective array2( (std::size_t)54 );
    CPPUNIT_ASSERT( array2.getCapacity() == 54 );
    ByteArrayPerspective array3( (std::size_t)5555 );
    CPPUNIT_ASSERT( array3.getCapacity() == 5555 );
    ByteArrayPerspective array4( (std::size_t)0 );
    CPPUNIT_ASSERT( array4.getCapacity() == 0 );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayPerspectiveTest::testCtor2(){

    unsigned char* data1 = new unsigned char[256];
    unsigned char* data2 = new unsigned char[999];
    unsigned char* data3 = new unsigned char[12];
    unsigned char* data4 = new unsigned char[0];

    ByteArrayPerspective array1( data1, 256, true );
    ByteArrayPerspective array2( data2, 999, true );
    ByteArrayPerspective array3( data3, 10, true );
    ByteArrayPerspective array4( data4, 0, true );

    CPPUNIT_ASSERT( array1.getCapacity() == 256 );
    CPPUNIT_ASSERT( array2.getCapacity() == 999 );
    CPPUNIT_ASSERT( array3.getCapacity() == 10 );
    CPPUNIT_ASSERT( array4.getCapacity() == 0 );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayPerspectiveTest::testClear(){

    unsigned char* data = new unsigned char[256];
    for( int i = 0; i < 256; ++i ) {
        data[i] = i;
    }

    ByteArrayPerspective array( (std::size_t)256 );

    CPPUNIT_ASSERT( array.getCapacity() == 256 );

    array.write( data, 0, 256 );
    unsigned char* result = new unsigned char[256];
    array.read( result, 0, 256 );

    for( int i = 0; i < 256; ++i ){
        CPPUNIT_ASSERT( data[i] == result[i] );
    }

    array.clear();
    array.read( result, 0, 256 );

    for( int i = 0; i < 256; ++i ){
        CPPUNIT_ASSERT( result[i] == 0 );
    }

    delete [] data;
    delete [] result;
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayPerspectiveTest::testReszie(){

    unsigned char* data = new unsigned char[256];
    for( int i = 0; i < 256; ++i ) {
        data[i] = i;
    }

    ByteArrayPerspective array( (std::size_t)256 );

    CPPUNIT_ASSERT( array.getCapacity() == 256 );

    array.write( data, 0, 256 );
    unsigned char* result = new unsigned char[256];
    array.read( result, 0, 256 );

    for( int i = 0; i < 256; ++i ){
        CPPUNIT_ASSERT( data[i] == result[i] );
    }

    array.resize( 128 );
    CPPUNIT_ASSERT( array.getCapacity() == 128 );
    unsigned char* result2 = new unsigned char[128];
    array.read( result2, 0, 128 );

    for( int i = 0; i < 128; ++i ){
        CPPUNIT_ASSERT( result[i] == data[i] );
    }

    delete [] data;
    delete [] result;
    delete [] result2;
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayPerspectiveTest::testOperators(){

    unsigned char* data = new unsigned char[256];
    for( int i = 0; i < 256; ++i ) {
        data[i] = i;
    }

    ByteArrayPerspective array( data, 256 );

    CPPUNIT_ASSERT( array.getCapacity() == 256 );

    for( int i = 0; i < 256; ++i ){
        CPPUNIT_ASSERT( data[i] == array[i] );
    }

    delete [] data;
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayPerspectiveTest::testReadExceptions() {

    unsigned char* data = new unsigned char[256];
    for( int i = 0; i < 256; ++i ) {
        data[i] = i;
    }

    ByteArrayPerspective array( data, 256, true );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should Throw NullPointerException",
        array.read( NULL, 0, 500 ),
        NullPointerException );

    unsigned char result[5000];
    ByteArrayPerspective array2( 256 );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should Throw BufferUnderflowException",
        array.read( result, 0, 500 ),
        BufferUnderflowException );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayPerspectiveTest::testWriteExceptions() {

    unsigned char* data = new unsigned char[256];
    for( int i = 0; i < 256; ++i ) {
        data[i] = i;
    }

    ByteArrayPerspective array( data, 256, true );
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should Throw NullPointerException",
        array.write( NULL, 0, 500 ),
        NullPointerException );

    unsigned char result[5000];
    ByteArrayPerspective array2( 256 );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should Throw BufferOverflowException",
        array.write( result, 0, 500 ),
        BufferOverflowException );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayPerspectiveTest::testOperatorsExceptions() {

    unsigned char* data = new unsigned char[256];
    for( int i = 0; i < 256; ++i ) {
        data[i] = i;
    }

    ByteArrayPerspective array( data, 256, true );
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should Throw IndexOutOfBoundsException",
        array[9999],
        IndexOutOfBoundsException );
}
