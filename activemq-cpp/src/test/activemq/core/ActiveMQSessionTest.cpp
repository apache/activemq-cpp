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

#include "ActiveMQSessionTest.h"

#include <cms/ExceptionListener.h>
#include <activemq/transport/mock/MockTransportFactory.h>
#include <activemq/transport/TransportRegistry.h>
#include <activemq/commands/ActiveMQTextMessage.h>
#include <activemq/commands/ConsumerId.h>
#include <activemq/commands/MessageDispatch.h>
#include <activemq/core/ActiveMQConnectionFactory.h>
#include <activemq/core/ActiveMQSession.h>
#include <activemq/core/ActiveMQConsumer.h>
#include <activemq/core/ActiveMQProducer.h>
#include <decaf/util/Properties.h>
#include <decaf/lang/System.h>
#include <decaf/lang/Pointer.h>
#include <decaf/lang/Thread.h>
#include <decaf/net/Socket.h>
#include <decaf/net/ServerSocket.h>

using namespace std;
using namespace activemq;
using namespace activemq::core;
using namespace activemq::commands;
using namespace decaf;
using namespace decaf::lang;

////////////////////////////////////////////////////////////////////////////////
namespace activemq {
namespace core {

    class MyCMSMessageListener : public cms::MessageListener {
    public:

        std::vector< Pointer<cms::Message> > messages;
        decaf::util::concurrent::Mutex mutex;
        bool ack;

    public:

        MyCMSMessageListener(bool ack = false) : messages(), mutex(), ack(ack) {
        }

        virtual ~MyCMSMessageListener() {
            clear();
        }

        virtual void setAck( bool ack ){
            this->ack = ack;
        }

        virtual void clear() {
            messages.clear();
        }

        virtual void onMessage( const cms::Message* message ) {

            synchronized( &mutex ) {
                if( ack ){
                    message->acknowledge();
                }

                messages.push_back( Pointer<cms::Message>( message->clone() ) );
                mutex.notifyAll();
            }
        }

        void asyncWaitForMessages( unsigned int count ) {

            try {

                synchronized( &mutex ) {
                    int stopAtZero = count + 5;

                    while( messages.size() < count ) {
                        mutex.wait( 750 );

                        if( --stopAtZero == 0 ) {
                            break;
                        }
                    }
                }
            }
            AMQ_CATCH_RETHROW( activemq::exceptions::ActiveMQException )
            AMQ_CATCHALL_THROW( activemq::exceptions::ActiveMQException )
        }
    };
}}

