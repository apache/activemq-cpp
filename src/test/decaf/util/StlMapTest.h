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

#ifndef _DECAF_UTIL_STLMAPTEST_H_
#define _DECAF_UTIL_STLMAPTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace decaf {
namespace util {

    class StlMapTest : public CppUnit::TestFixture
    {
        CPPUNIT_TEST_SUITE( StlMapTest );
        CPPUNIT_TEST( testConstructor );
        CPPUNIT_TEST( testContainsKey );
        CPPUNIT_TEST( testClear );
        CPPUNIT_TEST( testCopy );
        CPPUNIT_TEST( testSize );
        CPPUNIT_TEST( testGet );
        CPPUNIT_TEST( testPut );
        CPPUNIT_TEST( testPutAll );
        CPPUNIT_TEST( testRemove );
        CPPUNIT_TEST( testContiansValue );
        CPPUNIT_TEST( testIsEmpty );
        CPPUNIT_TEST( testEntrySet );
        CPPUNIT_TEST( testKeySet );
        CPPUNIT_TEST( testValues );
        CPPUNIT_TEST( testEntrySetIterator );
        CPPUNIT_TEST( testKeySetIterator );
        CPPUNIT_TEST( testValuesIterator );
        CPPUNIT_TEST_SUITE_END();

    public:

        StlMapTest() {}
        virtual ~StlMapTest() {}

        void testConstructor();
        void testContainsKey();
        void testClear();
        void testCopy();
        void testSize();
        void testGet();
        void testPut();
        void testPutAll();
        void testRemove();
        void testContiansValue();
        void testIsEmpty();
        void testEntrySet();
        void testKeySet();
        void testValues();
        void testEntrySetIterator();
        void testKeySetIterator();
        void testValuesIterator();

    };

}}

#endif /* _DECAF_UTIL_STLMAPTEST_H_ */
