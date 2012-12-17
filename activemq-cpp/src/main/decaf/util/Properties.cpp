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

#include "Properties.h"

#include <map>
#include <sstream>
#include <decaf/util/Date.h>
#include <decaf/util/Map.h>
#include <decaf/util/StlMap.h>
#include <decaf/io/BufferedInputStream.h>
#include <decaf/lang/Character.h>
#include <decaf/lang/Integer.h>
#include <decaf/lang/exceptions/UnsupportedOperationException.h>

using namespace decaf;
using namespace decaf::util;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::io;

////////////////////////////////////////////////////////////////////////////////
namespace decaf{
namespace util{

    class PropertiesInternal{
    public:

        decaf::util::StlMap< std::string, std::string > properties;

        PropertiesInternal() : properties() {}
    };

}}

////////////////////////////////////////////////////////////////////////////////
namespace {

    /**
     * internal namespace for Properties utility methods, these might change often
     * so we hide them in here to preserve ABI rules on the header.
     */

    enum ParsingMode {
        PARSE_MODE_NONE = 0,
        PARSE_MODE_SLASH = 1,
        PARSE_MODE_CONTINUE = 2,
        PARSE_MODE_KEY_DONE = 3,
        PARSE_MODE_IGNORE = 4
    };

    void dumpString( std::ostringstream& buffer, const std::string& string, bool key ) {

        std::size_t i = 0;
        if( !key && i < string.length() && string.at(i) == ' ' ) {
            buffer << "\\ ";
            i++;
        }

        for( ; i < string.length(); i++ ) {

            char ch = string.at(i);

            switch(ch) {
                case '\t':
                    buffer << "\\t";
                    break;
                case '\n':
                    buffer << "\\n";
                    break;
                case '\f':
                    buffer << "\\f";
                    break;
                case '\r':
                    buffer << "\\r";
                    break;
                default:
                    if( std::string( "\\#!=:" ).find( ch ) != std::string::npos || ( key && ch == ' ' ) ) {
                        buffer << '\\';
                    }

                    if( ch >= ' ' && ch <= '~' ) {
                        buffer << ch;
                    } else {

                        std::string hex = Integer::toHexString( ch );
                        buffer << "\\u";
                        for( std::size_t j = 0; j < 4 - hex.length(); j++ ) {
                            buffer << "0";
                        }
                        buffer << hex;
                    }
                    break;
            }
        }
    }

}

////////////////////////////////////////////////////////////////////////////////
Properties::Properties() : internal( new PropertiesInternal() ), defaults() {
}

////////////////////////////////////////////////////////////////////////////////
Properties::Properties( const Properties& src ) : internal( new PropertiesInternal() ), defaults() {

    this->internal->properties.copy( src.internal->properties );

    if( src.defaults.get() != NULL ) {
        this->defaults.reset( src.defaults->clone() );
    }
}

