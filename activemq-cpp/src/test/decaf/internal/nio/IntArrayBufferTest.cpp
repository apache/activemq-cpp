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

#include "IntArrayBufferTest.h"
#include <decaf/lang/Integer.h>
#include <decaf/lang/Double.h>
#include <decaf/lang/Float.h>

using namespace std;
using namespace decaf;
using namespace decaf::nio;
using namespace decaf::internal::nio;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
void IntArrayBufferTest::test() {

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
void IntArrayBufferTest::testArray() {

    int* array = testBuffer1->array();

    testBuffer1->put( 0, 10 );
    CPPUNIT_ASSERT( array[0] == 10.0 );

    assertContentEquals(
        testBuffer1, array, testBuffer1->arrayOffset(), testBuffer1->capacity() );

    loadTestData1( array, testBuffer1->arrayOffset(), testBuffer1->capacity() );
    assertContentEquals(
        testBuffer1, array, testBuffer1->arrayOffset(), testBuffer1->capacity()) ;

    loadTestData2( array, testBuffer1->arrayOffset(), testBuffer1->capacity());
    assertContentEquals(
        testBuffer1, array, testBuffer1->arrayOffset(), testBuffer1->capacity() );

    loadTestData1( testBuffer1 );
    assertContentEquals(
        testBuffer1, array, testBuffer1->arrayOffset(), testBuffer1->capacity() );

    loadTestData2( testBuffer1 );
    assertContentEquals(
        testBuffer1, array, testBuffer1->arrayOffset(), testBuffer1->capacity() );
}

////////////////////////////////////////////////////////////////////////////////
void IntArrayBufferTest::testArrayOffset() {

    int* array = testBuffer1->array();

    assertContentEquals(testBuffer1, array, testBuffer1->arrayOffset(), testBuffer1->capacity());

    loadTestData1(array, testBuffer1->arrayOffset(), testBuffer1->capacity());
    assertContentEquals(testBuffer1, array, testBuffer1->arrayOffset(), testBuffer1->capacity());

    loadTestData2(array, testBuffer1->arrayOffset(), testBuffer1->capacity());
    assertContentEquals(testBuffer1, array, testBuffer1->arrayOffset(), testBuffer1->capacity());

    loadTestData1(testBuffer1);
    assertContentEquals(testBuffer1, array, testBuffer1->arrayOffset(), testBuffer1->capacity());

    loadTestData2(testBuffer1);
    assertContentEquals(testBuffer1, array, testBuffer1->arrayOffset(), testBuffer1->capacity());
}

////////////////////////////////////////////////////////////////////////////////
void IntArrayBufferTest::testReadOnlyArray() {

    IntBuffer* readOnly = testBuffer1->asReadOnlyBuffer();

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
void IntArrayBufferTest::testAsReadOnlyBuffer() {

    testBuffer1->clear();
    testBuffer1->mark();
    testBuffer1->position( testBuffer1->limit() );

    // readonly's contents should be the same as testBuffer1
    IntBuffer* readOnly = testBuffer1->asReadOnlyBuffer();
    CPPUNIT_ASSERT( testBuffer1 != readOnly );
    CPPUNIT_ASSERT( readOnly->isReadOnly() );
    CPPUNIT_ASSERT( testBuffer1->position() == readOnly->position() );
    CPPUNIT_ASSERT( testBuffer1->limit() == readOnly->limit() );

    for( std::size_t i = 0; i < testBuffer1->capacity(); ++i ) {
        CPPUNIT_ASSERT( testBuffer1->get( i ) == readOnly->get( i ) );
    }

    // readOnly's position, mark, and limit should be independent to testBuffer1
    readOnly->reset();
    CPPUNIT_ASSERT( readOnly->position() == 0 );
    readOnly->clear();
    CPPUNIT_ASSERT( testBuffer1->position() == testBuffer1->limit() );
    testBuffer1->reset();
    CPPUNIT_ASSERT( testBuffer1->position() == 0 );

    delete readOnly;
}

////////////////////////////////////////////////////////////////////////////////
void IntArrayBufferTest::testCompact() {

    loadTestData1( testBuffer1 );

    // case: buffer is full
    testBuffer1->clear();
    testBuffer1->mark();

    IntBuffer& ret = testBuffer1->compact();
    CPPUNIT_ASSERT( &ret == testBuffer1 );
    CPPUNIT_ASSERT( testBuffer1->position() == testBuffer1->capacity() );
    CPPUNIT_ASSERT( testBuffer1->limit() == testBuffer1->capacity() );

    assertContentLikeTestData1( testBuffer1, 0, 0, testBuffer1->capacity() );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw InvalidMarkException",
        testBuffer1->reset(),
        InvalidMarkException );

    // case: buffer is empty
    testBuffer1->position(0);
    testBuffer1->limit(0);
    testBuffer1->mark();
    ret = testBuffer1->compact();
    CPPUNIT_ASSERT( &ret == testBuffer1 );
    CPPUNIT_ASSERT( testBuffer1->position() == 0 );
    CPPUNIT_ASSERT( testBuffer1->limit() == testBuffer1->capacity() );

    assertContentLikeTestData1(testBuffer1, 0, 0, testBuffer1->capacity());

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw InvalidMarkException",
        testBuffer1->reset(),
        InvalidMarkException );

    // case: normal
    CPPUNIT_ASSERT( testBuffer1->capacity() > 5 );

    testBuffer1->position(1);
    testBuffer1->limit(5);
    testBuffer1->mark();
    ret = testBuffer1->compact();
    CPPUNIT_ASSERT( &ret == testBuffer1);
    CPPUNIT_ASSERT( testBuffer1->position() == 4 );
    CPPUNIT_ASSERT( testBuffer1->limit() == testBuffer1->capacity() );

    assertContentLikeTestData1(testBuffer1, 0, 1, 4);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw InvalidMarkException",
        testBuffer1->reset(),
        InvalidMarkException );
}

