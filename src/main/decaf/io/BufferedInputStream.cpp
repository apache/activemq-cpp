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

#include "BufferedInputStream.h"
#include <algorithm>

using namespace std;
using namespace decaf;
using namespace decaf::io;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
BufferedInputStream::BufferedInputStream( InputStream* stream, bool own )
: FilterInputStream( stream, own ) {
    // Default to a 1k buffer.
    init( 1024 );
}

////////////////////////////////////////////////////////////////////////////////
BufferedInputStream::BufferedInputStream( InputStream* stream,
                                          std::size_t bufferSize,
                                          bool own )
    throw ( lang::exceptions::IllegalArgumentException )

: FilterInputStream( stream, own ) {

    try {
        this->init( bufferSize );
    }
    DECAF_CATCH_RETHROW( IllegalArgumentException )
    DECAF_CATCHALL_THROW( IllegalArgumentException )
}

////////////////////////////////////////////////////////////////////////////////
BufferedInputStream::~BufferedInputStream() {
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
void BufferedInputStream::init( std::size_t bufferSize ){

    if( bufferSize <= 0 ) {
        throw new IllegalArgumentException(
            __FILE__, __LINE__,
            "BufferedInputStream::init - Size must be greater than zero");
    }

    this->bufferSize = bufferSize;

    // Create the buffer and initialize the head and tail positions.
    this->buffer = new unsigned char[bufferSize];
    this->head = 0;
    this->tail = 0;
    this->markLimit = 0;
    this->markpos = -1;
}

////////////////////////////////////////////////////////////////////////////////
void BufferedInputStream::close() throw( IOException ) {

    // let parent close the inputStream
    FilterInputStream::close();
}

////////////////////////////////////////////////////////////////////////////////
unsigned char BufferedInputStream::read() throw ( IOException ){

    try{

        if( isClosed() ){
            throw IOException(
                __FILE__, __LINE__,
                "BufferedInputStream::bufferData - Stream is clsoed" );
        }

        // If there's no data left, reset to pointers to the beginning of the
        // buffer.
        normalizeBuffer();

        // If we don't have any data buffered yet - read as much as
        // we can.
        if( isEmpty() ){

            // If we hit EOF without getting any Data, then throw IOException
            if( bufferData() == -1 ){
                throw IOException(
                    __FILE__, __LINE__,
                    "BufferedInputStream::read - EOF has been Reached");
            }
        }

        // Get the next character.
        char returnValue = buffer[head++];

        return returnValue;
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
int BufferedInputStream::read( unsigned char* targetBuffer,
                               std::size_t offset,
                               std::size_t targetBufferSize )
    throw ( IOException, NullPointerException ){

    try{

        if( isClosed() ){
            throw IOException(
                __FILE__, __LINE__,
                "BufferedInputStream::bufferData - Stream is clsoed" );
        }

        // For zero, do nothing
        if( targetBufferSize == 0 ) {
            return 0;
        }

        if( targetBuffer == NULL ) {
            throw NullPointerException(
                __FILE__, __LINE__,
                "BufferedInputStream::read - Passed NULL for target Buffer");
        }

        // If there's no data left, reset to pointers to the beginning of the
        // buffer.
        normalizeBuffer();

        // If we still haven't filled the output buffer AND there is data
        // on the input stream to be read, read a buffer's worth from the stream.
        std::size_t totalRead = 0;
        while( totalRead < targetBufferSize ){

            // Get the remaining bytes to copy.
            std::size_t bytesToCopy = min( tail-head, (targetBufferSize-totalRead) );

            // Copy the data to the output buffer.
            memcpy( targetBuffer+totalRead+offset, this->buffer+head, bytesToCopy );

            // Increment the total bytes read.
            totalRead += bytesToCopy;

            // Increment the head position.
            head += bytesToCopy;

            // If the buffer is now empty, reset the positions to the
            // head of the buffer.
            normalizeBuffer();

            // If we still haven't satisified the request,
            // read more data.
            if( totalRead < targetBufferSize ){

                // Buffer as much data as we can, return EOF if we hit it.
                if( bufferData() == -1 ) {
                    return -1;
                }
            }
        }

        // Return the total number of bytes read.
        return (int)totalRead;
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
std::size_t BufferedInputStream::skip( std::size_t num )
    throw ( IOException, lang::exceptions::UnsupportedOperationException ){

    try{

        if( isClosed() ){
            throw IOException(
                __FILE__, __LINE__,
                "BufferedInputStream::skip - Stream is clsoed" );
        }

        // If there's no data left, reset to pointers to the beginning of the
        // buffer.
        normalizeBuffer();

        // loop until we've skipped the desired number of bytes
        std::size_t totalSkipped = 0;
        while( totalSkipped < num ){

            // Get the remaining bytes to copy.
            std::size_t bytesToSkip = min( tail-head, num-totalSkipped );

            // Increment the head position.
            head += bytesToSkip;
            totalSkipped += bytesToSkip;

            // If the buffer is now empty, reset the positions to the
            // head of the buffer.
            normalizeBuffer();

            // If we still haven't satisified the request,
            // read more data.
            if( totalSkipped < num ){

                // Buffer as much data as we can.
                bufferData();
            }
        }

        // Return the total number of bytes read.
        return totalSkipped;
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
int BufferedInputStream::bufferData() throw ( IOException ){

    try{
        if( getUnusedBytes() == 0 ){
            throw IOException(
                __FILE__, __LINE__,
                "BufferedInputStream::bufferData - buffer full" );
        }

        // Get the number of bytes currently available on the input stream
        // that could be read without blocking.
        std::size_t available = inputStream->available();

        // Calculate the number of bytes that we can read.  Always >= 1 byte!
        std::size_t bytesToRead = max( (std::size_t)1, min( available, getUnusedBytes() ) );

        // Read the bytes from the input stream.
        int bytesRead = inputStream->read( getTail(), 0, bytesToRead );
        if( bytesRead == 0 ){
            throw IOException(
                __FILE__, __LINE__,
                "BufferedInputStream::read() - failed reading bytes from stream");
        }

        // Dont add -1 to tail if we hit EOF
        if( bytesRead == -1 ) {
            return bytesRead;
        }

        // Increment the tail to the new end position.
        tail += bytesRead;

        return bytesRead;
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}
