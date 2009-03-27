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

#ifndef _ACTIVEMQ_CMSUTIL_CMSACCESSORTEST_H_
#define _ACTIVEMQ_CMSUTIL_CMSACCESSORTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <activemq/cmsutil/CmsAccessor.h>

namespace activemq{
namespace cmsutil{

    class DummyConnectionFactory;
    
    class CmsAccessorTest : public CppUnit::TestFixture
    {
        CPPUNIT_TEST_SUITE( CmsAccessorTest );
        CPPUNIT_TEST( testConnectionFactory );
        CPPUNIT_TEST( testAckMode );
        CPPUNIT_TEST( testCreateResources );
        CPPUNIT_TEST_SUITE_END();               

        class MyAccessor : public CmsAccessor {
        
        public:
            
            virtual ~MyAccessor(){}
            
            virtual cms::Connection* createConnection() throw (cms::CMSException) {
                return CmsAccessor::createConnection();
            }
                
            virtual cms::Session* createSession(cms::Connection* con) 
                throw (cms::CMSException) {
                return CmsAccessor::createSession(con);
            }
        };
        
        MyAccessor* accessor;
        DummyConnectionFactory* cf;
        
    public:

        CmsAccessorTest() {}
        virtual ~CmsAccessorTest() {}

        virtual void setUp();
        virtual void tearDown();
        
        void testConnectionFactory();
        void testAckMode();
        void testCreateResources();
    };

}}

#endif /*_ACTIVEMQ_CMSUTIL_CMSACCESSORTEST_H_*/
