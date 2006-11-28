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

#ifndef ACTIVEMQ_UTIL_MAPTEST_H_
#define ACTIVEMQ_UTIL_MAPTEST_H_

#include <activemq/util/Map.h>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace activemq{
namespace util{
    
    class MapTest : public CppUnit::TestFixture
    {
        CPPUNIT_TEST_SUITE( MapTest );
        CPPUNIT_TEST( testContains );
        CPPUNIT_TEST( testClear );
        CPPUNIT_TEST( testCount );
        CPPUNIT_TEST( testValue );
        CPPUNIT_TEST( testRemove );
        CPPUNIT_TEST_SUITE_END();
        
    public:
    	MapTest();
    	virtual ~MapTest();
        
        void testContains();
        void testClear();
        void testCount();
        void testValue();
        void testRemove();
    };

}}

#endif /*ACTIVEMQ_UTIL_MAPTEST_H_*/
