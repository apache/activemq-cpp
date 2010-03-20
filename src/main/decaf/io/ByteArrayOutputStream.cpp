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

#include "ByteArrayOutputStream.h"

using namespace std;
using namespace decaf;
using namespace decaf::io;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
ByteArrayOutputStream::ByteArrayOutputStream() : OutputStream() {
    activeBuffer = &defaultBuffer;
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayOutputStream::~ByteArrayOutputStream() {
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayOutputStream::ByteArrayOutputStream( vector<unsigned char>& buffer) {
    setBuffer( buffer );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayOutputStream::setBuffer( vector<unsigned char>& buffer) {
    activeBuffer = &buffer;
}

////////////////////////////////////////////////////////////////////////////////
const unsigned char* ByteArrayOutputStream::toByteArray() const {
    if( activeBuffer == NULL || activeBuffer->size() == 0 ){
        return NULL;
    }

    return &(*activeBuffer)[0];
}

////////////////////////////////////////////////////////////////////////////////
const std::vector<unsigned char>& ByteArrayOutputStream::toByteArrayRef() const {
    if( activeBuffer == NULL ){
        return defaultBuffer;
    }

    return *activeBuffer;
}

////////////////////////////////////////////////////////////////////////////////
long long ByteArrayOutputStream::size() const {
    return activeBuffer->size();
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayOutputStream::reset() throw ( IOException ) {
    // Empty the contents of the buffer to the output stream.
    activeBuffer->clear();
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayOutputStream::doWriteByte( unsigned char c )
    throw ( IOException ) {

    try{
        activeBuffer->push_back( c );
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayOutputStream::doWriteArrayBounded( const unsigned char* buffer, int size, int offset, int length )
    throw ( decaf::io::IOException,
            decaf::lang::exceptions::NullPointerException,
            decaf::lang::exceptions::IndexOutOfBoundsException ) {

    if( length == 0 ) {
        return;
    }

    if( buffer == NULL ) {
        throw NullPointerException(
            __FILE__, __LINE__, "passed buffer is null" );
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

    try{
        activeBuffer->insert( activeBuffer->end(), buffer + offset, buffer + offset + length );
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
std::string ByteArrayOutputStream::toString() const {

    if( this->activeBuffer->empty() ) {
        return "";
    }

    return string( (const char*)this->toByteArray(), this->size() );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayOutputStream::writeTo( OutputStream* out ) const
    throw ( IOException, lang::exceptions::NullPointerException ) {

    try{

        if( this->size() == 0 ) {
            return;
        }

        if( out == NULL ) {
            throw NullPointerException(
                __FILE__, __LINE__, "Passed stream pointer is null" );
        }

        out->write( this->toByteArray(), this->size() );
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCHALL_THROW( IOException )
}
