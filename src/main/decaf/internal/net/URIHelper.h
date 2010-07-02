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

#ifndef _DECAF_INTERNAL_NET_URIHELPER_H_
#define _DECAF_INTERNAL_NET_URIHELPER_H_

#include <string>
#include <decaf/util/Config.h>
#include <decaf/net/URISyntaxException.h>
#include <decaf/internal/net/URIType.h>

namespace decaf {
namespace internal {
namespace net {

    /**
     * Helper class used by the URI classes in encoding and decoding of URI's.
     */
    class DECAF_API URIHelper {
    private:

        const std::string unreserved;
        const std::string punct;
        const std::string reserved;
        const std::string someLegal;
        const std::string allLegal;

    public:

        /**
         * Setup the URIHelper with values assigned to the various fields that
         * are used in the validation process.  The defaults are overridden by
         * these values.
         *
         * @param unreserved - characters not reserved for use.
         * @param punct - allowable punctuation symbols.
         * @param reserved - characters not allowed for general use in the URI.
         * @param someLegal - characters that are legal in certain cases.
         * @param allLegal - characters that are always legal.
         */
        URIHelper( const std::string& unreserved,
                   const std::string& punct,
                   const std::string& reserved,
                   const std::string& someLegal,
                   const std::string& allLegal );

        /**
         * Sets up the filter strings with sane defaults.
         */
        URIHelper();

        virtual ~URIHelper() {}

        /**
         * Parse the passed in URI.
         * @param uri - the URI to Parse
         * @param forceServer - if true invalid URI data throws an Exception
         * @returns a URIType instance containing the parsed data.
         * @throws URISyntaxException if forceServer is true and the URI is invalid.
         */
        URIType parseURI( const std::string& uri, bool forceServer );

        /**
         * Validate the schema portin of the URI.
         * @param uri - the URI to check.
         * @param scheme - the schema section of the URI.
         * @param index - index in uri where schema starts.
         * @throw URISyntaxException if the fragment has errors.
         */
        void validateScheme( const std::string& uri, const std::string& scheme, int index );

        /**
         * Validate that the URI Ssp Segment contains no invalid encodings.
         * @param uri - the full uri.
         * @param ssp - the SSP to check.
         * @param index - position in the uri where Ssp starts.
         * @throw URISyntaxException if the fragment has errors.
         */
        void validateSsp( const std::string& uri, const std::string& ssp,
                          std::size_t index );

        /**
         * Validate that the URI Authority Segment contains no invalid encodings.
         * @param uri - the full uri.
         * @param authority - the Authority to check.
         * @param index - position in the uri where Authority starts.
         * @throw URISyntaxException if the fragment has errors.
         */
        void validateAuthority( const std::string& uri, const std::string& authority,
                                std::size_t index );

        /**
         * Validate that the URI Path Segment contains no invalid encodings.
         * @param uri - the full uri.
         * @param path - the path to check.
         * @param index - position in the uri where path starts.
         * @throw URISyntaxException if the fragment has errors.
         */
        void validatePath( const std::string& uri, const std::string& path,
                           std::size_t index );

        /**
         * Validate that the URI Query Segment contains no invalid encodings.
         * @param uri - the full uri.
         * @param query - the query to check.
         * @param index - position in the uri where fragment starts.
         * @throw URISyntaxException if the fragment has errors.
         */
        void validateQuery( const std::string& uri, const std::string& query,
                            std::size_t index );

        /**
         * Validate that the URI fragment contains no invalid encodings.
         * @param uri - the full uri.
         * @param fragment - the fragment to check.
         * @param index - position in the uri where fragment starts.
         * @throw URISyntaxException if the fragment has errors.
         */
        void validateFragment( const std::string& uri, const std::string& fragment,
                               std::size_t index );

        /**
         * determine the host, port and user-info if the authority parses
         * successfully to a server based authority
         * <p>
         * behavior in error cases: if forceServer is true, throw
         * URISyntaxException with the proper diagnostic messages. if
         * forceServer is false assume this is a registry based uri, and just
         * return leaving the host, port and user-info fields undefined.
         * <p>
         * and there are some error cases where URISyntaxException is thrown
         * regardless of the forceServer parameter e.g. mal-formed ipv6 address
         * <p>
         * @param forceServer
         * @param authority
         * @returns a URIType instance containing the parsed data.
         * @throw URISyntaxException
         */
        URIType parseAuthority( bool forceServer, const std::string& authority );

        /**
         * Check the supplied user info for validity.
         * @param uri - the uri to parse.
         * @param userinfo - supplied user info
         * @param index - index into the URI string where the data is located.
         * @return true if valid
         * @throw URISyntaxException if an error occurs
         */
        void validateUserinfo( const std::string& uri, const std::string& userinfo, std::size_t index );

        /**
         * distinguish between IPv4, IPv6, domain name and validate it based on
         * its type
         * @param forceServer - true if the forceServer mode should be active.
         * @param host - Host string to validate.
         * @return true if the host value if a valid domain name.
         * @throws URISyntaxException if the host is invalid and forceServer is true.
         */
        bool isValidHost( bool forceServer, const std::string& host );

        /**
         * Validates the string past to determine if it is a well formed
         * domain name.
         * @param host - domain name to validate.
         * @return true if host is well formed.
         */
        bool isValidDomainName( const std::string& host );

        /**
         * Validate if the host value is a well formed IPv4 address, this is
         * the form XXX.XXX.XXX.XXX were X is any number 0-9. and XXX is not
         * greater than 255.
         * @param host - IPv4 address string to parse.
         * @return true if host is a well formed IPv4 address.
         */
        bool isValidIPv4Address( const std::string& host );

        /**
         * Determines if the given address is valid according to the IPv6 spec.
         * @param ipAddress - string ip address value to validate.
         * @return true if the address string is valid.
         */
        bool isValidIP6Address( const std::string& ipAddress );

        /**
         * Check is the string passed contains a Valid IPv4 word, which is
         * an integer in the range of 0 to 255.
         * @param word - string value to check.
         * @return true if the word is a valid IPv4 word.
         */
        bool isValidIP4Word( const std::string& word );

        /**
         * Determines if the given char is a valid Hex char.  Valid
         * chars are A-F (upper or lower case) and 0-9.
         * @param c - char to inspect
         * @return true if c is a valid hex char.
         */
        bool isValidHexChar( char c );

    };

}}}

#endif /* _DECAF_INTERNAL_NET_URIHELPER_H_ */
