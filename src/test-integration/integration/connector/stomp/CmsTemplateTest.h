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

#ifndef _INTEGRATION_CONNECTOR_STOMP_CMSTEMPLATETEST_H_
#define _INTEGRATION_CONNECTOR_STOMP_CMSTEMPLATETEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <integration/TestSupport.h>
#include <activemq/core/ActiveMQConnectionFactory.h>
#include <activemq/cmsutil/CmsTemplate.h>
#include <activemq/cmsutil/MessageCreator.h>

namespace integration{
namespace connector{
namespace stomp{

    class CmsTemplateTest : public CppUnit::TestFixture
    {
        CPPUNIT_TEST_SUITE( CmsTemplateTest );
        CPPUNIT_TEST( testBasics );
        CPPUNIT_TEST( testReceiveException );
        CPPUNIT_TEST( testSendException );
        CPPUNIT_TEST_SUITE_END();
        
        class TextMessageCreator : public activemq::cmsutil::MessageCreator {
        private:
            std::string text;
        public:
            TextMessageCreator( const std::string& text) {
                this->text = text;
            }
            virtual ~TextMessageCreator() {}
            
            std::string getText() const {
                return text;
            }
            
            virtual cms::Message* createMessage(cms::Session* session ) 
                        throw (cms::CMSException) {
                return session->createTextMessage(text);
            } 
        };
        
        class Sender : public decaf::lang::Runnable {
        private:
                    
            activemq::core::ActiveMQConnectionFactory cf;
            activemq::cmsutil::CmsTemplate cmsTemplate;
            int count;
            
        public:
            
            Sender( const std::string& url, bool pubSub, const std::string& destName, int count ) {
                cf.setBrokerURL(url);
                cmsTemplate.setConnectionFactory(&cf);
                cmsTemplate.setPubSubDomain(pubSub);
                cmsTemplate.setDefaultDestinationName(destName);
                cmsTemplate.init();
                this->count = count;
            }
            virtual ~Sender(){
                cmsTemplate.destroy();
            }
            virtual void run() {                
                try {
                    // Send a batch of messages.
                    TextMessageCreator tmc("hello world");
                    for( int ix=0; ix<count; ++ix ) {
                        cmsTemplate.send(&tmc);
                    }
                    
                    decaf::lang::Thread::sleep(100);
                } catch( cms::CMSException& ex) {
                    ex.printStackTrace();
                }
            }
        };
        
        class Receiver : public decaf::lang::Runnable {
        private:
            
            activemq::core::ActiveMQConnectionFactory cf;
            activemq::cmsutil::CmsTemplate cmsTemplate;
            int count;
            int numReceived;
            
        public:
            
            Receiver( const std::string& url, bool pubSub, const std::string& destName, int count ) {
                cf.setBrokerURL(url);
                cmsTemplate.setConnectionFactory(&cf);
                cmsTemplate.setPubSubDomain(pubSub);
                cmsTemplate.setDefaultDestinationName(destName);
                cmsTemplate.init();
                this->count = count;
            }
            virtual ~Receiver(){
                cmsTemplate.destroy();
            }
            int getNumReceived() const {
                return numReceived;
            }
            virtual void run() {
                
                try {
                    numReceived = 0;
                    
                    // Receive a batch of messages.
                    for( int ix=0; ix<count; ++ix ) {
                        cms::Message* message = cmsTemplate.receive();
                        numReceived++;
                        delete message;                    
                    }
                } catch( cms::CMSException& ex) {
                    ex.printStackTrace();
                }
            }
        };
        
    public:

        CmsTemplateTest(){}
        virtual ~CmsTemplateTest(){}

        virtual void setUp();
        virtual void tearDown();
                
        virtual void testBasics();
        virtual void testReceiveException();
        virtual void testSendException();

    };

}}}

#endif /*_INTEGRATION_CONNECTOR_STOMP_CMSTEMPLATETEST_H_*/
