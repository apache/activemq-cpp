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

#ifndef _DECAF_UTIL_HASHSETTEST_H_
#define _DECAF_UTIL_HASHSETTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace decaf {
namespace util {

    class HashSetTest : public CppUnit::TestFixture {

        CPPUNIT_TEST_SUITE( HashSetTest );
        CPPUNIT_TEST( testConstructor );
        CPPUNIT_TEST( testConstructorI );
        CPPUNIT_TEST( testConstructorIF );
        CPPUNIT_TEST( testConstructorCollection );
        CPPUNIT_TEST( testAdd );
        CPPUNIT_TEST( testClear );
        CPPUNIT_TEST( testContains );
        CPPUNIT_TEST( testIsEmpty );
        CPPUNIT_TEST( testIterator );
        CPPUNIT_TEST( testRemove );
        CPPUNIT_TEST( testSize );
        CPPUNIT_TEST( testToString );
        CPPUNIT_TEST( testToArray );
        CPPUNIT_TEST( testCopy1 );
        CPPUNIT_TEST( testCopy2 );
        CPPUNIT_TEST( testEquals );
        CPPUNIT_TEST( testRemoveAll );
        CPPUNIT_TEST( testRetainAll );
        CPPUNIT_TEST_SUITE_END();

    public:

        HashSetTest();
        virtual ~HashSetTest();

        void testConstructor();
        void testConstructorI();
        void testConstructorIF();
        void testConstructorCollection();
        void testAdd();
        void testClear();
        void testContains();
        void testIsEmpty();
        void testIterator();
        void testRemove();
        void testSize();
        void testToString();
        void testToArray();
        void testCopy1();
        void testCopy2();
        void testEquals();
        void testRemoveAll();
        void testRetainAll();

    };

}}

#endif /* _DECAF_UTIL_HASHSETTEST_H_ */
