/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "URISupport.h"

#include <activemq/exceptions/ExceptionDefines.h>

#include <decaf/util/StringTokenizer.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>
#include <decaf/lang/System.h>

using namespace std;
using namespace activemq;
using namespace activemq::util;
using namespace decaf::util;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
void URISupport::parseURL( const std::string& URI,
                           decaf::util::Properties& properties )
    throw ( decaf::lang::exceptions::IllegalArgumentException ) {

    try{

        StringTokenizer tokenizer( URI, ":/" );

        std::vector<std::string> tokens;

        // Require that there be three tokens at the least, these are
        // transport, url, port.
        if( tokenizer.countTokens() < 3 ) {
            throw decaf::lang::exceptions::IllegalArgumentException(
                __FILE__, __LINE__,
                (string( "URISupport::parseURL - "
                         "Marlformed URI: ") + URI).c_str() );
        }

        // First element should be the Transport Type, following that is the
        // URL and any params.
        properties.setProperty( "transport.protocol", tokenizer.nextToken() );

        // Parse URL and Port as one item, optional params follow the ?
        // and then each param set is delimited with & we extract first
        // three chars as they are the left over ://
        properties.setProperty( "transport.uri", tokenizer.nextToken("&?").substr(3) );

        // Now get all the optional parameters and store them as properties
        int count = tokenizer.toArray( tokens );

        for( int i = 0; i < count; ++i ) {
            tokenizer.reset( tokens[i], "=" );

            if( tokenizer.countTokens() != 2 ) {
                throw decaf::lang::exceptions::IllegalArgumentException(
                    __FILE__, __LINE__,
                    ( string( "URISupport::parseURL - "
                              "Marlformed Parameter = " ) + tokens[i] ).c_str() );
            }

            // Get them in order, passing both as nextToken calls in the
            // set Property can cause reversed order.
            string key = tokenizer.nextToken();
            string value = URISupport::replaceEnvValues( tokenizer.nextToken() );

            // Store this param as a property
            properties.setProperty( key, value );
        }
    }
    AMQ_CATCH_RETHROW( IllegalArgumentException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, IllegalArgumentException )
    AMQ_CATCHALL_THROW( IllegalArgumentException )
}

////////////////////////////////////////////////////////////////////////////////
Properties URISupport::parseQuery( std::string query )
    throw ( IllegalArgumentException ) {

    try {

        Properties options;
        URISupport::parseQuery( query, &options );
        return options;
    }
    AMQ_CATCH_RETHROW( IllegalArgumentException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, IllegalArgumentException )
    AMQ_CATCHALL_THROW( IllegalArgumentException )
}

////////////////////////////////////////////////////////////////////////////////
void URISupport::parseQuery( std::string query,
                             Properties* properties )
    throw ( IllegalArgumentException ) {

    try {

        if( properties == NULL ) {
            throw IllegalArgumentException(
                __FILE__,
                __LINE__,
                "URISupport::parseQuery - Can't pass in a null properties object" );
        }

        // strip the initial "?"
        size_t pos = query.find_first_of( "?" );
        if( pos != std::string::npos ) {
            query = query.substr(pos+1);
        }

        // split the query into parameters
        StringTokenizer tokenizer( query, "&" );
        std::vector<std::string> options;
        tokenizer.toArray( options );

        std::vector<std::string>::const_iterator iter = options.begin();

        for( ; iter != options.end(); ++iter ) {

            tokenizer.reset( *iter, "=" );

            std::string key = "";
            std::string value = "";

            if( tokenizer.countTokens() != 2 ) {
                throw IllegalArgumentException(
                    __FILE__,
                    __LINE__,
                    "URISupport::parseQuery - Invalid URI Option." );
            }

            // Get the Key
            if( tokenizer.hasMoreTokens() != false ) {
                key = tokenizer.nextToken();
            }

            // Get the Value
            if( tokenizer.hasMoreTokens() != false ) {
                value = URISupport::replaceEnvValues( tokenizer.nextToken() );
            }

            // Store them.
            properties->setProperty( key, value );
        }
    }
    AMQ_CATCH_RETHROW( IllegalArgumentException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, IllegalArgumentException )
    AMQ_CATCHALL_THROW( IllegalArgumentException )
}

////////////////////////////////////////////////////////////////////////////////
std::string URISupport::replaceEnvValues( const std::string& value )
    throw ( decaf::lang::exceptions::IllegalArgumentException ) {

    try {

        // If it matches the first env var indicator then we validate that it is
        // surrounded by an { and } bracket.  once done we remove the inner value
        // and look up the env var.
        if( value.at(0) == '$' ) {

            if( value.size() > 3 && value.at(1) != '{' && value.at( value.size() - 1 ) != '}' ) {
                throw new decaf::lang::exceptions::IllegalArgumentException(
                    __FILE__, __LINE__,
                    "URISupport::replaceEnvValues - Invalid Env Var Syntax: %s",
                    value.c_str() );
            }

            string var = value.substr( 2, value.size() - 3 );
            var = decaf::lang::System::getenv( var );

            if( var == "" ) {
                throw new decaf::lang::exceptions::IllegalArgumentException(
                    __FILE__, __LINE__,
                    "URISupport::replaceEnvValues - Env Var not set: %s",
                    value.c_str() );
            }

            return var;
        }

        return value;
    }
    AMQ_CATCH_RETHROW( IllegalArgumentException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, IllegalArgumentException )
    AMQ_CATCHALL_THROW( IllegalArgumentException )
}
