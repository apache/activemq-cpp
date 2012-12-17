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

#include "OpenwireXATransactionsTest.h"

#include <activemq/core/ActiveMQXAConnectionFactory.h>
#include <activemq/core/ActiveMQXAConnection.h>
#include <activemq/core/ActiveMQXASession.h>
#include <activemq/exceptions/ActiveMQException.h>
#include <activemq/commands/XATransactionId.h>

#include <decaf/lang/Thread.h>
#include <decaf/util/UUID.h>

#include <cms/ConnectionFactory.h>
#include <cms/Connection.h>
#include <cms/Session.h>
#include <cms/MessageConsumer.h>
#include <cms/MessageProducer.h>
#include <cms/MessageListener.h>
#include <cms/XAConnectionFactory.h>
#include <cms/XAConnection.h>
#include <cms/Message.h>
#include <cms/TextMessage.h>

#include <memory>

using namespace cms;
using namespace std;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::util;
using namespace activemq;
using namespace activemq::core;
using namespace activemq::commands;
using namespace activemq::exceptions;
using namespace activemq::test;
using namespace activemq::test::openwire;

////////////////////////////////////////////////////////////////////////////////
const int OpenwireXATransactionsTest::batchCount = 10;
const int OpenwireXATransactionsTest::batchSize = 20;

////////////////////////////////////////////////////////////////////////////////
OpenwireXATransactionsTest::OpenwireXATransactionsTest() : txIdGen() {
}

