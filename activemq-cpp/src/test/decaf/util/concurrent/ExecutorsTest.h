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

#ifndef _DECAF_UTIL_CONCURRENT_EXECUTORSTEST_H_
#define _DECAF_UTIL_CONCURRENT_EXECUTORSTEST_H_

#include <decaf/util/concurrent/ExecutorsTestSupport.h>

namespace decaf {
namespace util {
namespace concurrent {

    class ExecutorsTest : public ExecutorsTestSupport {

        CPPUNIT_TEST_SUITE( ExecutorsTest );
        CPPUNIT_TEST( testDefaultThreadFactory );
        CPPUNIT_TEST( testNewFixedThreadPool1 );
        CPPUNIT_TEST( testNewFixedThreadPool2 );
        CPPUNIT_TEST( testNewFixedThreadPool3 );
        CPPUNIT_TEST( testNewFixedThreadPool4 );
        CPPUNIT_TEST( testNewSingleThreadExecutor1 );
        CPPUNIT_TEST( testNewSingleThreadExecutor2 );
        CPPUNIT_TEST( testNewSingleThreadExecutor3 );
        CPPUNIT_TEST( testCastNewSingleThreadExecutor );
        CPPUNIT_TEST( testUnconfigurableExecutorService );
        CPPUNIT_TEST( testUnconfigurableExecutorServiceNPE );
        CPPUNIT_TEST( testCallable1 );
        CPPUNIT_TEST( testCallable2 );
        CPPUNIT_TEST( testCallableNPE1 );
        CPPUNIT_TEST( testCallableNPE2 );
        CPPUNIT_TEST_SUITE_END();

    public:

        ExecutorsTest();
        virtual ~ExecutorsTest();

        void testDefaultThreadFactory();
        void testNewFixedThreadPool1();
        void testNewFixedThreadPool2();
        void testNewFixedThreadPool3();
        void testNewFixedThreadPool4();
        void testNewSingleThreadExecutor1();
        void testNewSingleThreadExecutor2();
        void testNewSingleThreadExecutor3();
        void testCastNewSingleThreadExecutor();
        void testUnconfigurableExecutorService();
        void testUnconfigurableExecutorServiceNPE();
        void testCallable1();
        void testCallable2();
        void testCallableNPE1();
        void testCallableNPE2();

    };

}}}

#endif /* _DECAF_UTIL_CONCURRENT_EXECUTORSTEST_H_ */
