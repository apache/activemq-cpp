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

#include "Inflater.h"

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

    class InflaterData {
    public:

        bool nowrap;
        bool finished;
        bool needDictionary;
        int flush;

        z_stream* stream;

    private:

        InflaterData( const InflaterData& );
        InflaterData& operator= ( const InflaterData& );

    public:

        InflaterData() : nowrap(true), finished(false), needDictionary(false), flush(0), stream(NULL) {}

    public:

        static void initZlibInflate( InflaterData* handle, bool nowrap = false ) {

            if( handle == NULL ) {
                throw NullPointerException(
                    __FILE__, __LINE__, "Error While initializing the Decompression Library." );
            }

            handle->stream = new z_stream;
            handle->finished = false;
            handle->needDictionary = false;
            handle->flush = Z_NO_FLUSH;
            handle->nowrap = nowrap;

            // Init the ZLib stream to defaults
            handle->stream->zalloc = Z_NULL;
            handle->stream->zfree = Z_NULL;
            handle->stream->opaque = Z_NULL;
            handle->stream->avail_in = 0;
            handle->stream->next_in = Z_NULL;

            int result = Z_OK;
            if( nowrap == false ) {
                result = inflateInit( handle->stream );
            } else {
                // Disable the ZLib header.
                result = inflateInit2( handle->stream, -15 );
            }

            if( result != Z_OK ) {
                throw RuntimeException(
                    __FILE__, __LINE__, "Error While initializing the Decompression Library." );
            }
        }

        static void finishZlibDeflate( InflaterData* handle ) {

            if( handle == NULL ) {
                throw NullPointerException(
                    __FILE__, __LINE__, "Error While initializing the Decompression Library." );
            }

            if( handle->stream != NULL ) {

                // Shutdown the ZLib stream
                inflateEnd( handle->stream );
                delete handle->stream;
                handle->stream = NULL;
            }
        }

        static void resetZlibStream( InflaterData* handle ) {

            if( handle == NULL ) {
                throw NullPointerException(
                    __FILE__, __LINE__, "Error While initializing the Decompression Library." );
            }

            if( handle->stream != NULL ) {

                handle->finished = false;
                handle->needDictionary = false;
                handle->flush = Z_NO_FLUSH;

                // Ask ZLib to do the reset.
                inflateReset( handle->stream );

                // clear any old data
                handle->stream->opaque = Z_NULL;
                handle->stream->avail_in = 0;
                handle->stream->next_in = Z_NULL;
            }
        }
    };

}}}

////////////////////////////////////////////////////////////////////////////////
Inflater::Inflater( bool nowrap ) : data( new InflaterData() ) {
    InflaterData::initZlibInflate( this->data, nowrap );
}

////////////////////////////////////////////////////////////////////////////////
Inflater::Inflater() : data( new InflaterData() ) {
    InflaterData::initZlibInflate( this->data );
}

