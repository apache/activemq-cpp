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

#include "CmsTemplateTest.h"
#include <activemq/cmsutil/DynamicDestinationResolver.h>
#include <activemq/cmsutil/ResourceLifecycleManager.h>
#include "DummyConnectionFactory.h"

CPPUNIT_TEST_SUITE_REGISTRATION( activemq::cmsutil::CmsTemplateTest );

using namespace activemq;
using namespace activemq::cmsutil;

////////////////////////////////////////////////////////////////////////////////
void CmsTemplateTest::setUp() {
    cf = new DummyConnectionFactory();
    cmsTemplate = new CmsTemplate(cf);
    cmsTemplate->setDefaultDestinationName("test");
    cmsTemplate->init();
}

////////////////////////////////////////////////////////////////////////////////
void CmsTemplateTest::tearDown() {
    cmsTemplate->destroy();
    delete cmsTemplate;
    delete cf;
}

////////////////////////////////////////////////////////////////////////////////
void CmsTemplateTest::testExecuteSession() {

    try {
        cmsTemplate->setSessionAcknowledgeMode(cms::Session::CLIENT_ACKNOWLEDGE);
        
        // Test basics.
        MySessionCallback sessionCallback;    
        cmsTemplate->execute(&sessionCallback);    
        CPPUNIT_ASSERT(sessionCallback.session != NULL);
        CPPUNIT_ASSERT(sessionCallback.ackMode == cms::Session::CLIENT_ACKNOWLEDGE);
        
        // Try again and make sure we get the same session
        MySessionCallback sessionCallback2;
        cmsTemplate->execute(&sessionCallback2);    
        CPPUNIT_ASSERT(sessionCallback2.session == sessionCallback.session);
        CPPUNIT_ASSERT(sessionCallback2.ackMode == cms::Session::CLIENT_ACKNOWLEDGE);
        
        // Now try different ack mode and make sure we have a different session.
        cmsTemplate->setSessionAcknowledgeMode(cms::Session::AUTO_ACKNOWLEDGE);
        MySessionCallback sessionCallback3;
        cmsTemplate->execute(&sessionCallback3);  
        CPPUNIT_ASSERT(sessionCallback3.session != NULL);
        CPPUNIT_ASSERT(sessionCallback3.session != sessionCallback.session);
        CPPUNIT_ASSERT(sessionCallback3.ackMode == cms::Session::AUTO_ACKNOWLEDGE);
        
    } catch( cms::CMSException& e) {
        e.printStackTrace();
    }
}

////////////////////////////////////////////////////////////////////////////////
void CmsTemplateTest::testExecuteProducer() {
    
    try {
       
        // Test basics.
        MyProducerCallback callback;    
        cmsTemplate->execute(&callback);    
        CPPUNIT_ASSERT(callback.session != NULL);
        CPPUNIT_ASSERT(callback.producer != NULL);
    
        // Try again and make sure we have the same producer
        MyProducerCallback callback2;    
        cmsTemplate->execute(&callback2);    
        CPPUNIT_ASSERT(callback2.session == callback.session);
        CPPUNIT_ASSERT(callback2.producer == callback.producer);   
        
    } catch( cms::CMSException& e) {
        e.printStackTrace();
    }
}

