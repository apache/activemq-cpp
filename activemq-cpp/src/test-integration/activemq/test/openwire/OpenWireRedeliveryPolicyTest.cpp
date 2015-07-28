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

#include "OpenWireRedeliveryPolicyTest.h"

#include <cms/Connection.h>
#include <cms/Session.h>
#include <cms/MessageProducer.h>
#include <cms/MessageConsumer.h>

#include <activemq/core/policies/DefaultRedeliveryPolicy.h>
#include <activemq/core/ActiveMQConnectionFactory.h>
#include <activemq/core/ActiveMQConnection.h>
#include <activemq/core/ActiveMQConsumer.h>
#include <activemq/commands/ActiveMQTextMessage.h>

#include <decaf/lang/Thread.h>
#include <decaf/lang/Pointer.h>
#include <decaf/lang/Long.h>
#include <decaf/util/concurrent/CountDownLatch.h>
#include <decaf/util/concurrent/atomic/AtomicInteger.h>

using namespace cms;
using namespace activemq;
using namespace activemq::commands;
using namespace activemq::core;
using namespace activemq::core::policies;
using namespace activemq::test;
using namespace activemq::test::openwire;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace decaf::util::concurrent::atomic;

////////////////////////////////////////////////////////////////////////////////
OpenWireRedeliveryPolicyTest::OpenWireRedeliveryPolicyTest() {
}

////////////////////////////////////////////////////////////////////////////////
OpenWireRedeliveryPolicyTest::~OpenWireRedeliveryPolicyTest() {
}

