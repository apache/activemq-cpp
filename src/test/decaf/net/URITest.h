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

#ifndef _DECAF_NET_URITEST_H_
#define _DECAF_NET_URITEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace decaf{
namespace net{

    class URITest : public CppUnit::TestFixture {

        CPPUNIT_TEST_SUITE( URITest );
        CPPUNIT_TEST( testConstructorOneString );
        CPPUNIT_TEST( testConstructorThreeString );
        CPPUNIT_TEST( testConstructorFourString );
        CPPUNIT_TEST( testConstructorFiveString );
        CPPUNIT_TEST( testConstructorFiveString2 );
        CPPUNIT_TEST( testConstructorStringPlusInts );
        CPPUNIT_TEST( testURIString );
        CPPUNIT_TEST( testCompareToOne );
        CPPUNIT_TEST( testCompareToTwo );
        CPPUNIT_TEST( testCreate );
        CPPUNIT_TEST( testEquals );
        CPPUNIT_TEST( testEquals2 );
        CPPUNIT_TEST( testGetAuthority );
        CPPUNIT_TEST( testGetAuthority2 );
        CPPUNIT_TEST( testGetFragment );
        CPPUNIT_TEST( testGetHost );
        CPPUNIT_TEST( testGetPath );
        CPPUNIT_TEST( testGetPort );
        CPPUNIT_TEST_SUITE_END();

    public:

        URITest();
        virtual ~URITest() {}

        void testURIString();
        void testConstructorOneString();
        void testConstructorThreeString();
        void testConstructorFourString();
        void testConstructorFiveString();
        void testConstructorFiveString2();
        void testConstructorStringPlusInts();
        void testCompareToOne();
        void testCompareToTwo();
        void testCreate();
        void testEquals();
        void testEquals2();
        void testGetAuthority();
        void testGetAuthority2();
        void testGetFragment();
        void testGetHost();
        void testGetPath();
        void testGetPort();

    };

}}

#endif /*_DECAF_NET_URITEST_H_*/
