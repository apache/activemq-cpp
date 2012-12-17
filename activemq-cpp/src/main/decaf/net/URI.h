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

#ifndef _DECAF_NET_URI_H_
#define _DECAF_NET_URI_H_

#include <decaf/util/Config.h>
#include <decaf/lang/Comparable.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>
#include <decaf/net/URISyntaxException.h>
#include <decaf/net/MalformedURLException.h>
#include <decaf/net/URL.h>
#include <decaf/internal/net/URIType.h>
#include <string>

namespace decaf{
namespace net{

    /**
     * This class represents an instance of a URI as defined by RFC 2396.
     */
    class DECAF_API URI : public lang::Comparable<URI> {
    private:

        // The structure that holds the parsed URI data.
        decaf::internal::net::URIType uri;

        // The original string entered from URI( string ), empty if not set.
        mutable std::string uriString;

        static const std::string unreserved;
        static const std::string punct;
        static const std::string reserved;
        static const std::string someLegal;
        static const std::string allLegal;

    public:

        /**
         * Default Constructor, same as calling a Constructor with all fields empty.
         */
        URI();

        /**
         * Constructs a URI as a copy of another URI
         *
         * @param uri - uri to copy
         *
         * @throws URISyntaxException if the URI passed is malformed.
         */
        URI( const URI& uri );

        /**
         * Constructs a URI from the given string.
         *
         * @param uri - string uri to parse.
         *
         * @throws URISyntaxException if the URI passed is malformed.
         */
        URI( const std::string& uri );

        /**
         * Constructs a URI from the given components.
         *
         * @param scheme - the uri scheme
         * @param ssp - Scheme specific part
         * @param fragment - Fragment
         *
         * @throws URISyntaxException if the URI passed is malformed.
         */
        URI( const std::string& scheme, const std::string& ssp, const std::string& fragment );

        /**
         * Constructs a URI from the given components.
         *
         * @param scheme - Scheme name
         * @param userInfo - User name and authorization information
         * @param host - Host name
         * @param port - Port number
         * @param path - Path
         * @param query - Query
         * @param fragment - Fragment
         *
         * @throws URISyntaxException if the URI passed is malformed.
         */
        URI( const std::string& scheme, const std::string& userInfo,
             const std::string& host, int port,
             const std::string& path, const std::string& query,
             const std::string& fragment );

        /**
         * Constructs a URI from the given components.
         * @param scheme - Scheme name
         * @param host - Host name
         * @param path - Path
         * @param fragment - Fragment
         *
         * @throws URISyntaxException if the URI passed is malformed.
         */
        URI( const std::string& scheme, const std::string& host,
             const std::string& path, const std::string& fragment );

        /**
         * Constructs a URI from the given components.
         * @param scheme - Scheme name
         * @param authority - Authority
         * @param path - Path
         * @param query - Query
         * @param fragment - Fragment
         *
         * @throws URISyntaxException if the URI passed is malformed.
         */
        URI( const std::string& scheme, const std::string& authority,
             const std::string& path, const std::string& query,
             const std::string& fragment );

        virtual ~URI() {}

        /**
         * Compares this object with the specified object for order. Returns a
         * negative integer, zero, or a positive integer as this object is less
         * than, equal to, or greater than the specified object.
         * @param value - the value to compare to this one.
         * @returns zero if equal minus one if less than and one if greater than.
         */
        virtual int compareTo( const URI& value ) const;

        /**
         * @return true if this value is considered equal to the passed value.
         */
        virtual bool equals( const URI& value ) const;

        /**
         * Compares equality between this object and the one passed.
         * @param value - the value to be compared to this one.
         * @return true if this object is equal to the one passed.
         */
        virtual bool operator==( const URI& value ) const;

        /**
         * Compares this object to another and returns true if this object
         * is considered to be less than the one passed.  This
         * @param value - the value to be compared to this one.
         * @return true if this object is equal to the one passed.
         */
        virtual bool operator<( const URI& value ) const;

        /**
         * @returns the decoded authority component of this URI.
         */
        std::string getAuthority() const;

        /**
         * @returns the decoded fragment component of this URI.
         */
        std::string getFragment() const;

        /**
         * @returns the host component of this URI.
         */
        std::string getHost() const;

        /**
         * @returns the path component of this URI.
         */
        std::string getPath() const;

        /**
         * @returns the port component of this URI.
         */
        int getPort() const;

        /**
         * @returns the query component of this URI.
         */
        std::string getQuery() const;

        /**
         * @returns the scheme component of this URI
         */
        std::string getScheme() const;

        /**
         * @returns the user info component of this URI
         */
        std::string getUserInfo() const;

        /**
         * Returns the raw authority component of this URI.
         *
         * The authority component of a URI, if defined, only contains the
         * commercial-at character ('@') and characters in the unreserved, punct,
         * escaped, and other categories. If the authority is server-based then
         * it is further constrained to have valid user-information, host, and
         * port components.
         * @returns the raw authority component of the URI
         */
        std::string getRawAuthority() const;

