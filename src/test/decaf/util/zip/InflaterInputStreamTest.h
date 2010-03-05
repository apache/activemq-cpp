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

#ifndef _DECAF_UTIL_ZIP_INFLATERINPUTSTREAMTEST_H_
#define _DECAF_UTIL_ZIP_INFLATERINPUTSTREAMTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace decaf {
namespace util {
namespace zip {

    class InflaterInputStreamTest : public CppUnit::TestFixture {

        CPPUNIT_TEST_SUITE( InflaterInputStreamTest );
        CPPUNIT_TEST( testConstructorInputStreamInflater );
        CPPUNIT_TEST( testConstructorInputStreamInflaterI );
        CPPUNIT_TEST( testMark );
        CPPUNIT_TEST( testMarkSupported );
        CPPUNIT_TEST( testRead );
        CPPUNIT_TEST( testAvailableNonEmptySource );
        CPPUNIT_TEST( testAvailableSkip );
        CPPUNIT_TEST( testAvailableEmptySource );
        CPPUNIT_TEST( testReadBIII );
        CPPUNIT_TEST( testReadBIII2 );
        CPPUNIT_TEST( testReadBIII3 );
        CPPUNIT_TEST( testReset );
        CPPUNIT_TEST( testClose );
        CPPUNIT_TEST( testSkip );
        CPPUNIT_TEST( testSkip2 );
        CPPUNIT_TEST_SUITE_END();

    private:

        static const std::string testString;

        std::vector<unsigned char> deflatedData;
        std::vector<unsigned char> inputBuffer;

    public:

        InflaterInputStreamTest();
        virtual ~InflaterInputStreamTest();

        void setUp();

        void testConstructorInputStreamInflater();
        void testConstructorInputStreamInflaterI();
        void testMark();
        void testMarkSupported();
        void testRead();
        void testAvailableNonEmptySource();
        void testAvailableSkip();
        void testAvailableEmptySource();
        void testReadBIII();
        void testReadBIII2();
        void testReadBIII3();
        void testReset();
        void testClose();
        void testAvailable();
        void testSkip();
        void testSkip2();

    };

}}}

#endif /* _DECAF_UTIL_ZIP_INFLATERINPUTSTREAMTEST_H_ */
