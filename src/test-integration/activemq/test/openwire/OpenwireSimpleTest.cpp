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

#include "OpenwireSimpleTest.h"

#include <activemq/util/CMSListener.h>
#include <activemq/core/ActiveMQConnection.h>
#include <activemq/exceptions/ActiveMQException.h>

#include <decaf/util/UUID.h>

using namespace std;
using namespace cms;
using namespace activemq;
using namespace activemq::core;
using namespace activemq::test;
using namespace activemq::test::openwire;
using namespace activemq::util;
using namespace activemq::exceptions;
using namespace decaf;
using namespace decaf::util;

////////////////////////////////////////////////////////////////////////////////
OpenwireSimpleTest::OpenwireSimpleTest() {
}

////////////////////////////////////////////////////////////////////////////////
OpenwireSimpleTest::~OpenwireSimpleTest() {
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireSimpleTest::testWithZeroConsumerPrefetch() {

    try {

        cmsProvider->setTopic( false );
        cmsProvider->setDestinationName(
            UUID::randomUUID().toString() + "?consumer.prefetchSize=0" );

        cmsProvider->reconnectSession();

        // Create CMS Object for Comms
        cms::Session* session( cmsProvider->getSession() );
        cms::MessageConsumer* consumer = cmsProvider->getConsumer();
        cms::MessageProducer* producer = cmsProvider->getProducer();
        producer->setDeliveryMode( DeliveryMode::NON_PERSISTENT );

        auto_ptr<cms::TextMessage> txtMessage( session->createTextMessage( "TEST MESSAGE" ) );

        // Send some text messages
        producer->send( txtMessage.get() );

        auto_ptr<cms::Message> message( consumer->receive( 1000 ) );
        CPPUNIT_ASSERT( message.get() != NULL );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireSimpleTest::testMapMessageSendToQueue() {

    try {

        cmsProvider->setTopic( false );
        cmsProvider->setDestinationName(
            UUID::randomUUID().toString() + "?consumer.prefetchSize=0" );

        cmsProvider->reconnectSession();

        // Create CMS Object for Comms
        cms::Session* session( cmsProvider->getSession() );
        cms::MessageConsumer* consumer = cmsProvider->getConsumer();
        cms::MessageProducer* producer = cmsProvider->getProducer();
        producer->setDeliveryMode( DeliveryMode::NON_PERSISTENT );

        unsigned char byteValue = 'A';
        char charValue = 'B';
        bool booleanValue = true;
        short shortValue = 2048;
        int intValue = 655369;
        long long longValue = 0xFFFFFFFF00000000ULL;
        float floatValue = 45.6545f;
        double doubleValue = 654564.654654;
        std::string stringValue = "The test string";

        auto_ptr<cms::MapMessage> mapMessage( session->createMapMessage() );

        mapMessage->setString( "stringKey", stringValue );
        mapMessage->setBoolean( "boolKey", booleanValue );
        mapMessage->setByte( "byteKey", byteValue );
        mapMessage->setChar( "charKey", charValue );
        mapMessage->setShort( "shortKey", shortValue );
        mapMessage->setInt( "intKey", intValue );
        mapMessage->setLong( "longKey", longValue );
        mapMessage->setFloat( "floatKey", floatValue );
        mapMessage->setDouble( "doubleKey", doubleValue );

        std::vector<unsigned char> bytes;
        bytes.push_back( 65 );
        bytes.push_back( 66 );
        bytes.push_back( 67 );
        bytes.push_back( 68 );
        bytes.push_back( 69 );
        mapMessage->setBytes( "bytesKey", bytes );

        // Send some text messages
        producer->send( mapMessage.get() );

        auto_ptr<cms::Message> message( consumer->receive( 2000 ) );
        CPPUNIT_ASSERT( message.get() != NULL );

        cms::MapMessage* recvMapMessage = dynamic_cast<MapMessage*>( message.get() );
        CPPUNIT_ASSERT( recvMapMessage != NULL );
        CPPUNIT_ASSERT( recvMapMessage->getString( "stringKey" ) == stringValue );
        CPPUNIT_ASSERT( recvMapMessage->getBoolean( "boolKey" ) == booleanValue );
        CPPUNIT_ASSERT( recvMapMessage->getByte( "byteKey" ) == byteValue );
        CPPUNIT_ASSERT( recvMapMessage->getChar( "charKey" ) == charValue );
        CPPUNIT_ASSERT( recvMapMessage->getShort( "shortKey" ) == shortValue );
        CPPUNIT_ASSERT( recvMapMessage->getInt( "intKey" ) == intValue );
        CPPUNIT_ASSERT( recvMapMessage->getLong( "longKey" ) == longValue );
        CPPUNIT_ASSERT( recvMapMessage->getFloat( "floatKey" ) == floatValue );
        CPPUNIT_ASSERT( recvMapMessage->getDouble( "doubleKey" ) == doubleValue );
        CPPUNIT_ASSERT( recvMapMessage->getBytes( "bytesKey" ) == bytes );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireSimpleTest::testMapMessageSendToTopic() {

    try {

        // Create CMS Object for Comms
        cms::Session* session( cmsProvider->getSession() );
        cms::MessageConsumer* consumer = cmsProvider->getConsumer();
        cms::MessageProducer* producer = cmsProvider->getProducer();
        producer->setDeliveryMode( DeliveryMode::NON_PERSISTENT );

        unsigned char byteValue = 'A';
        char charValue = 'B';
        bool booleanValue = true;
        short shortValue = 2048;
        int intValue = 655369;
        long long longValue = 0xFFFFFFFF00000000ULL;
        float floatValue = 45.6545f;
        double doubleValue = 654564.654654;
        std::string stringValue = "The test string";

        auto_ptr<cms::MapMessage> mapMessage( session->createMapMessage() );

        mapMessage->setString( "stringKey", stringValue );
        mapMessage->setBoolean( "boolKey", booleanValue );
        mapMessage->setByte( "byteKey", byteValue );
        mapMessage->setChar( "charKey", charValue );
        mapMessage->setShort( "shortKey", shortValue );
        mapMessage->setInt( "intKey", intValue );
        mapMessage->setLong( "longKey", longValue );
        mapMessage->setFloat( "floatKey", floatValue );
        mapMessage->setDouble( "doubleKey", doubleValue );

        std::vector<unsigned char> bytes;
        bytes.push_back( 65 );
        bytes.push_back( 66 );
        bytes.push_back( 67 );
        bytes.push_back( 68 );
        bytes.push_back( 69 );
        mapMessage->setBytes( "bytesKey", bytes );

        // Send some text messages
        producer->send( mapMessage.get() );

        auto_ptr<cms::Message> message( consumer->receive( 2000 ) );
        CPPUNIT_ASSERT( message.get() != NULL );

        cms::MapMessage* recvMapMessage = dynamic_cast<MapMessage*>( message.get() );
        CPPUNIT_ASSERT( recvMapMessage != NULL );
        CPPUNIT_ASSERT( recvMapMessage->getString( "stringKey" ) == stringValue );
        CPPUNIT_ASSERT( recvMapMessage->getBoolean( "boolKey" ) == booleanValue );
        CPPUNIT_ASSERT( recvMapMessage->getByte( "byteKey" ) == byteValue );
        CPPUNIT_ASSERT( recvMapMessage->getChar( "charKey" ) == charValue );
        CPPUNIT_ASSERT( recvMapMessage->getShort( "shortKey" ) == shortValue );
        CPPUNIT_ASSERT( recvMapMessage->getInt( "intKey" ) == intValue );
        CPPUNIT_ASSERT( recvMapMessage->getLong( "longKey" ) == longValue );
        CPPUNIT_ASSERT( recvMapMessage->getFloat( "floatKey" ) == floatValue );
        CPPUNIT_ASSERT( recvMapMessage->getDouble( "doubleKey" ) == doubleValue );
        CPPUNIT_ASSERT( recvMapMessage->getBytes( "bytesKey" ) == bytes );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireSimpleTest::testDestroyDestination() {

    try {

        cmsProvider->setDestinationName( "testDestroyDestination" );
        cmsProvider->reconnectSession();

        // Create CMS Object for Comms
        cms::Session* session( cmsProvider->getSession() );
        cms::MessageConsumer* consumer = cmsProvider->getConsumer();
        cms::MessageProducer* producer = cmsProvider->getProducer();
        producer->setDeliveryMode( DeliveryMode::NON_PERSISTENT );

        auto_ptr<cms::TextMessage> txtMessage( session->createTextMessage( "TEST MESSAGE" ) );

        // Send some text messages
        producer->send( txtMessage.get() );

        auto_ptr<cms::Message> message( consumer->receive( 1000 ) );
        CPPUNIT_ASSERT( message.get() != NULL );

        ActiveMQConnection* connection =
            dynamic_cast<ActiveMQConnection*>( cmsProvider->getConnection() );

        CPPUNIT_ASSERT( connection != NULL );

        try{
            connection->destroyDestination( cmsProvider->getDestination() );
            CPPUNIT_ASSERT_MESSAGE( "Destination Should be in use.", false );
        } catch( ActiveMQException& ex ) {
        }

        cmsProvider->reconnectSession();

        connection->destroyDestination( cmsProvider->getDestination() );

    } catch( ActiveMQException& ex ) {
        ex.printStackTrace();
        CPPUNIT_ASSERT_MESSAGE( "CAUGHT EXCEPTION", false );
    } AMQ_CATCHALL_THROW( ActiveMQException )
}