////////////////////////////////////////////////////////////////////////////////
void IntArrayBufferTest::testCompareTo() {

    IntBuffer* other = IntBuffer::allocate( testBuffer1->capacity() );

    loadTestData1(testBuffer1);
    loadTestData1(other);

    CPPUNIT_ASSERT( 0 == testBuffer1->compareTo( *other ) );
    CPPUNIT_ASSERT( 0 == other->compareTo( *testBuffer1 ) );
    testBuffer1->position(1);
    CPPUNIT_ASSERT( testBuffer1->compareTo( *other ) > 0 );
    CPPUNIT_ASSERT( other->compareTo( *testBuffer1 ) < 0 );
    other->position( 2 );
    CPPUNIT_ASSERT( testBuffer1->compareTo( *other ) < 0 );
    CPPUNIT_ASSERT( other->compareTo( *testBuffer1 ) > 0 );
    testBuffer1->position(2);
    other->limit(5);
    CPPUNIT_ASSERT( testBuffer1->compareTo( *other ) > 0 );
    CPPUNIT_ASSERT( other->compareTo( *testBuffer1 ) < 0 );

    std::vector<int> array1( 1, 545645 );
    std::vector<int> array2( 1, 545645 );
    std::vector<int> array3( 1, 42 );

    IntBuffer* dbuffer1 = IntBuffer::wrap( array1 );
    IntBuffer* dbuffer2 = IntBuffer::wrap( array2 );
    IntBuffer* dbuffer3 = IntBuffer::wrap( array3 );

    CPPUNIT_ASSERT_MESSAGE(
        "Failed equal comparison with NaN entry",
        dbuffer1->compareTo( *dbuffer2 ) == 0 );
    CPPUNIT_ASSERT_MESSAGE(
        "Failed greater than comparison with NaN entry",
        dbuffer3->compareTo( *dbuffer1 ) );
    CPPUNIT_ASSERT_MESSAGE(
        "Failed greater than comparison with NaN entry",
        dbuffer1->compareTo( *dbuffer3 ) );

    delete other;
    delete dbuffer1;
    delete dbuffer2;
    delete dbuffer3;
}

