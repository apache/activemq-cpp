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

#include "CharBuffer.h"
#include <decaf/lang/Character.h>
#include <decaf/lang/Math.h>
#include "decaf/internal/nio/BufferFactory.h"
#include <memory>

using namespace std;
using namespace decaf;
using namespace decaf::nio;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::internal::nio;

////////////////////////////////////////////////////////////////////////////////
CharBuffer::CharBuffer( std::size_t capacity )
 :  Buffer( capacity ) {

}

////////////////////////////////////////////////////////////////////////////////
CharBuffer* CharBuffer::allocate( std::size_t capacity ) {

    try{

        return BufferFactory::createCharBuffer( capacity );
    }
    DECAF_CATCH_RETHROW( Exception )
    DECAF_CATCHALL_THROW( Exception )
}

////////////////////////////////////////////////////////////////////////////////
CharBuffer* CharBuffer::wrap( char* buffer, std::size_t offset, std::size_t length )
    throw( lang::exceptions::NullPointerException ) {

    try{

        if( buffer == NULL ) {
            throw NullPointerException(
                __FILE__, __LINE__,
                "CharBuffer::wrap - Passed Buffer is Null.");
        }

        return BufferFactory::createCharBuffer( buffer, offset, length );
    }
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, NullPointerException )
    DECAF_CATCHALL_THROW( NullPointerException )
}

////////////////////////////////////////////////////////////////////////////////
CharBuffer* CharBuffer::wrap( std::vector<char>& buffer ) {

    try{

        if( buffer.empty() ) {
            throw NullPointerException(
                __FILE__, __LINE__,
                "CharBuffer::wrap - Passed Buffer is Empty.");
        }

        return BufferFactory::createCharBuffer( &buffer[0], 0, buffer.size() );
    }
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, NullPointerException )
    DECAF_CATCHALL_THROW( NullPointerException )
}

////////////////////////////////////////////////////////////////////////////////
std::string CharBuffer::toString() const {

    std::string strbuf;

    for( std::size_t i = this->position(); i < this->limit(); i++ ) {
        strbuf.append( Character::valueOf( get( i ) ).toString() );
    }

    return strbuf;
}

////////////////////////////////////////////////////////////////////////////////
CharBuffer& CharBuffer::append( char value )
    throw ( BufferOverflowException, ReadOnlyBufferException ) {

    try{

        this->put( value );

        return *this;
    }
    DECAF_CATCH_RETHROW( BufferOverflowException )
    DECAF_CATCH_RETHROW( ReadOnlyBufferException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, BufferOverflowException )
    DECAF_CATCHALL_THROW( BufferOverflowException )
}

////////////////////////////////////////////////////////////////////////////////
CharBuffer& CharBuffer::append( const CharSequence* value )
    throw ( BufferOverflowException, ReadOnlyBufferException ) {

    try{

        if( value != NULL ) {
            return this->put( value->toString() );
        }

        return this->put( "null" );
    }
    DECAF_CATCH_RETHROW( BufferOverflowException )
    DECAF_CATCH_RETHROW( ReadOnlyBufferException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, BufferOverflowException )
    DECAF_CATCHALL_THROW( BufferOverflowException )
}

////////////////////////////////////////////////////////////////////////////////
CharBuffer& CharBuffer::append( const CharSequence* value, std::size_t start, std::size_t end )
    throw ( decaf::lang::exceptions::IndexOutOfBoundsException,
            BufferOverflowException, ReadOnlyBufferException ) {

    try{

        if( value != NULL ) {
            auto_ptr<CharSequence> temp( value->subSequence( start, end ) );
            this->append( temp.get() );

            return *this;
        }

        return this->put( "null", start, end );
    }
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCH_RETHROW( BufferOverflowException )
    DECAF_CATCH_RETHROW( ReadOnlyBufferException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, BufferOverflowException )
    DECAF_CATCHALL_THROW( BufferOverflowException )
}

////////////////////////////////////////////////////////////////////////////////
char CharBuffer::charAt( std::size_t index ) const
    throw( decaf::lang::exceptions::IndexOutOfBoundsException ) {

    try{

        return this->get( index );
    }
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IndexOutOfBoundsException )
    DECAF_CATCHALL_THROW( IndexOutOfBoundsException )
}

////////////////////////////////////////////////////////////////////////////////
CharBuffer& CharBuffer::get( std::vector<char> buffer )
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
CharBuffer& CharBuffer::get( char* buffer, std::size_t offset, std::size_t length )
    throw( BufferUnderflowException,
           lang::exceptions::NullPointerException ) {

    try{

        if( length == 0 ) {
            return *this;
        }

        if( buffer == NULL ) {
            throw NullPointerException(
                __FILE__, __LINE__,
                "CharBuffer::get - Passed Buffer is Null" );
        }

        if( length > remaining() ) {
            throw BufferUnderflowException(
                __FILE__, __LINE__,
                "CharBuffer::get - Not enough data to fill length = %d", length );
        }

        for( std::size_t ix = 0; ix < length; ++ix ){
            buffer[offset + ix] = this->get();
        }

        return *this;
    }
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCH_RETHROW( BufferUnderflowException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, BufferUnderflowException )
    DECAF_CATCHALL_THROW( BufferUnderflowException )
}

