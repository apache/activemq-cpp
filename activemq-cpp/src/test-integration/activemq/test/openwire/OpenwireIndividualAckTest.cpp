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

#include "OpenwireIndividualAckTest.h"

#include <activemq/exceptions/ActiveMQException.h>

#include <cms/Session.h>

#include <memory>

using namespace cms;
using namespace std;
using namespace activemq;
using namespace activemq::test;
using namespace activemq::test::openwire;
using namespace activemq::exceptions;

////////////////////////////////////////////////////////////////////////////////
OpenwireIndividualAckTest::OpenwireIndividualAckTest() {
}

////////////////////////////////////////////////////////////////////////////////
OpenwireIndividualAckTest::~OpenwireIndividualAckTest() {
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireIndividualAckTest::testAckedMessageAreConsumed() {

    Connection* connection = this->cmsProvider->getConnection();
    connection->start();

    std::auto_ptr<Session> session( connection->createSession( cms::Session::INDIVIDUAL_ACKNOWLEDGE ) );
    std::auto_ptr<Destination> queue( session->createTemporaryQueue() );
    std::auto_ptr<MessageProducer> producer( session->createProducer( queue.get() ) );

    std::auto_ptr<TextMessage> msg1( session->createTextMessage("Hello") );
    producer->send( msg1.get() );

    // Consume the message...
    std::auto_ptr<MessageConsumer> consumer( session->createConsumer( queue.get() ) );
    std::auto_ptr<Message> msg( consumer->receive( 1000 ) );
    CPPUNIT_ASSERT( msg.get() != NULL );
    msg->acknowledge();

    // Reset the session.
    session->close();
    session.reset( connection->createSession( Session::INDIVIDUAL_ACKNOWLEDGE ) );

    // Attempt to Consume the message...
    consumer.reset( session->createConsumer( queue.get() ) );
    msg.reset( consumer->receive( 1000 ) );
    CPPUNIT_ASSERT( msg.get() == NULL );

    session->close();
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireIndividualAckTest::testLastMessageAcked(){

    Connection* connection = this->cmsProvider->getConnection();
    connection->start();

    std::auto_ptr<Session> session( connection->createSession( cms::Session::INDIVIDUAL_ACKNOWLEDGE ) );
    std::auto_ptr<Destination> queue( session->createTemporaryQueue() );
    std::auto_ptr<MessageProducer> producer( session->createProducer( queue.get() ) );

    std::auto_ptr<TextMessage> msg1( session->createTextMessage("msg1") );
    std::auto_ptr<TextMessage> msg2( session->createTextMessage("msg2") );
    std::auto_ptr<TextMessage> msg3( session->createTextMessage("msg3") );

    producer->send( msg1.get() );
    producer->send( msg2.get() );
    producer->send( msg3.get() );

    // Consume the message...
    std::auto_ptr<MessageConsumer> consumer( session->createConsumer( queue.get() ) );
    std::auto_ptr<Message> msg( consumer->receive( 1000 ) );
    CPPUNIT_ASSERT( msg.get() != NULL );
    msg.reset( consumer->receive( 1000 ) );
    CPPUNIT_ASSERT( msg.get() != NULL );
    msg.reset( consumer->receive( 1000 ) );
    CPPUNIT_ASSERT( msg.get() != NULL );
    msg->acknowledge();

    // Reset the session->
    session->close();
    session.reset( connection->createSession( Session::INDIVIDUAL_ACKNOWLEDGE) );

    // Attempt to Consume the message...
    consumer.reset( session->createConsumer( queue.get() ) );
    msg.reset( consumer->receive( 1000 ) );
    CPPUNIT_ASSERT( msg.get() != NULL );
    CPPUNIT_ASSERT( msg1->getText() == dynamic_cast<TextMessage*>( msg.get() )->getText() );
    msg.reset( consumer->receive( 1000 ) );
    CPPUNIT_ASSERT( msg.get() != NULL );
    CPPUNIT_ASSERT( msg2->getText() == dynamic_cast<TextMessage*>( msg.get() )->getText() );
    msg.reset( consumer->receive( 1000 ) );
    CPPUNIT_ASSERT( msg.get() == NULL );

    session->close();
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireIndividualAckTest::testUnAckedMessageAreNotConsumedOnSessionClose() {

    Connection* connection = this->cmsProvider->getConnection();
    connection->start();

    std::auto_ptr<Session> session( connection->createSession( cms::Session::INDIVIDUAL_ACKNOWLEDGE ) );
    std::auto_ptr<Destination> queue( session->createTemporaryQueue() );
    std::auto_ptr<MessageProducer> producer( session->createProducer( queue.get() ) );

    std::auto_ptr<TextMessage> msg1( session->createTextMessage("Hello") );
    producer->send( msg1.get() );

    // Consume the message...
    std::auto_ptr<MessageConsumer> consumer( session->createConsumer( queue.get() ) );
    std::auto_ptr<Message> msg( consumer->receive( 1000 ) );
    CPPUNIT_ASSERT( msg.get() != NULL );
    // Don't ack the message.

    // Reset the session->  This should cause the unacknowledged message to be re-delivered.
    session->close();
    session.reset( connection->createSession( Session::INDIVIDUAL_ACKNOWLEDGE ) );

    // Attempt to Consume the message...
    consumer.reset( session->createConsumer( queue.get() ) );
    msg.reset( consumer->receive( 2000 ) );
    CPPUNIT_ASSERT( msg.get() != NULL );
    msg->acknowledge();

    session->close();
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireIndividualAckTest::testIndividualAcknowledgeMultiMessages_AcknowledgeFirstTest()
{
    Connection* connection = this->cmsProvider->getConnection();
    connection->start();

    std::auto_ptr<Session> session( connection->createSession( cms::Session::INDIVIDUAL_ACKNOWLEDGE ) );
    std::auto_ptr<Destination> queue( session->createTemporaryQueue() );
    std::auto_ptr<MessageProducer> producer( session->createProducer( queue.get() ) );

    std::auto_ptr<TextMessage> msg1( session->createTextMessage("test 1") );
    producer->send( msg1.get() );
    std::auto_ptr<TextMessage> msg2( session->createTextMessage("test 2") );
    producer->send( msg2.get() );

    producer->close();

    std::auto_ptr<MessageConsumer> consumer( session->createConsumer( queue.get() ) );

    // Read the first message
    std::auto_ptr<Message> recvMsg1( consumer->receive( 2000 ) );
    CPPUNIT_ASSERT( recvMsg1.get() != NULL );
    CPPUNIT_ASSERT( msg1->getText() == dynamic_cast<TextMessage*>( recvMsg1.get() )->getText() );

    // Read the second message
    std::auto_ptr<Message> recvMsg2( consumer->receive( 2000 ) );
    CPPUNIT_ASSERT( recvMsg2.get() != NULL );
    CPPUNIT_ASSERT( msg2->getText() == dynamic_cast<TextMessage*>( recvMsg2.get() )->getText() );

    // Acknowledge first message
    recvMsg1->acknowledge();

    consumer->close();

    // Read first message a second time
    consumer.reset( session->createConsumer( queue.get() ) );
    std::auto_ptr<Message> recvMsg3( consumer->receive( 2000 ) );
    CPPUNIT_ASSERT( recvMsg3.get() != NULL );
    CPPUNIT_ASSERT( msg2->getText() == dynamic_cast<TextMessage*>( recvMsg3.get() )->getText() );

    // Try to read second message a second time
    std::auto_ptr<Message> recvMsg4( consumer->receive( 2000 ) );
    CPPUNIT_ASSERT( recvMsg4.get() == NULL );

    consumer->close();
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireIndividualAckTest::testManyMessageAckedAfterMessageConsumption()
{
    int messageCount = 20;
    std::auto_ptr<Message> msg;

    Connection* connection = this->cmsProvider->getConnection();
    connection->start();

    std::auto_ptr<Session> session( connection->createSession( cms::Session::INDIVIDUAL_ACKNOWLEDGE ) );
    std::auto_ptr<Destination> queue( session->createTemporaryQueue() );
    std::auto_ptr<MessageProducer> producer( session->createProducer( queue.get() ) );

    for( int i = 0; i < messageCount; i++ )
    {
        msg.reset( session->createTextMessage("msg") );
        producer->send( msg.get() );
    }

    // Consume the message...
    std::auto_ptr<MessageConsumer> consumer( session->createConsumer( queue.get() ) );

    for( int i = 0; i < messageCount; i++ )
    {
        msg.reset( consumer->receive( 1000 ) );
        CPPUNIT_ASSERT( msg.get() != NULL );
        msg->acknowledge();
    }

    msg.reset( consumer->receive( 1000 ) );
    CPPUNIT_ASSERT( msg.get() == NULL );

    // Reset the session.
    session->close();
    session.reset( connection->createSession( cms::Session::AUTO_ACKNOWLEDGE ) );

    // Attempt to Consume the message...
    consumer.reset( session->createConsumer( queue.get() ) );
    msg.reset( consumer->receive( 1000 ) );
    CPPUNIT_ASSERT( msg.get() == NULL );
    session->close();
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireIndividualAckTest::testManyMessageAckedAfterAllConsumption()
{
    int messageCount = 20;
    std::auto_ptr<Message> msg;

    Connection* connection = this->cmsProvider->getConnection();
    connection->start();

    std::auto_ptr<Session> session( connection->createSession( cms::Session::INDIVIDUAL_ACKNOWLEDGE ) );
    std::auto_ptr<Destination> queue( session->createTemporaryQueue() );
    std::auto_ptr<MessageProducer> producer( session->createProducer( queue.get() ) );

    for( int i = 0; i < messageCount; i++ )
    {
        msg.reset( session->createTextMessage("msg") );
        producer->send( msg.get() );
    }

    // Consume the message...
    std::auto_ptr<MessageConsumer> consumer( session->createConsumer( queue.get() ) );

    std::vector<Message*> consumedMessages;

    for( int i = 0; i < messageCount; i++ )
    {
        Message* message = consumer->receive( 1000 );
        CPPUNIT_ASSERT( message != NULL );
        consumedMessages.push_back( message );
    }

    for( int i = 0; i < messageCount; i++ )
    {
        consumedMessages[i]->acknowledge();
        delete consumedMessages[i];
    }
    msg.reset( consumer->receive( 1000 ) );
    CPPUNIT_ASSERT( msg.get() == NULL );

    // Reset the session.
    session->close();
    session.reset( connection->createSession( cms::Session::AUTO_ACKNOWLEDGE ) );

    // Attempt to Consume the message...
    consumer.reset( session->createConsumer( queue.get() ) );
    msg.reset( consumer->receive( 1000 ) );
    CPPUNIT_ASSERT( msg.get() == NULL );
    session->close();
}