////////////////////////////////////////////////////////////////////////////////
void IntArrayBufferTest::testDuplicate() {
    testBuffer1->clear();
    testBuffer1->mark();
    testBuffer1->position(testBuffer1->limit());

    // duplicate's contents should be the same as testBuffer1
    IntBuffer* duplicate = testBuffer1->duplicate();
    CPPUNIT_ASSERT( testBuffer1 != duplicate );
    CPPUNIT_ASSERT( testBuffer1->position() == duplicate->position() );
    CPPUNIT_ASSERT( testBuffer1->limit() == duplicate->limit() );
    CPPUNIT_ASSERT( testBuffer1->isReadOnly() == duplicate->isReadOnly() );
    assertContentEquals( testBuffer1, duplicate );

    // duplicate's position, mark, and limit should be independent to testBuffer1
    duplicate->reset();
    CPPUNIT_ASSERT( duplicate->position() == 0 );
    duplicate->clear();
    CPPUNIT_ASSERT( testBuffer1->position() == testBuffer1->limit() );
    testBuffer1->reset();
    CPPUNIT_ASSERT( testBuffer1->position() == 0 );

    delete duplicate;
}

////////////////////////////////////////////////////////////////////////////////
void IntArrayBufferTest::testEquals() {

    // equal to self
    CPPUNIT_ASSERT( testBuffer1->equals( *testBuffer1 ) );
    IntBuffer* readOnly = testBuffer1->asReadOnlyBuffer();
    CPPUNIT_ASSERT( testBuffer1->equals( *readOnly ) );
    IntBuffer* duplicate = testBuffer1->duplicate();
    CPPUNIT_ASSERT( testBuffer1->equals( *duplicate ) );

    CPPUNIT_ASSERT( testBuffer1->capacity() > 5 );

    testBuffer1->limit( testBuffer1->capacity() ).position(0);
    readOnly->limit( readOnly->capacity() ).position( 1 );
    CPPUNIT_ASSERT( !testBuffer1->equals( *readOnly ) );

    testBuffer1->limit( testBuffer1->capacity() - 1).position(0);
    duplicate->limit( duplicate->capacity() ).position( 0 );
    CPPUNIT_ASSERT( !testBuffer1->equals( *duplicate ) );

    delete readOnly;
    delete duplicate;
}

////////////////////////////////////////////////////////////////////////////////
void IntArrayBufferTest::testGet() {

    testBuffer1->clear();
    for( std::size_t i = 0; i < testBuffer1->capacity(); i++ ) {
        CPPUNIT_ASSERT( testBuffer1->position() == i );
        CPPUNIT_ASSERT( testBuffer1->get() == testBuffer1->get(i) );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw BufferUnderflowException",
        testBuffer1->get(),
        BufferUnderflowException );
}

////////////////////////////////////////////////////////////////////////////////
void IntArrayBufferTest::testGetIntArray() {

    std::vector<int> array(1);
    testBuffer1->clear();

    for( std::size_t i = 0; i < testBuffer1->capacity(); i++ ) {
        CPPUNIT_ASSERT( testBuffer1->position() == i );
        IntBuffer& ret = testBuffer1->get( array );
        CPPUNIT_ASSERT( array[0] == testBuffer1->get(i) );
        CPPUNIT_ASSERT( &ret == testBuffer1 );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw BufferUnderflowException",
        testBuffer1->get( array ),
        BufferUnderflowException );
}

