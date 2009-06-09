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

#include "DoubleBuffer.h"

#include <decaf/lang/Double.h>
#include <decaf/lang/Math.h>
#include "decaf/internal/nio/BufferFactory.h"

using namespace std;
using namespace decaf;
using namespace decaf::nio;
using namespace decaf::internal::nio;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
DoubleBuffer::DoubleBuffer( std::size_t capacity )
 :  Buffer( capacity ) {

}

////////////////////////////////////////////////////////////////////////////////
DoubleBuffer* DoubleBuffer::allocate( std::size_t capacity ) {

    try{

        return BufferFactory::createDoubleBuffer( capacity );
    }
    DECAF_CATCH_RETHROW( Exception )
    DECAF_CATCHALL_THROW( Exception )
}

////////////////////////////////////////////////////////////////////////////////
DoubleBuffer* DoubleBuffer::wrap( double* buffer, std::size_t offset, std::size_t length )
    throw( lang::exceptions::NullPointerException ) {

    try{

        if( buffer == NULL ) {
            throw NullPointerException(
                __FILE__, __LINE__,
                "DoubleBuffer::wrap - Passed Buffer is Null.");
        }

        return BufferFactory::createDoubleBuffer( buffer, offset, length );
    }
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, NullPointerException )
    DECAF_CATCHALL_THROW( NullPointerException )
}

////////////////////////////////////////////////////////////////////////////////
DoubleBuffer* DoubleBuffer::wrap( std::vector<double>& buffer ) {

    try{

        if( buffer.empty() ) {
            throw NullPointerException(
                __FILE__, __LINE__,
                "DoubleBuffer::wrap - Passed Buffer is Empty.");
        }

        return BufferFactory::createDoubleBuffer( &buffer[0], 0, buffer.size() );
    }
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, NullPointerException )
    DECAF_CATCHALL_THROW( NullPointerException )
}

////////////////////////////////////////////////////////////////////////////////
std::string DoubleBuffer::toString() const {

    std::ostringstream stream;

    stream << "DoubleBuffer, status: "
           << "capacity =" << this->capacity()
           << " position =" << this->position()
           << " limit = " << this->limit();

    return stream.str();
}

////////////////////////////////////////////////////////////////////////////////
DoubleBuffer& DoubleBuffer::get( std::vector<double> buffer )
    throw ( BufferUnderflowException ) {

    try{

        if( !buffer.empty() ) {
            this->get( &buffer[0], 0, buffer.size() );
        }
        return *this;
    }
    DECAF_CATCH_RETHROW( BufferUnderflowException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, BufferUnderflowException )
    DECAF_CATCHALL_THROW( BufferUnderflowException )
}

////////////////////////////////////////////////////////////////////////////////
DoubleBuffer& DoubleBuffer::get( double* buffer, std::size_t offset, std::size_t length )
    throw( BufferUnderflowException,
           lang::exceptions::NullPointerException ) {

    try{

        if( length == 0 ) {
            return *this;
        }

        if( buffer == NULL ) {
            throw NullPointerException(
                __FILE__, __LINE__,
                "DoubleBuffer::get - Passed Buffer is Null" );
        }

        if( length > remaining() ) {
            throw BufferUnderflowException(
                __FILE__, __LINE__,
                "DoubleBuffer::get - Not enough data to fill length = %d", length );
        }

        for( std::size_t ix = 0; ix < length; ++ix ){
            buffer[offset + ix] = this->get();
        }

        return *this;
    }
    DECAF_CATCH_RETHROW( BufferUnderflowException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, BufferUnderflowException )
    DECAF_CATCHALL_THROW( BufferUnderflowException )
}