////////////////////////////////////////////////////////////////////////////////
ActiveMQSessionTest::ActiveMQSessionTest() : connection(), dTransport(), exListener() {
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQSessionTest::~ActiveMQSessionTest() {
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionTest::testCreateManyConsumersAndSetListeners() {

    MyCMSMessageListener msgListener1;

    CPPUNIT_ASSERT( connection.get() != NULL );
    CPPUNIT_ASSERT( connection->isStarted() == true );

    // Create an Auto Ack Session
    std::auto_ptr<cms::Session> session( connection->createSession() );

    // Create a Topic
    std::auto_ptr<cms::Topic> topic1( session->createTopic( "TestTopic1" ) );

    CPPUNIT_ASSERT( topic1.get() != NULL );

    std::list<cms::MessageConsumer*> consumers;
    for( int ix = 0; ix < 100; ++ix ) {
        cms::MessageConsumer* consumer = session->createConsumer( topic1.get() );
        consumer->setMessageListener( &msgListener1 );
        consumers.push_back( consumer );
    }

    std::list<cms::MessageConsumer*>::iterator iter = consumers.begin();
    for( ; iter != consumers.end(); ++iter ) {
        (*iter)->close();
        delete *iter;
    }
    consumers.clear();
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionTest::testAutoAcking() {

    MyCMSMessageListener msgListener1;
    MyCMSMessageListener msgListener2;

    CPPUNIT_ASSERT( connection.get() != NULL );

    // Create an Auto Ack Session
    std::auto_ptr<cms::Session> session( connection->createSession() );

    // Create a Topic
    std::auto_ptr<cms::Topic> topic1( session->createTopic( "TestTopic1" ) );
    std::auto_ptr<cms::Topic> topic2( session->createTopic( "TestTopic2" ) );

    CPPUNIT_ASSERT( topic1.get() != NULL );
    CPPUNIT_ASSERT( topic2.get() != NULL );

    // Create a consumer
    std::auto_ptr<ActiveMQConsumer> consumer1(
        dynamic_cast<ActiveMQConsumer*>( session->createConsumer( topic1.get() ) ) );
    std::auto_ptr<ActiveMQConsumer> consumer2(
        dynamic_cast<ActiveMQConsumer*>( session->createConsumer( topic2.get() ) ) );

    CPPUNIT_ASSERT( consumer1.get() != NULL );
    CPPUNIT_ASSERT( consumer2.get() != NULL );

    CPPUNIT_ASSERT( consumer1->getMessageSelector() == "" );
    CPPUNIT_ASSERT( consumer2->getMessageSelector() == "" );

    CPPUNIT_ASSERT( consumer1->receiveNoWait() == NULL );
    CPPUNIT_ASSERT( consumer1->receive( 5 ) == NULL );
    CPPUNIT_ASSERT( consumer2->receiveNoWait() == NULL );
    CPPUNIT_ASSERT( consumer2->receive( 5 ) == NULL );

    consumer1->setMessageListener( &msgListener1 );
    consumer2->setMessageListener( &msgListener2 );

    injectTextMessage( "This is a Test 1" , *topic1, *( consumer1->getConsumerId() ) );

    msgListener1.asyncWaitForMessages( 1 );

    CPPUNIT_ASSERT( msgListener1.messages.size() == 1 );

    injectTextMessage( "This is a Test 2" , *topic2, *( consumer2->getConsumerId() ) );

    msgListener2.asyncWaitForMessages( 1 );

    CPPUNIT_ASSERT( msgListener2.messages.size() == 1 );

    Pointer<cms::TextMessage> msg1 = msgListener1.messages[0].dynamicCast<cms::TextMessage>();
    Pointer<cms::TextMessage> msg2 = msgListener2.messages[0].dynamicCast<cms::TextMessage>();

    std::string text1 = msg1->getText();
    std::string text2 = msg2->getText();

    CPPUNIT_ASSERT( text1 == "This is a Test 1" );
    CPPUNIT_ASSERT( text2 == "This is a Test 2" );
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionTest::testClientAck() {

    MyCMSMessageListener msgListener1( true );
    MyCMSMessageListener msgListener2( true );

    CPPUNIT_ASSERT( connection.get() != NULL );

    // Create an Client Ack Session
    std::auto_ptr<cms::Session> session(
        connection->createSession( cms::Session::CLIENT_ACKNOWLEDGE ) );

    // Create a Topic
    std::auto_ptr<cms::Topic> topic1( session->createTopic( "TestTopic1" ) );
    std::auto_ptr<cms::Topic> topic2( session->createTopic( "TestTopic2" ) );

    CPPUNIT_ASSERT( topic1.get() != NULL );
    CPPUNIT_ASSERT( topic2.get() != NULL );

    // Create a consumer
    std::auto_ptr<ActiveMQConsumer> consumer1(
        dynamic_cast<ActiveMQConsumer*>( session->createConsumer( topic1.get() ) ) );
    std::auto_ptr<ActiveMQConsumer> consumer2(
        dynamic_cast<ActiveMQConsumer*>( session->createConsumer( topic2.get() ) ) );

    CPPUNIT_ASSERT( consumer1.get() != NULL );
    CPPUNIT_ASSERT( consumer2.get() != NULL );

    CPPUNIT_ASSERT( consumer1->getMessageSelector() == "" );
    CPPUNIT_ASSERT( consumer2->getMessageSelector() == "" );

    CPPUNIT_ASSERT( consumer1->receiveNoWait() == NULL );
    CPPUNIT_ASSERT( consumer1->receive( 5 ) == NULL );
    CPPUNIT_ASSERT( consumer2->receiveNoWait() == NULL );
    CPPUNIT_ASSERT( consumer2->receive( 5 ) == NULL );

    consumer1->setMessageListener( &msgListener1 );
    consumer2->setMessageListener( &msgListener2 );

    injectTextMessage( "This is a Test 1" , *topic1, *( consumer1->getConsumerId() ) );

    msgListener1.asyncWaitForMessages( 1 );

    CPPUNIT_ASSERT_EQUAL( 1, (int)msgListener1.messages.size() );

    msgListener1.messages[0]->acknowledge();

    injectTextMessage( "This is a Test 2" , *topic2, *( consumer2->getConsumerId() ) );

    msgListener2.asyncWaitForMessages( 1 );

    CPPUNIT_ASSERT_EQUAL( 1, (int)msgListener2.messages.size() );

    msgListener2.messages[0]->acknowledge();

    Pointer<cms::TextMessage> msg1 = msgListener1.messages[0].dynamicCast<cms::TextMessage>();
    Pointer<cms::TextMessage> msg2 = msgListener2.messages[0].dynamicCast<cms::TextMessage>();

    std::string text1 = msg1->getText();
    std::string text2 = msg2->getText();

    CPPUNIT_ASSERT( text1 == "This is a Test 1" );
    CPPUNIT_ASSERT( text2 == "This is a Test 2" );
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionTest::testTransactionCommitOneConsumer() {

    static const int MSG_COUNT = 50;

    MyCMSMessageListener msgListener1;

    CPPUNIT_ASSERT( connection.get() != NULL );

    // Create an Transacted Session
    std::auto_ptr<cms::Session> session(
        connection->createSession( cms::Session::SESSION_TRANSACTED ) );

    // Create a Topic
    std::auto_ptr<cms::Topic> topic1( session->createTopic( "TestTopic1" ) );

    CPPUNIT_ASSERT( topic1.get() != NULL );

    // Create a consumer
    std::auto_ptr<ActiveMQConsumer> consumer1(
        dynamic_cast<ActiveMQConsumer*>( session->createConsumer( topic1.get() ) ) );

    CPPUNIT_ASSERT( consumer1.get() != NULL );

    CPPUNIT_ASSERT( consumer1->getMessageSelector() == "" );

    CPPUNIT_ASSERT( consumer1->receiveNoWait() == NULL );
    CPPUNIT_ASSERT( consumer1->receive( 5 ) == NULL );

    consumer1->setMessageListener( &msgListener1 );

    for( int i = 0; i < MSG_COUNT; ++i ) {
        injectTextMessage( "This is a Test 1" , *topic1, *( consumer1->getConsumerId() ) );
    }

    msgListener1.asyncWaitForMessages( MSG_COUNT );

    CPPUNIT_ASSERT_EQUAL( MSG_COUNT, (int)msgListener1.messages.size() );

    session->commit();

    Pointer<cms::TextMessage> msg1 = msgListener1.messages[0].dynamicCast<cms::TextMessage>();

    std::string text1 = msg1->getText();

    CPPUNIT_ASSERT( text1 == "This is a Test 1" );

    msgListener1.clear();
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionTest::testTransactionCommitTwoConsumer() {

    MyCMSMessageListener msgListener1;
    MyCMSMessageListener msgListener2;

    CPPUNIT_ASSERT( connection.get() != NULL );

    // Create an Auto Ack Session
    std::auto_ptr<cms::Session> session(
        connection->createSession( cms::Session::SESSION_TRANSACTED ) );

    // Create a Topic
    std::auto_ptr<cms::Topic> topic1( session->createTopic( "TestTopic1" ) );
    std::auto_ptr<cms::Topic> topic2( session->createTopic( "TestTopic2" ) );

    CPPUNIT_ASSERT( topic1.get() != NULL );
    CPPUNIT_ASSERT( topic2.get() != NULL );

    // Create a consumer
    std::auto_ptr<ActiveMQConsumer> consumer1(
        dynamic_cast<ActiveMQConsumer*>( session->createConsumer( topic1.get() ) ) );
    std::auto_ptr<ActiveMQConsumer> consumer2(
        dynamic_cast<ActiveMQConsumer*>( session->createConsumer( topic2.get() ) ) );

    CPPUNIT_ASSERT( consumer1.get() != NULL );
    CPPUNIT_ASSERT( consumer2.get() != NULL );

    CPPUNIT_ASSERT( consumer1->getMessageSelector() == "" );
    CPPUNIT_ASSERT( consumer2->getMessageSelector() == "" );

    CPPUNIT_ASSERT( consumer1->receiveNoWait() == NULL );
    CPPUNIT_ASSERT( consumer1->receive( 5 ) == NULL );
    CPPUNIT_ASSERT( consumer2->receiveNoWait() == NULL );
    CPPUNIT_ASSERT( consumer2->receive( 5 ) == NULL );

    consumer1->setMessageListener( &msgListener1 );
    consumer2->setMessageListener( &msgListener2 );

    injectTextMessage( "This is a Test 1" , *topic1, *( consumer1->getConsumerId() ) );

    msgListener1.asyncWaitForMessages( 1 );

    CPPUNIT_ASSERT_EQUAL( 1, (int)msgListener1.messages.size() );

    injectTextMessage( "This is a Test 2" , *topic2, *( consumer2->getConsumerId() ) );

    msgListener2.asyncWaitForMessages( 1 );

    CPPUNIT_ASSERT( msgListener2.messages.size() == 1 );

    session->commit();

    Pointer<cms::TextMessage> msg1 = msgListener1.messages[0].dynamicCast<cms::TextMessage>();
    Pointer<cms::TextMessage> msg2 = msgListener2.messages[0].dynamicCast<cms::TextMessage>();

    std::string text1 = msg1->getText();
    std::string text2 = msg2->getText();

    CPPUNIT_ASSERT( text1 == "This is a Test 1" );
    CPPUNIT_ASSERT( text2 == "This is a Test 2" );

    msgListener1.clear();
    msgListener2.clear();
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionTest::testTransactionRollbackOneConsumer() {

    MyCMSMessageListener msgListener1;

    CPPUNIT_ASSERT( connection.get() != NULL );

    std::auto_ptr<cms::Session> session(
        connection->createSession( cms::Session::SESSION_TRANSACTED ) );

    // Create a Topic
    std::auto_ptr<cms::Topic> topic1( session->createTopic( "TestTopic1" ) );

    CPPUNIT_ASSERT( topic1.get() != NULL );

    // Create a consumer
    std::auto_ptr<ActiveMQConsumer> consumer1(
        dynamic_cast<ActiveMQConsumer*>( session->createConsumer( topic1.get() ) ) );

    CPPUNIT_ASSERT( consumer1.get() != NULL );

    CPPUNIT_ASSERT( consumer1->getMessageSelector() == "" );

    CPPUNIT_ASSERT( consumer1->receiveNoWait() == NULL );
    CPPUNIT_ASSERT( consumer1->receive( 5 ) == NULL );

    consumer1->setMessageListener( &msgListener1 );

    const unsigned int msgCount = 50;

    for( unsigned int i = 0; i < msgCount; ++i ) {
        std::ostringstream stream;
        stream << "This is test message #" << i << std::ends;
        injectTextMessage( stream.str() , *topic1, *( consumer1->getConsumerId() ) );
    }

    msgListener1.asyncWaitForMessages( msgCount );

    CPPUNIT_ASSERT( msgListener1.messages.size() == msgCount );

    msgListener1.clear();

    session->rollback();

    msgListener1.asyncWaitForMessages( msgCount );

    CPPUNIT_ASSERT( msgListener1.messages.size() == msgCount );

    session->commit();
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionTest::testTransactionRollbackTwoConsumer() {

    MyCMSMessageListener msgListener1;
    MyCMSMessageListener msgListener2;

    CPPUNIT_ASSERT( connection.get() != NULL );

    std::auto_ptr<cms::Session> session(
        connection->createSession( cms::Session::SESSION_TRANSACTED ) );

    // Create a Topic
    std::auto_ptr<cms::Topic> topic1( session->createTopic( "TestTopic1" ) );
    std::auto_ptr<cms::Topic> topic2( session->createTopic( "TestTopic2" ) );

    CPPUNIT_ASSERT( topic1.get() != NULL );
    CPPUNIT_ASSERT( topic2.get() != NULL );

    // Create a consumer
    std::auto_ptr<ActiveMQConsumer> consumer1(
        dynamic_cast<ActiveMQConsumer*>( session->createConsumer( topic1.get() ) ) );
    std::auto_ptr<ActiveMQConsumer> consumer2(
        dynamic_cast<ActiveMQConsumer*>( session->createConsumer( topic2.get() ) ) );

    CPPUNIT_ASSERT( consumer1.get() != NULL );
    CPPUNIT_ASSERT( consumer2.get() != NULL );

    CPPUNIT_ASSERT( consumer1->getMessageSelector() == "" );
    CPPUNIT_ASSERT( consumer2->getMessageSelector() == "" );

    CPPUNIT_ASSERT( consumer1->receiveNoWait() == NULL );
    CPPUNIT_ASSERT( consumer1->receive( 5 ) == NULL );
    CPPUNIT_ASSERT( consumer2->receiveNoWait() == NULL );
    CPPUNIT_ASSERT( consumer2->receive( 5 ) == NULL );

    consumer1->setMessageListener( &msgListener1 );
    consumer2->setMessageListener( &msgListener2 );

    const unsigned int msgCount = 50;

    for( unsigned int i = 0; i < msgCount; ++i ) {
        std::ostringstream stream;
        stream << "This is test message #" << i << std::ends;
        injectTextMessage( stream.str() , *topic1, *( consumer1->getConsumerId() ) );
    }

    for( unsigned int i = 0; i < msgCount; ++i ) {
        std::ostringstream stream;
        stream << "This is test message #" << i << std::ends;
        injectTextMessage( stream.str() , *topic2, *( consumer2->getConsumerId() ) );
    }

    msgListener1.asyncWaitForMessages( msgCount );

    CPPUNIT_ASSERT( msgListener1.messages.size() == msgCount );

    msgListener2.asyncWaitForMessages( msgCount );

    CPPUNIT_ASSERT( msgListener2.messages.size() == msgCount );

    msgListener1.clear();
    msgListener2.clear();

    session->rollback();

    msgListener1.asyncWaitForMessages( msgCount );

    CPPUNIT_ASSERT( msgListener1.messages.size() == msgCount );

    msgListener2.asyncWaitForMessages( msgCount );

    CPPUNIT_ASSERT( msgListener2.messages.size() == msgCount );

    session->commit();
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionTest::testTransactionCloseWithoutCommit() {

    static const int MSG_COUNT = 50;

    MyCMSMessageListener msgListener1;

    CPPUNIT_ASSERT(connection.get() != NULL);

    // Create an Transacted Session
    std::auto_ptr<cms::Session> session(connection->createSession(cms::Session::SESSION_TRANSACTED));

    // Create a Topic
    std::auto_ptr<cms::Topic> topic1(session->createTopic("TestTopic1"));

    CPPUNIT_ASSERT(topic1.get() != NULL );

    // Create a consumer
    std::auto_ptr<ActiveMQConsumer> consumer1(
        dynamic_cast<ActiveMQConsumer*> (session->createConsumer(topic1.get())));

    CPPUNIT_ASSERT(consumer1.get() != NULL);

    CPPUNIT_ASSERT(consumer1->getMessageSelector() == "");

    CPPUNIT_ASSERT(consumer1->receiveNoWait() == NULL);
    CPPUNIT_ASSERT(consumer1->receive( 5 ) == NULL);

    consumer1->setMessageListener(&msgListener1);

    for (int i = 0; i < MSG_COUNT; ++i) {
        injectTextMessage("This is a Test 1", *topic1, *(consumer1->getConsumerId()));
    }

    msgListener1.asyncWaitForMessages(MSG_COUNT);

    CPPUNIT_ASSERT_EQUAL(MSG_COUNT, (int)msgListener1.messages.size());

    // This is what we are testing, since there was no commit, the session
    // will rollback the transaction when this is closed.
    // session->commit();

    consumer1->close();
    session->close();
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionTest::testExpiration() {

    MyCMSMessageListener msgListener1;
    MyCMSMessageListener msgListener2;

    CPPUNIT_ASSERT( connection.get() != NULL );

    // Create an Auto Ack Session
    std::auto_ptr<cms::Session> session( connection->createSession() );

    // Create a Topic
    std::auto_ptr<cms::Topic> topic1( session->createTopic( "TestTopic1" ) );
    std::auto_ptr<cms::Topic> topic2( session->createTopic( "TestTopic2" ) );

    CPPUNIT_ASSERT( topic1.get() != NULL );
    CPPUNIT_ASSERT( topic2.get() != NULL );

    // Create a consumer
    std::auto_ptr<ActiveMQConsumer> consumer1(
        dynamic_cast<ActiveMQConsumer*>( session->createConsumer( topic1.get() ) ) );
    std::auto_ptr<ActiveMQConsumer> consumer2(
        dynamic_cast<ActiveMQConsumer*>( session->createConsumer( topic2.get() ) ) );

    CPPUNIT_ASSERT( consumer1.get() != NULL );
    CPPUNIT_ASSERT( consumer2.get() != NULL );

    consumer1->setMessageListener( &msgListener1 );
    consumer2->setMessageListener( &msgListener2 );

    injectTextMessage( "This is a Test 1" ,
                       *topic1,
                       *( consumer1->getConsumerId() ),
                       decaf::lang::System::currentTimeMillis(),
                       50 );

    msgListener1.asyncWaitForMessages( 1 );

    CPPUNIT_ASSERT( msgListener1.messages.size() == 1 );

    injectTextMessage( "This is a Test 2" ,
                       *topic2,
                       *( consumer2->getConsumerId() ),
                       decaf::lang::System::currentTimeMillis() - 100,
                       1 );

    msgListener2.asyncWaitForMessages( 1 );

    CPPUNIT_ASSERT( msgListener2.messages.size() == 0 );

    Pointer<cms::TextMessage> msg1 = msgListener1.messages[0].dynamicCast<cms::TextMessage>();

    std::string text1 = msg1->getText();

    CPPUNIT_ASSERT( text1 == "This is a Test 1" );
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionTest::testTransactionCommitAfterConsumerClosed() {

    static const int MSG_COUNT = 50;
    MyCMSMessageListener msgListener1;

    CPPUNIT_ASSERT(connection.get() != NULL);

    // Create an Transacted Session
    std::auto_ptr<cms::Session> session(connection->createSession(cms::Session::SESSION_TRANSACTED));

    // Create a Topic
    std::auto_ptr<cms::Topic> topic1(session->createTopic("TestTopic1"));

    CPPUNIT_ASSERT(topic1.get() != NULL);

    // Create a consumer
    std::auto_ptr<ActiveMQConsumer> consumer1(
        dynamic_cast<ActiveMQConsumer*>( session->createConsumer( topic1.get() ) ) );

    CPPUNIT_ASSERT(consumer1.get() != NULL);
    CPPUNIT_ASSERT(consumer1->getMessageSelector() == "");
    CPPUNIT_ASSERT(consumer1->receiveNoWait() == NULL);
    CPPUNIT_ASSERT(consumer1->receive(5) == NULL);

    consumer1->setMessageListener(&msgListener1);

    for (int i = 0; i < MSG_COUNT; ++i) {
        injectTextMessage("This is a Test 1", *topic1, *(consumer1->getConsumerId()));
    }

    msgListener1.asyncWaitForMessages(MSG_COUNT);
    CPPUNIT_ASSERT_EQUAL(MSG_COUNT, (int)msgListener1.messages.size());

    consumer1->close();
    consumer1.reset();
    session->commit();

    Pointer<cms::TextMessage> msg1 = msgListener1.messages[0].dynamicCast<cms::TextMessage>();
    std::string text1 = msg1->getText();

    CPPUNIT_ASSERT(text1 == "This is a Test 1");
    msgListener1.clear();
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionTest::testCreateTempQueueByName() {

    MyCMSMessageListener msgListener1;

    CPPUNIT_ASSERT(connection.get() != NULL);
    CPPUNIT_ASSERT(connection->isStarted() == true);

    // Create an Auto Ack Session
    std::auto_ptr<cms::Session> session(connection->createSession());

    // Create a Topic
    std::auto_ptr<cms::Queue> queue(session->createQueue("ID:TestQueue"));

    CPPUNIT_ASSERT(queue.get() != NULL);
    CPPUNIT_ASSERT(queue->getDestinationType() == cms::Destination::TEMPORARY_QUEUE);
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionTest::testCreateTempTopicByName() {

    MyCMSMessageListener msgListener1;

    CPPUNIT_ASSERT(connection.get() != NULL);
    CPPUNIT_ASSERT(connection->isStarted() == true);

    // Create an Auto Ack Session
    std::auto_ptr<cms::Session> session(connection->createSession());

    // Create a Topic
    std::auto_ptr<cms::Topic> topic(session->createTopic("ID:TestTopic"));

    CPPUNIT_ASSERT(topic.get() != NULL);
    CPPUNIT_ASSERT(topic->getDestinationType() == cms::Destination::TEMPORARY_TOPIC);
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionTest::setUp() {

    try {
        ActiveMQConnectionFactory factory("mock://127.0.0.1:12345?wireFormat=openwire");

        connection.reset(dynamic_cast<ActiveMQConnection*>(factory.createConnection()));

        // Get a pointer to the Mock Transport for Message injection.
        dTransport = dynamic_cast<transport::mock::MockTransport*>(
            connection->getTransport().narrow(typeid(transport::mock::MockTransport)));
        CPPUNIT_ASSERT(dTransport != NULL);

        connection->setExceptionListener(&exListener);
        connection->start();
    } catch (...) {
        bool exceptionThrown = false;
        CPPUNIT_ASSERT(exceptionThrown);
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionTest::tearDown() {
    connection.reset(NULL);
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionTest::injectTextMessage(const std::string message,
                                            const cms::Destination& destination,
                                            const commands::ConsumerId& id,
                                            const long long timeStamp,
                                            const long long timeToLive) {

    Pointer<ActiveMQTextMessage> msg(new ActiveMQTextMessage());

    Pointer<ProducerId> producerId(new ProducerId());
    producerId->setConnectionId(id.getConnectionId());
    producerId->setSessionId(id.getSessionId());
    producerId->setValue(1);

    Pointer<MessageId> messageId(new MessageId());
    messageId->setProducerId(producerId);
    messageId->setProducerSequenceId(2);

    // Init Message
    msg->setText(message.c_str());
    msg->setCMSDestination(&destination);
    msg->setCMSMessageID("Id: 123456");
    msg->setMessageId(messageId);

    long long expiration = 0LL;

    if (timeStamp != 0) {
        msg->setCMSTimestamp(timeStamp);

        if (timeToLive > 0LL) {
            expiration = timeToLive + timeStamp;
        }
    }

    msg->setCMSExpiration(expiration);

    // Send the Message
    CPPUNIT_ASSERT(dTransport != NULL);

    Pointer<MessageDispatch> dispatch(new MessageDispatch());
    dispatch->setMessage(msg);
    dispatch->setConsumerId(Pointer<ConsumerId>(id.cloneDataStructure()));

    dTransport->fireCommand(dispatch);
}