////////////////////////////////////////////////////////////////////////////////
Inflater::~Inflater() {
    try{
        this->end();
        delete data;
    }
    DECAF_CATCH_NOTHROW( Exception )
    DECAF_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void Inflater::setInput( const unsigned char* buffer, int size, int offset, int length ) {

    try{

        if( buffer == NULL ) {
            throw NullPointerException(
                __FILE__, __LINE__, "Passed Buffer was NULL." );
        }

        if( this->data->stream == NULL ) {
            throw IllegalStateException(
                __FILE__, __LINE__, "The Inflater end method has already been called." );
        }

        if( offset + length > size ) {
            throw IndexOutOfBoundsException(
                __FILE__, __LINE__, "The offset + length given is greater than the specified buffer size." );
        }

        this->data->stream->avail_in = (uInt)length;
        this->data->stream->next_in = (Bytef*)( buffer + offset );
    }
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCH_RETHROW( IllegalStateException )
    DECAF_CATCHALL_THROW( IllegalStateException )
}

////////////////////////////////////////////////////////////////////////////////
void Inflater::setInput( const std::vector<unsigned char>& buffer, int offset, int length ) {

    if( buffer.empty() ) {
        return;
    }

    this->setInput( &buffer[0], (int)buffer.size(), offset, length );
}

////////////////////////////////////////////////////////////////////////////////
void Inflater::setInput( const std::vector<unsigned char>& buffer ) {

    if( buffer.empty() ) {
        return;
    }

    this->setInput( &buffer[0], (int)buffer.size(), 0, (int)buffer.size() );
}

////////////////////////////////////////////////////////////////////////////////
int Inflater::getRemaining() const {

    if( this->data->stream != NULL ) {
        return this->data->stream->avail_in;
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
void Inflater::setDictionary( const unsigned char* buffer, int size, int offset, int length ) {

    try{

        if( buffer == NULL ) {
            throw NullPointerException(
                __FILE__, __LINE__, "Passed Buffer was NULL." );
        }

        if( this->data->stream == NULL ) {
            throw IllegalStateException(
                __FILE__, __LINE__, "The Inflater end method has already been called." );
        }

        if( offset + length > size ) {
            throw IndexOutOfBoundsException(
                __FILE__, __LINE__, "The offset + length given is greater than the specified buffer size." );
        }

        // From the ZLib documentation
        // inflateSetDictionary returns Z_OK if success, Z_STREAM_ERROR if a parameter is invalid (such
        // as NULL dictionary) or the stream state is inconsistent, Z_DATA_ERROR if the given dictionary
        // doesn't match the expected one (incorrect adler32 value). inflateSetDictionary does not
        // perform any decompression: this will be done by subsequent calls of inflate().
        int result = inflateSetDictionary( this->data->stream, buffer + offset, (uInt)length );
        if( result != Z_OK ) {
            throw IllegalArgumentException(
                __FILE__, __LINE__, "Dictionary given does not match required checksum value." );
        }
    }
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCH_RETHROW( IllegalStateException )
    DECAF_CATCH_RETHROW( IllegalArgumentException )
    DECAF_CATCHALL_THROW( IllegalStateException )
}

////////////////////////////////////////////////////////////////////////////////
void Inflater::setDictionary( const std::vector<unsigned char>& buffer, int offset, int length ) {

    if( buffer.empty() ) {
        return;
    }

    this->setDictionary( &buffer[0], (int)buffer.size(), offset, length );
}

////////////////////////////////////////////////////////////////////////////////
void Inflater::setDictionary( const std::vector<unsigned char>& buffer ) {

    if( buffer.empty() ) {
        return;
    }

    this->setDictionary( &buffer[0], (int)buffer.size(), 0, (int)buffer.size() );
}

////////////////////////////////////////////////////////////////////////////////
bool Inflater::needsInput() const {

    if( this->data->stream == NULL ) {
        return false;
    }

    return this->data->stream->avail_in == 0;
}

////////////////////////////////////////////////////////////////////////////////
bool Inflater::needsDictionary() const {
    return this->data->needDictionary;
}

////////////////////////////////////////////////////////////////////////////////
void Inflater::finish() {
    this->data->flush = Z_FINISH;
}

////////////////////////////////////////////////////////////////////////////////
bool Inflater::finished() const {
    return this->data->finished;
}

////////////////////////////////////////////////////////////////////////////////
int Inflater::inflate( unsigned char* buffer, int size, int offset, int length ) {

    try{

        if( buffer == NULL ) {
            throw NullPointerException(
                __FILE__, __LINE__, "Passed Buffer was NULL." );
        }

        if( this->data->stream == NULL ) {
            throw IllegalStateException(
                __FILE__, __LINE__, "The Inflater end method has already been called." );
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
        int result = ::inflate( this->data->stream, this->data->flush );

        if( result == Z_STREAM_END ) {
            this->data->finished = true;
        } else if( result == Z_NEED_DICT ) {

            if( this->needsDictionary() ) {
                throw DataFormatException(
                    __FILE__, __LINE__, "Inflate cannot proceed until a Dictionary is set." );
            }

            this->data->needDictionary = true;
        } else if( result == Z_DATA_ERROR ) {
            throw DataFormatException(
                __FILE__, __LINE__, "Inflate failed because a block of invalid data was found." );
        }

        return (int)( this->data->stream->total_out - outStart );
    }
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCH_RETHROW( DataFormatException )
    DECAF_CATCH_RETHROW( IllegalStateException )
    DECAF_CATCHALL_THROW( IllegalStateException )
}

////////////////////////////////////////////////////////////////////////////////
int Inflater::inflate( std::vector<unsigned char>& buffer, int offset, int length ) {

    if( buffer.empty() ) {
        return 0;
    }

    return this->inflate( &buffer[0], (int)buffer.size(), offset, length );
}

////////////////////////////////////////////////////////////////////////////////
int Inflater::inflate( std::vector<unsigned char>& buffer ) {

    if( buffer.empty() ) {
        return 0;
    }

    return this->inflate( &buffer[0], (int)buffer.size(), 0, (int)buffer.size() );
}

////////////////////////////////////////////////////////////////////////////////
long long Inflater::getAdler() const {

    if( this->data->stream == NULL ) {
        throw IllegalStateException(
            __FILE__, __LINE__, "The Inflater has already been ended." );
    }

    return this->data->stream->adler;
}

////////////////////////////////////////////////////////////////////////////////
long long Inflater::getBytesRead() const {

    if( this->data->stream == NULL ) {
        throw IllegalStateException(
            __FILE__, __LINE__, "The Inflater has already been ended." );
    }

    return this->data->stream->total_in;
}

////////////////////////////////////////////////////////////////////////////////
long long Inflater::getBytesWritten() const {

    if( this->data->stream == NULL ) {
        throw IllegalStateException(
            __FILE__, __LINE__, "The Inflater has already been ended." );
    }

    return this->data->stream->total_out;
}

////////////////////////////////////////////////////////////////////////////////
void Inflater::reset() {

    if( this->data->stream == NULL ) {
        throw IllegalStateException(
            __FILE__, __LINE__, "The Inflater has already been ended." );
    }

    InflaterData::resetZlibStream( this->data );
}

////////////////////////////////////////////////////////////////////////////////
void Inflater::end() {

    if( this->data ) {
        InflaterData::finishZlibDeflate( this->data );
    }
}