////////////////////////////////////////////////////////////////////////////////
CharBuffer& CharBuffer::put( CharBuffer& src )
    throw( BufferOverflowException, ReadOnlyBufferException,
           lang::exceptions::IllegalArgumentException ) {

    try{

        if( this == &src ) {
            throw IllegalArgumentException(
                __FILE__, __LINE__,
                "CharBuffer::put - Can't put Self" );
        }

        if( this->isReadOnly() ) {
            throw ReadOnlyBufferException(
                __FILE__, __LINE__,
                "CharBuffer::put - This buffer is Read Only.");
        }

        if( src.remaining() > this->remaining() ) {
            throw BufferOverflowException(
                __FILE__, __LINE__,
                "CharBuffer::put - Not enough space remaining to put src." );
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
CharBuffer& CharBuffer::put( const char* buffer, std::size_t offset, std::size_t length )
    throw( BufferOverflowException, ReadOnlyBufferException,
           lang::exceptions::NullPointerException ) {

    try{

        if( length == 0 ) {
            return *this;
        }

        if( this->isReadOnly() ) {
            throw ReadOnlyBufferException(
                __FILE__, __LINE__,
                "CharBuffer::put - This buffer is Read Only.");
        }

        if( buffer == NULL ) {
            throw NullPointerException(
                __FILE__, __LINE__,
                "CharBuffer::put - Passed Buffer is Null.");
        }

        if( length > this->remaining() ) {
            throw BufferOverflowException(
                __FILE__, __LINE__,
                "CharBuffer::put - Not Enough space to store requested Data.");
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
CharBuffer& CharBuffer::put( std::vector<char>& buffer )
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
CharBuffer& CharBuffer::put( const std::string& src, std::size_t start, std::size_t end )
    throw( BufferOverflowException, ReadOnlyBufferException,
           decaf::lang::exceptions::IndexOutOfBoundsException ) {

    try{

        if( ( start > end ) || ( src.size() < ( end - start ) ) ) {
            throw IndexOutOfBoundsException(
                __FILE__, __LINE__,
                "CharBuffer::put - invalid start and end pos; start = %d, end = %d",
                start, end );
        }

        if( start > src.size() || end > src.size() ) {
            throw IndexOutOfBoundsException(
                __FILE__, __LINE__,
                "CharBuffer::put - invalid start and end pos; start = %d, end = %d",
                start, end );
        }

        this->put( src.substr( start, end-start ) );

        return *this;
    }
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCH_RETHROW( BufferOverflowException )
    DECAF_CATCH_RETHROW( ReadOnlyBufferException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, BufferOverflowException )
    DECAF_CATCHALL_THROW( BufferOverflowException )
}

////////////////////////////////////////////////////////////////////////////////
CharBuffer& CharBuffer::put( const std::string& src )
    throw( BufferOverflowException, ReadOnlyBufferException ) {

    try{

        if( !src.empty() ) {
            this->put( src.c_str(), 0, src.size() );
        }

        return *this;
    }
    DECAF_CATCH_RETHROW( BufferOverflowException )
    DECAF_CATCH_RETHROW( ReadOnlyBufferException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, BufferOverflowException )
    DECAF_CATCHALL_THROW( BufferOverflowException )
}

////////////////////////////////////////////////////////////////////////////////
std::size_t CharBuffer::read( CharBuffer* target )
    throw ( decaf::lang::exceptions::NullPointerException,
            decaf::lang::exceptions::IllegalArgumentException,
            ReadOnlyBufferException ) {

    try{

        if( target == this ) {
            throw IllegalArgumentException(
                __FILE__, __LINE__,
                "CharBuffer::read - Cannot read to self" );
        }

        if( target == NULL ) {
            throw NullPointerException(
                __FILE__, __LINE__,
                "CharBuffer::read - Null CharBuffer Passed" );
        }

        if( this->remaining() == 0 ) {
            return target->remaining() == 0 ? 0 : string::npos;
        }

        std::size_t result = (std::size_t)Math::min(
                (int)target->remaining(),
                (int)this->remaining() );
        std::vector<char> chars( result, 0 );
        get( &chars[0], 0, result );
        target->put( &chars[0], 0, result );

        return result;
    }
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCH_RETHROW( IllegalArgumentException )
    DECAF_CATCH_RETHROW( ReadOnlyBufferException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IllegalArgumentException )
    DECAF_CATCHALL_THROW( IllegalArgumentException )
}

////////////////////////////////////////////////////////////////////////////////
int CharBuffer::compareTo( const CharBuffer& value ) const {

    int compareRemaining = Math::min( (int)remaining(), (int)value.remaining() );

    std::size_t thisPos = this->position();
    std::size_t otherPos = value.position();
    char thisByte, otherByte;

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
bool CharBuffer::equals( const CharBuffer& value ) const {

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
bool CharBuffer::operator==( const CharBuffer& value ) const {
    return this->equals( value );
}

////////////////////////////////////////////////////////////////////////////////
bool CharBuffer::operator<( const CharBuffer& value ) const {
    return this->compareTo( value ) < 0 ? true : false;
}
