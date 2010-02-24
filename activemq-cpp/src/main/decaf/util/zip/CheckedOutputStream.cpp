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

#include "CheckedOutputStream.h"

using namespace decaf;
using namespace decaf::io;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::util;
using namespace decaf::util::zip;

////////////////////////////////////////////////////////////////////////////////
CheckedOutputStream::CheckedOutputStream( OutputStream* outputStream, Checksum* sum, bool own )
 :  FilterOutputStream( outputStream, own ), sum( sum ) {

    if( sum == NULL ) {
        throw NullPointerException(
            __FILE__, __LINE__, "The Checksum instance cannot be NULL." );
    }
}

////////////////////////////////////////////////////////////////////////////////
CheckedOutputStream::~CheckedOutputStream() {
}

////////////////////////////////////////////////////////////////////////////////
void CheckedOutputStream::doWriteByte( unsigned char value ) throw ( decaf::io::IOException ) {

    try{

        if( isClosed() ) {
            throw IOException(
                 __FILE__, __LINE__, "Stream already closed" );
        }

        this->outputStream->write( value );
        this->sum->update( value );
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void CheckedOutputStream::doWriteArrayBounded( const unsigned char* buffer, std::size_t size,
                                               std::size_t offset, std::size_t length )
    throw ( decaf::io::IOException,
            decaf::lang::exceptions::NullPointerException,
            decaf::lang::exceptions::IndexOutOfBoundsException ) {

    try{

        if( buffer == NULL ) {
            throw NullPointerException(
                 __FILE__, __LINE__, "The buffer passed was NULL." );
        }

        if( offset + length > size ) {
            throw IndexOutOfBoundsException(
                __FILE__, __LINE__, "Given offset + length exceeds the buffer size." );
        }

        if( isClosed() ) {
            throw IOException(
                 __FILE__, __LINE__, "Stream already closed" );
        }

        this->outputStream->write( buffer, size, offset, length );
        this->sum->update( buffer, size, offset, length );
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCHALL_THROW( IOException )
}
