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

#include "URIHelperTest.h"
#include <vector>

#include <decaf/internal/net/URIHelper.h>
#include <decaf/net/URISyntaxException.h>

using namespace std;
using namespace decaf;
using namespace decaf::net;
using namespace decaf::internal;
using namespace decaf::internal::net;
using namespace decaf::lang;

////////////////////////////////////////////////////////////////////////////////
URIHelperTest::URIHelperTest() {
}

////////////////////////////////////////////////////////////////////////////////
void URIHelperTest::testParseURI() {

    std::vector<std::string> constructorTests;
    constructorTests.push_back( "http://user@www.google.com:45/search?q=helpinfo#somefragment" );
    // http with authority, query and fragment
    constructorTests.push_back( "ftp://ftp.is.co.za/rfc/rfc1808.txt" ); // ftp
    constructorTests.push_back(
        "gopher://spinaltap.micro.umn.edu/00/Weather/California/Los%20Angeles" );
    // gopher
    constructorTests.push_back( "mailto:mduerst@ifi.unizh.ch" ); // mailto
    constructorTests.push_back( "news:comp.infosystems.www.servers.unix" ); // news
    constructorTests.push_back( "telnet://melvyl.ucop.edu/" ); // telnet
    constructorTests.push_back( "http://123.24.17.98/test" ); // IPv4 authority
    constructorTests.push_back( "http://www.google.com:80/test" );// domain name authority
    constructorTests.push_back( "http://joe@[3ffe:2a00:100:7031::1]:80/test" );
    // IPv6 authority, with userinfo and port
    constructorTests.push_back( "/relative" ); // relative starting with /
    constructorTests.push_back( "//relative" ); // relative starting with //
    constructorTests.push_back( "relative" ); // relative with no /
    constructorTests.push_back( "#fragment" );// relative just with fragment
    constructorTests.push_back( "http://user@host:80" ); // UI, host,port
    constructorTests.push_back( "http://user@host" ); // ui, host
    constructorTests.push_back( "http://host" ); // host
    constructorTests.push_back( "http://host:80" ); // host,port
    constructorTests.push_back( "http://joe@:80" ); // ui, port (becomes registry-based)
    constructorTests.push_back( "file:///foo/bar" ); // empty authority, non empty path
    constructorTests.push_back( "ht?tp://hoe@host:80" ); // miscellaneous tests
    constructorTests.push_back( "mai/lto:hey?joe#man" );
    constructorTests.push_back( "http://host/a%20path#frag" );
    // path with an escaped octet for space char
    constructorTests.push_back( "http://host/a%E2%82%ACpath#frag" );
    // path with escaped octet for unicode char, not USASCII
    constructorTests.push_back( "http://host/a\u20ACpath#frag" );
    // path with unicode char, not USASCII equivalent to
    constructorTests.push_back( "http://host%20name/" );
    // escaped octets in host (becomes registry based)
    constructorTests.push_back( "http://host\u00DFname/" );
    // unicodechar in host (becomes registry based)
    // equivalent to = "http://host\u00dfname/",
    constructorTests.push_back( "ht123-+tp://www.google.com:80/test" );
    // legal chars in scheme

    for( unsigned int i = 0; i < constructorTests.size(); i++ ) {
        CPPUNIT_ASSERT_NO_THROW_MESSAGE(
            string( "Failed to construct URI for: " ) + constructorTests[i],
            URIHelper().parseURI( constructorTests.at(i), false ) );
    }

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

    for( unsigned int i = 0; i < constructorTestsInvalid.size(); i++ ) {
        CPPUNIT_ASSERT_THROW_MESSAGE(
            string( "URI not caught as invalid: " ) + constructorTestsInvalid[i],
            URIHelper().parseURI( constructorTestsInvalid.at(i), false ),
            URISyntaxException );
    }

    std::vector<const char*> constructorTestsInvalid2;
    // authority validation
    constructorTestsInvalid2.push_back( "http://user@[3ffe:2x00:100:7031::1]:80/test" );// malformed
    // IPv6 authority
    constructorTestsInvalid2.push_back( "http://[ipv6address]/apath#frag" ); // malformed ipv6 address
    constructorTestsInvalid2.push_back( "http://[ipv6address/apath#frag" ); // malformed ipv6 address
    constructorTestsInvalid2.push_back( "http://ipv6address]/apath#frag" ); // illegal char in host name
    constructorTestsInvalid2.push_back( "http://ipv6[address/apath#frag" );
    constructorTestsInvalid2.push_back( "http://ipv6addr]ess/apath#frag" );
    constructorTestsInvalid2.push_back( "http://ipv6address[]/apath#frag" );
    // illegal char in username...
    constructorTestsInvalid2.push_back( "http://us[]er@host/path?query#frag" );
    constructorTestsInvalid2.push_back( "http://host name/path" ); // illegal
    // char in authority
    constructorTestsInvalid2.push_back( "http://host^name#fragment" ); // illegal char in authority
    constructorTestsInvalid2.push_back( "telnet://us er@hostname/" ); // illegal char in authority
    // missing components
    constructorTestsInvalid2.push_back( "//" ); // Authority expected
    constructorTestsInvalid2.push_back( "ascheme://" ); // Authority expected
    constructorTestsInvalid2.push_back( "ascheme:" ); // Scheme-specific part expected
    // scheme validation
    constructorTestsInvalid2.push_back( "a scheme://reg/" ); // illegal char
    constructorTestsInvalid2.push_back( "1scheme://reg/" ); // non alpha char as 1st char
    constructorTestsInvalid2.push_back( "asche\u00dfme:ssp" ); // unicode char , not USASCII
    constructorTestsInvalid2.push_back( "asc%20heme:ssp" );// escape octets

    for( unsigned int i = 0; i < constructorTestsInvalid2.size(); i++ ) {
        CPPUNIT_ASSERT_THROW_MESSAGE(
            string( "URI not caught as invalid: " ) + constructorTestsInvalid2[i],
            URIHelper().parseURI( constructorTestsInvalid2.at(i), false ),
            URISyntaxException );
    }
}
