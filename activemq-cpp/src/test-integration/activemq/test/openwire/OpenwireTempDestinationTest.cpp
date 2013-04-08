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

#include "OpenwireTempDestinationTest.h"

#include <decaf/lang/Thread.h>
#include <decaf/util/concurrent/Mutex.h>
#include <decaf/util/concurrent/CountDownLatch.h>
#include <decaf/util/UUID.h>
#include <decaf/util/ArrayList.h>
#include <activemq/exceptions/ActiveMQException.h>
#include <activemq/core/ActiveMQConnectionFactory.h>
#include <decaf/util/ArrayList.h>

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
using namespace decaf::util::concurrent;
using namespace decaf::lang;

namespace activemq {
namespace test {
namespace openwire {

    class Requester : public cms::MessageListener,
                      public decaf::lang::Runnable {
    private:

        auto_ptr<CMSProvider> cmsProvider;
        auto_ptr<cms::MessageConsumer> tempTopicConsumer;

        unsigned int numReceived;
        unsigned int messageCount;

        decaf::util::concurrent::CountDownLatch ready;
        decaf::util::concurrent::CountDownLatch responses;

    public:

        Requester(const std::string& url, const std::string& destination, unsigned int messageCount ) :
            cmsProvider(), tempTopicConsumer(), numReceived(0), messageCount(messageCount), ready(1), responses(messageCount) {

            this->cmsProvider.reset( new CMSProvider( url ) );
            this->cmsProvider->setDestinationName( destination );

            this->cmsProvider->getProducer()->setDeliveryMode( DeliveryMode::NON_PERSISTENT );

            this->tempTopicConsumer.reset(
                cmsProvider->getSession()->createConsumer(
                    cmsProvider->getTempDestination() ) );
            this->tempTopicConsumer->setMessageListener( this );
        }

        virtual ~Requester() {}

        virtual unsigned int getNumReceived() const {
            return this->numReceived;
        }

        virtual void waitUnitReady() {
            this->ready.await();
        }

        virtual void awaitAllResponses() {
            this->responses.await( 2000 * this->messageCount );
        }

        virtual void run() {

            try {

                auto_ptr<cms::TextMessage> message(
                    this->cmsProvider->getSession()->createTextMessage() );
                message->setCMSReplyTo( this->cmsProvider->getTempDestination() );

                this->ready.countDown();

                for( unsigned int i = 0; i < messageCount; ++i ) {
                    this->cmsProvider->getProducer()->send( message.get() );
                }

            } catch( CMSException& e ) {
                e.printStackTrace();
            }
        }

        virtual void onMessage( const cms::Message* message ) {

            try {

                this->numReceived++;
                this->responses.countDown();

            } catch( CMSException& e ) {
                e.printStackTrace();
            }
        }
    };

    class Responder : public cms::MessageListener {
    private:

        auto_ptr<CMSProvider> cmsProvider;

        unsigned int numReceived;
        unsigned int messageCount;

        decaf::util::concurrent::CountDownLatch requests;

    public:

        Responder(const std::string& url, const std::string& destination, unsigned int messageCount) :
            cmsProvider(), numReceived(0), messageCount(messageCount), requests(messageCount) {

            this->cmsProvider.reset(new CMSProvider(url));
            this->cmsProvider->setDestinationName(destination);
            this->cmsProvider->getNoDestProducer()->setDeliveryMode(DeliveryMode::NON_PERSISTENT);
            this->cmsProvider->getConsumer()->setMessageListener(this);
        }

        virtual ~Responder() {}

        virtual unsigned int getNumReceived() const {
            return this->numReceived;
        }

        virtual void awaitAllRequests() {
            this->requests.await( 2000 * this->messageCount );
        }

