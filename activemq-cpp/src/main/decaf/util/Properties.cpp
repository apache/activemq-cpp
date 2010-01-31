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
#include <decaf/lang/exceptions/UnsupportedOperationException.h>
#include <decaf/util/StlMap.h>

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

    enum TokenState {
        NONE = 0,
        SLASH = 1,
        CONTINUE = 2,
        KEY_DONE = 3,
        IGNOE = 4
    };

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

        throw UnsupportedOperationException(
            __FILE__, __LINE__,
            "Not yet Implemented." );
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
