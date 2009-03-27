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

#include "ByteArrayAdapter.h"

#include <string.h>
#include <stdio.h>
#include <decaf/lang/Math.h>
#include <decaf/lang/Float.h>
#include <decaf/lang/Double.h>

using namespace decaf;
using namespace decaf::nio;
using namespace decaf::internal;
using namespace decaf::internal::util;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
ByteArrayAdapter::ByteArrayAdapter( std::size_t capacity ) {

    try{

        this->array.bytes = new unsigned char[capacity];
        memset( this->array.bytes, 0, capacity );
        this->initialize( array.bytes, capacity, true );
    }
    DECAF_CATCH_RETHROW( Exception )
    DECAF_CATCHALL_THROW( Exception )
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayAdapter::ByteArrayAdapter( unsigned char* array, std::size_t capacity, bool own )
    throw( lang::exceptions::NullPointerException ) {

    try{
        this->initialize( array, capacity, own );
    }
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCHALL_THROW( NullPointerException )
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayAdapter::ByteArrayAdapter( char* array, std::size_t capacity, bool own )
    throw( lang::exceptions::NullPointerException ) {

    try{
        this->initialize( reinterpret_cast<unsigned char*>( array ), capacity, own );
    }
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCHALL_THROW( NullPointerException )
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayAdapter::ByteArrayAdapter( double* array, std::size_t capacity, bool own )
    throw( lang::exceptions::NullPointerException ) {

    try{
        this->initialize( reinterpret_cast<unsigned char*>( array ),
                          capacity * sizeof( double ), own );
    }
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCHALL_THROW( NullPointerException )
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayAdapter::ByteArrayAdapter( float* array, std::size_t capacity, bool own )
    throw( lang::exceptions::NullPointerException ) {

    try{
        this->initialize( reinterpret_cast<unsigned char*>( array ),
                          capacity * sizeof( float ), own );
    }
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCHALL_THROW( NullPointerException )
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayAdapter::ByteArrayAdapter( long long* array, std::size_t capacity, bool own )
    throw( lang::exceptions::NullPointerException ) {

    try{
        this->initialize( reinterpret_cast<unsigned char*>( array ),
                          capacity * sizeof( long long ), own );
    }
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCHALL_THROW( NullPointerException )
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayAdapter::ByteArrayAdapter( int* array, std::size_t capacity, bool own )
    throw( lang::exceptions::NullPointerException ) {

    try{
        this->initialize( reinterpret_cast<unsigned char*>( array ),
                          capacity * sizeof( int ), own );
    }
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCHALL_THROW( NullPointerException )
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayAdapter::ByteArrayAdapter( short* array, std::size_t capacity, bool own )
    throw( lang::exceptions::NullPointerException ) {

    try{
        this->initialize( reinterpret_cast<unsigned char*>( array ),
                          capacity * sizeof( short ), own );
    }
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCHALL_THROW( NullPointerException )
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayAdapter::initialize( unsigned char* array, std::size_t capacity, bool own ) {

    try{

        if( array == NULL ) {
            throw NullPointerException(
                __FILE__, __LINE__,
                "ByteArrayAdapter::initialize - Passed Buffer is null" );
        }

        this->array.bytes = array;
        this->capacity = capacity;
        this->own = own;
    }
    DECAF_CATCH_RETHROW( Exception )
    DECAF_CATCHALL_THROW( Exception )
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayAdapter::~ByteArrayAdapter() {

    try{

        if( own ) {
            delete [] array.bytes;
        }
    }
    DECAF_CATCH_NOTHROW( Exception )
    DECAF_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayAdapter::read( unsigned char* buffer, std::size_t offset, std::size_t length ) const
    throw( NullPointerException, BufferUnderflowException ) {

    try{

        if( length == 0 ) {
            return;
        }

        if( buffer == NULL ) {
            throw NullPointerException(
                __FILE__, __LINE__,
                "ByteArrayAdapter::write - Passed buffer is null" );
        }

        if( ( offset + length ) > this->capacity ) {
            throw BufferUnderflowException(
                __FILE__, __LINE__,
                "ByteArrayAdapter::read - Not enough data to fill request." );
        }

        // Read, starting at offset, length number of bytes to Buffer
        memcpy( buffer, this->array.bytes + offset, length );
    }
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCH_RETHROW( BufferUnderflowException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, BufferUnderflowException )
    DECAF_CATCHALL_THROW( BufferUnderflowException )
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayAdapter::write( unsigned char* buffer, std::size_t offset, std::size_t length )
    throw( NullPointerException, BufferOverflowException ) {

    try{

        if( length == 0 ) {
            return;
        }

        if( buffer == NULL ) {
            throw NullPointerException(
                __FILE__, __LINE__,
                "ByteArrayAdapter::write - Passed buffer is null" );
        }

        if( ( offset + length ) > this->capacity ) {
            throw BufferOverflowException(
                __FILE__, __LINE__,
                "ByteArrayAdapter::read - Not room in array to hold request." );
        }

        // Write, starting at offset, length number of bytes from buffer.
        memcpy( this->array.bytes + offset, buffer, length );
    }
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCH_RETHROW( BufferOverflowException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, BufferOverflowException )
    DECAF_CATCHALL_THROW( BufferOverflowException )
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayAdapter::resize( std::size_t capacity )
    throw ( lang::exceptions::InvalidStateException ) {

    try{

        if( !own ) {
            throw InvalidStateException(
                __FILE__, __LINE__,
                "ByteArrayAdapter::resize - Not the array owner, can't resize" );
        }

        // Save old state
        std::size_t oldCapacity = this->capacity;
        unsigned char* oldArray = this->array.bytes;

        // Resize and copy as much of the old as we can back and delete old array
        this->array.bytes = new unsigned char[capacity];
        this->capacity = capacity;
        memcpy( this->array.bytes, oldArray, Math::min( (int)oldCapacity, (int)capacity ) );
        delete [] oldArray;
    }
    DECAF_CATCH_RETHROW( InvalidStateException )
    DECAF_CATCHALL_THROW( InvalidStateException )
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayAdapter::clear() {
    memset( this->array.bytes, 0, this->capacity );
}

////////////////////////////////////////////////////////////////////////////////
unsigned char& ByteArrayAdapter::operator[]( std::size_t index )
    throw ( IndexOutOfBoundsException ) {

    try{

        if( index > this->capacity ) {
            throw IndexOutOfBoundsException(
                __FILE__, __LINE__,
                "ByteArrayAdapter::operator[] - Index %d is out of bounds", index );
        }

        return this->array.bytes[index];
    }
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCHALL_THROW( IndexOutOfBoundsException )
}

////////////////////////////////////////////////////////////////////////////////
const unsigned char& ByteArrayAdapter::operator[]( std::size_t index ) const
    throw ( decaf::lang::exceptions::IndexOutOfBoundsException ) {

    try{

        if( index > this->capacity ) {
            throw IndexOutOfBoundsException(
                __FILE__, __LINE__,
                "ByteArrayAdapter::operator[] - Index %d is out of bounds", index );
        }

        return this->array.bytes[index];
    }
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCHALL_THROW( IndexOutOfBoundsException )
}

////////////////////////////////////////////////////////////////////////////////
unsigned char ByteArrayAdapter::get( std::size_t index ) const
    throw ( lang::exceptions::IndexOutOfBoundsException ) {

    try{

        if( index >= this->getCapacity() ) {
            throw IndexOutOfBoundsException(
                __FILE__, __LINE__,
                "ByteArrayAdapter::get - Not enough data to fill request." );
        }

        return (*this)[index];
    }
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IndexOutOfBoundsException )
    DECAF_CATCHALL_THROW( IndexOutOfBoundsException )
}

////////////////////////////////////////////////////////////////////////////////
double ByteArrayAdapter::getDouble( std::size_t index ) const
    throw ( lang::exceptions::IndexOutOfBoundsException ) {

    try{

        if( index >= this->getDoubleCapacity() ) {
            throw IndexOutOfBoundsException(
                __FILE__, __LINE__,
                "ByteArrayAdapter::getDouble(i) - Not enough data to fill a long long." );
        }

        return this->array.doubles[ index ];
    }
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IndexOutOfBoundsException )
    DECAF_CATCHALL_THROW( IndexOutOfBoundsException )
}

////////////////////////////////////////////////////////////////////////////////
double ByteArrayAdapter::getDoubleAt( std::size_t index ) const
    throw ( lang::exceptions::IndexOutOfBoundsException ) {

    try{

        unsigned long long lvalue = this->getLongAt( index );
        return Double::longBitsToDouble( lvalue );
    }
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IndexOutOfBoundsException )
    DECAF_CATCHALL_THROW( IndexOutOfBoundsException )
}

////////////////////////////////////////////////////////////////////////////////
float ByteArrayAdapter::getFloat( std::size_t index ) const
    throw ( lang::exceptions::IndexOutOfBoundsException ) {

    try{

        if( index >= this->getFloatCapacity() ) {
            throw IndexOutOfBoundsException(
                __FILE__, __LINE__,
                "ByteArrayAdapter::getFloat(i) - Not enough data to fill a long long." );
        }

        return this->array.floats[ index ];
    }
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IndexOutOfBoundsException )
    DECAF_CATCHALL_THROW( IndexOutOfBoundsException )
}

////////////////////////////////////////////////////////////////////////////////
float ByteArrayAdapter::getFloatAt( std::size_t index ) const
    throw ( lang::exceptions::IndexOutOfBoundsException ) {

    try{

        unsigned int ivalue = this->getIntAt( index );
        return Float::intBitsToFloat( ivalue );
    }
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IndexOutOfBoundsException )
    DECAF_CATCHALL_THROW( IndexOutOfBoundsException )
}

////////////////////////////////////////////////////////////////////////////////
long long ByteArrayAdapter::getLong( std::size_t index ) const
    throw ( lang::exceptions::IndexOutOfBoundsException ) {

    try{

        if( index >= this->getLongCapacity() ) {
            throw IndexOutOfBoundsException(
                __FILE__, __LINE__,
                "ByteArrayAdapter::getLong(i) - Not enough data to fill a long long." );
        }

        return this->array.longs[ index ];
    }
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IndexOutOfBoundsException )
    DECAF_CATCHALL_THROW( IndexOutOfBoundsException )
}

////////////////////////////////////////////////////////////////////////////////
long long ByteArrayAdapter::getLongAt( std::size_t index ) const
    throw ( lang::exceptions::IndexOutOfBoundsException ) {

    try{

        if( ( index + sizeof( long long ) ) > this->getCapacity() ) {
            throw IndexOutOfBoundsException(
                __FILE__, __LINE__,
                "ByteArrayAdapter::getLong(i) - Not enough data to fill a long long." );
        }

        unsigned long long value = 0;
        unsigned char buffer[sizeof(value)] = {0};
        this->read( buffer, index, sizeof(value) );

        // Have to do it this way because on Solaris and Cygwin we get all
        // kinds of warnings when shifting a byte up into a long long.
        unsigned long long byte1 = buffer[0] & 0x00000000000000FFULL;
        unsigned long long byte2 = buffer[1] & 0x00000000000000FFULL;
        unsigned long long byte3 = buffer[2] & 0x00000000000000FFULL;
        unsigned long long byte4 = buffer[3] & 0x00000000000000FFULL;
        unsigned long long byte5 = buffer[4] & 0x00000000000000FFULL;
        unsigned long long byte6 = buffer[5] & 0x00000000000000FFULL;
        unsigned long long byte7 = buffer[6] & 0x00000000000000FFULL;
        unsigned long long byte8 = buffer[7] & 0x00000000000000FFULL;

        value = ( byte1 << 56 | byte2 << 48 | byte3 << 40 | byte4 << 32 |
                  byte5 << 24 | byte6 << 16 | byte7 << 8  | byte8 << 0 );

        return value;
    }
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IndexOutOfBoundsException )
    DECAF_CATCHALL_THROW( IndexOutOfBoundsException )
}

////////////////////////////////////////////////////////////////////////////////
int ByteArrayAdapter::getInt( std::size_t index ) const
    throw ( lang::exceptions::IndexOutOfBoundsException ) {

    try{

        if( index >= this->getIntCapacity() ) {
            throw IndexOutOfBoundsException(
                __FILE__, __LINE__,
                "ByteArrayAdapter::getInt(i) - Not enough data to fill an int." );
        }

        return this->array.ints[ index ];
    }
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IndexOutOfBoundsException )
    DECAF_CATCHALL_THROW( IndexOutOfBoundsException )
}

////////////////////////////////////////////////////////////////////////////////
int ByteArrayAdapter::getIntAt( std::size_t index ) const
    throw ( lang::exceptions::IndexOutOfBoundsException ) {

    try{

        if( ( index + sizeof( int ) ) > this->getCapacity() ) {
            throw IndexOutOfBoundsException(
                __FILE__, __LINE__,
                "ByteArrayAdapter::getInt(i) - Not enough data to fill an int." );
        }

        unsigned int value = 0;
        unsigned char buffer[sizeof(value)] = {0};
        this->read( buffer, index, sizeof(value) );
        value |= (buffer[0] << 24 | buffer[1] << 16 |
                  buffer[2] << 8 | buffer[3] << 0);

        return value;
    }
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IndexOutOfBoundsException )
    DECAF_CATCHALL_THROW( IndexOutOfBoundsException )
}

////////////////////////////////////////////////////////////////////////////////
short ByteArrayAdapter::getShort( std::size_t index ) const
    throw ( lang::exceptions::IndexOutOfBoundsException )  {

    try{

        if( index >= this->getShortCapacity() ) {
            throw IndexOutOfBoundsException(
                __FILE__, __LINE__,
                "ByteArrayAdapter::getShort(i) - Not enough data to fill a short." );
        }

        return this->array.shorts[ index ];
    }
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IndexOutOfBoundsException )
    DECAF_CATCHALL_THROW( IndexOutOfBoundsException )
}

////////////////////////////////////////////////////////////////////////////////
short ByteArrayAdapter::getShortAt( std::size_t index ) const
    throw ( lang::exceptions::IndexOutOfBoundsException )  {

    try{

        if( ( index + sizeof( short ) ) > this->getCapacity() ) {
            throw IndexOutOfBoundsException(
                __FILE__, __LINE__,
                "ByteArrayAdapter::getShort(i) - Not enough data to fill a short." );
        }

        short value = 0;
        unsigned char buffer[sizeof(value)] = {0};
        this->read( buffer, index, sizeof(value) );
        value |= (buffer[0] << 8 | buffer[1] << 0);

        return value;
    }
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IndexOutOfBoundsException )
    DECAF_CATCHALL_THROW( IndexOutOfBoundsException )
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayAdapter& ByteArrayAdapter::put( std::size_t index, unsigned char value )
    throw( lang::exceptions::IndexOutOfBoundsException ) {

    try{

        if( index >= this->getCapacity() ) {
            throw IndexOutOfBoundsException(
                __FILE__, __LINE__,
                "ByteArrayAdapter::put(i,i) - Not enough data to fill request." );
        }

        (*this)[index] = value;

        return *this;
    }
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IndexOutOfBoundsException )
    DECAF_CATCHALL_THROW( IndexOutOfBoundsException )
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayAdapter& ByteArrayAdapter::putChar( std::size_t index, char value )
    throw( lang::exceptions::IndexOutOfBoundsException ) {

    try{

        this->put( index, value );
        return *this;
    }
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IndexOutOfBoundsException )
    DECAF_CATCHALL_THROW( IndexOutOfBoundsException )
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayAdapter& ByteArrayAdapter::putDouble( std::size_t index, double value )
    throw( lang::exceptions::IndexOutOfBoundsException ) {

    try{

        if( index >= this->getDoubleCapacity() ) {
            throw IndexOutOfBoundsException(
                __FILE__, __LINE__,
                "ByteArrayAdapter::putDouble(i,i) - Not enough data to fill request." );
        }

        this->array.doubles[ index ] = value;
        return *this;
    }
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IndexOutOfBoundsException )
    DECAF_CATCHALL_THROW( IndexOutOfBoundsException )
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayAdapter& ByteArrayAdapter::putDoubleAt( std::size_t index, double value )
    throw( lang::exceptions::IndexOutOfBoundsException ) {

    try{

        this->putLongAt( index, Double::doubleToLongBits( value ) );
        return *this;
    }
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IndexOutOfBoundsException )
    DECAF_CATCHALL_THROW( IndexOutOfBoundsException )
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayAdapter& ByteArrayAdapter::putFloat( std::size_t index, float value )
    throw( lang::exceptions::IndexOutOfBoundsException ) {

    try{

        if( index >= this->getFloatCapacity() ) {
            throw IndexOutOfBoundsException(
                __FILE__, __LINE__,
                "ByteArrayAdapter::putFloat(i,i) - Not enough data to fill request." );
        }

        this->array.floats[index] = value;
        return *this;
    }
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IndexOutOfBoundsException )
    DECAF_CATCHALL_THROW( IndexOutOfBoundsException )
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayAdapter& ByteArrayAdapter::putFloatAt( std::size_t index, float value )
    throw( lang::exceptions::IndexOutOfBoundsException ) {

    try{

        this->putIntAt( index, Float::floatToIntBits( value ) );
        return *this;
    }
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IndexOutOfBoundsException )
    DECAF_CATCHALL_THROW( IndexOutOfBoundsException )
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayAdapter& ByteArrayAdapter::putLong( std::size_t index, long long value )
    throw( lang::exceptions::IndexOutOfBoundsException ) {

    try{

        if( index >= this->getLongCapacity() ) {
            throw IndexOutOfBoundsException(
                __FILE__, __LINE__,
                "ByteArrayAdapter::putLong(i,i) - Not enough data to fill request." );
        }

        this->array.longs[index] = value;
        return *this;
    }
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IndexOutOfBoundsException )
    DECAF_CATCHALL_THROW( IndexOutOfBoundsException )
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayAdapter& ByteArrayAdapter::putLongAt( std::size_t index, long long value )
    throw( lang::exceptions::IndexOutOfBoundsException ) {

    try{

        unsigned char buffer[sizeof(value)];

        buffer[0] = (unsigned char)((value & 0xFF00000000000000ULL) >> 56);
        buffer[1] = (unsigned char)((value & 0x00FF000000000000ULL) >> 48);
        buffer[2] = (unsigned char)((value & 0x0000FF0000000000ULL) >> 40);
        buffer[3] = (unsigned char)((value & 0x000000FF00000000ULL) >> 32);
        buffer[4] = (unsigned char)((value & 0x00000000FF000000ULL) >> 24);
        buffer[5] = (unsigned char)((value & 0x0000000000FF0000ULL) >> 16);
        buffer[6] = (unsigned char)((value & 0x000000000000FF00ULL) >> 8);
        buffer[7] = (unsigned char)((value & 0x00000000000000FFULL) >> 0);

        this->write( buffer, index, sizeof(value) );

        return *this;
    }
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IndexOutOfBoundsException )
    DECAF_CATCHALL_THROW( IndexOutOfBoundsException )
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayAdapter& ByteArrayAdapter::putInt( std::size_t index, int value )
    throw( lang::exceptions::IndexOutOfBoundsException ) {

    try{

        if( index >= this->getIntCapacity() ) {
            throw IndexOutOfBoundsException(
                __FILE__, __LINE__,
                "ByteArrayAdapter::putInt(i,i) - Not enough data to fill request." );
        }

        this->array.ints[index] = value;
        return *this;
    }
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IndexOutOfBoundsException )
    DECAF_CATCHALL_THROW( IndexOutOfBoundsException )
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayAdapter& ByteArrayAdapter::putIntAt( std::size_t index, int value )
    throw( lang::exceptions::IndexOutOfBoundsException ) {

    try{

        unsigned char buffer[sizeof(value)];

        buffer[0] = (value & 0xFF000000) >> 24;
        buffer[1] = (value & 0x00FF0000) >> 16;
        buffer[2] = (value & 0x0000FF00) >> 8;
        buffer[3] = (value & 0x000000FF) >> 0;

        this->write( buffer, index, sizeof(value) );

        return *this;
    }
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IndexOutOfBoundsException )
    DECAF_CATCHALL_THROW( IndexOutOfBoundsException )
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayAdapter& ByteArrayAdapter::putShort( std::size_t index, short value )
    throw( lang::exceptions::IndexOutOfBoundsException ) {

    try{

        if( index >= this->getShortCapacity() ) {
            throw IndexOutOfBoundsException(
                __FILE__, __LINE__,
                "ByteArrayAdapter::putShort(i,i) - Not enough data to fill request." );
        }

        this->array.shorts[index] = value;
        return *this;
    }
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IndexOutOfBoundsException )
    DECAF_CATCHALL_THROW( IndexOutOfBoundsException )
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayAdapter& ByteArrayAdapter::putShortAt( std::size_t index, short value )
    throw( lang::exceptions::IndexOutOfBoundsException ) {

    try{

        unsigned char buffer[sizeof(value)];

        buffer[0] = (value & 0xFF00) >> 8;
        buffer[1] = (value & 0x00FF) >> 0;

        this->write( buffer, index, sizeof(value) );

        return *this;
    }
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IndexOutOfBoundsException )
    DECAF_CATCHALL_THROW( IndexOutOfBoundsException )
}
