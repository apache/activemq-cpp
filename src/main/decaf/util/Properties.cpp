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
#include <decaf/util/concurrent/Mutex.h>

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

        std::map< std::string, std::string > properties;
        decaf::util::concurrent::Mutex mutex;

    };

}}

////////////////////////////////////////////////////////////////////////////////
Properties::Properties() {
    this->internal.reset( new PropertiesInternal() );
}

////////////////////////////////////////////////////////////////////////////////
Properties::Properties( const Properties& src ) {
    this->internal.reset( new PropertiesInternal() );
    this->internal->properties = src.internal->properties;

    if( src.defaults.get() != NULL ) {
        this->defaults.reset( src.defaults->clone() );
    }
}

////////////////////////////////////////////////////////////////////////////////
Properties::~Properties() {
}

////////////////////////////////////////////////////////////////////////////////
Properties& Properties::operator= ( const Properties& src ) {

    if( this == &src ) {
        return *this;
    }

    synchronized( &( internal->mutex ) ) {
        this->internal.reset( new PropertiesInternal() );

        synchronized( &( src.internal->mutex ) ) {
            this->internal->properties = src.internal->properties;
        }

        if( src.defaults.get() != NULL ) {
            this->defaults.reset( src.defaults->clone() );
        }
    }

    return *this;
}

////////////////////////////////////////////////////////////////////////////////
bool Properties::isEmpty() const {
    synchronized( &( internal->mutex ) ) {
        return internal->properties.empty();
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////
const char* Properties::getProperty( const std::string& name ) const{

    synchronized( &( internal->mutex ) ) {
        std::map< std::string, std::string >::const_iterator iter =
            internal->properties.find( name );
        if( iter == internal->properties.end() ){
            return NULL;
        }

        return iter->second.c_str();
    }

    return NULL;
}

////////////////////////////////////////////////////////////////////////////////
std::string Properties::getProperty( const std::string& name,
                                     const std::string& defaultValue ) const {

    synchronized( &( internal->mutex ) ) {
        std::map< std::string, std::string >::const_iterator iter =
            internal->properties.find( name );
        if( iter != internal->properties.end() ){
            return iter->second;
        }
    }

    return defaultValue;
}

////////////////////////////////////////////////////////////////////////////////
void Properties::setProperty( const std::string& name,
                              const std::string& value ){

    synchronized( &( internal->mutex ) ) {
        internal->properties[name] = value;
    }
}

////////////////////////////////////////////////////////////////////////////////
bool Properties::hasProperty( const std::string& name ) const {

    synchronized( &( internal->mutex ) ) {
        if( internal->properties.find(name) != internal->properties.end() ) {
            return true;
        }
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////
void Properties::remove( const std::string& name ){
    synchronized( &( internal->mutex ) ) {
        internal->properties.erase( name );
    }
}

////////////////////////////////////////////////////////////////////////////////
std::vector< std::pair< std::string, std::string > > Properties::toArray() const{

    std::vector< std::pair<std::string, std::string> > result;

    synchronized( &( internal->mutex ) ) {
        result.assign( internal->properties.begin(), internal->properties.end() );
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
void Properties::copy( const Properties& source ){

    if( &source == this ) {
        return;
    }

    synchronized( &( internal->mutex ) ) {
        *this = source;
    }
}

////////////////////////////////////////////////////////////////////////////////
Properties* Properties::clone() const{

    Properties* props = new Properties();

    *props = *this;

    return props;
}

////////////////////////////////////////////////////////////////////////////////
void Properties::clear(){

    synchronized( &( internal->mutex ) ) {
        internal->properties.clear();
    }
}

////////////////////////////////////////////////////////////////////////////////
std::string Properties::toString() const {

    std::ostringstream stream;
    std::map< std::string, std::string >::const_iterator iter;

    stream << "Begin Class decaf::util::Properties:" << std::endl;

    synchronized( &( internal->mutex ) ) {
        for( iter = internal->properties.begin(); iter != internal->properties.end(); ++iter ){
            stream << " property[" << iter->first << "] = "
                   << iter->second << std::endl;
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
