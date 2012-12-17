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

#include "MapMessageTest.h"

#include <activemq/util/CMSListener.h>
#include <activemq/exceptions/ActiveMQException.h>
#include <activemq/core/ActiveMQConnection.h>
#include <activemq/commands/ActiveMQMapMessage.h>

using namespace std;
using namespace cms;
using namespace activemq;
using namespace activemq::commands;
using namespace activemq::core;
using namespace activemq::test;
using namespace activemq::util;
using namespace activemq::exceptions;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::util;

////////////////////////////////////////////////////////////////////////////////
MapMessageTest::MapMessageTest() {
}

////////////////////////////////////////////////////////////////////////////////
MapMessageTest::~MapMessageTest() {
}

////////////////////////////////////////////////////////////////////////////////
void MapMessageTest::testEmptyMapSendReceive() {

    // Create CMS Object for Comms
    cms::Session* session( cmsProvider->getSession() );
    cms::MessageConsumer* consumer = cmsProvider->getConsumer();
    cms::MessageProducer* producer = cmsProvider->getProducer();
    producer->setDeliveryMode( DeliveryMode::NON_PERSISTENT );

    auto_ptr<cms::MapMessage> mapMessage( session->createMapMessage() );

    // Send some text messages
    producer->send( mapMessage.get() );

    auto_ptr<cms::Message> message( consumer->receive( 2000 ) );
    CPPUNIT_ASSERT( message.get() != NULL );

    cms::MapMessage* recvMapMessage = dynamic_cast<MapMessage*>( message.get() );
    CPPUNIT_ASSERT( recvMapMessage != NULL );
    CPPUNIT_ASSERT( recvMapMessage->itemExists( "SomeKey" ) == false );
}

////////////////////////////////////////////////////////////////////////////////
void MapMessageTest::testMapWithEmptyStringValue() {

    // Create CMS Object for Comms
    cms::Session* session( cmsProvider->getSession() );
    cms::MessageConsumer* consumer = cmsProvider->getConsumer();
    cms::MessageProducer* producer = cmsProvider->getProducer();
    producer->setDeliveryMode( DeliveryMode::NON_PERSISTENT );

    auto_ptr<cms::MapMessage> mapMessage( session->createMapMessage() );

    mapMessage->setString("String1", "");
    mapMessage->setString("String2", "value");

    // Send some text messages
    producer->send( mapMessage.get() );

    auto_ptr<cms::Message> message( consumer->receive( 2000 ) );
    CPPUNIT_ASSERT( message.get() != NULL );

    cms::MapMessage* recvMapMessage = dynamic_cast<MapMessage*>( message.get() );
    CPPUNIT_ASSERT( recvMapMessage != NULL );
    CPPUNIT_ASSERT( recvMapMessage->itemExists( "String1" ) == true );
    CPPUNIT_ASSERT( recvMapMessage->getString( "String1" ) == "" );
    CPPUNIT_ASSERT( recvMapMessage->itemExists( "String2" ) == true );
    CPPUNIT_ASSERT( recvMapMessage->itemExists( "String3" ) == false );
    CPPUNIT_ASSERT( recvMapMessage->getString( "String2" ) == string( "value" ) );
}

////////////////////////////////////////////////////////////////////////////////
void MapMessageTest::testMapSetEmptyBytesVector() {

    // Create CMS Object for Comms
    cms::Session* session( cmsProvider->getSession() );
    cms::MessageConsumer* consumer = cmsProvider->getConsumer();
    cms::MessageProducer* producer = cmsProvider->getProducer();
    producer->setDeliveryMode( DeliveryMode::NON_PERSISTENT );

    auto_ptr<cms::MapMessage> mapMessage( session->createMapMessage() );

    std::vector<unsigned char> bytes;

    mapMessage->setBytes( "BYTES", bytes );

    // Send some text messages
    producer->send( mapMessage.get() );

    auto_ptr<cms::Message> message( consumer->receive( 2000 ) );
    CPPUNIT_ASSERT( message.get() != NULL );

    cms::MapMessage* recvMapMessage = dynamic_cast<MapMessage*>( message.get() );
    CPPUNIT_ASSERT( recvMapMessage != NULL );
    CPPUNIT_ASSERT( recvMapMessage->itemExists( "BYTES" ) == true );
    CPPUNIT_ASSERT( recvMapMessage->getBytes( "BYTES" ).empty() == true );
}
