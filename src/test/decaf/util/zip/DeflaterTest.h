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

#ifndef _DECAF_UTIL_ZIP_DEFLATERTEST_H_
#define _DECAF_UTIL_ZIP_DEFLATERTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <decaf/util/zip/Deflater.h>

namespace decaf {
namespace util {
namespace zip {

    class DeflaterTest : public CppUnit::TestFixture {

        CPPUNIT_TEST_SUITE( DeflaterTest );
        CPPUNIT_TEST( testDeflateVector );
        CPPUNIT_TEST( testDeflateArray );
        CPPUNIT_TEST( testEnd );
        CPPUNIT_TEST( testInitialState );
        CPPUNIT_TEST( testDeflateBeforeSetInput );
        CPPUNIT_TEST( testGetBytesRead );
        CPPUNIT_TEST( testGetBytesWritten );
        CPPUNIT_TEST( testFinish );
        CPPUNIT_TEST( testFinished );
        CPPUNIT_TEST( testGetAdler );
        CPPUNIT_TEST( testNeedsInput );
        CPPUNIT_TEST( testReset );
        CPPUNIT_TEST( testConstructor );
        CPPUNIT_TEST( testConstructorI );
        CPPUNIT_TEST( testConstructorIB );
        CPPUNIT_TEST( testSetDictionaryVector );
        CPPUNIT_TEST( testSetDictionaryBIII );
        CPPUNIT_TEST( testSetInputVector );
        CPPUNIT_TEST( testSetInputBIII );
        CPPUNIT_TEST( testSetLevel );
        CPPUNIT_TEST( testSetStrategy );
        CPPUNIT_TEST_SUITE_END();

    public:

        DeflaterTest();
        virtual ~DeflaterTest();

        void testDeflateVector();
        void testDeflateArray();
        void testEnd();
        void testInitialState();
        void testDeflateBeforeSetInput();
        void testGetBytesRead();
        void testGetBytesWritten();
        void testFinish();
        void testFinished();
        void testGetAdler();
        void testNeedsInput();
        void testReset();
        void testConstructor();
        void testConstructorI();
        void testConstructorIB();
        void testSetDictionaryVector();
        void testSetDictionaryBIII();
        void testSetInputVector();
        void testSetInputBIII();
        void testSetLevel();
        void testSetStrategy();

    private:

        void helperEndTest( Deflater& defl, const std::string& testName );

    };

}}}

#endif /* _DECAF_UTIL_ZIP_DEFLATERTEST_H_ */
