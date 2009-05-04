/**
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _ACTIVEMQ_UTIL_PRIMITIVEVALUECONVERTERTEST_H_
#define _ACTIVEMQ_UTIL_PRIMITIVEVALUECONVERTERTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace activemq {
namespace util {

    class PrimitiveValueConverterTest  : public CppUnit::TestFixture {

        CPPUNIT_TEST_SUITE( PrimitiveValueConverterTest );
        CPPUNIT_TEST( testConvertToBoolean );
        CPPUNIT_TEST( testConvertToChar );
        CPPUNIT_TEST( testConvertToByte );
        CPPUNIT_TEST( testConvertToShort );
        CPPUNIT_TEST( testConvertToInt );
        CPPUNIT_TEST( testConvertToLong );
        CPPUNIT_TEST( testConvertToFloat );
        CPPUNIT_TEST( testConvertToDouble );
        CPPUNIT_TEST( testConvertToString );
        CPPUNIT_TEST_SUITE_END();

    public:

        PrimitiveValueConverterTest() {}
        virtual ~PrimitiveValueConverterTest() {}

        void testConvertToBoolean();
        void testConvertToChar();
        void testConvertToByte();
        void testConvertToShort();
        void testConvertToInt();
        void testConvertToLong();
        void testConvertToFloat();
        void testConvertToDouble();
        void testConvertToString();

    };

}}

#endif /* _ACTIVEMQ_UTIL_PRIMITIVEVALUECONVERTERTEST_H_ */
