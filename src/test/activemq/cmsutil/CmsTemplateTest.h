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

#ifndef _ACTIVEMQ_CMSUTIL_CMSTEMPLATETEST_H_
#define _ACTIVEMQ_CMSUTIL_CMSTEMPLATETEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <activemq/cmsutil/CmsTemplate.h>
#include <activemq/cmsutil/SessionCallback.h>
#include <activemq/cmsutil/ProducerCallback.h>

namespace activemq{
namespace cmsutil{

    class DummyConnectionFactory;
    
    class CmsTemplateTest : public CppUnit::TestFixture
    {
        CPPUNIT_TEST_SUITE( CmsTemplateTest );
        CPPUNIT_TEST( testExecuteSession );
        CPPUNIT_TEST( testExecuteProducer );
        CPPUNIT_TEST_SUITE_END();               
             

        CmsTemplate* cmsTemplate;
        DummyConnectionFactory* cf;
        
        class MySessionCallback : public SessionCallback {
        public:
            
            cms::Session* session;
            cms::Session::AcknowledgeMode ackMode;
            
            MySessionCallback() {
                session = NULL;
            }
            virtual ~MySessionCallback(){}
            
            virtual void doInCms(cms::Session* session) throw (cms::CMSException) {            
                this->session = session;
                this->ackMode = session->getAcknowledgeMode();
            }
        };
        
        class MyProducerCallback : public ProducerCallback {
        public:
            
            cms::Session* session;
            cms::MessageProducer* producer;
            
            MyProducerCallback() {
                session = NULL;
                producer = NULL;
            }
            virtual ~MyProducerCallback(){}
            
            virtual void doInCms(cms::Session* session, 
                    cms::MessageProducer* producer) throw (cms::CMSException) {
                this->session = session;
                this->producer = producer;
            }
        };
        
    public:

        CmsTemplateTest() {}
        virtual ~CmsTemplateTest() {}

        virtual void setUp();
        virtual void tearDown();
        
        void testExecuteSession();
        void testExecuteProducer();
    };

}}

#endif /*_ACTIVEMQ_CMSUTIL_CMSTEMPLATETEST_H_*/
