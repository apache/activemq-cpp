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

#include <activemq/util/IntegrationCommon.h>
#include <activemq/core/ActiveMQConnectionFactory.h>
#include <activemq/cmsutil/CmsTemplate.h>
#include <activemq/cmsutil/MessageCreator.h>
#include <activemq/exceptions/ActiveMQException.h>

#include <decaf/util/concurrent/CountDownLatch.h>
#include <decaf/lang/Thread.h>

using namespace std;
using namespace cms;
using namespace activemq;
using namespace activemq::test;
using namespace activemq::util;
using namespace activemq::core;
using namespace activemq::exceptions;
using namespace decaf;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace decaf::lang;

////////////////////////////////////////////////////////////////////////////////
namespace activemq {
namespace test {

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

        virtual cms::Message* createMessage( cms::Session* session )
            throw ( cms::CMSException ) {

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
                    cmsTemplate.send( &tmc );
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
}}

////////////////////////////////////////////////////////////////////////////////
void CmsTemplateTest::testBasics() {

    try {

        const unsigned int NUM_MESSAGES = IntegrationCommon::defaultMsgCount;

        Receiver receiver( this->getBrokerURL(), false, "testBasics1", NUM_MESSAGES);
        Thread rt( &receiver );
        rt.start();

        // Wait for receiver thread to start.
        receiver.waitUntilReady();

        Sender sender( this->getBrokerURL(), false, "testBasics1", NUM_MESSAGES);
        Thread st( &sender );
        st.start();

        st.join();
        rt.join();

        unsigned int numReceived = receiver.getNumReceived();
        CPPUNIT_ASSERT( numReceived == NUM_MESSAGES );

    } catch ( ActiveMQException e ) {
        e.printStackTrace();
        throw e;
    }
}

////////////////////////////////////////////////////////////////////////////////
void CmsTemplateTest::testReceiveException() {

    try {

        // First, try receiving from a bad url
        activemq::core::ActiveMQConnectionFactory cf("tcp://localhost:61666");
        activemq::cmsutil::CmsTemplate cmsTemplate(&cf);
        cmsTemplate.setDefaultDestinationName("testReceive1");

        try {
            cmsTemplate.receive();
            CPPUNIT_FAIL("failed to throw expected exception");
        } catch( ActiveMQException& ex ) {
            // Expected.
        }

        // Now change to a good url and verify that we can reuse the same
        // CmsTemplate successfully.
        activemq::core::ActiveMQConnectionFactory cf2( this->getBrokerURL() );
        cmsTemplate.setConnectionFactory(&cf2);

        // Send 1 message.
        Sender sender( this->getBrokerURL(), false, "testReceive1", 1);
        Thread st( &sender );
        st.start();
        st.join();

        // Receive the message.
        cms::Message* message = cmsTemplate.receive();
        CPPUNIT_ASSERT( message != NULL );
        delete message;

    } catch ( ActiveMQException e ) {
        e.printStackTrace();
        CPPUNIT_ASSERT( false );
    }
}

////////////////////////////////////////////////////////////////////////////////
void CmsTemplateTest::testSendException() {

    try {

        // First, try sending to a bad url.
        activemq::core::ActiveMQConnectionFactory cf( "tcp://localhost:61666" );
        activemq::cmsutil::CmsTemplate cmsTemplate( &cf );
        cmsTemplate.setDefaultDestinationName( "testSend1" );
        try {
            TextMessageCreator msgCreator( "hello world" );
            cmsTemplate.send( &msgCreator );
            CPPUNIT_FAIL( "failed to throw expected exception" );
        } catch( ActiveMQException& ex ) {
            // Expected.
        }

        // Now change to a good url and verify that we can reuse the same
        // CmsTemplate successfully.
        activemq::core::ActiveMQConnectionFactory cf2( this->getBrokerURL() );
        cmsTemplate.setConnectionFactory( &cf2 );
        TextMessageCreator msgCreator( "hello world" );
        cmsTemplate.send( &msgCreator );

    } catch ( ActiveMQException e ) {
        e.printStackTrace();
        throw e;
    }
}
