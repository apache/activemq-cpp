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

#include "BufferFactoryTest.h"

#include <decaf/nio/ByteBuffer.h>
#include <decaf/nio/CharBuffer.h>
#include <decaf/nio/DoubleBuffer.h>
#include <decaf/nio/FloatBuffer.h>
#include <decaf/nio/LongBuffer.h>
#include <decaf/nio/IntBuffer.h>
#include <decaf/nio/ShortBuffer.h>

using namespace decaf;
using namespace decaf::internal;
using namespace decaf::internal::nio;
using namespace decaf::nio;

////////////////////////////////////////////////////////////////////////////////
void BufferFactoryTest::testCreateByteBuffer1() {

    ByteBuffer* buffer = BufferFactory::createByteBuffer( 500 );
    CPPUNIT_ASSERT( buffer != NULL );
    CPPUNIT_ASSERT( buffer->capacity() == 500 );
    CPPUNIT_ASSERT( buffer->isReadOnly() == false );

    delete buffer;
}

////////////////////////////////////////////////////////////////////////////////
void BufferFactoryTest::testCreateByteBuffer2() {

    std::vector<unsigned char> array;
    array.resize( 500 );
    ByteBuffer* buffer = BufferFactory::createByteBuffer( array );
    CPPUNIT_ASSERT( buffer != NULL );
    CPPUNIT_ASSERT( buffer->hasArray() == true );
    CPPUNIT_ASSERT( buffer->array() == &array[0] );
    CPPUNIT_ASSERT( buffer->capacity() == 500 );
    CPPUNIT_ASSERT( buffer->isReadOnly() == false );

    delete buffer;
}

////////////////////////////////////////////////////////////////////////////////
void BufferFactoryTest::testCreateByteBuffer3() {

    std::vector<unsigned char> array;
    array.resize( 500 );
    ByteBuffer* buffer = BufferFactory::createByteBuffer( &array[0], 100, 400 );
    CPPUNIT_ASSERT( buffer != NULL );
    CPPUNIT_ASSERT( buffer->hasArray() == true );
    CPPUNIT_ASSERT( buffer->array() == &array[0] );
    CPPUNIT_ASSERT( buffer->capacity() == 400 );
    CPPUNIT_ASSERT( buffer->isReadOnly() == false );

    delete buffer;
}
