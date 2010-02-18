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

    };

}}

////////////////////////////////////////////////////////////////////////////////
namespace {

    /**
     * internal namespace for Properties utility methods, these might change often
     * so we hide them in here to preserve ABI rules on the header.
     */

    enum ParsingMode {
        NONE = 0,
        SLASH = 1,
        CONTINUE = 2,
        KEY_DONE = 3,
        IGNORE = 4
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
            }
        }
    }

}

////////////////////////////////////////////////////////////////////////////////
Properties::Properties() {
    this->internal.reset( new PropertiesInternal() );
}

////////////////////////////////////////////////////////////////////////////////
Properties::Properties( const Properties& src ) {
    this->internal.reset( new PropertiesInternal() );
    this->internal->properties.copy( src.internal->properties );

    if( src.defaults.get() != NULL ) {
        this->defaults.reset( src.defaults->clone() );
    }
}

////////////////////////////////////////////////////////////////////////////////
Properties::~Properties() {
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
std::size_t Properties::size() const {

    synchronized( &( internal->properties ) ) {
        return internal->properties.size();
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
void Properties::setProperty( const std::string& name,
                              const std::string& value ){

    synchronized( &( internal->properties ) ) {
        internal->properties.put( name, value );
    }
}

////////////////////////////////////////////////////////////////////////////////
bool Properties::hasProperty( const std::string& name ) const {

    synchronized( &( internal->properties ) ) {
        return this->internal->properties.containsKey( name );
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////
void Properties::remove( const std::string& name ){
    synchronized( &( internal->properties ) ) {
        if( this->internal->properties.containsKey( name ) ) {
            this->internal->properties.remove( name );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
std::vector< std::pair< std::string, std::string > > Properties::toArray() const{

    std::vector< std::pair<std::string, std::string> > result;

    synchronized( &( internal->properties ) ) {
        std::vector<std::string> keys = this->internal->properties.keySet();
        std::vector<std::string>::const_iterator iter = keys.begin();

        for( ; iter != keys.end(); ++iter ) {
            result.push_back(
                std::make_pair( *iter, this->internal->properties.get( *iter ) ) );
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
        std::vector<std::string> keys = this->internal->properties.keySet();
        std::vector<std::string>::const_iterator iter = keys.begin();

        for( iter = keys.begin(); iter != keys.end(); ++iter ){
            stream << " property[" << *iter << "] = "
                   << this->internal->properties.get( *iter ) << std::endl;
        }
    }

    stream << "End Class decaf::util::Properties:" << std::endl;

    return stream.str();
}

////////////////////////////////////////////////////////////////////////////////
std::vector<std::string> Properties::propertyNames() const {

    StlMap<std::string, std::string> selectedProperties;

    this->selectProperties( selectedProperties );

    return selectedProperties.keySet();
}

////////////////////////////////////////////////////////////////////////////////
void Properties::selectProperties( StlMap<std::string, std::string>& selectProperties ) const {

    if( this->defaults != NULL ) {
        this->defaults->selectProperties( selectProperties );
    }

    std::vector<std::string> keys = this->internal->properties.keySet();
    std::vector<std::string>::const_iterator key = keys.begin();

    for( ; key != keys.end(); ++key ) {
        selectProperties.put( *key, this->internal->properties.get( *key ) );
    }
}

////////////////////////////////////////////////////////////////////////////////
void Properties::load( decaf::io::InputStream* stream )
    throw( decaf::io::IOException,
           decaf::lang::exceptions::IllegalArgumentException,
           decaf::lang::exceptions::NullPointerException ) {

    try{

        if( stream == NULL ) {
            throw NullPointerException(
                __FILE__, __LINE__,
                "The Stream instance passed was Null" );
        }

        int mode = NONE;
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

            if( mode == SLASH ) {

                mode = NONE;
                switch( nextChar ) {
                    case '\r':
                        mode = CONTINUE; // Look for a following \n
                        continue;
                    case '\n':
                        mode = IGNORE; // Ignore whitespace on the next line
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
                        if( mode == CONTINUE) { // Part of a \r\n sequence
                            mode = IGNORE; // Ignore whitespace on the next line
                            continue;
                        }
                        // fall into the next case
                    case '\r':
                        mode = NONE;
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
                        if( mode == KEY_DONE ) {
                            keyLength = offset;
                        }
                        mode = SLASH;
                        continue;
                    case ':':
                    case '=':
                        if( keyLength == -1 ) { // if parsing the key
                            mode = NONE;
                            keyLength = offset;
                            continue;
                        }
                        break;
                }
                if( Character::isWhitespace( nextChar ) ) {
                    if( mode == CONTINUE ) {
                        mode = IGNORE;
                    }
                    // if key length == 0 or value length == 0
                    if( offset == 0 || offset == keyLength || mode == IGNORE ) {
                        continue;
                    }
                    if( keyLength == -1 ) { // if parsing the key
                        mode = KEY_DONE;
                        continue;
                    }
                }

                if( mode == IGNORE || mode == CONTINUE ) {
                    mode = NONE;
                }
            }

            firstChar = false;
            if( mode == KEY_DONE ) {
                keyLength = offset;
                mode = NONE;
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
void Properties::load( decaf::io::Reader* reader )
    throw( decaf::io::IOException,
           decaf::lang::exceptions::IllegalArgumentException,
           decaf::lang::exceptions::NullPointerException ) {

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
void Properties::store( decaf::io::OutputStream* out, const std::string& comments DECAF_UNUSED )
    throw( decaf::io::IOException,
           decaf::lang::exceptions::NullPointerException ) {

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

        std::vector<std::string> keys = this->internal->properties.keySet();
        std::vector<std::string>::const_iterator key = keys.begin();

        for( ; key != keys.end(); ++key ) {

            dumpString( buffer, *key, true );
            buffer << "=";
            dumpString( buffer, this->internal->properties.get( *key ), false );
            buffer << std::endl;

            writer << buffer.str();
            buffer.str("");
        }

        std::size_t length = writer.str().length();

        out->write( (const unsigned char*)writer.str().c_str(), length, 0, length );
        out->flush();
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void Properties::store( decaf::io::Writer* writer, const std::string& comments DECAF_UNUSED )
    throw( decaf::io::IOException,
           decaf::lang::exceptions::NullPointerException ) {

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

