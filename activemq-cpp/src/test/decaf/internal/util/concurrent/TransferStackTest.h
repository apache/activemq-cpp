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

#ifndef _DECAF_INTERNAL_UTIL_CONCURRENT_TRANSFERSTACKTEST_H_
#define _DECAF_INTERNAL_UTIL_CONCURRENT_TRANSFERSTACKTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <decaf/internal/util/ByteArrayAdapter.h>
#include <decaf/util/Config.h>

namespace decaf {
namespace internal {
namespace util {
namespace concurrent {

    class TransferStackTest : public CppUnit::TestFixture {

        CPPUNIT_TEST_SUITE( TransferStackTest );
        CPPUNIT_TEST( test );
        CPPUNIT_TEST_SUITE_END();

    public:

        TransferStackTest();
        virtual ~TransferStackTest();

        void test();

    };

}}}}

#endif /* _DECAF_INTERNAL_UTIL_CONCURRENT_TRANSFERSTACKTEST_H_ */
