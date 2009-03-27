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

#include "CharArrayBufferTest.h"
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
void CharArrayBufferTest::test() {

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
void CharArrayBufferTest::testArray() {

    testBuffer1->put( testData1, 0, testData1Size );
    testBuffer1->position( 0 );

    testBuffer1->mark();

    for( std::size_t ix = 0; ix < testBuffer1->capacity(); ++ix ) {
        CPPUNIT_ASSERT( testBuffer1->get() == testData1[ix] );
    }

    testBuffer1->reset();

    char* array = testBuffer1->array();
    CPPUNIT_ASSERT( array != NULL );

    for( std::size_t ix = 0; ix < testBuffer1->capacity(); ++ix ) {
        CPPUNIT_ASSERT( array[ix] == testData1[ix] );
    }
}

////////////////////////////////////////////////////////////////////////////////
void CharArrayBufferTest::testArrayOffset() {

    testBuffer1->put( testData1, 0, testData1Size );
    CPPUNIT_ASSERT( testBuffer1->arrayOffset() == 0 );
    testBuffer1->rewind();
    CPPUNIT_ASSERT( testBuffer1->arrayOffset() == 0 );

    testBuffer1->get();
    CharBuffer* sliced = testBuffer1->slice();
    CPPUNIT_ASSERT( sliced->arrayOffset() == 1 );
    delete sliced;
}

////////////////////////////////////////////////////////////////////////////////
void CharArrayBufferTest::testReadOnlyArray() {

    CharBuffer* readOnly = testBuffer1->asReadOnlyBuffer();

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
void CharArrayBufferTest::testAsReadOnlyBuffer() {

    testBuffer1->clear();
    testBuffer1->mark();
    testBuffer1->position( testBuffer1->limit() );

    // readonly's contents should be the same as buf
    CharBuffer* readOnly = testBuffer1->asReadOnlyBuffer();

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
void CharArrayBufferTest::testCompact() {

    // readonly's contents should be the same as buf
    CharBuffer* readOnly = testBuffer1->asReadOnlyBuffer();

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

    CharBuffer& ret = testBuffer1->compact();

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
void CharArrayBufferTest::testCompareTo() {

    // compare to self
    CPPUNIT_ASSERT( 0 == testBuffer1->compareTo( *testBuffer1 ) );

    CPPUNIT_ASSERT( testBuffer1->capacity() > SMALL_TEST_LENGTH );
    testBuffer1->clear();
    CharBuffer* other = CharBuffer::allocate( testBuffer1->capacity() );

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

    char* data = new char[21];
    memset( data, 0, 21 );
    CharBuffer* empty = CharBuffer::allocate(21);
    CharBuffer* wrapped = CharBuffer::wrap( data, (std::size_t)0, (std::size_t)21 );

    CPPUNIT_ASSERT( wrapped->compareTo( *empty ) == 0 );

    delete empty;
    delete wrapped;
    delete other;
    delete [] data;
}

////////////////////////////////////////////////////////////////////////////////
void CharArrayBufferTest::testDuplicate() {

    testBuffer1->clear();
    testBuffer1->mark();
    testBuffer1->position( testBuffer1->limit() );

    for( std::size_t ix = 0; ix < testData1Size; ++ix ){
        testBuffer1->put( ix, testData1[ix] );
    }

    // duplicate's contents should be the same as buf
    CharBuffer* duplicate = testBuffer1->duplicate();
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
void CharArrayBufferTest::testEquals() {

    // equal to self
    CPPUNIT_ASSERT( testBuffer1->equals( *testBuffer1 ) );
    CharBuffer* readOnly = testBuffer1->asReadOnlyBuffer();
    CPPUNIT_ASSERT( testBuffer1->equals( *readOnly ) );
    CharBuffer* duplicate = testBuffer1->duplicate();
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
void CharArrayBufferTest::testGet() {

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
void CharArrayBufferTest::testGetbyteArray() {

    std::vector<char> array;
    array.resize( 1 );
    testBuffer1->clear();

    for( std::size_t i = 0; i < testBuffer1->capacity(); i++ ) {

        CPPUNIT_ASSERT( testBuffer1->position() == i );
        CharBuffer& ret = testBuffer1->get( array );
        CPPUNIT_ASSERT( array[0] == testBuffer1->get( i ) );
        CPPUNIT_ASSERT( &ret == testBuffer1 );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a BufferUnderflowException",
        testBuffer1->get( array ),
        BufferUnderflowException );
}

////////////////////////////////////////////////////////////////////////////////
void CharArrayBufferTest::testGetbyteArray2() {

    testBuffer1->clear();
    char* array = new char[testBuffer1->capacity()];
    char* array2 = new char[testBuffer1->capacity() + 1];

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
    CharBuffer& ret = testBuffer1->get( array, 0, testBuffer1->capacity() );
    CPPUNIT_ASSERT( testBuffer1->position() == testBuffer1->capacity() );
    for( std::size_t ix = 0; ix < testBuffer1->capacity() - 1; ix++ ) {
        CPPUNIT_ASSERT( testBuffer1->get( ix ) == array[ix] );
    }

    CPPUNIT_ASSERT( &ret == testBuffer1);

    delete [] array;
    delete [] array2;
}

////////////////////////////////////////////////////////////////////////////////
void CharArrayBufferTest::testGetWithIndex() {

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
void CharArrayBufferTest::testPutbyte() {

    CharBuffer* readOnly = testBuffer1->asReadOnlyBuffer();

    readOnly->clear();

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a ReadOnlyBufferException",
        readOnly->put( 0 ),
        ReadOnlyBufferException );

    delete readOnly;

    testBuffer1->clear();
    for( size_t i = 0; i < testBuffer1->capacity(); i++) {
        CPPUNIT_ASSERT( testBuffer1->position() == i );
        CharBuffer& ret = testBuffer1->put( (char)i );
        CPPUNIT_ASSERT( testBuffer1->get(i) == (char)i );
        CPPUNIT_ASSERT( &ret == testBuffer1 );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a BufferOverflowException",
        testBuffer1->put( 0 ),
        BufferOverflowException );
}

////////////////////////////////////////////////////////////////////////////////
void CharArrayBufferTest::testPutbyteArray() {

    std::vector<char> array;
    array.push_back( 127 );

    CharBuffer* readOnly = testBuffer1->asReadOnlyBuffer();
    readOnly->clear();
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a ReadOnlyBufferException",
        readOnly->put( array ),
        ReadOnlyBufferException );
    delete readOnly;

    testBuffer1->clear();
    for( std::size_t i = 0; i < testBuffer1->capacity(); i++ ) {
        CPPUNIT_ASSERT( testBuffer1->position() == i );
        array[0] = (char)i;
        CharBuffer& ret = testBuffer1->put( array );
        CPPUNIT_ASSERT( testBuffer1->get(i) == (char)i );
        CPPUNIT_ASSERT( &ret == testBuffer1 );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a BufferOverflowException",
        testBuffer1->put( array ),
        BufferOverflowException );
}

////////////////////////////////////////////////////////////////////////////////
void CharArrayBufferTest::testPutbyteArray2() {

    testBuffer1->clear();
    char* array = new char[testBuffer1->capacity()];
    char* array2 = new char[testBuffer1->capacity() + 1];

    for( std::size_t ix = 0; ix < testBuffer1->capacity(); ++ix ){
        array[ix] = 0;
    }
    for( std::size_t ix = 0; ix < testBuffer1->capacity() + 1; ++ix ){
        array2[ix] = 0;
    }

    CharBuffer* readOnly = testBuffer1->asReadOnlyBuffer();
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

    CharBuffer& ret = testBuffer1->put( array, 0, testBuffer1->capacity() );
    CPPUNIT_ASSERT( testBuffer1->position() == testBuffer1->capacity() );
    for( std::size_t ix = 0; ix < testBuffer1->capacity() - 1; ix++ ) {
        CPPUNIT_ASSERT( testBuffer1->get( ix ) == array[ix] );
    }
    CPPUNIT_ASSERT( &ret == testBuffer1 );

    delete [] array;
    delete [] array2;
}

////////////////////////////////////////////////////////////////////////////////
void CharArrayBufferTest::testPutCharBuffer() {

    CharBuffer* other = CharBuffer::allocate( testBuffer1->capacity() );

    CharBuffer* readOnly = testBuffer1->asReadOnlyBuffer();
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

    CharBuffer* toBig = testBuffer1->allocate( testBuffer1->capacity() + 1 );
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
    CharBuffer& ret = testBuffer1->put( *other );
    CPPUNIT_ASSERT( other->position() == other->capacity() );
    CPPUNIT_ASSERT( testBuffer1->position() == testBuffer1->capacity() );
    for( std::size_t ix = 0; ix < testBuffer1->capacity() - 1; ix++ ) {
        CPPUNIT_ASSERT( testBuffer1->get( ix ) == other->get( ix ) );
    }
    CPPUNIT_ASSERT( &ret == testBuffer1 );

    delete other;
}

////////////////////////////////////////////////////////////////////////////////
void CharArrayBufferTest::testPutIndexed() {

    CharBuffer* readOnly = testBuffer1->asReadOnlyBuffer();
    readOnly->clear();
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a ReadOnlyBufferException",
        readOnly->put( 0, 0 ),
        ReadOnlyBufferException );
    delete readOnly;

    testBuffer1->clear();

    for( std::size_t i = 0; i < testBuffer1->capacity(); i++ ) {
        CPPUNIT_ASSERT( testBuffer1->position() == 0 );
        CharBuffer& ret = testBuffer1->put( i, (char)i );
        CPPUNIT_ASSERT( testBuffer1->get(i) == (char)i );
        CPPUNIT_ASSERT( &ret == testBuffer1 );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a IndexOutOfBoundsException",
        testBuffer1->put( testBuffer1->limit(), 0 ),
        IndexOutOfBoundsException );
}

////////////////////////////////////////////////////////////////////////////////
void CharArrayBufferTest::testSlice() {

    CPPUNIT_ASSERT( testBuffer1->capacity() > SMALL_TEST_LENGTH );
    testBuffer1->position( 1 );
    testBuffer1->limit( testBuffer1->capacity() - 1 );

    CharBuffer* slice = testBuffer1->slice();
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
void CharArrayBufferTest::testToString() {

    string expected = "";
    for( std::size_t i = testBuffer1->position(); i < testBuffer1->limit(); i++ ) {
        expected += testBuffer1->get(i);
    }
    string str = testBuffer1->toString();
    CPPUNIT_ASSERT( expected == str );
}

////////////////////////////////////////////////////////////////////////////////
void CharArrayBufferTest::testWrapNullArray() {

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException",
        testBuffer1->wrap( (char*)NULL, 0, 3 ),
        NullPointerException );
}

////////////////////////////////////////////////////////////////////////////////
void CharArrayBufferTest::testCharAt() {

    for( std::size_t i = 0; i < testBuffer1->remaining(); i++ ) {
        CPPUNIT_ASSERT( testBuffer1->get( testBuffer1->position() + i ) ==
                        testBuffer1->charAt(i) );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a IndexOutOfBoundsException",
        testBuffer1->charAt( testBuffer1->remaining() ),
        IndexOutOfBoundsException );
}

////////////////////////////////////////////////////////////////////////////////
void CharArrayBufferTest::testLength() {
    CPPUNIT_ASSERT( testBuffer1->length() == testBuffer1->remaining() );
}

////////////////////////////////////////////////////////////////////////////////
void CharArrayBufferTest::testSubSequence() {

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a IndexOutOfBoundsException",
        testBuffer1->subSequence(testBuffer1->length() + 1, testBuffer1->length() + 1),
        IndexOutOfBoundsException );

    CharSequence* sub1 = testBuffer1->subSequence(
        testBuffer1->length(), testBuffer1->length() );
    CPPUNIT_ASSERT( sub1->length() == 0 );
    delete sub1;

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a IndexOutOfBoundsException",
        testBuffer1->subSequence( 1, 0 ),
        IndexOutOfBoundsException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a IndexOutOfBoundsException",
        testBuffer1->subSequence( 1, testBuffer1->length() + 1 ),
        IndexOutOfBoundsException );

    CharSequence* sub2 = testBuffer1->subSequence( 0, testBuffer1->length() );
    CPPUNIT_ASSERT( sub2->toString() == testBuffer1->toString() );
    delete sub2;
}

////////////////////////////////////////////////////////////////////////////////
void CharArrayBufferTest::testPutString() {

    testBuffer1->clear();
    for( std::size_t i = 0; i < testBuffer1->capacity(); i++ ) {
        CPPUNIT_ASSERT( testBuffer1->position() == i );
        string testString = " ";
        testString[0] = (char)i;
        CharBuffer& ret = testBuffer1->put( testString );
        CPPUNIT_ASSERT( (char)testBuffer1->get(i) == (char)i );
        CPPUNIT_ASSERT( &ret == testBuffer1 );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a BufferOverflowException",
        testBuffer1->put( "ASDFGHJKJKL" ),
        BufferOverflowException );
}

////////////////////////////////////////////////////////////////////////////////
void CharArrayBufferTest::testPutStringWithArgs() {

    testBuffer1->clear();
    string str;
    str.resize( testBuffer1->capacity() );

    // Throw a BufferOverflowException and no character is transfered to
    // CharBuffer
    std::string toBig = "";
    toBig.resize( testBuffer1->capacity() + 1 );
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a BufferOverflowException",
        testBuffer1->put( toBig, 0, testBuffer1->capacity() + 1 ),
        BufferOverflowException );

    CPPUNIT_ASSERT( 0 == testBuffer1->position() );

    testBuffer1->clear();

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a IndexOutOfBoundsException",
        testBuffer1->put( str, str.length() + 1, str.length() + 2 ),
        IndexOutOfBoundsException );

    testBuffer1->put( str, str.length(), str.length() );
    CPPUNIT_ASSERT( testBuffer1->position() == 0 );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a IndexOutOfBoundsException",
        testBuffer1->put( str, 2, 1 ),
        IndexOutOfBoundsException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a IndexOutOfBoundsException",
        testBuffer1->put( str, 2, str.length() + 1 ),
        IndexOutOfBoundsException );

    CPPUNIT_ASSERT( testBuffer1->position() == 0 );

    str.resize( testBuffer1->capacity() );

    CharBuffer& ret = testBuffer1->put( str, 0, str.length() );
    CPPUNIT_ASSERT( testBuffer1->position() == testBuffer1->capacity() );

    for( std::size_t ix = 0; ix < testBuffer1->capacity(); ++ix ) {
        CPPUNIT_ASSERT( testBuffer1->get( ix ) == str.at( ix ) );
    }

    CPPUNIT_ASSERT( &ret == testBuffer1 );
}

