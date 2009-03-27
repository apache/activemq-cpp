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
#include <activemq/cmsutil/DummyMessageCreator.h>
#include <activemq/cmsutil/MessageContext.h>
#include <activemq/exceptions/ActiveMQException.h>

namespace activemq{
namespace cmsutil{

    class DummyConnectionFactory;
    
    class CmsTemplateTest : public CppUnit::TestFixture
    {
        CPPUNIT_TEST_SUITE( CmsTemplateTest );
        CPPUNIT_TEST( testExecuteSession );
        CPPUNIT_TEST( testExecuteProducer );
        CPPUNIT_TEST( testSend );
        CPPUNIT_TEST( testReceive );
        CPPUNIT_TEST( testReceive_Destination );
        CPPUNIT_TEST( testReceive_DestinationName );
        CPPUNIT_TEST( testReceiveSelected );
        CPPUNIT_TEST( testReceiveSelected_Destination );
        CPPUNIT_TEST( testReceiveSelected_DestinationName );        
        CPPUNIT_TEST_SUITE_END();               
             

        CmsTemplate* cmsTemplate;
        DummyConnectionFactory* cf;
        
        class MySendListener : public MessageContext::SendListener {
        public:
            
            const cms::Destination* dest;
            cms::Message* message;
            int deliveryMode;
            int priority;
            long long ttl;
            std::string selector;
            bool noLocal;
            long long timeout;
                    
            MySendListener() {
                dest = NULL;
                message = NULL;
                deliveryMode = 0;
                priority = 0;
                ttl = 0LL;
            }
            virtual ~MySendListener(){}
            
            virtual void onSend(const cms::Destination* destination,
                cms::Message* message, int deliveryMode, int priority, 
                long long timeToLive) throw (cms::CMSException){
                this->dest = destination;
                this->message = message;
                this->deliveryMode = deliveryMode;
                this->priority = priority;
                this->ttl = timeToLive;
            }
            
            virtual cms::Message* doReceive(const cms::Destination* dest, 
                    const std::string& selector, 
                    bool noLocal, 
                    long long timeout) throw (cms::CMSException){
                this->dest = dest;
                this->selector = selector;
                this->noLocal = noLocal;
                this->timeout = timeout;
                return new DummyMessage();
            }
        };
        
        class FailSendListener : public MessageContext::SendListener {
        public:
                    
            FailSendListener() {
            }
            virtual ~FailSendListener(){}
            
            virtual void onSend(const cms::Destination* destination,
                    cms::Message* message, int deliveryMode, int priority, 
                    long long timeToLive) throw (cms::CMSException){
                throw exceptions::ActiveMQException(__FILE__, __LINE__, "");
            }
            
            virtual cms::Message* doReceive(const cms::Destination* dest, 
                    const std::string& selector, 
                    bool noLocal, 
                    long long timeout) throw (cms::CMSException){
                throw exceptions::ActiveMQException(__FILE__, __LINE__, "");
            }
        };
        
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
        void testSend();
        void testReceive();
        void testReceive_Destination();
        void testReceive_DestinationName();
        void testReceiveSelected();
        void testReceiveSelected_Destination();
        void testReceiveSelected_DestinationName();
    };

}}

#endif /*_ACTIVEMQ_CMSUTIL_CMSTEMPLATETEST_H_*/
