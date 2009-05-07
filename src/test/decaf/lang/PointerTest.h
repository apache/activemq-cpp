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

#ifndef _DECAF_LANG_POINTERTEST_H_
#define _DECAF_LANG_POINTERTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace decaf {
namespace lang {

    class PointerTest : public CppUnit::TestFixture {

        CPPUNIT_TEST_SUITE( PointerTest );
        CPPUNIT_TEST( testBasics );
        CPPUNIT_TEST( testAssignment );
        CPPUNIT_TEST( testComparisons );
        CPPUNIT_TEST( testThreaded1 );
        CPPUNIT_TEST( testThreaded2 );
        CPPUNIT_TEST( testOperators );
        CPPUNIT_TEST( testSTLContainers );
        CPPUNIT_TEST( testReturnByValue );
        CPPUNIT_TEST( testDynamicCast );
        CPPUNIT_TEST( testThreadSafety );
        CPPUNIT_TEST_SUITE_END();

    public:

        PointerTest() {}
        virtual ~PointerTest() {}

        void testBasics();
        void testAssignment();
        void testComparisons();
        void testThreaded1();
        void testThreaded2();
        void testOperators();
        void testSTLContainers();
        void testReturnByValue();
        void testDynamicCast();
        void testThreadSafety();

    };

}}

#endif /*_DECAF_LANG_POINTERTEST_H_*/
