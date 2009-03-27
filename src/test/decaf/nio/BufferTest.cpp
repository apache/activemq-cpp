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

#include "BufferTest.h"

using namespace decaf;
using namespace decaf::nio;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
void BufferTest::test() {

    // Check that we have setup the array and our initial assumptions on state
    // are correct.  This is the first test run.
    CPPUNIT_ASSERT( buffer != NULL );
    CPPUNIT_ASSERT( buffer->capacity() == DEFAULT_BUFFER_SIZE );
    CPPUNIT_ASSERT( buffer->hasRemaining() == true );
    CPPUNIT_ASSERT( buffer->limit() == buffer->capacity() );
    CPPUNIT_ASSERT( buffer->position() == 0 );
    CPPUNIT_ASSERT( buffer->isReadOnly() == false );
}

////////////////////////////////////////////////////////////////////////////////
void BufferTest::testCapacity() {

    CPPUNIT_ASSERT( 0 == buffer->position() &&
                    buffer->position() <= buffer->limit() &&
                    buffer->limit() <= buffer->capacity() );
}

////////////////////////////////////////////////////////////////////////////////
void BufferTest::testClear() {

    Buffer& ret = buffer->clear();

    CPPUNIT_ASSERT( &ret == buffer );
    CPPUNIT_ASSERT( buffer->position() == 0 );
    CPPUNIT_ASSERT( buffer->limit() == buffer->capacity() );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw InvalidMarkException",
        buffer->reset(),
        InvalidMarkException );
}

////////////////////////////////////////////////////////////////////////////////
void BufferTest::testFlip() {

    std::size_t oldPosition = buffer->position();

    Buffer& ret = buffer->flip();
    CPPUNIT_ASSERT( &ret == buffer );
    CPPUNIT_ASSERT( buffer->position() == 0 );
    CPPUNIT_ASSERT( buffer->limit() == oldPosition );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw InvalidMarkException",
        buffer->reset(),
        InvalidMarkException );
}

////////////////////////////////////////////////////////////////////////////////
void BufferTest::testHasRemaining() {

    CPPUNIT_ASSERT( buffer->hasRemaining() == ( buffer->position() < buffer->limit() ) );
    buffer->position( buffer->limit() );
    CPPUNIT_ASSERT( !buffer->hasRemaining() );
}

////////////////////////////////////////////////////////////////////////////////
void BufferTest::testIsReadOnly() {

    CPPUNIT_ASSERT( !buffer->isReadOnly() );
}

////////////////////////////////////////////////////////////////////////////////
void BufferTest::testLimit() {

    CPPUNIT_ASSERT( 0 == buffer->position() &&
                    buffer->position() <= buffer->limit() &&
                    buffer->limit() <= buffer->capacity() );
}

////////////////////////////////////////////////////////////////////////////////
void BufferTest::testLimitInt() {

    std::size_t oldPosition = buffer->position();
    Buffer& ret = buffer->limit(buffer->limit());
    CPPUNIT_ASSERT( &ret == buffer );

    buffer->mark();
    buffer->limit( buffer->capacity() );
    CPPUNIT_ASSERT( buffer->limit() == buffer->capacity() );
    // position should not change
    CPPUNIT_ASSERT( buffer->position() == oldPosition );

    // mark should be valid
    buffer->reset();

    buffer->limit(buffer->capacity());
    buffer->position(buffer->capacity());
    buffer->mark();
    buffer->limit(buffer->capacity() - 1);
    // position should be the new limit
    CPPUNIT_ASSERT( buffer->position() == buffer->limit() );
    // mark should be invalid
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw InvalidMarkException",
        buffer->reset(),
        InvalidMarkException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw IllegalArgumentException",
        buffer->limit( buffer->capacity() + 1 ),
        IllegalArgumentException );
}

////////////////////////////////////////////////////////////////////////////////
void BufferTest::testMark() {

    std::size_t oldPosition = buffer->position();
    Buffer& ret = buffer->mark();
    CPPUNIT_ASSERT( &ret == buffer );

    buffer->mark();
    buffer->position(buffer->limit());
    buffer->reset();
    CPPUNIT_ASSERT( buffer->position() == oldPosition);

    buffer->mark();
    buffer->position(buffer->limit());
    buffer->reset();
    CPPUNIT_ASSERT( buffer->position() == oldPosition);
}

////////////////////////////////////////////////////////////////////////////////
void BufferTest::testPosition() {

    CPPUNIT_ASSERT( 0 == buffer->position() &&
                    buffer->position() <= buffer->limit() &&
                    buffer->limit() <= buffer->capacity() );
}

////////////////////////////////////////////////////////////////////////////////
void BufferTest::testPositionInt() {

    std::size_t oldPosition = buffer->position();

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw IllegalArgumentException",
        buffer->position( buffer->limit() + 1 ),
        IllegalArgumentException );

    buffer->mark();
    buffer->position(buffer->position());
    buffer->reset();
    CPPUNIT_ASSERT( buffer->position() == oldPosition );

    buffer->position(0);
    CPPUNIT_ASSERT( buffer->position() == 0 );
    buffer->position(buffer->limit());
    CPPUNIT_ASSERT( buffer->position() == buffer->limit() );

    if (buffer->capacity() > 0) {
        buffer->limit( buffer->capacity() );
        buffer->position( buffer->limit() );
        buffer->mark();
        buffer->position( buffer->limit() - 1);
        CPPUNIT_ASSERT( buffer->position() == buffer->limit() - 1 );

        // mark should be invalid
        CPPUNIT_ASSERT_THROW_MESSAGE(
            "Should throw InvalidMarkException",
            buffer->reset(),
            InvalidMarkException );
    }

    Buffer& ret = buffer->position(0);
    CPPUNIT_ASSERT( &ret == buffer );
}

////////////////////////////////////////////////////////////////////////////////
void BufferTest::testRemaining() {
    CPPUNIT_ASSERT( buffer->remaining() == ( buffer->limit() - buffer->position() ) );
}

////////////////////////////////////////////////////////////////////////////////
void BufferTest::testReset() {

    std::size_t oldPosition = buffer->position();

    buffer->mark();
    buffer->position(buffer->limit());
    buffer->reset();
    CPPUNIT_ASSERT( buffer->position() == oldPosition );

    buffer->mark();
    buffer->position(buffer->limit());
    buffer->reset();
    CPPUNIT_ASSERT( buffer->position() == oldPosition );

    Buffer& ret = buffer->reset();
    CPPUNIT_ASSERT( &ret == buffer );

    buffer->clear();
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw InvalidMarkException",
        buffer->reset(),
        InvalidMarkException );
}

////////////////////////////////////////////////////////////////////////////////
void BufferTest::testRewind() {

    Buffer& ret = buffer->rewind();
    CPPUNIT_ASSERT( buffer->position() == 0 );
    CPPUNIT_ASSERT( &ret == buffer );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw InvalidMarkException",
        buffer->reset(),
        InvalidMarkException );
}