////////////////////////////////////////////////////////////////////////////////
void IntArrayBufferTest::testGetIntArray2() {

    testBuffer1->clear();
    int* array = new int[testBuffer1->capacity()];

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw BufferUnderflowException",
        testBuffer1->get( array, 0, testBuffer1->capacity() + 1 ),
        BufferUnderflowException );

    CPPUNIT_ASSERT( testBuffer1->position() == 0 );

    testBuffer1->get( array, 10, 0 );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw BufferUnderflowException",
        testBuffer1->get( array, 1, Integer::MAX_VALUE ),
        BufferUnderflowException );

    CPPUNIT_ASSERT( testBuffer1->position() == 0 );

    testBuffer1->clear();
    IntBuffer& ret = testBuffer1->get( array, 0, testBuffer1->capacity() );
    CPPUNIT_ASSERT( testBuffer1->position() == testBuffer1->capacity() );
    assertContentEquals( testBuffer1, array, 0, testBuffer1->capacity() );
    CPPUNIT_ASSERT( &ret == testBuffer1 );

    delete [] array;
}

////////////////////////////////////////////////////////////////////////////////
void IntArrayBufferTest::testGet2() {

    testBuffer1->clear();
    for( std::size_t i = 0; i < testBuffer1->capacity(); i++ ) {
        CPPUNIT_ASSERT( testBuffer1->position() == i );
        CPPUNIT_ASSERT( testBuffer1->get() == testBuffer1->get(i) );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw IndexOutOfBoundsException",
        testBuffer1->get( testBuffer1->limit() ),
        IndexOutOfBoundsException );
}

////////////////////////////////////////////////////////////////////////////////
void IntArrayBufferTest::testHasArray() {
    CPPUNIT_ASSERT( testBuffer1->hasArray() );
}

////////////////////////////////////////////////////////////////////////////////
void IntArrayBufferTest::testPutInt() {

    testBuffer1->clear();

    for( std::size_t i = 0; i < testBuffer1->capacity(); i++) {
        CPPUNIT_ASSERT( testBuffer1->position() == i );
        IntBuffer& ret = testBuffer1->put( (int)i );
        CPPUNIT_ASSERT( testBuffer1->get(i) == (int)i );
        CPPUNIT_ASSERT( &ret == testBuffer1 );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw BufferOverflowException",
        testBuffer1->put( 0 ),
        BufferOverflowException );
}

////////////////////////////////////////////////////////////////////////////////
void IntArrayBufferTest::testPutIntArray() {

    int* array = new int[1];

    testBuffer1->clear();
    for( std::size_t i = 0; i < testBuffer1->capacity(); i++ ) {
        CPPUNIT_ASSERT( testBuffer1->position() == i );
        array[0] = (int) i;
        IntBuffer& ret = testBuffer1->put( array, 0, 1 );
        CPPUNIT_ASSERT( testBuffer1->get(i) == (int)i );
        CPPUNIT_ASSERT( &ret == testBuffer1 );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw BufferOverflowException",
        testBuffer1->put( array, 0, 1 ),
        BufferOverflowException );

    delete [] array;
}

////////////////////////////////////////////////////////////////////////////////
void IntArrayBufferTest::testPutIntArray2() {

    testBuffer1->clear();
    int* array1 = new int[ testBuffer1->capacity() ];
    int* array2 = new int[ testBuffer1->capacity() + 1 ];

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw BufferOverflowException",
        testBuffer1->put( array2, 0, testBuffer1->capacity() + 1 ),
        BufferOverflowException );

    CPPUNIT_ASSERT( testBuffer1->position() == 0 );

    testBuffer1->put( array1, testBuffer1->capacity() + 1, 0 );
    CPPUNIT_ASSERT( testBuffer1->position() == 0 );

    CPPUNIT_ASSERT( testBuffer1->position() == 0 );

    loadTestData2( array1, 0, testBuffer1->capacity() );
    IntBuffer& ret = testBuffer1->put( array1, 0, testBuffer1->capacity() );
    CPPUNIT_ASSERT( testBuffer1->position() == testBuffer1->capacity() );
    assertContentEquals( testBuffer1, array1, 0, testBuffer1->capacity() );
    CPPUNIT_ASSERT( &ret == testBuffer1 );

    delete [] array1;
    delete [] array2;
}

