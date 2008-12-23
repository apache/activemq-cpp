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
#include <decaf/lang/Integer.h>
#include <decaf/lang/Boolean.h>

using namespace std;
using namespace decaf;
using namespace decaf::net;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
URITest::URITest() {
}

namespace decaf{
namespace net{

////////////////////////////////////////////////////////////////////////////////
std::vector<URI> getUris() {

    std::vector<URI> uris;

    uris.push_back( URI( "http://user%60%20info@host/a%20path?qu%60%20ery#fr%5E%20ag" ) );
    // escaped octets for illegal chars
    uris.push_back( URI( "http", "user%60%20info", "host", 80, "/a%20path", "qu%60%20ery", "fr%5E%20ag" ) );
    // escaped octets for illegal
    uris.push_back( URI( "http", "user%C3%9F%C2%A3info", "host", -1,
                         "/a%E2%82%ACpath", "qu%C2%A9%C2%AEery", "fr%C3%A4%C3%A8g" ) );
    // Illegal Chars
    uris.push_back( URI( "http", "user` info", "host", 81, "/a path", "qu` ery", "fr^ ag" ) );
    // % as illegal char, not escaped octet
    uris.push_back( URI( "http", "user%info", "host", 0, "/a%path", "que%ry", "f%rag" ) );
    uris.push_back( URI( "mailto", "user@domain.com", "" ) );
    // no host, path, query or fragment
    uris.push_back( URI( "../adirectory/file.html#" ) );
    // relative path with empty fragment;
    uris.push_back( URI( "news", "comp.infosystems.www.servers.unix", "" ) );
    uris.push_back( URI( "", "", "", "fragment" ) );
    uris.push_back( URI( "telnet://server.org" ) );
    uris.push_back( URI( "http://reg:istry?query" ) );
    uris.push_back( URI( "file:///c:/temp/calculate.pl?" ) );

    return uris;
}

}}

////////////////////////////////////////////////////////////////////////////////
void URITest::testConstructorOneString() {

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
            URI( constructorTests.at(i) ) );
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
            URI( constructorTestsInvalid.at(i) ),
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
            URI( constructorTestsInvalid2.at(i) ),
            URISyntaxException );
    }

    try {
        URI("%3");
        CPPUNIT_FAIL( "Assert 0: URI constructor failed to throw exception on invalid input." );
    } catch( URISyntaxException& e ) {
        CPPUNIT_ASSERT_MESSAGE(
            "Assert 1: Wrong index in URISyntaxException.", 0 == e.getIndex() );
    }

    // Regression test for HARMONY-25
    // if port value is negative, the authority should be considered
    // registry-based.
    URI uri("http://host:-8096/path/index.html");
    CPPUNIT_ASSERT_MESSAGE(
        "Assert 2: returned wrong port value,", -1 == uri.getPort() );
    CPPUNIT_ASSERT_MESSAGE( "Assert 3: returned wrong host value,", uri.getHost() == "" );
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Assert 4: Expected URISyntaxException: ",
        uri.parseServerAuthority(),
        URISyntaxException );

    URI uri2( "http", "//myhost:-8096", "" );
    CPPUNIT_ASSERT_MESSAGE(
        "Assert 5: returned wrong port value,", -1 == uri2.getPort() );
    CPPUNIT_ASSERT_MESSAGE( "Assert 6: returned wrong host value,", uri2.getHost() == "" );
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Assert 7: Expected URISyntaxException: ",
        uri.parseServerAuthority(),
        URISyntaxException );
}

////////////////////////////////////////////////////////////////////////////////
void URITest::testURIString() {

    CPPUNIT_ASSERT_NO_THROW_MESSAGE(
        "Should Not Throw an Exception URI( \"\\\" )",
        URI uri( "/" )
    );

    try {
        URI myUri( ":abc@mymail.com" );
        CPPUNIT_FAIL("TestA, URISyntaxException expected, but not received.");
    } catch( URISyntaxException& e ) {
        CPPUNIT_ASSERT_MESSAGE(
            "TestA, Wrong URISyntaxException index, ", 0 == e.getIndex());
    }

    try {
        URI uri( "path[one" );
        CPPUNIT_FAIL("TestB, URISyntaxException expected, but not received.");
    } catch( URISyntaxException& e1 ) {
        CPPUNIT_ASSERT_MESSAGE(
            "TestB, Wrong URISyntaxException index, ", 4 == e1.getIndex());
    }

    try {
        URI uri( " " );
        CPPUNIT_FAIL("TestC, URISyntaxException expected, but not received.");
    } catch( URISyntaxException& e2 ) {
        CPPUNIT_ASSERT_MESSAGE(
            "TestC, Wrong URISyntaxException index, ", 0 == e2.getIndex());
    }
}

////////////////////////////////////////////////////////////////////////////////
void URITest::testConstructorThreeString() {

    URI uri( "mailto", "mduerst@ifi.unizh.ch", "" );

    CPPUNIT_ASSERT_MESSAGE( "wrong userinfo", uri.getUserInfo() == "" );
    CPPUNIT_ASSERT_MESSAGE( "wrong hostname", uri.getHost() == "" );
    CPPUNIT_ASSERT_MESSAGE( "wrong authority", uri.getAuthority() == "" );
    CPPUNIT_ASSERT_MESSAGE( "wrong port number", -1 == uri.getPort() );
    CPPUNIT_ASSERT_MESSAGE( "wrong path", uri.getPath() == "" );
    CPPUNIT_ASSERT_MESSAGE( "wrong query", uri.getQuery() == "" );
    CPPUNIT_ASSERT_MESSAGE( "wrong fragment", uri.getFragment() == "" );
    CPPUNIT_ASSERT_MESSAGE(
        "wrong SchemeSpecificPart", uri.getSchemeSpecificPart() == "mduerst@ifi.unizh.ch" );

    // scheme specific part can not be null
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Expected URISyntaxException: ",
        URI( "mailto", "", "" ),
        URISyntaxException );

    // scheme needs to start with an alpha char
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Expected URISyntaxException: ",
        URI( "3scheme", "//authority/path", "fragment" ),
        URISyntaxException );

    // scheme can not be a colon
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Expected URISyntaxException: ",
        URI( ":", "//authority/path", "fragment" ),
        URISyntaxException );
}

