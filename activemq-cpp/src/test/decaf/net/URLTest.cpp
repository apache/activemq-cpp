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
void URLTest::testConstructorString() {

    URL u("http://www.yahoo1.com:8080/dir1/dir2/test.cgi?point1.html#anchor1");

    CPPUNIT_ASSERT_EQUAL_MESSAGE("u returns a wrong protocol", String("http"), u.getProtocol());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("u returns a wrong host", String("www.yahoo1.com"), u.getHost());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("u returns a wrong port", 8080, u.getPort());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("u returns a wrong file",
                                 String("/dir1/dir2/test.cgi?point1.html"), u.getFile());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("u returns a wrong anchor", String("anchor1"), u.getRef());

}

////////////////////////////////////////////////////////////////////////////////
void URLTest::testEquals() {
}
