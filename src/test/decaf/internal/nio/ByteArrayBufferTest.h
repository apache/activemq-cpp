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

#ifndef _DECAF_INTERNAL_NIO_BYTEBUFFERTEST_H_
#define _DECAF_INTERNAL_NIO_BYTEBUFFERTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <decaf/nio/ByteBuffer.h>

namespace decaf{
namespace internal{
namespace nio{

    class ByteArrayBufferTest : public CppUnit::TestFixture {

        CPPUNIT_TEST_SUITE( ByteArrayBufferTest );
        CPPUNIT_TEST( test );
        CPPUNIT_TEST( testArray );
        CPPUNIT_TEST( testArrayOffset );
        CPPUNIT_TEST( testReadOnlyArray );
        CPPUNIT_TEST( testAsReadOnlyBuffer );
        CPPUNIT_TEST( testCompact );
        CPPUNIT_TEST( testCompareTo );
        CPPUNIT_TEST( testDuplicate );
        CPPUNIT_TEST( testEquals );
        CPPUNIT_TEST( testGet );
        CPPUNIT_TEST( testGetbyteArray );
        CPPUNIT_TEST( testGetbyteArray2 );
        CPPUNIT_TEST( testGetWithIndex );
        CPPUNIT_TEST( testPutbyte );
        CPPUNIT_TEST( testPutbyteArray );
        CPPUNIT_TEST( testPutbyteArray2 );
        CPPUNIT_TEST( testPutByteBuffer );
        CPPUNIT_TEST( testPutIndexed );
        CPPUNIT_TEST( testSlice );
        CPPUNIT_TEST( testToString );
        CPPUNIT_TEST( testGetChar );
        CPPUNIT_TEST( testGetChar2 );
        CPPUNIT_TEST( testPutChar );
        CPPUNIT_TEST( testPutChar2 );
        CPPUNIT_TEST( testGetDouble );
        CPPUNIT_TEST( testGetDouble2 );
        CPPUNIT_TEST( testPutDouble );
        CPPUNIT_TEST( testPutDouble2 );
        CPPUNIT_TEST( testGetFloat );
        CPPUNIT_TEST( testGetFloat2 );
        CPPUNIT_TEST( testPutFloat );
        CPPUNIT_TEST( testPutFloat2 );
        CPPUNIT_TEST( testGetLong );
        CPPUNIT_TEST( testGetLong2 );
        CPPUNIT_TEST( testPutLong );
        CPPUNIT_TEST( testPutLong2 );
        CPPUNIT_TEST( testGetInt );
        CPPUNIT_TEST( testGetInt2 );
        CPPUNIT_TEST( testPutInt );
        CPPUNIT_TEST( testPutInt2 );
        CPPUNIT_TEST( testGetShort );
        CPPUNIT_TEST( testGetShort2 );
        CPPUNIT_TEST( testPutShort );
        CPPUNIT_TEST( testPutShort2 );
        CPPUNIT_TEST( testWrapNullArray );
        CPPUNIT_TEST_SUITE_END();

    private:

        decaf::nio::ByteBuffer* testBuffer1;
        unsigned char* testData1;

        static const std::size_t testData1Size = 100;
        static const std::size_t SMALL_TEST_LENGTH = 5;
        static const std::size_t BUFFER_LENGTH = 250;

    public:

        ByteArrayBufferTest() {}
        virtual ~ByteArrayBufferTest() {}

        void setUp() {
           testBuffer1 = decaf::nio::ByteBuffer::allocate( testData1Size );

            testData1 = new unsigned char[testData1Size];
            for( std::size_t i = 0; i < testData1Size; ++i ){
                testData1[i] = (unsigned char)i;
            }
        }

        void tearDown() {
            delete testBuffer1;
            delete [] testData1;
        }

        void test();
        void testArray();
        void testArrayOffset();
        void testReadOnlyArray();
        void testAsReadOnlyBuffer();
        void testCompact();
        void testCompareTo();
        void testDuplicate();
        void testEquals();
        void testGet();
        void testGetbyteArray();
        void testGetbyteArray2();
        void testGetWithIndex();
        void testPutbyte();
        void testPutbyteArray();
        void testPutbyteArray2();
        void testPutByteBuffer();
        void testPutIndexed();
        void testSlice();
        void testToString();
        void testGetChar();
        void testGetChar2();
        void testPutChar();
        void testPutChar2();
        void testGetDouble();
        void testGetDouble2();
        void testPutDouble();
        void testPutDouble2();
        void testGetFloat();
        void testGetFloat2();
        void testPutFloat();
        void testPutFloat2();
        void testGetLong();
        void testGetLong2();
        void testPutLong();
        void testPutLong2();
        void testGetInt();
        void testGetInt2();
        void testPutInt();
        void testPutInt2();
        void testGetShort();
        void testGetShort2();
        void testPutShort();
        void testPutShort2();
        void testWrapNullArray();

    };

}}}

#endif /*_DECAF_INTERNAL_NIO_BYTEBUFFERTEST_H_*/