////////////////////////////////////////////////////////////////////////////////
void URITest::testConstructorStringPlusInts() {

    // check for URISyntaxException for invalid Server Authority
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Expected URISyntaxException: ",
        URI( "http", "user", "host\u00DFname", -1, "/file", "query", "fragment" ),
        URISyntaxException );

    // escaped octets in host name
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Expected URISyntaxException: ",
        URI( "http", "user", "host%20name", -1, "/file", "query", "fragment" ),
        URISyntaxException );

    // illegal char in host name
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Expected URISyntaxException: ",
        URI( "http", "user", "host name", -1, "/file", "query", "fragment" ),
        URISyntaxException );

    // illegal char in host name
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Expected URISyntaxException: ",
        URI( "http", "user", "host]name", -1, "/file", "query", "fragment" ),
        URISyntaxException );

    // missing host name
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Expected URISyntaxException: ",
        URI( "http", "user", "", 80, "/file", "query", "fragment" ),
        URISyntaxException );

    // missing host name
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Expected URISyntaxException: ",
        URI( "http", "user", "", -1, "/file", "query", "fragment" ),
        URISyntaxException );

    // malformed ipv4 address
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Expected URISyntaxException: ",
        URI( "telnet", "", "256.197.221.200", -1, "", "", "" ),
        URISyntaxException );

    // malformed ipv4 address
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Expected URISyntaxException: ",
        URI( "ftp", "", "198.256.221.200", -1, "", "", "" ),
        URISyntaxException );

    // check for URISyntaxException for invalid scheme
    // escaped octets in scheme
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Expected URISyntaxException: ",
        URI( "ht%20tp", "user", "hostname", -1, "/file", "query", "fragment" ),
        URISyntaxException );

    // illegal char in scheme
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Expected URISyntaxException: ",
        URI( "ht tp", "user", "hostname", -1, "/file", "query", "fragment" ),
        URISyntaxException );

    // illegal char in scheme
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Expected URISyntaxException: ",
        URI( "ht]tp", "user", "hostname", -1, "/file", "query", "fragment" ),
        URISyntaxException );

    // relative path with scheme
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Expected URISyntaxException: ",
        URI( "http", "user", "hostname", -1, "relative", "query", "fragment" ),
        URISyntaxException );

    // functional test
    try {

        URI uri( "http", "us:e@r", "hostname", 85, "/file/dir#/qu?e/", "qu?er#y", "frag#me?nt" );

        CPPUNIT_ASSERT_MESSAGE( "wrong userinfo", uri.getUserInfo() == "us:e@r" );
        CPPUNIT_ASSERT_MESSAGE( "wrong hostname", uri.getHost() == "hostname" );
        CPPUNIT_ASSERT_MESSAGE( "wrong port number", 85 == uri.getPort() );
        CPPUNIT_ASSERT_MESSAGE( "wrong path", uri.getPath() =="/file/dir#/qu?e/" );
        CPPUNIT_ASSERT_MESSAGE( "wrong query", uri.getQuery() == "qu?er#y" );
        CPPUNIT_ASSERT_MESSAGE( "wrong fragment", uri.getFragment() == "frag#me?nt" );
        CPPUNIT_ASSERT_MESSAGE( "wrong SchemeSpecificPart",
            uri.getSchemeSpecificPart() == "//us:e@r@hostname:85/file/dir#/qu?e/?qu?er#y" );

    } catch( URISyntaxException& e ) {
        CPPUNIT_FAIL( "Unexpected Exception: " + e.getMessage() );
    }
}

////////////////////////////////////////////////////////////////////////////////
void URITest::testConstructorFourString() {

    // relative path
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Expected URISyntaxException: ",
        URI( "http", "www.joe.com", "relative", "jimmy" ),
        URISyntaxException );

    // valid parameters for this constructor
    {
        URI uri( "http", "www.joe.com", "/path", "jimmy" );
    }

    // illegal char in path
    {
        URI uri( "http", "www.host.com", "/path?q", "somefragment" );
    }

    // empty fragment
    {
        URI uri( "ftp", "ftp.is.co.za", "/rfc/rfc1808.txt", "" );
    }

    // path with escaped octet for unicode char, not USASCII
    {
        URI uri( "http", "host", "/a%E2%82%ACpath", "frag" );
    }

    // All fields empty
    {
        URI uri( "", "", "", "" );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Expected URISyntaxException: ",
        URI( "http", ":2:3:4:5:6:7:8", "/apath", "\u20ACfrag" ),
        URISyntaxException );
}

////////////////////////////////////////////////////////////////////////////////
void URITest::testConstructorFiveString() {

    // URISyntaxException on relative path
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Expected URISyntaxException: ",
        URI( "http", "www.joe.com", "relative", "query", "jimmy" ),
        URISyntaxException );

    // test if empty authority is parsed into undefined host, userinfo and port
    // and if escaped octets in components are preserved, illegal chars are quoted
    URI uri("ht12-3+tp", "", "/p#a%E2%82%ACth", "q^u%25ery", "fragment");

    CPPUNIT_ASSERT_MESSAGE( "wrong scheme", uri.getScheme() == "ht12-3+tp" );
    CPPUNIT_ASSERT_MESSAGE( "wrong port number", -1 == uri.getPort() );
    CPPUNIT_ASSERT_MESSAGE( "wrong path", uri.getPath() == "/p#a%E2%82%ACth" );
    CPPUNIT_ASSERT_MESSAGE( "wrong query", uri.getQuery() == "q^u%25ery" );
    CPPUNIT_ASSERT_MESSAGE( "wrong fragment", uri.getFragment() == "fragment" );

    CPPUNIT_ASSERT_MESSAGE(
        string( "wrong SchemeSpecificPart: " ) + uri.getSchemeSpecificPart(),
        uri.getSchemeSpecificPart() == "///p#a%E2%82%ACth?q^u%25ery" );

    CPPUNIT_ASSERT_MESSAGE(
        string( "wrong RawSchemeSpecificPart" ) + uri.getRawSchemeSpecificPart(),
        uri.getRawSchemeSpecificPart() == "///p%23a%25E2%2582%25ACth?q%5Eu%2525ery" );

    CPPUNIT_ASSERT_MESSAGE(
        string( "incorrect toString()" ) + uri.toString(),
        uri.toString() == "ht12-3+tp:/p%23a%25E2%2582%25ACth?q%5Eu%2525ery#fragment" );
}

////////////////////////////////////////////////////////////////////////////////
void URITest::testConstructorFiveString2() {

    // accept [] as part of valid ipv6 host name
    URI uri( "ftp", "[0001:1234::0001]", "/dir1/dir2", "query", "frag" );

    CPPUNIT_ASSERT_MESSAGE( "Returned incorrect host",
                            uri.getHost() == "[0001:1234::0001]" );

    // do not accept [] as part of invalid ipv6 address
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Expected URISyntaxException for invalid ipv6 address",
        URI( "ftp", "[www.abc.com]", "/dir1/dir2", "query", "frag" ),
        URISyntaxException );

    // do not accept [] as part of user info
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Expected URISyntaxException for invalid user info",
        URI( "ftp", "[user]@host", "/dir1/dir2", "query", "frag" ),
        URISyntaxException );

}

