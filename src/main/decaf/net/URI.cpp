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

#include "URI.h"

#include <apr_strings.h>
#include <decaf/lang/Integer.h>

using namespace decaf;
using namespace decaf::net;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
const std::string URI::unreserved = "_-!.~\'()*";
const std::string URI::punct = ",;:$&+=";
const std::string URI::reserved = punct + "?/[]@";
const std::string URI::someLegal = unreserved + punct;
const std::string URI::allLegal = unreserved + reserved;

////////////////////////////////////////////////////////////////////////////////
URI::URI( const std::string& uri ) throw ( URISyntaxException) {

    this->uriString = NULL;
    this->parseURI( uri );
}

////////////////////////////////////////////////////////////////////////////////
URI::URI( const std::string& scheme,
          const std::string& ssp,
          const std::string& fragment ) throw ( URISyntaxException ) {

    std::string uri = "";

    if( scheme != "" ) {
        uri.append( scheme );
        uri.append( ":" );
    }

    if( ssp != "" ) {
        // QUOTE ILLEGAL CHARACTERS
        uri.append( quoteComponent( ssp, allLegal ) );
    }

    if( fragment != "" ) {
        uri.append( "#" );
        // QUOTE ILLEGAL CHARACTERS
        uri.append( quoteComponent( fragment, allLegal ) );
    }

    // Now hand of to the main parse function.
    this->parseURI( uri );
}

////////////////////////////////////////////////////////////////////////////////
URI::URI( const std::string& scheme, const std::string& userInfo,
          const std::string& host, int port,
          const std::string& path, const std::string& query,
          const std::string& fragment ) throw ( URISyntaxException ) {

    this->uriString = NULL;

    if( scheme == "" && userInfo == "" && host == "" &&
        path == "" && query == "" && fragment == "" ) {

        this->uri.path = "";
        return;
    }

    if( scheme != "" && path.length() > 0 && path.at(0) != '/') {

        throw URISyntaxException(
            __FILE__, __LINE__, path,
            "URI::URI - Path string: %s starts with invalid char '/'" );
    }

    std::string uri = "";
    if( scheme != "" ) {
        uri.append( scheme );
        uri.append( ":" );
    }

    if( userInfo != "" || host != "" || port != -1 ) {
        uri.append( "//" );
    }

    if( userInfo != "" ) {
        // QUOTE ILLEGAL CHARACTERS in userinfo
        uri.append(quoteComponent( userInfo, someLegal ) );
        uri.append( "@" );
    }

    if( host != "" ) {
        std::string newHost = host;

        // check for ipv6 addresses that hasn't been enclosed
        // in square brackets
        if( host.find( ":" ) != std::string::npos &&
            host.find( "]" ) == std::string::npos &&
            host.find( "[" ) == std::string::npos ) {

            newHost = std::string( "[" ) + host + "]";
        }

        uri.append( newHost );
    }

    if( port != -1 ) {
        uri.append( ":" );
        uri.append( Integer::toString( port ) );
    }

    if( path != "" ) {
        // QUOTE ILLEGAL CHARS
        uri.append( quoteComponent( path, "/@" + someLegal ) );
    }

    if( query != "" ) {
        uri.append( "?" );
        // QUOTE ILLEGAL CHARS
        uri.append( quoteComponent( query, allLegal ) );
    }

    if( fragment != "" ) {
        // QUOTE ILLEGAL CHARS
        uri.append( "#" );
        uri.append( quoteComponent( fragment, allLegal ) );
    }

    this->parseURI( uri );
}

////////////////////////////////////////////////////////////////////////////////
URI::URI( const std::string& scheme, const std::string& host,
          const std::string& path, const std::string& fragment )
            throw ( URISyntaxException ) {

    this->uriString = NULL;

    URI::URI( scheme, "", host, -1, path, "", fragment );
}

