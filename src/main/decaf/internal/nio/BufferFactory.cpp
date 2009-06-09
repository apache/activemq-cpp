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

#include "BufferFactory.h"

#include <decaf/internal/nio/ByteArrayBuffer.h>
#include <decaf/internal/nio/CharArrayBuffer.h>
#include <decaf/internal/nio/DoubleArrayBuffer.h>
#include <decaf/internal/nio/FloatArrayBuffer.h>
#include <decaf/internal/nio/LongArrayBuffer.h>
#include <decaf/internal/nio/IntArrayBuffer.h>
#include <decaf/internal/nio/ShortArrayBuffer.h>

using namespace decaf;
using namespace decaf::internal;
using namespace decaf::internal::nio;
using namespace decaf::nio;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
ByteBuffer* BufferFactory::createByteBuffer( std::size_t capacity ) {

    try{
        return new ByteArrayBuffer( capacity );
    }
    DECAF_CATCH_RETHROW( Exception )
    DECAF_CATCHALL_THROW( Exception )
}

////////////////////////////////////////////////////////////////////////////////
ByteBuffer* BufferFactory::createByteBuffer( unsigned char* buffer,
                                             std::size_t offset,
                                             std::size_t length )
    throw( lang::exceptions::NullPointerException ) {

    try{
        return new ByteArrayBuffer( buffer, offset, length, false );
    }
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, NullPointerException )
    DECAF_CATCHALL_THROW( NullPointerException )
}

////////////////////////////////////////////////////////////////////////////////
ByteBuffer* BufferFactory::createByteBuffer( std::vector<unsigned char>& buffer ) {

    try{
        return new ByteArrayBuffer( &buffer[0], 0, buffer.size(), false );
    }
    DECAF_CATCH_RETHROW( Exception )
    DECAF_CATCHALL_THROW( Exception )
}

////////////////////////////////////////////////////////////////////////////////
CharBuffer* BufferFactory::createCharBuffer( std::size_t capacity ) {

    try{
        return new CharArrayBuffer( capacity );
    }
    DECAF_CATCH_RETHROW( Exception )
    DECAF_CATCHALL_THROW( Exception )
}

////////////////////////////////////////////////////////////////////////////////
CharBuffer* BufferFactory::createCharBuffer( char* buffer,
                                             std::size_t offset,
                                             std::size_t length )
    throw( lang::exceptions::NullPointerException ) {

    try{
        return new CharArrayBuffer( buffer, offset, length, false );
    }
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, NullPointerException )
    DECAF_CATCHALL_THROW( NullPointerException )
}

////////////////////////////////////////////////////////////////////////////////
CharBuffer* BufferFactory::createCharBuffer( std::vector<char>& buffer ) {

    try{
        return new CharArrayBuffer( &buffer[0], 0, buffer.size(), false );
    }
    DECAF_CATCH_RETHROW( Exception )
    DECAF_CATCHALL_THROW( Exception )
}

////////////////////////////////////////////////////////////////////////////////
DoubleBuffer* BufferFactory::createDoubleBuffer( std::size_t capacity ) {

    try{
        return new DoubleArrayBuffer( capacity );
    }
    DECAF_CATCH_RETHROW( Exception )
    DECAF_CATCHALL_THROW( Exception )
}

////////////////////////////////////////////////////////////////////////////////
DoubleBuffer* BufferFactory::createDoubleBuffer( double* buffer,
                                                 std::size_t offset,
                                                 std::size_t length )
    throw( lang::exceptions::NullPointerException ) {

    try{
        return new DoubleArrayBuffer( buffer, offset, length, false );
    }
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, NullPointerException )
    DECAF_CATCHALL_THROW( NullPointerException )
}

////////////////////////////////////////////////////////////////////////////////
DoubleBuffer* BufferFactory::createDoubleBuffer( std::vector<double>& buffer ) {

    try{
        return new DoubleArrayBuffer( &buffer[0], 0, buffer.size(), false );
    }
    DECAF_CATCH_RETHROW( Exception )
    DECAF_CATCHALL_THROW( Exception )
}

////////////////////////////////////////////////////////////////////////////////
FloatBuffer* BufferFactory::createFloatBuffer( std::size_t capacity ) {

    try{
        return new FloatArrayBuffer( capacity );
    }
    DECAF_CATCH_RETHROW( Exception )
    DECAF_CATCHALL_THROW( Exception )
}

