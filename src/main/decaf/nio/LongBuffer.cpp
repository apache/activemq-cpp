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

#include "LongBuffer.h"

#include <decaf/lang/Long.h>
#include <decaf/lang/Math.h>
#include "decaf/internal/nio/BufferFactory.h"

using namespace std;
using namespace decaf;
using namespace decaf::nio;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::internal::nio;

////////////////////////////////////////////////////////////////////////////////
LongBuffer::LongBuffer( std::size_t capacity )
 :  Buffer( capacity ) {

}

////////////////////////////////////////////////////////////////////////////////
LongBuffer* LongBuffer::allocate( std::size_t capacity ) {

    try{

        return BufferFactory::createLongBuffer( capacity );
    }
    DECAF_CATCH_RETHROW( Exception )
    DECAF_CATCHALL_THROW( Exception )
}

////////////////////////////////////////////////////////////////////////////////
LongBuffer* LongBuffer::wrap( long long* buffer, std::size_t offset, std::size_t length )
    throw( lang::exceptions::NullPointerException ) {

    try{

        if( buffer == NULL ) {
            throw NullPointerException(
                __FILE__, __LINE__,
                "LongBuffer::wrap - Passed Buffer is Null.");
        }

        return BufferFactory::createLongBuffer( buffer, offset, length );
    }
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, NullPointerException )
    DECAF_CATCHALL_THROW( NullPointerException )
}

////////////////////////////////////////////////////////////////////////////////
LongBuffer* LongBuffer::wrap( std::vector<long long>& buffer ) {

    try{

        if( buffer.empty() ) {
            throw NullPointerException(
                __FILE__, __LINE__,
                "LongBuffer::wrap - Passed Buffer is Empty.");
        }

        return BufferFactory::createLongBuffer( &buffer[0], 0, buffer.size() );
    }
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, NullPointerException )
    DECAF_CATCHALL_THROW( NullPointerException )
}

////////////////////////////////////////////////////////////////////////////////
std::string LongBuffer::toString() const {

    std::ostringstream stream;

    stream << "LongBuffer, status: "
           << "capacity =" << this->capacity()
           << " position =" << this->position()
           << " limit = " << this->limit();

    return stream.str();
}

////////////////////////////////////////////////////////////////////////////////
LongBuffer& LongBuffer::get( std::vector<long long> buffer )
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
LongBuffer& LongBuffer::get( long long* buffer, std::size_t offset, std::size_t length )
    throw( BufferUnderflowException,
           lang::exceptions::NullPointerException ) {

    try{

        if( length == 0 ) {
            return *this;
        }

        if( buffer == NULL ) {
            throw NullPointerException(
                __FILE__, __LINE__,
                "LongBuffer::get - Passed Buffer is Null" );
        }

        if( length > remaining() ) {
            throw BufferUnderflowException(
                __FILE__, __LINE__,
                "LongBuffer::get - Not enough data to fill length = %d", length );
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
LongBuffer& LongBuffer::put( LongBuffer& src )
    throw( BufferOverflowException, ReadOnlyBufferException,
           lang::exceptions::IllegalArgumentException ) {

    try{

        if( this == &src ) {
            throw IllegalArgumentException(
                __FILE__, __LINE__,
                "LongBuffer::put - Can't put Self" );
        }

        if( this->isReadOnly() ) {
            throw ReadOnlyBufferException(
                __FILE__, __LINE__,
                "LongBuffer::put - This buffer is Read Only.");
        }

        if( src.remaining() > this->remaining() ) {
            throw BufferOverflowException(
                __FILE__, __LINE__,
                "LongBuffer::put - Not enough space remaining to put src." );
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
LongBuffer& LongBuffer::put( const long long* buffer, std::size_t offset, std::size_t length )
    throw( BufferOverflowException, ReadOnlyBufferException,
           lang::exceptions::NullPointerException ) {

    try{

        if( length == 0 ) {
            return *this;
        }

        if( this->isReadOnly() ) {
            throw ReadOnlyBufferException(
                __FILE__, __LINE__,
                "LongBuffer::put - This buffer is Read Only.");
        }

        if( buffer == NULL ) {
            throw NullPointerException(
                __FILE__, __LINE__,
                "LongBuffer::put - Passed Buffer is Null.");
        }

        if( length > this->remaining() ) {
            throw BufferOverflowException(
                __FILE__, __LINE__,
                "LongBuffer::put - Not Enough space to store requested Data.");
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
LongBuffer& LongBuffer::put( std::vector<long long>& buffer )
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
int LongBuffer::compareTo( const LongBuffer& value ) const {

    std::size_t compareRemaining = (std::size_t)Math::min( (int)remaining(), (int)value.remaining() );

    std::size_t thisPos = this->position();
    std::size_t otherPos = value.position();
    long long thisVal, otherVal;

    while( compareRemaining > 0 ) {

        thisVal = get( thisPos );
        otherVal = value.get( otherPos );

        if( thisVal != otherVal ) {
            return thisVal < otherVal ? -1 : 1;
        }

        thisPos++;
        otherPos++;
        compareRemaining--;
    }

    return (int)( remaining() - value.remaining() );
}

////////////////////////////////////////////////////////////////////////////////
bool LongBuffer::equals( const LongBuffer& value ) const {

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
bool LongBuffer::operator==( const LongBuffer& value ) const {
    return this->equals( value );
}

////////////////////////////////////////////////////////////////////////////////
bool LongBuffer::operator<( const LongBuffer& value ) const {
    return this->compareTo( value ) < 0 ? true : false;
}