////////////////////////////////////////////////////////////////////////////////
DoubleBuffer& DoubleBuffer::put( DoubleBuffer& src )
    throw( BufferOverflowException, ReadOnlyBufferException,
           lang::exceptions::IllegalArgumentException ) {

    try{

        if( this == &src ) {
            throw IllegalArgumentException(
                __FILE__, __LINE__,
                "DoubleBuffer::put - Can't put Self" );
        }

        if( this->isReadOnly() ) {
            throw ReadOnlyBufferException(
                __FILE__, __LINE__,
                "DoubleBuffer::put - This buffer is Read Only.");
        }

        if( src.remaining() > this->remaining() ) {
            throw BufferOverflowException(
                __FILE__, __LINE__,
                "DoubleBuffer::put - Not enough space remaining to put src." );
        }

        while( src.hasRemaining() ) {
            this->put( src.get() );
        }

        return *this;
    }
    DECAF_CATCH_RETHROW( BufferOverflowException )
    DECAF_CATCH_RETHROW( ReadOnlyBufferException )
    DECAF_CATCH_RETHROW( IllegalArgumentException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, BufferOverflowException )
    DECAF_CATCHALL_THROW( BufferOverflowException )
}

////////////////////////////////////////////////////////////////////////////////
DoubleBuffer& DoubleBuffer::put( const double* buffer, std::size_t offset, std::size_t length )
    throw( BufferOverflowException, ReadOnlyBufferException,
           lang::exceptions::NullPointerException ) {

    try{

        if( length == 0 ) {
            return *this;
        }

        if( this->isReadOnly() ) {
            throw ReadOnlyBufferException(
                __FILE__, __LINE__,
                "DoubleBuffer::put - This buffer is Read Only.");
        }

        if( buffer == NULL ) {
            throw NullPointerException(
                __FILE__, __LINE__,
                "DoubleBuffer::put - Passed Buffer is Null.");
        }

        if( length > this->remaining() ) {
            throw BufferOverflowException(
                __FILE__, __LINE__,
                "DoubleBuffer::put - Not Enough space to store requested Data.");
        }

        // read length bytes starting from the offset
        for( std::size_t ix = 0; ix < length; ++ix ) {
            this->put( buffer[ix + offset] );
        }

        return *this;
    }
    DECAF_CATCH_RETHROW( BufferOverflowException )
    DECAF_CATCH_RETHROW( ReadOnlyBufferException )
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, BufferOverflowException )
    DECAF_CATCHALL_THROW( BufferOverflowException )
}

////////////////////////////////////////////////////////////////////////////////
DoubleBuffer& DoubleBuffer::put( std::vector<double>& buffer )
    throw( BufferOverflowException, ReadOnlyBufferException ) {

    try{

        if( !buffer.empty() ) {
            this->put( &buffer[0], 0, buffer.size() );
        }

        return *this;
    }
    DECAF_CATCH_RETHROW( BufferOverflowException )
    DECAF_CATCH_RETHROW( ReadOnlyBufferException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, BufferOverflowException )
    DECAF_CATCHALL_THROW( BufferOverflowException )
}

////////////////////////////////////////////////////////////////////////////////
int DoubleBuffer::compareTo( const DoubleBuffer& value ) const {

    int compareRemaining = Math::min( (int)remaining(), (int)value.remaining() );

    std::size_t thisPos = this->position();
    std::size_t otherPos = value.position();
    double thisByte, otherByte;

    while( compareRemaining > 0 ) {

        thisByte = get( thisPos );
        otherByte = value.get( otherPos );

        if( thisByte != otherByte ) {
            return thisByte < otherByte ? -1 : 1;
        }

        thisPos++;
        otherPos++;
        compareRemaining--;
    }

    return (int)( remaining() - value.remaining() );
}

////////////////////////////////////////////////////////////////////////////////
bool DoubleBuffer::equals( const DoubleBuffer& value ) const {

    if( &value == this ) {
        return true;
    }

    if( this->remaining() != value.remaining() ) {
        return false;
    }

    std::size_t myPosition = this->position();
    std::size_t otherPosition = value.position();
    bool equalSoFar = true;

    while( equalSoFar && ( myPosition < this->limit() ) ) {
        equalSoFar = get( myPosition++ ) == value.get( otherPosition++ );
    }

    return equalSoFar;
}

////////////////////////////////////////////////////////////////////////////////
bool DoubleBuffer::operator==( const DoubleBuffer& value ) const {
    return this->equals( value );
}

////////////////////////////////////////////////////////////////////////////////
bool DoubleBuffer::operator<( const DoubleBuffer& value ) const {
    return this->compareTo( value ) < 0 ? true : false;
}
