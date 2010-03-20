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

#include "ByteArrayInputStream.h"
#include <algorithm>

using namespace std;
using namespace decaf;
using namespace decaf::io;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
ByteArrayInputStream::ByteArrayInputStream(){
    this->activeBuffer = NULL;
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayInputStream::ByteArrayInputStream( const vector<unsigned char>& buffer ){
    setBuffer(buffer);
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayInputStream::ByteArrayInputStream( const unsigned char* buffer, int bufferSize )
    throw( decaf::lang::exceptions::NullPointerException,
           decaf::lang::exceptions::IllegalArgumentException ) {

    setByteArray( buffer, bufferSize );
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayInputStream::~ByteArrayInputStream(){
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayInputStream::setBuffer( const vector<unsigned char>& buffer ){

    // We're using the default buffer.
    this->activeBuffer = &buffer;

    // Start the stream off at the beginning marking begin as the reset point.
    this->markpos = this->activeBuffer->begin();
    this->pos = this->markpos;
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayInputStream::setByteArray( const unsigned char* buffer, int bufferSize )
    throw( decaf::lang::exceptions::NullPointerException,
           decaf::lang::exceptions::IllegalArgumentException ) {

    if( buffer == NULL ) {
        throw NullPointerException(
            __FILE__, __LINE__, "Input Buffer cannot be NULL." );
    }

    if( bufferSize < 0 ) {
        throw IllegalArgumentException(
            __FILE__, __LINE__, "Size given for input buffer was negative." );
    }

    // We're using the default buffer.
    this->activeBuffer = &this->defaultBuffer;

    // Remove old data
    this->defaultBuffer.clear();
    this->defaultBuffer.reserve( bufferSize );

    // Copy data to internal buffer.
    this->defaultBuffer.insert( this->defaultBuffer.begin(), buffer, buffer + bufferSize );

    // Start the stream off at the beginning marking begin as the reset point.
    this->markpos = this->activeBuffer->begin();
    this->pos = this->markpos;
}

////////////////////////////////////////////////////////////////////////////////
int ByteArrayInputStream::available() const throw ( IOException ) {

    if( activeBuffer == NULL ){
        throw IOException(
            __FILE__, __LINE__,
            "buffer has not been initialized");
    }

    return (int)std::distance( pos, activeBuffer->end() );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayInputStream::mark( int readLimit DECAF_UNUSED ) {

    // the reset point is now the marked position until a new byte buffer
    // is set on this stream.
    this->markpos = pos;
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayInputStream::reset() throw ( IOException ){

    try{

        if( activeBuffer == NULL ){
            throw IOException( __FILE__, __LINE__, "Buffer has not been initialized" );
        }

        // Begin at the Beginning if mark hasn't been called otherwise it
        // starts at the marked pos.
        pos = this->markpos;
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
int ByteArrayInputStream::doReadByte() throw ( IOException ){

    try{

        if( activeBuffer == NULL ){
            throw IOException(
                __FILE__, __LINE__,
                "ByteArrayInputStream::read - Buffer has not been initialized" );
        }

        if( pos == activeBuffer->end() ){
            return -1;
        }

        return *(pos++);
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
int ByteArrayInputStream::doReadArrayBounded( unsigned char* buffer, int size,
                                              int offset, int length )
    throw ( decaf::io::IOException,
            decaf::lang::exceptions::IndexOutOfBoundsException,
            decaf::lang::exceptions::NullPointerException ) {

    try{

        if( length == 0 ) {
            return 0;
        }

        if( activeBuffer == NULL ){
            throw IOException(
                __FILE__, __LINE__,
                "ByteArrayInputStream::read - Buffer has not been initialized" );
        }

        if( buffer == NULL ) {
            throw NullPointerException(
                __FILE__, __LINE__,
                "ByteArrayInputStream::read - Buffer passed is Null" );
        }

        if( size < 0 ) {
            throw IndexOutOfBoundsException(
                __FILE__, __LINE__, "size parameter out of Bounds: %d.", size );
        }

        if( offset > size || offset < 0 ) {
            throw IndexOutOfBoundsException(
                __FILE__, __LINE__, "offset parameter out of Bounds: %d.", offset );
        }

        if( length < 0 || length > size - offset ) {
            throw IndexOutOfBoundsException(
                __FILE__, __LINE__, "length parameter out of Bounds: %d.", length );
        }

        int ix = 0;

        if( pos == activeBuffer->end() ) {
            return -1;
        }

        // How far are we from end
        int remaining = (int)distance( pos, activeBuffer->end() );

        // We only read as much as is left if the amount remaining is less than
        // the amount of data asked for.
        length = remaining < length ? remaining : length;

        for( ; ix < length; ++ix, ++pos) {
            buffer[ix + offset] = *(pos);
        }

        return ix;
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
long long ByteArrayInputStream::skip( long long num )
    throw ( IOException, lang::exceptions::UnsupportedOperationException ){

    try{

        if( activeBuffer == NULL ){
            throw IOException(
                __FILE__, __LINE__,
                "ByteArrayInputStream::skip - Buffer has not been initialized" );
        }

        long long ix = 0;

        // Increment the position until we've skipped the desired number
        // or we've hit the end of the buffer.
        for( ; ix < num && pos != activeBuffer->end(); ++ix, ++pos) {}

        return ix;
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}