        /**
         * Returns the raw fragment component of this URI.
         *
         * The fragment component of a URI, if defined, only contains legal URI
         * characters.
         * @returns the raw fragment component of this URI
         */
        std::string getRawFragment() const;

        /**
         * Returns the raw path component of this URI.
         *
         * The path component of a URI, if defined, only contains the slash
         * character ('/'), the commercial-at character ('@'), and characters
         * in the unreserved, punct, escaped, and other categories.
         * @returns the raw path component of this URI
         */
        std::string getRawPath() const;

        /**
         * Returns the raw query component of this URI.
         * The query component of a URI, if defined, only contains legal URI characters.
         * @returns the raw query component of the URI.
         */
        std::string getRawQuery() const;

        /**
         * Returns the raw scheme-specific part of this URI. The scheme-specific
         * part is never undefined, though it may be empty.
         * The scheme-specific part of a URI only contains legal URI characters.
         * @returns the raw scheme special part of the uri
         */
        std::string getRawSchemeSpecificPart() const;

        /**
         * Returns the decoded scheme-specific part of this URI.
         * The string returned by this method is equal to that returned by the
         * getRawSchemeSpecificPart method except that all sequences of escaped
         * octets are decoded.
         * @returns the raw scheme specific part of the uri.
         */
        std::string getSchemeSpecificPart() const;

        /**
         * Returns the raw user-information component of this URI.
         * The user-information component of a URI, if defined, only contains
         * characters in the unreserved, punct, escaped, and other categories.
         * @returns the raw user-information component of the URI
         */
        std::string getRawUserInfo() const;

        /**
         * Tells whether or not this URI is absolute.  A URI is absolute if,
         * and only if, it has a scheme component.
         * @returns true if, and only if, this URI is absolute
         */
        bool isAbsolute() const;

        /**
         * Tells whether or not this URI is opaque.  A URI is opaque if, and only
         * if, it is absolute and its scheme-specific part does not begin with a
         * slash character ('/'). An opaque URI has a scheme, a scheme-specific
         * part, and possibly a fragment; all other components are undefined.
         * @returns true if, and only if, this URI is opaque
         */
        bool isOpaque() const;

        /**
         * Normalizes this URI's path.  If this URI is opaque, or if its path is
         * already in normal form, then this URI is returned. Otherwise a new URI
         * is constructed that is identical to this URI except that its path is
         * computed by normalizing this URI's path in a manner consistent with
         * RFC 2396, section 5.2, step 6, sub-steps c through f; that is:
         *
         *  1. All "." segments are removed.
         *  2. If a ".." segment is preceded by a non-".." segment then both of
         *     these segments are removed. This step is repeated until it is no
         *     longer applicable.
         *  3. If the path is relative, and if its first segment contains a colon
         *     character (':'), then a "." segment is prepended. This prevents a
         *     relative URI with a path such as "a:b/c/d" from later being re-parsed
         *     as an opaque URI with a scheme of "a" and a scheme-specific part of
         *     "b/c/d". (Deviation from RFC 2396)
         *
         * A normalized path will begin with one or more ".." segments if there were
         * insufficient non-".." segments preceding them to allow their removal. A
         * normalized path will begin with a "." segment if one was inserted by step
         * 3 above. Otherwise, a normalized path will not contain any "." or ".."
         * segments.
         * @returns A URI equivalent to this URI, but whose path is in normal form
         */
        URI normalize() const;

        /**
         * Attempts to parse this URI's authority component, if defined, into
         * user-information, host, and port components.
         *
         * If this URI's authority component has already been recognized as being
         * server-based then it will already have been parsed into user-information,
         * host, and port components. In this case, or if this URI has no authority
         * component, this method simply returns this URI.
         *
         * Otherwise this method attempts once more to parse the authority component
         * into user-information, host, and port components, and throws an exception
         * describing why the authority component could not be parsed in that way.
         * @returns A URI whose authority field has been parsed as a server-based
         * authority
         * @throws URISyntaxException - If the authority component of this URI is
         * defined but cannot be parsed as a server-based authority.
         */
        URI parseServerAuthority() const;

        /**
         * Relativizes the given URI against this URI.  The relativization of the
         * given URI against this URI is computed as follows:
         *
         *  1. If either this URI or the given URI are opaque, or if the scheme and
         *     authority components of the two URIs are not identical, or if the path
         *     of this URI is not a prefix of the path of the given URI, then the
         *     given URI is returned.
         *  2. Otherwise a new relative hierarchical URI is constructed with query
         *     and fragment components taken from the given URI and with a path
         *     component computed by removing this URI's path from the beginning of
         *     the given URI's path.
         *
         * @param uri - The URI to be relativized against this URI
         * @returns The resulting URI
         */
        URI relativize( const URI& uri ) const;

        /**
         * Constructs a new URI by parsing the given string and then resolving it
         * against this URI.
         *
         * This convenience method works as if invoking it were equivalent to
         * evaluating the expression resolve( URI::create( str ) ).
         * @param str - The string to be parsed into a URI
         * @returns The resulting URI
         * @throws IllegalArgumentException - If the given string violates RFC 2396
         */
        URI resolve( const std::string& str ) const;

