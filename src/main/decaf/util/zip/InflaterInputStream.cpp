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

#include "InflaterInputStream.h"

#include <decaf/lang/Math.h>
#include <decaf/io/EOFException.h>

using namespace decaf;
using namespace decaf::io;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::util;
using namespace decaf::util::zip;

////////////////////////////////////////////////////////////////////////////////
const int InflaterInputStream::DEFAULT_BUFFER_SIZE = 512;

////////////////////////////////////////////////////////////////////////////////
InflaterInputStream::InflaterInputStream( InputStream* inputStream, bool own ) :
    FilterInputStream( inputStream, own ),
    inflater(new Inflater()), buff(), length(0), ownInflater(true), atEOF(false) {

    this->buff.resize( DEFAULT_BUFFER_SIZE );
}

////////////////////////////////////////////////////////////////////////////////
InflaterInputStream::InflaterInputStream( InputStream* inputStream, Inflater* inflater, bool own, bool ownInflater )
 :  FilterInputStream( inputStream, own ),
    inflater(inflater), buff(), length(0), ownInflater(ownInflater), atEOF(false) {

    if( inflater == NULL ) {
        throw NullPointerException(
             __FILE__, __LINE__, "Inflater passed was NULL." );
    }

    this->buff.resize( DEFAULT_BUFFER_SIZE );
}

////////////////////////////////////////////////////////////////////////////////
InflaterInputStream::InflaterInputStream( InputStream* inputStream, Inflater* inflater,
                                          int bufferSize, bool own, bool ownInflater )
 :  FilterInputStream( inputStream, own ),
    inflater(inflater), buff(), length(0), ownInflater(ownInflater), atEOF(false) {

    if( inflater == NULL ) {
        throw NullPointerException(
             __FILE__, __LINE__, "Inflater passed was NULL." );
    }

    if( bufferSize <= 0 ) {
        throw IllegalArgumentException(
             __FILE__, __LINE__, "Cannot create a zero sized buffer." );
    }

    this->buff.resize( bufferSize );
}

////////////////////////////////////////////////////////////////////////////////
InflaterInputStream::~InflaterInputStream() {
    try{
        this->close();

        if( ownInflater ) {
            delete inflater;
        }
    }
    DECAF_CATCH_NOTHROW( Exception )
    DECAF_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
bool InflaterInputStream::markSupported() const {
    return false;
}

////////////////////////////////////////////////////////////////////////////////
void InflaterInputStream::reset() {
    throw IOException(
         __FILE__, __LINE__, "Not Supported for this class." );
}

////////////////////////////////////////////////////////////////////////////////
void InflaterInputStream::mark( int readLimit DECAF_UNUSED ) {
    // No-op
}

////////////////////////////////////////////////////////////////////////////////
long long InflaterInputStream::skip( long long num ) {

    try{

        if( num <= 0 ) {
            return 0;
        }

        long long count = 0;
        long long remaining = (std::size_t)Math::min( num, (long long)buff.size() );

        std::vector<unsigned char> buffer( (std::size_t)remaining );

        while( count < num ) {
            int x = read( &buffer[0], (int)buffer.size() , 0, (int)remaining );
            if( x == -1 ) {
                return count;
            }
            count += x;
            remaining = ( num - count ) < (long long)buffer.size() ? num - count : buffer.size();
        }

        return count;
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void InflaterInputStream::close() {

    try{

        if( !isClosed() ) {
            inflater->end();
            this->atEOF = true;
            FilterInputStream::close();
        }
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
int InflaterInputStream::available() const {

    try{

        if( isClosed() ) {
            throw IOException(
                __FILE__, __LINE__, "Stream already closed." );
        }

        if( atEOF ) {
            return 0;
        }

        return 1;
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
int InflaterInputStream::doReadByte() {

    try{

        unsigned char buffer[1];
        if( doReadArrayBounded( buffer, 1, 0, 1 ) < 0 ) {
            return -1;
        }

        return (int)buffer[0];
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
int InflaterInputStream::doReadArrayBounded( unsigned char* buffer, int size, int offset, int length ) {

    try{

        if( buffer == NULL ) {
            throw NullPointerException(
                __FILE__, __LINE__, "Buffer passed was NULL." );
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

        if( length == 0 ) {
            return 0;
        }

        if( isClosed() ) {
            throw IOException(
                __FILE__, __LINE__, "Stream already closed." );
        }

        if( atEOF ) {
            return -1;
        }

        do {

            if( inflater->needsInput() ) {
                this->fill();
            }

            // Invariant: if reading returns -1 or throws, eof must be true.
            // It may also be true if the next read() should return -1.
            try {

                int result = inflater->inflate( buffer, size, offset, length );

                atEOF = inflater->finished();

                if( result > 0 ) {
                    return result;
                } else if( atEOF ) {
                    return -1;
                } else if( inflater->needsDictionary() ) {
                    atEOF = true;
                    return -1;
                } else if( this->length == -1 ) {
                    atEOF = true;
                    throw EOFException(
                        __FILE__, __LINE__, "Reached end of Input." );
                }

            } catch( DataFormatException& e ) {

                atEOF = true;
                if( this->length == -1 ) {
                    throw EOFException(
                        __FILE__, __LINE__, "Reached end of Input." );
                }

                IOException ex( __FILE__, __LINE__, "Error from Inflater" );
                ex.initCause(e.clone());
                throw ex;
            }

        } while(true);
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void InflaterInputStream::fill() {

    try{

        if( isClosed() ) {
            throw IOException(
                __FILE__, __LINE__, "Stream already closed." );
        }

        // Try and fill the input buffer, whatever we get goes into the inflater.
        length = inputStream->read( &buff[0], (int)buff.size(), 0, (int)buff.size() );

        if( length > 0 ) {
            inflater->setInput( &buff[0], (int)buff.size(), 0, length );
        }
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}
