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

using namespace activemq;
using namespace activemq::util;
using namespace decaf::util;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

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
                value = tokenizer.nextToken();
            }

            // Store them.
            properties->setProperty( key, value );
        }
    }
    AMQ_CATCH_RETHROW( IllegalArgumentException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, IllegalArgumentException )
    AMQ_CATCHALL_THROW( IllegalArgumentException )
}
