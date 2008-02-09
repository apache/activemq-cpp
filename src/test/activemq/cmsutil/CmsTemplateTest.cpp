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
#include "DummyMessageCreator.h"

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
        
        cmsTemplate->setPubSubDomain(false);
       
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
        
        // Change to topics and make sure it's a different producer.
        cmsTemplate->setPubSubDomain(true);
        MyProducerCallback callback3;    
        cmsTemplate->execute(&callback3);    
        CPPUNIT_ASSERT(callback3.session == callback.session);
        CPPUNIT_ASSERT(callback3.producer != callback.producer);
        
        // Now change destination name and make sure it's different yet again.
        cmsTemplate->setDefaultDestinationName("fred");
        MyProducerCallback callback4;    
        cmsTemplate->execute(&callback4);    
        CPPUNIT_ASSERT(callback4.session == callback.session);
        CPPUNIT_ASSERT(callback4.producer != callback3.producer);
                
    } catch( cms::CMSException& e) {
        e.printStackTrace();
    }
}

////////////////////////////////////////////////////////////////////////////////
void CmsTemplateTest::testSend() {
    
    try {
        
        MessageContext* messageContext = cf->getMessageContext();
        
        MySendListener listener;
        messageContext->setSendListener(&listener);
        
        DummyMessageCreator msgCreator;
        
        // First, test basics.
        cmsTemplate->send(&msgCreator);        
        const cms::Queue* q = dynamic_cast<const cms::Queue*>(listener.dest);
        CPPUNIT_ASSERT(q != NULL);
        CPPUNIT_ASSERT_EQUAL((std::string)"test", q->getQueueName());
        CPPUNIT_ASSERT(listener.message != NULL);
        CPPUNIT_ASSERT_EQUAL(4, listener.priority);
        CPPUNIT_ASSERT_EQUAL(0LL, listener.ttl);
        CPPUNIT_ASSERT_EQUAL(1, listener.deliveryMode);
        
        // Now change the defaults and verify that they did not change
        // the values (explicit qos not enabled). Also, change to using topics.
        cmsTemplate->setPubSubDomain(true);
        cmsTemplate->setPriority(5);
        cmsTemplate->setDeliveryMode(cms::DeliveryMode::NON_PERSISTENT);
        cmsTemplate->setTimeToLive(10LL);
        cmsTemplate->setDefaultDestinationName("bob");
        cmsTemplate->setDeliveryMode(0);
        cmsTemplate->send(&msgCreator);  
        const cms::Topic* t = dynamic_cast<const cms::Topic*>(listener.dest);
        CPPUNIT_ASSERT(t != NULL);
        CPPUNIT_ASSERT_EQUAL((std::string)"bob", t->getTopicName());
        CPPUNIT_ASSERT(listener.message != NULL);
        CPPUNIT_ASSERT_EQUAL(4, listener.priority);
        CPPUNIT_ASSERT_EQUAL(0LL, listener.ttl);
        CPPUNIT_ASSERT_EQUAL(1, listener.deliveryMode);
        
        // Now enable explicit quality of service and verify that the new default
        // values are used.
        cmsTemplate->setExplicitQosEnabled(true);
        cmsTemplate->send(&msgCreator);  
        t = dynamic_cast<const cms::Topic*>(listener.dest);
        CPPUNIT_ASSERT(t != NULL);
        CPPUNIT_ASSERT_EQUAL((std::string)"bob", t->getTopicName());
        CPPUNIT_ASSERT(listener.message != NULL);
        CPPUNIT_ASSERT_EQUAL(5, listener.priority);
        CPPUNIT_ASSERT_EQUAL(10LL, listener.ttl);
        CPPUNIT_ASSERT_EQUAL(0, listener.deliveryMode);
        
        // Now try the version of send with an explicit destination.
        activemq::connector::stomp::StompTopic myTopic("anothertopic");
        cmsTemplate->send(&myTopic, &msgCreator);  
        t = dynamic_cast<const cms::Topic*>(listener.dest);
        CPPUNIT_ASSERT(t == &myTopic);
        
        // Now try the version of send with just a named destination.
        cmsTemplate->send("yetanothertopic", &msgCreator);  
        t = dynamic_cast<const cms::Topic*>(listener.dest);
        CPPUNIT_ASSERT(t != NULL);
        CPPUNIT_ASSERT_EQUAL((std::string)"yetanothertopic", t->getTopicName());
                
    } catch( cms::CMSException& e) {
        e.printStackTrace();
    }
}
