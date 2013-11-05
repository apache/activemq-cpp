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

#ifndef _DECAF_NET_URLTEST_H_
#define _DECAF_NET_URLTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace decaf {
namespace net {

    class URLTest : public CppUnit::TestFixture {

        CPPUNIT_TEST_SUITE( URLTest );
        CPPUNIT_TEST( testConstructor1 );
        CPPUNIT_TEST( testConstructor2 );
        CPPUNIT_TEST( testConstructor3 );
        CPPUNIT_TEST( testConstructor4 );
        CPPUNIT_TEST( testEquals );
        CPPUNIT_TEST( testSameFile );
        CPPUNIT_TEST( testToString );
        CPPUNIT_TEST( testToExternalForm );
        CPPUNIT_TEST( testGetFile );
        CPPUNIT_TEST( testGetHost );
        CPPUNIT_TEST( testGetPort );
        CPPUNIT_TEST( testGetDefaultPort );
        CPPUNIT_TEST( testGetProtocol );
        CPPUNIT_TEST( testGetRef );
        CPPUNIT_TEST( testGetAuthority );
        CPPUNIT_TEST( testToURI );
        CPPUNIT_TEST( testURLStreamHandlerParseURL );
        CPPUNIT_TEST( testUrlParts );
        CPPUNIT_TEST( testFileEqualsWithEmptyHost );
        CPPUNIT_TEST_SUITE_END();

    public:

        URLTest();
        virtual ~URLTest();

        void testConstructor1();
        void testConstructor2();
        void testConstructor3();
        void testConstructor4();
        void testEquals();
        void testSameFile();
        void testToString();
        void testToExternalForm();
        void testGetFile();
        void testGetHost();
        void testGetPort();
        void testGetDefaultPort();
        void testGetProtocol();
        void testGetRef();
        void testGetAuthority();
        void testToURI();
        void testURLStreamHandlerParseURL();
        void testUrlParts();
        void testFileEqualsWithEmptyHost();

    };

}}

#endif /* _DECAF_NET_URLTEST_H_ */