////////////////////////////////////////////////////////////////////////////////
void CharArrayBufferTest::testAppendSelf() {

    CharBuffer* cb = CharBuffer::allocate(10);
    CharBuffer* cb2 = cb->duplicate();

    cb->append( cb );
    CPPUNIT_ASSERT( 10 == cb->position() );
    cb->clear();
    CPPUNIT_ASSERT( cb2->equals( *cb ) );

    delete cb2;
    cb->put( "abc" );
    cb2 = cb->duplicate();
    cb->append( cb );
    CPPUNIT_ASSERT( 10 == cb->position() );
    cb->clear();
    cb2->clear();
    CPPUNIT_ASSERT( cb2->equals( *cb ) );

    delete cb2;
    cb->put( "edfg" );
    cb->clear();
    cb2 = cb->duplicate();
    cb->append( cb );
    CPPUNIT_ASSERT( 10 == cb->position() );
    cb->clear();
    cb2->clear();
    CPPUNIT_ASSERT( cb->equals( *cb2 ) );

    delete cb;
    delete cb2;
}

////////////////////////////////////////////////////////////////////////////////
void CharArrayBufferTest::testAppendOverFlow() {

    CharBuffer* cb = CharBuffer::allocate( 1 );
    CharSequence* cs = testBuffer1->subSequence( 0, 25 );

    cb->put( 'A' );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a BufferOverflowException",
        cb->append('C'),
        BufferOverflowException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a BufferOverflowException",
        cb->append( cs ),
        BufferOverflowException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a BufferOverflowException",
        cb->append( cs, 1, 2 ),
        BufferOverflowException );

    delete cb;
    delete cs;
}

