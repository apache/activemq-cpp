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

#ifndef _DECAF_INTERNAL_NIO_CHARARRAYBUFFERTEST_H_
#define _DECAF_INTERNAL_NIO_CHARARRAYBUFFERTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <decaf/nio/CharBuffer.h>

namespace decaf{
namespace internal{
namespace nio{

    class CharArrayBufferTest : public CppUnit::TestFixture {

        CPPUNIT_TEST_SUITE( CharArrayBufferTest );
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
        CPPUNIT_TEST( testPutCharBuffer );
        CPPUNIT_TEST( testPutIndexed );
        CPPUNIT_TEST( testSlice );
        CPPUNIT_TEST( testToString );
        CPPUNIT_TEST( testWrapNullArray );
        CPPUNIT_TEST( testCharAt );
        CPPUNIT_TEST( testLength );
        CPPUNIT_TEST( testSubSequence );
        CPPUNIT_TEST( testPutString );
        CPPUNIT_TEST( testPutStringWithArgs );
        CPPUNIT_TEST( testAppendSelf );
        CPPUNIT_TEST( testAppendOverFlow );
        CPPUNIT_TEST( testReadOnlyMap );
        CPPUNIT_TEST( testAppendCNormal );
        CPPUNIT_TEST( testAppendCharSequenceNormal );
        CPPUNIT_TEST( testAppendCharSequenceIINormal );
        CPPUNIT_TEST( testAppendCharSequenceII_IllegalArgument );
        CPPUNIT_TEST( testReadCharBuffer );
        CPPUNIT_TEST( testReadReadOnly );
        CPPUNIT_TEST( testReadOverflow );
        CPPUNIT_TEST( testReadSelf );
        CPPUNIT_TEST_SUITE_END();

        decaf::nio::CharBuffer* testBuffer1;
        char* testData1;

        static const std::size_t testData1Size = 100;
        static const std::size_t SMALL_TEST_LENGTH = 5;
        static const std::size_t BUFFER_LENGTH = 250;

        class MyCharSequence : public lang::CharSequence {
        private:

            std::string value;

        public:

            MyCharSequence( std::string value ) {
                this->value = value;
            }

            virtual ~MyCharSequence() {}

            virtual std::size_t length() const {
                return this->value.length();
            }

            virtual char charAt( std::size_t index ) const
                throw( lang::exceptions::IndexOutOfBoundsException ) {

                if( index > this->value.length() ) {
                    throw decaf::lang::exceptions::IndexOutOfBoundsException(
                        __FILE__, __LINE__,
                        "MyCharSequence::charAt - index is to big: %d", index );
                }

                return this->value.at( index );
            }

            virtual CharSequence* subSequence( std::size_t start, std::size_t end ) const
                throw( lang::exceptions::IndexOutOfBoundsException ) {

                if( start > end ) {
                    throw decaf::lang::exceptions::IndexOutOfBoundsException(
                        __FILE__, __LINE__,
                        "CharArrayBuffer::subSequence - start > end" );
                }

                if( start > this->length() || end > this->length() ) {
                    throw decaf::lang::exceptions::IndexOutOfBoundsException(
                        __FILE__, __LINE__,
                        "CharArrayBuffer::subSequence - Sequence exceed limit" );
                }

                return new MyCharSequence( this->value.substr( start, end - start ) );
            }

            virtual std::string toString() const {
                return this->value;
            }
        };

    public:

        CharArrayBufferTest() {}
        virtual ~CharArrayBufferTest() {}

        void setUp() {
            testBuffer1 = decaf::nio::CharBuffer::allocate( testData1Size );

            testData1 = new char[testData1Size];
            for( std::size_t i = 0; i < testData1Size; ++i ){
                testData1[i] = (char)i;
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
        void testPutCharBuffer();
        void testPutIndexed();
        void testSlice();
        void testToString();
        void testWrapNullArray();
        void testCharAt();
        void testLength();
        void testSubSequence();
        void testPutString();
        void testPutStringWithArgs();
        void testAppendSelf();
        void testAppendOverFlow();
        void testReadOnlyMap();
        void testAppendCNormal();
        void testAppendCharSequenceNormal();
        void testAppendCharSequenceIINormal();
        void testAppendCharSequenceII_IllegalArgument();
        void testReadCharBuffer();
        void testReadReadOnly();
        void testReadOverflow();
        void testReadSelf();

    };

}}}

#endif /*_DECAF_INTERNAL_NIO_CHARARRAYBUFFERTEST_H_*/
