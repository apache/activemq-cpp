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

#ifndef _DECAF_INTERNAL_NET_URITYPE_H_
#define _DECAF_INTERNAL_NET_URITYPE_H_

#include <string>
#include <decaf/util/Config.h>

namespace decaf {
namespace internal {
namespace net {

    /**
     * Basic type object that holds data that composes a given URI
     */
    class DECAF_API URIType {
    private:

        std::string source;
        std::string scheme;
        std::string schemeSpecificPart;
        std::string authority;
        std::string userinfo;
        std::string host;
        int port;
        std::string path;
        std::string query;
        std::string fragment;
        bool opaque;
        bool absolute;
        bool serverAuthority;
        bool valid;

    public:

        URIType( const std::string& source ) : source( source ),
                                               scheme(),
                                               schemeSpecificPart(),
                                               authority(),
                                               userinfo(),
                                               host(),
                                               port( -1 ),
                                               path(),
                                               query(),
                                               fragment(),
                                               opaque( false ),
                                               absolute( false ),
                                               serverAuthority( false ),
                                               valid( false ) {
        }

        URIType() : source(),
                    scheme(),
                    schemeSpecificPart(),
                    authority(),
                    userinfo(),
                    host(),
                    port( -1 ),
                    path(),
                    query(),
                    fragment(),
                    opaque( false ),
                    absolute( false ),
                    serverAuthority( false ),
                    valid( false ) {
        }

        virtual ~URIType() {}

        /**
         * Gets the source URI string that was parsed to obtain this URIType
         * instance and the resulting data,
         * @returns the source URI string
         */
        std::string getSource() const {
            return this->source;
        }

        /**
         * Sets the source URI string that was parsed to obtain this URIType
         * instance and the resulting data,
         * @param source - the source URI string
         */
        void setSource( const std::string& source ) {
            this->source = source;
        }

        /**
         * Gets the Scheme of the URI, e.g. scheme ("http"/"ftp"/...).
         * @return scheme part string.
         */
        std::string getScheme() const {
            return scheme;
        }

        /**
         * Sets the Scheme of the URI, e.g. scheme ("http"/"ftp"/...).
         * @param scheme - scheme part string.
         */
        void setScheme( const std::string& scheme ) {
            this->scheme = scheme;
        }

        /**
         * Gets the Scheme Specific Part of the URI.
         * @return scheme specific part string.
         */
        std::string getSchemeSpecificPart() const {
            return schemeSpecificPart;
        }

        /**
         * Sets the Scheme Specific Part of the URI.
         * @param schemeSpecificPart - scheme specific part string.
         */
        void setSchemeSpecificPart( const std::string& schemeSpecificPart ) {
            this->schemeSpecificPart = schemeSpecificPart;
        }

        /**
         * Gets the Authority of the URI.
         * @return Authority part string.
         */
        std::string getAuthority() const {
            return authority;
        }

        /**
         * Sets the Authority of the URI.
         * @param authority Authority part string.
         */
        void setAuthority( const std::string& authority ) {
            this->authority = authority;
        }

        /**
         * Gets the user info part of the URI, e.g. user name, as in
         * http://user:passwd@host:port/
         * @return user info part string.
         */
        std::string getUserInfo() const {
            return userinfo;
        }

        /**
         * Sets the user info part of the URI, e.g. user name, as in
         * http://user:passwd@host:port/
         * @param userinfo - user info part string.
         */
        void setUserInfo( const std::string& userinfo ) {
            this->userinfo = userinfo;
        }

        /**
         * Gets the Host name part of the URI.
         * @return Host name part string.
         */
        std::string getHost() const {
            return host;
        }

        /**
         * Sets the Host name part of the URI.
         * @param host - Host name part string.
         */
        void setHost( const std::string& host ) {
            this->host = host;
        }

        /**
         * Gets the port part of the URI.
         * @return port part string, -1 if not set.
         */
        int getPort() const {
            return port;
        }

        /**
         * Sets the port part of the URI.
         * @param port - port part string, -1 if not set.
         */
        void setPort( int port ) {
            this->port = port;
        }

        /**
         * Gets the Path part of the URI.
         * @return Path part string.
         */
        std::string getPath() const {
            return path;
        }

        /**
         * Sets the Path part of the URI.
         * @param path - Path part string.
         */
        void setPath( const std::string& path ) {
            this->path = path;
        }

        /**
         * Gets the Query part of the URI.
         * @return Query part string.
         */
        std::string getQuery() const {
            return query;
        }

        /**
         * Sets the Query part of the URI.
         * @param query - Query part string.
         */
        void setQuery( const std::string& query ) {
            this->query = query;
        }

        /**
         * Gets the Fragment part of the URI.
         * @return Fragment part string.
         */
        std::string getFragment() const {
            return fragment;
        }

        /**
         * Sets the Fragment part of the URI.
         * @param fragment - Fragment part string.
         */
        void setFragment( const std::string& fragment ) {
            this->fragment = fragment;
        }

        /**
         * Gets if the URI is Opaque.
         * @return true if opaque.
         */
        bool isOpaque() const {
            return opaque;
        }

        /**
         * Sets if the URI is Opaque.
         * @param opaque true if opaque.
         */
        void setOpaque( bool opaque ) {
            this->opaque = opaque;
        }

        /**
         * Gets if the URI is Absolute.
         * @return true if Absolute.
         */
        bool isAbsolute() const {
            return absolute;
        }

        /**
         * Sets if the URI is Absolute.
         * @param absolute - true if Absolute.
         */
        void setAbsolute( bool absolute ) {
            this->absolute = absolute;
        }

        /**
         * Gets if the URI is a Server Authority.
         * @return true if Server Authority.
         */
        bool isServerAuthority() const {
            return serverAuthority;
        }

        /**
         * Sets if the URI is a Server Authority.
         * @param serverAuthority - true if Server Authority.
         */
        void setServerAuthority( bool serverAuthority ) {
            this->serverAuthority = serverAuthority;
        }

        /**
         * Gets if the URI is valid, meaning that the source has been set and
         * parsed and all relevant data fields have been set.
         * @returns true if the URIType contains valid data.
         */
        bool isValid() const {
            return valid;
        }

        /**
         * Sets if the URI is valid, meaning that the source has been set and
         * parsed and all relevant data fields have been set.
         * @param valid - true if the URIType contains valid data.
         */
        void setValid( bool valid ) {
            this->valid = valid;
        }

    };

}}}

#endif /* _DECAF_INTERNAL_NET_URITYPE_H_ */
