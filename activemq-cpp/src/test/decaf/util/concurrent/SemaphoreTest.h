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

#ifndef _DECAF_UTIL_CONCURRENT_SEMAPHORETEST_H_
#define _DECAF_UTIL_CONCURRENT_SEMAPHORETEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <decaf/util/concurrent/ExecutorsTestSupport.h>

namespace decaf {
namespace util {
namespace concurrent {

    class SemaphoreTest : public ExecutorsTestSupport {

        CPPUNIT_TEST_SUITE( SemaphoreTest );
        CPPUNIT_TEST( testConstructor );
        CPPUNIT_TEST( testConstructor2 );
        CPPUNIT_TEST( testTryAcquireInSameThread );
        CPPUNIT_TEST( testAcquireReleaseInSameThread );
        CPPUNIT_TEST( testAcquireUninterruptiblyReleaseInSameThread );
        CPPUNIT_TEST( testTimedAcquireReleaseInSameThread );
        CPPUNIT_TEST( testAcquireReleaseInDifferentThreads );
        CPPUNIT_TEST( testUninterruptibleAcquireReleaseInDifferentThreads );
        CPPUNIT_TEST( testTimedAcquireReleaseInDifferentThreads );
        CPPUNIT_TEST( testAcquireInterruptedException );
        CPPUNIT_TEST( testTryAcquireInterruptedException);
        CPPUNIT_TEST( testHasQueuedThreads );
        CPPUNIT_TEST( testGetQueueLength );
        CPPUNIT_TEST( testGetQueuedThreads );
        CPPUNIT_TEST( testDrainPermits );
        CPPUNIT_TEST( testReducePermits );
        CPPUNIT_TEST( testConstructorFair );
        CPPUNIT_TEST( testTryAcquireInSameThreadFair );
        CPPUNIT_TEST( testTryAcquireNInSameThreadFair );
        CPPUNIT_TEST( testAcquireReleaseInSameThreadFair );
        CPPUNIT_TEST( testAcquireReleaseNInSameThreadFair );
        CPPUNIT_TEST( testAcquireUninterruptiblyReleaseNInSameThreadFair );
        CPPUNIT_TEST( testTimedAcquireReleaseNInSameThreadFair );
        CPPUNIT_TEST( testTimedAcquireReleaseInSameThreadFair );
        CPPUNIT_TEST( testAcquireReleaseInDifferentThreadsFair );
        CPPUNIT_TEST( testAcquireReleaseNInDifferentThreadsFair );
        CPPUNIT_TEST( testAcquireReleaseNInDifferentThreadsFair2 );
        CPPUNIT_TEST( testTimedAcquireReleaseInDifferentThreadsFair );
        CPPUNIT_TEST( testTimedAcquireReleaseNInDifferentThreadsFair );
        CPPUNIT_TEST( testAcquireInterruptedExceptionFair );
        CPPUNIT_TEST( testAcquireNInterruptedExceptionFair );
        CPPUNIT_TEST( testTryAcquireInterruptedExceptionFair );
        CPPUNIT_TEST( testTryAcquireNInterruptedExceptionFair );
        CPPUNIT_TEST( testGetQueueLengthFair );
        CPPUNIT_TEST( testToString );
        CPPUNIT_TEST_SUITE_END();

    public:

        SemaphoreTest();
        virtual ~SemaphoreTest();

        void testConstructor();
        void testConstructor2();
        void testTryAcquireInSameThread();
        void testAcquireReleaseInSameThread();

        void testAcquireUninterruptiblyReleaseInSameThread();
        void testTimedAcquireReleaseInSameThread();
        void testAcquireReleaseInDifferentThreads();
        void testUninterruptibleAcquireReleaseInDifferentThreads();
        void testTimedAcquireReleaseInDifferentThreads();
        void testAcquireInterruptedException();
        void testTryAcquireInterruptedException();
        void testHasQueuedThreads();
        void testGetQueueLength();
        void testGetQueuedThreads();
        void testDrainPermits();
        void testReducePermits();
        void testConstructorFair();
        void testTryAcquireInSameThreadFair();
        void testTryAcquireNInSameThreadFair();
        void testAcquireReleaseInSameThreadFair();
        void testAcquireReleaseNInSameThreadFair();
        void testAcquireUninterruptiblyReleaseNInSameThreadFair();
        void testTimedAcquireReleaseNInSameThreadFair();
        void testTimedAcquireReleaseInSameThreadFair();
        void testAcquireReleaseInDifferentThreadsFair();
        void testAcquireReleaseNInDifferentThreadsFair();
        void testAcquireReleaseNInDifferentThreadsFair2();
        void testTimedAcquireReleaseInDifferentThreadsFair();
        void testTimedAcquireReleaseNInDifferentThreadsFair();
        void testAcquireInterruptedExceptionFair();
        void testAcquireNInterruptedExceptionFair();
        void testTryAcquireInterruptedExceptionFair();
        void testTryAcquireNInterruptedExceptionFair();
        void testGetQueueLengthFair();
        void testToString();

    };

}}}

#endif /* _DECAF_UTIL_CONCURRENT_SEMAPHORETEST_H_ */
