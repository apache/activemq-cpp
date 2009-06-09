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

#include "BufferedOutputStream.h"
#include <algorithm>

using namespace std;
using namespace decaf;
using namespace decaf::io;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
BufferedOutputStream::BufferedOutputStream( OutputStream* stream, bool own )
: FilterOutputStream( stream, own ) {
    // Default to 1k buffer.
    init( 1024 );
}

////////////////////////////////////////////////////////////////////////////////
BufferedOutputStream::BufferedOutputStream( OutputStream* stream,
    std::size_t bufSize,
    bool own )
        throw ( lang::exceptions::IllegalArgumentException )

: FilterOutputStream( stream, own ) {

    try {
        this->init( bufSize );
    }
    DECAF_CATCH_RETHROW( IllegalArgumentException )
    DECAF_CATCHALL_THROW( IllegalArgumentException )
}

////////////////////////////////////////////////////////////////////////////////
BufferedOutputStream::~BufferedOutputStream() {
    try{
        this->close();

        // Destroy the buffer.
        if( buffer != NULL ){
            delete [] buffer;
            buffer = NULL;
        }
    }
    DECAF_CATCH_NOTHROW( IOException )
    DECAF_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void BufferedOutputStream::init( std::size_t bufSize ) {

    if( bufSize <= 0 ) {
        throw new IllegalArgumentException(
            __FILE__, __LINE__,
            "BufferedOutputStream::init - Size must be greater than zero");
    }

    this->bufferSize = bufSize;

    buffer = new unsigned char[bufSize];
    head = tail = 0;
}

////////////////////////////////////////////////////////////////////////////////
void BufferedOutputStream::close() throw( lang::Exception ){

    // let parent close the inputStream
    FilterOutputStream::close();
}

////////////////////////////////////////////////////////////////////////////////
void BufferedOutputStream::emptyBuffer() throw ( IOException ){

    if( outputStream == NULL ) {
        throw IOException(
            __FILE__, __LINE__,
            "BufferedOutputStream::emptyBuffer - OutputStream is closed" );
    }

    if( head != tail ){
        outputStream->write( buffer+head, 0, tail-head );
    }
    head = tail = 0;
}

////////////////////////////////////////////////////////////////////////////////
void BufferedOutputStream::flush() throw ( IOException ){

    try {

        if( isClosed() ){
            throw IOException(
                __FILE__, __LINE__,
                "BufferedOutputStream::write - Stream is clsoed" );
        }

        // Empty the contents of the buffer to the output stream.
        emptyBuffer();

        // Flush the output stream.
        outputStream->flush();
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void BufferedOutputStream::write( const unsigned char c ) throw ( IOException ){

    try{

        if( isClosed() ){
            throw IOException(
                __FILE__, __LINE__,
                "BufferedOutputStream::write - Stream is clsoed" );
        }

        if( tail >= bufferSize ){
            emptyBuffer();
        }

        buffer[tail++] = c;
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void BufferedOutputStream::write( const std::vector<unsigned char>& buffer )
    throw ( IOException ) {

    try{

        if( buffer.empty() ) {
            return;
        }

        if( isClosed() ){
            throw IOException(
                __FILE__, __LINE__,
                "BufferedOutputStream::write - Stream is clsoed" );
        }

        this->write( &buffer[0], 0, buffer.size() );
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void BufferedOutputStream::write( const unsigned char* buffer,
                                  std::size_t offset, std::size_t len )
    throw ( IOException, lang::exceptions::NullPointerException ) {

    try{

        // Fast exit.
        if( len == 0 ) {
            return;
        }

        if( isClosed() ){
            throw IOException(
                __FILE__, __LINE__,
                "BufferedOutputStream::write - Stream is clsoed" );
        }

        if( buffer == NULL ) {
            throw NullPointerException(
                __FILE__, __LINE__,
                "BufferedOutputStream::write - Buffer passed is Null.");
        }

        // Iterate until all the data is written.
        for( std::size_t pos=0; pos < len; ){

            if( tail >= bufferSize ){
                emptyBuffer();
            }

            // Get the number of bytes left to write.
            std::size_t bytesToWrite = min( (int)bufferSize-tail, len-pos );

            // Copy the data.
            memcpy( this->buffer+tail, buffer+offset+pos, bytesToWrite );

            // Increase the tail position.
            tail += bytesToWrite;

            // Decrease the number of bytes to write.
            pos += bytesToWrite;
        }
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCHALL_THROW( IOException )
}
