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

#include "ShortBuffer.h"

#include <decaf/lang/Float.h>
#include <decaf/lang/Math.h>
#include "decaf/internal/nio/BufferFactory.h"

using namespace std;
using namespace decaf;
using namespace decaf::nio;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::internal::nio;

////////////////////////////////////////////////////////////////////////////////
ShortBuffer::ShortBuffer( std::size_t capacity )
 :  Buffer( capacity ) {

}

////////////////////////////////////////////////////////////////////////////////
ShortBuffer* ShortBuffer::allocate( std::size_t capacity ) {

    try{

        return BufferFactory::createShortBuffer( capacity );
    }
    DECAF_CATCH_RETHROW( Exception )
    DECAF_CATCHALL_THROW( Exception )
}

////////////////////////////////////////////////////////////////////////////////
ShortBuffer* ShortBuffer::wrap( short* buffer, std::size_t offset, std::size_t length )
    throw( lang::exceptions::NullPointerException ) {

    try{

        if( buffer == NULL ) {
            throw NullPointerException(
                __FILE__, __LINE__,
                "ShortBuffer::wrap - Passed Buffer is Null.");
        }

        return BufferFactory::createShortBuffer( buffer, offset, length );
    }
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, NullPointerException )
    DECAF_CATCHALL_THROW( NullPointerException )
}

////////////////////////////////////////////////////////////////////////////////
ShortBuffer* ShortBuffer::wrap( std::vector<short>& buffer ) {

    try{

        if( buffer.empty() ) {
            throw NullPointerException(
                __FILE__, __LINE__,
                "ShortBuffer::wrap - Passed Buffer is Empty.");
        }

        return BufferFactory::createShortBuffer( &buffer[0], 0, buffer.size() );
    }
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, NullPointerException )
    DECAF_CATCHALL_THROW( NullPointerException )
}

////////////////////////////////////////////////////////////////////////////////
std::string ShortBuffer::toString() const {

    std::ostringstream stream;

    stream << "ShortBuffer, status: "
           << "capacity =" << this->capacity()
           << " position =" << this->position()
           << " limit = " << this->limit();

    return stream.str();
}

////////////////////////////////////////////////////////////////////////////////
ShortBuffer& ShortBuffer::get( std::vector<short> buffer )
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
ShortBuffer& ShortBuffer::get( short* buffer, std::size_t offset, std::size_t length )
    throw( BufferUnderflowException,
           lang::exceptions::NullPointerException ) {

    try{

        if( length == 0 ) {
            return *this;
        }

        if( buffer == NULL ) {
            throw NullPointerException(
                __FILE__, __LINE__,
                "ShortBuffer::get - Passed Buffer is Null" );
        }

        if( length > remaining() ) {
            throw BufferUnderflowException(
                __FILE__, __LINE__,
                "ShortBuffer::get - Not enough data to fill length = %d", length );
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
ShortBuffer& ShortBuffer::put( ShortBuffer& src )
    throw( BufferOverflowException, ReadOnlyBufferException,
           lang::exceptions::IllegalArgumentException ) {

    try{

        if( this == &src ) {
            throw IllegalArgumentException(
                __FILE__, __LINE__,
                "ShortBuffer::put - Can't put Self" );
        }

        if( this->isReadOnly() ) {
            throw ReadOnlyBufferException(
                __FILE__, __LINE__,
                "ShortBuffer::put - This buffer is Read Only.");
        }

        if( src.remaining() > this->remaining() ) {
            throw BufferOverflowException(
                __FILE__, __LINE__,
                "ShortBuffer::put - Not enough space remaining to put src." );
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
ShortBuffer& ShortBuffer::put( const short* buffer, std::size_t offset, std::size_t length )
    throw( BufferOverflowException, ReadOnlyBufferException,
           lang::exceptions::NullPointerException ) {

    try{

        if( length == 0 ) {
            return *this;
        }

        if( this->isReadOnly() ) {
            throw ReadOnlyBufferException(
                __FILE__, __LINE__,
                "ShortBuffer::put - This buffer is Read Only.");
        }

        if( buffer == NULL ) {
            throw NullPointerException(
                __FILE__, __LINE__,
                "ShortBuffer::put - Passed Buffer is Null.");
        }

        if( length > this->remaining() ) {
            throw BufferOverflowException(
                __FILE__, __LINE__,
                "ShortBuffer::put - Not Enough space to store requested Data.");
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
ShortBuffer& ShortBuffer::put( std::vector<short>& buffer )
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
int ShortBuffer::compareTo( const ShortBuffer& value ) const {

    int compareRemaining = Math::min( (int)remaining(), (int)value.remaining() );

    std::size_t thisPos = this->position();
    std::size_t otherPos = value.position();
    short thisVal, otherVal;

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
bool ShortBuffer::equals( const ShortBuffer& value ) const {

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
bool ShortBuffer::operator==( const ShortBuffer& value ) const {
    return this->equals( value );
}

////////////////////////////////////////////////////////////////////////////////
bool ShortBuffer::operator<( const ShortBuffer& value ) const {
    return this->compareTo( value ) < 0 ? true : false;
}
