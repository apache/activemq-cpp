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

#ifndef _DECAF_UTIL_ZIP_INFLATERTEST_H_
#define _DECAF_UTIL_ZIP_INFLATERTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace decaf {
namespace util {
namespace zip {

    class InflaterTest : public CppUnit::TestFixture {

        CPPUNIT_TEST_SUITE( InflaterTest );
        CPPUNIT_TEST( testEnd );
        CPPUNIT_TEST( testFinished );
        CPPUNIT_TEST( testGetAdler );
        CPPUNIT_TEST( testGetRemaining );
        CPPUNIT_TEST( testInflateVector );
        CPPUNIT_TEST( testInflateB1 );
        CPPUNIT_TEST( testInflateBII );
        CPPUNIT_TEST( testInflateBII1 );
        CPPUNIT_TEST( testConstructor );
        CPPUNIT_TEST( testConstructorZ );
        CPPUNIT_TEST( testNeedsDictionary );
        CPPUNIT_TEST( testNeedsInput );
        CPPUNIT_TEST( testReset );
        CPPUNIT_TEST( testSetInputB );
        CPPUNIT_TEST( testSetInputBIII );
        CPPUNIT_TEST( testGetBytesRead );
        CPPUNIT_TEST( testGetBytesWritten );
        CPPUNIT_TEST( testInflate );
        CPPUNIT_TEST( testSetDictionaryB );
        CPPUNIT_TEST( testSetDictionaryBIII );
        CPPUNIT_TEST_SUITE_END();

    public:

        InflaterTest();
        virtual ~InflaterTest();

        virtual void setUp();
        virtual void tearDown();

        void testEnd();
        void testFinished();
        void testGetAdler();
        void testGetRemaining();
        void testInflateVector();
        void testInflateB1();
        void testInflateBII();
        void testInflateBII1();
        void testConstructor();
        void testConstructorZ();
        void testNeedsDictionary();
        void testNeedsInput();
        void testReset();
        void testSetInputB();
        void testSetInputBIII();
        void testGetBytesRead();
        void testGetBytesWritten();
        void testInflate();
        void testSetDictionaryB();
        void testSetDictionaryBIII();

    };

}}}

#endif /* _DECAF_UTIL_ZIP_INFLATERTEST_H_ */
