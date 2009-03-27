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

#include "ByteArrayBuffer.h"
#include "decaf/lang/Short.h"
#include "decaf/lang/Integer.h"
#include "decaf/lang/Long.h"
#include "decaf/lang/Float.h"
#include "decaf/lang/Double.h"
#include <string.h>

using namespace std;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::internal::nio;

////////////////////////////////////////////////////////////////////////////////
ByteArrayBuffer::ByteArrayBuffer( std::size_t capacity, bool readOnly )
 : decaf::nio::ByteBuffer( capacity ) {

    // Allocate using the ByteArray, not read-only initially.  Take a reference to it.
    this->_array = new ByteArrayPerspective( capacity );
    this->offset = 0;
    this->readOnly = readOnly;
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayBuffer::ByteArrayBuffer( unsigned char* array, std::size_t offset,
                                  std::size_t capacity, bool readOnly )
    throw( decaf::lang::exceptions::NullPointerException )
    : decaf::nio::ByteBuffer( capacity ) {

    try{

        // Allocate using the ByteArray, not read-only initially.
        this->_array = new ByteArrayPerspective( array, capacity, false );
        this->offset = offset;
        this->readOnly = readOnly;
    }
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, NullPointerException )
    DECAF_CATCHALL_THROW( NullPointerException )
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayBuffer::ByteArrayBuffer( ByteArrayPerspective& array, std::size_t offset,
                                  std::size_t length, bool readOnly )
    throw( decaf::lang::exceptions::IndexOutOfBoundsException )
    : decaf::nio::ByteBuffer( length ) {

    try{
        if( offset > array.getCapacity() ) {
            throw IndexOutOfBoundsException(
                __FILE__, __LINE__,
                "ByteArrayBuffer::ByteArrayBuffer - offset %d is greater than capacity %d",
                offset, array.getCapacity() );
        }

        // Allocate using the ByteArray, not read-only initially.
        this->_array = array.takeRef();
        this->offset = offset;
        this->readOnly = readOnly;
    }
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, NullPointerException )
    DECAF_CATCHALL_THROW( NullPointerException )
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayBuffer::ByteArrayBuffer( const ByteArrayBuffer& other )
    : decaf::nio::ByteBuffer( other ) {

    // get the byte buffer of the caller and take a reference
    this->_array = other._array->takeRef();
    this->offset = other.offset;
    this->readOnly = other.readOnly;
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayBuffer::~ByteArrayBuffer() {

    try{

        // Return this object's reference to the buffer.
        this->_array->returnRef();

        // If there are no other Buffers out there that reference it then we
        // delete it now, the internal unsigned char* array will be deleted
        // if we where the owner.
        if( this->_array->getReferences() == 0 ) {
            delete this->_array;
        }
    }
    DECAF_CATCH_NOTHROW( Exception )
    DECAF_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
unsigned char* ByteArrayBuffer::array()
    throw( decaf::nio::ReadOnlyBufferException, UnsupportedOperationException ) {

    try{

        if( !this->hasArray() ) {
            throw UnsupportedOperationException(
                __FILE__, __LINE__,
                "ByteArrayBuffer::arrayOffset() - This Buffer has no backing array." );
        }

        if( this->isReadOnly() ) {
            throw decaf::nio::ReadOnlyBufferException(
                __FILE__, __LINE__,
                "ByteArrayBuffer::array() - Buffer is Read Only." );
        }

        return this->_array->getByteArray();
    }
    DECAF_CATCH_RETHROW( decaf::nio::ReadOnlyBufferException )
    DECAF_CATCH_RETHROW( UnsupportedOperationException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, decaf::nio::ReadOnlyBufferException )
    DECAF_CATCHALL_THROW( decaf::nio::ReadOnlyBufferException )
}

////////////////////////////////////////////////////////////////////////////////
std::size_t ByteArrayBuffer::arrayOffset() const
    throw( decaf::nio::ReadOnlyBufferException,
           lang::exceptions::UnsupportedOperationException ) {

    try{

        if( !this->hasArray() ) {
            throw UnsupportedOperationException(
                __FILE__, __LINE__,
                "ByteArrayBuffer::arrayOffset() - This Buffer has no backing array." );
        }

        if( this->isReadOnly() ) {
            throw decaf::nio::ReadOnlyBufferException(
                __FILE__, __LINE__,
                "ByteArrayBuffer::arrayOffset() - Buffer is Read Only." );
        }

        return this->offset;
    }
    DECAF_CATCH_RETHROW( decaf::nio::ReadOnlyBufferException )
    DECAF_CATCH_RETHROW( UnsupportedOperationException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, decaf::nio::ReadOnlyBufferException )
    DECAF_CATCHALL_THROW( decaf::nio::ReadOnlyBufferException )
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayBuffer* ByteArrayBuffer::asReadOnlyBuffer() const {

    try{

        ByteArrayBuffer* buffer = new ByteArrayBuffer( *this );
        buffer->setReadOnly( true );

        return buffer;
    }
    DECAF_CATCH_RETHROW( Exception )
    DECAF_CATCHALL_THROW( Exception )
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayBuffer& ByteArrayBuffer::compact() throw( decaf::nio::ReadOnlyBufferException ) {

    try{

        if( this->isReadOnly() ) {
            throw decaf::nio::ReadOnlyBufferException(
                __FILE__, __LINE__,
                "ByteArrayBuffer::compact() - Buffer is Read Only." );
        }

        // copy from the current pos to the beginning all the remaining bytes
        // the set pos to the
        for( std::size_t ix = 0; ix < this->remaining(); ++ix ) {
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

////////////////////////////////////////////////////////////////////////////////
ByteArrayBuffer* ByteArrayBuffer::duplicate() {

    try{
        return new ByteArrayBuffer( *this );
    }
    DECAF_CATCH_RETHROW( Exception )
    DECAF_CATCHALL_THROW( Exception )
}

////////////////////////////////////////////////////////////////////////////////
unsigned char ByteArrayBuffer::get() const throw( decaf::nio::BufferUnderflowException ) {

    try{
        return this->get( this->_position++ );
    }
    DECAF_CATCH_RETHROW( decaf::nio::BufferUnderflowException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, decaf::nio::BufferUnderflowException )
    DECAF_CATCHALL_THROW( decaf::nio::BufferUnderflowException )
}

////////////////////////////////////////////////////////////////////////////////
unsigned char ByteArrayBuffer::get( std::size_t index ) const
    throw ( lang::exceptions::IndexOutOfBoundsException ) {

    try{

        if( ( index ) >= this->limit() ) {
            throw IndexOutOfBoundsException(
                __FILE__, __LINE__,
                "ByteArrayBuffer::get - Not enough data to fill request." );
        }

        return this->_array->get( offset + index );
    }
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IndexOutOfBoundsException )
    DECAF_CATCHALL_THROW( IndexOutOfBoundsException )
}

////////////////////////////////////////////////////////////////////////////////
double ByteArrayBuffer::getDouble() throw( decaf::nio::BufferUnderflowException ) {

    try{

        unsigned long long lvalue = this->getLong();
        return Double::longBitsToDouble( lvalue );
    }
    DECAF_CATCH_RETHROW( decaf::nio::BufferUnderflowException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, decaf::nio::BufferUnderflowException )
    DECAF_CATCHALL_THROW( decaf::nio::BufferUnderflowException )
}

////////////////////////////////////////////////////////////////////////////////
double ByteArrayBuffer::getDouble( std::size_t index ) const
    throw ( lang::exceptions::IndexOutOfBoundsException ) {

    try{

        unsigned long long lvalue = this->getLong( index );
        return Double::longBitsToDouble( lvalue );
    }
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IndexOutOfBoundsException )
    DECAF_CATCHALL_THROW( IndexOutOfBoundsException )
}

////////////////////////////////////////////////////////////////////////////////
float ByteArrayBuffer::getFloat() throw( decaf::nio::BufferUnderflowException ) {

    try{

        unsigned int ivalue = this->getInt();
        return Float::intBitsToFloat( ivalue );
    }
    DECAF_CATCH_RETHROW( decaf::nio::BufferUnderflowException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, decaf::nio::BufferUnderflowException )
    DECAF_CATCHALL_THROW( decaf::nio::BufferUnderflowException )
}

////////////////////////////////////////////////////////////////////////////////
float ByteArrayBuffer::getFloat( std::size_t index ) const
    throw ( lang::exceptions::IndexOutOfBoundsException ) {

    try{

        unsigned int ivalue = this->getInt( index );
        return Float::intBitsToFloat( ivalue );
    }
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IndexOutOfBoundsException )
    DECAF_CATCHALL_THROW( IndexOutOfBoundsException )
}

////////////////////////////////////////////////////////////////////////////////
long long ByteArrayBuffer::getLong() throw( decaf::nio::BufferUnderflowException ) {

    try{

        long long value = this->getLong( this->_position );
        this->_position += sizeof(value);
        return value;
    }
    DECAF_CATCH_RETHROW( decaf::nio::BufferUnderflowException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, decaf::nio::BufferUnderflowException )
    DECAF_CATCHALL_THROW( decaf::nio::BufferUnderflowException )
}

////////////////////////////////////////////////////////////////////////////////
long long ByteArrayBuffer::getLong( std::size_t index ) const
    throw ( lang::exceptions::IndexOutOfBoundsException ) {

    try{

        if( (offset + index + sizeof(long long)) > this->limit() ) {
            throw IndexOutOfBoundsException(
                __FILE__, __LINE__,
                "ByteArrayBuffer::getLong(i) - Not enough data to fill a long long." );
        }

        return this->_array->getLongAt( index + offset );
    }
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IndexOutOfBoundsException )
    DECAF_CATCHALL_THROW( IndexOutOfBoundsException )
}

////////////////////////////////////////////////////////////////////////////////
int ByteArrayBuffer::getInt() throw( decaf::nio::BufferUnderflowException )  {

    try{

        int value = this->getInt( this->_position );
        this->_position += sizeof(value);
        return value;
    }
    DECAF_CATCH_RETHROW( decaf::nio::BufferUnderflowException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, decaf::nio::BufferUnderflowException )
    DECAF_CATCHALL_THROW( decaf::nio::BufferUnderflowException );
}

////////////////////////////////////////////////////////////////////////////////
int ByteArrayBuffer::getInt( std::size_t index ) const
    throw ( lang::exceptions::IndexOutOfBoundsException ) {

    try{

        if( (offset + index + sizeof(int)) > this->limit() ) {
            throw IndexOutOfBoundsException(
                __FILE__, __LINE__,
                "ByteArrayBuffer::getInt(i) - Not enough data to fill an int." );
        };

        return this->_array->getIntAt( offset + index );
    }
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IndexOutOfBoundsException )
    DECAF_CATCHALL_THROW( IndexOutOfBoundsException )
}

////////////////////////////////////////////////////////////////////////////////
short ByteArrayBuffer::getShort() throw( decaf::nio::BufferUnderflowException ) {

    try{

        short value = this->getShort( this->_position );
        this->_position += sizeof(value);
        return value;
    }
    DECAF_CATCH_RETHROW( decaf::nio::BufferUnderflowException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, decaf::nio::BufferUnderflowException )
    DECAF_CATCHALL_THROW( decaf::nio::BufferUnderflowException );
}

////////////////////////////////////////////////////////////////////////////////
short ByteArrayBuffer::getShort( std::size_t index ) const
    throw ( lang::exceptions::IndexOutOfBoundsException )  {

    try{

        if( (offset + index + sizeof(short)) > this->limit() ) {
            throw IndexOutOfBoundsException(
                __FILE__, __LINE__,
                "ByteArrayBuffer::getShort(i) - Not enough data to fill a short." );
        }

        return this->_array->getShortAt( offset + index );
    }
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IndexOutOfBoundsException )
    DECAF_CATCHALL_THROW( IndexOutOfBoundsException )
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayBuffer& ByteArrayBuffer::put( unsigned char value )
    throw( decaf::nio::BufferOverflowException, decaf::nio::ReadOnlyBufferException ) {

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
ByteArrayBuffer& ByteArrayBuffer::put( std::size_t index, unsigned char value )
    throw( lang::exceptions::IndexOutOfBoundsException,
           decaf::nio::ReadOnlyBufferException ) {

    try{

        if( this->isReadOnly() ) {
            throw decaf::nio::ReadOnlyBufferException(
                __FILE__, __LINE__,
                "ByteArrayBuffer::put(i,i) - Buffer is Read Only." );
        }

        if( index >= this->limit() ) {
            throw IndexOutOfBoundsException(
                __FILE__, __LINE__,
                "ByteArrayBuffer::put(i,i) - Not enough data to fill request." );
        }

        this->_array->put( index + offset, value );

        return *this;
    }
    DECAF_CATCH_RETHROW( decaf::nio::ReadOnlyBufferException )
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IndexOutOfBoundsException )
    DECAF_CATCHALL_THROW( IndexOutOfBoundsException )
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayBuffer& ByteArrayBuffer::putChar( char value )
    throw( decaf::nio::BufferOverflowException, decaf::nio::ReadOnlyBufferException ) {

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
ByteArrayBuffer& ByteArrayBuffer::putChar( std::size_t index, char value )
    throw( lang::exceptions::IndexOutOfBoundsException,
           decaf::nio::ReadOnlyBufferException ) {

    try{

        this->put( index, value );
        return *this;
    }
    DECAF_CATCH_RETHROW( decaf::nio::ReadOnlyBufferException )
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IndexOutOfBoundsException )
    DECAF_CATCHALL_THROW( IndexOutOfBoundsException )
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayBuffer& ByteArrayBuffer::putDouble( double value )
    throw( decaf::nio::BufferOverflowException, decaf::nio::ReadOnlyBufferException ) {

    try{

        this->putDouble( this->_position, value );
        this->_position += sizeof(value);
        return *this;
    }
    DECAF_CATCH_RETHROW( decaf::nio::ReadOnlyBufferException )
    DECAF_CATCH_RETHROW( decaf::nio::BufferOverflowException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, decaf::nio::BufferOverflowException )
    DECAF_CATCHALL_THROW( decaf::nio::BufferOverflowException )
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayBuffer& ByteArrayBuffer::putDouble( std::size_t index, double value )
    throw( lang::exceptions::IndexOutOfBoundsException,
           decaf::nio::ReadOnlyBufferException ) {

    try{

        this->putLong( index, Double::doubleToLongBits( value ) );
        return *this;
    }
    DECAF_CATCH_RETHROW( decaf::nio::ReadOnlyBufferException )
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IndexOutOfBoundsException )
    DECAF_CATCHALL_THROW( IndexOutOfBoundsException )
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayBuffer& ByteArrayBuffer::putFloat( float value )
    throw( decaf::nio::BufferOverflowException, decaf::nio::ReadOnlyBufferException ) {

    try{

        this->putFloat( this->_position, value );
        this->_position += sizeof(value);
        return *this;
    }
    DECAF_CATCH_RETHROW( decaf::nio::ReadOnlyBufferException )
    DECAF_CATCH_RETHROW( decaf::nio::BufferOverflowException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, decaf::nio::BufferOverflowException )
    DECAF_CATCHALL_THROW( decaf::nio::BufferOverflowException )
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayBuffer& ByteArrayBuffer::putFloat( std::size_t index, float value )
    throw( lang::exceptions::IndexOutOfBoundsException,
           decaf::nio::ReadOnlyBufferException ) {

    try{

        this->putInt( index, Float::floatToIntBits( value ) );
        return *this;
    }
    DECAF_CATCH_RETHROW( decaf::nio::ReadOnlyBufferException )
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IndexOutOfBoundsException )
    DECAF_CATCHALL_THROW( IndexOutOfBoundsException )
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayBuffer& ByteArrayBuffer::putLong( long long value )
    throw( decaf::nio::BufferOverflowException, decaf::nio::ReadOnlyBufferException ) {

    try{

        this->putLong( this->_position, value );
        this->_position += sizeof(value);
        return *this;
    }
    DECAF_CATCH_RETHROW( decaf::nio::ReadOnlyBufferException )
    DECAF_CATCH_RETHROW( decaf::nio::BufferOverflowException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, decaf::nio::BufferOverflowException )
    DECAF_CATCHALL_THROW( decaf::nio::BufferOverflowException )
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayBuffer& ByteArrayBuffer::putLong( std::size_t index, long long value )
    throw( lang::exceptions::IndexOutOfBoundsException,
           decaf::nio::ReadOnlyBufferException ) {

    try{

        if( this->isReadOnly() ) {
            throw decaf::nio::ReadOnlyBufferException(
                __FILE__, __LINE__,
                "ByteArrayBuffer::putLong() - Buffer is Read Only." );
        }

        this->_array->putLongAt( index + offset, value );

        return *this;
    }
    DECAF_CATCH_RETHROW( decaf::nio::ReadOnlyBufferException )
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IndexOutOfBoundsException )
    DECAF_CATCHALL_THROW( IndexOutOfBoundsException )
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayBuffer& ByteArrayBuffer::putInt( int value )
    throw( decaf::nio::BufferOverflowException, decaf::nio::ReadOnlyBufferException ) {

    try{

        this->putInt( this->_position, value );
        this->_position += sizeof(value);
        return *this;
    }
    DECAF_CATCH_RETHROW( decaf::nio::ReadOnlyBufferException )
    DECAF_CATCH_RETHROW( decaf::nio::BufferOverflowException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, decaf::nio::BufferOverflowException )
    DECAF_CATCHALL_THROW( decaf::nio::BufferOverflowException )
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayBuffer& ByteArrayBuffer::putInt( std::size_t index, int value )
    throw( lang::exceptions::IndexOutOfBoundsException,
           decaf::nio::ReadOnlyBufferException ) {

    try{

        if( this->isReadOnly() ) {
            throw decaf::nio::ReadOnlyBufferException(
                __FILE__, __LINE__,
                "ByteArrayBuffer::putInt() - Buffer is Read Only." );
        }

        this->_array->putIntAt( index + offset, value );

        return *this;
    }
    DECAF_CATCH_RETHROW( decaf::nio::ReadOnlyBufferException )
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IndexOutOfBoundsException )
    DECAF_CATCHALL_THROW( IndexOutOfBoundsException )
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayBuffer& ByteArrayBuffer::putShort( short value )
    throw( decaf::nio::BufferOverflowException, decaf::nio::ReadOnlyBufferException ) {

    try{

        this->putShort( this->_position, value );
        this->_position += sizeof(value);
        return *this;
    }
    DECAF_CATCH_RETHROW( decaf::nio::ReadOnlyBufferException )
    DECAF_CATCH_RETHROW( decaf::nio::BufferOverflowException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, decaf::nio::BufferOverflowException )
    DECAF_CATCHALL_THROW( decaf::nio::BufferOverflowException )
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayBuffer& ByteArrayBuffer::putShort( std::size_t index, short value )
    throw( lang::exceptions::IndexOutOfBoundsException,
           decaf::nio::ReadOnlyBufferException ) {

    try{

        if( this->isReadOnly() ) {
            throw decaf::nio::ReadOnlyBufferException(
                __FILE__, __LINE__,
                "ByteArrayBuffer::putShort() - Buffer is Read Only." );
        }

        this->_array->putShortAt( index + offset, value );

        return *this;
    }
    DECAF_CATCH_RETHROW( decaf::nio::ReadOnlyBufferException )
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IndexOutOfBoundsException )
    DECAF_CATCHALL_THROW( IndexOutOfBoundsException )
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayBuffer* ByteArrayBuffer::slice() const {

    try{

        return new ByteArrayBuffer( *(this->_array),
                                    this->offset + this->position(),
                                    this->remaining(),
                                    this->isReadOnly() );
    }
    DECAF_CATCH_RETHROW( Exception )
    DECAF_CATCHALL_THROW( Exception )
}
