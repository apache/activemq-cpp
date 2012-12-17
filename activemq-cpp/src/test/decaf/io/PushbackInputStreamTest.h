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

#ifndef _DECAF_IO_PUSHBACKINPUTSTREAMTEST_H_
#define _DECAF_IO_PUSHBACKINPUTSTREAMTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace decaf {
namespace io {

    class PushbackInputStreamTest : public CppUnit::TestFixture {

        CPPUNIT_TEST_SUITE( PushbackInputStreamTest );
        CPPUNIT_TEST( testReset );
        CPPUNIT_TEST( testMark );
        CPPUNIT_TEST( testMarkSupported );
        CPPUNIT_TEST( testAvailable );
        CPPUNIT_TEST( testConstructor1 );
        CPPUNIT_TEST( testConstructor2 );
        CPPUNIT_TEST( testConstructor3 );
        CPPUNIT_TEST( testRead );
        CPPUNIT_TEST( testReadBIII );
        CPPUNIT_TEST( testSkip );
        CPPUNIT_TEST( testUnreadBI );
        CPPUNIT_TEST( testUnreadBIII );
        CPPUNIT_TEST( testUnread );
        CPPUNIT_TEST_SUITE_END();

    private:

        static std::string testString;

    public:

        PushbackInputStreamTest();
        virtual ~PushbackInputStreamTest();

        void testReset();
        void testMark();
        void testMarkSupported();
        void testAvailable();
        void testConstructor1();
        void testConstructor2();
        void testConstructor3();
        void testRead();
        void testReadBIII();
        void testSkip();
        void testUnreadBI();
        void testUnreadBIII();
        void testUnread();

    };

}}

#endif /* _DECAF_IO_PUSHBACKINPUTSTREAMTEST_H_ */