////////////////////////////////////////////////////////////////////////////////
void CharArrayBufferTest::testReadOnlyMap() {

    CharBuffer* cb = testBuffer1->asReadOnlyBuffer();
    MyCharSequence cs( "String" );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a ReadOnlyBufferException",
        cb->append( 'A' ),
        ReadOnlyBufferException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a ReadOnlyBufferException",
        cb->append( &cs ),
        ReadOnlyBufferException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a ReadOnlyBufferException",
        cb->append( &cs, 1, 2 ),
        ReadOnlyBufferException );

    delete cb;
}

////////////////////////////////////////////////////////////////////////////////
void CharArrayBufferTest::testAppendCNormal() {
    CharBuffer* cb = CharBuffer::allocate(2);
    cb->put('A');
    CPPUNIT_ASSERT( cb == &( cb->append('B') ) );
    CPPUNIT_ASSERT( 'B' == cb->get( 1 ) );
    delete cb;
}

////////////////////////////////////////////////////////////////////////////////
void CharArrayBufferTest::testAppendCharSequenceNormal() {

    CharBuffer* cb = CharBuffer::allocate(10);
    cb->put('A');
    MyCharSequence cs( "String" );

    CPPUNIT_ASSERT( cb == &( cb->append( &cs ) ) );
    cb->flip();
    CPPUNIT_ASSERT( MyCharSequence("AString").toString() == cb->toString() );
    cb->append( (const lang::CharSequence*)NULL );
    cb->flip();
    CPPUNIT_ASSERT( cb->toString() == "null" );

    delete cb;
}