////////////////////////////////////////////////////////////////////////////////
FloatBuffer* BufferFactory::createFloatBuffer( float* buffer,
                                               std::size_t offset,
                                               std::size_t length )
    throw( lang::exceptions::NullPointerException ) {

    try{
        return new FloatArrayBuffer( buffer, offset, length, false );
    }
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, NullPointerException )
    DECAF_CATCHALL_THROW( NullPointerException )
}

////////////////////////////////////////////////////////////////////////////////
FloatBuffer* BufferFactory::createFloatBuffer( std::vector<float>& buffer ) {

    try{
        return new FloatArrayBuffer( &buffer[0], 0, buffer.size(), false );
    }
    DECAF_CATCH_RETHROW( Exception )
    DECAF_CATCHALL_THROW( Exception )
}

////////////////////////////////////////////////////////////////////////////////
LongBuffer* BufferFactory::createLongBuffer( std::size_t capacity ) {

    try{
        return new LongArrayBuffer( capacity );
    }
    DECAF_CATCH_RETHROW( Exception )
    DECAF_CATCHALL_THROW( Exception )
}

////////////////////////////////////////////////////////////////////////////////
LongBuffer* BufferFactory::createLongBuffer( long long* buffer,
                                             std::size_t offset,
                                             std::size_t length )
    throw( lang::exceptions::NullPointerException ) {

    try{
        return new LongArrayBuffer( buffer, offset, length, false );
    }
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, NullPointerException )
    DECAF_CATCHALL_THROW( NullPointerException )
}

////////////////////////////////////////////////////////////////////////////////
LongBuffer* BufferFactory::createLongBuffer( std::vector<long long>& buffer ) {

    try{
        return new LongArrayBuffer( &buffer[0], 0, buffer.size(), false );
    }
    DECAF_CATCH_RETHROW( Exception )
    DECAF_CATCHALL_THROW( Exception )
}

////////////////////////////////////////////////////////////////////////////////
IntBuffer* BufferFactory::createIntBuffer( std::size_t capacity ) {

    try{
        return new IntArrayBuffer( capacity );
    }
    DECAF_CATCH_RETHROW( Exception )
    DECAF_CATCHALL_THROW( Exception )
}

////////////////////////////////////////////////////////////////////////////////
IntBuffer* BufferFactory::createIntBuffer( int* buffer,
                                           std::size_t offset,
                                           std::size_t length )
    throw( lang::exceptions::NullPointerException ) {

    try{
        return new IntArrayBuffer( buffer, offset, length, false );
    }
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, NullPointerException )
    DECAF_CATCHALL_THROW( NullPointerException )
}

////////////////////////////////////////////////////////////////////////////////
IntBuffer* BufferFactory::createIntBuffer( std::vector<int>& buffer ) {

    try{
        return new IntArrayBuffer( &buffer[0], 0, buffer.size(), false );
    }
    DECAF_CATCH_RETHROW( Exception )
    DECAF_CATCHALL_THROW( Exception )
}

////////////////////////////////////////////////////////////////////////////////
ShortBuffer* BufferFactory::createShortBuffer( std::size_t capacity ) {

    try{
        return new ShortArrayBuffer( capacity );
    }
    DECAF_CATCH_RETHROW( Exception )
    DECAF_CATCHALL_THROW( Exception )
}

////////////////////////////////////////////////////////////////////////////////
ShortBuffer* BufferFactory::createShortBuffer( short* buffer,
                                               std::size_t offset,
                                               std::size_t length )
    throw( lang::exceptions::NullPointerException ) {

    try{
        return new ShortArrayBuffer( buffer, offset, length, false );
    }
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, NullPointerException )
    DECAF_CATCHALL_THROW( NullPointerException )
}

////////////////////////////////////////////////////////////////////////////////
ShortBuffer* BufferFactory::createShortBuffer( std::vector<short>& buffer ) {

    try{
        return new ShortArrayBuffer( &buffer[0], 0, buffer.size(), false );
    }
    DECAF_CATCH_RETHROW( Exception )
    DECAF_CATCHALL_THROW( Exception )
}
