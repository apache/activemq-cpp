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

#include "String.h"

#include <decaf/lang/ArrayPointer.h>
#include <decaf/lang/System.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/exceptions/IndexOutOfBoundsException.h>
#include <decaf/lang/Short.h>
#include <decaf/lang/Integer.h>
#include <decaf/lang/Long.h>
#include <decaf/lang/Float.h>
#include <decaf/lang/Double.h>

using namespace std;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
namespace decaf{
namespace lang{

    class Contents {
    public:

        ArrayPointer<unsigned char> value;
        int length;
        int offset;

    public:

        Contents() : value(), length( 0 ), offset( 0 ) {
        }

        Contents( int length ) : value( length ), length( length ), offset( 0 ) {
        }

    };

}}

////////////////////////////////////////////////////////////////////////////////
String::String() : contents(new Contents()) {
}

////////////////////////////////////////////////////////////////////////////////
String::String( const String& source ) : contents(new Contents(source.length())) {

    // TODO
    // load the passed string into the contents value.
    //System::arraycopy( (unsigned char*)source.c_str(), 0, contents->value.get(), 0, source.length() );
}

////////////////////////////////////////////////////////////////////////////////
String::String( const std::string& source ) : contents(new Contents((int)source.length())) {

    // load the passed string into the contents value.
    System::arraycopy( (unsigned char*)source.c_str(), 0, contents->value.get(), 0, source.length() );
}

////////////////////////////////////////////////////////////////////////////////
String::String(const char* array, int size) : contents(new Contents) {

    if( size < 0 ) {
        throw IndexOutOfBoundsException(
            __FILE__, __LINE__, "size parameter out of Bounds: %d.", size );
    }

    if( array == NULL ) {
        throw NullPointerException(
            __FILE__, __LINE__, "Buffer pointer passed was NULL." );
    }

    if(size > 0) {

        this->contents->value = ArrayPointer<unsigned char>(size);
        this->contents->length = size;

        System::arraycopy( (unsigned char*)array, 0, contents->value.get(), 0, size );
    }
}

////////////////////////////////////////////////////////////////////////////////
String::String(const char* array, int size, int offset, int length) : contents(new Contents) {

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

    if( array == NULL ) {
        throw NullPointerException(
            __FILE__, __LINE__, "Buffer pointer passed was NULL." );
    }

    if(size > 0) {

        this->contents->value = ArrayPointer<unsigned char>(length);
        this->contents->length = length;

        System::arraycopy( (unsigned char*)array, offset, contents->value.get(), 0, length );
    }
}

////////////////////////////////////////////////////////////////////////////////
String::~String() {
    try{
        delete this->contents;
    }
    DECAF_CATCH_NOTHROW( Exception )
    DECAF_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
int String::length() const {
    return this->contents->offset + this->contents->length;
}

////////////////////////////////////////////////////////////////////////////////
bool String::isEmpty() const {
    return ( this->contents->offset + this->contents->length ) == 0;
}

////////////////////////////////////////////////////////////////////////////////
char String::charAt( int index ) const {

    try{

        if( index < 0 || index >= this->length() ) {
            throw IndexOutOfBoundsException(
                __FILE__, __LINE__, "Index given is out of bounds: %d.", index );
        }

        return this->contents->value[this->contents->offset + index];
    }
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCHALL_THROW( IndexOutOfBoundsException )
}

////////////////////////////////////////////////////////////////////////////////
CharSequence* String::subSequence( int start DECAF_UNUSED, int end DECAF_UNUSED ) const {

    try{

        if( start > end ) {
            throw IndexOutOfBoundsException(
                __FILE__, __LINE__, "Start index is greater than end index." );
        }

        if( end - start > this->length() ) {
            throw IndexOutOfBoundsException(
                __FILE__, __LINE__, "Requested Range is greater than the String length." );
        }

        return NULL;
    }
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCHALL_THROW( IndexOutOfBoundsException )
}

////////////////////////////////////////////////////////////////////////////////
std::string String::toString() const {

    if( this->contents->value == NULL ) {
        return "null";
    }

    return std::string( (const char*)this->contents->value.get(), this->length() );
}

////////////////////////////////////////////////////////////////////////////////
String String::valueOf(bool value) {

    if(value) {
        return String("true");
    }

    return String("false");
}

////////////////////////////////////////////////////////////////////////////////
String String::valueOf(char value) {
    return String( &value, 1 );
}

////////////////////////////////////////////////////////////////////////////////
String String::valueOf(float value) {
    return String( Float::toString(value) );
}

////////////////////////////////////////////////////////////////////////////////
String String::valueOf(double value) {
    return String( Double::toString(value) );
}

////////////////////////////////////////////////////////////////////////////////
String String::valueOf(short value) {
    return String( Short::toString(value) );
}

////////////////////////////////////////////////////////////////////////////////
String String::valueOf(int value) {
    return String( Integer::toString(value) );
}

////////////////////////////////////////////////////////////////////////////////
String String::valueOf(long long value) {
    return String( Long::toString(value) );
}