////////////////////////////////////////////////////////////////////////////////
URI::URI( const std::string& scheme, const std::string& authority,
          const std::string& path, const std::string& query,
          const std::string& fragment ) throw ( URISyntaxException ) {

    this->uriString = NULL;

    if( scheme != "" && path.length() > 0 && path.at(0) != '/' ) {
         throw URISyntaxException(
            __FILE__, __LINE__, path,
            "URI::URI - Path String %s must start with a '/'" );
     }

     std::string uri = "";
     if( scheme != "" ) {
         uri.append( scheme );
         uri.append( ":" );
     }
     if( authority != "" ) {
         uri.append("//");
         // QUOTE ILLEGAL CHARS
         uri.append( quoteComponent( authority, "@[]" + someLegal ) );
     }

     if( path != "" ) {
         // QUOTE ILLEGAL CHARS
         uri.append( quoteComponent( path, "/@" + someLegal ) );
     }
     if( query != "" ) {
         // QUOTE ILLEGAL CHARS
         uri.append( "?" );
         uri.append( quoteComponent( query, allLegal ) );
     }
     if( fragment != "" ) {
         // QUOTE ILLEGAL CHARS
         uri.append( "#" );
         uri.append( quoteComponent( fragment, allLegal ) );
     }

     this->parseURI( uri );
}

////////////////////////////////////////////////////////////////////////////////
void URI::parseURI( const std::string& uri ) throw ( URISyntaxException ) {

    // Use APR to perform the main parse.
    apr_status_t result = apr_uri_parse( pool.getAprPool(),
        uri.c_str(), &this->uri );

    if( result != APR_SUCCESS ) {
        throw URISyntaxException(
            __FILE__, __LINE__, uri,
            "URI::praseURI - URI String %s invalid." );
    }

    std::cout << "\n";
    std::cout << "Original URI String: " << uri << std::endl;
    std::cout << "\n";
    std::cout << "Scheme: "
              << ( this->uri.scheme ? this->uri.scheme : "" ) << std::endl;
    std::cout << "Host: "
              << ( this->uri.hostinfo ? this->uri.hostinfo : "" ) << std::endl;
    std::cout << "User: "
              << ( this->uri.user ? this->uri.user : "" ) << std::endl;
    std::cout << "Passwrod: "
              << ( this->uri.password ? this->uri.password : "" ) << std::endl;
    std::cout << "Host Name: "
              << ( this->uri.hostname ? this->uri.hostname : "" ) << std::endl;
    std::cout << "Port Str: "
              << ( this->uri.port_str ? this->uri.port_str : "" ) << std::endl;
    std::cout << "Path: "
              << ( this->uri.path ? this->uri.path : "" ) << std::endl;
    std::cout << "Query: "
              << ( this->uri.query ? this->uri.query : "" ) << std::endl;
    std::cout << "Fragment: "
              << ( this->uri.fragment ? this->uri.fragment : "" ) << std::endl;
    std::cout << "Port: " << this->uri.port << std::endl;
    std::cout << "Is Initialized: " << this->uri.is_initialized << std::endl;
    std::cout << "DNS Looked Up: " << this->uri.dns_looked_up << std::endl;
    std::cout << "DNS Resolved: " << this->uri.dns_resolved << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
int URI::compareTo( DECAF_UNUSED const URI& value ) const {
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
bool URI::equals( const URI& value ) const {
    return compareTo( value ) == 0 ? true : false;
}

////////////////////////////////////////////////////////////////////////////////
bool URI::operator==( const URI& value ) const {
    return compareTo( value ) == 0 ? true : false;
}

////////////////////////////////////////////////////////////////////////////////
bool URI::operator<( const URI& value ) const {
    return compareTo( value ) == -1 ? true : false;
}

////////////////////////////////////////////////////////////////////////////////
URI URI::create( const std::string uri )
    throw ( lang::exceptions::IllegalArgumentException ) {

    try {
        return URI( uri );
    } catch( URISyntaxException& e ) {
        throw IllegalArgumentException( e );
    }
}

////////////////////////////////////////////////////////////////////////////////
std::string URI::quoteComponent( DECAF_UNUSED const std::string& component,
        DECAF_UNUSED const std::string& legalset ) {
//    try {
        /*
         * Use a different encoder than URLEncoder since: 1. chars like "/",
         * "#", "@" etc needs to be preserved instead of being encoded, 2.
         * UTF-8 char set needs to be used for encoding instead of default
         * platform one
         */
//        return URIEncoderDecoder.quoteIllegal(component, legalset);
//    } catch( UnsupportedEncodingException e ) {
//        throw RuntimeException( e );
//    }
    
    return "";
}
