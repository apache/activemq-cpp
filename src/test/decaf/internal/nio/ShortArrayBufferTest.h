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

#ifndef _DECAF_INTERNAL_NIO_SHORTARRAYBUFFERTEST_H_
#define _DECAF_INTERNAL_NIO_SHORTARRAYBUFFERTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <decaf/nio/ShortBuffer.h>

namespace decaf{
namespace internal{
namespace nio{

    class ShortArrayBufferTest : public CppUnit::TestFixture {

        CPPUNIT_TEST_SUITE( ShortArrayBufferTest );
        CPPUNIT_TEST( test );
        CPPUNIT_TEST( testArray );
        CPPUNIT_TEST( testArrayOffset );
        CPPUNIT_TEST( testReadOnlyArray );
        CPPUNIT_TEST( testAsReadOnlyBuffer );
        CPPUNIT_TEST( testCompact );
        CPPUNIT_TEST( testCompareTo );
        CPPUNIT_TEST( testDuplicate );
        CPPUNIT_TEST( testEquals );
        CPPUNIT_TEST( testHasArray );
        CPPUNIT_TEST( testGet );
        CPPUNIT_TEST( testGet2 );
        CPPUNIT_TEST( testGetShortArray );
        CPPUNIT_TEST( testGetShortArray2 );
        CPPUNIT_TEST( testGetWithIndex );
        CPPUNIT_TEST( testPutShort );
        CPPUNIT_TEST( testPutShortArray );
        CPPUNIT_TEST( testPutShortArray2 );
        CPPUNIT_TEST( testPutShortBuffer );
        CPPUNIT_TEST( testPutIndexed );
        CPPUNIT_TEST( testSlice );
        CPPUNIT_TEST( testToString );
        CPPUNIT_TEST_SUITE_END();

        decaf::nio::ShortBuffer* testBuffer1;
        short* testData1;

        static const std::size_t testData1Size = 100;
        static const std::size_t SMALL_TEST_LENGTH = 5;
        static const std::size_t BUFFER_LENGTH = 250;

    public:

        ShortArrayBufferTest() {}
        virtual ~ShortArrayBufferTest() {}

        void setUp() {
            testBuffer1 = decaf::nio::ShortBuffer::allocate( testData1Size );

            testData1 = new short[testData1Size];
            for( std::size_t i = 0; i < testData1Size; ++i ){
                testData1[i] = (short)i;
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
        void testHasArray();
        void testGet();
        void testGet2();
        void testGetShortArray();
        void testGetShortArray2();
        void testGetWithIndex();
        void testPutShort();
        void testPutShortArray();
        void testPutShortArray2();
        void testPutShortBuffer();
        void testPutIndexed();
        void testSlice();
        void testToString();

    protected:

        void loadTestData1( short* array, std::size_t offset, std::size_t length ) {
            for( std::size_t i = 0; i < length; i++ ) {
                array[offset + i] = (short)i;
            }
        }

        void loadTestData2( short* array, std::size_t offset, std::size_t length ) {
            for( std::size_t i = 0; i < length; i++ ) {
                array[offset + i] = (short)(length - i);
            }
        }

        void loadTestData1( decaf::nio::ShortBuffer* buf ) {
            buf->clear();
            for( std::size_t i = 0; i < buf->capacity(); i++ ) {
                buf->put( i, (short)i );
            }
        }

        void loadTestData2( decaf::nio::ShortBuffer* buf ) {
            buf->clear();
            for( std::size_t i = 0; i < buf->capacity(); i++ ) {
                buf->put(i, (short)( buf->capacity() - i) );
            }
        }

        void assertContentEquals( decaf::nio::ShortBuffer* buf, short* array,
                                  std::size_t offset, std::size_t length) {

            for( std::size_t i = 0; i < length; i++ ) {
                CPPUNIT_ASSERT( buf->get(i) == array[offset + i] );
            }
        }

        void assertContentEquals( decaf::nio::ShortBuffer* buf,
                                  decaf::nio::ShortBuffer* other ) {
            CPPUNIT_ASSERT( buf->capacity() == other->capacity() );
            for( std::size_t i = 0; i < buf->capacity(); i++ ) {
                CPPUNIT_ASSERT(buf->get(i) == other->get(i) );
            }
        }

        void assertContentLikeTestData1(
            decaf::nio::ShortBuffer* buf, std::size_t startIndex,
            short startValue, std::size_t length ) {

            short value = startValue;
            for( std::size_t i = 0; i < length; i++ ) {
                CPPUNIT_ASSERT( buf->get( startIndex + i ) == value );
                value = value + 1;
            }
        }

    };

}}}

#endif /*_DECAF_INTERNAL_NIO_SHORTARRAYBUFFERTEST_H_*/
