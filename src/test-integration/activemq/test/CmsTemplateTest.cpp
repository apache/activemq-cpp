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
#include <activemq/exceptions/ActiveMQException.h>

#include <decaf/lang/Thread.h>

#include <cms/Connection.h>
#include <cms/MessageConsumer.h>
#include <cms/MessageProducer.h>
#include <cms/MessageListener.h>
#include <cms/Startable.h>
#include <cms/Closeable.h>
#include <cms/MessageListener.h>
#include <cms/ExceptionListener.h>
#include <cms/Topic.h>
#include <cms/Queue.h>
#include <cms/TemporaryTopic.h>
#include <cms/TemporaryQueue.h>
#include <cms/Session.h>
#include <cms/BytesMessage.h>
#include <cms/TextMessage.h>

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
