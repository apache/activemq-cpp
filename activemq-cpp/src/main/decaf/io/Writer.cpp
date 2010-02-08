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

#include "Writer.h"

#include <decaf/lang/CharSequence.h>

using namespace std;
using namespace decaf;
using namespace decaf::io;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
void Writer::write( char v ) throw( IOException ) {

    try {
        this->write( (const char*)&v, 0, 1 );
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void Writer::write( const std::vector<char>& buffer ) throw( IOException ) {

    try {
        this->write( &buffer[0], 0, buffer.size() );
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void Writer::write( const std::string& str ) throw( IOException ) {

    try {
        this->write( str.c_str(), 0, str.length() );
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void Writer::write( const std::string& str, std::size_t offset, std::size_t length )
    throw( IOException, IndexOutOfBoundsException ) {

    try {

        if( ( offset + length ) > str.length() ) {
            throw IndexOutOfBoundsException(
                __FILE__, __LINE__, "Given Offset + Length value greater than the String length." );
        }

        this->write( str.c_str(), offset, length );
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
Appendable& Writer::append( char value ) throw( decaf::io::IOException ) {

    try {
        this->write( value );
        return *this;
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
Appendable& Writer::append( const decaf::lang::CharSequence* csq )
    throw ( decaf::io::IOException ) {

    try {

        if( csq == NULL ) {
            this->write( std::string( "null" ) );
        } else {
            this->write( csq->toString() );
        }

        return *this;
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
Appendable& Writer::append( const decaf::lang::CharSequence* csq, std::size_t start, std::size_t end )
    throw( decaf::io::IOException,
           decaf::lang::exceptions::IndexOutOfBoundsException ) {

    try {

        if( csq == NULL ) {
            this->write( string( "null" ).substr( start, end - start ) );
        } else {
            CharSequence* result = csq->subSequence( start, end );
            this->write( result->toString() );
            delete result;
        }

        return *this;
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCHALL_THROW( IOException )
}

