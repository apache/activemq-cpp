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

#include "URITest.h"

#include <decaf/net/URI.h>

using namespace std;
using namespace decaf;
using namespace decaf::net;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
URITest::URITest() {
}

////////////////////////////////////////////////////////////////////////////////
void URITest::test_Constructor_String() {

//    const std::string constructorTests[27] = {
//        "http://user@www.google.com:45/search?q=helpinfo#somefragment",
//        // http with authority, query and fragment
//        "ftp://ftp.is.co.za/rfc/rfc1808.txt", // ftp
//        "gopher://spinaltap.micro.umn.edu/00/Weather/California/Los%20Angeles",
//        // gopher
//        "mailto:mduerst@ifi.unizh.ch", // mailto
//        "news:comp.infosystems.www.servers.unix", // news
//        "telnet://melvyl.ucop.edu/", // telnet
//        "http://123.24.17.98/test", // IPv4 authority
//        "http://www.google.com:80/test",// domain name authority
//        "http://joe@[3ffe:2a00:100:7031::1]:80/test",
//        // IPv6 authority, with userinfo and port
//        "/relative", // relative starting with /
//        "//relative", // relative starting with //
//        "relative", // relative with no /
//        "#fragment",// relative just with fragment
//        "http://user@host:80", // UI, host,port
//        "http://user@host", // ui, host
//        "http://host", // host
//        "http://host:80", // host,port
//        "http://joe@:80", // ui, port (becomes registry-based)
//        "file:///foo/bar", // empty authority, non empty path
//        "ht?tp://hoe@host:80", // miscellaneous tests
//        "mai/lto:hey?joe#man", "http://host/a%20path#frag",
//        // path with an escaped octet for space char
//        "http://host/a%E2%82%ACpath#frag",
//        // path with escaped octet for unicode char, not USASCII
//        "http://host/a\u20ACpath#frag",
//        // path with unicode char, not USASCII equivalent to
//        // = "http://host/a\u0080path#frag",
//        "http://host%20name/", // escaped octets in host (becomes
//        // registry based)
//        "http://host\u00DFname/", // unicodechar in host (becomes
//        // registry based)
//        // equivalent to = "http://host\u00dfname/",
//        "ht123-+tp://www.google.com:80/test", // legal chars in scheme
//    };
//
//    for( int i = 0; i < 27; i++ ) {
//        try {
//            new URI(constructorTests[i]);
//        } catch ( URISyntaxException e ) {
//            CPPUNIT_FAIL( string( "Failed to construct URI for: " ) +
//                          constructorTests[i] + " : " +
//                          e.getMessage() );
//        }
//    }

    std::vector<const char*> constructorTestsInvalid;
    // space char in path, not in escaped
    constructorTestsInvalid.push_back( "http:///a path#frag" );
    // octet form, with no host
    constructorTestsInvalid.push_back( "http://host/a[path#frag" );
    // an illegal char, not in escaped octet form, should throw an exception
    // invalid escape sequence in path
    constructorTestsInvalid.push_back( "http://host/a%path#frag" );
    // incomplete escape sequence in path
    constructorTestsInvalid.push_back( "http://host/a%#frag" );
    // space char in fragment, not in
    constructorTestsInvalid.push_back( "http://host#a frag" );
    // escaped octet form, no path
    // illegal char in fragment
    constructorTestsInvalid.push_back( "http://host/a#fr#ag" );
    // invalid escape sequence in fragment,
    constructorTestsInvalid.push_back( "http:///path#fr%ag" );
    // with no host
    // incomplete escape sequence in fragment
    constructorTestsInvalid.push_back( "http://host/path#frag%" );
    // space char in query, not in escaped octet form
    constructorTestsInvalid.push_back( "http://host/path?a query#frag" );
    // invalid escape sequence in query, no path
    constructorTestsInvalid.push_back( "http://host?query%ag" );
    // incomplete escape sequence in query, with no host
    constructorTestsInvalid.push_back( "http:///path?query%" );
    // invalid char in scheme specific part
    constructorTestsInvalid.push_back( "mailto:user^name@fklkf.com"  );

//    for( size_t i = 0; i < constructorTestsInvalid.size(); i++ ) {
//        try {
//            new URI( constructorTestsInvalid[i] );
//            CPPUNIT_FAIL( string( "Failed to throw URISyntaxException for: " ) +
//                          constructorTestsInvalid[i] );
//        } catch( URISyntaxException e ) {}
//    }

//    std::string invalid2[18] = {
//        // authority validation
//        "http://user@[3ffe:2x00:100:7031::1]:80/test", // malformed
//        // IPv6 authority
//        "http://[ipv6address]/apath#frag", // malformed ipv6 address
//        "http://[ipv6address/apath#frag", // malformed ipv6 address
//        "http://ipv6address]/apath#frag", // illegal char in host name
//        "http://ipv6[address/apath#frag",
//        "http://ipv6addr]ess/apath#frag",
//        "http://ipv6address[]/apath#frag",
//        // illegal char in username...
//        "http://us[]er@host/path?query#frag", "http://host name/path", // illegal
//        // char
//        // in
//        // authority
//        "http://host^name#fragment", // illegal char in authority
//        "telnet://us er@hostname/", // illegal char in authority
//        // missing components
//        "//", // Authority expected
//        "ascheme://", // Authority expected
//        "ascheme:", // Scheme-specific part expected
//        // scheme validation
//        "a scheme://reg/", // illegal char
//        "1scheme://reg/", // non alpha char as 1st char
//        "asche\u00dfme:ssp", // unicode char , not USASCII
//        "asc%20heme:ssp" // escape octets
//    };
//
//    for( int i = 0; i < 18; i++ ) {
//        try {
//            new URI( invalid2[i] );
//            CPPUNIT_FAIL(
//                string( "Failed to throw URISyntaxException for: " ) + invalid2[i] );
//        } catch( URISyntaxException e ) {}
//    }
}
