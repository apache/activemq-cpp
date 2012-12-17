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

#ifndef _DECAF_UTIL_ZIP_CHECKEDINPUTSTREAMTEST_H_
#define _DECAF_UTIL_ZIP_CHECKEDINPUTSTREAMTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace decaf {
namespace util {
namespace zip {

    class CheckedInputStreamTest : public CppUnit::TestFixture {

        CPPUNIT_TEST_SUITE( CheckedInputStreamTest );
        CPPUNIT_TEST( testConstructor );
        CPPUNIT_TEST( testGetChecksum );
        CPPUNIT_TEST( testSkip );
        CPPUNIT_TEST( testRead );
        CPPUNIT_TEST( testReadBIII );
        CPPUNIT_TEST_SUITE_END();

    public:

        CheckedInputStreamTest();
        virtual ~CheckedInputStreamTest();

        void testConstructor();
        void testGetChecksum();
        void testSkip();
        void testRead();
        void testReadBIII();

    };

}}}

#endif /* _DECAF_UTIL_ZIP_CHECKEDINPUTSTREAMTEST_H_ */