////////////////////////////////////////////////////////////////////////////////
void CharArrayBufferTest::testAppendCharSequenceIINormal() {

    CharBuffer* cb = CharBuffer::allocate( 10 );
    cb->put( 'A' );
    MyCharSequence cs( "String" );

    CPPUNIT_ASSERT( cb == &( cb->append( &cs, 1, 3 ) ) );
    cb->flip();
    CPPUNIT_ASSERT( "Atr" == cb->toString() );

    cb->append( (const lang::CharSequence*)NULL, 0, 1 );
    cb->flip();
    CPPUNIT_ASSERT( "n" == cb->toString() );
    delete cb;
}

////////////////////////////////////////////////////////////////////////////////
void CharArrayBufferTest::testAppendCharSequenceII_IllegalArgument() {

    CharBuffer* cb = CharBuffer::allocate( 10 );
    MyCharSequence cs( "String" );

    cb->append( &cs, 0, 0 );
    cb->append( &cs, 2, 2 );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a IndexOutOfBoundsException",
        cb->append( &cs, 3, 2 ),
        IndexOutOfBoundsException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a IndexOutOfBoundsException",
        cb->append( &cs, 3, 0 ),
        IndexOutOfBoundsException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a IndexOutOfBoundsException",
        cb->append( &cs, 3, 110 ),
        IndexOutOfBoundsException );

    delete cb;
}

