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

#ifndef _DECAF_UTIL_ARRAYSTEST_H_
#define _DECAF_UTIL_ARRAYSTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace decaf {
namespace util {

    class ArraysTest : public CppUnit::TestFixture {
    private:

        CPPUNIT_TEST_SUITE( ArraysTest );
        CPPUNIT_TEST( testFill1 );
        CPPUNIT_TEST( testFill2 );
        CPPUNIT_TEST_SUITE_END();

    private:

        static const int SIZE;

    public:

        ArraysTest();
        virtual ~ArraysTest();

        void testFill1();
        void testFill2();

    };

}}

#endif /* _DECAF_UTIL_ARRAYSTEST_H_ */
