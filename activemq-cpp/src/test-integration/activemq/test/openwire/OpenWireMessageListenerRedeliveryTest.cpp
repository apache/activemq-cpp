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

#include "OpenWireMessageListenerRedeliveryTest.h"

#include <cms/ConnectionFactory.h>
#include <cms/Connection.h>
#include <cms/Session.h>

#include <activemq/core/policies/DefaultRedeliveryPolicy.h>
#include <activemq/core/ActiveMQConnectionFactory.h>
#include <activemq/core/ActiveMQConnection.h>

#include <decaf/lang/Pointer.h>
#include <decaf/lang/Thread.h>
#include <decaf/lang/Runnable.h>
#include <decaf/util/ArrayList.h>
#include <decaf/util/concurrent/TimeUnit.h>
#include <decaf/util/concurrent/CountDownLatch.h>
#include <decaf/util/concurrent/atomic/AtomicInteger.h>

#include <memory>

using namespace cms;
using namespace std;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace decaf::util::concurrent::atomic;
using namespace activemq;
using namespace activemq::core;
using namespace activemq::test;
using namespace activemq::test::openwire;

////////////////////////////////////////////////////////////////////////////////
namespace {

    static std::string DLQ_DELIVERY_FAILURE_CAUSE_PROPERTY = "dlqDeliveryFailureCause";

    cms::Connection* createConnection(const std::string& brokerUri) {

        ActiveMQConnectionFactory factory(brokerUri);

        factory.getRedeliveryPolicy()->setInitialRedeliveryDelay(0);
        factory.getRedeliveryPolicy()->setRedeliveryDelay(1000);
        factory.getRedeliveryPolicy()->setMaximumRedeliveries(3);
        factory.getRedeliveryPolicy()->setBackOffMultiplier((short) 2);
        factory.getRedeliveryPolicy()->setUseExponentialBackOff(true);

        return factory.createConnection();
    }

    class TestMessageListener : public cms::MessageListener {
    private:

        int counter;
        cms::Session* session;

    public:

        TestMessageListener(cms::Session* session) : counter(0), session(session) {}

        int getCounter() {
            return counter;
        }

        virtual void onMessage(const cms::Message* message) {

            try {
                counter++;

                if (counter <= 4) {
                    session->rollback();
                } else {
                    message->acknowledge();
                    session->commit();
                }
            } catch (CMSException& e) {
            }
        }
    };

    class ExceptionMessageListener : public cms::MessageListener {
    private:

        CountDownLatch doneLatch;
        ArrayList<std::string> received;
        std::string testName;
        int maxDeliveries;
        int count;

    public:

        ExceptionMessageListener(const std::string& testName, int messageCount, int maxDeliveries) :
            doneLatch(messageCount),
            received(),
            testName(testName),
            maxDeliveries(maxDeliveries),
            count(0) {
        }

        int getCount() {
            return count;
        }

        ArrayList<std::string>& getReceived() {
            return received;
        }

        bool await(long long timeout, const TimeUnit& unit) {
            return doneLatch.await(timeout, unit);
        }

        virtual void onMessage(const cms::Message* message) {
            try {
                const TextMessage* textMessage = dynamic_cast<const cms::TextMessage*>(message);
                received.add(textMessage->getText());
            } catch (cms::CMSException& e) {
                e.printStackTrace();
            }

            if (++count < maxDeliveries) {
                throw decaf::lang::exceptions::RuntimeException(
                    __FILE__, __LINE__, testName.append(" forced a redelivery").c_str());
            }

            // new blood
            count = 0;
            doneLatch.countDown();
        }
    };

    class TrackingMessageListener : public cms::MessageListener {
    private:

        CountDownLatch doneLatch;
        ArrayList< Pointer<cms::Message> > received;
        std::string testName;
        int count;

    public:

        TrackingMessageListener(const std::string& testName) :
            doneLatch(1),
            received(),
            testName(testName),
            count(0) {
        }

        TrackingMessageListener(const std::string& testName, int expected) :
            doneLatch(expected),
            received(),
            testName(testName),
            count(0) {
        }

        int getCount() {
            return received.size();
        }

        ArrayList< Pointer<cms::Message> >& getReceived() {
            return received;
        }

        bool await(long long timeout, const TimeUnit& unit) {
            return doneLatch.await(timeout, unit);
        }

        virtual void onMessage(const cms::Message* message) {
            try {
                Pointer<cms::Message> copy(message->clone());
                received.add(copy);
                doneLatch.countDown();
            } catch (cms::CMSException& e) {
                e.printStackTrace();
            }
        }
    };

    class FailingMessageListener : public cms::MessageListener {
    private:

        CountDownLatch doneLatch;
        cms::Session* session;
        std::string testName;

    public:

        FailingMessageListener(cms::Session* session, const std::string& testName, int expected) :
            doneLatch(expected),
            session(session),
            testName(testName) {
        }

        bool await(long long timeout, const TimeUnit& unit) {
            return doneLatch.await(timeout, unit);
        }

