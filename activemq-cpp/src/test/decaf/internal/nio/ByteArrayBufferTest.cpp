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

#include "ByteArrayBufferTest.h"
#include <decaf/lang/Integer.h>
#include <decaf/lang/Double.h>
#include <decaf/lang/Float.h>
#include <string.h>

using namespace std;
using namespace decaf;
using namespace decaf::nio;
using namespace decaf::internal::nio;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
void ByteArrayBufferTest::test() {

    // Check that we have setup the array and our initial assumptions on state
    // are correct.  This is the first test run.
    CPPUNIT_ASSERT( testBuffer1 != NULL );
    CPPUNIT_ASSERT( testBuffer1->capacity() == testData1Size );
    CPPUNIT_ASSERT( testBuffer1->hasRemaining() == true );
    CPPUNIT_ASSERT( testBuffer1->limit() == testBuffer1->capacity() );
    CPPUNIT_ASSERT( testBuffer1->position() == 0 );
    CPPUNIT_ASSERT( testBuffer1->isReadOnly() == false );
    CPPUNIT_ASSERT( testBuffer1->toString() != "" );
    CPPUNIT_ASSERT( testBuffer1->hasArray() == true );
    CPPUNIT_ASSERT( testBuffer1->array() != NULL );
    CPPUNIT_ASSERT( testBuffer1->arrayOffset() == 0 );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayBufferTest::testArray() {

    testBuffer1->put( testData1, 0, testData1Size );
    testBuffer1->position( 0 );

    testBuffer1->mark();

    for( std::size_t ix = 0; ix < testBuffer1->capacity(); ++ix ) {
        CPPUNIT_ASSERT( testBuffer1->get() == testData1[ix] );
    }

    testBuffer1->reset();

    unsigned char* array = testBuffer1->array();
    CPPUNIT_ASSERT( array != NULL );

    for( std::size_t ix = 0; ix < testBuffer1->capacity(); ++ix ) {
        CPPUNIT_ASSERT( array[ix] == testData1[ix] );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayBufferTest::testArrayOffset() {

    testBuffer1->put( testData1, 0, testData1Size );
    CPPUNIT_ASSERT( testBuffer1->arrayOffset() == 0 );
    testBuffer1->rewind();
    CPPUNIT_ASSERT( testBuffer1->arrayOffset() == 0 );

    testBuffer1->get();
    ByteBuffer* sliced = testBuffer1->slice();
    CPPUNIT_ASSERT( sliced->arrayOffset() == 1 );
    delete sliced;
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayBufferTest::testReadOnlyArray() {

    ByteBuffer* readOnly = testBuffer1->asReadOnlyBuffer();

    CPPUNIT_ASSERT( readOnly != NULL );
    CPPUNIT_ASSERT( readOnly->isReadOnly() == true );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw UnsupportedOperationException",
        readOnly->array(),
        UnsupportedOperationException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw UnsupportedOperationException",
        readOnly->arrayOffset(),
        UnsupportedOperationException );

    delete readOnly;
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayBufferTest::testAsReadOnlyBuffer() {

    testBuffer1->clear();
    testBuffer1->mark();
    testBuffer1->position( testBuffer1->limit() );

    // readonly's contents should be the same as buf
    ByteBuffer* readOnly = testBuffer1->asReadOnlyBuffer();

    CPPUNIT_ASSERT( testBuffer1 != readOnly );
    CPPUNIT_ASSERT( readOnly->isReadOnly() );
    CPPUNIT_ASSERT( testBuffer1->position() == readOnly->position() );
    CPPUNIT_ASSERT( testBuffer1->limit() == readOnly->limit() );

    CPPUNIT_ASSERT( *testBuffer1 == *readOnly );
    CPPUNIT_ASSERT( testBuffer1->compareTo( *readOnly ) == 0 );

    // readonly's position, mark, and limit should be independent to buf
    readOnly->reset();
    CPPUNIT_ASSERT( readOnly->position() == 0 );
    readOnly->clear();
    CPPUNIT_ASSERT( testBuffer1->position() == testBuffer1->limit() );
    testBuffer1->reset();
    CPPUNIT_ASSERT( testBuffer1->position() == 0);

    delete readOnly;
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayBufferTest::testCompact() {

    // readonly's contents should be the same as buf
    ByteBuffer* readOnly = testBuffer1->asReadOnlyBuffer();

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a ReadOnlyBufferExceptio",
        readOnly->compact(),
        ReadOnlyBufferException );

    // case: buffer is full
    testBuffer1->clear();
    testBuffer1->mark();

    for( std::size_t ix = 0; ix < testData1Size; ++ix ){
        testBuffer1->put( ix, testData1[ix] );
    }

    ByteBuffer& ret = testBuffer1->compact();

    CPPUNIT_ASSERT( &ret == testBuffer1 );
    CPPUNIT_ASSERT( testBuffer1->position() == testBuffer1->capacity() );
    CPPUNIT_ASSERT( testBuffer1->limit() == testBuffer1->capacity() );

    for( std::size_t ix = 0; ix < testBuffer1->capacity(); ix++ ) {
        CPPUNIT_ASSERT( testBuffer1->get( ix ) == testData1[ix] );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a InvalidMarkException",
        readOnly->reset(),
        InvalidMarkException );

    delete readOnly;

    // case: buffer is empty
    testBuffer1->position(0);
    testBuffer1->limit(0);
    testBuffer1->mark();
    ret = testBuffer1->compact();
    CPPUNIT_ASSERT( &ret == testBuffer1 );
    CPPUNIT_ASSERT( testBuffer1->position() == 0 );
    CPPUNIT_ASSERT( testBuffer1->limit() == testBuffer1->capacity() );

    for( std::size_t ix = 0; ix < testBuffer1->capacity(); ix++ ) {
        CPPUNIT_ASSERT( testBuffer1->get( ix ) == testData1[ix] );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a InvalidMarkException",
        testBuffer1->reset(),
        InvalidMarkException );

    // case: normal
    testBuffer1->position(1);
    testBuffer1->limit(SMALL_TEST_LENGTH);
    testBuffer1->mark();
    ret = testBuffer1->compact();
    CPPUNIT_ASSERT( &ret == testBuffer1 );
    CPPUNIT_ASSERT( testBuffer1->position() == SMALL_TEST_LENGTH - 1  );
    CPPUNIT_ASSERT( testBuffer1->limit() == testBuffer1->capacity() );

    for( std::size_t ix = 0; ix < SMALL_TEST_LENGTH - 1; ix++ ) {
        CPPUNIT_ASSERT( testBuffer1->get( ix ) == testData1[ix + 1] );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a InvalidMarkException",
        testBuffer1->reset(),
        InvalidMarkException );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayBufferTest::testCompareTo() {

    // compare to self
    CPPUNIT_ASSERT( 0 == testBuffer1->compareTo( *testBuffer1 ) );

    CPPUNIT_ASSERT( testBuffer1->capacity() > SMALL_TEST_LENGTH );
    testBuffer1->clear();
    ByteBuffer* other = ByteBuffer::allocate( testBuffer1->capacity() );

    for( std::size_t ix = 0; ix < testData1Size; ++ix ){
        testBuffer1->put( ix, testData1[ix] );
    }

    for( std::size_t ix = 0; ix < testData1Size; ++ix ){
        other->put( ix, testData1[ix] );
    }

    CPPUNIT_ASSERT( 0 == testBuffer1->compareTo( *other ) );
    CPPUNIT_ASSERT( 0 == other->compareTo( *testBuffer1 ) );
    testBuffer1->position(1);
    CPPUNIT_ASSERT( testBuffer1->compareTo( *other ) > 0 );
    CPPUNIT_ASSERT( other->compareTo( *testBuffer1 ) < 0 );
    other->position( 2 );
    CPPUNIT_ASSERT( testBuffer1->compareTo( *other ) < 0 );
    CPPUNIT_ASSERT( other->compareTo( *testBuffer1 ) > 0 );
    testBuffer1->position( 2 );
    other->limit(SMALL_TEST_LENGTH);
    CPPUNIT_ASSERT( testBuffer1->compareTo( *other ) > 0 );
    CPPUNIT_ASSERT( other->compareTo( *testBuffer1 ) < 0 );

    unsigned char* data = new unsigned char[21];
    memset( data, 0, 21 );
    ByteBuffer* empty = ByteBuffer::allocate(21);
    ByteBuffer* wrapped = ByteBuffer::wrap( data, (std::size_t)0, (std::size_t)21 );

    CPPUNIT_ASSERT( wrapped->compareTo( *empty ) == 0 );

    delete empty;
    delete wrapped;
    delete other;
    delete [] data;
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayBufferTest::testDuplicate() {

    testBuffer1->clear();
    testBuffer1->mark();
    testBuffer1->position( testBuffer1->limit() );

    for( std::size_t ix = 0; ix < testData1Size; ++ix ){
        testBuffer1->put( ix, testData1[ix] );
    }

    // duplicate's contents should be the same as buf
    ByteBuffer* duplicate = testBuffer1->duplicate();
    CPPUNIT_ASSERT( testBuffer1 != duplicate );
    CPPUNIT_ASSERT( testBuffer1->position() == duplicate->position() );
    CPPUNIT_ASSERT( testBuffer1->limit() == duplicate->limit() );
    CPPUNIT_ASSERT( testBuffer1->isReadOnly() == duplicate->isReadOnly() );

    for( std::size_t ix = 0; ix < testBuffer1->capacity(); ix++ ) {
        CPPUNIT_ASSERT( testBuffer1->get( ix ) == duplicate->get( ix ) );
    }

    // duplicate's position, mark, and limit should be independent to buf
    duplicate->reset();
    CPPUNIT_ASSERT( duplicate->position() == 0 );
    duplicate->clear();
    CPPUNIT_ASSERT( testBuffer1->position() == testBuffer1->limit() );
    testBuffer1->reset();
    CPPUNIT_ASSERT( testBuffer1->position() == 0 );

    delete duplicate;
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayBufferTest::testEquals() {

    // equal to self
    CPPUNIT_ASSERT( testBuffer1->equals( *testBuffer1 ) );
    ByteBuffer* readOnly = testBuffer1->asReadOnlyBuffer();
    CPPUNIT_ASSERT( testBuffer1->equals( *readOnly ) );
    ByteBuffer* duplicate = testBuffer1->duplicate();
    CPPUNIT_ASSERT( testBuffer1->equals( *duplicate ) );

    CPPUNIT_ASSERT( testBuffer1->capacity() > SMALL_TEST_LENGTH );

    testBuffer1->limit( testBuffer1->capacity() ).position( 0 );
    readOnly->limit( readOnly->capacity() ).position( 1 );
    CPPUNIT_ASSERT( !testBuffer1->equals( *readOnly ) );

    testBuffer1->limit( testBuffer1->capacity() - 1 ).position( 0 );
    duplicate->limit( duplicate->capacity() ).position( 0 );
    CPPUNIT_ASSERT( !testBuffer1->equals( *duplicate ) );

    delete readOnly;
    delete duplicate;
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayBufferTest::testGet() {

    testBuffer1->clear();

    for( std::size_t i = 0; i < testBuffer1->capacity(); i++ ) {
        CPPUNIT_ASSERT( testBuffer1->position() == i );
        CPPUNIT_ASSERT( testBuffer1->get() == testBuffer1->get(i) );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a BufferUnderflowException",
        testBuffer1->get(),
        BufferUnderflowException );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayBufferTest::testGetbyteArray() {

    std::vector<unsigned char> array;
    array.resize( 1 );
    testBuffer1->clear();

    for( std::size_t i = 0; i < testBuffer1->capacity(); i++ ) {

        CPPUNIT_ASSERT( testBuffer1->position() == i );
        ByteBuffer& ret = testBuffer1->get( array );
        CPPUNIT_ASSERT( array[0] == testBuffer1->get( i ) );
        CPPUNIT_ASSERT( &ret == testBuffer1 );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a BufferUnderflowException",
        testBuffer1->get( array ),
        BufferUnderflowException );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayBufferTest::testGetbyteArray2() {

    testBuffer1->clear();
    unsigned char* array = new unsigned char[testBuffer1->capacity()];
    unsigned char* array2 = new unsigned char[testBuffer1->capacity() + 1];

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a BufferUnderflowException",
        testBuffer1->get( array2, 0, testBuffer1->capacity() + 1 ),
        BufferUnderflowException );

    CPPUNIT_ASSERT( testBuffer1->position() == 0 );

    testBuffer1->get( array, testBuffer1->capacity(), 0 );

    CPPUNIT_ASSERT( testBuffer1->position() == 0 );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException",
        testBuffer1->get( NULL, 0, 1 ),
        NullPointerException );

    CPPUNIT_ASSERT( testBuffer1->position() == 0 );

    testBuffer1->clear();
    ByteBuffer& ret = testBuffer1->get( array, 0, testBuffer1->capacity() );
    CPPUNIT_ASSERT( testBuffer1->position() == testBuffer1->capacity() );
    for( std::size_t ix = 0; ix < testBuffer1->capacity() - 1; ix++ ) {
        CPPUNIT_ASSERT( testBuffer1->get( ix ) == array[ix] );
    }

    CPPUNIT_ASSERT( &ret == testBuffer1);

    delete [] array;
    delete [] array2;
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayBufferTest::testGetWithIndex() {

    testBuffer1->clear();

    for( std::size_t i = 0; i < testBuffer1->capacity(); i++) {
        CPPUNIT_ASSERT( testBuffer1->position() == i );
        CPPUNIT_ASSERT( testBuffer1->get() == testBuffer1->get(i) );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a IndexOutOfBoundsException",
        testBuffer1->get( testBuffer1->limit() ),
        IndexOutOfBoundsException );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayBufferTest::testPutbyte() {

    ByteBuffer* readOnly = testBuffer1->asReadOnlyBuffer();

    readOnly->clear();

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a ReadOnlyBufferException",
        readOnly->put( 0 ),
        ReadOnlyBufferException );

    delete readOnly;

    testBuffer1->clear();
    for( size_t i = 0; i < testBuffer1->capacity(); i++) {
        CPPUNIT_ASSERT( testBuffer1->position() == i );
        ByteBuffer& ret = testBuffer1->put( (unsigned char)i );
        CPPUNIT_ASSERT( testBuffer1->get(i) == i );
        CPPUNIT_ASSERT( &ret == testBuffer1 );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a BufferOverflowException",
        testBuffer1->put( 0 ),
        BufferOverflowException );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayBufferTest::testPutbyteArray() {

    std::vector<unsigned char> array;
    array.push_back( 127 );

    ByteBuffer* readOnly = testBuffer1->asReadOnlyBuffer();
    readOnly->clear();
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a ReadOnlyBufferException",
        readOnly->put( array ),
        ReadOnlyBufferException );
    delete readOnly;

    testBuffer1->clear();
    for( std::size_t i = 0; i < testBuffer1->capacity(); i++ ) {
        CPPUNIT_ASSERT( testBuffer1->position() == i );
        array[0] = (unsigned char)i;
        ByteBuffer& ret = testBuffer1->put( array );
        CPPUNIT_ASSERT( testBuffer1->get(i) == i );
        CPPUNIT_ASSERT( &ret == testBuffer1 );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a BufferOverflowException",
        testBuffer1->put( array ),
        BufferOverflowException );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayBufferTest::testPutbyteArray2() {

    testBuffer1->clear();
    unsigned char* array = new unsigned char[testBuffer1->capacity()];
    unsigned char* array2 = new unsigned char[testBuffer1->capacity() + 1];

    for( std::size_t ix = 0; ix < testBuffer1->capacity(); ++ix ){
        array[ix] = 0;
    }
    for( std::size_t ix = 0; ix < testBuffer1->capacity()+1; ++ix ){
        array2[ix] = 0;
    }

    ByteBuffer* readOnly = testBuffer1->asReadOnlyBuffer();
    readOnly->clear();
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a ReadOnlyBufferException",
        readOnly->put( array, 0, testBuffer1->capacity() ),
        ReadOnlyBufferException );
    delete readOnly;

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a BufferOverflowException",
        testBuffer1->put( array2, 0, testBuffer1->capacity() + 1 ),
        BufferOverflowException );

    CPPUNIT_ASSERT( testBuffer1->position() == 0 );
    testBuffer1->put( array, testBuffer1->capacity(), 0 );
    CPPUNIT_ASSERT( testBuffer1->position() == 0 );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException",
        testBuffer1->put( NULL, 2, Integer::MAX_VALUE ),
        NullPointerException );

    CPPUNIT_ASSERT( testBuffer1->position() == 0 );

    for( std::size_t ix = 0; ix < testData1Size; ++ix ){
        testBuffer1->put( ix, testData1[ix] );
    }

    ByteBuffer& ret = testBuffer1->put( array, 0, testBuffer1->capacity() );
    CPPUNIT_ASSERT( testBuffer1->position() == testBuffer1->capacity() );
    for( std::size_t ix = 0; ix < testBuffer1->capacity() - 1; ix++ ) {
        CPPUNIT_ASSERT( testBuffer1->get( ix ) == array[ix] );
    }
    CPPUNIT_ASSERT( &ret == testBuffer1 );

    delete [] array;
    delete [] array2;
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayBufferTest::testPutByteBuffer() {

    ByteBuffer* other = ByteBuffer::allocate( testBuffer1->capacity() );

    ByteBuffer* readOnly = testBuffer1->asReadOnlyBuffer();
    readOnly->clear();
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a ReadOnlyBufferException",
        readOnly->put( *other ),
        ReadOnlyBufferException );
    delete readOnly;

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a IllegalArgumentException",
        testBuffer1->put( *testBuffer1 ),
        IllegalArgumentException );

    ByteBuffer* toBig = testBuffer1->allocate( testBuffer1->capacity() + 1 );
    toBig->clear();
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a BufferOverflowException",
        testBuffer1->put( *toBig ),
        BufferOverflowException );
    delete toBig;

    for( std::size_t ix = 0; ix < testData1Size; ++ix ){
        other->put( ix, testData1[ix] );
    }
    other->clear();

    testBuffer1->clear();
    ByteBuffer& ret = testBuffer1->put( *other );
    CPPUNIT_ASSERT( other->position() == other->capacity() );
    CPPUNIT_ASSERT( testBuffer1->position() == testBuffer1->capacity() );
    for( std::size_t ix = 0; ix < testBuffer1->capacity() - 1; ix++ ) {
        CPPUNIT_ASSERT( testBuffer1->get( ix ) == other->get( ix ) );
    }
    CPPUNIT_ASSERT( &ret == testBuffer1 );

    delete other;
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayBufferTest::testPutIndexed() {

    ByteBuffer* readOnly = testBuffer1->asReadOnlyBuffer();
    readOnly->clear();
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a ReadOnlyBufferException",
        readOnly->put( 0, 0 ),
        ReadOnlyBufferException );
    delete readOnly;

    testBuffer1->clear();

    for( std::size_t i = 0; i < testBuffer1->capacity(); i++ ) {
        CPPUNIT_ASSERT( testBuffer1->position() == 0 );
        ByteBuffer& ret = testBuffer1->put(i, i );
        CPPUNIT_ASSERT( testBuffer1->get(i) == i );
        CPPUNIT_ASSERT( &ret == testBuffer1 );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a IndexOutOfBoundsException",
        testBuffer1->put( testBuffer1->limit(), 0 ),
        IndexOutOfBoundsException );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayBufferTest::testSlice() {

    CPPUNIT_ASSERT( testBuffer1->capacity() > SMALL_TEST_LENGTH );
    testBuffer1->position( 1 );
    testBuffer1->limit( testBuffer1->capacity() - 1 );

    ByteBuffer* slice = testBuffer1->slice();
    CPPUNIT_ASSERT( testBuffer1->isReadOnly() == slice->isReadOnly() );
    CPPUNIT_ASSERT( slice->position() == 0 );
    CPPUNIT_ASSERT( slice->limit() == testBuffer1->remaining() );
    CPPUNIT_ASSERT( slice->capacity() == testBuffer1->remaining() );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a InvalidMarkException",
        slice->reset(),
        InvalidMarkException );

    // slice share the same content with buf
    for( std::size_t ix = 0; ix < slice->capacity(); ++ix ){
        slice->put( ix, testData1[ix] );
    }

    for( std::size_t ix = 0; ix < slice->capacity(); ix++ ) {
        CPPUNIT_ASSERT( testBuffer1->get( ix + 1 ) == slice->get( ix ) );
    }
    testBuffer1->put( 2, 100 );
    CPPUNIT_ASSERT( slice->get(1) == 100 );

    delete slice;
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayBufferTest::testToString() {

    std::string str = testBuffer1->toString();
    CPPUNIT_ASSERT( str.find( "Byte" ) == 0 || str.find( "byte" ) == 0 );
    CPPUNIT_ASSERT( str.find( Integer::toString( testBuffer1->position() ) ) > 0 );
    CPPUNIT_ASSERT( str.find( Integer::toString( testBuffer1->limit() ) ) > 0 );
    CPPUNIT_ASSERT( str.find( Integer::toString( testBuffer1->capacity() ) ) > 0 );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayBufferTest::testGetChar() {

    std::vector<char> chars;
    for( std::size_t i = 0; i < testBuffer1->capacity(); i++ ) {
        testBuffer1->put( i, (char)i);
        chars.push_back( (char)i );
    }
    testBuffer1->clear();

    for( std::size_t i = 0; testBuffer1->remaining() != 0; i++ ) {
        CPPUNIT_ASSERT( testBuffer1->getChar() == chars[i] );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a BufferUnderflowException",
        testBuffer1->getChar(),
        BufferUnderflowException );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayBufferTest::testGetChar2() {

    std::vector<char> chars;
    for( std::size_t i = 0; i < testBuffer1->capacity(); i++ ) {
        testBuffer1->put( i, (char)i);
        chars.push_back( (char)i );
    }
    testBuffer1->clear();

    for( std::size_t i = 0; i < testBuffer1->capacity(); i++ ) {
        CPPUNIT_ASSERT( testBuffer1->getChar( i ) == chars[i] );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a IndexOutOfBoundsException",
        testBuffer1->getChar( testBuffer1->capacity() + 1 ),
        IndexOutOfBoundsException );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayBufferTest::testPutChar() {

    ByteBuffer* readOnly = testBuffer1->asReadOnlyBuffer();
    readOnly->clear();
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a ReadOnlyBufferException",
        readOnly->putChar( (char)1 ),
        ReadOnlyBufferException );
    delete readOnly;

    testBuffer1->clear();

    for( std::size_t i = 0; testBuffer1->remaining() > 0; i++ ) {

        testBuffer1->mark();
        testBuffer1->putChar( (char)i );
        testBuffer1->reset();
        CPPUNIT_ASSERT( testBuffer1->get() == (char)i );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a BufferOverflowException",
        testBuffer1->putChar( 'A' ),
        BufferOverflowException );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayBufferTest::testPutChar2() {

    ByteBuffer* readOnly = testBuffer1->asReadOnlyBuffer();
    readOnly->clear();
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a ReadOnlyBufferException",
        readOnly->putChar( (char)1 ),
        ReadOnlyBufferException );
    delete readOnly;

    testBuffer1->clear();

    std::size_t i = 0;
    for( ; i < testBuffer1->capacity(); i++ ) {

        testBuffer1->mark();
        testBuffer1->putChar( i, (char)i );
        testBuffer1->reset();
        CPPUNIT_ASSERT( testBuffer1->get( i ) == (char)i );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a IndexOutOfBoundsException",
        testBuffer1->putChar( i, 'A' ),
        IndexOutOfBoundsException );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayBufferTest::testGetDouble() {

    std::vector<double> values;
    for( std::size_t i = 0; i < testBuffer1->capacity() / sizeof( double ); i++ ) {
        testBuffer1->putDouble( (double)i );
        values.push_back( (double)i );
    }
    testBuffer1->clear();

    for( std::size_t i = 0;
         testBuffer1->remaining() >= sizeof( double ); i++ ) {

        CPPUNIT_ASSERT( testBuffer1->getDouble() == values[i] );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a BufferUnderflowException",
        testBuffer1->getDouble(),
        BufferUnderflowException );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayBufferTest::testGetDouble2() {

    std::vector<double> values;
    for( std::size_t i = 0; i < testBuffer1->capacity() / sizeof( double ); i++ ) {
        testBuffer1->putDouble( (double)i );
        values.push_back( (double)i );
    }
    testBuffer1->clear();

    std::size_t i = 0;
    std::size_t j = 0;

    for( ; ( testBuffer1->capacity() - i ) >= sizeof( double ); i += sizeof( double ), j++ ) {
        CPPUNIT_ASSERT( testBuffer1->getDouble( i ) == values[j] );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a IndexOutOfBoundsException",
        testBuffer1->getDouble( i ),
        IndexOutOfBoundsException );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayBufferTest::testPutDouble() {

    ByteBuffer* readOnly = testBuffer1->asReadOnlyBuffer();
    readOnly->clear();
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a ReadOnlyBufferException",
        readOnly->putDouble( 1.64684 ),
        ReadOnlyBufferException );
    delete readOnly;

    testBuffer1->clear();

    for( std::size_t i = 0; testBuffer1->remaining() >= sizeof( double );
         i += sizeof( double ) ) {

        testBuffer1->mark();
        testBuffer1->putDouble( i + 48.25136 );
        testBuffer1->reset();
        CPPUNIT_ASSERT( Double::doubleToLongBits( testBuffer1->getDouble() )==
                        Double::doubleToLongBits( 48.25136 + i ) );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a BufferOverflowException",
        testBuffer1->putDouble( 3.14159 ),
        BufferOverflowException );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayBufferTest::testPutDouble2() {

    ByteBuffer* readOnly = testBuffer1->asReadOnlyBuffer();
    readOnly->clear();
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a ReadOnlyBufferException",
        readOnly->putDouble( (double)1.004 ),
        ReadOnlyBufferException );
    delete readOnly;

    testBuffer1->clear();

    std::size_t i = 0;
    for( ; ( testBuffer1->capacity() - i ) >= sizeof( double ); i += sizeof( double ) ) {

        testBuffer1->mark();
        testBuffer1->putDouble( i, i + 99.99 );
        testBuffer1->reset();
        CPPUNIT_ASSERT( Double::doubleToLongBits( testBuffer1->getDouble( i ) )==
                        Double::doubleToLongBits( 99.99 + i ) );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a IndexOutOfBoundsException",
        testBuffer1->putDouble( i, 3.14159 ),
        IndexOutOfBoundsException );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayBufferTest::testGetFloat() {

    std::vector<float> values;
    for( std::size_t i = 0; i < testBuffer1->capacity() / sizeof( float ); i++ ) {
        testBuffer1->putFloat( (float)i );
        values.push_back( (float)i );
    }
    testBuffer1->clear();

    for( std::size_t i = 0;
         testBuffer1->remaining() >= sizeof( float ); i++ ) {

        CPPUNIT_ASSERT( testBuffer1->getFloat() == values[i] );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a BufferUnderflowException",
        testBuffer1->getFloat(),
        BufferUnderflowException );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayBufferTest::testGetFloat2() {

    std::vector<float> values;
    for( std::size_t i = 0; i < testBuffer1->capacity() / sizeof( float ); i++ ) {
        testBuffer1->putFloat( (float)i );
        values.push_back( (float)i );
    }
    testBuffer1->clear();

    std::size_t i = 0;
    std::size_t j = 0;

    for( ; ( testBuffer1->capacity() - i ) >= sizeof( float ); i += sizeof( float ), j++ ) {
        CPPUNIT_ASSERT( testBuffer1->getFloat( i ) == values[j] );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a IndexOutOfBoundsException",
        testBuffer1->getFloat( i ),
        IndexOutOfBoundsException );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayBufferTest::testPutFloat() {

    ByteBuffer* readOnly = testBuffer1->asReadOnlyBuffer();
    readOnly->clear();
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a ReadOnlyBufferException",
        readOnly->putFloat( 1.64684f ),
        ReadOnlyBufferException );
    delete readOnly;

    testBuffer1->clear();

    for( std::size_t i = 0; testBuffer1->remaining() >= sizeof( float );
         i += sizeof( float ) ) {

        testBuffer1->mark();
        testBuffer1->putFloat( i + 48.25136f );
        testBuffer1->reset();
        CPPUNIT_ASSERT( Float::floatToIntBits( testBuffer1->getFloat() ) ==
                        Float::floatToIntBits( 48.25136f + i ) );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a BufferOverflowException",
        testBuffer1->putFloat( 3.14159f ),
        BufferOverflowException );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayBufferTest::testPutFloat2() {

    ByteBuffer* readOnly = testBuffer1->asReadOnlyBuffer();
    readOnly->clear();
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a ReadOnlyBufferException",
        readOnly->putFloat( (float)1.004f ),
        ReadOnlyBufferException );
    delete readOnly;

    testBuffer1->clear();

    std::size_t i = 0;
    for( ; ( testBuffer1->capacity() - i ) >= sizeof( float ); i += sizeof( float ) ) {

        testBuffer1->mark();
        testBuffer1->putFloat( i, i + 99.99f );
        testBuffer1->reset();
        CPPUNIT_ASSERT( Float::floatToIntBits( testBuffer1->getFloat( i ) )==
                        Float::floatToIntBits( 99.99f + i ) );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a IndexOutOfBoundsException",
        testBuffer1->putFloat( i, 3.14159f ),
        IndexOutOfBoundsException );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayBufferTest::testGetLong() {

    std::vector<long long> values;
    for( std::size_t i = 0; i < testBuffer1->capacity() / sizeof( long long ); i++ ) {
        testBuffer1->putLong( (long long)i );
        values.push_back( (long long)i );
    }
    testBuffer1->clear();

    for( std::size_t i = 0;
         testBuffer1->remaining() >= sizeof( long long ); i++ ) {

        CPPUNIT_ASSERT( testBuffer1->getLong() == values[i] );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a BufferUnderflowException",
        testBuffer1->getLong(),
        BufferUnderflowException );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayBufferTest::testGetLong2() {

    std::vector<long long> values;
    for( std::size_t i = 0; i < testBuffer1->capacity() / sizeof( long long ); i++ ) {
        testBuffer1->putLong( (long long)i );
        values.push_back( (long long)i );
    }
    testBuffer1->clear();

    std::size_t i = 0;
    std::size_t j = 0;

    for( ; ( testBuffer1->capacity() - i ) >= sizeof( long long ); i += sizeof( long long ), j++ ) {
        CPPUNIT_ASSERT( testBuffer1->getLong( i ) == values[j] );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a IndexOutOfBoundsException",
        testBuffer1->getLong( i ),
        IndexOutOfBoundsException );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayBufferTest::testPutLong() {

    ByteBuffer* readOnly = testBuffer1->asReadOnlyBuffer();
    readOnly->clear();
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a ReadOnlyBufferException",
        readOnly->putLong( 15474 ),
        ReadOnlyBufferException );
    delete readOnly;

    testBuffer1->clear();

    for( std::size_t i = 0; testBuffer1->remaining() >= sizeof( long long );
         i += sizeof( long long ) ) {

        testBuffer1->mark();
        testBuffer1->putLong( i + 48 );
        testBuffer1->reset();
        CPPUNIT_ASSERT( testBuffer1->getLong() == ( i + 48 ) );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a BufferOverflowException",
        testBuffer1->putLong( 3 ),
        BufferOverflowException );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayBufferTest::testPutLong2() {

    ByteBuffer* readOnly = testBuffer1->asReadOnlyBuffer();
    readOnly->clear();
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a ReadOnlyBufferException",
        readOnly->putLong( (long long)85 ),
        ReadOnlyBufferException );
    delete readOnly;

    testBuffer1->clear();

    std::size_t i = 0;
    for( ; ( testBuffer1->capacity() - i ) >= sizeof( long long ); i += sizeof( long long ) ) {

        testBuffer1->mark();
        testBuffer1->putLong( i, i + 99 );
        testBuffer1->reset();
        CPPUNIT_ASSERT( testBuffer1->getLong( i ) == i + 99 );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a IndexOutOfBoundsException",
        testBuffer1->putLong( i, 3 ),
        IndexOutOfBoundsException );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayBufferTest::testGetInt() {

    std::vector<int> values;
    for( std::size_t i = 0; i < testBuffer1->capacity() / sizeof( int ); i++ ) {
        testBuffer1->putInt( (int)i );
        values.push_back( (int)i );
    }
    testBuffer1->clear();

    for( std::size_t i = 0;
         testBuffer1->remaining() >= sizeof( int ); i++ ) {

        CPPUNIT_ASSERT( testBuffer1->getInt() == values[i] );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a BufferUnderflowException",
        testBuffer1->getInt(),
        BufferUnderflowException );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayBufferTest::testGetInt2() {

    std::vector<int> values;
    for( std::size_t i = 0; i < testBuffer1->capacity() / sizeof( int ); i++ ) {
        testBuffer1->putInt( (int)i );
        values.push_back( (int)i );
    }
    testBuffer1->clear();

    std::size_t i = 0;
    std::size_t j = 0;

    for( ; ( testBuffer1->capacity() - i ) >= sizeof( int ); i += sizeof( int ), j++ ) {
        CPPUNIT_ASSERT( testBuffer1->getInt( i ) == values[j] );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a IndexOutOfBoundsException",
        testBuffer1->getInt( i ),
        IndexOutOfBoundsException );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayBufferTest::testPutInt() {

    ByteBuffer* readOnly = testBuffer1->asReadOnlyBuffer();
    readOnly->clear();
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a ReadOnlyBufferException",
        readOnly->putInt( 15474 ),
        ReadOnlyBufferException );
    delete readOnly;

    testBuffer1->clear();

    for( std::size_t i = 0; testBuffer1->remaining() >= sizeof( int );
         i += sizeof( int ) ) {

        testBuffer1->mark();
        testBuffer1->putInt( i + 48 );
        testBuffer1->reset();
        CPPUNIT_ASSERT( testBuffer1->getInt() == (int)( i + 48 ) );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a BufferOverflowException",
        testBuffer1->putInt( 3 ),
        BufferOverflowException );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayBufferTest::testPutInt2() {

    ByteBuffer* readOnly = testBuffer1->asReadOnlyBuffer();
    readOnly->clear();
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a ReadOnlyBufferException",
        readOnly->putInt( (int)85 ),
        ReadOnlyBufferException );
    delete readOnly;

    testBuffer1->clear();

    std::size_t i = 0;
    for( ; ( testBuffer1->capacity() - i ) >= sizeof( int ); i += sizeof( int ) ) {

        testBuffer1->mark();
        testBuffer1->putInt( i, i + 99 );
        testBuffer1->reset();
        CPPUNIT_ASSERT( testBuffer1->getInt( i ) == (int)(i + 99) );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a IndexOutOfBoundsException",
        testBuffer1->putInt( i, 3 ),
        IndexOutOfBoundsException );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayBufferTest::testGetShort() {

    std::vector<short> values;
    for( std::size_t i = 0; i < testBuffer1->capacity() / sizeof( short ); i++ ) {
        testBuffer1->putShort( (short)i );
        values.push_back( (short)i );
    }
    testBuffer1->clear();

    for( std::size_t i = 0;
         testBuffer1->remaining() >= sizeof( short ); i++ ) {

        CPPUNIT_ASSERT( testBuffer1->getShort() == values[i] );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a BufferUnderflowException",
        testBuffer1->getShort(),
        BufferUnderflowException );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayBufferTest::testGetShort2() {

    std::vector<short> values;
    for( std::size_t i = 0; i < testBuffer1->capacity() / sizeof( short ); i++ ) {
        testBuffer1->putShort( (short)i );
        values.push_back( (short)i );
    }
    testBuffer1->clear();

    std::size_t i = 0;
    std::size_t j = 0;

    for( ; ( testBuffer1->capacity() - i ) >= sizeof( short ); i += sizeof( short ), j++ ) {
        CPPUNIT_ASSERT( testBuffer1->getShort( i ) == values[j] );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a IndexOutOfBoundsException",
        testBuffer1->getShort( i ),
        IndexOutOfBoundsException );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayBufferTest::testPutShort() {

    ByteBuffer* readOnly = testBuffer1->asReadOnlyBuffer();
    readOnly->clear();
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a ReadOnlyBufferException",
        readOnly->putShort( 15474 ),
        ReadOnlyBufferException );
    delete readOnly;

    testBuffer1->clear();

    for( std::size_t i = 0; testBuffer1->remaining() >= sizeof( short );
         i += sizeof( short ) ) {

        testBuffer1->mark();
        testBuffer1->putShort( i + 48 );
        testBuffer1->reset();
        CPPUNIT_ASSERT( testBuffer1->getShort() == (short)( i + 48 ) );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a BufferOverflowException",
        testBuffer1->putShort( 3 ),
        BufferOverflowException );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayBufferTest::testPutShort2() {

    ByteBuffer* readOnly = testBuffer1->asReadOnlyBuffer();
    readOnly->clear();
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a ReadOnlyBufferException",
        readOnly->putShort( (short)85 ),
        ReadOnlyBufferException );
    delete readOnly;

    testBuffer1->clear();

    std::size_t i = 0;
    for( ; ( testBuffer1->capacity() - i ) >= sizeof( short ); i += sizeof( short ) ) {

        testBuffer1->mark();
        testBuffer1->putShort( i, i + 99 );
        testBuffer1->reset();
        CPPUNIT_ASSERT( testBuffer1->getShort( i ) == (short)(i + 99) );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a IndexOutOfBoundsException",
        testBuffer1->putShort( i, 3 ),
        IndexOutOfBoundsException );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayBufferTest::testWrapNullArray() {

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException",
        testBuffer1->wrap( NULL, 0, 3 ),
        NullPointerException );
}
