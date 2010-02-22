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

#include <decaf/lang/System.h>
#include <decaf/lang/Math.h>

using namespace std;
using namespace decaf;
using namespace decaf::io;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
BufferedOutputStream::BufferedOutputStream( OutputStream* stream, bool own )
: FilterOutputStream( stream, own ) {

    // Default to 1k buffer.
    init( 8192 );
}

////////////////////////////////////////////////////////////////////////////////
BufferedOutputStream::BufferedOutputStream( OutputStream* stream, std::size_t bufSize, bool own )
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

    this->bufferSize = bufSize;

    buffer = new unsigned char[bufSize];
    head = tail = 0;
}

////////////////////////////////////////////////////////////////////////////////
void BufferedOutputStream::emptyBuffer() throw ( IOException ){

    if( this->outputStream == NULL ) {
        throw IOException(
            __FILE__, __LINE__,
            "BufferedOutputStream::emptyBuffer - OutputStream is closed" );
    }

    if( this->head != this->tail ){
        this->outputStream->write( this->buffer + this->head, this->tail -this->head );
    }
    this->head = this->tail = 0;
}

////////////////////////////////////////////////////////////////////////////////
void BufferedOutputStream::flush() throw ( decaf::io::IOException ) {

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
void BufferedOutputStream::doWriteByte( const unsigned char c )
    throw ( decaf::io::IOException ) {

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
void BufferedOutputStream::doWriteArray( const unsigned char* buffer, std::size_t size )
    throw ( decaf::io::IOException ) {

    try{

        if( size == 0 ) {
            return;
        }

        if( isClosed() ){
            throw IOException(
                __FILE__, __LINE__,
                "BufferedOutputStream::write - Stream is clsoed" );
        }

        this->doWriteArrayBounded( buffer, size, 0, size );
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void BufferedOutputStream::doWriteArrayBounded( const unsigned char* buffer, std::size_t size,
                                                std::size_t offset, std::size_t length )
    throw ( decaf::io::IOException,
            decaf::lang::exceptions::NullPointerException,
            decaf::lang::exceptions::IndexOutOfBoundsException ) {

    try{

        if( length == 0 ) {
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

        if( ( offset + length ) > size ) {
            throw decaf::lang::exceptions::IndexOutOfBoundsException(
                __FILE__, __LINE__,
                "DataOutputStream::write - given offset + length is greater than buffer size.");
        }

        // Iterate until all the data is written.
        for( std::size_t pos=0; pos < length; ){

            if( tail >= bufferSize ){
                emptyBuffer();
            }

            // Get the number of bytes left to write.
            std::size_t bytesToWrite = Math::min( (long long)( bufferSize - tail ),
                                                  (long long)( length - pos ) );

            // Copy the data.
            //memcpy( this->buffer+tail, buffer+offset+pos, bytesToWrite );

            System::arraycopy( buffer, offset + pos, this->buffer, this->tail, bytesToWrite );

            // Increase the tail position.
            tail += bytesToWrite;

            // Decrease the number of bytes to write.
            pos += bytesToWrite;
        }
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCHALL_THROW( IOException )
}
