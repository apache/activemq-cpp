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

#include "InputStreamReader.h"

#include <decaf/io/InputStream.h>
#include <decaf/lang/exceptions/NullPointerException.h>

using namespace decaf;
using namespace decaf::io;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
InputStreamReader::InputStreamReader( InputStream* stream, bool own ) : Reader(), own( own ) {

    if( stream == NULL ) {
        throw NullPointerException(
            __FILE__, __LINE__, "The passed InputStream cannot be NULL." );
    }

    this->stream = stream;
    this->closed = false;
}

////////////////////////////////////////////////////////////////////////////////
InputStreamReader::~InputStreamReader() {

    try{

        this->close();

        if( this->own ) {
            delete this->stream;
        }

        this->stream = NULL;
    }
    DECAF_CATCH_NOTHROW( Exception )
    DECAF_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void InputStreamReader::close() throw( decaf::io::IOException ) {

    try{

        if( !closed ) {
            this->stream->close();
            this->closed = true;
        }
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
bool InputStreamReader::ready() const throw( decaf::io::IOException ) {

    try{
        checkClosed();
        try{
            return this->stream->available() != 0;
        } catch( IOException& ex ) {
            return false;
        }
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
int InputStreamReader::read( char* buffer, std::size_t offset, std::size_t length )
    throw( IOException, lang::exceptions::NullPointerException ) {

    try{
        checkClosed();

        if( length == 0 ) {
            return 0;
        }

        return this->stream->read( (unsigned char*)buffer, offset, length );
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void InputStreamReader::checkClosed() const throw( decaf::io::IOException ) {
    if( closed ) {
        throw IOException( __FILE__, __LINE__, "This Reader is Closed" );
    }
}

////////////////////////////////////////////////////////////////////////////////
int InputStreamReader::read( std::vector<char>& buffer )
    throw( decaf::io::IOException ) {

    try{
        checkClosed();
        return Reader::read( buffer );
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
int InputStreamReader::read() throw( decaf::io::IOException ) {

    try{
        checkClosed();
        return Reader::read();
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
int InputStreamReader::read( decaf::nio::CharBuffer* charBuffer )
         throw( decaf::io::IOException,
                decaf::lang::exceptions::NullPointerException,
                decaf::nio::ReadOnlyBufferException ) {

    try{
        checkClosed();
        return Reader::read( charBuffer );
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}
