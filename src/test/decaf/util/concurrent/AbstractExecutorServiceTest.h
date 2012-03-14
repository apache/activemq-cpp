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

#ifndef _DECAF_UTIL_CONCURRENT_ABSTRACTEXECUTORSERVICETEST_H_
#define _DECAF_UTIL_CONCURRENT_ABSTRACTEXECUTORSERVICETEST_H_

#include <decaf/util/concurrent/ExecutorsTestSupport.h>

namespace decaf {
namespace util {
namespace concurrent {

    class AbstractExecutorServiceTest : public ExecutorsTestSupport
    {
        CPPUNIT_TEST_SUITE( AbstractExecutorServiceTest );
        CPPUNIT_TEST( testExecuteRunnable );
        CPPUNIT_TEST( testSubmitCallable );
        CPPUNIT_TEST( testSubmitRunnable );
        CPPUNIT_TEST( testSubmitRunnable2 );
        CPPUNIT_TEST( testExecuteNullRunnable );
        CPPUNIT_TEST( testSubmitNullCallable );
        CPPUNIT_TEST( testExecute1 );
        CPPUNIT_TEST( testExecute2 );
        CPPUNIT_TEST( testInterruptedSubmit );
        CPPUNIT_TEST( testSubmitIE );
        CPPUNIT_TEST( testSubmitEE );
        CPPUNIT_TEST_SUITE_END();

    public:

        AbstractExecutorServiceTest();
        virtual ~AbstractExecutorServiceTest();

        void testExecuteRunnable();
        void testSubmitCallable();
        void testSubmitRunnable();
        void testSubmitRunnable2();
        void testExecuteNullRunnable();
        void testSubmitNullCallable();
        void testExecute1();
        void testExecute2();
        void testInterruptedSubmit();
        void testSubmitIE();
        void testSubmitEE();

    };

}}}

#endif /* _DECAF_UTIL_CONCURRENT_ABSTRACTEXECUTORSERVICETEST_H_ */
