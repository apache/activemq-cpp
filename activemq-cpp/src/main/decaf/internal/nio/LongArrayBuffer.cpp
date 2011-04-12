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

#include "LongArrayBuffer.h"

using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::internal;
using namespace decaf::internal::nio;
using namespace decaf::internal::util;
using namespace decaf::nio;

///////////////////////////////////////////////////////////////////////////////
LongArrayBuffer::LongArrayBuffer( int size, bool readOnly ) :
    LongBuffer(size), _array(), offset(0), length(size), readOnly(readOnly) {

    // Allocate using the ByteArray, not read-only initially.  Take a reference to it.
    // The capacity is the given capacity times the size of the stored datatype
    this->_array.reset( new ByteArrayAdapter( size * (int)sizeof(long long) ) );
}

///////////////////////////////////////////////////////////////////////////////
LongArrayBuffer::LongArrayBuffer( long long* array, int size, int offset, int length, bool readOnly ) :
    LongBuffer(length), _array(), offset(offset), length(length), readOnly(readOnly) {

    try{

        if( offset < 0 || offset > size ) {
            throw IndexOutOfBoundsException(
                __FILE__, __LINE__, "Offset parameter if out of bounds, %d", offset );
        }

        if( length < 0 || offset + length > size ) {
            throw IndexOutOfBoundsException(
                __FILE__, __LINE__, "length parameter if out of bounds, %d", length );
        }

        // Allocate using the ByteArray, not read-only initially.
        this->_array.reset( new ByteArrayAdapter( array, size, false ) );
    }
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, NullPointerException )
    DECAF_CATCHALL_THROW( NullPointerException )
}

///////////////////////////////////////////////////////////////////////////////
LongArrayBuffer::LongArrayBuffer( const Pointer<ByteArrayAdapter>& array, int offset, int length, bool readOnly ) :
    LongBuffer(length), _array(array), offset(offset), length(length), readOnly(readOnly) {

    try{

        if( offset < 0 || offset > array->getCapacity() ) {
            throw IndexOutOfBoundsException(
                __FILE__, __LINE__, "Offset parameter if out of bounds, %d", offset );
        }

        if( length < 0 || offset + length > array->getCapacity() ) {
            throw IndexOutOfBoundsException(
                __FILE__, __LINE__, "length parameter if out of bounds, %d", length );
        }
    }
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, NullPointerException )
    DECAF_CATCHALL_THROW( NullPointerException )
}

///////////////////////////////////////////////////////////////////////////////
LongArrayBuffer::LongArrayBuffer( const LongArrayBuffer& other ) :
    LongBuffer(other), _array(other._array), offset(other.offset), length(other.length), readOnly(other.readOnly) {
}

////////////////////////////////////////////////////////////////////////////////
LongArrayBuffer::~LongArrayBuffer() {
}

///////////////////////////////////////////////////////////////////////////////
long long* LongArrayBuffer::array() {

    try{

        if( !this->hasArray() ) {
            throw UnsupportedOperationException(
                __FILE__, __LINE__,
                "LongArrayBuffer::arrayOffset() - This Buffer has no backing array." );
        }

        if( this->isReadOnly() ) {
            throw ReadOnlyBufferException(
                __FILE__, __LINE__,
                "LongArrayBuffer::array - Buffer is Read-Only" );
        }

        return this->_array->getLongArray();
    }
    DECAF_CATCH_RETHROW( ReadOnlyBufferException )
    DECAF_CATCH_RETHROW( UnsupportedOperationException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, UnsupportedOperationException )
    DECAF_CATCHALL_THROW( UnsupportedOperationException )
}

///////////////////////////////////////////////////////////////////////////////
int LongArrayBuffer::arrayOffset() {

    try{

        if( !this->hasArray() ) {
            throw UnsupportedOperationException(
                __FILE__, __LINE__,
                "LongArrayBuffer::arrayOffset() - This Buffer has no backing array." );
        }

        if( this->isReadOnly() ) {
            throw decaf::nio::ReadOnlyBufferException(
                __FILE__, __LINE__,
                "LongArrayBuffer::arrayOffset() - Buffer is Read Only." );
        }

        return this->offset;
    }
    DECAF_CATCH_RETHROW( ReadOnlyBufferException )
    DECAF_CATCH_RETHROW( UnsupportedOperationException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, UnsupportedOperationException )
    DECAF_CATCHALL_THROW( UnsupportedOperationException )
}

