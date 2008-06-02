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

#ifndef _DECAF_INTERNAL_NIO_INTARRAYBUFFERTEST_H_
#define _DECAF_INTERNAL_NIO_INTARRAYBUFFERTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <decaf/nio/IntBuffer.h>

namespace decaf{
namespace internal{
namespace nio{

    class IntArrayBufferTest : public CppUnit::TestFixture {

        CPPUNIT_TEST_SUITE( IntArrayBufferTest );
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
        CPPUNIT_TEST( testGetIntArray );
        CPPUNIT_TEST( testGetIntArray2 );
        CPPUNIT_TEST( testGetWithIndex );
        CPPUNIT_TEST( testPutInt );
        CPPUNIT_TEST( testPutIntArray );
        CPPUNIT_TEST( testPutIntArray2 );
        CPPUNIT_TEST( testPutIntBuffer );
        CPPUNIT_TEST( testPutIndexed );
        CPPUNIT_TEST( testSlice );
        CPPUNIT_TEST( testToString );
        CPPUNIT_TEST_SUITE_END();

        decaf::nio::IntBuffer* testBuffer1;
        int* testData1;

        static const std::size_t testData1Size = 100;
        static const std::size_t SMALL_TEST_LENGTH = 5;
        static const std::size_t BUFFER_LENGTH = 250;

    public:

        IntArrayBufferTest() {}
        virtual ~IntArrayBufferTest() {}

        void setUp() {
            testBuffer1 = decaf::nio::IntBuffer::allocate( testData1Size );

            testData1 = new int[testData1Size];
            for( std::size_t i = 0; i < testData1Size; ++i ){
                testData1[i] = (int)i;
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
        void testGetIntArray();
        void testGetIntArray2();
        void testGetWithIndex();
        void testPutInt();
        void testPutIntArray();
        void testPutIntArray2();
        void testPutIntBuffer();
        void testPutIndexed();
        void testSlice();
        void testToString();

    protected:

        void loadTestData1( int* array, std::size_t offset, std::size_t length ) {
            for( std::size_t i = 0; i < length; i++ ) {
                array[offset + i] = (int)i;
            }
        }

        void loadTestData2( int* array, std::size_t offset, std::size_t length ) {
            for( std::size_t i = 0; i < length; i++ ) {
                array[offset + i] = (int)(length - i);
            }
        }

        void loadTestData1( decaf::nio::IntBuffer* buf ) {
            buf->clear();
            for( std::size_t i = 0; i < buf->capacity(); i++ ) {
                buf->put( i, (int)i );
            }
        }

        void loadTestData2( decaf::nio::IntBuffer* buf ) {
            buf->clear();
            for( std::size_t i = 0; i < buf->capacity(); i++ ) {
                buf->put(i, (int)( buf->capacity() - i) );
            }
        }

        void assertContentEquals( decaf::nio::IntBuffer* buf, int* array,
                                  std::size_t offset, std::size_t length) {

            for( std::size_t i = 0; i < length; i++ ) {
                CPPUNIT_ASSERT( buf->get(i) == array[offset + i] );
            }
        }

        void assertContentEquals( decaf::nio::IntBuffer* buf,
                                  decaf::nio::IntBuffer* other ) {
            CPPUNIT_ASSERT( buf->capacity() == other->capacity() );
            for( std::size_t i = 0; i < buf->capacity(); i++ ) {
                CPPUNIT_ASSERT(buf->get(i) == other->get(i) );
            }
        }

        void assertContentLikeTestData1(
            decaf::nio::IntBuffer* buf, std::size_t startIndex,
            int startValue, std::size_t length ) {

            int value = startValue;
            for( std::size_t i = 0; i < length; i++ ) {
                CPPUNIT_ASSERT( buf->get( startIndex + i ) == value );
                value = value + 1;
            }
        }

    };

}}}

#endif /*_DECAF_INTERNAL_NIO_INTARRAYBUFFERTEST_H_*/