////////////////////////////////////////////////////////////////////////////////
std::string OpenWireRedeliveryPolicyTest::getBrokerURL() const {
    return activemq::util::IntegrationCommon::getInstance().getOpenwireURL();
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireRedeliveryPolicyTest::testGetNext() {

    DefaultRedeliveryPolicy policy;
    policy.setInitialRedeliveryDelay(0);
    policy.setRedeliveryDelay(500);
    policy.setBackOffMultiplier((short) 2);
    policy.setUseExponentialBackOff(true);

    long long delay = policy.getNextRedeliveryDelay(0);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect delay for cycle 1", 500LL, delay);
    delay = policy.getNextRedeliveryDelay(delay);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect delay for cycle 2", 500L*2LL, delay);
    delay = policy.getNextRedeliveryDelay(delay);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect delay for cycle 3", 500L*4LL, delay);

    policy.setUseExponentialBackOff(false);
    delay = policy.getNextRedeliveryDelay(delay);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect delay for cycle 4", 500LL, delay);
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireRedeliveryPolicyTest::testGetNextWithInitialDelay() {

    DefaultRedeliveryPolicy policy;
    policy.setInitialRedeliveryDelay(500);

    long long delay = policy.getNextRedeliveryDelay(500);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect delay for cycle 1", 1000LL, delay);
    delay = policy.getNextRedeliveryDelay(delay);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect delay for cycle 2", 1000LL, delay);
    delay = policy.getNextRedeliveryDelay(delay);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect delay for cycle 3", 1000LL, delay);
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireRedeliveryPolicyTest::testExponentialRedeliveryPolicyDelaysDeliveryOnRollback() {

    Pointer<ActiveMQConnectionFactory> connectionFactory(
        new ActiveMQConnectionFactory(getBrokerURL()));

    Pointer<Connection> connection(connectionFactory->createConnection());
    Pointer<ActiveMQConnection> amqConnection = connection.dynamicCast<ActiveMQConnection>();

    // Receive a message with the JMS API
    RedeliveryPolicy* policy = amqConnection->getRedeliveryPolicy();
    policy->setInitialRedeliveryDelay(0);
    policy->setRedeliveryDelay(500);
    policy->setBackOffMultiplier((short) 2);
    policy->setUseExponentialBackOff(true);

    connection->start();
    Pointer<Session> session(connection->createSession(Session::SESSION_TRANSACTED));
    Pointer<Queue> destination(session->createTemporaryQueue());
    Pointer<MessageProducer> producer(session->createProducer(destination.get()));
    Pointer<MessageConsumer> consumer(session->createConsumer(destination.get()));

    // Send the messages
    Pointer<TextMessage> message1(session->createTextMessage("1st"));
    Pointer<TextMessage> message2(session->createTextMessage("2nd"));

    producer->send(message1.get());
    producer->send(message2.get());
    session->commit();

    Pointer<cms::Message> received(consumer->receive(1000));
    Pointer<TextMessage> textMessage = received.dynamicCast<TextMessage>();
    CPPUNIT_ASSERT(textMessage != NULL);
    CPPUNIT_ASSERT_EQUAL(std::string("1st"), textMessage->getText());
    session->rollback();

    // No delay on first rollback..
    received.reset(consumer->receive(250));
    CPPUNIT_ASSERT(received != NULL);
    session->rollback();

    // Show subsequent re-delivery delay is incrementing.
    received.reset(consumer->receive(250));
    CPPUNIT_ASSERT(received == NULL);

    received.reset(consumer->receive(750));
    CPPUNIT_ASSERT(received != NULL);
    textMessage = received.dynamicCast<TextMessage>();
    CPPUNIT_ASSERT_EQUAL(std::string("1st"), textMessage->getText());
    session->rollback();

    // Show re-delivery delay is incrementing exponentially
    received.reset(consumer->receive(100));
    CPPUNIT_ASSERT(received == NULL);
    received.reset(consumer->receive(500));
    CPPUNIT_ASSERT(received == NULL);
    received.reset(consumer->receive(800));
    CPPUNIT_ASSERT(received != NULL);
    textMessage = received.dynamicCast<TextMessage>();
    CPPUNIT_ASSERT_EQUAL(std::string("1st"), textMessage->getText());
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireRedeliveryPolicyTest::testNornalRedeliveryPolicyDelaysDeliveryOnRollback() {

    Pointer<ActiveMQConnectionFactory> connectionFactory(
        new ActiveMQConnectionFactory(getBrokerURL()));

    Pointer<Connection> connection(connectionFactory->createConnection());
    Pointer<ActiveMQConnection> amqConnection = connection.dynamicCast<ActiveMQConnection>();

    // Receive a message with the JMS API
    RedeliveryPolicy* policy = amqConnection->getRedeliveryPolicy();
    policy->setInitialRedeliveryDelay(0);
    policy->setRedeliveryDelay(500);

    connection->start();
    Pointer<Session> session(connection->createSession(Session::SESSION_TRANSACTED));
    Pointer<Queue> destination(session->createTemporaryQueue());
    Pointer<MessageProducer> producer(session->createProducer(destination.get()));
    Pointer<MessageConsumer> consumer(session->createConsumer(destination.get()));

    // Send the messages
    Pointer<TextMessage> message1(session->createTextMessage("1st"));
    Pointer<TextMessage> message2(session->createTextMessage("2nd"));

    producer->send(message1.get());
    producer->send(message2.get());
    session->commit();

    Pointer<cms::Message> received(consumer->receive(1000));
    Pointer<TextMessage> textMessage = received.dynamicCast<TextMessage>();
    CPPUNIT_ASSERT(textMessage != NULL);
    CPPUNIT_ASSERT_EQUAL(std::string("1st"), textMessage->getText());
    session->rollback();

    // No delay on first rollback..
    received.reset(consumer->receive(250));
    CPPUNIT_ASSERT(received != NULL);
    session->rollback();

    // Show subsequent re-delivery delay is incrementing.
    received.reset(consumer->receive(100));
    CPPUNIT_ASSERT(received == NULL);
    received.reset(consumer->receive(700));
    CPPUNIT_ASSERT(received != NULL);
    textMessage = received.dynamicCast<TextMessage>();
    CPPUNIT_ASSERT_EQUAL(std::string("1st"), textMessage->getText());
    session->rollback();

    // The message gets redelivered after 500 ms every time since
    // we are not using exponential backoff.
    received.reset(consumer->receive(100));
    CPPUNIT_ASSERT(received == NULL);
    received.reset(consumer->receive(700));
    CPPUNIT_ASSERT(received != NULL);
    textMessage = received.dynamicCast<TextMessage>();
    CPPUNIT_ASSERT_EQUAL(std::string("1st"), textMessage->getText());
    session->commit();
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireRedeliveryPolicyTest::testDLQHandling() {

    Pointer<ActiveMQConnectionFactory> connectionFactory(
        new ActiveMQConnectionFactory(getBrokerURL()));

    Pointer<Connection> connection(connectionFactory->createConnection());
    Pointer<ActiveMQConnection> amqConnection = connection.dynamicCast<ActiveMQConnection>();

    // Receive a message with the JMS API
    RedeliveryPolicy* policy = amqConnection->getRedeliveryPolicy();
    policy->setInitialRedeliveryDelay(100);
    policy->setUseExponentialBackOff(false);
    policy->setMaximumRedeliveries(2);

    connection->start();
    Pointer<Session> session(connection->createSession(Session::SESSION_TRANSACTED));
    Pointer<Queue> destination(session->createTemporaryQueue());
    Pointer<MessageProducer> producer(session->createProducer(destination.get()));
    Pointer<MessageConsumer> consumer(session->createConsumer(destination.get()));
    Pointer<Queue> dlq(session->createQueue("ActiveMQ.DLQ"));
    amqConnection->destroyDestination(dlq.get());
    Pointer<MessageConsumer> dlqConsumer(session->createConsumer(dlq.get()));

    // Send the messages
    Pointer<TextMessage> message1(session->createTextMessage("1st"));
    Pointer<TextMessage> message2(session->createTextMessage("2nd"));

    producer->send(message1.get());
    producer->send(message2.get());
    session->commit();

    Pointer<cms::Message> received(consumer->receive(1000));
    Pointer<TextMessage> textMessage = received.dynamicCast<TextMessage>();
    CPPUNIT_ASSERT_MESSAGE("Failed to get first delivery", textMessage != NULL);
    CPPUNIT_ASSERT_EQUAL(std::string("1st"), textMessage->getText());
    session->rollback();

    received.reset(consumer->receive(1000));
    CPPUNIT_ASSERT_MESSAGE("Failed to get second delivery", received != NULL);
    textMessage = received.dynamicCast<TextMessage>();
    CPPUNIT_ASSERT_EQUAL(std::string("1st"), textMessage->getText());
    session->rollback();

    received.reset(consumer->receive(2000));
    CPPUNIT_ASSERT_MESSAGE("Failed to get third delivery", received != NULL);
    textMessage = received.dynamicCast<TextMessage>();
    CPPUNIT_ASSERT_EQUAL(std::string("1st"), textMessage->getText());
    session->rollback();

    // The last rollback should cause the 1st message to get sent to the DLQ
    received.reset(consumer->receive(1000));
    CPPUNIT_ASSERT_MESSAGE("Failed to get first delivery of msg 2", received != NULL);
    textMessage = received.dynamicCast<TextMessage>();
    CPPUNIT_ASSERT_EQUAL(std::string("2nd"), textMessage->getText());
    session->commit();

    // We should be able to get the message off the DLQ now.
    received.reset(dlqConsumer->receive(1000));
    CPPUNIT_ASSERT_MESSAGE("Failed to get DLQ'd message", received != NULL);
    textMessage = received.dynamicCast<TextMessage>();
    CPPUNIT_ASSERT_EQUAL(std::string("1st"), textMessage->getText());
    session->commit();

    if (textMessage->propertyExists("dlqDeliveryFailureCause")) {
        std::string cause = textMessage->getStringProperty("dlqDeliveryFailureCause");
        CPPUNIT_ASSERT_MESSAGE("cause exception has no policy ref",
                               cause.find("RedeliveryPolicy") != std::string::npos);
    }
    session->commit();
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireRedeliveryPolicyTest::testInfiniteMaximumNumberOfRedeliveries() {

    Pointer<ActiveMQConnectionFactory> connectionFactory(
        new ActiveMQConnectionFactory(getBrokerURL()));

    Pointer<Connection> connection(connectionFactory->createConnection());
    Pointer<ActiveMQConnection> amqConnection = connection.dynamicCast<ActiveMQConnection>();

    // Receive a message with the JMS API
    RedeliveryPolicy* policy = amqConnection->getRedeliveryPolicy();
    policy->setInitialRedeliveryDelay(100);
    policy->setUseExponentialBackOff(false);
    // let's set the maximum redeliveries to no maximum (ie. infinite)
    policy->setMaximumRedeliveries(-1);

    connection->start();
    Pointer<Session> session(connection->createSession(Session::SESSION_TRANSACTED));
    Pointer<Queue> destination(session->createTemporaryQueue());
    Pointer<MessageProducer> producer(session->createProducer(destination.get()));
    Pointer<MessageConsumer> consumer(session->createConsumer(destination.get()));

    // Send the messages
    Pointer<TextMessage> message1(session->createTextMessage("1st"));
    Pointer<TextMessage> message2(session->createTextMessage("2nd"));

    producer->send(message1.get());
    producer->send(message2.get());
    session->commit();

    Pointer<cms::Message> received(consumer->receive(1000));
    Pointer<TextMessage> textMessage = received.dynamicCast<TextMessage>();
    CPPUNIT_ASSERT_MESSAGE("Failed to get first delivery", textMessage != NULL);
    CPPUNIT_ASSERT_EQUAL(std::string("1st"), textMessage->getText());
    session->rollback();

    // we should be able to get the 1st message redelivered until a session.commit is called
    received.reset(consumer->receive(1000));
    CPPUNIT_ASSERT_MESSAGE("Failed to get second delivery", received != NULL);
    textMessage = received.dynamicCast<TextMessage>();
    CPPUNIT_ASSERT_EQUAL(std::string("1st"), textMessage->getText());
    session->rollback();

    received.reset(consumer->receive(2000));
    CPPUNIT_ASSERT_MESSAGE("Failed to get third delivery", received != NULL);
    textMessage = received.dynamicCast<TextMessage>();
    CPPUNIT_ASSERT_EQUAL(std::string("1st"), textMessage->getText());
    session->rollback();

    received.reset(consumer->receive(2000));
    CPPUNIT_ASSERT_MESSAGE("Failed to get fourth delivery", received != NULL);
    textMessage = received.dynamicCast<TextMessage>();
    CPPUNIT_ASSERT_EQUAL(std::string("1st"), textMessage->getText());
    session->rollback();

    received.reset(consumer->receive(2000));
    CPPUNIT_ASSERT_MESSAGE("Failed to get fifth delivery", received != NULL);
    textMessage = received.dynamicCast<TextMessage>();
    CPPUNIT_ASSERT_EQUAL(std::string("1st"), textMessage->getText());
    session->rollback();

    received.reset(consumer->receive(2000));
    CPPUNIT_ASSERT_MESSAGE("Failed to get sixth delivery", received != NULL);
    textMessage = received.dynamicCast<TextMessage>();
    CPPUNIT_ASSERT_EQUAL(std::string("1st"), textMessage->getText());
    session->commit();

    received.reset(consumer->receive(1000));
    CPPUNIT_ASSERT_MESSAGE("Failed to get message two", received != NULL);
    textMessage = received.dynamicCast<TextMessage>();
    CPPUNIT_ASSERT_EQUAL(std::string("2nd"), textMessage->getText());
    session->commit();
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireRedeliveryPolicyTest::testMaximumRedeliveryDelay() {

    Pointer<ActiveMQConnectionFactory> connectionFactory(
        new ActiveMQConnectionFactory(getBrokerURL()));

    Pointer<Connection> connection(connectionFactory->createConnection());
    Pointer<ActiveMQConnection> amqConnection = connection.dynamicCast<ActiveMQConnection>();

    // Receive a message with the JMS API
    RedeliveryPolicy* policy = amqConnection->getRedeliveryPolicy();
    policy->setInitialRedeliveryDelay(10);
    policy->setUseExponentialBackOff(true);
    policy->setMaximumRedeliveries(-1);
    policy->setRedeliveryDelay(50);
    policy->setMaximumRedeliveryDelay(1000);
    policy->setBackOffMultiplier((short) 2);
    policy->setUseExponentialBackOff(true);

    connection->start();
    Pointer<Session> session(connection->createSession(Session::SESSION_TRANSACTED));
    Pointer<Queue> destination(session->createTemporaryQueue());
    Pointer<MessageProducer> producer(session->createProducer(destination.get()));
    Pointer<MessageConsumer> consumer(session->createConsumer(destination.get()));

    // Send the messages
    Pointer<TextMessage> message1(session->createTextMessage("1st"));
    Pointer<TextMessage> message2(session->createTextMessage("2nd"));

    producer->send(message1.get());
    producer->send(message2.get());
    session->commit();

    Pointer<cms::Message> received;

    for(int i = 0; i < 10; ++i) {
        // we should be able to get the 1st message redelivered until a session.commit is called
        received.reset(consumer->receive(2000));
        Pointer<TextMessage> textMessage = received.dynamicCast<TextMessage>();
        CPPUNIT_ASSERT_MESSAGE("Failed to get message", textMessage != NULL);
        CPPUNIT_ASSERT_EQUAL(std::string("1st"), textMessage->getText());
        session->rollback();
    }

    received.reset(consumer->receive(2000));
    Pointer<TextMessage> textMessage = received.dynamicCast<TextMessage>();
    CPPUNIT_ASSERT_MESSAGE("Failed to get message one last time", textMessage != NULL);
    CPPUNIT_ASSERT_EQUAL(std::string("1st"), textMessage->getText());
    session->commit();

    received.reset(consumer->receive(2000));
    CPPUNIT_ASSERT_MESSAGE("Failed to get message two", received != NULL);
    textMessage = received.dynamicCast<TextMessage>();
    CPPUNIT_ASSERT_EQUAL(std::string("2nd"), textMessage->getText());
    session->commit();

    long long result = policy->getNextRedeliveryDelay(Integer::MAX_VALUE);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Max delay should be 1 second.", 1000LL, result);
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireRedeliveryPolicyTest::testZeroMaximumNumberOfRedeliveries() {

    Pointer<ActiveMQConnectionFactory> connectionFactory(
        new ActiveMQConnectionFactory(getBrokerURL()));

    Pointer<Connection> connection(connectionFactory->createConnection());
    Pointer<ActiveMQConnection> amqConnection = connection.dynamicCast<ActiveMQConnection>();

    // Receive a message with the JMS API
    RedeliveryPolicy* policy = amqConnection->getRedeliveryPolicy();
    policy->setInitialRedeliveryDelay(100);
    policy->setUseExponentialBackOff(false);
    // let's set the maximum redeliveries to 0
    policy->setMaximumRedeliveries(0);

    connection->start();
    Pointer<Session> session(connection->createSession(Session::SESSION_TRANSACTED));
    Pointer<Queue> destination(session->createTemporaryQueue());
    Pointer<MessageProducer> producer(session->createProducer(destination.get()));
    Pointer<MessageConsumer> consumer(session->createConsumer(destination.get()));

    // Send the messages
    Pointer<TextMessage> message1(session->createTextMessage("1st"));
    Pointer<TextMessage> message2(session->createTextMessage("2nd"));

    producer->send(message1.get());
    producer->send(message2.get());
    session->commit();

    Pointer<cms::Message> received(consumer->receive(1000));
    Pointer<TextMessage> textMessage = received.dynamicCast<TextMessage>();
    CPPUNIT_ASSERT_MESSAGE("Failed to get first delivery", textMessage != NULL);
    CPPUNIT_ASSERT_EQUAL(std::string("1st"), textMessage->getText());
    session->rollback();

    // the 1st  message should not be redelivered since maximumRedeliveries is set to 0
    received.reset(consumer->receive(1000));
    CPPUNIT_ASSERT_MESSAGE("Failed to get message two", received != NULL);
    textMessage = received.dynamicCast<TextMessage>();
    CPPUNIT_ASSERT_EQUAL(std::string("2nd"), textMessage->getText());
    session->commit();
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireRedeliveryPolicyTest::testRepeatedRedeliveryReceiveNoCommit() {

    Pointer<ActiveMQConnectionFactory> connectionFactory(
        new ActiveMQConnectionFactory(getBrokerURL()));

    Pointer<Connection> connection(connectionFactory->createConnection());
    Pointer<ActiveMQConnection> amqConnection = connection.dynamicCast<ActiveMQConnection>();

    connection->start();
    Pointer<Session> dlqSession(connection->createSession(Session::AUTO_ACKNOWLEDGE));
    Pointer<Queue> destination(dlqSession->createQueue("testRepeatedRedeliveryReceiveNoCommit"));
    Pointer<Queue> dlq(dlqSession->createQueue("ActiveMQ.DLQ"));
    amqConnection->destroyDestination(destination.get());
    amqConnection->destroyDestination(dlq.get());
    Pointer<MessageProducer> producer(dlqSession->createProducer(destination.get()));
    Pointer<MessageConsumer> consumer(dlqSession->createConsumer(dlq.get()));

    Pointer<TextMessage> message1(dlqSession->createTextMessage("1st"));
    producer->send(message1.get());

    const int MAX_REDELIVERIES = 4;

    for (int i = 0; i <= MAX_REDELIVERIES + 1; i++) {
        Pointer<Connection> loopConnection(connectionFactory->createConnection());
        Pointer<ActiveMQConnection> amqConnection = loopConnection.dynamicCast<ActiveMQConnection>();

        // Receive a message with the JMS API
        RedeliveryPolicy* policy = amqConnection->getRedeliveryPolicy();
        policy->setInitialRedeliveryDelay(0);
        policy->setUseExponentialBackOff(false);
        policy->setMaximumRedeliveries(MAX_REDELIVERIES);

        loopConnection->start();
        Pointer<Session> session(loopConnection->createSession(Session::SESSION_TRANSACTED));
        Pointer<MessageConsumer> consumer(session->createConsumer(destination.get()));

        Pointer<cms::Message> received(consumer->receive(1000));

        if (i <= MAX_REDELIVERIES) {
            Pointer<ActiveMQTextMessage> textMessage = received.dynamicCast<ActiveMQTextMessage>();
            CPPUNIT_ASSERT_MESSAGE("Failed to get first delivery", textMessage != NULL);
            CPPUNIT_ASSERT_EQUAL(std::string("1st"), textMessage->getText());
            CPPUNIT_ASSERT_EQUAL(i, textMessage->getRedeliveryCounter());
        } else {
            CPPUNIT_ASSERT_MESSAGE("null on exceeding redelivery count", received == NULL);
        }

        loopConnection->close();
    }

    // We should be able to get the message off the DLQ now.
    Pointer<cms::Message> received(consumer->receive(1000));
    CPPUNIT_ASSERT_MESSAGE("Failed to get from DLQ", received != NULL);
    Pointer<TextMessage> textMessage = received.dynamicCast<TextMessage>();
    CPPUNIT_ASSERT_EQUAL(std::string("1st"), textMessage->getText());

    if (textMessage->propertyExists("dlqDeliveryFailureCause")) {
        std::string cause = textMessage->getStringProperty("dlqDeliveryFailureCause");
        CPPUNIT_ASSERT_MESSAGE("cause exception has no policy ref",
                               cause.find("RedeliveryPolicy") != std::string::npos);
    } else {
        CPPUNIT_FAIL("Message did not have a rollback cause");
    }
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class AsyncListener : public cms::MessageListener {
    private:

        AtomicInteger* receivedCount;
        CountDownLatch* done;

    public:

        AsyncListener(AtomicInteger* receivedCount, CountDownLatch* done) {
            this->receivedCount = receivedCount;
            this->done = done;
        }

        virtual void onMessage(const cms::Message* message) {
            try {
                const ActiveMQTextMessage* textMessage = dynamic_cast<const ActiveMQTextMessage*>(message);
                CPPUNIT_ASSERT_MESSAGE("Failed to get first delivery", textMessage != NULL);
                CPPUNIT_ASSERT_EQUAL(std::string("1st"), textMessage->getText());
                CPPUNIT_ASSERT_EQUAL(receivedCount->get(), textMessage->getRedeliveryCounter());
                receivedCount->incrementAndGet();
                done->countDown();
            } catch (Exception& ignored) {
                ignored.printStackTrace();
            }
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireRedeliveryPolicyTest::testRepeatedRedeliveryOnMessageNoCommit() {

    Pointer<ActiveMQConnectionFactory> connectionFactory(
        new ActiveMQConnectionFactory(getBrokerURL()));

    Pointer<Connection> connection(connectionFactory->createConnection());
    Pointer<ActiveMQConnection> amqConnection = connection.dynamicCast<ActiveMQConnection>();

    connection->start();
    Pointer<Session> dlqSession(connection->createSession(Session::AUTO_ACKNOWLEDGE));
    Pointer<Queue> destination(dlqSession->createQueue("testRepeatedRedeliveryOnMessageNoCommit"));
    Pointer<Queue> dlq(dlqSession->createQueue("ActiveMQ.DLQ"));
    amqConnection->destroyDestination(destination.get());
    amqConnection->destroyDestination(dlq.get());
    Pointer<MessageProducer> producer(dlqSession->createProducer(destination.get()));
    Pointer<MessageConsumer> consumer(dlqSession->createConsumer(dlq.get()));

    // Send the messages
    Pointer<TextMessage> message1(dlqSession->createTextMessage("1st"));
    producer->send(message1.get());

    const int MAX_REDELIVERIES = 4;
    AtomicInteger receivedCount(0);

    for (int i = 0; i <= MAX_REDELIVERIES + 1; i++) {

        Pointer<Connection> loopConnection(connectionFactory->createConnection());
        Pointer<ActiveMQConnection> amqConnection = loopConnection.dynamicCast<ActiveMQConnection>();

        // Receive a message with the JMS API
        RedeliveryPolicy* policy = amqConnection->getRedeliveryPolicy();
        policy->setInitialRedeliveryDelay(0);
        policy->setUseExponentialBackOff(false);
        policy->setMaximumRedeliveries(MAX_REDELIVERIES);

        loopConnection->start();
        Pointer<Session> session(loopConnection->createSession(Session::SESSION_TRANSACTED));
        Pointer<MessageConsumer> consumer(session->createConsumer(destination.get()));

        CountDownLatch done(1);

        AsyncListener listener(&receivedCount, &done);
        consumer->setMessageListener(&listener);

        if (i <= MAX_REDELIVERIES) {
            CPPUNIT_ASSERT_MESSAGE("listener didn't get a message", done.await(5, TimeUnit::SECONDS));
        } else {
            // final redlivery gets poisoned before dispatch
            CPPUNIT_ASSERT_MESSAGE("listener got unexpected message", !done.await(2, TimeUnit::SECONDS));
        }

        loopConnection->close();
    }

    // We should be able to get the message off the DLQ now.
    Pointer<cms::Message> received(consumer->receive(1000));
    CPPUNIT_ASSERT_MESSAGE("Failed to get from DLQ", received != NULL);
    Pointer<TextMessage> textMessage = received.dynamicCast<TextMessage>();
    CPPUNIT_ASSERT_EQUAL(std::string("1st"), textMessage->getText());

    if (textMessage->propertyExists("dlqDeliveryFailureCause")) {
        std::string cause = textMessage->getStringProperty("dlqDeliveryFailureCause");
        CPPUNIT_ASSERT_MESSAGE("cause exception has no policy ref",
                               cause.find("RedeliveryPolicy") != std::string::npos);
    } else {
        CPPUNIT_FAIL("Message did not have a rollback cause");
    }
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireRedeliveryPolicyTest::testInitialRedeliveryDelayZero() {

    Pointer<ActiveMQConnectionFactory> connectionFactory(
        new ActiveMQConnectionFactory(getBrokerURL()));

    Pointer<Connection> connection(connectionFactory->createConnection());
    Pointer<ActiveMQConnection> amqConnection = connection.dynamicCast<ActiveMQConnection>();

    // Receive a message with the JMS API
    RedeliveryPolicy* policy = amqConnection->getRedeliveryPolicy();
    policy->setInitialRedeliveryDelay(0);
    policy->setUseExponentialBackOff(false);
    policy->setMaximumRedeliveries(1);

    connection->start();
    Pointer<Session> session(connection->createSession(Session::SESSION_TRANSACTED));
    Pointer<Queue> destination(session->createTemporaryQueue());
    Pointer<MessageProducer> producer(session->createProducer(destination.get()));
    Pointer<MessageConsumer> consumer(session->createConsumer(destination.get()));

    // Send the messages
    Pointer<TextMessage> message1(session->createTextMessage("1st"));
    Pointer<TextMessage> message2(session->createTextMessage("2nd"));

    producer->send(message1.get());
    producer->send(message2.get());
    session->commit();

    Pointer<cms::Message> received(consumer->receive(100));
    Pointer<TextMessage> textMessage = received.dynamicCast<TextMessage>();
    CPPUNIT_ASSERT_MESSAGE("Failed to get first delivery", textMessage != NULL);
    CPPUNIT_ASSERT_EQUAL(std::string("1st"), textMessage->getText());
    session->rollback();

    // Both should be able for consumption.
    received.reset(consumer->receive(100));
    textMessage = received.dynamicCast<TextMessage>();
    CPPUNIT_ASSERT_MESSAGE("Failed to get message one again", textMessage != NULL);
    CPPUNIT_ASSERT_EQUAL(std::string("1st"), textMessage->getText());

    received.reset(consumer->receive(100));
    CPPUNIT_ASSERT_MESSAGE("Failed to get message two", received != NULL);
    textMessage = received.dynamicCast<TextMessage>();
    CPPUNIT_ASSERT_EQUAL(std::string("2nd"), textMessage->getText());
    session->commit();
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireRedeliveryPolicyTest::testInitialRedeliveryDelayOne() {

    Pointer<ActiveMQConnectionFactory> connectionFactory(
        new ActiveMQConnectionFactory(getBrokerURL()));

    Pointer<Connection> connection(connectionFactory->createConnection());
    Pointer<ActiveMQConnection> amqConnection = connection.dynamicCast<ActiveMQConnection>();

    // Receive a message with the JMS API
    RedeliveryPolicy* policy = amqConnection->getRedeliveryPolicy();
    policy->setInitialRedeliveryDelay(1000);
    policy->setUseExponentialBackOff(false);
    policy->setMaximumRedeliveries(1);

    connection->start();
    Pointer<Session> session(connection->createSession(Session::SESSION_TRANSACTED));
    Pointer<Queue> destination(session->createTemporaryQueue());
    Pointer<MessageProducer> producer(session->createProducer(destination.get()));
    Pointer<MessageConsumer> consumer(session->createConsumer(destination.get()));

    // Send the messages
    Pointer<TextMessage> message1(session->createTextMessage("1st"));
    Pointer<TextMessage> message2(session->createTextMessage("2nd"));

    producer->send(message1.get());
    producer->send(message2.get());
    session->commit();

    Pointer<cms::Message> received(consumer->receive(100));
    Pointer<TextMessage> textMessage = received.dynamicCast<TextMessage>();
    CPPUNIT_ASSERT_MESSAGE("Failed to get first delivery", textMessage != NULL);
    CPPUNIT_ASSERT_EQUAL(std::string("1st"), textMessage->getText());
    session->rollback();

    received.reset(consumer->receive(100));
    CPPUNIT_ASSERT(received == NULL);

    received.reset(consumer->receive(2000));
    textMessage = received.dynamicCast<TextMessage>();
    CPPUNIT_ASSERT_MESSAGE("Failed to get message one again", textMessage != NULL);
    CPPUNIT_ASSERT_EQUAL(std::string("1st"), textMessage->getText());

    received.reset(consumer->receive(100));
    CPPUNIT_ASSERT_MESSAGE("Failed to get message two", received != NULL);
    textMessage = received.dynamicCast<TextMessage>();
    CPPUNIT_ASSERT_EQUAL(std::string("2nd"), textMessage->getText());
    session->commit();
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireRedeliveryPolicyTest::testRedeliveryDelayOne() {

    Pointer<ActiveMQConnectionFactory> connectionFactory(
        new ActiveMQConnectionFactory(getBrokerURL()));

    Pointer<Connection> connection(connectionFactory->createConnection());
    Pointer<ActiveMQConnection> amqConnection = connection.dynamicCast<ActiveMQConnection>();

    // Receive a message with the JMS API
    RedeliveryPolicy* policy = amqConnection->getRedeliveryPolicy();
    policy->setInitialRedeliveryDelay(0);
    policy->setRedeliveryDelay(1000);
    policy->setUseExponentialBackOff(false);
    policy->setMaximumRedeliveries(2);

    connection->start();
    Pointer<Session> session(connection->createSession(Session::SESSION_TRANSACTED));
    Pointer<Queue> destination(session->createTemporaryQueue());
    Pointer<MessageProducer> producer(session->createProducer(destination.get()));
    Pointer<MessageConsumer> consumer(session->createConsumer(destination.get()));

    // Send the messages
    Pointer<TextMessage> message1(session->createTextMessage("1st"));
    Pointer<TextMessage> message2(session->createTextMessage("2nd"));

    producer->send(message1.get());
    producer->send(message2.get());
    session->commit();

    Pointer<cms::Message> received(consumer->receive(100));
    Pointer<TextMessage> textMessage = received.dynamicCast<TextMessage>();
    CPPUNIT_ASSERT_MESSAGE("Failed to get first delivery", textMessage != NULL);
    CPPUNIT_ASSERT_EQUAL(std::string("1st"), textMessage->getText());
    session->rollback();

    received.reset(consumer->receive(100));
    textMessage = received.dynamicCast<TextMessage>();
    CPPUNIT_ASSERT_MESSAGE("first redelivery was not immediate.", textMessage != NULL);
    CPPUNIT_ASSERT_EQUAL(std::string("1st"), textMessage->getText());
    session->rollback();

    received.reset(consumer->receive(100));
    CPPUNIT_ASSERT_MESSAGE("seconds redelivery should be delayed.", received == NULL);

    received.reset(consumer->receive(2000));
    textMessage = received.dynamicCast<TextMessage>();
    CPPUNIT_ASSERT_MESSAGE("Failed to get message one again", textMessage != NULL);
    CPPUNIT_ASSERT_EQUAL(std::string("1st"), textMessage->getText());

    received.reset(consumer->receive(100));
    CPPUNIT_ASSERT_MESSAGE("Failed to get message two", received != NULL);
    textMessage = received.dynamicCast<TextMessage>();
    CPPUNIT_ASSERT_EQUAL(std::string("2nd"), textMessage->getText());
    session->commit();
}
