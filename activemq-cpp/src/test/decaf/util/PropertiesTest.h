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

#ifndef _DECAF_UTIL_PROPERTIESTEST_H_
#define _DECAF_UTIL_PROPERTIESTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <decaf/util/Properties.h>

namespace decaf {
namespace util {

    class PropertiesTest : public CppUnit::TestFixture {

        CPPUNIT_TEST_SUITE( PropertiesTest );
        CPPUNIT_TEST( testPutAndGet );
        CPPUNIT_TEST( testAssign );
        CPPUNIT_TEST( testCopy );
        CPPUNIT_TEST( testClone );
        CPPUNIT_TEST( testRemove );
        CPPUNIT_TEST( testClear );
        CPPUNIT_TEST( testEquals );
        CPPUNIT_TEST( testLoadNPE );
        CPPUNIT_TEST( testLoadInputStream );
        CPPUNIT_TEST( testPropertyNames );
        CPPUNIT_TEST( testPropertyNamesOverride );
        CPPUNIT_TEST( testPropertyNamesScenario1 );
        CPPUNIT_TEST( testStoreOutputStream );
        CPPUNIT_TEST_SUITE_END();

    private:

        Properties testProperties;

    public:

        PropertiesTest() : testProperties() {}

        virtual ~PropertiesTest() {}

        virtual void setUp();
        virtual void tearDown();

        void testPutAndGet();
        void testAssign();
        void testCopy();
        void testClone();
        void testRemove();
        void testClear();
        void testEquals();
        void testLoadNPE();
        void testLoadInputStream();
        void testPropertyNames();
        void testPropertyNamesOverride();
        void testPropertyNamesScenario1();
        void testStoreOutputStream();

    };

}}

#endif /* _DECAF_UTIL_PROPERTIESTEST_H_ */
