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

#ifndef _DECAF_UTIL_CONCURRENT_FUTURETASKTEST_H_
#define _DECAF_UTIL_CONCURRENT_FUTURETASKTEST_H_

#include <decaf/util/concurrent/ExecutorsTestSupport.h>

namespace decaf {
namespace util {
namespace concurrent {

    class FutureTaskTest : public ExecutorsTestSupport {

        CPPUNIT_TEST_SUITE( FutureTaskTest );
        CPPUNIT_TEST( testConstructor1 );
        CPPUNIT_TEST( testConstructor2 );
        CPPUNIT_TEST( testConstructor3 );
        CPPUNIT_TEST( testConstructor4 );
        CPPUNIT_TEST( testIsDone );
        CPPUNIT_TEST( testRunAndReset );
        CPPUNIT_TEST( testResetAfterCancel );
        CPPUNIT_TEST( testSet );
        CPPUNIT_TEST( testSetException );
        CPPUNIT_TEST( testCancelBeforeRun );
        CPPUNIT_TEST( testCancelBeforeRun2 );
        CPPUNIT_TEST( testCancelAfterRun );
        CPPUNIT_TEST( testCancelInterrupt );
        CPPUNIT_TEST( testCancelNoInterrupt );
        CPPUNIT_TEST( testGet1 );
        CPPUNIT_TEST( testTimedGet1 );
        CPPUNIT_TEST( testTimedGetCancellation );
        CPPUNIT_TEST( testGetCancellation );
        CPPUNIT_TEST( testGetExecutionException );
        CPPUNIT_TEST( testTimedGetExecutionException2 );
        CPPUNIT_TEST( testGetInterruptedException );
        CPPUNIT_TEST( testTimedGetInterruptedException2 );
        CPPUNIT_TEST( testGetTimeoutException );
        CPPUNIT_TEST_SUITE_END();

    public:

        FutureTaskTest();
        virtual ~FutureTaskTest();

        void testConstructor1();
        void testConstructor2();
        void testConstructor3();
        void testConstructor4();
        void testIsDone();
        void testRunAndReset();
        void testResetAfterCancel();
        void testSet();
        void testSetException();
        void testCancelBeforeRun();
        void testCancelBeforeRun2();
        void testCancelAfterRun();
        void testCancelInterrupt();
        void testCancelNoInterrupt();
        void testGet1();
        void testTimedGet1();
        void testTimedGetCancellation();
        void testGetCancellation();
        void testGetExecutionException();
        void testTimedGetExecutionException2();
        void testGetInterruptedException();
        void testTimedGetInterruptedException2();
        void testGetTimeoutException();

    };

}}}

#endif /* _DECAF_UTIL_CONCURRENT_FUTURETASKTEST_H_ */