        virtual void onMessage( const cms::Message* message ) {

            try {

                if( message->getCMSReplyTo() != NULL ) {

                    auto_ptr<cms::Message> response(
                        cmsProvider->getSession()->createMessage() );

                    // Send it back to the replyTo Destination
                    this->cmsProvider->getNoDestProducer()->send(
                        message->getCMSReplyTo(), response.get() );

                    this->requests.countDown();
                }

                this->numReceived++;

            } catch( CMSException& e ) {
                e.printStackTrace();
            }
        }
    };

}}}

///////////////////////////////////////////////////////////////////////////////
void OpenwireTempDestinationTest::testBasics() {

    try{

        auto_ptr<cms::MessageConsumer> tempConsumer(
            cmsProvider->getSession()->createConsumer(
                cmsProvider->getTempDestination() ) );

        auto_ptr<TextMessage> message(
            cmsProvider->getSession()->createTextMessage() );

        // Fire a message to the temporary topic
        cmsProvider->getNoDestProducer()->send(
            cmsProvider->getTempDestination(), message.get() );

        auto_ptr<cms::Message> received( tempConsumer->receive( 3000 ) );

        CPPUNIT_ASSERT( received.get() != NULL );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

///////////////////////////////////////////////////////////////////////////////
void OpenwireTempDestinationTest::testTwoConnections() {

    std::string destination = "REQUEST-TOPIC";

    auto_ptr<Requester> requester(
        new Requester( cmsProvider->getBrokerURL(), destination, 10 ) );
    auto_ptr<Responder> responder(
        new Responder( cmsProvider->getBrokerURL(), destination, 10 ) );

    // Launch the Consumers in new Threads.
    Thread requestorThread( requester.get() );
    requestorThread.start();

    // Responder should get all its requests first
    responder->awaitAllRequests();

    // Now the Requester should get all its responses.
    requester->awaitAllResponses();

    // Check that the responder received all the required requests
    CPPUNIT_ASSERT( responder->getNumReceived() == 10 );

    // Check that the requester received all the required responses
    CPPUNIT_ASSERT( requester->getNumReceived() == 10 );

    // Shutdown the Requester.
    requestorThread.join();
}

///////////////////////////////////////////////////////////////////////////////
void OpenwireTempDestinationTest::testTempDestOnlyConsumedByLocalConn() {

    std::auto_ptr<ActiveMQConnectionFactory> factory(
        new ActiveMQConnectionFactory(cmsProvider->getBrokerURL()));
    factory->setAlwaysSyncSend(true);

    std::auto_ptr<Connection> tempConnection(factory->createConnection());
    tempConnection->start();
    std::auto_ptr<Session> tempSession(tempConnection->createSession());
    std::auto_ptr<TemporaryQueue> queue(tempSession->createTemporaryQueue());
    std::auto_ptr<MessageProducer> producer(tempSession->createProducer(queue.get()));
    producer->setDeliveryMode(DeliveryMode::NON_PERSISTENT);
    std::auto_ptr<TextMessage> message(tempSession->createTextMessage("First"));
    producer->send(message.get());

    // temp destination should not be consume when using another connection
    std::auto_ptr<Connection> otherConnection(factory->createConnection());
    std::auto_ptr<Session> otherSession(otherConnection->createSession());
    std::auto_ptr<TemporaryQueue> otherQueue(otherSession->createTemporaryQueue());
    std::auto_ptr<MessageConsumer> consumer(otherSession->createConsumer(otherQueue.get()));
    std::auto_ptr<Message> msg(consumer->receive(3000));
    CPPUNIT_ASSERT(msg.get() == NULL);

    // should throw InvalidDestinationException when consuming a temp
    // destination from another connection
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a CMS InvalidDestinationException",
        otherSession->createConsumer(queue.get()),
        InvalidDestinationException);

    // should be able to consume temp destination from the same connection
    consumer.reset(tempSession->createConsumer(queue.get()));
    msg.reset(consumer->receive(3000));
    CPPUNIT_ASSERT(msg.get() != NULL);
}

///////////////////////////////////////////////////////////////////////////////
void OpenwireTempDestinationTest::testTempQueueHoldsMessagesWithConsumers() {

    std::auto_ptr<TemporaryQueue> queue(cmsProvider->getSession()->createTemporaryQueue());
    std::auto_ptr<MessageConsumer> consumer(cmsProvider->getSession()->createConsumer(queue.get()));
    std::auto_ptr<MessageProducer> producer(cmsProvider->getSession()->createProducer(queue.get()));
    producer->setDeliveryMode(DeliveryMode::NON_PERSISTENT);
    std::auto_ptr<TextMessage> message(cmsProvider->getSession()->createTextMessage("Hello"));
    producer->send(message.get());

    std::auto_ptr<Message> message2(consumer->receive(3000));
    CPPUNIT_ASSERT(message2.get() != NULL);
    CPPUNIT_ASSERT_MESSAGE("Expected message to be a TextMessage", dynamic_cast<TextMessage*>(message2.get()) != NULL);
    CPPUNIT_ASSERT_MESSAGE(std::string("Expected message to be a '") + message->getText() + "'",
        dynamic_cast<TextMessage*>(message2.get())->getText() == message->getText());
}

///////////////////////////////////////////////////////////////////////////////
void OpenwireTempDestinationTest::testTempQueueHoldsMessagesWithoutConsumers() {

    std::auto_ptr<TemporaryQueue> queue(cmsProvider->getSession()->createTemporaryQueue());
    std::auto_ptr<MessageProducer> producer(cmsProvider->getSession()->createProducer(queue.get()));
    producer->setDeliveryMode(DeliveryMode::NON_PERSISTENT);
    std::auto_ptr<TextMessage> message(cmsProvider->getSession()->createTextMessage("Hello"));
    producer->send(message.get());

    std::auto_ptr<MessageConsumer> consumer(cmsProvider->getSession()->createConsumer(queue.get()));
    std::auto_ptr<Message> message2(consumer->receive(3000));
    CPPUNIT_ASSERT(message2.get() != NULL);
    CPPUNIT_ASSERT_MESSAGE("Expected message to be a TextMessage", dynamic_cast<TextMessage*>(message2.get()) != NULL);
    CPPUNIT_ASSERT_MESSAGE(std::string("Expected message to be a '") + message->getText() + "'",
        dynamic_cast<TextMessage*>(message2.get())->getText() == message->getText());
}

///////////////////////////////////////////////////////////////////////////////
void OpenwireTempDestinationTest::testTmpQueueWorksUnderLoad() {

    int count = 500;
    int dataSize = 1024;

    ArrayList<Pointer<BytesMessage> > list(count);
    std::auto_ptr<TemporaryQueue> queue(cmsProvider->getSession()->createTemporaryQueue());
    std::auto_ptr<MessageProducer> producer(cmsProvider->getSession()->createProducer(queue.get()));
    producer->setDeliveryMode(DeliveryMode::NON_PERSISTENT);

    unsigned char data[1024];
    for (int i = 0; i < dataSize; ++i) {
        data[i] = 255;
    }

    for (int i = 0; i < count; i++) {
        Pointer<BytesMessage> message(cmsProvider->getSession()->createBytesMessage());
        message->writeBytes(data, 0, dataSize);
        message->setIntProperty("c", i);
        producer->send(message.get());
        list.add(message);
    }

    std::auto_ptr<MessageConsumer> consumer(cmsProvider->getSession()->createConsumer(queue.get()));
    for (int i = 0; i < count; i++) {
        Pointer<Message> message2(consumer->receive(2000));
        CPPUNIT_ASSERT(message2 != NULL);
        CPPUNIT_ASSERT_EQUAL(i, message2->getIntProperty("c"));
        CPPUNIT_ASSERT_MESSAGE("Expected message to be a BytesMessage", dynamic_cast<BytesMessage*>(message2.get()) != NULL);
    }

    list.clear();
}

///////////////////////////////////////////////////////////////////////////////
void OpenwireTempDestinationTest::testPublishFailsForClosedConnection() {

    Pointer<ActiveMQConnectionFactory> factory(
        new ActiveMQConnectionFactory(cmsProvider->getBrokerURL()));
    factory->setAlwaysSyncSend(true);

    std::auto_ptr<Connection> tempConnection(factory->createConnection());
    tempConnection->start();

    std::auto_ptr<Session> tempSession(tempConnection->createSession());
    std::auto_ptr<TemporaryQueue> queue(tempSession->createTemporaryQueue());

    Thread::sleep(2000);

    // This message delivery should work since the temp connection is still open.
    std::auto_ptr<MessageProducer> producer(cmsProvider->getSession()->createProducer(queue.get()));
    producer->setDeliveryMode(DeliveryMode::NON_PERSISTENT);
    std::auto_ptr<TextMessage> message(cmsProvider->getSession()->createTextMessage("First"));
    producer->send(message.get());
    Thread::sleep(2000);

    // Closing the connection should destroy the temp queue that was created.
    tempConnection->close();
    Thread::sleep(5000);

    message.reset(cmsProvider->getSession()->createTextMessage("Hello"));

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a InvalidDestinationException since temp destination should not exist anymore.",
        producer->send(message.get()),
        cms::InvalidDestinationException);
}