////////////////////////////////////////////////////////////////////////////////
void CharArrayBufferTest::testReadCharBuffer() {

    std::vector<char> buffer;
    buffer.push_back('S');
    buffer.push_back('t');
    buffer.push_back('r');
    buffer.push_back('i');
    buffer.push_back('n');
    buffer.push_back('g');

    CharBuffer* source = CharBuffer::wrap( buffer );
    CharBuffer* target = CharBuffer::allocate(10);

    CPPUNIT_ASSERT( 6 == source->read( target ) );
    target->flip();
    CPPUNIT_ASSERT( "String" == target->toString() );

    // return -1 when nothing to read
    CPPUNIT_ASSERT( string::npos == source->read( target ) );

    // NullPointerException
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException",
        source->read( NULL ),
        NullPointerException );

    delete source;
    delete target;
}

////////////////////////////////////////////////////////////////////////////////
void CharArrayBufferTest::testReadReadOnly() {

    CharBuffer* source = CharBuffer::wrap( testData1, 0, testData1Size );
    CharBuffer* target = testBuffer1->asReadOnlyBuffer();

    // NullPointerException
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a ReadOnlyBufferException",
        source->read( target ),
        ReadOnlyBufferException );

    // if target has no remaining, needn't to check the isReadOnly
    target->flip();
    CPPUNIT_ASSERT( 0 == source->read( target ) );

    delete source;
    delete target;
}

////////////////////////////////////////////////////////////////////////////////
void CharArrayBufferTest::testReadOverflow() {

    std::vector<char> buffer;
    buffer.push_back('S');
    CharBuffer* source = CharBuffer::wrap( buffer );
    CharBuffer* target = CharBuffer::allocate( 1 );

    CPPUNIT_ASSERT( 1 == source->read(target) );
    target->flip();
    CPPUNIT_ASSERT( "S" == target->toString() );
    CPPUNIT_ASSERT( 1 == source->position() );

    delete source;
    delete target;
}

////////////////////////////////////////////////////////////////////////////////
void CharArrayBufferTest::testReadSelf() {

    CharBuffer* source = CharBuffer::wrap( testData1, 0, testData1Size );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a IllegalArgumentException",
        source->read( source ),
        IllegalArgumentException );

    delete source;
}
