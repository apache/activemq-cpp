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

using namespace std;
using namespace decaf;
using namespace decaf::net;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

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
    CPPUNIT_ASSERT_EQUAL_MESSAGE("u2a returns a wrong protocol", String("file"), d.getProtocol());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("u2a returns a wrong host", String("www.yahoo3.com"), d.getHost());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("u2a returns a wrong port", -1, d.getPort());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("u2a returns a wrong file", String("/dir1/dir2/test.cgi"), d.getFile());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("u2a returns a wrong anchor", String("anchor1"), d.getRef());

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

    URL q("file:////file.txt");
    CPPUNIT_ASSERT_MESSAGE("q returns a wrong authority ", q.getAuthority().isEmpty());
    CPPUNIT_ASSERT_MESSAGE("q returns a wrong file ", q.getFile().equals("////file.txt"));

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
    CPPUNIT_ASSERT(u.getFile().equals("test.html"));
    CPPUNIT_ASSERT(u.getRef().equals("foo"));

    // Strange behavior in reference, the hostname contains a ':' so it gets
    // wrapped in '[', ']'
    URL testURL("http", "www.apache.org:8080", "test.html#anch");
    CPPUNIT_ASSERT_EQUAL_MESSAGE("wrong protocol", String("http"), testURL.getProtocol());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("wrong host", String("[www.apache.org:8080]"), testURL.getHost());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("wrong port", -1, testURL.getPort());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("wrong file", String("test.html"), testURL.getFile());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("wrong anchor", String("anch"), testURL.getRef());
}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testConstructor3() {

    URL u("http", "www.yahoo.com", 8080, "test.html#foo");
    CPPUNIT_ASSERT_EQUAL_MESSAGE("SSIS returns a wrong protocol", String("http"), u.getProtocol());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("SSIS returns a wrong host", String("www.yahoo.com"), u.getHost());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("SSIS returns a wrong port", 8080, u.getPort());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("SSIS returns a wrong file", String("test.html"), u.getFile());
    CPPUNIT_ASSERT_MESSAGE("SSIS returns a wrong anchor: ", u.getRef().equals("foo"));

    CPPUNIT_ASSERT_NO_THROW(URL("http", "apache.org", 123456789, "file"));

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown MalformedURLException",
        URL("http", "apache.org", -123, "file"),
        MalformedURLException);
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