///////////////////////////////////////////////////////////////////////////////
LongBuffer* LongArrayBuffer::asReadOnlyBuffer() const {

    try{

        LongArrayBuffer* buffer = new LongArrayBuffer( *this );
        buffer->setReadOnly( true );

        return buffer;
    }
    DECAF_CATCH_RETHROW( Exception )
    DECAF_CATCHALL_THROW( Exception )
}

///////////////////////////////////////////////////////////////////////////////
LongBuffer& LongArrayBuffer::compact() {

    try{

        if( this->isReadOnly() ) {
            throw decaf::nio::ReadOnlyBufferException(
                __FILE__, __LINE__,
                "LongArrayBuffer::compact() - Buffer is Read Only." );
        }

        // copy from the current pos to the beginning all the remaining bytes
        // the set pos to the
        for( int ix = 0; ix < this->remaining(); ++ix ) {
            this->put( ix, this->get( this->position() + ix ) );
        }

        this->position( this->limit() - this->position() );
        this->limit( this->capacity() );
        this->_markSet = false;

        return *this;
    }
    DECAF_CATCH_RETHROW( decaf::nio::ReadOnlyBufferException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, decaf::nio::ReadOnlyBufferException )
    DECAF_CATCHALL_THROW( decaf::nio::ReadOnlyBufferException )
}

///////////////////////////////////////////////////////////////////////////////
LongBuffer* LongArrayBuffer::duplicate() {

    try{
        return new LongArrayBuffer( *this );
    }
    DECAF_CATCH_RETHROW( Exception )
    DECAF_CATCHALL_THROW( Exception )
}

///////////////////////////////////////////////////////////////////////////////
long long LongArrayBuffer::get() {

    try{
        return this->get( this->_position++ );
    }
    DECAF_CATCH_RETHROW( BufferUnderflowException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, BufferUnderflowException )
    DECAF_CATCHALL_THROW( BufferUnderflowException )
}

///////////////////////////////////////////////////////////////////////////////
long long LongArrayBuffer::get( int index ) const {

    try{

        if( index >= this->limit() ) {
            throw IndexOutOfBoundsException(
                __FILE__, __LINE__,
                "LongArrayBuffer::get - Not enough data to fill request." );
        }

        return this->_array->getLong( offset + index );
    }
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IndexOutOfBoundsException )
    DECAF_CATCHALL_THROW( IndexOutOfBoundsException )
}

////////////////////////////////////////////////////////////////////////////////
LongBuffer& LongArrayBuffer::put( long long value ) {

    try{

        this->put( this->_position++, value );
        return *this;
    }
    DECAF_CATCH_RETHROW( decaf::nio::ReadOnlyBufferException )
    DECAF_CATCH_RETHROW( decaf::nio::BufferOverflowException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, decaf::nio::BufferOverflowException )
    DECAF_CATCHALL_THROW( decaf::nio::BufferOverflowException )
}

////////////////////////////////////////////////////////////////////////////////
LongBuffer& LongArrayBuffer::put( int index, long long value ) {

    try{

        if( this->isReadOnly() ) {
            throw decaf::nio::ReadOnlyBufferException(
                __FILE__, __LINE__,
                "LongArrayBuffer::put(i,i) - Buffer is Read Only." );
        }

        if( index >= this->limit() ) {
            throw IndexOutOfBoundsException(
                __FILE__, __LINE__,
                "LongArrayBuffer::put(i,i) - Not enough data to fill request." );
        }

        this->_array->putLong( index + offset, value );

        return *this;
    }
    DECAF_CATCH_RETHROW( decaf::nio::ReadOnlyBufferException )
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IndexOutOfBoundsException )
    DECAF_CATCHALL_THROW( IndexOutOfBoundsException )
}

////////////////////////////////////////////////////////////////////////////////
LongBuffer* LongArrayBuffer::slice() const {

    try{

        return new LongArrayBuffer( this->_array,
                                    this->offset + this->position(),
                                    this->remaining(),
                                    this->isReadOnly() );
    }
    DECAF_CATCH_RETHROW( Exception )
    DECAF_CATCHALL_THROW( Exception )
}
