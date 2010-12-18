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

#include "IntArrayBuffer.h"

using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::internal;
using namespace decaf::internal::nio;
using namespace decaf::internal::util;
using namespace decaf::nio;

///////////////////////////////////////////////////////////////////////////////
IntArrayBuffer::IntArrayBuffer( int size, bool readOnly ) :
    IntBuffer(size), _array(), offset(0), length(size), readOnly(readOnly) {

    // Allocate using the ByteArray, not read-only initially.  Take a reference to it.
    // The size is the given size times the size of the stored datatype
    this->_array.reset( new ByteArrayAdapter( size * (int)sizeof(int) ) );
}

///////////////////////////////////////////////////////////////////////////////
IntArrayBuffer::IntArrayBuffer( int* array, int size, int offset, int length, bool readOnly ) :
    IntBuffer(length), _array(), offset(offset), length(length), readOnly(readOnly) {

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
IntArrayBuffer::IntArrayBuffer( const Pointer<ByteArrayAdapter>& array, int offset, int length, bool readOnly ) :
    IntBuffer(length), _array(array), offset(offset), length(length), readOnly(readOnly) {

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
IntArrayBuffer::IntArrayBuffer( const IntArrayBuffer& other ) :
    IntBuffer(other), _array(other._array), offset(other.offset), length(other.length), readOnly(other.readOnly) {
}

////////////////////////////////////////////////////////////////////////////////
IntArrayBuffer::~IntArrayBuffer() {
}

///////////////////////////////////////////////////////////////////////////////
int* IntArrayBuffer::array() {

    try{

        if( !this->hasArray() ) {
            throw UnsupportedOperationException(
                __FILE__, __LINE__,
                "IntArrayBuffer::arrayOffset() - This Buffer has no backing array." );
        }

        if( this->isReadOnly() ) {
            throw ReadOnlyBufferException(
                __FILE__, __LINE__,
                "IntArrayBuffer::array - Buffer is Read-Only" );
        }

        return this->_array->getIntArray();
    }
    DECAF_CATCH_RETHROW( ReadOnlyBufferException )
    DECAF_CATCH_RETHROW( UnsupportedOperationException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, UnsupportedOperationException )
    DECAF_CATCHALL_THROW( UnsupportedOperationException )
}

///////////////////////////////////////////////////////////////////////////////
int IntArrayBuffer::arrayOffset() {

    try{

        if( !this->hasArray() ) {
            throw UnsupportedOperationException(
                __FILE__, __LINE__,
                "IntArrayBuffer::arrayOffset() - This Buffer has no backing array." );
        }

        if( this->isReadOnly() ) {
            throw decaf::nio::ReadOnlyBufferException(
                __FILE__, __LINE__,
                "IntArrayBuffer::arrayOffset() - Buffer is Read Only." );
        }

        return this->offset;
    }
    DECAF_CATCH_RETHROW( ReadOnlyBufferException )
    DECAF_CATCH_RETHROW( UnsupportedOperationException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, UnsupportedOperationException )
    DECAF_CATCHALL_THROW( UnsupportedOperationException )
}

///////////////////////////////////////////////////////////////////////////////
IntBuffer* IntArrayBuffer::asReadOnlyBuffer() const {

    try{

        IntArrayBuffer* buffer = new IntArrayBuffer( *this );
        buffer->setReadOnly( true );

        return buffer;
    }
    DECAF_CATCH_RETHROW( Exception )
    DECAF_CATCHALL_THROW( Exception )
}

///////////////////////////////////////////////////////////////////////////////
IntBuffer& IntArrayBuffer::compact() {

    try{

        if( this->isReadOnly() ) {
            throw decaf::nio::ReadOnlyBufferException(
                __FILE__, __LINE__,
                "IntArrayBuffer::compact() - Buffer is Read Only." );
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
IntBuffer* IntArrayBuffer::duplicate() {

    try{
        return new IntArrayBuffer( *this );
    }
    DECAF_CATCH_RETHROW( Exception )
    DECAF_CATCHALL_THROW( Exception )
}

///////////////////////////////////////////////////////////////////////////////
int IntArrayBuffer::get() {

    try{
        return this->get( this->_position++ );
    }
    DECAF_CATCH_RETHROW( BufferUnderflowException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, BufferUnderflowException )
    DECAF_CATCHALL_THROW( BufferUnderflowException )
}

///////////////////////////////////////////////////////////////////////////////
int IntArrayBuffer::get( int index ) const {

    try{

        if( index >= this->limit() ) {
            throw IndexOutOfBoundsException(
                __FILE__, __LINE__,
                "IntArrayBuffer::get - Not enough data to fill request." );
        }

        return this->_array->getInt( offset + index );
    }
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IndexOutOfBoundsException )
    DECAF_CATCHALL_THROW( IndexOutOfBoundsException )
}

////////////////////////////////////////////////////////////////////////////////
IntBuffer& IntArrayBuffer::put( int value ) {

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
IntBuffer& IntArrayBuffer::put( int index, int value ) {

    try{

        if( this->isReadOnly() ) {
            throw decaf::nio::ReadOnlyBufferException(
                __FILE__, __LINE__,
                "IntArrayBuffer::put(i,i) - Buffer is Read Only." );
        }

        if( index >= this->limit() ) {
            throw IndexOutOfBoundsException(
                __FILE__, __LINE__,
                "IntArrayBuffer::put(i,i) - Not enough data to fill request." );
        }

        this->_array->putInt( index + offset, value );

        return *this;
    }
    DECAF_CATCH_RETHROW( decaf::nio::ReadOnlyBufferException )
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IndexOutOfBoundsException )
    DECAF_CATCHALL_THROW( IndexOutOfBoundsException )
}

////////////////////////////////////////////////////////////////////////////////
IntBuffer* IntArrayBuffer::slice() const {

    try{

        return new IntArrayBuffer( this->_array,
                                   this->offset + this->position(),
                                   this->remaining(),
                                   this->isReadOnly() );
    }
    DECAF_CATCH_RETHROW( Exception )
    DECAF_CATCHALL_THROW( Exception )
}
