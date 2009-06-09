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

#include "AsyncSenderTest.h"

#include <activemq/util/CMSListener.h>
#include <activemq/util/IntegrationCommon.h>

using namespace activemq;
using namespace activemq::test;
using namespace activemq::util;
using namespace std;
using namespace cms;

////////////////////////////////////////////////////////////////////////////////
AsyncSenderTest::AsyncSenderTest() {
}

////////////////////////////////////////////////////////////////////////////////
AsyncSenderTest::~AsyncSenderTest() {
}

////////////////////////////////////////////////////////////////////////////////
void AsyncSenderTest::testAsyncSends() {

    try {

        // Create CMS Object for Comms
        cms::Session* session( cmsProvider->getSession() );

        CMSListener listener( session );

        cms::MessageConsumer* consumer = cmsProvider->getConsumer();
        consumer->setMessageListener( &listener );
        cms::MessageProducer* producer = cmsProvider->getProducer();
        producer->setDeliveryMode( DeliveryMode::NON_PERSISTENT );

        auto_ptr<cms::TextMessage> txtMessage( session->createTextMessage( "TEST MESSAGE" ) );
        auto_ptr<cms::BytesMessage> bytesMessage( session->createBytesMessage() );

        for( unsigned int i = 0; i < IntegrationCommon::defaultMsgCount; ++i ) {
            producer->send( txtMessage.get() );
        }

        for( unsigned int i = 0; i < IntegrationCommon::defaultMsgCount; ++i ) {
            producer->send( bytesMessage.get() );
        }

        // Wait for the messages to get here
        listener.asyncWaitForMessages( IntegrationCommon::defaultMsgCount * 2 );

        unsigned int numReceived = listener.getNumReceived();
        CPPUNIT_ASSERT( numReceived == IntegrationCommon::defaultMsgCount * 2 );

    } catch(...) {
        CPPUNIT_ASSERT( false );
    }
}
