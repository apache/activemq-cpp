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

#include "OpenwireClientAckTest.h"

#include <activemq/exceptions/ActiveMQException.h>

#include <decaf/lang/Thread.h>

#include <cms/Session.h>
#include <cms/MessageListener.h>

#include <memory>

using namespace cms;
using namespace std;
using namespace decaf;
using namespace decaf::lang;
using namespace activemq;
using namespace activemq::test;
using namespace activemq::test::openwire;

////////////////////////////////////////////////////////////////////////////////
namespace {

    class MyMesageListener: public cms::MessageListener {
    private:

        bool dontAck;

    public:

        MyMesageListener( bool dontAck = false ) : MessageListener(), dontAck(dontAck) {}

        virtual ~MyMesageListener() {}

        virtual void onMessage(const Message* message) {

            CPPUNIT_ASSERT( message != NULL);

            if (!dontAck) {

                try {
                    message->acknowledge();
                } catch (Exception& e) {
                    e.printStackTrace();
                }
            }
        }

    };
}

////////////////////////////////////////////////////////////////////////////////
OpenwireClientAckTest::OpenwireClientAckTest() {
}

////////////////////////////////////////////////////////////////////////////////
OpenwireClientAckTest::~OpenwireClientAckTest() {
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireClientAckTest::testAckedMessageAreConsumed() {

    Connection* connection = this->cmsProvider->getConnection();
    connection->start();

    std::auto_ptr<Session> session( connection->createSession( Session::CLIENT_ACKNOWLEDGE ) );
    std::auto_ptr<Destination> queue( session->createTemporaryQueue() );
    std::auto_ptr<MessageProducer> producer( session->createProducer( queue.get() ) );

    std::auto_ptr<TextMessage> msg1( session->createTextMessage( "Hello" ) );
    producer->send( msg1.get() );

    // Consume the message...
    std::auto_ptr<MessageConsumer> consumer( session->createConsumer( queue.get() ) );
    std::auto_ptr<Message> msg( consumer->receive( 1000 ) );
    CPPUNIT_ASSERT( msg.get() != NULL );
    msg->acknowledge();

    // Reset the session->
    session->close();
    session.reset( connection->createSession( Session::CLIENT_ACKNOWLEDGE ) );

    // Attempt to Consume the message...
    consumer.reset( session->createConsumer( queue.get() ) );
    msg.reset( consumer->receive( 1000 ) );
    CPPUNIT_ASSERT( msg.get() == NULL );

    session->close();
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireClientAckTest::testLastMessageAcked() {

    Connection* connection = this->cmsProvider->getConnection();
    connection->start();

    std::auto_ptr<Session> session( connection->createSession( Session::CLIENT_ACKNOWLEDGE ) );
    std::auto_ptr<Destination> queue( session->createTemporaryQueue() );
    std::auto_ptr<MessageProducer> producer( session->createProducer( queue.get() ) );

    std::auto_ptr<TextMessage> msg1( session->createTextMessage( "Hello1" ) );
    std::auto_ptr<TextMessage> msg2( session->createTextMessage( "Hello2" ) );
    std::auto_ptr<TextMessage> msg3( session->createTextMessage( "Hello3" ) );
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
    session.reset( connection->createSession( Session::CLIENT_ACKNOWLEDGE ) );

    // Attempt to Consume the message...
    consumer.reset( session->createConsumer( queue.get() ) );
    msg.reset( consumer->receive( 1000 ) );
    CPPUNIT_ASSERT( msg.get() == NULL );

    session->close();
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireClientAckTest::testUnAckedMessageAreNotConsumedOnSessionClose() {

    Connection* connection = this->cmsProvider->getConnection();
    connection->start();

    std::auto_ptr<Session> session( connection->createSession( Session::CLIENT_ACKNOWLEDGE ) );
    std::auto_ptr<Destination> queue( session->createTemporaryQueue() );
    std::auto_ptr<MessageProducer> producer( session->createProducer( queue.get() ) );

    std::auto_ptr<TextMessage> msg1( session->createTextMessage( "Hello" ) );
    producer->send( msg1.get() );

    // Consume the message...
    std::auto_ptr<MessageConsumer> consumer( session->createConsumer( queue.get() ) );
    std::auto_ptr<Message> msg( consumer->receive( 1000 ) );
    CPPUNIT_ASSERT( msg.get() != NULL );
    // Don't ack the message.

    // Reset the session->  This should cause the unacknowledged message to be re-delivered.
    session->close();
    session.reset( connection->createSession( Session::CLIENT_ACKNOWLEDGE ) );

    // Attempt to Consume the message...
    consumer.reset( session->createConsumer( queue.get() ) );
    msg.reset( consumer->receive( 1000 ) );
    CPPUNIT_ASSERT( msg.get() != NULL );
    msg->acknowledge();

    session->close();
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireClientAckTest::testAckedMessageAreConsumedAsync() {

    Connection* connection = this->cmsProvider->getConnection();
    connection->start();

    MyMesageListener listener( false );

    std::auto_ptr<Session> session( connection->createSession( Session::CLIENT_ACKNOWLEDGE ) );
    std::auto_ptr<Destination> queue( session->createTemporaryQueue() );
    std::auto_ptr<MessageProducer> producer( session->createProducer( queue.get() ) );

    std::auto_ptr<TextMessage> msg1( session->createTextMessage( "Hello" ) );
    producer->send( msg1.get() );

    // Consume the message...
    std::auto_ptr<MessageConsumer> consumer( session->createConsumer( queue.get() ) );
    consumer->setMessageListener( &listener );

    Thread::sleep( 5000 );

    // Reset the session->
    session->close();

    session.reset( connection->createSession( Session::CLIENT_ACKNOWLEDGE ) );

    // Attempt to Consume the message...
    consumer.reset( session->createConsumer( queue.get() ) );
    std::auto_ptr<Message> msg( consumer->receive( 1000 ) );
    CPPUNIT_ASSERT( msg.get() == NULL );

    session->close();
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireClientAckTest::testUnAckedMessageAreNotConsumedOnSessionCloseAsync() {

    Connection* connection = this->cmsProvider->getConnection();
    connection->start();

    // Don't send an ack
    MyMesageListener listener( true );

    std::auto_ptr<Session> session( connection->createSession( Session::CLIENT_ACKNOWLEDGE ) );
    std::auto_ptr<Destination> queue( session->createTemporaryQueue() );
    std::auto_ptr<MessageProducer> producer( session->createProducer( queue.get() ) );

    std::auto_ptr<TextMessage> msg1( session->createTextMessage( "Hello" ) );
    producer->send( msg1.get() );

    // Consume the message...
    std::auto_ptr<MessageConsumer> consumer( session->createConsumer( queue.get() ) );
    consumer->setMessageListener( &listener );
    // Don't ack the message.

    // Reset the session-> This should cause the Unacked message to be redelivered.
    session->close();

    Thread::sleep( 5000 );
    session.reset( connection->createSession( Session::CLIENT_ACKNOWLEDGE ) );

    // Attempt to Consume the message...
    consumer.reset( session->createConsumer( queue.get() ) );
    std::auto_ptr<Message> msg( consumer->receive( 2000 ) );
    CPPUNIT_ASSERT( msg.get() != NULL );
    msg->acknowledge();

    session->close();
}

