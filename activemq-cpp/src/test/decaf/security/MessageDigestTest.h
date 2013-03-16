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

#ifndef _DECAF_SECURITY_MESSAGEDIGESTTEST_H_
#define _DECAF_SECURITY_MESSAGEDIGESTTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace decaf {
namespace security {

    class MessageDigestTest : public CppUnit::TestFixture {

        CPPUNIT_TEST_SUITE( MessageDigestTest );
        CPPUNIT_TEST( testGetInstance1 );
        CPPUNIT_TEST( testGetInstance2 );
        CPPUNIT_TEST( testGetInstance3 );
        CPPUNIT_TEST( testGetInstance4 );
        CPPUNIT_TEST( testResults1 );
        CPPUNIT_TEST( testResults2 );
        CPPUNIT_TEST( testResults3 );
        CPPUNIT_TEST_SUITE_END();

    public:

        MessageDigestTest();
        virtual ~MessageDigestTest();

        void testGetInstance1();
        void testGetInstance2();
        void testGetInstance3();
        void testGetInstance4();

        void testResults1();
        void testResults2();
        void testResults3();

    };

}}

#endif /* _DECAF_SECURITY_MESSAGEDIGESTTEST_H_ */