////////////////////////////////////////////////////////////////////////////////
Properties::~Properties() {
    try{
        delete this->internal;
    }
    DECAF_CATCH_NOTHROW( Exception )
    DECAF_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
Properties& Properties::operator= ( const Properties& source ) {

    if( this == &source ) {
        return *this;
    }

    this->copy( source );

    return *this;
}

////////////////////////////////////////////////////////////////////////////////
bool Properties::isEmpty() const {
    synchronized( &( internal->properties ) ) {
        return internal->properties.isEmpty();
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////
int Properties::size() const {

    synchronized( &( internal->properties ) ) {
        return (int)internal->properties.size();
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
const char* Properties::getProperty( const std::string& name ) const{

    synchronized( &( internal->properties ) ) {
        if( this->internal->properties.containsKey( name ) ) {
            return this->internal->properties.get( name ).c_str();
        } else if( this->defaults != NULL && this->defaults->hasProperty( name ) ) {
            return this->defaults->getProperty( name );
        }
    }

    return NULL;
}

////////////////////////////////////////////////////////////////////////////////
std::string Properties::getProperty( const std::string& name,
                                     const std::string& defaultValue ) const {

    synchronized( &( internal->properties ) ) {
        if( this->internal->properties.containsKey( name ) ) {
            return this->internal->properties.get( name );
        } else if( this->defaults != NULL && this->defaults->hasProperty( name ) ) {
            return this->defaults->getProperty( name );
        }
    }

    return defaultValue;
}

////////////////////////////////////////////////////////////////////////////////
std::string Properties::setProperty( const std::string& name, const std::string& value ){

    std::string oldValue;

    synchronized( &( internal->properties ) ) {

        if( internal->properties.containsKey( name ) ) {
            oldValue = internal->properties.get( name );
        }

        internal->properties.put( name, value );
    }

    return oldValue;
}

////////////////////////////////////////////////////////////////////////////////
bool Properties::hasProperty( const std::string& name ) const {

    synchronized( &( internal->properties ) ) {
        return this->internal->properties.containsKey( name );
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////
std::string Properties::remove( const std::string& name ){

    std::string oldValue;

    synchronized( &( internal->properties ) ) {
        if( this->internal->properties.containsKey( name ) ) {
            oldValue = this->internal->properties.get( name );
            this->internal->properties.remove( name );
        }
    }

    return oldValue;
}

////////////////////////////////////////////////////////////////////////////////
std::vector< std::pair< std::string, std::string > > Properties::toArray() const{

    std::vector< std::pair<std::string, std::string> > result;

    synchronized( &( internal->properties ) ) {
        Pointer< Iterator< MapEntry<std::string, std::string> > > entries(
                this->internal->properties.entrySet().iterator());
        while (entries->hasNext()) {
            MapEntry<std::string, std::string> entry = entries->next();
            result.push_back(std::make_pair(entry.getKey(), entry.getValue()));
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
void Properties::copy( const Properties& source ){

    if( &source == this ) {
        return;
    }

    synchronized( &( this->internal->properties ) ) {

        synchronized( &( source.internal->properties ) ) {
            this->internal->properties.copy( source.internal->properties );

            if( source.defaults.get() != NULL ) {
                this->defaults.reset( source.defaults->clone() );
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
Properties* Properties::clone() const{

    Properties* props = new Properties();
    props->internal->properties.copy( this->internal->properties );
    return props;
}

////////////////////////////////////////////////////////////////////////////////
void Properties::clear(){

    synchronized( &( internal->properties ) ) {
        this->internal->properties.clear();
    }
}

////////////////////////////////////////////////////////////////////////////////
bool Properties::equals( const Properties& source ) const {
    return this->internal->properties.equals( source.internal->properties );
}

////////////////////////////////////////////////////////////////////////////////
std::string Properties::toString() const {

    std::ostringstream stream;
    std::map< std::string, std::string >::const_iterator iter;

    stream << "Begin Class decaf::util::Properties:" << std::endl;

    synchronized( &( internal->properties ) ) {
        Pointer< Iterator< MapEntry<std::string, std::string> > > entries(
                this->internal->properties.entrySet().iterator());
        while (entries->hasNext()) {
            MapEntry<std::string, std::string> entry = entries->next();
            stream << " property[" << entry.getKey() << "] = " << entry.getValue() << std::endl;
        }
    }

    stream << "End Class decaf::util::Properties:" << std::endl;

    return stream.str();
}

////////////////////////////////////////////////////////////////////////////////
std::vector<std::string> Properties::propertyNames() const {
    StlMap<std::string, std::string> selectedProperties;
    this->selectProperties( selectedProperties );
    return selectedProperties.keySet().toArray();
}

////////////////////////////////////////////////////////////////////////////////
void Properties::selectProperties( StlMap<std::string, std::string>& selectProperties ) const {

    if( this->defaults != NULL ) {
        this->defaults->selectProperties( selectProperties );
    }

    Pointer< Iterator< MapEntry<std::string, std::string> > > entries(
            this->internal->properties.entrySet().iterator());
    while (entries->hasNext()) {
        MapEntry<std::string, std::string> entry = entries->next();
        selectProperties.put(entry.getKey(), entry.getValue());
    }
}

////////////////////////////////////////////////////////////////////////////////
void Properties::load( decaf::io::InputStream* stream ) {

    try{

        if( stream == NULL ) {
            throw NullPointerException(
                __FILE__, __LINE__,
                "The Stream instance passed was Null" );
        }

        int mode = PARSE_MODE_NONE;
        char nextChar;
        std::vector<char> buf;
        int offset = 0;
        int keyLength = -1;
        int intVal;
        bool firstChar = true;
        BufferedInputStream bis( stream );

        while(true) {

            intVal = bis.read();

            if( intVal == -1 ) {
                break;
            }

            nextChar = (char) ( intVal & 0xFF );

            if( mode == PARSE_MODE_SLASH ) {

                mode = PARSE_MODE_NONE;
                switch( nextChar ) {
                    case '\r':
                        mode = PARSE_MODE_CONTINUE; // Look for a following \n
                        continue;
                    case '\n':
                        mode = PARSE_MODE_IGNORE; // Ignore whitespace on the next line
                        continue;
                    case 'b':
                        nextChar = '\b';
                        break;
                    case 'f':
                        nextChar = '\f';
                        break;
                    case 'n':
                        nextChar = '\n';
                        break;
                    case 'r':
                        nextChar = '\r';
                        break;
                    case 't':
                        nextChar = '\t';
                        break;
                }

            } else {

                switch( nextChar ) {
                    case '#':
                    case '!':
                        if( firstChar ) {
                            while( true ) {
                                intVal = bis.read();
                                if( intVal == -1 ) {
                                    break;
                                }

                                nextChar = (char)( intVal & 0xFF );

                                if( nextChar == '\r' || nextChar == '\n' ) {
                                    break;
                                }
                            }
                            continue;
                        }
                        break;
                    case '\n':
                        if( mode == PARSE_MODE_CONTINUE) { // Part of a \r\n sequence
                            mode = PARSE_MODE_IGNORE; // Ignore whitespace on the next line
                            continue;
                        }
                        // fall into the next case
                    case '\r':
                        mode = PARSE_MODE_NONE;
                        firstChar = true;
                        if( offset > 0 || ( offset == 0 && keyLength == 0 ) ) {

                            if( keyLength == -1 ) {
                                keyLength = offset;
                            }
                            std::string temp( buf.begin(), buf.begin() + offset );

                            this->internal->properties.put( temp.substr( 0, keyLength ),
                                                            temp.substr( keyLength ) );
                        }

                        keyLength = -1;
                        offset = 0;
                        buf.clear();
                        continue;
                    case '\\':
                        if( mode == PARSE_MODE_KEY_DONE ) {
                            keyLength = offset;
                        }
                        mode = PARSE_MODE_SLASH;
                        continue;
                    case ':':
                    case '=':
                        if( keyLength == -1 ) { // if parsing the key
                            mode = PARSE_MODE_NONE;
                            keyLength = offset;
                            continue;
                        }
                        break;
                }
                if( Character::isWhitespace( nextChar ) ) {
                    if( mode == PARSE_MODE_CONTINUE ) {
                        mode = PARSE_MODE_IGNORE;
                    }
                    // if key length == 0 or value length == 0
                    if( offset == 0 || offset == keyLength || mode == PARSE_MODE_IGNORE ) {
                        continue;
                    }
                    if( keyLength == -1 ) { // if parsing the key
                        mode = PARSE_MODE_KEY_DONE;
                        continue;
                    }
                }

                if( mode == PARSE_MODE_IGNORE || mode == PARSE_MODE_CONTINUE ) {
                    mode = PARSE_MODE_NONE;
                }
            }

            firstChar = false;
            if( mode == PARSE_MODE_KEY_DONE ) {
                keyLength = offset;
                mode = PARSE_MODE_NONE;
            }

            offset += 1;
            buf.push_back( nextChar );
        }

        if( keyLength == -1 && offset > 0 ) {
            keyLength = offset;
        }

        if( keyLength >= 0 ) {
            std::string temp( buf.begin(), buf.begin() + offset );
            this->internal->properties.put( temp.substr( 0, keyLength ), temp.substr( keyLength ) );
        }
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_RETHROW( IllegalArgumentException )
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void Properties::load( decaf::io::Reader* reader ) {

    try{

        if( reader == NULL ) {
            throw NullPointerException(
                __FILE__, __LINE__,
                "The Reader instance passed was Null" );
        }

        throw UnsupportedOperationException(
            __FILE__, __LINE__,
            "Not yet Implemented." );
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_RETHROW( IllegalArgumentException )
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void Properties::store( decaf::io::OutputStream* out, const std::string& comments DECAF_UNUSED ) {

    try{

        if( out == NULL ) {
            throw NullPointerException(
                __FILE__, __LINE__,
                "The OutputStream instance passed was Null" );
        }

        std::ostringstream buffer;
        std::ostringstream writer;

        if( comments != "" ) {
            writer << "#";
            writer << comments;
            writer << std::endl;
        }

        writer << "#";
        writer << Date().toString();
        writer << std::endl;

        Pointer< Iterator< MapEntry<std::string, std::string> > > entries(
                this->internal->properties.entrySet().iterator());
        while (entries->hasNext()) {
            MapEntry<std::string, std::string> entry = entries->next();

            dumpString( buffer, entry.getKey(), true );
            buffer << "=";
            dumpString( buffer, entry.getValue(), false );
            buffer << std::endl;

            writer << buffer.str();
            buffer.str("");
        }

        int length = (int)writer.str().length();

        out->write( (const unsigned char*)writer.str().c_str(), length, 0, length );
        out->flush();
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void Properties::store( decaf::io::Writer* writer, const std::string& comments DECAF_UNUSED ) {

    try{

        if( writer == NULL ) {
            throw NullPointerException(
                __FILE__, __LINE__,
                "The Writer instance passed was Null" );
        }

        throw UnsupportedOperationException(
            __FILE__, __LINE__,
            "Not yet Implemented." );
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    DECAF_CATCHALL_THROW( IOException )
}

