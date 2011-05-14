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

#ifndef _DECAF_UTIL_CONCURRENT_LOCKS_LOCKSUPPORTTEST_H_
#define _DECAF_UTIL_CONCURRENT_LOCKS_LOCKSUPPORTTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <decaf/util/concurrent/ExecutorsTestSupport.h>

namespace decaf {
namespace util {
namespace concurrent {
namespace locks {

    class LockSupportTest : public ExecutorsTestSupport {

        CPPUNIT_TEST_SUITE( LockSupportTest );
        CPPUNIT_TEST( testPark1 );
        CPPUNIT_TEST( testPark2 );
        CPPUNIT_TEST( testPark3 );
        CPPUNIT_TEST( testPark4 );
        CPPUNIT_TEST( testParkNanos );
        CPPUNIT_TEST( testParkUntil );
        CPPUNIT_TEST_SUITE_END();

    public:

        LockSupportTest();

        virtual ~LockSupportTest();

        void testPark1();
        void testPark2();
        void testPark3();
        void testPark4();
        void testParkNanos();
        void testParkUntil();

    };

}}}}

#endif /* _DECAF_UTIL_CONCURRENT_LOCKS_LOCKSUPPORTTEST_H_ */
