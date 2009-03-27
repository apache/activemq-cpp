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

#ifndef _DECAF_INTERNAL_UTIL_BYTEARRAYTEST_H_
#define _DECAF_INTERNAL_UTIL_BYTEARRAYTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <decaf/internal/util/ByteArrayAdapter.h>
#include <decaf/util/Config.h>

namespace decaf{
namespace internal{
namespace util{

    class ByteArrayAdapterTest  : public CppUnit::TestFixture {

        CPPUNIT_TEST_SUITE( ByteArrayAdapterTest );
        CPPUNIT_TEST( testRead );
        CPPUNIT_TEST( testReadExceptions );
        CPPUNIT_TEST( testWrite );
        CPPUNIT_TEST( testWriteExceptions );
        CPPUNIT_TEST( testCtor1 );
        CPPUNIT_TEST( testCtor2 );
        CPPUNIT_TEST( testClear );
        CPPUNIT_TEST( testReszie );
        CPPUNIT_TEST( testOperators );
        CPPUNIT_TEST( testOperatorsExceptions );
        CPPUNIT_TEST( testArray );
        CPPUNIT_TEST( testGet );
        CPPUNIT_TEST( testGetChar );
        CPPUNIT_TEST( testGetShort );
        CPPUNIT_TEST( testGetInt );
        CPPUNIT_TEST( testGetLong );
        CPPUNIT_TEST( testGetDouble );
        CPPUNIT_TEST( testGetFloat );
        CPPUNIT_TEST( testPut );
        CPPUNIT_TEST( testPutChar );
        CPPUNIT_TEST( testPutShort );
        CPPUNIT_TEST( testPutInt );
        CPPUNIT_TEST( testPutLong );
        CPPUNIT_TEST( testPutDouble );
        CPPUNIT_TEST( testPutFloat );
        CPPUNIT_TEST_SUITE_END();

        unsigned char* testData1;
        static const std::size_t testData1Size = 100;

    public:

        ByteArrayAdapterTest() {}
        virtual ~ByteArrayAdapterTest() {}

        void setUp() {
            testData1 = new unsigned char[testData1Size];
            for( std::size_t i = 0; i < testData1Size; ++i ){
                testData1[i] = (unsigned char)i;
            }
        }

        void tearDown() {
            delete [] testData1;
        }

        void testRead();
        void testWrite();
        void testCtor1();
        void testCtor2();
        void testClear();
        void testReszie();
        void testOperators();
        void testReadExceptions();
        void testWriteExceptions();
        void testOperatorsExceptions();
        void testArray();
        void testGet();
        void testGetChar();
        void testGetShort();
        void testGetInt();
        void testGetLong();
        void testGetDouble();
        void testGetFloat();
        void testPut();
        void testPutChar();
        void testPutShort();
        void testPutInt();
        void testPutLong();
        void testPutDouble();
        void testPutFloat();

    };

}}}

#endif /*_DECAF_INTERNAL_UTIL_BYTEARRAYTEST_H_*/
