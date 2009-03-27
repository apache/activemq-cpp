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
ByteArrayInputStream::ByteArrayInputStream( const unsigned char* buffer,
                                            std::size_t bufferSize ){
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
void ByteArrayInputStream::setByteArray( const unsigned char* buffer,
                                         std::size_t bufferSize ) {

    // We're using the default buffer.
    this->activeBuffer = &this->defaultBuffer;

    // Remove old data
    this->defaultBuffer.clear();
    this->defaultBuffer.reserve( bufferSize );

    // Copy data to internal buffer.
    std::back_insert_iterator< std::vector<unsigned char> > iter( this->defaultBuffer );
    std::copy( buffer, buffer + bufferSize, iter );

    // Start the stream off at the beginning marking begin as the reset point.
    this->markpos = this->activeBuffer->begin();
    this->pos = this->markpos;
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayInputStream::reset() throw ( IOException){
    if( activeBuffer == NULL ){
        throw IOException( __FILE__, __LINE__, "Buffer has not been initialized" );
    }

    // Begin at the Beginning if mark hasn't been called otherwise it
    // starts at the marked pos.
    pos = this->markpos;
}

////////////////////////////////////////////////////////////////////////////////
unsigned char ByteArrayInputStream::read() throw ( IOException ){

    if( activeBuffer == NULL ){
        throw IOException(
            __FILE__, __LINE__,
            "ByteArrayInputStream::read - Buffer has not been initialized" );
    }

    if( pos == activeBuffer->end() ){
        throw IOException(
            __FILE__, __LINE__,
            "ByteArrayInputStream::read - Buffer is empty" );
    }

    return *(pos++);
}

////////////////////////////////////////////////////////////////////////////////
int ByteArrayInputStream::read( unsigned char* buffer,
                                std::size_t offset,
                                std::size_t bufferSize )
    throw ( IOException, lang::exceptions::NullPointerException ){

    try{

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

        std::size_t ix = 0;

        if( pos == activeBuffer->end() ) {
            return -1;
        }

        // How far are we from end
        std::size_t remaining = (std::size_t)distance( pos, activeBuffer->end() );

        // We only read as much as is left if the amount remaining is less than
        // the amount of data asked for.
        bufferSize = remaining < bufferSize ? remaining : bufferSize;

        for( ; ix < bufferSize; ++ix, ++pos) {
            buffer[ix + offset] = *(pos);
        }

        return (int)ix;
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
std::size_t ByteArrayInputStream::skip( std::size_t num )
    throw ( IOException, lang::exceptions::UnsupportedOperationException ){

    if( activeBuffer == NULL ){
        throw IOException(
            __FILE__, __LINE__,
            "ByteArrayInputStream::skip - Buffer has not been initialized" );
    }

    std::size_t ix = 0;

    // Increment the position until we've skipped the desired number
    // or we've hit the end of the buffer.
    for( ; ix < num && pos != activeBuffer->end(); ++ix, ++pos) {}

    return ix;
}
