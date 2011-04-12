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

#include "Deflater.h"

#include <stdio.h>
#include <assert.h>

#include <decaf/internal/util/zip/zlib.h>
#include <decaf/lang/exceptions/RuntimeException.h>

using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::util;
using namespace decaf::util::zip;

////////////////////////////////////////////////////////////////////////////////
namespace decaf{
namespace util{
namespace zip{

    class DeflaterData {
    public:

        int level;
        bool nowrap;
        int strategy;
        int flush;
        bool finished;
        bool ended;

        z_stream* stream;

    private:

        DeflaterData( const DeflaterData& );
        DeflaterData& operator= ( const DeflaterData& );

    public:

        DeflaterData() : level(0),
                         nowrap(true),
                         strategy(0),
                         flush(0),
                         finished(false),
                         ended(false),
                         stream(NULL) {
        }

    public:

        static void initZLibDeflate( DeflaterData* handle, int level, bool nowrap = false ) {

            if( handle == NULL ) {
                throw NullPointerException(
                    __FILE__, __LINE__, "Error While initializing the Compression Library." );
            }

            handle->stream = new z_stream;
            handle->ended = false;
            handle->level = level;
            handle->strategy = 0;
            handle->nowrap = nowrap;
            handle->finished = false;
            handle->flush = Z_NO_FLUSH;

            // Init the ZLib stream to defaults
            handle->stream->zalloc = Z_NULL;
            handle->stream->zfree = Z_NULL;
            handle->stream->opaque = Z_NULL;
            handle->stream->avail_in = 0;
            handle->stream->next_in = Z_NULL;

            int result = Z_OK;
            if( nowrap == false ) {
                result = deflateInit( handle->stream, handle->level );
            } else {

                // Turn off ZLib header wrapping and encode raw.  Attempts
                // to set all other values to their normal defaults.
                result = deflateInit2( handle->stream,
                                       handle->level,
                                       Z_DEFLATED,
                                       -15, 8,
                                       Z_DEFAULT_STRATEGY );
            }

            if( result != Z_OK ) {
                throw RuntimeException(
                    __FILE__, __LINE__, "Error While initializing the Compression Library." );
            }
        }

        static void finishZlibDeflate( DeflaterData* handle ) {

            if( handle == NULL ) {
                throw NullPointerException(
                    __FILE__, __LINE__, "Error While initializing the Compression Library." );
            }

            handle->ended = true;

            if( handle->stream != NULL ) {

                // Shutdown the ZLib stream
                deflateEnd( handle->stream );
                delete handle->stream;
                handle->stream = NULL;
            }
        }

        static void resetZlibStream( DeflaterData* handle ) {

            if( handle == NULL ) {
                throw NullPointerException(
                    __FILE__, __LINE__, "Error While initializing the Compression Library." );
            }

            if( handle->stream != NULL ) {

                handle->finished = false;
                handle->flush = Z_NO_FLUSH;
                handle->ended = false;

                // Ask ZLib to do the reset.
                deflateReset( handle->stream );

                // Clear any old data that might still be around.
                handle->stream->opaque = Z_NULL;
                handle->stream->avail_in = 0;
                handle->stream->next_in = Z_NULL;
            }
        }

    };

}}}

////////////////////////////////////////////////////////////////////////////////
const int Deflater::BEST_COMPRESSION = 9;
const int Deflater::BEST_SPEED = 1;
const int Deflater::NO_COMPRESSION = 0;
const int Deflater::DEFAULT_COMPRESSION = -1;

const int Deflater::DEFAULT_STRATEGY = 0;
const int Deflater::DEFLATED = 0;
const int Deflater::FILTERED = 1;
const int Deflater::HUFFMAN_ONLY = 2;

////////////////////////////////////////////////////////////////////////////////
Deflater::Deflater( int level, bool nowrap ) : data( new DeflaterData() ) {

    if( level < DEFAULT_COMPRESSION || level > BEST_COMPRESSION ) {
        throw IllegalArgumentException(
            __FILE__, __LINE__, "Compression level passed was Invalid: %d", level );
    }

    // Initialize all the ZLib structures.
    DeflaterData::initZLibDeflate( this->data, level, nowrap );
}

