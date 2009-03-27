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

#ifndef _DECAF_INTERNAL_NET_URIENCODERDECODERTEST_H_
#define _DECAF_INTERNAL_NET_URIENCODERDECODERTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <decaf/internal/net/URIEncoderDecoder.h>

namespace decaf {
namespace internal {
namespace net {

    class URIEncoderDecoderTest : public CppUnit::TestFixture {

        CPPUNIT_TEST_SUITE( URIEncoderDecoderTest );
        CPPUNIT_TEST( testValidate );
        CPPUNIT_TEST( testValidateSimple );
        CPPUNIT_TEST( testQuoteIllegal );
        CPPUNIT_TEST( testEncodeOthers );
        CPPUNIT_TEST( testDecode );
        CPPUNIT_TEST_SUITE_END();

    public:

        URIEncoderDecoderTest();
        virtual ~URIEncoderDecoderTest() {}

        void testValidate();
        void testValidateSimple();
        void testQuoteIllegal();
        void testEncodeOthers();
        void testDecode();

    };

}}}

#endif /* _DECAF_INTERNAL_NET_URIENCODERDECODERTEST_H_ */
