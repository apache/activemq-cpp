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

#include "ByteBuffer.h"
#include "decaf/internal/nio/BufferFactory.h"
#include "decaf/lang/Short.h"
#include "decaf/lang/Integer.h"
#include "decaf/lang/Long.h"
#include "decaf/lang/Float.h"
#include "decaf/lang/Double.h"

using namespace std;
using namespace decaf;
using namespace decaf::nio;
using namespace decaf::internal::nio;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
ByteBuffer::ByteBuffer( std::size_t capacity ) : Buffer( capacity ) {
}

////////////////////////////////////////////////////////////////////////////////
ByteBuffer* ByteBuffer::allocate( std::size_t capacity ) {

    try{

        return BufferFactory::createByteBuffer( capacity );
    }
    DECAF_CATCH_RETHROW( Exception )
    DECAF_CATCHALL_THROW( Exception )
}

////////////////////////////////////////////////////////////////////////////////
ByteBuffer* ByteBuffer::wrap( unsigned char* buffer, std::size_t offset, std::size_t length )
    throw( lang::exceptions::NullPointerException ) {

    try{

        if( buffer == NULL ) {
            throw NullPointerException(
                __FILE__, __LINE__,
                "ByteBuffer::wrap - Passed Buffer is Null.");
        }

        return BufferFactory::createByteBuffer( buffer, offset, length );
    }
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, NullPointerException )
    DECAF_CATCHALL_THROW( NullPointerException )
}

////////////////////////////////////////////////////////////////////////////////
ByteBuffer* ByteBuffer::wrap( std::vector<unsigned char>& buffer ) {

    try{

        if( buffer.empty() ) {
            throw NullPointerException(
                __FILE__, __LINE__,
                "ByteBuffer::wrap - Passed Buffer is Empty.");
        }

        return BufferFactory::createByteBuffer( &buffer[0], 0, buffer.size() );
    }
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, NullPointerException )
    DECAF_CATCHALL_THROW( NullPointerException )
}

////////////////////////////////////////////////////////////////////////////////
ByteBuffer& ByteBuffer::get( std::vector<unsigned char> buffer )
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
ByteBuffer& ByteBuffer::get( unsigned char* buffer,
                             std::size_t offset,
                             std::size_t length )
    throw( BufferUnderflowException, NullPointerException ) {

    try{

        if( length == 0 ) {
            return *this;
        }

        if( buffer == NULL ) {
            throw NullPointerException(
                __FILE__, __LINE__,
                "ByteBuffer::get - Passed Buffer is Null.");
        }

        if( length > this->remaining() ) {
            throw BufferUnderflowException(
                __FILE__, __LINE__,
                "ByteBuffer::get - Not Enough Data to Fill Request.");
        }

        // read length bytes starting from the offset
        for( std::size_t ix = 0; ix < length; ++ix ) {
            buffer[ix + offset] = this->get();
        }

        return *this;
    }
    DECAF_CATCH_RETHROW( BufferUnderflowException )
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, BufferUnderflowException )
    DECAF_CATCHALL_THROW( BufferUnderflowException )
}

////////////////////////////////////////////////////////////////////////////////
ByteBuffer& ByteBuffer::put( ByteBuffer& src )
    throw( BufferOverflowException, ReadOnlyBufferException, IllegalArgumentException ) {

    try{

        if( this == &src ) {
            throw IllegalArgumentException(
                __FILE__, __LINE__,
                "ByteBuffer::put - Can't put Self" );
        }

        if( this->isReadOnly() ) {
            throw ReadOnlyBufferException(
                __FILE__, __LINE__,
                "ByteBuffer::put - This buffer is Read Only.");
        }

        if( src.remaining() > this->remaining() ) {
            throw BufferOverflowException(
                __FILE__, __LINE__,
                "ByteBuffer::put - Not enough space remaining to put src." );
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
ByteBuffer& ByteBuffer::put( const unsigned char* buffer,
                             std::size_t offset,
                             std::size_t length )
    throw( BufferOverflowException, ReadOnlyBufferException, NullPointerException ) {

    try{

        if( length == 0 ) {
            return *this;
        }

        if( this->isReadOnly() ) {
            throw ReadOnlyBufferException(
                __FILE__, __LINE__,
                "ByteBuffer::put - This buffer is Read Only.");
        }

        if( buffer == NULL ) {
            throw NullPointerException(
                __FILE__, __LINE__,
                "ByteBuffer::put - Passed Buffer is Null.");
        }

        if( length > this->remaining() ) {
            throw BufferOverflowException(
                __FILE__, __LINE__,
                "ByteBuffer::put - Not Enough space to store requested Data.");
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
ByteBuffer& ByteBuffer::put( std::vector<unsigned char>& buffer )
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
int ByteBuffer::compareTo( const ByteBuffer& value ) const {

    try{

        std::size_t compareRemaining =
            ( remaining() < value.remaining() ) ?
                remaining() : value.remaining();

        std::size_t thisPos = this->position();
        std::size_t otherPos = value.position();

        unsigned char thisByte, otherByte = 0;

        while( compareRemaining > 0 ) {
            thisByte = this->get( thisPos );
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
    DECAF_CATCH_RETHROW( Exception )
    DECAF_CATCHALL_THROW( Exception )
}

////////////////////////////////////////////////////////////////////////////////
bool ByteBuffer::equals( const ByteBuffer& value ) const {

    try{

        if( this->remaining() != value.remaining()) {
            return false;
        }

        std::size_t myPosition = this->position();
        std::size_t otherPosition = value.position();

        while( myPosition < this->limit() ) {
            if( get( myPosition++ ) != value.get( otherPosition++ ) ) {
                return false;
            }
        }

        return true;
    }
    DECAF_CATCH_RETHROW( Exception )
    DECAF_CATCHALL_THROW( Exception )
}

////////////////////////////////////////////////////////////////////////////////
bool ByteBuffer::operator==( const ByteBuffer& value ) const {
    return this->equals( value );
}

////////////////////////////////////////////////////////////////////////////////
bool ByteBuffer::operator<( const ByteBuffer& value ) const {
    return this->compareTo( value ) == -1;
}

////////////////////////////////////////////////////////////////////////////////
std::string ByteBuffer::toString() const {

    std::ostringstream stream;
    stream << "ByteBuffer";
    stream << ", status: capacity=" << this->capacity();
    stream << " position=" << this->position();
    stream << " limit=" << this->limit();
    return stream.str();
}
