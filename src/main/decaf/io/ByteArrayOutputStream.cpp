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
#include <algorithm>

using namespace std;
using namespace decaf;
using namespace decaf::io;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
ByteArrayOutputStream::ByteArrayOutputStream() {
    activeBuffer = &defaultBuffer;
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
void ByteArrayOutputStream::reset() throw ( IOException ) {
    // Empty the contents of the buffer to the output stream.
    activeBuffer->clear();
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayOutputStream::write( unsigned char c )
    throw ( IOException ) {
    activeBuffer->push_back( c );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayOutputStream::write( const std::vector<unsigned char>& buffer )
    throw ( IOException ) {

    try{

        if( buffer.empty() ) {
            return;
        }

        this->write( &buffer[0], 0, buffer.size() );
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayOutputStream::write( const unsigned char* buffer,
                                   std::size_t offset,
                                   std::size_t len )
    throw ( IOException, lang::exceptions::NullPointerException ) {

    if( buffer == NULL ) {
        throw NullPointerException(
            __FILE__, __LINE__,
            "ByteArrayOutputStream::write - passed buffer is null" );
    }

    std::back_insert_iterator< std::vector<unsigned char> > iter( *activeBuffer );
    std::copy( buffer + offset, buffer + offset + len, iter );
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
                __FILE__, __LINE__,
                "ByteArrayOutputStream::writeTo - Passed stream pointer is null" );
        }

        out->write( this->toByteArray(), 0, this->size() );
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCHALL_THROW( IOException )
}
