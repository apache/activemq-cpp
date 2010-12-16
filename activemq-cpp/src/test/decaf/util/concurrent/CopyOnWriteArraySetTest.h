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

#ifndef _DECAF_UTIL_CONCURRENT_COPYONWRITEARRAYSETTEST_H_
#define _DECAF_UTIL_CONCURRENT_COPYONWRITEARRAYSETTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace decaf {
namespace util {
namespace concurrent {

    class CopyOnWriteArraySetTest : public CppUnit::TestFixture {

        CPPUNIT_TEST_SUITE( CopyOnWriteArraySetTest );
        CPPUNIT_TEST( testConstructor1 );
        CPPUNIT_TEST( testConstructor2 );
        CPPUNIT_TEST( testConstructor3 );
        CPPUNIT_TEST( testAddAll1 );
        CPPUNIT_TEST( testAddAll2 );
        CPPUNIT_TEST( testAdd1 );
        CPPUNIT_TEST( testAdd2 );
        CPPUNIT_TEST( testClear );
        CPPUNIT_TEST( testContains );
        CPPUNIT_TEST( testContainsAll );
        CPPUNIT_TEST( testEquals );
        CPPUNIT_TEST( testIsEmpty );
        CPPUNIT_TEST( testIterator );
        CPPUNIT_TEST( testIteratorRemove );
        CPPUNIT_TEST( testRemoveAll );
        CPPUNIT_TEST( testRemove );
        CPPUNIT_TEST( testSize );
        CPPUNIT_TEST( testToArray );
        CPPUNIT_TEST_SUITE_END();

    private:

        static const int SIZE;

    public:

        CopyOnWriteArraySetTest();
        virtual ~CopyOnWriteArraySetTest();

        void testConstructor1();
        void testConstructor2();
        void testConstructor3();
        void testAddAll1();
        void testAddAll2();
        void testAdd1();
        void testAdd2();
        void testClear();
        void testContains();
        void testContainsAll();
        void testEquals();
        void testIsEmpty();
        void testIterator();
        void testIteratorRemove();
        void testRemoveAll();
        void testRemove();
        void testSize();
        void testToArray();

    };

}}}

#endif /* _DECAF_UTIL_CONCURRENT_COPYONWRITEARRAYSETTEST_H_ */
