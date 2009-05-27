/**
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "VirtualTopicTest.h"

#include <activemq/exceptions/ActiveMQException.h>
#include <memory>
#include <string>

#include <cms/Topic.h>
#include <cms/TextMessage.h>
#include <cms/MessageConsumer.h>
#include <cms/MessageProducer.h>

using namespace std;
using namespace activemq;
using namespace activemq::util;
using namespace activemq::test;
using namespace activemq::exceptions;

////////////////////////////////////////////////////////////////////////////////
const string VirtualTopicTest::PRODUCER_DESTINATION_NAME = "VirtualTopic.TestDestination";
const string VirtualTopicTest::CONSUMER_A_DESTINATION_NAME = "Consumer.A.VirtualTopic.TestDestination";
const string VirtualTopicTest::CONSUMER_B_DESTINATION_NAME = "Consumer.B.VirtualTopic.TestDestination";

////////////////////////////////////////////////////////////////////////////////
VirtualTopicTest::VirtualTopicTest() {
}

////////////////////////////////////////////////////////////////////////////////
VirtualTopicTest::~VirtualTopicTest() {
}

////////////////////////////////////////////////////////////////////////////////
void VirtualTopicTest::testVirtualTopicSyncReceiveAutoAck() {
    this->testRunnerSync( cms::Session::AUTO_ACKNOWLEDGE );
}

////////////////////////////////////////////////////////////////////////////////
void VirtualTopicTest::testVirtualTopicSyncReceiveClinetAck() {
    this->testRunnerSync( cms::Session::CLIENT_ACKNOWLEDGE );
}

////////////////////////////////////////////////////////////////////////////////
void VirtualTopicTest::testVirtualTopicSyncReceiveTransacted() {
    this->testRunnerSync( cms::Session::SESSION_TRANSACTED );
}

////////////////////////////////////////////////////////////////////////////////
void VirtualTopicTest::testRunnerSync( cms::Session::AcknowledgeMode mode ) {

    cmsProvider->setAckMode( mode );
    cmsProvider->reconnectSession();

    // Create CMS Object for Comms
    cms::Session* session( cmsProvider->getSession() );

    auto_ptr<cms::Destination> topic( session->createTopic( PRODUCER_DESTINATION_NAME ) );
    auto_ptr<cms::Destination> queueA( session->createQueue( CONSUMER_A_DESTINATION_NAME ) );
    auto_ptr<cms::Destination> queueB( session->createQueue( CONSUMER_B_DESTINATION_NAME ) );

    auto_ptr<cms::MessageProducer> producer( session->createProducer( topic.get() ) );
    auto_ptr<cms::MessageConsumer> consumerA( session->createConsumer( queueA.get() ) );
    auto_ptr<cms::MessageConsumer> consumerB( session->createConsumer( queueB.get() ) );

    producer->setDeliveryMode( cms::DeliveryMode::NON_PERSISTENT );

    auto_ptr<cms::TextMessage> txtMessage( session->createTextMessage( "TEST MESSAGE" ) );

    for( std::size_t i = 0; i < IntegrationCommon::defaultMsgCount; ++i ) {
        producer->send( txtMessage.get() );
    }

    if( cms::Session::SESSION_TRANSACTED == mode ) {
        session->commit();
    }

    for( std::size_t i = 0; i < IntegrationCommon::defaultMsgCount; ++i ) {

        auto_ptr<cms::Message> messageA( consumerA->receive( 2000 ) );
        CPPUNIT_ASSERT( messageA.get() != NULL );
        if( cms::Session::CLIENT_ACKNOWLEDGE == mode ) {
            messageA->acknowledge();
        }

        auto_ptr<cms::Message> messageB( consumerB->receive( 2000 ) );
        CPPUNIT_ASSERT( messageB.get() != NULL );
        if( cms::Session::CLIENT_ACKNOWLEDGE == mode ) {
            messageB->acknowledge();
        }
    }

    if( cms::Session::SESSION_TRANSACTED == mode ) {
        session->commit();
    }
}