        /**
         * Resolves the given URI against this URI.
         *
         * If the given URI is already absolute, or if this URI is opaque, then a
         * copy of the given URI is returned.
         *
         * If the given URI's fragment component is defined, its path component is
         * empty, and its scheme, authority, and query components are undefined, then
         * a URI with the given fragment but with all other components equal to those
         * of this URI is returned. This allows a URI representing a standalone
         * fragment reference, such as "#foo", to be usefully resolved against a base
         * URI.
         *
         * Otherwise this method constructs a new hierarchical URI in a manner
         * consistent with RFC 2396, section 5.2; that is:
         *
         *  1. A new URI is constructed with this URI's scheme and the given URI's
         *     query and fragment components.
         *  2. If the given URI has an authority component then the new URI's authority
         *     and path are taken from the given URI.
         *  3. Otherwise the new URI's authority component is copied from this URI,
         *     and its path is computed as follows:
         *
         *     1. If the given URI's path is absolute then the new URI's path is
         *        taken from the given URI.
         *     2. Otherwise the given URI's path is relative, and so the new URI's
         *        path is computed by resolving the path of the given URI against the
         *        path of this URI. This is done by concatenating all but the last
         *        segment of this URI's path, if any, with the given URI's path and
         *        then normalizing the result as if by invoking the normalize method.
         *
         * The result of this method is absolute if, and only if, either this URI is
         * absolute or the given URI is absolute.
         * @param uri - The URI to be resolved against this URI
         * @returns The resulting URI
         */
        URI resolve( const URI& uri ) const;

        /**
         * Returns the content of this URI as a string.
         *
         * If this URI was created by invoking one of the constructors in this class
         * then a string equivalent to the original input string, or to the string
         * computed from the originally-given components, as appropriate, is returned.
         * Otherwise this URI was created by normalization, resolution, or
         * relativization, and so a string is constructed from this URI's components
         * according to the rules specified in RFC 2396, section 5.2, step 7.
         * @returns the string form of this URI
         */
        std::string toString() const;

        /**
         * Constructs a URL from this URI.
         *
         * This convenience method works as if invoking it were equivalent to
         * evaluating the expression new URL(this.toString()) after first checking
         * that this URI is absolute.
         * @returns A URL constructed from this URI
         * @throws IllegalArgumentException - If this URL is not absolute
         * @throws MalformedURLException - If a protocol handler for the URL could not
         * be found, or if some other error occurred while constructing the URL
         */
        URL toURL() const;

    public:   // Static Methods

        /**
         * Creates a URI by parsing the given string.
         * This convenience factory method works as if by invoking the URI(string)
         * constructor; any URISyntaxException thrown by the constructor is caught
         * and wrapped in a new IllegalArgumentException object, which is then thrown.
         *
         * @param uri - URI string to parse
         *
         * @throws IllegalArgumentException
         */
        static URI create( const std::string uri );

    private:

        /**
         * Parses a URI string and fills in the member data, throws a
         * URISyntaxException if things fail.
         *
         * @param uri - the URI to parse
         * @param forceServer - should a server authority be enforced.
         * @throws URISyntaxException if an error occurs.
         */
        void parseURI( const std::string& uri, bool forceServer );

        /*
         * Quote illegal chars for each component, but not the others
         *
         * @param component the component to be converted
         * @param legalset the legal character set allowed in the component strng
         * @return the converted string
         */
        std::string quoteComponent( const std::string& component,
                                    const std::string& legalset );

        /*
         * Encode Unicode chars that are not part of US-ASCII char set into the
         * escaped form
         *
         * i.e. The Euro currency symbol is encoded as "%E2%82%AC".
         *
         * @param src the string to be encoded
         * @return the converted string
         */
        std::string encodeOthers( const std::string& src ) const;

        /**
         * Decode an encoded URI String.
         *
         * @param src - the encoded string
         * @return the unencoded string version of src.
         */
        std::string decode( const std::string& src ) const;

        /**
         * Compare the Two Hexadecimal encoded strings and return if they are equal.
         *
         * @param first - First String to compare.
         * @param second - The second string to compare.
         */
        bool equalsHexCaseInsensitive( const std::string& first,
                                       const std::string& second ) const;

        /*
         * Takes a string that may contain hex sequences like %F1 or %2b and
         * converts the hex values following the '%' to lowercase.
         *
         * @param s - String to convert the hex in.
         */
        std::string convertHexToLowerCase( const std::string& s ) const;

        /*
         * Normalize path, and return the resulting string.
         *
         * @param path - the path value to normalize.
         */
        std::string normalize( const std::string& path ) const;

        /**
         * Helper method used to re-calculate the scheme specific part of the
         * resolved or normalized URIs
         */
        void setSchemeSpecificPart();

    };

}}

#endif /*_DECAF_NET_URI_H_*/
