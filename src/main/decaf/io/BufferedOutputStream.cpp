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

////////////////////////////////////////////////////////////////////////////////
BufferedOutputStream::BufferedOutputStream( OutputStream* stream, bool own )
: FilterOutputStream( stream, own )
{
    // Default to 1k buffer.
    init( 1024 );
}

////////////////////////////////////////////////////////////////////////////////
BufferedOutputStream::BufferedOutputStream( OutputStream* stream,
    std::size_t bufSize,
    bool own )
: FilterOutputStream( stream, own )
{
    init( bufSize );
}

////////////////////////////////////////////////////////////////////////////////
BufferedOutputStream::~BufferedOutputStream()
{
    // Destroy the buffer.
    if( buffer != NULL ){
        delete [] buffer;
        buffer = NULL;
    }
}

////////////////////////////////////////////////////////////////////////////////
void BufferedOutputStream::init( std::size_t bufSize ){

    this->bufferSize = bufSize;

    buffer = new unsigned char[bufSize];
    head = tail = 0;
}

////////////////////////////////////////////////////////////////////////////////
void BufferedOutputStream::close() throw( lang::Exception ){

    // Flush this stream.
    flush();

    // Close the delegate stream.
    outputStream->close();
}

////////////////////////////////////////////////////////////////////////////////
void BufferedOutputStream::emptyBuffer() throw ( IOException ){

    if( head != tail ){
        outputStream->write( buffer+head, tail-head );
    }
    head = tail = 0;
}

////////////////////////////////////////////////////////////////////////////////
void BufferedOutputStream::flush() throw ( IOException ){

    // Empty the contents of the buffer to the output stream.
    emptyBuffer();

    // Flush the output stream.
    outputStream->flush();
}

////////////////////////////////////////////////////////////////////////////////
void BufferedOutputStream::write( const unsigned char c ) throw ( IOException ){

    if( tail >= bufferSize ){
        emptyBuffer();
    }

    buffer[tail++] = c;
}

////////////////////////////////////////////////////////////////////////////////
void BufferedOutputStream::write( const unsigned char* buffer, std::size_t len )
    throw ( IOException )
{
    // Iterate until all the data is written.
    for( std::size_t pos=0; pos < len; ){

        if( tail >= bufferSize ){
            emptyBuffer();
        }

        // Get the number of bytes left to write.
        std::size_t bytesToWrite = min( (int)bufferSize-tail, len-pos );

        // Copy the data.
        memcpy( this->buffer+tail, buffer+pos, bytesToWrite );

        // Increase the tail position.
        tail += bytesToWrite;

        // Decrease the number of bytes to write.
        pos += bytesToWrite;
    }
}