////////////////////////////////////////////////////////////////////////////////
OpenwireXATransactionsTest::~OpenwireXATransactionsTest() {
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireXATransactionsTest::testCreateXAConnectionFactory() {

    std::auto_ptr<XAConnectionFactory> factory(
        XAConnectionFactory::createCMSXAConnectionFactory( getBrokerURL() ) );
    CPPUNIT_ASSERT( factory.get() != NULL );

    ConnectionFactory* cmsFactory = dynamic_cast<ConnectionFactory*>( factory.get() );
    CPPUNIT_ASSERT( cmsFactory != NULL );

    ActiveMQConnectionFactory* amqFactory = dynamic_cast<ActiveMQConnectionFactory*>( factory.get() );
    CPPUNIT_ASSERT( amqFactory != NULL );

    ActiveMQXAConnectionFactory* amqXAFactory = dynamic_cast<ActiveMQXAConnectionFactory*>( factory.get() );
    CPPUNIT_ASSERT( amqXAFactory != NULL );
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireXATransactionsTest::testCreateXAConnection() {

    std::auto_ptr<XAConnectionFactory> factory(
        XAConnectionFactory::createCMSXAConnectionFactory( getBrokerURL() ) );
    CPPUNIT_ASSERT( factory.get() != NULL );

    std::auto_ptr<XAConnection> connection( factory->createXAConnection() );
    CPPUNIT_ASSERT( connection.get() != NULL );

    Connection* cmsConnection = dynamic_cast<Connection*>( connection.get() );
    CPPUNIT_ASSERT( cmsConnection != NULL );

    ActiveMQConnection* amqConnection = dynamic_cast<ActiveMQConnection*>( connection.get() );
    CPPUNIT_ASSERT( amqConnection != NULL );

    ActiveMQXAConnection* amqXAConnection = dynamic_cast<ActiveMQXAConnection*>( connection.get() );
    CPPUNIT_ASSERT( amqXAConnection != NULL );
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireXATransactionsTest::testCreateXASession() {

    std::auto_ptr<XAConnectionFactory> factory(
        XAConnectionFactory::createCMSXAConnectionFactory( getBrokerURL() ) );
    CPPUNIT_ASSERT( factory.get() != NULL );

    std::auto_ptr<XAConnection> connection( factory->createXAConnection() );
    CPPUNIT_ASSERT( connection.get() != NULL );

    std::auto_ptr<XASession> session( connection->createXASession() );
    CPPUNIT_ASSERT( session.get() != NULL );

    Session* cmsSession = dynamic_cast<Session*>( session.get() );
    CPPUNIT_ASSERT( cmsSession != NULL );

    ActiveMQSession* amqSession = dynamic_cast<ActiveMQSession*>( session.get() );
    CPPUNIT_ASSERT( amqSession != NULL );

    ActiveMQXASession* amqXASession = dynamic_cast<ActiveMQXASession*>( session.get() );
    CPPUNIT_ASSERT( amqXASession != NULL );
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireXATransactionsTest::testGetXAResource() {

    std::auto_ptr<XAConnectionFactory> factory(
        XAConnectionFactory::createCMSXAConnectionFactory( getBrokerURL() ) );
    CPPUNIT_ASSERT( factory.get() != NULL );

    std::auto_ptr<XAConnection> connection( factory->createXAConnection() );
    CPPUNIT_ASSERT( connection.get() != NULL );

    std::auto_ptr<XASession> session( connection->createXASession() );
    CPPUNIT_ASSERT( session.get() != NULL );

    XAResource* xaResource = session->getXAResource();
    CPPUNIT_ASSERT( xaResource != NULL );
    CPPUNIT_ASSERT( xaResource->isSameRM( xaResource ) );
    CPPUNIT_ASSERT_NO_THROW( xaResource->setTransactionTimeout( 10000 ) );
    CPPUNIT_ASSERT( xaResource->getTransactionTimeout() == 0 );
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireXATransactionsTest::testSendReceiveOutsideTX() {

    std::auto_ptr<XAConnectionFactory> factory(
        XAConnectionFactory::createCMSXAConnectionFactory( getBrokerURL() ) );
    CPPUNIT_ASSERT( factory.get() != NULL );

    std::auto_ptr<XAConnection> connection( factory->createXAConnection() );
    CPPUNIT_ASSERT( connection.get() != NULL );

    std::auto_ptr<XASession> session( connection->createXASession() );
    CPPUNIT_ASSERT( session.get() != NULL );

    ActiveMQXASession* amqXASession = dynamic_cast<ActiveMQXASession*>( session.get() );
    CPPUNIT_ASSERT( amqXASession != NULL );

    std::auto_ptr<Destination> destination( session->createTemporaryQueue() );
    std::auto_ptr<MessageProducer> producer( session->createProducer( destination.get() ) );
    std::auto_ptr<MessageConsumer> consumer( session->createConsumer( destination.get() ) );

    CPPUNIT_ASSERT( amqXASession->isAutoAcknowledge() == true );
    CPPUNIT_ASSERT( amqXASession->isTransacted() == false );

    connection->start();

    for( int i = 0; i < 50; ++i ) {
        std::auto_ptr<cms::Message> message( session->createTextMessage( "TEST" ) );
        producer->send( message.get() );
    }

    for( int i = 0; i < 50; ++i ) {
        std::auto_ptr<cms::Message> message( consumer->receive( 3000 ) );
        CPPUNIT_ASSERT( message.get() != NULL );
        CPPUNIT_ASSERT( dynamic_cast<TextMessage*>( message.get() ) != NULL );
    }
}

////////////////////////////////////////////////////////////////////////////////
cms::Xid* OpenwireXATransactionsTest::createXid() const {

    std::string branchQualStr = UUID::randomUUID().toString();
    std::string globalTxIdStr = this->txIdGen.generateId();

    std::vector<unsigned char> branchQual( branchQualStr.begin(), branchQualStr.end() );
    std::vector<unsigned char> globalTxId( globalTxIdStr.begin(), globalTxIdStr.end() );

    if( (int)branchQual.size() > Xid::MAXBQUALSIZE ) {
        branchQual.resize( Xid::MAXBQUALSIZE );
    }

    if( (int)globalTxId.size() > Xid::MAXGTRIDSIZE ) {
        globalTxId.resize( Xid::MAXGTRIDSIZE );
    }

    XATransactionId* id = new XATransactionId();

    id->setFormatId( 0 );
    id->setBranchQualifier( branchQual );
    id->setGlobalTransactionId( globalTxId );

    return id;
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireXATransactionsTest::testSendReceiveTransactedBatches() {

    std::auto_ptr<XAConnectionFactory> factory(
        XAConnectionFactory::createCMSXAConnectionFactory( getBrokerURL() ) );
    CPPUNIT_ASSERT( factory.get() != NULL );

    std::auto_ptr<XAConnection> connection( factory->createXAConnection() );
    CPPUNIT_ASSERT( connection.get() != NULL );

    std::auto_ptr<XASession> session( connection->createXASession() );
    CPPUNIT_ASSERT( session.get() != NULL );

    std::auto_ptr<Destination> destination( session->createTemporaryQueue() );
    std::auto_ptr<MessageProducer> producer( session->createProducer( destination.get() ) );
    std::auto_ptr<MessageConsumer> consumer( session->createConsumer( destination.get() ) );

    XAResource* xaResource = session->getXAResource();
    CPPUNIT_ASSERT( xaResource != NULL );

    connection->start();

    for( int j = 0; j < batchCount; j++ ) {

        std::auto_ptr<cms::Xid> txIdSend( this->createXid() );
        xaResource->start( txIdSend.get(), 0 );

        std::auto_ptr<TextMessage> message( session->createTextMessage( "Batch Message" ) );

        for( int i = 0; i < batchSize; i++ ) {
            CPPUNIT_ASSERT_NO_THROW_MESSAGE(
                "Send should not throw an exception here.",
                producer->send( message.get() ) );
        }

        CPPUNIT_ASSERT_NO_THROW_MESSAGE(
             "Should not have thrown an Exception for xaResource->end",
             xaResource->end( txIdSend.get(), XAResource::TMSUCCESS ) );
        CPPUNIT_ASSERT_NO_THROW_MESSAGE(
             "Should not have thrown an Exception for xaResource->prepare",
             xaResource->prepare( txIdSend.get() ) );
        CPPUNIT_ASSERT_NO_THROW_MESSAGE(
             "Should not have thrown an Exception for xaResource->commit",
             xaResource->commit( txIdSend.get(), false ) );

        std::auto_ptr<cms::Xid> txIdRecv( this->createXid() );
        xaResource->start( txIdRecv.get(), 0 );

        for( int i = 0; i < batchSize; i++ ) {

            CPPUNIT_ASSERT_NO_THROW_MESSAGE(
                "Receive Shouldn't throw a Message here:",
                message.reset( dynamic_cast<TextMessage*>( consumer->receive( 1000 * 5 ) ) ) );

            CPPUNIT_ASSERT_MESSAGE(
                "Failed to receive all messages in batch", message.get() != NULL );
            CPPUNIT_ASSERT( string("Batch Message") == message->getText() );
         }

        CPPUNIT_ASSERT_NO_THROW_MESSAGE(
             "Should not have thrown an Exception for xaResource->end",
             xaResource->end( txIdRecv.get(), XAResource::TMSUCCESS ) );
        CPPUNIT_ASSERT_NO_THROW_MESSAGE(
             "Should not have thrown an Exception for xaResource->prepare",
             xaResource->prepare( txIdRecv.get() ) );
        CPPUNIT_ASSERT_NO_THROW_MESSAGE(
             "Should not have thrown an Exception for xaResource->commit",
             xaResource->commit( txIdRecv.get(), false ) );
    }

    std::auto_ptr<cms::Message> message;

    CPPUNIT_ASSERT_NO_THROW_MESSAGE(
        "Receive Shouldn't throw a Message here:",
        message.reset( consumer->receive( 2000 ) ) );

    CPPUNIT_ASSERT_MESSAGE(
        "Unexpected Message Received after XA Batches all processed", message.get() == NULL );
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireXATransactionsTest::testSendRollback() {

    std::auto_ptr<XAConnectionFactory> factory(
        XAConnectionFactory::createCMSXAConnectionFactory( getBrokerURL() ) );
    CPPUNIT_ASSERT( factory.get() != NULL );

    std::auto_ptr<XAConnection> connection( factory->createXAConnection() );
    CPPUNIT_ASSERT( connection.get() != NULL );

    std::auto_ptr<XASession> session( connection->createXASession() );
    CPPUNIT_ASSERT( session.get() != NULL );

    std::auto_ptr<Destination> destination( session->createTemporaryQueue() );
    std::auto_ptr<MessageProducer> producer( session->createProducer( destination.get() ) );
    std::auto_ptr<MessageConsumer> consumer( session->createConsumer( destination.get() ) );

    XAResource* xaResource = session->getXAResource();
    CPPUNIT_ASSERT( xaResource != NULL );

    connection->start();

    auto_ptr<TextMessage> outbound1( session->createTextMessage( "First Message" ) );
    auto_ptr<TextMessage> outbound2( session->createTextMessage( "Second Message" ) );

    // start a new XA Transaction
    std::auto_ptr<cms::Xid> ixId( this->createXid() );
    xaResource->start( ixId.get(), 0 );

    // sends a message
    producer->send( outbound1.get() );

    // commit the sent message
    xaResource->end( ixId.get(), XAResource::TMSUCCESS );
    xaResource->prepare( ixId.get() );
    xaResource->commit( ixId.get(), false );

    // start a new XA Transaction
    ixId.reset( this->createXid() );
    xaResource->start( ixId.get(), 0 );

    // sends a message that gets rollbacked
    auto_ptr<cms::Message> rollback(
        session->createTextMessage( "I'm going to get rolled back." ) );
    producer->send( rollback.get() );

    // Roll back the sent message
    xaResource->end( ixId.get(), XAResource::TMSUCCESS );
    xaResource->rollback( ixId.get() );

    // start a new XA Transaction
    ixId.reset( this->createXid() );
    xaResource->start( ixId.get(), 0 );

    // sends a message
    producer->send( outbound2.get() );

    // commit the sent message
    xaResource->end( ixId.get(), XAResource::TMSUCCESS );
    xaResource->prepare( ixId.get() );
    xaResource->commit( ixId.get(), false );

    // receives the first message
    auto_ptr<TextMessage> inbound1(
        dynamic_cast<TextMessage*>( consumer->receive( 1500 ) ) );

    // receives the second message
    auto_ptr<TextMessage> inbound2(
        dynamic_cast<TextMessage*>( consumer->receive( 4000 ) ) );

    CPPUNIT_ASSERT( outbound1->getText() == inbound1->getText() );
    CPPUNIT_ASSERT( outbound2->getText() == inbound2->getText() );

    // Checks to make sure there's no other messages on the Destination.
    CPPUNIT_ASSERT( consumer->receive( 3000 ) == NULL );
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireXATransactionsTest::testSendRollbackCommitRollback() {

    std::auto_ptr<XAConnectionFactory> factory(
        XAConnectionFactory::createCMSXAConnectionFactory( getBrokerURL() ) );
    CPPUNIT_ASSERT( factory.get() != NULL );

    std::auto_ptr<XAConnection> connection( factory->createXAConnection() );
    CPPUNIT_ASSERT( connection.get() != NULL );

    std::auto_ptr<XASession> session( connection->createXASession() );
    CPPUNIT_ASSERT( session.get() != NULL );

    std::auto_ptr<Destination> destination( session->createTemporaryQueue() );
    std::auto_ptr<MessageProducer> producer( session->createProducer( destination.get() ) );

    XAResource* xaResource = session->getXAResource();
    CPPUNIT_ASSERT( xaResource != NULL );

    connection->start();

    auto_ptr<TextMessage> outbound1( session->createTextMessage( "First Message" ) );
    auto_ptr<TextMessage> outbound2( session->createTextMessage( "Second Message" ) );

    // start a new XA Transaction
    std::auto_ptr<cms::Xid> ixId( this->createXid() );
    xaResource->start( ixId.get(), 0 );

    // sends them and then rolls back.
    producer->send( outbound1.get() );
    producer->send( outbound2.get() );

    // Roll back the sent message
    xaResource->end( ixId.get(), XAResource::TMSUCCESS );
    xaResource->rollback( ixId.get() );

    // start a new XA Transaction
    ixId.reset( this->createXid() );
    xaResource->start( ixId.get(), 0 );

    // Send one and commit.
    producer->send( outbound1.get() );

    // commit the sent message
    xaResource->end( ixId.get(), XAResource::TMSUCCESS );
    xaResource->prepare( ixId.get() );
    xaResource->commit( ixId.get(), false );

    // start a new XA Transaction
    ixId.reset( this->createXid() );
    xaResource->start( ixId.get(), 0 );

    std::auto_ptr<MessageConsumer> consumer( session->createConsumer( destination.get() ) );

    // receives the first message
    auto_ptr<TextMessage> inbound1(
        dynamic_cast<TextMessage*>( consumer->receive( 1500 ) ) );

    CPPUNIT_ASSERT( NULL == consumer->receive( 1500 ) );
    CPPUNIT_ASSERT( outbound1->getText() == inbound1->getText() );

    // Roll back the sent message
    xaResource->end( ixId.get(), XAResource::TMSUCCESS );
    xaResource->rollback( ixId.get() );

    consumer->close();

    // start a new XA Transaction
    ixId.reset( this->createXid() );
    xaResource->start( ixId.get(), 0 );

    consumer.reset( session->createConsumer( destination.get() ) );

    inbound1.reset(
        dynamic_cast<TextMessage*>( consumer->receive( 1500 ) ) );

    CPPUNIT_ASSERT( NULL == consumer->receive( 1500 ) );
    CPPUNIT_ASSERT( outbound1->getText() == inbound1->getText() );

    // commit the received message
    xaResource->end( ixId.get(), XAResource::TMSUCCESS );
    xaResource->prepare( ixId.get() );
    xaResource->commit( ixId.get(), false );
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireXATransactionsTest::testWithTTLSet() {

    std::auto_ptr<XAConnectionFactory> factory(
        XAConnectionFactory::createCMSXAConnectionFactory( getBrokerURL() ) );
    CPPUNIT_ASSERT( factory.get() != NULL );

    std::auto_ptr<XAConnection> connection( factory->createXAConnection() );
    CPPUNIT_ASSERT( connection.get() != NULL );

    std::auto_ptr<XASession> session( connection->createXASession() );
    CPPUNIT_ASSERT( session.get() != NULL );

    std::auto_ptr<Destination> destination( session->createTemporaryQueue() );
    std::auto_ptr<MessageProducer> producer( session->createProducer( destination.get() ) );

    XAResource* xaResource = session->getXAResource();
    CPPUNIT_ASSERT( xaResource != NULL );

    connection->start();

    auto_ptr<TextMessage> outbound1( session->createTextMessage( "First Message" ) );

    const std::size_t NUM_MESSAGES = 50;

    // start a new XA Transaction
    std::auto_ptr<cms::Xid> ixId( this->createXid() );
    xaResource->start( ixId.get(), 0 );

    // sends a message
    for( std::size_t i = 0; i < NUM_MESSAGES; ++i ) {
        producer->send( outbound1.get(), cms::DeliveryMode::PERSISTENT, 4, 120*1000 );
    }

    // commit the sent messages
    xaResource->end( ixId.get(), XAResource::TMSUCCESS );
    xaResource->prepare( ixId.get() );
    xaResource->commit( ixId.get(), false );

    // start a new XA Transaction
    ixId.reset( this->createXid() );
    xaResource->start( ixId.get(), 0 );

    std::auto_ptr<MessageConsumer> consumer( session->createConsumer( destination.get() ) );

    for( std::size_t i = 0; i < NUM_MESSAGES; ++i ) {

        auto_ptr<TextMessage> inbound1(
            dynamic_cast<TextMessage*>( consumer->receive( 600000 ) ) );
        CPPUNIT_ASSERT( inbound1.get() != NULL );
        CPPUNIT_ASSERT( outbound1->getText() == inbound1->getText() );
    }

    // commit the received messages
    xaResource->end( ixId.get(), XAResource::TMSUCCESS );
    xaResource->prepare( ixId.get() );
    xaResource->commit( ixId.get(), false );
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireXATransactionsTest::testXAResource_Exception1() {

    std::auto_ptr<XAConnectionFactory> factory(
        XAConnectionFactory::createCMSXAConnectionFactory( getBrokerURL() ) );
    CPPUNIT_ASSERT( factory.get() != NULL );

    std::auto_ptr<XAConnection> connection( factory->createXAConnection() );
    CPPUNIT_ASSERT( connection.get() != NULL );

    std::auto_ptr<XASession> session( connection->createXASession() );
    CPPUNIT_ASSERT( session.get() != NULL );

    std::auto_ptr<Destination> destination( session->createTemporaryQueue() );
    std::auto_ptr<MessageProducer> producer( session->createProducer( destination.get() ) );

    XAResource* xaResource = session->getXAResource();
    CPPUNIT_ASSERT( xaResource != NULL );

    // start a new XA Transaction
    std::auto_ptr<cms::Xid> ixId( this->createXid() );
    xaResource->start( ixId.get(), 0 );

    // prepare the sent messages without an end call.
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Prepare Should have thrown an XAException",
        xaResource->prepare( ixId.get() ),
        XAException );

    xaResource->forget( ixId.get() );
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireXATransactionsTest::testXAResource_Exception2() {

    std::auto_ptr<XAConnectionFactory> factory(
        XAConnectionFactory::createCMSXAConnectionFactory( getBrokerURL() ) );
    CPPUNIT_ASSERT( factory.get() != NULL );

    std::auto_ptr<XAConnection> connection( factory->createXAConnection() );
    CPPUNIT_ASSERT( connection.get() != NULL );

    std::auto_ptr<XASession> session( connection->createXASession() );
    CPPUNIT_ASSERT( session.get() != NULL );

    std::auto_ptr<Destination> destination( session->createTemporaryQueue() );
    std::auto_ptr<MessageProducer> producer( session->createProducer( destination.get() ) );

    XAResource* xaResource = session->getXAResource();
    CPPUNIT_ASSERT( xaResource != NULL );

    // start a new XA Transaction
    std::auto_ptr<cms::Xid> ixId( this->createXid() );
    xaResource->start( ixId.get(), 0 );

    // commit the sent messages without an end call.
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Commit Should have thrown an XAException",
        xaResource->commit( ixId.get(), true ),
        XAException );

    xaResource->forget( ixId.get() );
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireXATransactionsTest::testXAResource_Exception3() {

    std::auto_ptr<XAConnectionFactory> factory(
        XAConnectionFactory::createCMSXAConnectionFactory( getBrokerURL() ) );
    CPPUNIT_ASSERT( factory.get() != NULL );

    std::auto_ptr<XAConnection> connection( factory->createXAConnection() );
    CPPUNIT_ASSERT( connection.get() != NULL );

    std::auto_ptr<XASession> session( connection->createXASession() );
    CPPUNIT_ASSERT( session.get() != NULL );

    std::auto_ptr<Destination> destination( session->createTemporaryQueue() );
    std::auto_ptr<MessageProducer> producer( session->createProducer( destination.get() ) );

    XAResource* xaResource = session->getXAResource();
    CPPUNIT_ASSERT( xaResource != NULL );

    // start a new XA Transaction
    std::auto_ptr<cms::Xid> ixId( this->createXid() );
    std::auto_ptr<cms::Xid> ixIdOther( this->createXid() );
    xaResource->start( ixId.get(), 0 );

    // rollback the sent messages without an end call.
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "end Should have thrown an XAException",
        xaResource->end( ixIdOther.get(), XAResource::TMSUSPEND ),
        XAException );

    xaResource->forget( ixId.get() );
}
