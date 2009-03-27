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

#ifndef _DECAF_NIO_BUFFERTEST_H_
#define _DECAF_NIO_BUFFERTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <decaf/nio/Buffer.h>

namespace decaf{
namespace nio{

    class BufferTest : public CppUnit::TestFixture {

        CPPUNIT_TEST_SUITE( BufferTest );
        CPPUNIT_TEST( test );
        CPPUNIT_TEST( testCapacity );
        CPPUNIT_TEST( testClear );
        CPPUNIT_TEST( testFlip );
        CPPUNIT_TEST( testHasRemaining );
        CPPUNIT_TEST( testIsReadOnly );
        CPPUNIT_TEST( testLimit );
        CPPUNIT_TEST( testLimitInt );
        CPPUNIT_TEST( testMark );
        CPPUNIT_TEST( testPosition );
        CPPUNIT_TEST( testPositionInt );
        CPPUNIT_TEST( testRemaining );
        CPPUNIT_TEST( testReset );
        CPPUNIT_TEST( testRewind );
        CPPUNIT_TEST_SUITE_END();

    private:

        Buffer* buffer;

        static const std::size_t DEFAULT_BUFFER_SIZE = 512;

        class MyBuffer : public Buffer {
        public:

            MyBuffer( std::size_t capacity ) : Buffer( capacity ) {
            }

            virtual ~MyBuffer() {}

            bool isReadOnly() const { return false; }
        };

    public:

        BufferTest() {}
        virtual ~BufferTest() {}

        void setUp() {
            buffer = new MyBuffer( DEFAULT_BUFFER_SIZE );
        }

        void tearDown() {
            delete buffer;
            buffer = NULL;
        }

        void test();
        void testCapacity();
        void testClear();
        void testFlip();
        void testHasRemaining();
        void testIsReadOnly();
        void testLimit();
        void testLimitInt();
        void testMark();
        void testPosition();
        void testPositionInt();
        void testRemaining();
        void testReset();
        void testRewind();

    };

}}

#endif /*_DECAF_NIO_BUFFERTEST_H_*/
