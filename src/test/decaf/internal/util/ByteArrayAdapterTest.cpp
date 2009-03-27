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

#include "ByteArrayAdapterTest.h"

#include <decaf/lang/Double.h>
#include <decaf/lang/Float.h>

using namespace decaf;
using namespace decaf::internal;
using namespace decaf::internal::util;
using namespace decaf::nio;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
void ByteArrayAdapterTest::testArray() {

    ByteArrayAdapter testBuffer1( testData1Size );

    testBuffer1.write( testData1, 0, testData1Size );

    for( std::size_t ix = 0; ix < testBuffer1.getCapacity(); ++ix ) {
        CPPUNIT_ASSERT( testBuffer1.get( ix ) == testData1[ix] );
    }

    unsigned char* array = testBuffer1.getByteArray();
    CPPUNIT_ASSERT( array != NULL );

    for( std::size_t ix = 0; ix < testBuffer1.getCapacity(); ++ix ) {
        CPPUNIT_ASSERT( array[ix] == testData1[ix] );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayAdapterTest::testRead(){

    unsigned char* data = new unsigned char[256];
    for( int i = 0; i < 256; ++i ) {
        data[i] = i;
    }

    ByteArrayAdapter array( data, 256 );

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
void ByteArrayAdapterTest::testWrite(){

    unsigned char* data = new unsigned char[256];
    for( int i = 0; i < 256; ++i ) {
        data[i] = i;
    }

    ByteArrayAdapter array( (std::size_t)256 );

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
void ByteArrayAdapterTest::testCtor1(){

    ByteArrayAdapter array1( (std::size_t)256 );
    CPPUNIT_ASSERT( array1.getCapacity() == 256 );
    ByteArrayAdapter array2( (std::size_t)54 );
    CPPUNIT_ASSERT( array2.getCapacity() == 54 );
    ByteArrayAdapter array3( (std::size_t)5555 );
    CPPUNIT_ASSERT( array3.getCapacity() == 5555 );
    ByteArrayAdapter array4( (std::size_t)0 );
    CPPUNIT_ASSERT( array4.getCapacity() == 0 );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayAdapterTest::testCtor2(){

    unsigned char* data1 = new unsigned char[256];
    unsigned char* data2 = new unsigned char[999];
    unsigned char* data3 = new unsigned char[12];
    unsigned char* data4 = new unsigned char[0];

    ByteArrayAdapter array1( data1, 256, true );
    ByteArrayAdapter array2( data2, 999, true );
    ByteArrayAdapter array3( data3, 10, true );
    ByteArrayAdapter array4( data4, 0, true );

    CPPUNIT_ASSERT( array1.getCapacity() == 256 );
    CPPUNIT_ASSERT( array2.getCapacity() == 999 );
    CPPUNIT_ASSERT( array3.getCapacity() == 10 );
    CPPUNIT_ASSERT( array4.getCapacity() == 0 );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayAdapterTest::testClear(){

    unsigned char* data = new unsigned char[256];
    for( int i = 0; i < 256; ++i ) {
        data[i] = i;
    }

    ByteArrayAdapter array( (std::size_t)256 );

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
void ByteArrayAdapterTest::testReszie(){

    unsigned char* data = new unsigned char[256];
    for( int i = 0; i < 256; ++i ) {
        data[i] = i;
    }

    ByteArrayAdapter array( (std::size_t)256 );

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
void ByteArrayAdapterTest::testOperators(){

    unsigned char* data = new unsigned char[256];
    for( int i = 0; i < 256; ++i ) {
        data[i] = i;
    }

    ByteArrayAdapter array( data, 256 );

    CPPUNIT_ASSERT( array.getCapacity() == 256 );

    for( int i = 0; i < 256; ++i ){
        CPPUNIT_ASSERT( data[i] == array[i] );
    }

    delete [] data;
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayAdapterTest::testReadExceptions() {

    unsigned char* data = new unsigned char[256];
    for( int i = 0; i < 256; ++i ) {
        data[i] = i;
    }

    ByteArrayAdapter array( data, 256, true );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should Throw NullPointerException",
        array.read( NULL, 0, 500 ),
        NullPointerException );

    unsigned char result[5000];
    ByteArrayAdapter array2( 256 );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should Throw BufferUnderflowException",
        array.read( result, 0, 500 ),
        BufferUnderflowException );


}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayAdapterTest::testWriteExceptions() {

    unsigned char* data = new unsigned char[256];
    for( int i = 0; i < 256; ++i ) {
        data[i] = i;
    }

    ByteArrayAdapter array( data, 256, true );
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should Throw NullPointerException",
        array.write( NULL, 0, 500 ),
        NullPointerException );

    unsigned char result[5000];
    ByteArrayAdapter array2( 256 );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should Throw BufferOverflowException",
        array.write( result, 0, 500 ),
        BufferOverflowException );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayAdapterTest::testOperatorsExceptions() {

    unsigned char* data = new unsigned char[256];
    for( int i = 0; i < 256; ++i ) {
        data[i] = i;
    }

    ByteArrayAdapter array( data, 256, true );
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should Throw IndexOutOfBoundsException",
        array[9999],
        IndexOutOfBoundsException );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayAdapterTest::testPut() {

    ByteArrayAdapter testBuffer1( testData1Size );

    std::size_t i = 0;
    for( ; ( testBuffer1.getCapacity() - i ) >= sizeof(unsigned char); i+=sizeof(unsigned char) ) {
        testBuffer1.put( i, (unsigned char)(i + 99) );
        CPPUNIT_ASSERT( testBuffer1.get( i ) == (unsigned char)(i + 99) );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a IndexOutOfBoundsException",
        testBuffer1.put( i, 3 ),
        IndexOutOfBoundsException );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayAdapterTest::testPutChar() {

    ByteArrayAdapter testBuffer1( testData1Size );

    std::size_t i = 0;
    for( ; ( testBuffer1.getCapacity() - i ) >= sizeof(char); i+=sizeof(char) ) {
        testBuffer1.putChar( i, i + 99 );
        CPPUNIT_ASSERT( testBuffer1.getChar( i ) == (char)(i + 99) );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a IndexOutOfBoundsException",
        testBuffer1.putChar( i, 3 ),
        IndexOutOfBoundsException );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayAdapterTest::testPutLong() {

    ByteArrayAdapter testBuffer1( testData1Size );

    std::size_t i = 0;
    for( ; i < testBuffer1.getLongCapacity(); ++i  ) {
        testBuffer1.putLong( i, i + 99 );
        CPPUNIT_ASSERT( testBuffer1.getLong( i ) == (long long)(i + 99) );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a IndexOutOfBoundsException",
        testBuffer1.putLong( i, 3 ),
        IndexOutOfBoundsException );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayAdapterTest::testPutInt() {

    ByteArrayAdapter testBuffer1( testData1Size );

    std::size_t i = 0;
    for( ; i < testBuffer1.getIntCapacity(); ++i  ) {
        testBuffer1.putInt( i, i + 99 );
        CPPUNIT_ASSERT( testBuffer1.getInt( i ) == (int)(i + 99) );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a IndexOutOfBoundsException",
        testBuffer1.putInt( i, 3 ),
        IndexOutOfBoundsException );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayAdapterTest::testPutShort() {

    ByteArrayAdapter testBuffer1( testData1Size );

    std::size_t i = 0;
    for( ; i < testBuffer1.getShortCapacity(); ++i  ) {
        testBuffer1.putShort( i, i + 99 );
        CPPUNIT_ASSERT( testBuffer1.getShort( i ) == (short)(i + 99) );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a IndexOutOfBoundsException",
        testBuffer1.putShort( i, 3 ),
        IndexOutOfBoundsException );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayAdapterTest::testPutDouble() {

    ByteArrayAdapter testBuffer1( testData1Size );

    std::size_t i = 0;
    for( ; i < testBuffer1.getDoubleCapacity(); ++i  ) {
        testBuffer1.putDouble( i, i + 99.025 );
        CPPUNIT_ASSERT( Double::doubleToLongBits( testBuffer1.getDouble( i ) ) ==
                        Double::doubleToLongBits( (double)(i + 99.025) ) );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a IndexOutOfBoundsException",
        testBuffer1.putDouble( i, 3 ),
        IndexOutOfBoundsException );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayAdapterTest::testPutFloat() {

    ByteArrayAdapter testBuffer1( testData1Size );

    std::size_t i = 0;
    for( ; i < testBuffer1.getFloatCapacity(); ++i  ) {
        testBuffer1.putFloat( i, i + 99.025 );
        CPPUNIT_ASSERT( Float::floatToIntBits( testBuffer1.getFloat( i ) ) ==
                        Float::floatToIntBits( (float)(i + 99.025) ) );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a IndexOutOfBoundsException",
        testBuffer1.putFloat( i, 3 ),
        IndexOutOfBoundsException );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayAdapterTest::testGet() {

    ByteArrayAdapter testBuffer1( testData1Size );

    std::vector<unsigned char> values;
    for( std::size_t i = 0; i < testBuffer1.getCapacity(); ++i ) {
        testBuffer1.put( i, (unsigned char)i );
        values.push_back( (unsigned char)i );
    }

    std::size_t i = 0;
    std::size_t j = 0;

    for( ; i < testBuffer1.getCapacity(); ++i, ++j ) {
        CPPUNIT_ASSERT( testBuffer1.get( i ) == values[j] );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a IndexOutOfBoundsException",
        testBuffer1.get( i ),
        IndexOutOfBoundsException );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayAdapterTest::testGetChar() {

    ByteArrayAdapter testBuffer1( testData1Size );

    std::vector<char> values;
    for( std::size_t i = 0; i < testBuffer1.getCapacity(); ++i ) {
        testBuffer1.putChar( i, (char)i );
        values.push_back( (char)i );
    }

    std::size_t i = 0;
    std::size_t j = 0;

    for( ; i < testBuffer1.getCapacity(); ++i, ++j ) {
        CPPUNIT_ASSERT( testBuffer1.getChar( i ) == values[j] );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a IndexOutOfBoundsException",
        testBuffer1.getChar( i ),
        IndexOutOfBoundsException );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayAdapterTest::testGetShort() {

    ByteArrayAdapter testBuffer1( testData1Size );

    std::vector<short> values;
    for( std::size_t i = 0; i < testBuffer1.getShortCapacity(); ++i ) {
        testBuffer1.putShort( i, (short)i );
        values.push_back( (short)i );
    }

    std::size_t i = 0;
    std::size_t j = 0;

    for( ; i < testBuffer1.getShortCapacity(); ++i, ++j ) {
        CPPUNIT_ASSERT( testBuffer1.getShort( i ) == values[j] );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a IndexOutOfBoundsException",
        testBuffer1.getShort( i ),
        IndexOutOfBoundsException );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayAdapterTest::testGetInt() {

    ByteArrayAdapter testBuffer1( testData1Size );

    std::vector<int> values;
    for( std::size_t i = 0; i < testBuffer1.getIntCapacity(); ++i ) {
        testBuffer1.putInt( i, (int)i );
        values.push_back( (int)i );
    }

    std::size_t i = 0;
    std::size_t j = 0;

    for( ; i < testBuffer1.getIntCapacity(); ++i, ++j ) {
        CPPUNIT_ASSERT( testBuffer1.getInt( i ) == values[j] );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a IndexOutOfBoundsException",
        testBuffer1.getInt( i ),
        IndexOutOfBoundsException );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayAdapterTest::testGetLong() {

    ByteArrayAdapter testBuffer1( testData1Size );

    std::vector<long long> values;
    for( std::size_t i = 0; i < testBuffer1.getLongCapacity(); ++i ) {
        testBuffer1.putLong( i, (long long)i );
        values.push_back( (long long)i );
    }

    std::size_t i = 0;
    std::size_t j = 0;

    for( ; i < testBuffer1.getLongCapacity(); ++i, ++j ) {
        CPPUNIT_ASSERT( testBuffer1.getLong( i ) == values[j] );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a IndexOutOfBoundsException",
        testBuffer1.getLong( i ),
        IndexOutOfBoundsException );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayAdapterTest::testGetFloat() {

    ByteArrayAdapter testBuffer1( testData1Size );

    std::vector<float> values;
    for( std::size_t i = 0; i < testBuffer1.getFloatCapacity(); ++i ) {
        testBuffer1.putFloat( i, (float)i + 0.025f );
        values.push_back( (float)i + 0.025f );
    }

    std::size_t i = 0;
    std::size_t j = 0;

    for( ; i < testBuffer1.getFloatCapacity(); ++i, ++j ) {
        CPPUNIT_ASSERT( Float::floatToIntBits( testBuffer1.getFloat( i ) ) ==
                        Float::floatToIntBits( values[j] ) );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a IndexOutOfBoundsException",
        testBuffer1.getFloat( i ),
        IndexOutOfBoundsException );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayAdapterTest::testGetDouble() {

    ByteArrayAdapter testBuffer1( testData1Size );

    std::vector<double> values;
    for( std::size_t i = 0; i < testBuffer1.getDoubleCapacity(); ++i ) {
        testBuffer1.putDouble( i, (double)i + 0.025 );
        values.push_back( (double)i + 0.025 );
    }

    std::size_t i = 0;
    std::size_t j = 0;

    for( ; i < testBuffer1.getDoubleCapacity(); ++i, ++j ) {
        CPPUNIT_ASSERT( Double::doubleToLongBits( testBuffer1.getDouble( i ) ) ==
                        Double::doubleToLongBits( values[j] ) );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a IndexOutOfBoundsException",
        testBuffer1.getDouble( i ),
        IndexOutOfBoundsException );
}