////////////////////////////////////////////////////////////////////////////////
Deflater::Deflater() : data( new DeflaterData() ) {
    // Initialize all the ZLib structures.
    DeflaterData::initZLibDeflate( this->data, DEFAULT_COMPRESSION );
}

////////////////////////////////////////////////////////////////////////////////
Deflater::~Deflater() {
    try{
        this->end();
        delete this->data;
    }
    DECAF_CATCH_NOTHROW( Exception )
    DECAF_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void Deflater::setInput( const unsigned char* buffer, int size, int offset, int length ) {

    try{

        if( buffer == NULL ) {
            throw NullPointerException(
                __FILE__, __LINE__, "Buffer passed cannot be NULL." );
        }

        if( this->data->stream == NULL ) {
            throw IllegalStateException(
                __FILE__, __LINE__, "The Deflator has already been ended." );
        }

        if( offset + length > size ) {
            throw IndexOutOfBoundsException(
                __FILE__, __LINE__, "Given offset + length greater than the size of the buffer." );
        }


        // We can only change the level and strategy once an entire block of data is compressed.
        if( this->data->stream->next_in == NULL ) {
            deflateParams( this->data->stream, this->data->level, this->data->strategy );
        }

        this->data->stream->avail_in = (uInt)length;
        this->data->stream->next_in = (Bytef*)( buffer + offset );
    }
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCH_RETHROW( IllegalStateException )
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCHALL_THROW( IllegalStateException )
}

////////////////////////////////////////////////////////////////////////////////
void Deflater::setInput( const std::vector<unsigned char>& buffer, int offset, int length ) {

    this->setInput( &buffer[0], (int)buffer.size(), offset, length );
}

////////////////////////////////////////////////////////////////////////////////
void Deflater::setInput( const std::vector<unsigned char>& buffer ) {

    if( buffer.empty() ) {
        return;
    }

    this->setInput( &buffer[0], (int)buffer.size(), 0, (int)buffer.size() );
}

////////////////////////////////////////////////////////////////////////////////
void Deflater::setDictionary( const unsigned char* buffer, int size, int offset, int length ) {

    try{

        if( buffer == NULL ) {
            throw NullPointerException(
                __FILE__, __LINE__, "Buffer passed cannot be NULL." );
        }

        if( this->data->stream == NULL ) {
            throw IllegalStateException(
                __FILE__, __LINE__, "The Deflator has already been ended." );
        }

        if( offset + length > size ) {
            throw IndexOutOfBoundsException(
                __FILE__, __LINE__, "Given offset + length greater than the size of the buffer." );
        }

        // From the ZLib documentation.
        // deflateSetDictionary returns Z_OK if success, or Z_STREAM_ERROR if a parameter is invalid
        // (such as NULL dictionary) or the stream state is inconsistent (for example if deflate has
        // already been called for this stream or if the compression method is bsort).
        // deflateSetDictionary does not perform any compression: this will be done by deflate().
        if( deflateSetDictionary( this->data->stream, buffer + offset, (uInt)length ) != Z_OK ) {
            throw IllegalStateException(
                __FILE__, __LINE__, "Deflator could not accept the dictionary." );
        }
    }
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCH_RETHROW( IllegalStateException )
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCHALL_THROW( IllegalStateException )
}

////////////////////////////////////////////////////////////////////////////////
void Deflater::setDictionary( const std::vector<unsigned char>& buffer, int offset, int length ) {

    this->setDictionary( &buffer[0], (int)buffer.size(), offset, length );
}

////////////////////////////////////////////////////////////////////////////////
void Deflater::setDictionary( const std::vector<unsigned char>& buffer ) {

    if( buffer.empty() ) {
        return;
    }

    this->setDictionary( &buffer[0], (int)buffer.size(), 0, (int)buffer.size() );
}

////////////////////////////////////////////////////////////////////////////////
void Deflater::setStrategy( int strategy ) {

    if( strategy < DEFAULT_STRATEGY || strategy > HUFFMAN_ONLY ) {
        throw IllegalArgumentException(
            __FILE__, __LINE__, "Strategy value {%d} is not valid.", strategy );
    }

    if( this->data->stream == NULL && !this->data->ended ) {
        throw IllegalStateException(
            __FILE__, __LINE__, "The Deflator is in an invalid state." );
    }

    this->data->strategy = strategy;
}

////////////////////////////////////////////////////////////////////////////////
void Deflater::setLevel( int level ) {

    if( level < DEFAULT_COMPRESSION || level > BEST_COMPRESSION ) {
        throw IllegalArgumentException(
            __FILE__, __LINE__, "Strategy value {%d} is not valid.", level );
    }

    if( this->data->stream == NULL && !this->data->ended ) {
        throw IllegalStateException(
            __FILE__, __LINE__, "The Deflator is in an invalid state." );
    }

    this->data->level = level;
}

////////////////////////////////////////////////////////////////////////////////
bool Deflater::needsInput() const {
    if( this->data->stream == NULL ) {
        return false;
    }

    return this->data->stream->avail_in == 0;
}

////////////////////////////////////////////////////////////////////////////////
void Deflater::finish() {
    this->data->flush = Z_FINISH;
}

////////////////////////////////////////////////////////////////////////////////
bool Deflater::finished() const {
    return this->data->finished;
}

////////////////////////////////////////////////////////////////////////////////
int Deflater::deflate( unsigned char* buffer, int size, int offset, int length ) {

    try{

        if( buffer == NULL ) {
            throw NullPointerException(
                __FILE__, __LINE__, "Buffer passed cannot be NULL." );
        }

        if( this->data->stream == NULL ) {
            throw IllegalStateException(
                __FILE__, __LINE__, "The Deflator has already been ended." );
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

        unsigned long outStart = this->data->stream->total_out;

        this->data->stream->next_out = buffer + offset;
        this->data->stream->avail_out = (uInt)length;

        // Call ZLib and then process the resulting data to figure out what happened.
        int result = ::deflate( this->data->stream, this->data->flush );

        if( result == Z_STREAM_END ) {
            this->data->finished = true;
        }

        return (int)( this->data->stream->total_out - outStart );
    }
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCH_RETHROW( IllegalStateException )
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCHALL_THROW( IllegalStateException )
}

////////////////////////////////////////////////////////////////////////////////
int Deflater::deflate( std::vector<unsigned char>& buffer, int offset, int length ) {

    return this->deflate( &buffer[0], (int)buffer.size(), offset, length );
}

////////////////////////////////////////////////////////////////////////////////
int Deflater::deflate( std::vector<unsigned char>& buffer ) {

    return this->deflate( &buffer[0], (int)buffer.size(), 0, (int)buffer.size() );
}

////////////////////////////////////////////////////////////////////////////////
long long Deflater::getAdler() const {
    if( this->data->stream == NULL ) {
        throw IllegalStateException(
            __FILE__, __LINE__, "The Deflator has already been ended." );
    }

    return this->data->stream->adler;
}

////////////////////////////////////////////////////////////////////////////////
long long Deflater::getBytesRead() const {
    if( this->data->stream == NULL ) {
        throw IllegalStateException(
            __FILE__, __LINE__, "The Deflator has already been ended." );
    }

    return this->data->stream->total_in;
}

////////////////////////////////////////////////////////////////////////////////
long long Deflater::getBytesWritten() const {
    if( this->data->stream == NULL ) {
        throw IllegalStateException(
            __FILE__, __LINE__, "The Deflator has already been ended." );
    }

    return this->data->stream->total_out;
}

////////////////////////////////////////////////////////////////////////////////
void Deflater::reset() {

    if( this->data->stream == NULL ) {
        throw IllegalStateException(
            __FILE__, __LINE__, "The Deflator has already been ended." );
    }

    DeflaterData::resetZlibStream( this->data );
}

////////////////////////////////////////////////////////////////////////////////
void Deflater::end() {

    if( this->data ) {
        DeflaterData::finishZlibDeflate( this->data );
    }
}
