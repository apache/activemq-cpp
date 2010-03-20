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

#include <memory>

#include <decaf/lang/CharSequence.h>

using namespace std;
using namespace decaf;
using namespace decaf::io;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
void Writer::write( char v ) throw( IOException ) {

    try {
        this->doWriteChar( v );
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void Writer::write( const std::vector<char>& buffer ) throw( IOException ) {

    try {
        this->doWriteVector( buffer );
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void Writer::write( const char* buffer, int size )
    throw( decaf::io::IOException,
           decaf::lang::exceptions::NullPointerException ) {

    try {
        this->doWriteArray( buffer, size );
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void Writer::write( const char* buffer, int size, int offset, int length )
    throw( decaf::io::IOException,
           decaf::lang::exceptions::NullPointerException,
           decaf::lang::exceptions::IndexOutOfBoundsException ) {

    try {
        this->doWriteArrayBounded( buffer, size, offset, length );
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void Writer::write( const std::string& str ) throw( IOException ) {

    try {
        this->doWriteString( str );
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void Writer::write( const std::string& str, int offset, int length )
    throw( IOException, IndexOutOfBoundsException ) {

    try {

        if( ( offset + length ) > (int)str.length() ) {
            throw IndexOutOfBoundsException(
                __FILE__, __LINE__, "Given Offset + Length value greater than the String length." );
        }

        this->doWriteStringBounded( str.c_str(), offset, length );
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
Appendable& Writer::append( char value ) throw( decaf::io::IOException ) {

    try {
        return this->doAppendChar( value );
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
Appendable& Writer::append( const decaf::lang::CharSequence* csq )
    throw ( decaf::io::IOException ) {

    try {
        return this->doAppendCharSequence( csq );
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
Appendable& Writer::append( const decaf::lang::CharSequence* csq, int start, int end )
    throw( decaf::io::IOException,
           decaf::lang::exceptions::IndexOutOfBoundsException ) {

    try {
        return this->doAppendCharSequenceStartEnd( csq, start, end );
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void Writer::doWriteChar( char v ) throw( decaf::io::IOException ) {

    try {
        this->doWriteArrayBounded( (const char*)&v, 1, 0, 1 );
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void Writer::doWriteVector( const std::vector<char>& buffer )
    throw( decaf::io::IOException ) {

    try {
        this->doWriteArrayBounded( &buffer[0], buffer.size(), 0, buffer.size() );
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void Writer::doWriteArray( const char* buffer, int size )
    throw( decaf::io::IOException,
           decaf::lang::exceptions::NullPointerException ) {

    try {
        this->doWriteArrayBounded( buffer, size, 0, size );
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void Writer::doWriteString( const std::string& str ) throw( decaf::io::IOException ) {

    try {
        this->doWriteArrayBounded( str.c_str(), str.length(), 0, str.length() );
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void Writer::doWriteStringBounded( const std::string& str, int offset, int length )
    throw( decaf::io::IOException,
           decaf::lang::exceptions::IndexOutOfBoundsException ) {

    try {
        this->doWriteArrayBounded( str.c_str(), str.length(), offset, length );
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
decaf::lang::Appendable& Writer::doAppendChar( char value ) throw( decaf::io::IOException ) {

    try {
        this->doWriteChar( value );
        return *this;
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
decaf::lang::Appendable& Writer::doAppendCharSequence( const decaf::lang::CharSequence* csq )
    throw ( decaf::io::IOException ) {

    try {

        if( csq == NULL ) {
            this->doWriteString( std::string( "null" ) );
        } else {
            this->doWriteString( csq->toString() );
        }

        return *this;
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
decaf::lang::Appendable& Writer::doAppendCharSequenceStartEnd( const decaf::lang::CharSequence* csq,
                                                               int start, int end )
    throw( decaf::io::IOException,
           decaf::lang::exceptions::IndexOutOfBoundsException ) {

    try {

        if( csq == NULL ) {
            this->doWriteString( string( "null" ).substr( start, end - start ) );
        } else {
            std::auto_ptr<CharSequence> result( csq->subSequence( start, end ) );
            this->doWriteString( result->toString() );
        }

        return *this;
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCHALL_THROW( IOException )
}
