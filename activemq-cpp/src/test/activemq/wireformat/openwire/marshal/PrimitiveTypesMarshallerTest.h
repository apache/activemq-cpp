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

#ifndef _ACTIVEMQ_WIREFORMAT_OPENWIRE_MARSHAL_PRIMITIVETYPESMARSHALLERTEST_H_
#define _ACTIVEMQ_WIREFORMAT_OPENWIRE_MARSHAL_PRIMITIVETYPESMARSHALLERTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <activemq/util/PrimitiveMap.h>

namespace activemq{
namespace wireformat{
namespace openwire{
namespace marshal{

    class PrimitiveTypesMarshallerTest : public CppUnit::TestFixture {

        CPPUNIT_TEST_SUITE( PrimitiveTypesMarshallerTest );
        CPPUNIT_TEST( test );
        CPPUNIT_TEST( testLists );
        CPPUNIT_TEST( testMaps );
        CPPUNIT_TEST_SUITE_END();

    private:

        activemq::util::PrimitiveMap* unmarshaledMap;

    private:

        PrimitiveTypesMarshallerTest(const PrimitiveTypesMarshallerTest&);
        PrimitiveTypesMarshallerTest& operator= (const PrimitiveTypesMarshallerTest&);

    public:

        PrimitiveTypesMarshallerTest() : unmarshaledMap() {}
        virtual ~PrimitiveTypesMarshallerTest() {}

        virtual void setUp() {
            this->unmarshaledMap = NULL;
        }

        virtual void tearDown() {
            delete this->unmarshaledMap;
            this->unmarshaledMap = NULL;
        }

        void test();
        void testLists();
        void testMaps();

    };

}}}}

#endif /*_ACTIVEMQ_WIREFORMAT_OPENWIRE_MARSHAL_PRIMITIVETYPESMARSHALLERTEST_H_*/
