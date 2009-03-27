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

#include "TransactionTest.h"

#include <activemq/util/CMSListener.h>
#include <activemq/exceptions/ActiveMQException.h>

using namespace std;
using namespace cms;
using namespace activemq;
using namespace activemq::test;
using namespace activemq::util;
using namespace activemq::exceptions;

////////////////////////////////////////////////////////////////////////////////
void TransactionTest::testSendReceiveTransactedBatches() {

    try {

        // Create CMS Object for Comms
        cms::Session* session = cmsProvider->getSession();
        cms::MessageConsumer* consumer = cmsProvider->getConsumer();
        cms::MessageProducer* producer = cmsProvider->getProducer();

        producer->setDeliveryMode( DeliveryMode::NON_PERSISTENT );

        auto_ptr<TextMessage> message( session->createTextMessage( "Batch Message" ) );

        for( int j = 0; j < batchCount; j++ ) {

            for( int i = 0; i < batchSize; i++ ) {
                producer->send( message.get() );
            }

            session->commit();

            for( int i = 0; i < batchSize; i++ ) {
                message.reset( dynamic_cast<TextMessage*>( consumer->receive( 1000 * 5 ) ) );

                CPPUNIT_ASSERT_MESSAGE(
                    "Failed to receive all messages in batch", message.get() != NULL );
                CPPUNIT_ASSERT( string("Batch Message") == message->getText() );
            }

            session->commit();
        }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void TransactionTest::testSendRollback() {

    try {

        // Create CMS Object for Comms
        cms::Session* session = cmsProvider->getSession();
        cms::MessageConsumer* consumer = cmsProvider->getConsumer();
        cms::MessageProducer* producer = cmsProvider->getProducer();

        producer->setDeliveryMode( DeliveryMode::NON_PERSISTENT );

        auto_ptr<TextMessage> outbound1( session->createTextMessage( "First Message" ) );
        auto_ptr<TextMessage> outbound2( session->createTextMessage( "Second Message" ) );

        // sends a message
        producer->send( outbound1.get() );
        session->commit();

        // sends a message that gets rollbacked
        auto_ptr<Message> rollback(
            session->createTextMessage( "I'm going to get rolled back." ) );
        producer->send( rollback.get() );
        session->rollback();

        // sends a message
        producer->send( outbound2.get() );
        session->commit();

        // receives the first message
        auto_ptr<TextMessage> inbound1(
            dynamic_cast<TextMessage*>( consumer->receive( 1500 ) ) );

        // receives the second message
        auto_ptr<TextMessage> inbound2(
            dynamic_cast<TextMessage*>( consumer->receive( 4000 ) ) );

        // validates that the rollbacked was not consumed
        session->commit();

        CPPUNIT_ASSERT( outbound1->getText() == inbound1->getText() );
        CPPUNIT_ASSERT( outbound2->getText() == inbound2->getText() );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void TransactionTest::testSendSessionClose() {

    try {

        cmsProvider->getProducer()->setDeliveryMode( DeliveryMode::NON_PERSISTENT );

        auto_ptr<TextMessage> outbound1(
            cmsProvider->getSession()->createTextMessage( "First Message" ) );
        auto_ptr<TextMessage> outbound2(
            cmsProvider->getSession()->createTextMessage( "Second Message" ) );

        // sends a message
        cmsProvider->getProducer()->send( outbound1.get() );
        cmsProvider->getSession()->commit();

        // sends a message that gets rollbacked
        auto_ptr<cms::Message> rollback(
            cmsProvider->getSession()->createTextMessage( "I'm going to get rolled back." ) );
        cmsProvider->getProducer()->send( rollback.get() );
        cmsProvider->getConsumer()->close();

        cmsProvider->reconnectSession();

        // sends a message
        cmsProvider->getProducer()->send( outbound2.get() );
        cmsProvider->getSession()->commit();

        // receives the first message
        auto_ptr<TextMessage> inbound1(
            dynamic_cast<TextMessage*>( cmsProvider->getConsumer()->receive( 1500 ) ) );

        // receives the second message
        auto_ptr<cms::TextMessage> inbound2(
            dynamic_cast<TextMessage*>( cmsProvider->getConsumer()->receive( 4000 ) ) );

        // validates that the rollbacked was not consumed
        cmsProvider->getSession()->commit();

        CPPUNIT_ASSERT( outbound1->getText() == inbound1->getText() );
        CPPUNIT_ASSERT( outbound2->getText() == inbound2->getText() );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void TransactionTest::testWithTTLSet() {

    try{
        cmsProvider->getProducer()->setDeliveryMode( DeliveryMode::PERSISTENT );

        cms::MessageConsumer* consumer = cmsProvider->getConsumer();

        auto_ptr<TextMessage> outbound1(
            cmsProvider->getSession()->createTextMessage( "First Message" ) );

        const std::size_t NUM_MESSAGES = 50;

        // sends a message
        for( std::size_t i = 0; i < NUM_MESSAGES; ++i ) {
            cmsProvider->getProducer()->send( outbound1.get(),
                                              cms::DeliveryMode::PERSISTENT,
                                              cmsProvider->getProducer()->getPriority(),
                                              120*1000 );
        }

        cmsProvider->getSession()->commit();

        for( std::size_t i = 0; i < NUM_MESSAGES; ++i ) {

            // receives the second message
            auto_ptr<TextMessage> inbound1(
                dynamic_cast<TextMessage*>( consumer->receive( 4000 ) ) );
            CPPUNIT_ASSERT( outbound1->getText() == inbound1->getText() );
        }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}