////////////////////////////////////////////////////////////////////////////////
void IntArrayBufferTest::testPutIntBuffer() {

    IntBuffer* other = IntBuffer::allocate( testBuffer1->capacity() );
    IntBuffer* other1 = IntBuffer::allocate( testBuffer1->capacity() + 1 );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw IllegalArgumentException",
        testBuffer1->put( *testBuffer1 ),
        IllegalArgumentException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw BufferOverflowException",
        testBuffer1->put( *other1 ),
        BufferOverflowException );

    loadTestData2(other);
    other->clear();
    testBuffer1->clear();
    IntBuffer& ret = testBuffer1->put( *other );

    CPPUNIT_ASSERT( other->position() == other->capacity() );
    CPPUNIT_ASSERT( testBuffer1->position() == testBuffer1->capacity() );
    assertContentEquals( other, testBuffer1 );
    CPPUNIT_ASSERT( &ret == testBuffer1 );

    delete other;
    delete other1;
}

////////////////////////////////////////////////////////////////////////////////
void IntArrayBufferTest::testGetWithIndex() {

    testBuffer1->clear();

    for( std::size_t i = 0; i < testBuffer1->capacity(); i++ ) {
        CPPUNIT_ASSERT( testBuffer1->position() == 0 );
        IntBuffer& ret = testBuffer1->put( i, (int)i );
        CPPUNIT_ASSERT( testBuffer1->get(i) == (int)i );
        CPPUNIT_ASSERT( &ret == testBuffer1 );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw IndexOutOfBoundsException",
        testBuffer1->put( testBuffer1->limit(), 0 ),
        IndexOutOfBoundsException );
}

////////////////////////////////////////////////////////////////////////////////
void IntArrayBufferTest::testPutIndexed() {

    IntBuffer* readOnly = testBuffer1->asReadOnlyBuffer();
    readOnly->clear();
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a ReadOnlyBufferException",
        readOnly->put( 0, 0 ),
        ReadOnlyBufferException );
    delete readOnly;

    testBuffer1->clear();

    for( std::size_t i = 0; i < testBuffer1->capacity(); i++ ) {
        CPPUNIT_ASSERT( testBuffer1->position() == 0 );
        IntBuffer& ret = testBuffer1->put( i, (int)i );
        CPPUNIT_ASSERT( testBuffer1->get(i) == (int)i );
        CPPUNIT_ASSERT( &ret == testBuffer1 );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a IndexOutOfBoundsException",
        testBuffer1->put( testBuffer1->limit(), 0 ),
        IndexOutOfBoundsException );
}

////////////////////////////////////////////////////////////////////////////////
void IntArrayBufferTest::testSlice() {

    CPPUNIT_ASSERT( testBuffer1->capacity() > 5 );
    testBuffer1->position(1);
    testBuffer1->limit(testBuffer1->capacity() - 1);

    IntBuffer* slice = testBuffer1->slice();
    CPPUNIT_ASSERT( testBuffer1->isReadOnly() == slice->isReadOnly() );
    CPPUNIT_ASSERT( slice->position() == 0 );
    CPPUNIT_ASSERT( slice->limit() == testBuffer1->remaining() );
    CPPUNIT_ASSERT( slice->capacity() == testBuffer1->remaining() );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw InvalidMarkException",
        slice->reset(),
        InvalidMarkException );

    // slice share the same content with testBuffer1
    // FIXME:
    loadTestData1(slice);
    assertContentLikeTestData1(testBuffer1, 1, 0, slice->capacity());
    testBuffer1->put( 2, 500 );
    CPPUNIT_ASSERT( slice->get(1) == 500 );

    delete slice;
}

///////////////////////////////////////////////////////////////////////////////
void IntArrayBufferTest::testToString() {

    std::string str = testBuffer1->toString();
    CPPUNIT_ASSERT( str.find("Int") != string::npos );
    CPPUNIT_ASSERT( str.find( Integer::toString( (int)testBuffer1->position() ) ) != string::npos );
    CPPUNIT_ASSERT( str.find( Integer::toString( (int)testBuffer1->limit() ) ) != string::npos );
    CPPUNIT_ASSERT( str.find( Integer::toString( (int)testBuffer1->capacity() ) ) != string::npos );
}
