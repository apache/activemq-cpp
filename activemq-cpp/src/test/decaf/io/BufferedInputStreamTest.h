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

#ifndef _DECAF_IO_BUFFEREDINPUTSTREAMTEST_H_
#define _DECAF_IO_BUFFEREDINPUTSTREAMTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <decaf/io/BufferedInputStream.h>
#include <decaf/util/Config.h>

namespace decaf{
namespace io{

    class BufferedInputStreamTest : public CppUnit::TestFixture {

        CPPUNIT_TEST_SUITE( BufferedInputStreamTest );
        CPPUNIT_TEST( testSmallerBuffer );
        CPPUNIT_TEST( testBiggerBuffer );
        CPPUNIT_TEST( testConstructor );
        CPPUNIT_TEST( testAvailable );
        CPPUNIT_TEST( testClose );
        CPPUNIT_TEST( testRead );
        CPPUNIT_TEST( testRead2 );
        CPPUNIT_TEST( testReadException );
        CPPUNIT_TEST( testSkipNullInputStream );
        CPPUNIT_TEST( testMarkSupported );
        CPPUNIT_TEST( testResetScenario1 );
        CPPUNIT_TEST( testResetScenario2 );
        CPPUNIT_TEST( testResetException );
        CPPUNIT_TEST( testReset );
        CPPUNIT_TEST( testMarkI );
        CPPUNIT_TEST( testSkipJ );
        CPPUNIT_TEST_SUITE_END();

    private:

        static const std::string testString;

    public:

        virtual ~BufferedInputStreamTest(){}
        virtual void setUp(){}
        virtual void tearDown(){}

        void testSmallerBuffer();
        void testBiggerBuffer();
        void testConstructor();
        void testAvailable();
        void testClose();
        void testRead();
        void testReadException();
        void testRead2();
        void testMarkSupported();
        void testSkipNullInputStream();
        void testResetScenario1();
        void testResetScenario2();
        void testResetException();
        void testReset();
        void testMarkI();
        void testSkipJ();

    };

}}

#endif /*_DECAF_IO_BUFFEREDINPUTSTREAMTEST_H_*/