////////////////////////////////////////////////////////////////////////////////
void URITest::testCompareToOne() {

    // compareTo tests
    const char* compareToData[34][2] = {
        // scheme tests
        { "http:test", "" }, // scheme null, scheme not null
        { "", "http:test" }, // reverse
        { "http:test", "ftp:test" }, // schemes different
        { "/test", "/test" }, // schemes null
        { "http://joe", "http://joe" }, // schemes same
        { "http://joe", "hTTp://joe" }, // schemes same ignoring case

        // opacity : one opaque, the other not
        { "http:opaque", "http://nonopaque" },
        { "http://nonopaque", "http:opaque" },
        { "mailto:abc", "mailto:abc" }, // same ssp
        { "mailto:abC", "mailto:Abc" }, // different, by case
        { "mailto:abc", "mailto:def" }, // different by letter
        { "mailto:abc#ABC", "mailto:abc#DEF" },
        { "mailto:abc#ABC", "mailto:abc#ABC" },
        { "mailto:abc#DEF", "mailto:abc#ABC" },

        // hierarchical tests..

        // different authorities
        { "//www.test.com/test", "//www.test2.com/test" },

        { "/nullauth", "//nonnullauth/test" }, // one null authority
        { "//nonnull", "/null" },
        { "/hello", "/hello" }, // both authorities null
        // different userinfo
        { "http://joe@test.com:80", "http://test.com" },
        { "http://jim@test.com", "http://james@test.com" },
        // different hostnames
        { "http://test.com", "http://toast.com" },
        { "http://test.com:80", "test.com:87" }, // different ports
        { "http://test.com", "http://test.com:80" },
        // different paths
        { "http://test.com:91/dir1", "http://test.com:91/dir2" },
        // one null host
        { "http:/hostless", "http://hostfilled.com/hostless" },

        // queries
        { "http://test.com/dir?query", "http://test.com/dir?koory" },
        { "/test?query", "/test" },
        { "/test", "/test?query" },
        { "/test", "/test" },

        // fragments
        { "ftp://test.com/path?query#frag", "ftp://test.com/path?query" },
        { "ftp://test.com/path?query", "ftp://test.com/path?query#frag" },
        { "#frag", "#frag" },
        { "p", "" },

        { "http://www.google.com", "#test" } // miscellaneous
    };

    int compareToResults[34] = {
        1, -1, 1, 0, 0, 0, 1, -1, 0, 1, -1, -1, 0, 1, -1, -1, 1, 0, 1, 1,
        -1, -1, -1, -1, -1, 1, 1, -1, 0, 1, -1, 0, 1, 1
    };

    // test compareTo functionality
    for( unsigned int i = 0; i < 34; i++ ) {

        URI b( compareToData[i][0] );
        URI r( compareToData[i][1] );

        if( b.compareTo( r ) != compareToResults[i] ) {
            CPPUNIT_FAIL( std::string("Test ") + decaf::lang::Integer::toString(i)
                        + std::string(": ") + compareToData[i][0] + " compared to "
                        + compareToData[i][1] + " -> "
                        + decaf::lang::Integer::toString( b.compareTo(r) )
                        + " rather than "
                        + decaf::lang::Integer::toString( compareToResults[i] ) );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void URITest::testCompareToTwo() {

    {
        // test URIs with host names with different casing
        URI uri1( "http://AbC.cOm/root/news" );
        URI uri2( "http://aBc.CoM/root/news" );
        CPPUNIT_ASSERT_MESSAGE( "TestA", 0 == uri1.compareTo( uri2 ) );
        CPPUNIT_ASSERT_MESSAGE( "TestB", 0 == uri2.compareTo( uri1 ) );
    }

    {
        // test URIs with one undefined component
        URI uri1( "http://abc.com:80/root/news" );
        URI uri2( "http://abc.com/root/news" );
        CPPUNIT_ASSERT_MESSAGE( "TestC", uri1.compareTo( uri2 ) > 0 );
        CPPUNIT_ASSERT_MESSAGE( "TestD", uri2.compareTo( uri1 ) < 0 );
    }

    {
        // test URIs with one undefined component
        URI uri1( "http://user@abc.com/root/news" );
        URI uri2( "http://abc.com/root/news" );
        CPPUNIT_ASSERT_MESSAGE( "TestE", uri1.compareTo( uri2 ) > 0 );
        CPPUNIT_ASSERT_MESSAGE( "TestF", uri2.compareTo( uri1 ) < 0 );
    }
}

////////////////////////////////////////////////////////////////////////////////
void URITest::testCreate() {

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "IllegalArgumentException expected but not received.",
        URI::create( "a scheme://reg/" ),
        IllegalArgumentException );
}

////////////////////////////////////////////////////////////////////////////////
void URITest::testEquals() {

    const char* equalsData[30][2] = {
        { "", "" }, // null frags
        { "/path", "/path#frag" },
        { "#frag", "#frag2" },
        { "#frag", "#FRag" },
        // case insensitive on hex escapes
        { "#fr%4F", "#fr%4f" },
        { "scheme:test", "scheme2:test" }, // scheme stuff
        { "test", "http:test" },
        { "http:test", "test" },
        { "SCheme:test", "schEMe:test" },
        // hierarchical/opaque mismatch
        { "mailto:jim", "mailto://jim" },
        { "mailto://test", "mailto:test" },
        // opaque
        { "mailto:name", "mailto:name" },
        { "mailtO:john", "mailto:jim" },
        // test hex case insensitivity on ssp
        { "mailto:te%4Fst", "mailto:te%4fst" },
        { "mailto:john#frag", "mailto:john#frag2" },
        // hierarchical
        { "/test", "/test" }, // paths
        { "/te%F4st", "/te%f4st" },
        { "/TEst", "/teSt" },
        { "", "/test" },
        // registry based because they don't resolve properly to
        // server-based add more tests here
        { "//host.com:80err", "//host.com:80e" },
        { "//host.com:81e%Abrr", "//host.com:81e%abrr" },
        { "/test", "//auth.com/test" },
        { "//test.com", "/test" },
        { "//test.com", "//test.com" }, // hosts
        // case insensitivity for hosts
        { "//HoSt.coM/", "//hOsT.cOm/" },
        { "//te%ae.com", "//te%aE.com" },
        { "//test.com:80", "//test.com:81" },
        { "//joe@test.com:80", "//test.com:80" },
        { "//jo%3E@test.com:82", "//jo%3E@test.com:82" },
        { "//test@test.com:85", "//test@test.com" },
    };

    bool equalsResults[30] = {
        true, false, false, false, true, false, false, false,
        true, false, false, true, false, true, false, true,
        true, false, false, false, true, false, false, true,
        true, true, false, false, true, false, };

    // test equals functionality
    for( int i = 0; i < 30; i++ ) {
        URI b( equalsData[i][0] );
        URI r( equalsData[i][1] );
        if( b.equals( r ) != equalsResults[i] ) {
            CPPUNIT_FAIL( std::string( "Error: " ) + equalsData[i][0] +
                          std::string( " == " ) + equalsData[i][1] + "? -> " +
                          Boolean::toString( b.equals( r ) ) + " expected " +
                          Boolean::toString( equalsResults[i] ) );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void URITest::testEquals2() {

    {
        // test URIs with empty string authority
        URI uri1( "http:///~/dictionary" );
        URI uri2( uri1.getScheme(), uri1.getAuthority(), uri1.getPath(),
                  uri1.getQuery(), uri1.getFragment() );

        CPPUNIT_ASSERT( uri2.equals( uri1 ) );
    }

    {
        // test URIs with port number
        URI uri1( "http://abc.com%E2%82%AC:88/root/news" );
        URI uri2( "http://abc.com%E2%82%AC/root/news" );
        CPPUNIT_ASSERT( !uri1.equals( uri2 ) );
        CPPUNIT_ASSERT( !uri2.equals( uri1 ) );
    }

    {
        // test URIs with host names with different casing
        URI uri1( "http://AbC.cOm/root/news" );
        URI uri2( "http://aBc.CoM/root/news" );
        CPPUNIT_ASSERT( uri1.equals( uri2 ) );
        CPPUNIT_ASSERT( uri2.equals( uri1 ) );
    }
}

////////////////////////////////////////////////////////////////////////////////
void URITest::testGetAuthority() {

    std::vector<URI> uris = getUris();

    const char* getAuthorityResults[] = {
        "user` info@host",
        "user%60%20info@host:80",
        "user%C3%9F%C2%A3info@host",
        "user` info@host:81",
        "user%info@host:0",
        "",
        "",
        "",
        "",
        "server.org",
        "reg:istry",
        ""
    };

    for( unsigned int i = 0; i < uris.size(); i++) {

        std::string result = uris[i].getAuthority();

        if( getAuthorityResults[i] != result ) {

            CPPUNIT_FAIL( std::string( "Error: For URI \"" ) + uris[i].toString() +
                          std::string( "\"," ) + "\n" +
                          string( "getAuthority() returned: " ) + result + "\n" +
                          string( "expected: ") + getAuthorityResults[i] );
        }
    }

    CPPUNIT_ASSERT(
        URI( "", "", "", 127, "", "", "" ).getAuthority() == "" );
}

////////////////////////////////////////////////////////////////////////////////
void URITest::testGetAuthority2() {

    {
        // tests for URIs with empty string authority component
        URI uri( "file:///tmp/" );
        CPPUNIT_ASSERT_MESSAGE( string( "Authority not empty for URI: " ) + uri.toString(),
                                uri.getAuthority() == "" );
        CPPUNIT_ASSERT_MESSAGE( string( "Host not empty for URI " ) + uri.toString(),
                                uri.getHost() == "" );
        CPPUNIT_ASSERT_MESSAGE( "testA, toString() returned incorrect value",
                                string( "file:///tmp/" ) == uri.toString() );
    }
    {
        URI uri( "file", "", "/tmp", "frag" );
        CPPUNIT_ASSERT_MESSAGE( string( "Authority not null for URI: " ) + uri.toString(),
                                uri.getAuthority() == "" );
        CPPUNIT_ASSERT_MESSAGE( string( "Host not null for URI " ) + uri.toString(),
                                uri.getHost() == "" );
        CPPUNIT_ASSERT_MESSAGE( string( "testB, toString() returned incorrect value:" ) + uri.toString(),
                                string( "file:/tmp#frag" ) == uri.toString() );
    }
    {
        URI uri( "file", "", "/tmp", "query", "frag" );
        CPPUNIT_ASSERT_MESSAGE( string( "Authority not null for URI: " ) + uri.toString(),
                                uri.getAuthority() == "" );
        CPPUNIT_ASSERT_MESSAGE( "Host not null for URI " + uri.toString(),
                                uri.getHost() == "" );
        CPPUNIT_ASSERT_MESSAGE( "test C, toString() returned incorrect value",
                                string( "file:/tmp?query#frag" ) == uri.toString() );
    }

    // after normalization the host string info may be lost since the
    // uri string is reconstructed
    URI uri( "file", "www.google.com", "/tmp/a/../b/c", "query", "frag" );
    URI uri2 = uri.normalize();
    CPPUNIT_ASSERT_MESSAGE( string( "Authority not null for URI: " ) + uri2.toString(),
                            uri.getAuthority() == "www.google.com" );
    CPPUNIT_ASSERT_MESSAGE( string( "Host not null for URI " ) + uri2.toString(),
                            uri.getHost() == "www.google.com" );
    CPPUNIT_ASSERT_MESSAGE( "test D, toString() returned incorrect value: " + uri.toString(),
                            string( "file://www.google.com/tmp/a/../b/c?query#frag" ) == uri.toString() );
    CPPUNIT_ASSERT_MESSAGE( "test E, toString() returned incorrect value: " + uri2.toString(),
                            string( "file://www.google.com/tmp/b/c?query#frag" ) == uri2.toString() );

    // the empty string host will give URISyntaxException
    // for the 7 arg constructor
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "URISyntaxException expected but not received.",
        URI("file", "user", "", 80, "/path", "query", "frag"),
        URISyntaxException );
}

////////////////////////////////////////////////////////////////////////////////
void URITest::testGetFragment() {

    std::vector<URI> uris = getUris();

    const char* getFragmentResults[] = {
        "fr^ ag",
        "fr%5E%20ag",
        "fr%C3%A4%C3%A8g",
        "fr^ ag",
        "f%rag", "", "", "",
        "fragment", "", "", ""
    };

    for( unsigned int i = 0; i < uris.size(); i++ ) {
        string result = uris[i].getFragment();
        if( getFragmentResults[i] != result ) {
            CPPUNIT_FAIL( string( "Error: For URI \"" ) + uris[i].toString() +
                          string( "\", getFragment() returned: " ) + result +
                          string( ", expected: " ) + getFragmentResults[i] );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void URITest::testGetHost() {

    std::vector<URI> uris = getUris();

    const char* getHostResults[] = {
        "host", "host", "host",
        "host", "host", "",
        "", "", "",
        "server.org",
        "", "" };

    for( unsigned int i = 0; i < uris.size(); i++ ) {
        string result = uris[i].getHost();
        if( getHostResults[i] != result ) {
            CPPUNIT_FAIL( string( "Error: For URI \"" ) + uris[i].toString() +
                          string( "\", getHost() returned: " ) + result +
                          string( ", expected: " ) + getHostResults[i] );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void URITest::testGetPath() {

    std::vector<URI> uris = getUris();

    const char* getPathResults[] = {
        "/a path",
        "/a%20path",
        "/a%E2%82%ACpath",
        "/a path",
        "/a%path", "",
        "../adirectory/file.html",
        "", "", "", "",
        "/c:/temp/calculate.pl"
    };

    for( unsigned int i = 0; i < uris.size(); i++ ) {
        string result = uris[i].getPath();
        if( getPathResults[i] != result ) {
            CPPUNIT_FAIL( string( "Error: For URI \"" ) + uris[i].toString() +
                          string( "\", getPath() returned: " ) + result +
                          string( ", expected: " ) + getPathResults[i] );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void URITest::testGetPort() {

    std::vector<URI> uris = getUris();

    int getPortResults[] = {
        -1, 80, -1, 81, 0, -1, -1, -1, -1, -1, -1, -1 };

    for( unsigned int i = 0; i < uris.size(); i++ ) {
        int result = uris[i].getPort();
        CPPUNIT_ASSERT_MESSAGE( string( "Error: For URI \"" ) + uris[i].toString() +
                                string( "\", getPort() returned: " ) +
                                Integer::toString( result ) +
                                string( ", expected: " ) +
                                Integer::toString( getPortResults[i] ),
                                result == getPortResults[i] );
    }
}

////////////////////////////////////////////////////////////////////////////////
void URITest::testGetPort2() {

    // if port value is negative, the authority should be considered registry based.

    URI uri( "http://myhost:-8096/site/index.html" );
    CPPUNIT_ASSERT_MESSAGE( "TestA, returned wrong port value,", -1 == uri.getPort() );
    CPPUNIT_ASSERT_MESSAGE( "TestA, returned wrong host value,", uri.getHost() == "" );
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "TestA - URISyntaxException expected but not received.",
        uri.parseServerAuthority(),
        URISyntaxException );

    URI uri2( "http", "//myhost:-8096", "" );
    CPPUNIT_ASSERT_MESSAGE( "TestB returned wrong port value,", -1 == uri2.getPort() );
    CPPUNIT_ASSERT_MESSAGE( "TestB returned wrong host value,", uri2.getHost() == "" );
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "TestB - URISyntaxException expected but not received.",
        uri2.parseServerAuthority(),
        URISyntaxException );
}

////////////////////////////////////////////////////////////////////////////////
void URITest::testGetQuery() {

    std::vector<URI> uris = getUris();

    const char* getQueryResults[] = {
        "qu` ery",
        "qu%60%20ery",
        "qu%C2%A9%C2%AEery",
        "qu` ery",
        "que%ry",
        "", "", "", "", "",
        "query",
        ""
    };

    for( unsigned int i = 0; i < uris.size(); i++ ) {
        string result = uris[i].getQuery();
        if( getQueryResults[i] != result ) {
            CPPUNIT_FAIL( string( "Error: For URI \"" ) + uris[i].toString() +
                          string( "\", getQuery() returned: " ) + result +
                          string( ", expected: " ) + getQueryResults[i] );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void URITest::testGetRawAuthority() {

    std::vector<URI> uris = getUris();

    const char* getRawAuthorityResults[] = {
            "user%60%20info@host",
            "user%2560%2520info@host:80",
            "user%25C3%259F%25C2%25A3info@host",
            "user%60%20info@host:81",
            "user%25info@host:0",
            "", "", "", "",
            "server.org",
            "reg:istry",
            "" };

    for( unsigned int i = 0; i < uris.size(); i++ ) {
        string result = uris[i].getRawAuthority();
        if( getRawAuthorityResults[i] != result ) {
            CPPUNIT_FAIL( string( "Error: For URI \"" ) + uris[i].toString() +
                          string( "\", getRawAuthority() returned: " ) + result +
                          string( ", expected: " ) + getRawAuthorityResults[i] );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void URITest::testGetRawFragment() {

    std::vector<URI> uris = getUris();

    const char* getRawFragmentResults[] = {
        "fr%5E%20ag",
        "fr%255E%2520ag",
        "fr%25C3%25A4%25C3%25A8g",
        "fr%5E%20ag", "f%25rag",
        "", "", "",
        "fragment",
        "", "", ""
    };

    for( unsigned int i = 0; i < uris.size(); i++ ) {
        string result = uris[i].getRawFragment();
        if( getRawFragmentResults[i] != result ) {
            CPPUNIT_FAIL( string( "Error: For URI \"" ) + uris[i].toString() +
                          string( "\", getRawFragment() returned: " ) + result +
                          string( ", expected: " ) + getRawFragmentResults[i] );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void URITest::testGetRawPath() {

    std::vector<URI> uris = getUris();

    const char* getRawPathResults[] = {
        "/a%20path",
        "/a%2520path", "/a%25E2%2582%25ACpath",
        "/a%20path", "/a%25path", "",
        "../adirectory/file.html",
        "", "", "", "",
        "/c:/temp/calculate.pl" };

    for( unsigned int i = 0; i < uris.size(); i++ ) {
        string result = uris[i].getRawPath();
        if( getRawPathResults[i] != result ) {
            CPPUNIT_FAIL( string( "Error: For URI \"" ) + uris[i].toString() +
                          string( "\", getRawPath() returned: " ) + result +
                          string( ", expected: " ) + getRawPathResults[i] );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void URITest::testGetRawQuery() {

    std::vector<URI> uris = getUris();

    const char* getRawQueryResults[] = {
        "qu%60%20ery",
        "qu%2560%2520ery",
        "qu%25C2%25A9%25C2%25AEery",
        "qu%60%20ery", "que%25ry",
        "", "", "", "", "",
        "query", ""
    };

    for( unsigned int i = 0; i < uris.size(); i++ ) {
        string result = uris[i].getRawQuery();
        if( getRawQueryResults[i] != result ) {
            CPPUNIT_FAIL( string( "Error: For URI \"" ) + uris[i].toString() +
                          string( "\", getRawQuery() returned: " ) + result +
                          string( ", expected: " ) + getRawQueryResults[i] );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void URITest::testGetRawSchemeSpecificPart() {

    std::vector<URI> uris = getUris();

    const char* getRawSspResults[] = {
        "//user%60%20info@host/a%20path?qu%60%20ery",
        "//user%2560%2520info@host:80/a%2520path?qu%2560%2520ery",
        "//user%25C3%259F%25C2%25A3info@host/a%25E2%2582%25ACpath?qu%25C2%25A9%25C2%25AEery",
        "//user%60%20info@host:81/a%20path?qu%60%20ery",
        "//user%25info@host:0/a%25path?que%25ry",
        "user@domain.com",
        "../adirectory/file.html",
        "comp.infosystems.www.servers.unix",
        "",
        "//server.org",
        "//reg:istry?query",
        "///c:/temp/calculate.pl?"
    };

    for( unsigned int i = 0; i < uris.size(); i++ ) {
        string result = uris[i].getRawSchemeSpecificPart();
        if( getRawSspResults[i] != result ) {
            CPPUNIT_FAIL( string( "Error: For URI[" ) +
                          Integer::toString( i ) + string( "] \"" ) + uris[i].toString() +
                          string( "\", getRawSchemeSpecificPart() returned: " ) + result +
                          string( ", expected: " ) + getRawSspResults[i] );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void URITest::testGetRawUserInfo() {

    std::vector<URI> uris = getUris();

    const char* getRawUserInfoResults[] = {
        "user%60%20info",
        "user%2560%2520info",
        "user%25C3%259F%25C2%25A3info",
        "user%60%20info", "user%25info",
        "", "", "", "", "", "", "" };

    for( unsigned int i = 0; i < uris.size(); i++ ) {
        string result = uris[i].getRawUserInfo();
        if( getRawUserInfoResults[i] != result ) {
            CPPUNIT_FAIL( string( "Error: For URI[" ) +
                          Integer::toString( i ) + string( "] \"" ) + uris[i].toString() +
                          string( "\", getRawUserInfo() returned: " ) + result +
                          string( ", expected: " ) + getRawUserInfoResults[i] );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void URITest::testGetScheme() {

    std::vector<URI> uris = getUris();

    const char* getSchemeResults[] = {
        "http", "http",
        "http", "http",
        "http", "mailto",
        "", "news",
        "", "telnet",
        "http", "file"
    };

    for( unsigned int i = 0; i < uris.size(); i++ ) {
        string result = uris[i].getScheme();
        if( getSchemeResults[i] != result ) {
            CPPUNIT_FAIL( string( "Error: For URI[" ) +
                          Integer::toString( i ) + string( "] \"" ) + uris[i].toString() +
                          string( "\", getScheme() returned: " ) + result +
                          string( ", expected: " ) + getSchemeResults[i] );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void URITest::testGetSchemeSpecificPart() {

    std::vector<URI> uris = getUris();

    const char* getSspResults[] = {
        "//user` info@host/a path?qu` ery",
        "//user%60%20info@host:80/a%20path?qu%60%20ery",
        "//user%C3%9F%C2%A3info@host/a%E2%82%ACpath?qu%C2%A9%C2%AEery",
        "//user` info@host:81/a path?qu` ery",
        "//user%info@host:0/a%path?que%ry",
        "user@domain.com",
        "../adirectory/file.html",
        "comp.infosystems.www.servers.unix",
        "",
        "//server.org",
        "//reg:istry?query",
        "///c:/temp/calculate.pl?"
    };

    for( unsigned int i = 0; i < uris.size(); i++ ) {
        string result = uris[i].getSchemeSpecificPart();
        if( getSspResults[i] != result ) {
            CPPUNIT_FAIL( string( "Error: For URI[" ) +
                          Integer::toString( i ) + string( "] \"" ) + uris[i].toString() +
                          string( "\", getSchemeSpecificPart() returned: " ) + result +
                          string( ", expected: " ) + getSspResults[i] );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void URITest::testGetUserInfo() {

    std::vector<URI> uris = getUris();

    const char* getUserInfoResults[] = {
        "user` info",
        "user%60%20info",
        "user%C3%9F%C2%A3info",
        "user` info",
        "user%info",
        "", "", "", "", "", "", ""
    };

    for( unsigned int i = 0; i < uris.size(); i++ ) {
        string result = uris[i].getUserInfo();
        if( getUserInfoResults[i] != result ) {
            CPPUNIT_FAIL( string( "Error: For URI[" ) +
                          Integer::toString( i ) + string( "] \"" ) + uris[i].toString() +
                          string( "\", getUserInfo() returned: " ) + result +
                          string( ", expected: " ) + getUserInfoResults[i] );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void URITest::testIsAbsolute() {

    const char* isAbsoluteData[] = {
        "mailto:user@ca.ibm.com",
        "urn:isbn:123498989h",
        "news:software.ibm.com",
        "http://www.amazon.ca",
        "file:///d:/temp/results.txt",
        "scheme:ssp",
        "calculate.pl?isbn=123498989h",
        "?isbn=123498989h",
        "//www.amazon.ca",
        "a.html",
        "#top",
        "//pc1/",
        "//user@host/path/file"
    };

    bool results[] = {
        true, true, true, true, true, true,
        false, false, false, false, false, false, false
    };

    for( unsigned int i = 0; i < 13; i++) {
        bool result = URI( isAbsoluteData[i] ).isAbsolute();
        CPPUNIT_ASSERT_MESSAGE( string( "URI(" ) + isAbsoluteData[i] +
                                string( ").isAbsolute() = " ) + Boolean::toString( result ),
                                results[i] == result );
    }
}

////////////////////////////////////////////////////////////////////////////////
void URITest::testIsOpaque() {

    const char* isOpaqueData[] = {
        "mailto:user@ca.ibm.com",
        "urn:isbn:123498989h",
        "news:software.ibm.com",
        "http://www.amazon.ca",
        "file:///d:/temp/results.txt",
        "scheme:ssp",
        "calculate.pl?isbn=123498989h",
        "?isbn=123498989h",
        "//www.amazon.ca", "a.html", "#top",
        "//pc1/", "//user@host/path/file" };

    bool results[] = {
        true, true, true, false, false,
        true, false, false, false, false,
        false, false, false };

    for( unsigned int i = 0; i < 13; i++) {
        bool result = URI( isOpaqueData[i] ).isOpaque();
        CPPUNIT_ASSERT_MESSAGE( string( "URI(" ) + isOpaqueData[i] +
                                string( ").isOpaque() = " ) + Boolean::toString( result ),
                                results[i] == result );
    }
}

////////////////////////////////////////////////////////////////////////////////
void URITest::testNormalize() {

    const char* normalizeData[] = {
            // normal
            "/",
            "/a",
            "/a/b",
            "/a/b/c",
            // single, '.'
            "/.", "/./", "/./.", "/././",
            "/./a",
            "/./a/",
            "/././a",
            "/././a/",
            "/a/.",
            "/a/./",
            "/a/./.",
            "/a/./b",
            // double, '..'
            "/a/..", "/a/../", "/a/../b", "/a/../b/..", "/a/../b/../",
            "/a/../b/../c", "/..", "/../", "/../..", "/../../", "/../a",
            "/../a/", "/../../a", "/../../a/", "/a/b/../../c",
            "/a/b/../..",
            "/a/b/../../",
            "/a/b/../../c",
            "/a/b/c/../../../d",
            "/a/b/..",
            "/a/b/../",
            "/a/b/../c",
            // miscellaneous
            "/a/b/.././../../c/./d/../e",
            "/a/../../.c././../././c/d/../g/..",
            // '.' in the middle of segments
            "/a./b", "/.a/b", "/a.b/c", "/a/b../c",
            "/a/..b/c",
            "/a/b..c/d",
            // no leading slash, miscellaneous
            "", "a", "a/b", "a/b/c", "../", ".", "..", "../g",
            "g/a/../../b/c/./g", "a/b/.././../../c/./d/../e",
            "a/../../.c././../././c/d/../g/..", };

    const char* normalizeResults[] = {
        "/", "/a", "/a/b", "/a/b/c",
        "/", "/", "/", "/", "/a", "/a/", "/a", "/a/", "/a/", "/a/",
        "/a/", "/a/b", "/", "/", "/b", "/", "/", "/c", "/..", "/../",
        "/../..", "/../../", "/../a", "/../a/", "/../../a",
        "/../../a/", "/c", "/", "/", "/c", "/d", "/a/", "/a/", "/a/c",
        "/../c/e", "/../c/", "/a./b", "/.a/b", "/a.b/c", "/a/b../c",
        "/a/..b/c", "/a/b..c/d", "", "a", "a/b", "a/b/c", "../", "",
        "..", "../g", "b/c/g", "../c/e", "../c/",
    };

    for( unsigned int i = 0; i < 57; i++) {
        URI test( normalizeData[i] );
        std::string result = test.normalize().toString();
        CPPUNIT_ASSERT_MESSAGE( string( "URI(" ) + normalizeData[i] +
                                string( ") normalized incorrectly to := " ) + result,
                                result == normalizeResults[i] );
    }
}


////////////////////////////////////////////////////////////////////////////////
void URITest::testNormalize2() {

    URI uri1( "file:/D:/one/two/../../three" );
    URI uri2 = uri1.normalize();

    CPPUNIT_ASSERT_MESSAGE( "Normalized to incorrect URI: " + uri2.toString(),
                            string( "file:/D:/three" ) == uri2.toString() );
    CPPUNIT_ASSERT_MESSAGE( "Resolved URI is not absolute",  uri2.isAbsolute() );
    CPPUNIT_ASSERT_MESSAGE( "Resolved URI is opaque", !uri2.isOpaque() );
    CPPUNIT_ASSERT_MESSAGE( "Resolved URI has incorrect scheme specific part",
                            string( "/D:/three" ) == uri2.getRawSchemeSpecificPart() );
}

////////////////////////////////////////////////////////////////////////////////
void URITest::testNormalize3() {

    // return same URI if it has a normalized path already
    URI uri1( "http://host/D:/one/two/three" );
    URI uri2 = uri1.normalize();
    CPPUNIT_ASSERT_MESSAGE( "Failed to return same URI after normalization",
                            uri1.toString() == uri2.toString() );

    // try with empty path
    URI uri3( "http", "host", "", "fragment" );
    URI uri4 = uri3.normalize();
    CPPUNIT_ASSERT_MESSAGE( "Failed to return same URI after normalization",
                            uri3.toString() == uri4.toString() );
}


////////////////////////////////////////////////////////////////////////////////
void URITest::testParseServerAuthority() {

    // registry based uris
    std::vector<URI> uris;

    // port number not digits
    uris.push_back( URI( "http://foo:bar/file#fragment" ) );
    uris.push_back( URI( "http", "//foo:bar/file", "fragment" ) );
    // escaped octets in host name
    uris.push_back( URI( "http://host%20name/" ) );
    uris.push_back( URI( "http", "//host%20name/", "" ) );
    // missing host name, port number
    uris.push_back( URI( "http://joe@:80" ) );
    // missing host name, no port number
    uris.push_back( URI( "http://user@/file?query#fragment" ) );
    uris.push_back( URI( "//host.com:80err") ); // malformed port number
    uris.push_back( URI( "//host.com:81e%Abrr") );
    // malformed ipv4 address
    uris.push_back( URI( "telnet", "//256.197.221.200", "" ) );
    uris.push_back( URI( "telnet://198.256.221.200") );
    uris.push_back( URI( "//te%ae.com") ); // misc ..
    uris.push_back( URI( "//:port") );
    uris.push_back( URI( "//:80" ) );
    // last label has to start with alpha char
    uris.push_back( URI( "//fgj234fkgj.jhj.123.") );
    uris.push_back( URI( "//fgj234fkgj.jhj.123") );
    // '-' cannot be first or last character in a label
    uris.push_back( URI( "//-domain.name" ) );
    uris.push_back( URI( "//domain.name-" ) );
    uris.push_back( URI( "//domain-") );
    // illegal char in host name
    uris.push_back( URI( "//doma*in") );
    // host expected
    uris.push_back( URI( "http://:80/") );
    uris.push_back( URI( "http://user@/" ) );
    // ipv6 address not enclosed in "[]"
    uris.push_back( URI( "http://3ffe:2a00:100:7031:22:1:80:89/" ) );
    // expected ipv6 addresses to be enclosed in "[]"
    uris.push_back( URI( "http", "34::56:78", "/path", "query", "fragment" ) );
    // expected host
    uris.push_back( URI( "http", "user@", "/path", "query", "fragment" ) );

    // these URIs do not have valid server based authorities,
    // but single arg, 3 and 5 arg constructors
    // parse them as valid registry based authorities

    // exception should occur when parseServerAuthority is
    // requested on these uris
    for( unsigned int i = 0; i < uris.size(); i++ ) {
        try {
            URI uri = uris[i].parseServerAuthority();
            CPPUNIT_FAIL( string( "URISyntaxException expected but not received for URI: " ) +
                          uris[i].toString());
        } catch( URISyntaxException& e ) {
            // Expected
        }
    }

    // valid Server based authorities
    URI( "http", "3ffe:2a00:100:7031:2e:1:80:80", "/path", "fragment" ).parseServerAuthority();
    URI( "http", "host:80", "/path", "query", "fragment" ).parseServerAuthority();
    URI( "http://[::3abc:4abc]:80/" ).parseServerAuthority();
    URI( "http", "34::56:78", "/path", "fragment" ).parseServerAuthority();
    URI( "http", "[34:56::78]:80", "/path", "fragment" ).parseServerAuthority();

    // invalid authorities (neither server nor registry)
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Expected URISyntaxException for URI ",
        URI( "http://us[er@host:80/" ),
        URISyntaxException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Expected URISyntaxException for URI ",
        URI( "http://[ddd::hgghh]/" ),
        URISyntaxException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Expected URISyntaxException for URI ",
        URI( "http", "[3ffe:2a00:100:7031:2e:1:80:80]a:80", "/path", "fragment" ),
        URISyntaxException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Expected URISyntaxException for URI ",
        URI( "http", "host:80", "/path", "fragment" ),
        URISyntaxException );

    CPPUNIT_ASSERT_NO_THROW( URI::create("file://C:/1.txt").parseServerAuthority() );
}

////////////////////////////////////////////////////////////////////////////////
void URITest::testRelativizeLURI() {

    // relativization tests
    const char* relativizeData[][2] = {
        // first is base, second is the one to relativize
        { "http://www.google.com/dir1/dir2", "mailto:test" }, // rel = opaque
        { "mailto:test", "http://www.google.com" }, // base = opaque
        // different authority
        { "http://www.eclipse.org/dir1",
          "http://www.google.com/dir1/dir2" },
        // different scheme
        { "http://www.google.com", "ftp://www.google.com" },

        { "http://www.google.com/dir1/dir2/",
          "http://www.google.com/dir3/dir4/file.txt" },
        { "http://www.google.com/dir1/",
          "http://www.google.com/dir1/dir2/file.txt" },
        { "./dir1/", "./dir1/hi" },
        { "/dir1/./dir2", "/dir1/./dir2/hi" },
        { "/dir1/dir2/..", "/dir1/dir2/../hi" },
        { "/dir1/dir2/..", "/dir1/dir2/hi" },
        { "/dir1/dir2/", "/dir1/dir3/../dir2/text" },
        { "//www.google.com", "//www.google.com/dir1/file" },
        { "/dir1", "/dir1/hi" }, { "/dir1/", "/dir1/hi" },
    };

    // expected results
    const char* relativizeResults[] = {
        "mailto:test",
        "http://www.google.com",
        "http://www.google.com/dir1/dir2",
        "ftp://www.google.com",
        "http://www.google.com/dir3/dir4/file.txt",
        "dir2/file.txt",
        "hi",
        "hi",
        "hi",
        "dir2/hi",
        "text",
        "dir1/file",
        "hi",
        "hi",
    };

    for( unsigned int i = 0; i < 14; i++ ) {

        try {

            URI b( relativizeData[i][0] );
            URI r( relativizeData[i][1] );

            if( ( b.relativize( r ) ).toString() != relativizeResults[i] ) {
                CPPUNIT_FAIL( string( "Error: relativize, " ) + relativizeData[i][0] +
                              string( ", " ) + relativizeData[i][1] + " returned: " +
                              b.relativize( r ).toString() + ", expected:" +
                              relativizeResults[i] );
            }

        } catch( URISyntaxException& e ) {
            CPPUNIT_FAIL( string( "Exception on relativize test on data " ) +
                          relativizeData[i][0] + ", " + relativizeData[i][1] +
                          string( ": " ) + e.getMessage() );
        }
    }

    {
        URI a( "http://host/dir" );
        URI b( "http://host/dir/file?query" );
        CPPUNIT_ASSERT_MESSAGE(
            string( "Assert 0: URI relativized incorrectly: " ) + a.relativize( b ).toString(),
            URI( "file?query" ).equals( a.relativize( b ) ) );
    }
    {
        // One URI with empty host
        URI a( "file:///~/first" );
        URI b( "file://tools/~/first" );
        CPPUNIT_ASSERT_MESSAGE("Assert 1: URI relativized incorrectly,",
            URI( "file://tools/~/first" ).equals( a.relativize( b ) ) );
        CPPUNIT_ASSERT_MESSAGE("Assert 2: URI relativized incorrectly,",
            URI( "file:///~/first" ).equals( b.relativize( a ) ) );
    }
    {
        // Both URIs with empty hosts
        URI a( "file:///~/first" );
        URI b( "file:///~/second" );
        CPPUNIT_ASSERT_MESSAGE( "Assert 3: URI relativized incorrectly,",
                      URI( "file:///~/second").equals( a.relativize( b ) ) );
        CPPUNIT_ASSERT_MESSAGE("Assert 4: URI relativized incorrectly,",
            URI( "file:///~/first").equals( b.relativize( a ) ) );
    }
}

////////////////////////////////////////////////////////////////////////////////
void URITest::testRelativize2() {

    {
        URI a( "http://host/dir" );
        URI b( "http://host/dir/file?query" );
        CPPUNIT_ASSERT_MESSAGE( "relativized incorrectly,",
            URI( "file?query" ).equals( a.relativize( b ) ) );
    }
    {
        // one URI with empty host
        URI a( "file:///~/dictionary" );
        URI b( "file://tools/~/dictionary" );
        CPPUNIT_ASSERT_MESSAGE( "relativized incorrectly,",
            URI( "file://tools/~/dictionary" ).equals( a.relativize( b ) ) );
        CPPUNIT_ASSERT_MESSAGE("relativized incorrectly,",
            URI( "file:///~/dictionary" ).equals( b.relativize( a ) ) );
    }
    {
        // two URIs with empty hosts
        URI a( "file:///~/dictionary" );
        URI b( "file:///~/therasus" );
        CPPUNIT_ASSERT_MESSAGE( "relativized incorrectly,",
            URI( "file:///~/therasus" ).equals( a.relativize( b ) ) );
        CPPUNIT_ASSERT_MESSAGE( "relativized incorrectly,",
            URI( "file:///~/dictionary").equals( b.relativize( a ) ) );
    }

    {
        URI one( "file:/C:/test/ws" );
        URI two( "file:/C:/test/ws" );
        URI empty("");
        CPPUNIT_ASSERT( empty.equals( one.relativize( two ) ) );
    }
    {
        URI one( "file:/C:/test/ws" );
        URI two( "file:/C:/test/ws/p1" );
        URI result( "p1" );
        CPPUNIT_ASSERT( result.equals( one.relativize( two ) ) );
    }
    {
        URI one( "file:/C:/test/ws/" );
        URI two( "file:/C:/test/ws/p1" );
        URI result( "p1" );
        CPPUNIT_ASSERT( result.equals( one.relativize( two ) ) );
    }
}


////////////////////////////////////////////////////////////////////////////////
void URITest::testResolve() {

    URI uri1( "file:/D:/one/two/three" );
    URI uri2 = uri1.resolve( URI( ".." ) );

    CPPUNIT_ASSERT_MESSAGE(
        string( "Resolved to incorrect URI: " ) + uri2.toString(),
        string( "file:/D:/one/" ) == uri2.toString() );
    CPPUNIT_ASSERT_MESSAGE( "Resolved URI is not absolute", uri2.isAbsolute());
    CPPUNIT_ASSERT_MESSAGE( "Resolved URI is opaque", !uri2.isOpaque() );
    CPPUNIT_ASSERT_MESSAGE(
        string( "Resolved URI has incorrect scheme specific part" ) +
        uri2.getRawSchemeSpecificPart(),
        string( "/D:/one/" ) == uri2.getRawSchemeSpecificPart() );
}

////////////////////////////////////////////////////////////////////////////////
void URITest::testResolveURI() {

    // resolution tests
    const char* resolveData[12][2] = {
        // authority in given URI
        { "http://www.test.com/dir", "//www.test.com/hello?query#fragment" },
        // no authority, absolute path
        { "http://www.test.com/dir", "/abspath/file.txt" },
        // no authority, relative paths
        { "/", "dir1/file.txt" },
        { "/dir1", "dir2/file.txt" },
        { "/dir1/", "dir2/file.txt" },
        { "", "dir1/file.txt" },
        { "dir1", "dir2/file.txt" },
        { "dir1/", "dir2/file.txt" },
        // normalization required
        { "/dir1/dir2/../dir3/./", "dir4/./file.txt" },
        // allow a standalone fragment to be resolved
        { "http://www.google.com/hey/joe?query#fragment", "#frag2" },
        // return given when base is opaque
        { "mailto:idontexist@uk.ibm.com", "dir1/dir2" },
        // return given when given is absolute
        { "http://www.google.com/hi/joe", "http://www.oogle.com" },
    };

    // expected results
    const char* resolveResults[] = {
        "http://www.test.com/hello?query#fragment",
        "http://www.test.com/abspath/file.txt",
        "/dir1/file.txt",
        "/dir2/file.txt",
        "/dir1/dir2/file.txt",
        "dir1/file.txt",
        "dir2/file.txt",
        "dir1/dir2/file.txt",
        "/dir1/dir3/dir4/file.txt",
        "http://www.google.com/hey/joe?query#frag2",
        "dir1/dir2",
        "http://www.oogle.com",
    };

    for( unsigned int i = 0; i < 12; i++ ) {

        try {

            URI b( resolveData[i][0] );
            URI r( resolveData[i][1] );
            URI result = b.resolve( r );

            if( result.toString() != resolveResults[i] ) {
                CPPUNIT_FAIL( string( "Error: resolve at iteration: " ) +
                              Integer::toString( i ) + ": \n" +
                              resolveData[i][0] + ", " + resolveData[i][1] +
                              string( "\nreturned: " ) + b.resolve( r ).toString() +
                              string( "\nexpected: " ) + resolveResults[i] );
            }

            if( !b.isOpaque() ) {
                CPPUNIT_ASSERT_MESSAGE( b.toString() + " and " + result.toString() +
                                        " incorrectly differ in absoluteness",
                                        b.isAbsolute() == result.isAbsolute() );
            }

        } catch( URISyntaxException& e ) {
            CPPUNIT_FAIL( string( "Exception on resolve test on data " ) +
                          resolveData[i][0] + ", " + resolveData[i][1] +
                          ": " + e.getMessage() );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void URITest::testToString() {

    std::vector<URI> uris = getUris();

    const char* toStringResults[] = {
            "http://user%60%20info@host/a%20path?qu%60%20ery#fr%5E%20ag",
            "http://user%2560%2520info@host:80/a%2520path?qu%2560%2520ery#fr%255E%2520ag",
            "http://user%25C3%259F%25C2%25A3info@host/a%25E2%2582%25ACpath?qu%25C2%25A9%25C2%25AEery#fr%25C3%25A4%25C3%25A8g",
            "http://user%60%20info@host:81/a%20path?qu%60%20ery#fr%5E%20ag",
            "http://user%25info@host:0/a%25path?que%25ry#f%25rag",
            "mailto:user@domain.com",
            "../adirectory/file.html#",
            "news:comp.infosystems.www.servers.unix",
            "#fragment",
            "telnet://server.org",
            "http://reg:istry?query",
            "file:///c:/temp/calculate.pl?" };

    for( unsigned int i = 0; i < uris.size(); i++ ) {
        std::string result = uris[i].toString();
        CPPUNIT_ASSERT_MESSAGE( string( "Error: For URI \"" ) + uris[i].toString() +
                                string( "\", toString() returned: \n" ) + result +
                                string( "\n, expected: \n" ) + toStringResults[i],
                                result == toStringResults[i] );
    }
}

////////////////////////////////////////////////////////////////////////////////
void URITest::testToURL() {

    const char* absoluteuris[] = {
        "mailto:noreply@apache.org",
        "urn:isbn:123498989h",
        "news:software.ibm.com",
        "http://www.apache.org",
        "file:///d:/temp/results.txt",
        "scheme:ssp", };

    const char* relativeuris[] = {
        "calculate.pl?isbn=123498989h",
        "?isbn=123498989h",
        "//www.apache.org",
        "a.html",
        "#top",
        "//pc1/",
        "//user@host/path/file" };

    for( int i = 0; i < 6; i++ ) {
        try {
            URI( absoluteuris[i] ).toURL();
        } catch( MalformedURLException& e ) {
            // not all the URIs can be translated into valid URLs
        }
    }

    for( int i = 0; i < 7; i++ ) {
        try {
            URI( relativeuris[i] ).toURL();
            CPPUNIT_FAIL( "Expected IllegalArgumentException not thrown" );
        } catch( IllegalArgumentException& e ) {
            // Expected
        }
    }
}
