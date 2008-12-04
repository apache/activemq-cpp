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

#ifndef _ACTIVEMQ_TEST_CMSTEMPLATETEST_H_
#define _ACTIVEMQ_TEST_CMSTEMPLATETEST_H_

#include <activemq/test/CMSTestFixture.h>

#include <activemq/cmsutil/CmsTemplate.h>
#include <activemq/cmsutil/MessageCreator.h>
#include <activemq/core/ActiveMQConnectionFactory.h>
#include <decaf/util/concurrent/CountDownLatch.h>

namespace activemq{
namespace test{

    class CmsTemplateTest : public CMSTestFixture {

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
                cmsTemplate.setDeliveryPersistent(false);
                this->count = count;
            }

            virtual ~Sender(){
            }

            virtual void run() {
                try {

                    // Send a batch of messages.
                    TextMessageCreator tmc("hello world");
                    for( int ix=0; ix<count; ++ix ) {
                        cmsTemplate.send(&tmc);
                    }

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
            decaf::util::concurrent::CountDownLatch ready;

        public:

            Receiver( const std::string& url, bool pubSub, const std::string& destName, int count )
                : ready(1) {

                cf.setBrokerURL(url);
                cmsTemplate.setConnectionFactory(&cf);
                cmsTemplate.setPubSubDomain(pubSub);
                cmsTemplate.setDefaultDestinationName(destName);
                cmsTemplate.setDeliveryPersistent(false);
                this->count = count;
            }
            virtual ~Receiver(){
            }
            int getNumReceived() const {
                return numReceived;
            }
            virtual void waitUntilReady() {
                ready.await();
            }

            virtual void run() {

                try {
                    numReceived = 0;

                    ready.countDown();
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

        virtual void testBasics();
        virtual void testReceiveException();
        virtual void testSendException();

    };

}}

#endif /*_ACTIVEMQ_TEST_CMSTEMPLATETEST_H_*/