        virtual void onMessage(const cms::Message* message) {
            try {
                doneLatch.countDown();
                if (session->isTransacted()) {
                    session->rollback();
                }
            } catch (cms::CMSException& e) {
                e.printStackTrace();
            }

            throw decaf::lang::exceptions::RuntimeException(
                __FILE__, __LINE__, (testName + " forced a redelivery").c_str());
        }
    };

}

////////////////////////////////////////////////////////////////////////////////
OpenWireMessageListenerRedeliveryTest::OpenWireMessageListenerRedeliveryTest() {
}

////////////////////////////////////////////////////////////////////////////////
OpenWireMessageListenerRedeliveryTest::~OpenWireMessageListenerRedeliveryTest() {
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireMessageListenerRedeliveryTest::testQueueRollbackConsumerListener() {

    std::auto_ptr<cms::Connection> connection(createConnection(getBrokerURL()));
    connection->start();

    std::auto_ptr<Session> session(connection->createSession(cms::Session::SESSION_TRANSACTED));
    std::auto_ptr<Queue> queue(session->createQueue("testQueueRollbackConsumerListener"));
    std::auto_ptr<MessageProducer> producer(session->createProducer(queue.get()));
    std::auto_ptr<Message> message(session->createTextMessage("Hello"));

    ActiveMQConnection* amqConnection = dynamic_cast<ActiveMQConnection*>(connection.get());
    amqConnection->destroyDestination(queue.get());

    producer->setDeliveryMode(cms::DeliveryMode::PERSISTENT);
    producer->send(message.get());
    session->commit();

    std::auto_ptr<MessageConsumer> consumer(session->createConsumer(queue.get()));

    TestMessageListener listener(session.get());
    consumer->setMessageListener(&listener);

    TimeUnit::MILLISECONDS.sleep(500);

    // first try.. should get 2 since there is no delay on the first redelivery.
    CPPUNIT_ASSERT_EQUAL(2, listener.getCounter());

    TimeUnit::MILLISECONDS.sleep(1000);

    // 2nd redeliver (redelivery after 1 sec)
    CPPUNIT_ASSERT_EQUAL(3, listener.getCounter());

    TimeUnit::MILLISECONDS.sleep(2000);

    // 3rd redeliver (redelivery after 2 seconds) - it should give up after that
    CPPUNIT_ASSERT_EQUAL(4, listener.getCounter());

    // create new message
    std::auto_ptr<Message> secondMessage(session->createTextMessage("Hello 2"));
    producer->send(secondMessage.get());
    session->commit();

    TimeUnit::MILLISECONDS.sleep(500);

    // it should be committed, so no redelivery
    CPPUNIT_ASSERT_EQUAL(5, listener.getCounter());

    TimeUnit::MILLISECONDS.sleep(1500);

    // no redelivery, counter should still be 4
    CPPUNIT_ASSERT_EQUAL(5, listener.getCounter());

    connection->close();
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireMessageListenerRedeliveryTest::testQueueSessionListenerExceptionRetry() {

    std::auto_ptr<cms::Connection> connection(createConnection(getBrokerURL()));
    connection->start();

    std::auto_ptr<Session> session(connection->createSession(cms::Session::AUTO_ACKNOWLEDGE));
    std::auto_ptr<Queue> queue(session->createQueue("testQueueSessionListenerExceptionRetry"));
    std::auto_ptr<MessageProducer> producer(session->createProducer(queue.get()));
    std::auto_ptr<Message> message1(session->createTextMessage("1"));
    std::auto_ptr<Message> message2(session->createTextMessage("2"));

    ActiveMQConnection* amqConnection = dynamic_cast<ActiveMQConnection*>(connection.get());
    amqConnection->destroyDestination(queue.get());

    producer->setDeliveryMode(cms::DeliveryMode::PERSISTENT);
    producer->send(message1.get());
    producer->send(message2.get());

    std::auto_ptr<MessageConsumer> consumer(session->createConsumer(queue.get()));

    int maxDeliveries = amqConnection->getRedeliveryPolicy()->getMaximumRedeliveries();

    ExceptionMessageListener listener("testQueueSessionListenerExceptionRetry", 2, maxDeliveries);
    consumer->setMessageListener(&listener);

    CPPUNIT_ASSERT_MESSAGE("got message before retry expiry", listener.await(30, TimeUnit::SECONDS));

    for (int i = 0; i < maxDeliveries; i++) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE("got first redelivered: " + Integer::toString(i),
            std::string("1"), listener.getReceived().get(i));
    }

    for (int i = maxDeliveries; i < maxDeliveries * 2; i++) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE("got first redelivered: " + Integer::toString(i),
            std::string("2"), listener.getReceived().get(i));
    }

    connection->close();
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireMessageListenerRedeliveryTest::testQueueSessionListenerExceptionDlq() {

    const std::string TEST_NAME = "testQueueSessionListenerExceptionDlq";

    std::auto_ptr<cms::Connection> connection(createConnection(getBrokerURL()));
    connection->start();

    std::auto_ptr<Session> session(connection->createSession(cms::Session::AUTO_ACKNOWLEDGE));
    std::auto_ptr<Queue> queue(session->createQueue(TEST_NAME));
    std::auto_ptr<Queue> dlq(session->createQueue("ActiveMQ.DLQ"));

    ActiveMQConnection* amqConnection = dynamic_cast<ActiveMQConnection*>(connection.get());
    amqConnection->destroyDestination(queue.get());
    amqConnection->destroyDestination(dlq.get());

    std::auto_ptr<MessageProducer> producer(session->createProducer(queue.get()));
    std::auto_ptr<Message> message(session->createTextMessage("1"));
    producer->setDeliveryMode(cms::DeliveryMode::PERSISTENT);
    producer->send(message.get());

    // Track messages going to DLQ
    TrackingMessageListener dlqListener(TEST_NAME);
    std::auto_ptr<MessageConsumer> dlqConsumer(session->createConsumer(dlq.get()));
    dlqConsumer->setMessageListener(&dlqListener);

    // Receive and throw
    int maxDeliveries = amqConnection->getRedeliveryPolicy()->getMaximumRedeliveries();
    FailingMessageListener listener(session.get(), TEST_NAME, maxDeliveries);
    std::auto_ptr<MessageConsumer> consumer(session->createConsumer(queue.get()));
    consumer->setMessageListener(&listener);

    CPPUNIT_ASSERT_MESSAGE("got message before retry expiry", listener.await(20, TimeUnit::SECONDS));

    // check DLQ
    CPPUNIT_ASSERT_MESSAGE("got dlq message", dlqListener.await(30, TimeUnit::SECONDS));

    // check DLQ message cause is captured
    Pointer<cms::Message> dlqMessage = dlqListener.getReceived().get(0);
    CPPUNIT_ASSERT_MESSAGE("dlq message captured", dlqMessage != NULL);
    String cause = dlqMessage->getStringProperty(DLQ_DELIVERY_FAILURE_CAUSE_PROPERTY);

    CPPUNIT_ASSERT_MESSAGE("cause 'cause' exception is remembered", cause.contains("Exception"));
    CPPUNIT_ASSERT_MESSAGE("is correct exception", cause.contains(TEST_NAME));
    CPPUNIT_ASSERT_MESSAGE("cause exception is remembered", cause.contains("JMSException"));
    CPPUNIT_ASSERT_MESSAGE("cause policy is remembered", cause.contains("RedeliveryPolicy"));

    connection->close();
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireMessageListenerRedeliveryTest::testTransactedQueueSessionListenerExceptionDlq() {

    const std::string TEST_NAME = "testTransactedQueueSessionListenerExceptionDlq";

    std::auto_ptr<cms::Connection> connection(createConnection(getBrokerURL()));
    connection->start();

    std::auto_ptr<Session> session(connection->createSession(cms::Session::SESSION_TRANSACTED));
    std::auto_ptr<Queue> queue(session->createQueue(TEST_NAME));
    std::auto_ptr<Queue> dlq(session->createQueue("ActiveMQ.DLQ"));

    ActiveMQConnection* amqConnection = dynamic_cast<ActiveMQConnection*>(connection.get());
    amqConnection->destroyDestination(queue.get());
    amqConnection->destroyDestination(dlq.get());

    std::auto_ptr<MessageProducer> producer(session->createProducer(queue.get()));
    std::auto_ptr<Message> message(session->createTextMessage("1"));
    producer->setDeliveryMode(cms::DeliveryMode::PERSISTENT);
    producer->send(message.get());
    session->commit();

    // Track messages going to DLQ
    TrackingMessageListener dlqListener(TEST_NAME);
    std::auto_ptr<MessageConsumer> dlqConsumer(session->createConsumer(dlq.get()));
    dlqConsumer->setMessageListener(&dlqListener);

    // Receive and throw
    int maxDeliveries = amqConnection->getRedeliveryPolicy()->getMaximumRedeliveries();
    FailingMessageListener listener(session.get(), TEST_NAME, maxDeliveries);
    std::auto_ptr<MessageConsumer> consumer(session->createConsumer(queue.get()));
    consumer->setMessageListener(&listener);

    CPPUNIT_ASSERT_MESSAGE("got message before retry expiry", listener.await(20, TimeUnit::SECONDS));

    // check DLQ
    CPPUNIT_ASSERT_MESSAGE("got dlq message", dlqListener.await(30, TimeUnit::SECONDS));

    // check DLQ message cause is captured
    Pointer<cms::Message> dlqMessage = dlqListener.getReceived().get(0);
    CPPUNIT_ASSERT_MESSAGE("dlq message captured", dlqMessage != NULL);
    String cause = dlqMessage->getStringProperty(DLQ_DELIVERY_FAILURE_CAUSE_PROPERTY);

    CPPUNIT_ASSERT_MESSAGE("cause 'cause' exception is remembered", cause.contains("Exception"));
    CPPUNIT_ASSERT_MESSAGE("is correct exception", cause.contains(TEST_NAME));
    CPPUNIT_ASSERT_MESSAGE("cause exception is remembered", cause.contains("JMSException"));
    CPPUNIT_ASSERT_MESSAGE("cause policy is remembered", cause.contains("RedeliveryPolicy"));

    connection->close();
}