///////////////////////////////////////////////////////////////////////////////
void OpenwireTempDestinationTest::testPublishFailsForDestoryedTempDestination() {

    Pointer<ActiveMQConnectionFactory> factory(
        new ActiveMQConnectionFactory(cmsProvider->getBrokerURL()));
    factory->setAlwaysSyncSend(true);

    std::auto_ptr<Connection> tempConnection(factory->createConnection());
    tempConnection->start();

    std::auto_ptr<Session> tempSession(tempConnection->createSession());
    std::auto_ptr<TemporaryQueue> queue(tempSession->createTemporaryQueue());

    Thread::sleep(2000);

    // This message delivery should work since the temp connection is still open.
    std::auto_ptr<MessageProducer> producer(cmsProvider->getSession()->createProducer(queue.get()));
    producer->setDeliveryMode(DeliveryMode::NON_PERSISTENT);
    std::auto_ptr<TextMessage> message(cmsProvider->getSession()->createTextMessage("First"));
    producer->send(message.get());
    Thread::sleep(2000);

    // deleting the Queue will cause sends to fail
    queue->destroy();
    Thread::sleep(5000); // Wait a little bit to let the delete take effect.

    message.reset(cmsProvider->getSession()->createTextMessage("Hello"));

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a InvalidDestinationException since temp destination should not exist anymore.",
        producer->send(message.get()),
        InvalidDestinationException);
}

///////////////////////////////////////////////////////////////////////////////
void OpenwireTempDestinationTest::testDeleteDestinationWithSubscribersFails() {

    std::auto_ptr<TemporaryQueue> queue(cmsProvider->getSession()->createTemporaryQueue());
    std::auto_ptr<MessageConsumer> consumer(cmsProvider->getSession()->createConsumer(queue.get()));

    // This message delivery should NOT work since the temp connection is now closed.
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should fail with CMSException as Subscribers are active",
        queue->destroy(),
        CMSException);
}

///////////////////////////////////////////////////////////////////////////////
void OpenwireTempDestinationTest::testCloseConnectionWithManyTempDests() {

    ArrayList< Pointer<TemporaryQueue> > tempQueues;
    ArrayList< Pointer<MessageProducer> > producers;

    for (int i = 0; i < 25; ++i) {
        Pointer<TemporaryQueue> tempQueue(cmsProvider->getSession()->createTemporaryQueue());
        tempQueues.add(tempQueue);
        Pointer<MessageProducer> producer(cmsProvider->getSession()->createProducer(tempQueue.get()));
        producers.add(producer);
    }

    cmsProvider->getConnection()->close();

    tempQueues.clear();
    producers.clear();
}
