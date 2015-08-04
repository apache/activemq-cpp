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

#include "URLTest.h"

#include <decaf/net/URI.h>
#include <decaf/net/URL.h>
#include <decaf/lang/Integer.h>
#include <decaf/lang/Boolean.h>
#include <decaf/net/URLStreamHandler.h>
#include <decaf/net/URLStreamHandlerFactory.h>
#include <decaf/lang/exceptions/SecurityException.h>
#include <decaf/lang/exceptions/StringIndexOutOfBoundsException.h>

using namespace std;
using namespace decaf;
using namespace decaf::net;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
namespace {

    class MyURLStreamHandler : public URLStreamHandler {
    protected:

        virtual URLConnection* openConnection(const URL& url) {
            return NULL;
        }

        using URLStreamHandler::openConnection;

    public:

        void parse(URL& url, const String& spec, int start, int end) {
            parseURL(url, spec, start, end);
        }
    };

    class MyURLStreamHandlerFactory : public URLStreamHandlerFactory {
    public:

        virtual URLStreamHandler* createURLStreamHandler(const std::string& protocol) {
            return new MyURLStreamHandler();
        }
    };

}

////////////////////////////////////////////////////////////////////////////////
URLTest::URLTest() {
}

////////////////////////////////////////////////////////////////////////////////
URLTest::~URLTest() {
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testConstructor1() {

    // Tests for multiple URL instantiation basic parsing test
    URL a("http://www.yahoo1.com:8080/dir1/dir2/test.cgi?point1.html#anchor1");
    CPPUNIT_ASSERT_EQUAL_MESSAGE("a returns a wrong protocol", String("http"), a.getProtocol());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("a returns a wrong host", String("www.yahoo1.com"), a.getHost());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("a returns a wrong port", 8080, a.getPort());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("a returns a wrong file",
                                 String("/dir1/dir2/test.cgi?point1.html"), a.getFile());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("a returns a wrong anchor", String("anchor1"), a.getRef());

    // test for no file
    URL b("http://www.yahoo2.com:9999");
    CPPUNIT_ASSERT_EQUAL_MESSAGE("b returns a wrong protocol", String("http"), b.getProtocol());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("b returns a wrong host", String("www.yahoo2.com"), b.getHost());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("b returns a wrong port", 9999, b.getPort());
    CPPUNIT_ASSERT_MESSAGE("b returns a wrong file", b.getFile().equals(""));
    CPPUNIT_ASSERT_MESSAGE("b returns a wrong anchor", b.getRef().isEmpty());

    // test for no port
    URL c("http://www.yahoo3.com/dir1/dir2/test.cgi?point1.html#anchor1");
    CPPUNIT_ASSERT_EQUAL_MESSAGE("c returns a wrong protocol", String("http"), c.getProtocol());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("c returns a wrong host", String("www.yahoo3.com"), c.getHost());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("c returns a wrong port", -1, c.getPort());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("c returns a wrong file",
                                 String("/dir1/dir2/test.cgi?point1.html"), c.getFile());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("c returns a wrong anchor", String("anchor1"), c.getRef());

    // test for no port
    URL d("file://www.yahoo3.com/dir1/dir2/test.cgi#anchor1");
    CPPUNIT_ASSERT_EQUAL_MESSAGE("d returns a wrong protocol", String("file"), d.getProtocol());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("d returns a wrong host", String("www.yahoo3.com"), d.getHost());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("d returns a wrong port", -1, d.getPort());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("d returns a wrong file", String("/dir1/dir2/test.cgi"), d.getFile());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("d returns a wrong anchor", String("anchor1"), d.getRef());

    // test for no file, no port
    URL e("http://www.yahoo4.com/");
    CPPUNIT_ASSERT_EQUAL_MESSAGE("e returns a wrong protocol", String("http"), e.getProtocol());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("e returns a wrong host", String("www.yahoo4.com"), e.getHost());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("e returns a wrong port", -1, e.getPort());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("e returns a wrong file", String("/"), e.getFile());
    CPPUNIT_ASSERT_MESSAGE("e returns a wrong anchor", e.getRef().isEmpty());

    // test for no file, no port
    URL f("file://www.yahoo4.com/");
    CPPUNIT_ASSERT_EQUAL_MESSAGE("f returns a wrong protocol", String("file"), f.getProtocol());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("f returns a wrong host", String("www.yahoo4.com"), f.getHost());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("f returns a wrong port", -1, f.getPort());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("f returns a wrong file", String("/"), f.getFile());
    CPPUNIT_ASSERT_MESSAGE("f returns a wrong anchor", f.getRef().isEmpty());

    // test for no file, no port
    URL g("file://www.yahoo4.com");
    CPPUNIT_ASSERT_EQUAL_MESSAGE("g returns a wrong protocol", String("file"), g.getProtocol());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("g returns a wrong host", String("www.yahoo4.com"), g.getHost());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("g returns a wrong port", -1, g.getPort());
    CPPUNIT_ASSERT_MESSAGE("g returns a wrong file", g.getFile().equals(""));
    CPPUNIT_ASSERT_MESSAGE("g returns a wrong anchor", g.getRef().isEmpty());

    // test for non-port ":" and wierd characters occurrences
    URL h("http://www.yahoo5.com/di!@$%^&*()_+r1/di:::r2/test.cgi?point1.html#anchor1");
    CPPUNIT_ASSERT_EQUAL_MESSAGE("h returns a wrong protocol", String("http"), h.getProtocol());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("h returns a wrong host", String("www.yahoo5.com"), h.getHost());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("h returns a wrong port", -1, h.getPort());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("h returns a wrong file",
                                 String("/di!@$%^&*()_+r1/di:::r2/test.cgi?point1.html"), h.getFile());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("h returns a wrong anchor", String("anchor1"), h.getRef());

    URL i("file:/testing.tst");
    CPPUNIT_ASSERT_EQUAL_MESSAGE("i returns a wrong protocol", String("file"), i.getProtocol());
    CPPUNIT_ASSERT_MESSAGE("i returns a wrong host", i.getHost().equals(""));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("i returns a wrong port", -1, i.getPort());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("i returns a wrong file", String("/testing.tst"), i.getFile());
    CPPUNIT_ASSERT_MESSAGE("i returns a wrong anchor", i.getRef().isEmpty());

    URL j("file:testing.tst");
    CPPUNIT_ASSERT_EQUAL_MESSAGE("j returns a wrong protocol", String("file"), j.getProtocol());
    CPPUNIT_ASSERT_MESSAGE("j returns a wrong host", j.getHost().equals(""));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("j returns a wrong port", -1, j.getPort());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("j returns a wrong file", String("testing.tst"), j.getFile());
    CPPUNIT_ASSERT_MESSAGE("j returns a wrong anchor", j.getRef().isEmpty());

    URL l("http://host:/file");
    CPPUNIT_ASSERT_EQUAL_MESSAGE("l return a wrong port", -1, l.getPort());

    URL m("file:../../file.txt");
    CPPUNIT_ASSERT_MESSAGE(std::string("m returns a wrong file: ") + m.getFile().toString(),
                           m.getFile().equals("../../file.txt"));

    URL n("http://[fec0::1:20d:60ff:fe24:7410]:35/file.txt");
    CPPUNIT_ASSERT_MESSAGE("n returns a wrong protocol ", n.getProtocol().equals("http"));
    CPPUNIT_ASSERT_MESSAGE("n returns a wrong host ", n.getHost().equals("[fec0::1:20d:60ff:fe24:7410]"));
    CPPUNIT_ASSERT_MESSAGE("n returns a wrong port ", n.getPort() == 35);
    CPPUNIT_ASSERT_MESSAGE("n returns a wrong file ", n.getFile().equals("/file.txt"));
    CPPUNIT_ASSERT_MESSAGE("n returns a wrong anchor ", n.getRef().isEmpty());

    URL o("file://[fec0::1:20d:60ff:fe24:7410]/file.txt#sogood");
    CPPUNIT_ASSERT_MESSAGE("o returns a wrong protocol ", o.getProtocol().equals("file"));
    CPPUNIT_ASSERT_MESSAGE("o returns a wrong host ", o.getHost().equals("[fec0::1:20d:60ff:fe24:7410]"));
    CPPUNIT_ASSERT_MESSAGE("o returns a wrong port ", o.getPort() == -1);
    CPPUNIT_ASSERT_MESSAGE("o returns a wrong file ", o.getFile().equals("/file.txt"));
    CPPUNIT_ASSERT_MESSAGE("o returns a wrong anchor ", o.getRef().equals("sogood"));

    URL p("file://[fec0::1:20d:60ff:fe24:7410]");
    CPPUNIT_ASSERT_MESSAGE("p returns a wrong protocol ", p.getProtocol().equals("file"));
    CPPUNIT_ASSERT_MESSAGE("p returns a wrong host ", p.getHost()
            .equals("[fec0::1:20d:60ff:fe24:7410]"));
    CPPUNIT_ASSERT_MESSAGE("p returns a wrong port ", p.getPort() == -1);

    // TODO internal representation store '//' authority as "" which can't be distinguished
    //      from no authority versions.
//    URL q("file:////file.txt");
//    CPPUNIT_ASSERT_MESSAGE("q returns a wrong authority ", q.getAuthority().isEmpty());
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("q returns a wrong file ", String("////file.txt"), q.getFile());

    URL r("file:///file.txt");
    CPPUNIT_ASSERT_MESSAGE("r returns a wrong authority", r.getAuthority().equals(""));
    CPPUNIT_ASSERT_MESSAGE("r returns a wrong file ", r.getFile().equals("/file.txt"));

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown MalformedURLException",
        URL("http://www.yahoo5.com::22/dir1/di:::r2/test.cgi?point1.html#anchor1"),
        MalformedURLException);

    // unknown protocol
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown MalformedURLException",
        URL("myProtocol://www.yahoo.com:22"),
        MalformedURLException);

    // no protocol
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown MalformedURLException",
        URL("www.yahoo.com"),
        MalformedURLException);

    try {
        // No leading or trailing spaces.
        URL u1("file:/some/path");
        CPPUNIT_ASSERT_EQUAL_MESSAGE("5 got wrong file length1", 10, u1.getFile().length());

        // Leading spaces.
        URL u2("  file:/some/path");
        CPPUNIT_ASSERT_EQUAL_MESSAGE("5 got wrong file length2", 10, u2.getFile().length());

        // Trailing spaces.
        URL u3("file:/some/path  ");
        CPPUNIT_ASSERT_EQUAL_MESSAGE("5 got wrong file length3", 10, u3.getFile().length());

        // Leading and trailing.
        URL u4("  file:/some/path ");
        CPPUNIT_ASSERT_EQUAL_MESSAGE("5 got wrong file length4", 10, u4.getFile().length());

        // in-place spaces.
        URL u5("  file:  /some/path ");
        CPPUNIT_ASSERT_EQUAL_MESSAGE("5 got wrong file length5", 12, u5.getFile().length());

    } catch (MalformedURLException& e) {
        CPPUNIT_FAIL("5 Did not expect the exception ");
    }
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testConstructor2() {

    URL u("http", "www.yahoo.com", "test.html#foo");
    CPPUNIT_ASSERT(u.getProtocol().equals("http"));
    CPPUNIT_ASSERT(u.getHost().equals("www.yahoo.com"));
    CPPUNIT_ASSERT(-1 == u.getPort());
    // TODO
//    CPPUNIT_ASSERT_EQUAL(String("test.html"), u.getFile());
    CPPUNIT_ASSERT(u.getRef().equals("foo"));

    // Strange behavior in reference, the hostname contains a ':' so it gets
    // wrapped in '[', ']'
    URL testURL("http", "www.apache.org:8080", "test.html#anch");
    CPPUNIT_ASSERT_EQUAL_MESSAGE("wrong protocol", String("http"), testURL.getProtocol());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("wrong host", String("[www.apache.org:8080]"), testURL.getHost());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("wrong port", -1, testURL.getPort());
    // TODO
    // CPPUNIT_ASSERT_EQUAL_MESSAGE("wrong file", String("test.html"), testURL.getFile());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("wrong anchor", String("anch"), testURL.getRef());
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testConstructor3() {

    URL u("http", "www.yahoo.com", 8080, "test.html#foo");
    CPPUNIT_ASSERT_EQUAL_MESSAGE("SSIS returns a wrong protocol", String("http"), u.getProtocol());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("SSIS returns a wrong host", String("www.yahoo.com"), u.getHost());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("SSIS returns a wrong port", 8080, u.getPort());
    // TODO
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("SSIS returns a wrong file", String("test.html"), u.getFile());
    CPPUNIT_ASSERT_MESSAGE("SSIS returns a wrong anchor: ", u.getRef().equals("foo"));

    CPPUNIT_ASSERT_NO_THROW(URL("http", "apache.org", 123456789, "file"));

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown MalformedURLException",
        URL("http", "apache.org", -123, "file"),
        MalformedURLException);
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testConstructor4() {

    URL context("http://www.yahoo.com");

    // basic ones
    URL a(context, "file.java", new MyURLStreamHandler);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("1 returns a wrong protocol", String("http"), a.getProtocol());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("1 returns a wrong host", String("www.yahoo.com"), a.getHost());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("1 returns a wrong port", -1, a.getPort());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("1 returns a wrong file", String("/file.java"), a.getFile());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("1 returns a wrong anchor", String(), a.getRef());

    URL b(context, "systemresource:/+/FILE0/test.java", new MyURLStreamHandler);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("2 returns a wrong protocol", String("systemresource"), b.getProtocol());
    CPPUNIT_ASSERT_MESSAGE("2 returns a wrong host", b.getHost().equals(""));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("2 returns a wrong port", -1, b.getPort());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("2 returns a wrong file", String("/+/FILE0/test.java"), b.getFile());
    CPPUNIT_ASSERT_MESSAGE("2 returns a wrong anchor", b.getRef().equals(""));

    URL c(context, "dir1/dir2/../file.java", NULL);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("3 returns a wrong protocol", String("http"), c.getProtocol());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("3 returns a wrong host", String("www.yahoo.com"), c.getHost());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("3 returns a wrong port", -1, c.getPort());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("3 returns a wrong file", String("/dir1/dir2/../file.java"), c.getFile());
    CPPUNIT_ASSERT_MESSAGE("3 returns a wrong anchor", c.getRef().equals(""));

    // test for question mark processing
    URL d("http://www.foo.com/d0/d1/d2/cgi-bin?foo=bar/baz");

    // test for relative file and out of bound "/../" processing
    URL e(d, "../dir1/dir2/../file.java", new MyURLStreamHandler);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("A) returns a wrong file: ", String("/d0/d1/dir1/file.java"), e.getFile());

    // TODO
    // test for absolute and relative file processing
//    URL f(d, "/../dir1/dir2/../file.java", NULL);
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("B) returns a wrong file", String("/../dir1/dir2/../file.java"), f.getFile());

    CPPUNIT_ASSERT_NO_THROW(URL("http://www.ibm.com"));
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testEquals() {

    URL u("http://www.apache.org:8080/dir::23??????????test.html");
    URL u1("http://www.apache.org:8080/dir::23??????????test.html");
    CPPUNIT_ASSERT_MESSAGE("A) equals returns false for two identical URLs", u.equals(u1));

    URL u2("file://www.apache.org:8080/dir::23??????????test.html");
    CPPUNIT_ASSERT_MESSAGE("Returned true for non-equal URLs", !u2.equals(u1));

    URL a("file", "", 0, "/test.txt");
    URL b("file", "", 0, "/test.txt");
    CPPUNIT_ASSERT(a.equals(b));

    URL c("file", "first.invalid", 0, "/test.txt");
    URL d("file", "second.invalid", 0, "/test.txt");
    CPPUNIT_ASSERT(!c.equals(d));

    // TODO
//    URL e("file", "harmony.apache.org", 0, "/test.txt");
//    URL f("file", "www.apache.org", 0, "/test.txt");
//    CPPUNIT_ASSERT(e.equals(f));
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testSameFile() {

    URL a("http://www.yahoo.com");
    URL b("http", "www.yahoo.com", "");
    CPPUNIT_ASSERT_MESSAGE("Should be the same1", a.sameFile(b));

    URL c("http://www.yahoo.com/dir1/dir2/test.html#anchor1");
    URL d("http://www.yahoo.com/dir1/dir2/test.html#anchor2");
    CPPUNIT_ASSERT_MESSAGE("Should be the same ", c.sameFile(d));

    // TODO
//    URL e("file", "", -1, "/d:/somedir/");
//    URL f("file:/d:/somedir/");
//    CPPUNIT_ASSERT(!e.sameFile(f));

    URL g("file:///anyfile");
    URL h("file://localhost/anyfile");
    CPPUNIT_ASSERT(g.sameFile(h));

    URL i("http:///anyfile");
    URL j("http://localhost/anyfile");
    CPPUNIT_ASSERT(!i.sameFile(j));

    // TODO
//    URL k("ftp:///anyfile");
//    URL l("ftp://localhost/anyfile");
//    CPPUNIT_ASSERT(!k.sameFile(l));
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testToString() {

    URL a("http://www.yahoo2.com:9999");
    URL b("http://www.yahoo1.com:8080/dir1/dir2/test.cgi?point1.html#anchor1");

    CPPUNIT_ASSERT_EQUAL_MESSAGE("a) Does not return the right url string",
        std::string("http://www.yahoo1.com:8080/dir1/dir2/test.cgi?point1.html#anchor1"), b.toString());

    CPPUNIT_ASSERT_EQUAL_MESSAGE("b) Does not return the right url string",
                                 std::string("http://www.yahoo2.com:9999"), a.toString());

    CPPUNIT_ASSERT_MESSAGE("c) Does not return the right url string",
                           a.equals(URL(a.toString())));
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testToExternalForm() {
    URL b("http://www.yahoo2.com:9999");
    URL a("http://www.yahoo1.com:8080/dir1/dir2/test.cgi?point1.html#anchor1");

    CPPUNIT_ASSERT_EQUAL_MESSAGE("a) Does not return the right url string",
        String("http://www.yahoo1.com:8080/dir1/dir2/test.cgi?point1.html#anchor1"), a.toExternalForm());

    CPPUNIT_ASSERT_EQUAL_MESSAGE("b) Does not return the right url string",
                                 String("http://www.yahoo2.com:9999"), b.toExternalForm());

    CPPUNIT_ASSERT_MESSAGE("c) Does not return the right url string",
                           a.equals(URL(a.toExternalForm())));

    URL c("http:index");
    CPPUNIT_ASSERT_EQUAL_MESSAGE("2 wrong external form", String("http:index"), c.toExternalForm());

    URL d("http", "", "index");
    CPPUNIT_ASSERT_EQUAL_MESSAGE("2 wrong external form", String("http:index"), d.toExternalForm());
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testGetFile() {

    // TODO
//    URL a("http", "www.yahoo.com:8080", 1233, "test/!@$%^&*/test.html#foo");
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("returns a wrong file", String("test/!@$%^&*/test.html"), a.getFile());
    URL b("http", "www.yahoo.com:8080", 1233, "");
    CPPUNIT_ASSERT_MESSAGE("returns a wrong file", b.getFile().equals(""));
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testGetHost() {

    String ipv6Host = "FEDC:BA98:7654:3210:FEDC:BA98:7654:3210";
    URL url("http", ipv6Host, -1, "myfile");
    CPPUNIT_ASSERT_EQUAL((String("[") + ipv6Host + "]"), url.getHost());
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testGetPort() {
    URL a("http://member12.c++.com:9999");
    CPPUNIT_ASSERT_MESSAGE("return wrong port number", a.getPort() == 9999);
    URL b("http://member12.c++.com:9999/");
    CPPUNIT_ASSERT_EQUAL_MESSAGE("return wrong port number", 9999, b.getPort());
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testGetDefaultPort() {
    URL a("http://member12.c++.com:9999");
    CPPUNIT_ASSERT_EQUAL(80, a.getDefaultPort());

    URL b("http://www.google.com:80/example?language[id]=2");
    CPPUNIT_ASSERT_EQUAL(String("www.google.com"), b.getHost());
    CPPUNIT_ASSERT_EQUAL(80, b.getPort());

    // TODO
//    URL b("ftp://member12.c++.com:9999/");
//    CPPUNIT_ASSERT_EQUAL(21, b.getDefaultPort());
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testGetProtocol() {
    URL a("http://www.yahoo2.com:9999");
    CPPUNIT_ASSERT_MESSAGE("u returns a wrong protocol: ", a.getProtocol().equals("http"));
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testGetRef() {
    URL b("http://www.yahoo2.com:9999");
    URL a("http://www.yahoo1.com:8080/dir1/dir2/test.cgi?point1.html#anchor1");

    CPPUNIT_ASSERT_EQUAL_MESSAGE("returns a wrong anchor1", String("anchor1"), a.getRef());

    CPPUNIT_ASSERT_EQUAL_MESSAGE("returns a wrong anchor2", String(), b.getRef() );
    URL c("http://www.yahoo2.com#ref");
    CPPUNIT_ASSERT_EQUAL_MESSAGE("returns a wrong anchor3", String("ref"), c.getRef());
    URL d("http://www.yahoo2.com/file#ref1#ref2");
    CPPUNIT_ASSERT_EQUAL_MESSAGE("returns a wrong anchor4", String("ref1#ref2"), d.getRef());
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testGetAuthority() {

    URL a("http", "hostname", 80, "/java?q1#ref");
    CPPUNIT_ASSERT_EQUAL(String("hostname:80"), a.getAuthority());
    CPPUNIT_ASSERT_EQUAL(String("hostname"), a.getHost());
    CPPUNIT_ASSERT_EQUAL(String(), a.getUserInfo());
    CPPUNIT_ASSERT_EQUAL(String("/java?q1"), a.getFile());
    CPPUNIT_ASSERT_EQUAL(String("/java"), a.getPath());
    CPPUNIT_ASSERT_EQUAL(String("q1"), a.getQuery());
    CPPUNIT_ASSERT_EQUAL(String("ref"), a.getRef());

    URL b("http", "u:p@home", 80, "/java?q1#ref");
    CPPUNIT_ASSERT_EQUAL(String("[u:p@home]:80"), b.getAuthority());
    CPPUNIT_ASSERT_EQUAL(String("[u:p@home]"), b.getHost());
    CPPUNIT_ASSERT_EQUAL(String(""), b.getUserInfo());
    CPPUNIT_ASSERT_EQUAL(String("/java?q1"), b.getFile());
    CPPUNIT_ASSERT_EQUAL(String("/java"), b.getPath());
    CPPUNIT_ASSERT_EQUAL(String("q1"), b.getQuery());
    CPPUNIT_ASSERT_EQUAL(String("ref"), b.getRef());

    URL c("http", "home", -1, "/java");
    CPPUNIT_ASSERT_EQUAL_MESSAGE("wrong authority2", String("home"), c.getAuthority());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("wrong userInfo2", String(), c.getUserInfo());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("wrong host2", String("home"), c.getHost());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("wrong file2", String("/java"), c.getFile());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("wrong path2", String("/java"), c.getPath());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("wrong query2", String(), c.getQuery());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("wrong ref2", String(), c.getRef());
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testToURI() {
    URL a("http://www.apache.org");
    URI uri = a.toURI();
    CPPUNIT_ASSERT(a.equals(uri.toURL()));
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testURLStreamHandlerParseURL() {

    URL url("http://localhost");
    MyURLStreamHandler handler;

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an StringIndexOutOfBoundsException",
        handler.parse(url, "//", 0, Integer::MIN_VALUE),
        StringIndexOutOfBoundsException);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an StringIndexOutOfBoundsException",
        handler.parse(url, "1234//", 4, Integer::MIN_VALUE),
        StringIndexOutOfBoundsException);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an StringIndexOutOfBoundsException",
        handler.parse(url, "1", -1, 0),
        StringIndexOutOfBoundsException);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an SecurityException",
        handler.parse(url, "1", 3, 2),
        SecurityException);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an SecurityException",
        handler.parse(url, "11", 1, Integer::MIN_VALUE),
        SecurityException);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an StringIndexOutOfBoundsException",
        handler.parse(url, "any", 10, Integer::MIN_VALUE),
        StringIndexOutOfBoundsException);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an StringIndexOutOfBoundsException",
        handler.parse(url, "any", 10, Integer::MIN_VALUE+1),
        StringIndexOutOfBoundsException);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an StringIndexOutOfBoundsException",
        handler.parse(url, "any", Integer::MIN_VALUE, Integer::MIN_VALUE),
        StringIndexOutOfBoundsException);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an StringIndexOutOfBoundsException",
        handler.parse(url, "any", Integer::MIN_VALUE, 2),
        StringIndexOutOfBoundsException);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an StringIndexOutOfBoundsException",
        handler.parse(url, "any", -1, 2),
        StringIndexOutOfBoundsException);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an SecurityException",
        handler.parse(url, "any", -1, -1),
        SecurityException);
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testUrlParts() {
    URL url("http://username:password@host:8080/directory/file?query#ref");
    CPPUNIT_ASSERT_EQUAL(String("http"), url.getProtocol());
    CPPUNIT_ASSERT_EQUAL(String("username:password@host:8080"), url.getAuthority());
    CPPUNIT_ASSERT_EQUAL(String("username:password"), url.getUserInfo());
    CPPUNIT_ASSERT_EQUAL(String("host"), url.getHost());
    CPPUNIT_ASSERT_EQUAL(8080, url.getPort());
    CPPUNIT_ASSERT_EQUAL(80, url.getDefaultPort());
    CPPUNIT_ASSERT_EQUAL(String("/directory/file?query"), url.getFile());
    CPPUNIT_ASSERT_EQUAL(String("/directory/file"), url.getPath());
    CPPUNIT_ASSERT_EQUAL(String("query"), url.getQuery());
    CPPUNIT_ASSERT_EQUAL(String("ref"), url.getRef());
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testFileEqualsWithEmptyHost() {
    URL a("file", "", -1, "/a/");
    URL b("file:/a/");
    CPPUNIT_ASSERT(a.equals(b));
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testOmittedHost() {
    URL url("http:///path");
    CPPUNIT_ASSERT_EQUAL(String(""), url.getHost());
    CPPUNIT_ASSERT_EQUAL(String("/path"), url.getFile());
    CPPUNIT_ASSERT_EQUAL(String("/path"), url.getPath());
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testNoHost() {
    URL url("http:/path");
    CPPUNIT_ASSERT_EQUAL(String("http"), url.getProtocol());
    CPPUNIT_ASSERT_EQUAL(String(), url.getAuthority());
    CPPUNIT_ASSERT_EQUAL(String(), url.getUserInfo());
    CPPUNIT_ASSERT_EQUAL(String(), url.getHost());
    CPPUNIT_ASSERT_EQUAL(-1, url.getPort());
    CPPUNIT_ASSERT_EQUAL(80, url.getDefaultPort());
    CPPUNIT_ASSERT_EQUAL(String("/path"), url.getFile());
    CPPUNIT_ASSERT_EQUAL(String("/path"), url.getPath());
    CPPUNIT_ASSERT_EQUAL(String(), url.getQuery());
    CPPUNIT_ASSERT_EQUAL(String(), url.getRef());
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testNoPath() {
    URL url("http://host");
    CPPUNIT_ASSERT_EQUAL(String("host"), url.getHost());
    CPPUNIT_ASSERT_EQUAL(String(), url.getFile());
    CPPUNIT_ASSERT_EQUAL(String(), url.getPath());
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testEmptyHostAndNoPath() {
    URL url("http://");
    CPPUNIT_ASSERT_EQUAL(String("http"), url.getProtocol());
    CPPUNIT_ASSERT_EQUAL(String(), url.getAuthority());
    CPPUNIT_ASSERT_EQUAL(String(), url.getUserInfo());
    CPPUNIT_ASSERT_EQUAL(String(), url.getHost());
    CPPUNIT_ASSERT_EQUAL(-1, url.getPort());
    CPPUNIT_ASSERT_EQUAL(80, url.getDefaultPort());
    CPPUNIT_ASSERT_EQUAL(String(), url.getFile());
    CPPUNIT_ASSERT_EQUAL(String(), url.getPath());
    CPPUNIT_ASSERT_EQUAL(String(), url.getQuery());
    CPPUNIT_ASSERT_EQUAL(String(), url.getRef());
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testNoHostAndNoPath() {
    URL url("http:");
    CPPUNIT_ASSERT_EQUAL(String("http"), url.getProtocol());
    CPPUNIT_ASSERT_EQUAL(String(), url.getAuthority());
    CPPUNIT_ASSERT_EQUAL(String(), url.getUserInfo());
    CPPUNIT_ASSERT_EQUAL(String(), url.getHost());
    CPPUNIT_ASSERT_EQUAL(-1, url.getPort());
    CPPUNIT_ASSERT_EQUAL(80, url.getDefaultPort());
    CPPUNIT_ASSERT_EQUAL(String(), url.getFile());
    CPPUNIT_ASSERT_EQUAL(String(), url.getPath());
    CPPUNIT_ASSERT_EQUAL(String(), url.getQuery());
    CPPUNIT_ASSERT_EQUAL(String(), url.getRef());
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testAtSignInUserInfo() {
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an MalformedURLException",
        URL("http://user@userhost.com:password@host"),
        MalformedURLException);
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testUserNoPassword() {
    URL url("http://user@host");
    CPPUNIT_ASSERT_EQUAL(String("user@host"), url.getAuthority());
    CPPUNIT_ASSERT_EQUAL(String("user"), url.getUserInfo());
    CPPUNIT_ASSERT_EQUAL(String("host"), url.getHost());
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testUserNoPasswordExplicitPort() {
    URL url("http://user@host:8080");
    CPPUNIT_ASSERT_EQUAL(String("user@host:8080"), url.getAuthority());
    CPPUNIT_ASSERT_EQUAL(String("user"), url.getUserInfo());
    CPPUNIT_ASSERT_EQUAL(String("host"), url.getHost());
    CPPUNIT_ASSERT_EQUAL(8080, url.getPort());
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testUserPasswordHostPort() {
    URL url("http://user:password@host:8080");
    CPPUNIT_ASSERT_EQUAL(String("user:password@host:8080"), url.getAuthority());
    CPPUNIT_ASSERT_EQUAL(String("user:password"), url.getUserInfo());
    CPPUNIT_ASSERT_EQUAL(String("host"), url.getHost());
    CPPUNIT_ASSERT_EQUAL(8080, url.getPort());
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testUserPasswordEmptyHostPort() {
    URL url("http://user:password@:8080");
    CPPUNIT_ASSERT_EQUAL(String("user:password@:8080"), url.getAuthority());
    CPPUNIT_ASSERT_EQUAL(String("user:password"), url.getUserInfo());
    CPPUNIT_ASSERT_EQUAL(String(), url.getHost());
    CPPUNIT_ASSERT_EQUAL(8080, url.getPort());
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testUserPasswordEmptyHostEmptyPort() {
    URL url("http://user:password@");
    CPPUNIT_ASSERT_EQUAL(String("user:password@"), url.getAuthority());
    CPPUNIT_ASSERT_EQUAL(String("user:password"), url.getUserInfo());
    CPPUNIT_ASSERT_EQUAL(String(), url.getHost());
    CPPUNIT_ASSERT_EQUAL(-1, url.getPort());
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testPathOnly() {
    URL url("http://host/path");
    CPPUNIT_ASSERT_EQUAL(String("/path"), url.getFile());
    CPPUNIT_ASSERT_EQUAL(String("/path"), url.getPath());
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testQueryOnly() {
    URL url("http://host?query");
    CPPUNIT_ASSERT_EQUAL(String("?query"), url.getFile());
    CPPUNIT_ASSERT_EQUAL(String(), url.getPath());
    CPPUNIT_ASSERT_EQUAL(String("query"), url.getQuery());
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testFragmentOnly() {
    URL url("http://host#fragment");
    CPPUNIT_ASSERT_EQUAL(String(), url.getFile());
    CPPUNIT_ASSERT_EQUAL(String(), url.getPath());
    CPPUNIT_ASSERT_EQUAL(String("fragment"), url.getRef());
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testAtSignInPath() {
    URL url("http://host/file@foo");
    CPPUNIT_ASSERT_EQUAL(String("/file@foo"), url.getFile());
    CPPUNIT_ASSERT_EQUAL(String("/file@foo"), url.getPath());
    CPPUNIT_ASSERT_EQUAL(String(), url.getUserInfo());
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testColonInPath() {
    URL url("http://host/file:colon");
    CPPUNIT_ASSERT_EQUAL(String("/file:colon"), url.getFile());
    CPPUNIT_ASSERT_EQUAL(String("/file:colon"), url.getPath());
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testSlashInQuery() {
    URL url("http://host/file?query/path");
    CPPUNIT_ASSERT_EQUAL(String("/file?query/path"), url.getFile());
    CPPUNIT_ASSERT_EQUAL(String("/file"), url.getPath());
    CPPUNIT_ASSERT_EQUAL(String("query/path"), url.getQuery());
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testQuestionMarkInQuery() {
    URL url("http://host/file?query?another");
    CPPUNIT_ASSERT_EQUAL(String("/file?query?another"), url.getFile());
    CPPUNIT_ASSERT_EQUAL(String("/file"), url.getPath());
    CPPUNIT_ASSERT_EQUAL(String("query?another"), url.getQuery());
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testAtSignInQuery() {
    URL url("http://host/file?query@at");
    CPPUNIT_ASSERT_EQUAL(String("/file?query@at"), url.getFile());
    CPPUNIT_ASSERT_EQUAL(String("/file"), url.getPath());
    CPPUNIT_ASSERT_EQUAL(String("query@at"), url.getQuery());
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testColonInQuery() {
    URL url("http://host/file?query:colon");
    CPPUNIT_ASSERT_EQUAL(String("/file?query:colon"), url.getFile());
    CPPUNIT_ASSERT_EQUAL(String("/file"), url.getPath());
    CPPUNIT_ASSERT_EQUAL(String("query:colon"), url.getQuery());
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testQuestionMarkInFragment() {
    URL url("http://host/file#fragment?query");
    CPPUNIT_ASSERT_EQUAL(String("/file"), url.getFile());
    CPPUNIT_ASSERT_EQUAL(String("/file"), url.getPath());
    CPPUNIT_ASSERT_EQUAL(String(), url.getQuery());
    CPPUNIT_ASSERT_EQUAL(String("fragment?query"), url.getRef());
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testColonInFragment() {
    URL url("http://host/file#fragment:80");
    CPPUNIT_ASSERT_EQUAL(String("/file"), url.getFile());
    CPPUNIT_ASSERT_EQUAL(String("/file"), url.getPath());
    CPPUNIT_ASSERT_EQUAL(-1, url.getPort());
    CPPUNIT_ASSERT_EQUAL(String("fragment:80"), url.getRef());
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testSlashInFragment() {
    URL url("http://host/file#fragment/path");
    CPPUNIT_ASSERT_EQUAL(String("/file"), url.getFile());
    CPPUNIT_ASSERT_EQUAL(String("/file"), url.getPath());
    CPPUNIT_ASSERT_EQUAL(String("fragment/path"), url.getRef());
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testSlashInFragmentCombiningConstructor() {
    URL url("http", "host", "/file#fragment/path");
    CPPUNIT_ASSERT_EQUAL(String("/file"), url.getFile());
    CPPUNIT_ASSERT_EQUAL(String("/file"), url.getPath());
    CPPUNIT_ASSERT_EQUAL(String("fragment/path"), url.getRef());
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testHashInFragment() {
    URL url("http://host/file#fragment#another");
    CPPUNIT_ASSERT_EQUAL(String("/file"), url.getFile());
    CPPUNIT_ASSERT_EQUAL(String("/file"), url.getPath());
    CPPUNIT_ASSERT_EQUAL(String("fragment#another"), url.getRef());
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testEmptyPort() {
    URL url("http://host:/");
    CPPUNIT_ASSERT_EQUAL(-1, url.getPort());
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testNonNumericPort() {
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an MalformedURLException",
        URL("http://host:x/"),
        MalformedURLException);
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testNegativePort() {
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an MalformedURLException",
        URL("http://host:-2/"),
        MalformedURLException);
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testNegativePortEqualsPlaceholder() {
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an MalformedURLException",
        URL("http://host:-1/"),
        MalformedURLException);
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testRelativePathOnQuery() {
    URL base("http://host/file?query/x");
    URL url(base, "another");
    CPPUNIT_ASSERT_EQUAL(std::string("http://host/another"), url.toString());
    CPPUNIT_ASSERT_EQUAL(String("/another"), url.getFile());
    CPPUNIT_ASSERT_EQUAL(String("/another"), url.getPath());
    CPPUNIT_ASSERT_EQUAL(String(), url.getQuery());
    CPPUNIT_ASSERT_EQUAL(String(), url.getRef());
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testRelativeFragmentOnQuery() {
    URL base("http://host/file?query/x#fragment");
    URL url(base, "#another");
    CPPUNIT_ASSERT_EQUAL(std::string("http://host/file?query/x#another"), url.toString());
    CPPUNIT_ASSERT_EQUAL(String("/file?query/x"), url.getFile());
    CPPUNIT_ASSERT_EQUAL(String("/file"), url.getPath());
    CPPUNIT_ASSERT_EQUAL(String("query/x"), url.getQuery());
    CPPUNIT_ASSERT_EQUAL(String("another"), url.getRef());
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testPathContainsRelativeParts() {
    URL url("http://host/a/b/../c");
    CPPUNIT_ASSERT_EQUAL(std::string("http://host/a/c"), url.toString()); // RI doesn't canonicalize
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testRelativePathAndFragment() {
    URL base("http://host/file");
    CPPUNIT_ASSERT_EQUAL(std::string("http://host/another#fragment"),
                         URL(base, "another#fragment").toString());
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testRelativeParentDirectory() {
    URL base("http://host/a/b/c");
    CPPUNIT_ASSERT_EQUAL(std::string("http://host/a/d"), URL(base, "../d").toString());
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testRelativeChildDirectory() {
    URL base("http://host/a/b/c");
    CPPUNIT_ASSERT_EQUAL(std::string("http://host/a/b/d/e"), URL(base, "d/e").toString());
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testRelativeRootDirectory()  {
    URL base("http://host/a/b/c");
    CPPUNIT_ASSERT_EQUAL(std::string("http://host/d"), URL(base, "/d").toString());
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testRelativeFullUrl()  {
    URL base("http://host/a/b/c");
    CPPUNIT_ASSERT_EQUAL(std::string("http://host2/d/e"), URL(base, "http://host2/d/e").toString());
    CPPUNIT_ASSERT_EQUAL(std::string("https://host2/d/e"), URL(base, "https://host2/d/e").toString());
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testRelativeDifferentScheme()  {
    URL base("http://host/a/b/c");
    CPPUNIT_ASSERT_EQUAL(std::string("https://host2/d/e"), URL(base, "https://host2/d/e").toString());
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testRelativeDifferentAuthority()  {
    URL base("http://host/a/b/c");
    CPPUNIT_ASSERT_EQUAL(std::string("http://another/d/e"), URL(base, "//another/d/e").toString());
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testRelativeWithScheme()  {
    URL base("http://host/a/b/c");
    CPPUNIT_ASSERT_EQUAL(std::string("http://host/a/b/c"), URL(base, "http:").toString());
    CPPUNIT_ASSERT_EQUAL(std::string("http://host/"), URL(base, "http:/").toString());
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testMalformedUrlsRefusedByFirefoxAndChrome()  {
    URL base("http://host/a/b/c");
    // TODO
//    CPPUNIT_ASSERT_EQUAL(std::string("http://"), URL(base, "http://").toString()); // fails on RI; path retained
//    CPPUNIT_ASSERT_EQUAL(std::string("http://"), URL(base, "//").toString()); // fails on RI
    CPPUNIT_ASSERT_EQUAL(std::string("https:"), URL(base, "https:").toString());
    CPPUNIT_ASSERT_EQUAL(std::string("https:/"), URL(base, "https:/").toString());
//    CPPUNIT_ASSERT_EQUAL(std::string("https://"), URL(base, "https://").toString());
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testRfc1808NormalExamples()  {
    URL base("http://a/b/c/d;p?q");
    CPPUNIT_ASSERT_EQUAL(std::string("https:h"), URL(base, "https:h").toString());
    CPPUNIT_ASSERT_EQUAL(std::string("http://a/b/c/g"), URL(base, "g").toString());
    CPPUNIT_ASSERT_EQUAL(std::string("http://a/b/c/g"), URL(base, "./g").toString());
    CPPUNIT_ASSERT_EQUAL(std::string("http://a/b/c/g/"), URL(base, "g/").toString());
    CPPUNIT_ASSERT_EQUAL(std::string("http://a/g"), URL(base, "/g").toString());
    CPPUNIT_ASSERT_EQUAL(std::string("http://g"), URL(base, "//g").toString());
    CPPUNIT_ASSERT_EQUAL(std::string("http://a/b/c/d;p?y"), URL(base, "?y").toString()); // RI fails; file lost
    CPPUNIT_ASSERT_EQUAL(std::string("http://a/b/c/g?y"), URL(base, "g?y").toString());
    CPPUNIT_ASSERT_EQUAL(std::string("http://a/b/c/d;p?q#s"), URL(base, "#s").toString());
    CPPUNIT_ASSERT_EQUAL(std::string("http://a/b/c/g#s"), URL(base, "g#s").toString());
    CPPUNIT_ASSERT_EQUAL(std::string("http://a/b/c/g?y#s"), URL(base, "g?y#s").toString());
    CPPUNIT_ASSERT_EQUAL(std::string("http://a/b/c/;x"), URL(base, ";x").toString());
    CPPUNIT_ASSERT_EQUAL(std::string("http://a/b/c/g;x"), URL(base, "g;x").toString());
    CPPUNIT_ASSERT_EQUAL(std::string("http://a/b/c/g;x?y#s"), URL(base, "g;x?y#s").toString());
    CPPUNIT_ASSERT_EQUAL(std::string("http://a/b/c/d;p?q"), URL(base, "").toString());
    CPPUNIT_ASSERT_EQUAL(std::string("http://a/b/c/"), URL(base, ".").toString());
    CPPUNIT_ASSERT_EQUAL(std::string("http://a/b/c/"), URL(base, "./").toString());
    CPPUNIT_ASSERT_EQUAL(std::string("http://a/b/"), URL(base, "..").toString());
    CPPUNIT_ASSERT_EQUAL(std::string("http://a/b/"), URL(base, "../").toString());
    CPPUNIT_ASSERT_EQUAL(std::string("http://a/b/g"), URL(base, "../g").toString());
    CPPUNIT_ASSERT_EQUAL(std::string("http://a/"), URL(base, "../..").toString());
    CPPUNIT_ASSERT_EQUAL(std::string("http://a/"), URL(base, "../../").toString());
    CPPUNIT_ASSERT_EQUAL(std::string("http://a/g"), URL(base, "../../g").toString());
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testRfc1808AbnormalExampleTooManyDotDotSequences()  {
    URL base("http://a/b/c/d;p?q");
    CPPUNIT_ASSERT_EQUAL(std::string("http://a/g"), URL(base, "../../../g").toString()); // RI doesn't normalize
    CPPUNIT_ASSERT_EQUAL(std::string("http://a/g"), URL(base, "../../../../g").toString());
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testRfc1808AbnormalExampleRemoveDotSegments()  {
    URL base("http://a/b/c/d;p?q");
    CPPUNIT_ASSERT_EQUAL(std::string("http://a/g"), URL(base, "/./g").toString()); // RI doesn't normalize
    CPPUNIT_ASSERT_EQUAL(std::string("http://a/g"), URL(base, "/../g").toString()); // RI doesn't normalize
    CPPUNIT_ASSERT_EQUAL(std::string("http://a/b/c/g."), URL(base, "g.").toString());
    CPPUNIT_ASSERT_EQUAL(std::string("http://a/b/c/.g"), URL(base, ".g").toString());
    CPPUNIT_ASSERT_EQUAL(std::string("http://a/b/c/g.."), URL(base, "g..").toString());
    CPPUNIT_ASSERT_EQUAL(std::string("http://a/b/c/..g"), URL(base, "..g").toString());
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testRfc1808AbnormalExampleNonsensicalDots()  {
    URL base("http://a/b/c/d;p?q");
    CPPUNIT_ASSERT_EQUAL(std::string("http://a/b/g"), URL(base, "./../g").toString());
    CPPUNIT_ASSERT_EQUAL(std::string("http://a/b/c/g/"), URL(base, "./g/.").toString());
    CPPUNIT_ASSERT_EQUAL(std::string("http://a/b/c/g/h"), URL(base, "g/./h").toString());
    CPPUNIT_ASSERT_EQUAL(std::string("http://a/b/c/h"), URL(base, "g/../h").toString());
    CPPUNIT_ASSERT_EQUAL(std::string("http://a/b/c/g;x=1/y"), URL(base, "g;x=1/./y").toString());
    CPPUNIT_ASSERT_EQUAL(std::string("http://a/b/c/y"), URL(base, "g;x=1/../y").toString());
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testRfc1808AbnormalExampleRelativeScheme()  {
    URL base("http://a/b/c/d;p?q");
    // this result is permitted; strict parsers prefer "http:g"
    CPPUNIT_ASSERT_EQUAL(std::string("http://a/b/c/g"), URL(base, "http:g").toString());
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testRfc1808AbnormalExampleQueryOrFragmentDots()  {
    URL base("http://a/b/c/d;p?q");
    CPPUNIT_ASSERT_EQUAL(std::string("http://a/b/c/g?y/./x"), URL(base, "g?y/./x").toString());
    CPPUNIT_ASSERT_EQUAL(std::string("http://a/b/c/g?y/../x"), URL(base, "g?y/../x").toString());
    CPPUNIT_ASSERT_EQUAL(std::string("http://a/b/c/g#s/./x"), URL(base, "g#s/./x").toString());
    CPPUNIT_ASSERT_EQUAL(std::string("http://a/b/c/g#s/../x"), URL(base, "g#s/../x").toString());
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testSquareBracketsInUserInfo()  {
    URL url("http://user:[::1]@host");
    CPPUNIT_ASSERT_EQUAL(String("user:[::1]"), url.getUserInfo());
    CPPUNIT_ASSERT_EQUAL(String("host"), url.getHost());
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testComposeUrl()  {
    URL url("http", "host", "a");
    CPPUNIT_ASSERT_EQUAL(String("http"), url.getProtocol());
    CPPUNIT_ASSERT_EQUAL(String("host"), url.getAuthority());
    CPPUNIT_ASSERT_EQUAL(String("host"), url.getHost());
    CPPUNIT_ASSERT_EQUAL(String("/a"), url.getFile()); // RI fails; doesn't insert '/' separator
    CPPUNIT_ASSERT_EQUAL(std::string("http://host/a"), url.toString()); // fails on RI
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testComposeUrlWithNullHost()  {
    URL url("http", String(), "a");
    CPPUNIT_ASSERT_EQUAL(String("http"), url.getProtocol());
    CPPUNIT_ASSERT_EQUAL(String(), url.getAuthority());
    CPPUNIT_ASSERT_EQUAL(String(), url.getHost());
    CPPUNIT_ASSERT_EQUAL(String("a"), url.getFile());
    CPPUNIT_ASSERT_EQUAL(std::string("http:a"), url.toString()); // fails on RI
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testFileUrlExtraLeadingSlashes()  {
    URL url("file:////foo");
    CPPUNIT_ASSERT_EQUAL(String(), url.getAuthority()); // RI returns String()
    CPPUNIT_ASSERT_EQUAL(String("//foo"), url.getPath());
//    CPPUNIT_ASSERT_EQUAL(std::string("file:////foo"), url.toString());  TODO
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testFileUrlWithAuthority()  {
    URL url("file://x/foo");
    CPPUNIT_ASSERT_EQUAL(String("x"), url.getAuthority());
    CPPUNIT_ASSERT_EQUAL(String("/foo"), url.getPath());
    CPPUNIT_ASSERT_EQUAL(std::string("file://x/foo"), url.toString());
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testEmptyAuthority()  {
    URL url("http:///foo");
    CPPUNIT_ASSERT_EQUAL(String(), url.getAuthority());
    CPPUNIT_ASSERT_EQUAL(String("/foo"), url.getPath());
    //CPPUNIT_ASSERT_EQUAL(std::string("http:///foo"), url.toString()); // RI drops '//'  TODO
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testHttpUrlExtraLeadingSlashes() {
    URL url("http:////foo");
    CPPUNIT_ASSERT_EQUAL(String(), url.getAuthority()); // RI returns String()
    CPPUNIT_ASSERT_EQUAL(String("//foo"), url.getPath());
//    CPPUNIT_ASSERT_EQUAL(std::string("http:////foo"), url.toString());  TODO
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testFileUrlRelativePath() {
    URL base("file:a/b/c");
    CPPUNIT_ASSERT_EQUAL(std::string("file:a/b/d"), URL(base, "d").toString());
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testFileUrlDottedPath()  {
    URL url("file:../a/b");
    CPPUNIT_ASSERT_EQUAL(String("../a/b"), url.getPath());
    CPPUNIT_ASSERT_EQUAL(std::string("file:../a/b"), url.toString());
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testParsingDotAsHostname()  {
    URL url("http://./");
    CPPUNIT_ASSERT_EQUAL(String("."), url.getAuthority());
    CPPUNIT_ASSERT_EQUAL(String("."), url.getHost());
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testSquareBracketsWithIPv4()  {

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an MalformedURLException",
        URL("http://[192.168.0.1]/"),
        MalformedURLException);

    URL url("http", "[192.168.0.1]", "/");
    CPPUNIT_ASSERT_EQUAL(String("[192.168.0.1]"), url.getHost());
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testSquareBracketsWithHostname()  {

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an MalformedURLException",
        URL("http://[www.android.com]/"),
        MalformedURLException);

    URL url("http", "[www.android.com]", "/");
    CPPUNIT_ASSERT_EQUAL(String("[www.android.com]"), url.getHost());
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testIPv6WithoutSquareBrackets()  {

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an MalformedURLException",
        URL("http://fe80::1234/"),
        MalformedURLException);

    URL url("http", "fe80::1234", "/");
    CPPUNIT_ASSERT_EQUAL(String("[fe80::1234]"), url.getHost());
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testIpv6WithSquareBrackets()  {
    URL url("http://[::1]:2/");
    CPPUNIT_ASSERT_EQUAL(String("[::1]"), url.getHost());
    CPPUNIT_ASSERT_EQUAL(2, url.getPort());
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testEqualityWithNoPath()  {
    CPPUNIT_ASSERT(!URL("http://android.com").equals(URL("http://android.com/")));
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testUrlDoesNotEncodeParts()  {
    URL url("http", "host", 80, "/doc|search?q=green robots#over 6\"");
    CPPUNIT_ASSERT_EQUAL(String("http"), url.getProtocol());
    CPPUNIT_ASSERT_EQUAL(String("host:80"), url.getAuthority());
    CPPUNIT_ASSERT_EQUAL(String("/doc|search"), url.getPath());
    CPPUNIT_ASSERT_EQUAL(String("q=green robots"), url.getQuery());
    CPPUNIT_ASSERT_EQUAL(String("over 6\""), url.getRef());
    CPPUNIT_ASSERT_EQUAL(std::string("http://host:80/doc|search?q=green robots#over 6\""), url.toString());
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testSchemeCaseIsCanonicalized()  {
    URL url("HTTP://host/path");
    CPPUNIT_ASSERT_EQUAL(String("http"), url.getProtocol());
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testEmptyAuthorityWithPath()  {
    URL url("http:///path");
    CPPUNIT_ASSERT_EQUAL(String(), url.getAuthority());
    CPPUNIT_ASSERT_EQUAL(String("/path"), url.getPath());
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testEmptyAuthorityWithQuery()  {
    URL url("http://?query");
    CPPUNIT_ASSERT_EQUAL(String(), url.getAuthority());
    CPPUNIT_ASSERT_EQUAL(String(), url.getPath());
    CPPUNIT_ASSERT_EQUAL(String("query"), url.getQuery());
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testEmptyAuthorityWithFragment()  {
    URL url("http://#fragment");
    CPPUNIT_ASSERT_EQUAL(String(), url.getAuthority());
    CPPUNIT_ASSERT_EQUAL(String(), url.getPath());
    CPPUNIT_ASSERT_EQUAL(String("fragment"), url.getRef());
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testCombiningConstructorsMakeRelativePathsAbsolute()  {
    CPPUNIT_ASSERT_EQUAL(String("/relative"), URL("http", "host", "relative").getPath());
    CPPUNIT_ASSERT_EQUAL(String("/relative"), URL("http", "host", -1, "relative").getPath());
    CPPUNIT_ASSERT_EQUAL(String("/relative"), URL("http", "host", -1, "relative", NULL).getPath());
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testCombiningConstructorsDoNotMakeEmptyPathsAbsolute()  {
    CPPUNIT_ASSERT_EQUAL(String(), URL("http", "host", "").getPath());
    CPPUNIT_ASSERT_EQUAL(String(), URL("http", "host", -1, "").getPath());
    CPPUNIT_ASSERT_EQUAL(String(), URL("http", "host", -1, "", NULL).getPath());
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testPartContainsSpace() {

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an MalformedURLException",
        URL("ht tp://host/"),
        MalformedURLException);

    CPPUNIT_ASSERT_EQUAL(String("user name"), URL("http://user name@host/").getUserInfo());
    CPPUNIT_ASSERT_EQUAL(String("ho st"), URL("http://ho st/").getHost());

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an MalformedURLException",
        URL("http://host:80 80/"),
        MalformedURLException);

    CPPUNIT_ASSERT_EQUAL(String("/fi le"), URL("http://host/fi le").getFile());
    CPPUNIT_ASSERT_EQUAL(String("que ry"), URL("http://host/file?que ry").getQuery());
    CPPUNIT_ASSERT_EQUAL(String("re f"), URL("http://host/file?query#re f").getRef());
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testUnderscore() {
    URL url("http://a_b.c.d.net/");
    CPPUNIT_ASSERT_EQUAL(String("a_b.c.d.net"), url.getAuthority());
    // The RFC's don't permit underscores in hostnames, but URL accepts them (unlike URI).
    CPPUNIT_ASSERT_EQUAL(String("a_b.c.d.net"), url.getHost());
}
