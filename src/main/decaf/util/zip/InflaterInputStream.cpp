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
const std::size_t InflaterInputStream::DEFAULT_BUFFER_SIZE = 512;

////////////////////////////////////////////////////////////////////////////////
InflaterInputStream::InflaterInputStream( InputStream* inputStream, bool own ) :
    FilterInputStream( inputStream, own ) {

    this->atEOF = false;
    this->ownInflater = true;
    this->inflater = new Inflater();
    this->buff.resize( DEFAULT_BUFFER_SIZE );
}

////////////////////////////////////////////////////////////////////////////////
InflaterInputStream::InflaterInputStream( InputStream* inputStream, Inflater* inflater, bool own )
 :  FilterInputStream( inputStream, own ) {

    if( inflater == NULL ) {
        throw NullPointerException(
             __FILE__, __LINE__, "Inflater passed was NULL." );
    }

    this->inflater = inflater;
    this->ownInflater = false;
    this->buff.resize( DEFAULT_BUFFER_SIZE );
    this->atEOF = false;
}

////////////////////////////////////////////////////////////////////////////////
InflaterInputStream::InflaterInputStream( InputStream* inputStream, Inflater* inflater,
                                          std::size_t bufferSize, bool own )
 :  FilterInputStream( inputStream, own ) {

    if( inflater == NULL ) {
        throw NullPointerException(
             __FILE__, __LINE__, "Inflater passed was NULL." );
    }

    if( bufferSize == 0 ) {
        throw IllegalArgumentException(
             __FILE__, __LINE__, "Cannot create a zero sized buffer." );
    }

    this->inflater = inflater;
    this->ownInflater = false;
    this->buff.resize( bufferSize );
    this->atEOF = false;
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
void InflaterInputStream::reset() throw ( decaf::io::IOException ) {
    throw IOException(
         __FILE__, __LINE__, "Not Supported for this class." );
}

////////////////////////////////////////////////////////////////////////////////
void InflaterInputStream::mark( int readLimit DECAF_UNUSED ) {
    // No-op
}

////////////////////////////////////////////////////////////////////////////////
std::size_t InflaterInputStream::skip( std::size_t num )
    throw ( decaf::io::IOException,
            decaf::lang::exceptions::UnsupportedOperationException ) {

    try{

        std::size_t count = 0;
        std::size_t remaining = Math::min( (long long)num, (long long)buff.size() );

        std::vector<unsigned char> buffer( remaining );

        while( count < num ) {
            int x = read( &buffer[0], buffer.size() , 0, remaining );
            if( x == -1 ) {
                return count;
            }
            count += x;
            remaining = ( num - count ) < buffer.size() ? num - count : buffer.size();
        }

        return count;
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void InflaterInputStream::close() throw ( decaf::io::IOException ) {

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
std::size_t InflaterInputStream::available() const throw ( decaf::io::IOException ) {

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
int InflaterInputStream::doReadByte() throw ( decaf::io::IOException ) {

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
int InflaterInputStream::doReadArrayBounded( unsigned char* buffer, std::size_t size,
                                             std::size_t offset, std::size_t length )
    throw ( decaf::io::IOException,
            decaf::lang::exceptions::IndexOutOfBoundsException,
            decaf::lang::exceptions::NullPointerException ) {

    try{

        if( buffer == NULL ) {
            throw NullPointerException(
                __FILE__, __LINE__, "Buffer passed was NULL." );
        }

        if( offset + length > size ) {
            throw IndexOutOfBoundsException(
                __FILE__, __LINE__, "Offset plus Length exceeds Buffer Size." );
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

                std::size_t result = inflater->inflate( buffer, size, offset, length );

                atEOF = inflater->finished();

                if( result > 0 ) {
                    return (int)result;
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
                ex.initCause( e.clone() );
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
void InflaterInputStream::fill() throw ( decaf::io::IOException ) {

    try{

        if( isClosed() ) {
            throw IOException(
                __FILE__, __LINE__, "Stream already closed." );
        }

        // Try and fill the input buffer, whatever we get goes into the inflater.
        length = inputStream->read( &buff[0], buff.size(), 0, buff.size() );

        if( length > 0 ) {
            inflater->setInput( &buff[0], buff.size(), 0, length );
        }
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}
