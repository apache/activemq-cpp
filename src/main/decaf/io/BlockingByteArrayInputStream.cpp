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

#include "BlockingByteArrayInputStream.h"
#include <algorithm>

using namespace std;
using namespace decaf;
using namespace decaf::io;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
BlockingByteArrayInputStream::BlockingByteArrayInputStream(){
    pos = buffer.end();
    closing = false;
}

////////////////////////////////////////////////////////////////////////////////
BlockingByteArrayInputStream::BlockingByteArrayInputStream(
    const unsigned char* buffer,
    std::size_t bufferSize ){

    closing = false;
    setByteArray( buffer, bufferSize );
}

////////////////////////////////////////////////////////////////////////////////
BlockingByteArrayInputStream::~BlockingByteArrayInputStream(){
}

////////////////////////////////////////////////////////////////////////////////
void BlockingByteArrayInputStream::setByteArray( const unsigned char* lbuffer,
                                                 std::size_t lbufferSize ){
    synchronized( this ){

        // Remove old data
        buffer.clear();
        buffer.reserve( lbufferSize );

        // Copy data to internal buffer.
        std::back_insert_iterator< std::vector<unsigned char> > iter( buffer );
        std::copy( lbuffer, lbuffer + lbufferSize, iter );

        // Begin at the Beginning.
        pos = this->buffer.begin();

        // Notify any listening threds that there is now data available.
        notifyAll();
    }
}

////////////////////////////////////////////////////////////////////////////////
void BlockingByteArrayInputStream::close() throw ( lang::Exception ){

    synchronized( this ){

        // Indicate that we're shutting down.
        closing = true;

        // Clear out the buffer.
        buffer.clear();

        // Notify that this stream is shutting down.
        notifyAll();
    }
}

////////////////////////////////////////////////////////////////////////////////
unsigned char BlockingByteArrayInputStream::read() throw ( IOException ){

    try{
        synchronized( this ){

            while( !closing ){

                if( pos != buffer.end() ){
                    return *(pos++);
                }

                // Wait for more data
                wait();
            }

            throw IOException( __FILE__, __LINE__, "close occurred during read" );
        }

        return 0;
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
int BlockingByteArrayInputStream::read( unsigned char* buffer,
                                        std::size_t offset,
                                        std::size_t bufferSize )
    throw ( IOException, lang::exceptions::NullPointerException ){

    if( bufferSize == 0 ) {
        return 0;
    }

    if( buffer == NULL ) {
        throw NullPointerException(
            __FILE__, __LINE__,
            "BlockingByteArrayInputStream::read - Passed buffer is Null" );
    }

    synchronized( this ){

        std::size_t ix = 0;

        for( ; ix < bufferSize && !closing; ++ix ) {

            if( pos == this->buffer.end() ) {
                // Wait for more data to come in.
                wait();
            }

            if( !closing && pos != this->buffer.end() ){
                buffer[ix + offset] = *(pos);
                ++pos;
            }
        }

        if( closing ){
            throw IOException(
                __FILE__, __LINE__,
                "BlockingByteArrayInputStream::read - close occurred during read" );
        }

        return (int)ix;
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
std::size_t BlockingByteArrayInputStream::skip( std::size_t num )
    throw ( io::IOException, lang::exceptions::UnsupportedOperationException ){

    std::size_t ix = 0;

    synchronized( this ){

        // Increment the pos until we'v skipped the desired num
        // or we've hit the end of the buffer.
        for( ; ix < num && !closing && pos != buffer.end(); ++ix, ++pos) {}
    }

    return ix;
}


