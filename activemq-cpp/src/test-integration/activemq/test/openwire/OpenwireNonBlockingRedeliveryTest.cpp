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

#include "OpenwireNonBlockingRedeliveryTest.h"

#include <cms/MessageListener.h>
#include <cms/ExceptionListener.h>

#include <activemq/commands/Message.h>
#include <activemq/commands/ProducerId.h>
#include <activemq/commands/MessageId.h>
#include <activemq/core/ActiveMQConnectionFactory.h>
#include <activemq/core/ActiveMQConnection.h>
#include <activemq/core/ActiveMQConsumer.h>
#include <activemq/core/PrefetchPolicy.h>
#include <activemq/exceptions/ActiveMQException.h>

#include <decaf/lang/Thread.h>
#include <decaf/lang/Pointer.h>
#include <decaf/util/LinkedList.h>
#include <decaf/util/LinkedHashSet.h>
#include <decaf/util/concurrent/atomic/AtomicInteger.h>

using namespace std;
using namespace cms;
using namespace activemq;
using namespace activemq::commands;
using namespace activemq::core;
using namespace activemq::test;
using namespace activemq::test::openwire;
using namespace activemq::util;
using namespace activemq::exceptions;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace decaf::util::concurrent::atomic;

////////////////////////////////////////////////////////////////////////////////
namespace {

    void sendMessages(const std::string& uri, const std::string destinationName, int count) {
        Pointer<ActiveMQConnectionFactory> connectionFactory(new ActiveMQConnectionFactory(uri));
        Pointer<Connection> connection(connectionFactory->createConnection());
        Pointer<Session> session(connection->createSession(Session::AUTO_ACKNOWLEDGE));
        Pointer<Destination> destination(session->createQueue(destinationName));
        Pointer<MessageProducer> producer(session->createProducer(destination.get()));
        for(int i = 0; i < count; ++i) {
            Pointer<TextMessage> message(session->createTextMessage());
            producer->send(message.get());
        }
        connection->close();
    }

    void destroyDestination(const std::string& uri, const std::string destinationName) {
        Pointer<ActiveMQConnectionFactory> connectionFactory(new ActiveMQConnectionFactory(uri));
        Pointer<Connection> connection(connectionFactory->createConnection());
        Pointer<Session> session(connection->createSession(Session::AUTO_ACKNOWLEDGE));
        Pointer<Destination> destination(session->createQueue(destinationName));
        Pointer<ActiveMQConnection> amqCon = connection.dynamicCast<ActiveMQConnection>();
        amqCon->destroyDestination(destination.get());
        connection->close();
    }

    bool assertTrue(LinkedHashSet< Pointer<MessageId> >& set, int expected) {
        for (int i = 0; i <= 60; ++i) {
            if (set.size() == expected) {
                return true;
            }

            Thread::sleep(1000);
        }

        return false;
    }

    class TestProducer : public Thread {
    private:

        std::string destinationName;
        std::string brokerUri;
        int produceMessages;

    public:

        TestProducer(const std::string& brokerUri,
                     const std::string& destinationName,
                     int produceMessages) : Thread(),
                                            destinationName(destinationName),
                                            brokerUri(brokerUri),
                                            produceMessages(produceMessages) {
        }

        void run() {

            Pointer<ActiveMQConnectionFactory> connectionFactory;
            Pointer<Connection> connection;
            Pointer<Session> session;
            Pointer<Destination> destination;

            try {

                connectionFactory.reset(new ActiveMQConnectionFactory(brokerUri));
                connection.reset(connectionFactory->createConnection());
                connection->start();
                session.reset(connection->createSession(Session::AUTO_ACKNOWLEDGE));

                destination.reset(session->createQueue(destinationName));

                // Create a MessageProducer from the Session to the Topic or Queue
                Pointer<MessageProducer> producer(session->createProducer(destination.get()));
                producer->setDeliveryMode(cms::DeliveryMode::NON_PERSISTENT);

                for (int i = 0; i < produceMessages; i++) {
                    Pointer<TextMessage> message(session->createTextMessage());
                    message->setLongProperty("TestTime", System::currentTimeMillis());
                    try {
                        producer->send(message.get());
                    } catch (Exception& deeperException) {
                    }

                    Thread::sleep(50);
                }
            } catch (Exception& e) {
            }

            try {
                if (connection != NULL) {
                    connection->close();
                }
            } catch (Exception& e) {
            }
        }
    };

    class TestConsumer : public Thread, public MessageListener {
    private:

        std::string brokerUri;
        std::string destinationName;
        CountDownLatch totalMessages;
        int expected;
        int receivedCount;
        bool rolledBack;
        bool failed;
        LinkedList<int>* messages;
        Pointer<ActiveMQConnectionFactory> connectionFactory;
        Pointer<Connection> connection;
        Pointer<Session> session;
        Pointer<MessageConsumer> consumer;

    public:

        TestConsumer(const std::string& brokerUri,
                     const std::string& destinationName,
                     LinkedList<int>* messages,
                     int totalMessages) : Thread(),
                                          brokerUri(brokerUri),
                                          destinationName(destinationName),
                                          totalMessages(totalMessages),
                                          expected(totalMessages),
                                          receivedCount(0),
                                          rolledBack(false),
                                          failed(false),
                                          messages(messages),
                                          connectionFactory(),
                                          connection(),
                                          session(),
                                          consumer() {
        }

        bool isFailed() const {
            return this->failed;
        }

        virtual void run() {
            try {

                connectionFactory.reset(new ActiveMQConnectionFactory(brokerUri));
                connection.reset(connectionFactory->createConnection());
                session.reset(connection->createSession(Session::SESSION_TRANSACTED));

                Pointer<ActiveMQConnection> amqCon = connection.dynamicCast<ActiveMQConnection>();

                RedeliveryPolicy* policy = amqCon->getRedeliveryPolicy();
                policy->setInitialRedeliveryDelay(1000);
                policy->setBackOffMultiplier(-1);
                policy->setRedeliveryDelay(1000);
                policy->setUseExponentialBackOff(false);
                policy->setMaximumRedeliveries(10);

                Pointer<Destination> destination(session->createQueue(destinationName));
                consumer.reset(session->createConsumer(destination.get()));
                consumer->setMessageListener(this);

                connection->start();

                if (!totalMessages.await(10, TimeUnit::MINUTES)) {
                    this->failed = true;
                }

            } catch (Exception& e) {
            }
            try {
                if (connection != NULL) {
                    connection->close();
                }
            } catch (Exception& e) {
            }
        }

        virtual void onMessage(const cms::Message* message) {
            receivedCount++;

            try {

                const commands::Message* amqMessage =
                    dynamic_cast<const commands::Message*>(message);

                if (!rolledBack) {
                    if (++receivedCount == expected / 2) {
                        rolledBack = true;
                        session->rollback();
                    }
                } else {
                    Pointer<MessageId> msgId = amqMessage->getMessageId();
                    messages->add((int)msgId->getProducerSequenceId());
                    session->commit();
                    totalMessages.countDown();
                }

            } catch (Exception& ex) {
                this->failed = true;
            }
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
OpenwireNonBlockingRedeliveryTest::OpenwireNonBlockingRedeliveryTest() {
}

////////////////////////////////////////////////////////////////////////////////
OpenwireNonBlockingRedeliveryTest::~OpenwireNonBlockingRedeliveryTest() {
}

////////////////////////////////////////////////////////////////////////////////
std::string OpenwireNonBlockingRedeliveryTest::getBrokerURL() const {
    return activemq::util::IntegrationCommon::getInstance().getOpenwireURL() +
        "?connection.nonBlockingRedelivery=true";
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireNonBlockingRedeliveryTest::testConsumerMessagesAreNotOrdered() {

    LinkedList<int> messages;

    const std::string DEST_NAME = "QUEUE.FOO";

    destroyDestination(getBrokerURL(), DEST_NAME);

    TestProducer producer(getBrokerURL(), DEST_NAME, 100);
    TestConsumer consumer(getBrokerURL(), DEST_NAME, &messages, 100);

    producer.start();
    consumer.start();

    producer.join();
    consumer.join();

    CPPUNIT_ASSERT(!consumer.isFailed());

    bool ordered = true;
    int lastId = 0;
    Pointer<Iterator<int> > sequenceIds(messages.iterator());
    while (sequenceIds->hasNext()) {
        int id = sequenceIds->next();
        if (id != (lastId + 1)) {
            ordered = false;
        }

        lastId = id;
    }

    CPPUNIT_ASSERT(!ordered);
    destroyDestination(getBrokerURL(), DEST_NAME);
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class ReceivedListener : public cms::MessageListener {
    private:

        LinkedHashSet< Pointer<MessageId> >* received;

    public:

        ReceivedListener(LinkedHashSet< Pointer<MessageId> >* received) :
            cms::MessageListener(), received(received) {
        }

        virtual ~ReceivedListener() {
        }

        virtual void onMessage(const cms::Message* message) {
            const commands::Message* amqMessage =
                dynamic_cast<const commands::Message*>(message);

            received->add(amqMessage->getMessageId());
        }

    };
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireNonBlockingRedeliveryTest::testMessageDeleiveredWhenNonBlockingEnabled() {

    LinkedHashSet< Pointer<MessageId> > received;
    LinkedHashSet< Pointer<MessageId> > beforeRollback;
    LinkedHashSet< Pointer<MessageId> > afterRollback;

    const int MSG_COUNT = 100;
    const std::string destinationName = "testMessageDeleiveredWhenNonBlockingEnabled";

    destroyDestination(getBrokerURL(), destinationName);

    Pointer<ActiveMQConnectionFactory> connectionFactory(new ActiveMQConnectionFactory(getBrokerURL()));
    Pointer<Connection> connection(connectionFactory->createConnection());
    Pointer<Session> session(connection->createSession(Session::SESSION_TRANSACTED));
    Pointer<Destination> destination(session->createQueue(destinationName));
    Pointer<MessageConsumer> consumer(session->createConsumer(destination.get()));

    ReceivedListener receivedListener(&received);
    consumer->setMessageListener(&receivedListener);
    sendMessages(getBrokerURL(), destinationName, MSG_COUNT);

    connection->start();

    CPPUNIT_ASSERT_MESSAGE("Pre-Rollack received size incorrect", assertTrue(received, MSG_COUNT));

    beforeRollback.addAll(received);
    received.clear();
    session->rollback();

    CPPUNIT_ASSERT_MESSAGE("Post-Rollack received size incorrect", assertTrue(received, MSG_COUNT));

    afterRollback.addAll(received);
    received.clear();

    CPPUNIT_ASSERT_EQUAL(beforeRollback.size(), afterRollback.size());
    CPPUNIT_ASSERT(beforeRollback.equals(afterRollback));
    session->commit();
    connection->close();
    destroyDestination(getBrokerURL(), destinationName);
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireNonBlockingRedeliveryTest::testMessageRedeliveriesAreInOrder() {

    LinkedHashSet< Pointer<MessageId> > received;
    LinkedHashSet< Pointer<MessageId> > beforeRollback;
    LinkedHashSet< Pointer<MessageId> > afterRollback;

    const int MSG_COUNT = 100;
    const std::string destinationName = "testMessageDeleiveredWhenNonBlockingEnabled";

    destroyDestination(getBrokerURL(), destinationName);

    Pointer<ActiveMQConnectionFactory> connectionFactory(new ActiveMQConnectionFactory(getBrokerURL()));
    Pointer<Connection> connection(connectionFactory->createConnection());
    Pointer<Session> session(connection->createSession(Session::SESSION_TRANSACTED));
    Pointer<Destination> destination(session->createQueue(destinationName));
    Pointer<MessageConsumer> consumer(session->createConsumer(destination.get()));

    ReceivedListener receivedListener(&received);
    consumer->setMessageListener(&receivedListener);
    sendMessages(getBrokerURL(), destinationName, MSG_COUNT);

    connection->start();

    CPPUNIT_ASSERT_MESSAGE("Pre-Rollack received size incorrect", assertTrue(received, MSG_COUNT));

    beforeRollback.addAll(received);
    received.clear();
    session->rollback();

    CPPUNIT_ASSERT_MESSAGE("Post-Rollack received size incorrect", assertTrue(received, MSG_COUNT));

    afterRollback.addAll(received);
    received.clear();

    CPPUNIT_ASSERT_EQUAL(beforeRollback.size(), afterRollback.size());
    CPPUNIT_ASSERT(beforeRollback.equals(afterRollback));

    Pointer< Iterator<Pointer<MessageId> > > after(afterRollback.iterator());
    Pointer< Iterator<Pointer<MessageId> > > before(beforeRollback.iterator());

    while (before->hasNext() && after->hasNext()) {
        Pointer<MessageId> original = before->next();
        Pointer<MessageId> rolledBack = after->next();

        long long originalSeq = original->getProducerSequenceId();
        long long rolledbackSeq = rolledBack->getProducerSequenceId();

        CPPUNIT_ASSERT_EQUAL(originalSeq, rolledbackSeq);
    }

    session->commit();
    connection->close();
    destroyDestination(getBrokerURL(), destinationName);
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireNonBlockingRedeliveryTest::testMessageDeleiveryDoesntStop() {

    LinkedHashSet< Pointer<MessageId> > received;
    LinkedHashSet< Pointer<MessageId> > beforeRollback;
    LinkedHashSet< Pointer<MessageId> > afterRollback;

    const int MSG_COUNT = 100;
    const std::string destinationName = "testMessageDeleiveryDoesntStop";

    destroyDestination(getBrokerURL(), destinationName);

    Pointer<ActiveMQConnectionFactory> connectionFactory(new ActiveMQConnectionFactory(getBrokerURL()));
    Pointer<Connection> connection(connectionFactory->createConnection());
    Pointer<Session> session(connection->createSession(Session::SESSION_TRANSACTED));
    Pointer<Destination> destination(session->createQueue(destinationName));
    Pointer<MessageConsumer> consumer(session->createConsumer(destination.get()));

    ReceivedListener receivedListener(&received);
    consumer->setMessageListener(&receivedListener);
    sendMessages(getBrokerURL(), destinationName, MSG_COUNT);

    connection->start();

    CPPUNIT_ASSERT_MESSAGE("Pre-Rollack received size incorrect", assertTrue(received, MSG_COUNT));

    beforeRollback.addAll(received);
    received.clear();
    session->rollback();

    sendMessages(getBrokerURL(), destinationName, MSG_COUNT);

    CPPUNIT_ASSERT_MESSAGE("Post-Rollack received size incorrect", assertTrue(received, MSG_COUNT * 2));

    afterRollback.addAll(received);
    received.clear();

    CPPUNIT_ASSERT_EQUAL(beforeRollback.size() * 2, afterRollback.size());
    session->commit();
    connection->close();

    destroyDestination(getBrokerURL(), destinationName);
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireNonBlockingRedeliveryTest::testNonBlockingMessageDeleiveryIsDelayed() {

    LinkedHashSet< Pointer<MessageId> > received;

    const int MSG_COUNT = 100;
    const std::string destinationName = "testNonBlockingMessageDeleiveryIsDelayed";

    destroyDestination(getBrokerURL(), destinationName);

    Pointer<ActiveMQConnectionFactory> connectionFactory(new ActiveMQConnectionFactory(getBrokerURL()));
    connectionFactory->getRedeliveryPolicy()->setInitialRedeliveryDelay(TimeUnit::SECONDS.toMillis(10));

    Pointer<Connection> connection(connectionFactory->createConnection());
    Pointer<Session> session(connection->createSession(Session::SESSION_TRANSACTED));
    Pointer<Destination> destination(session->createQueue(destinationName));
    Pointer<MessageConsumer> consumer(session->createConsumer(destination.get()));

    ReceivedListener receivedListener(&received);
    consumer->setMessageListener(&receivedListener);
    sendMessages(getBrokerURL(), destinationName, MSG_COUNT);

    connection->start();

    CPPUNIT_ASSERT_MESSAGE("Pre-Rollack received size incorrect", assertTrue(received, MSG_COUNT));

    received.clear();
    session->rollback();

    TimeUnit::SECONDS.sleep(6);
    CPPUNIT_ASSERT_MESSAGE("Rollback redelivery was not delayed.", received.isEmpty());

    CPPUNIT_ASSERT_MESSAGE("Post-Rollack received size incorrect", assertTrue(received, MSG_COUNT));

    session->commit();
    connection->close();

    destroyDestination(getBrokerURL(), destinationName);
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class SomeRollbacksListener : public cms::MessageListener {
    private:

        int count;
        Pointer<Session> session;
        LinkedHashSet< Pointer<MessageId> >* received;

    public:

        SomeRollbacksListener(Pointer<Session> session, LinkedHashSet< Pointer<MessageId> >* received) :
            cms::MessageListener(), count(0), session(session), received(received) {
        }

        virtual ~SomeRollbacksListener() {}

        virtual void onMessage(const cms::Message* message) {
            const commands::Message* amqMessage =
                dynamic_cast<const commands::Message*>(message);

            if (++count > 10) {
                try {
                    session->rollback();
                    count = 0;
                } catch (CMSException& e) {
                }
            } else {
                received->add(amqMessage->getMessageId());
                try {
                    session->commit();
                } catch (CMSException& e) {
                }
            }
        }

    };
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireNonBlockingRedeliveryTest::testNonBlockingMessageDeleiveryWithRollbacks() {

    LinkedHashSet< Pointer<MessageId> > received;

    const int MSG_COUNT = 100;
    const std::string destinationName = "testNonBlockingMessageDeleiveryWithRollbacks";

    destroyDestination(getBrokerURL(), destinationName);

    Pointer<ActiveMQConnectionFactory> connectionFactory(new ActiveMQConnectionFactory(getBrokerURL()));
    connectionFactory->getRedeliveryPolicy()->setInitialRedeliveryDelay(TimeUnit::SECONDS.toMillis(10));

    Pointer<Connection> connection(connectionFactory->createConnection());
    Pointer<Session> session(connection->createSession(Session::SESSION_TRANSACTED));
    Pointer<Destination> destination(session->createQueue(destinationName));
    Pointer<MessageConsumer> consumer(session->createConsumer(destination.get()));

    ReceivedListener receivedListener(&received);
    consumer->setMessageListener(&receivedListener);
    sendMessages(getBrokerURL(), destinationName, MSG_COUNT);

    connection->start();

    CPPUNIT_ASSERT_MESSAGE("Pre-Rollack received size incorrect", assertTrue(received, MSG_COUNT));

    received.clear();

    SomeRollbacksListener newListener(session, &received);
    consumer->setMessageListener(&newListener);

    session->rollback();

    CPPUNIT_ASSERT_MESSAGE("Post-Rollack received size incorrect", assertTrue(received, MSG_COUNT));

    session->commit();
    connection->close();

    destroyDestination(getBrokerURL(), destinationName);
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class RollbacksListener : public cms::MessageListener {
    private:

        Pointer<Session> session;

    public:

        RollbacksListener(Pointer<Session> session) :
            cms::MessageListener(), session(session) {
        }

        virtual ~RollbacksListener() {
        }

        virtual void onMessage(const cms::Message* message) {
            try {
                session->rollback();
            } catch (CMSException& e) {
            }
        }

    };
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireNonBlockingRedeliveryTest::testNonBlockingMessageDeleiveryWithAllRolledBack() {

    LinkedHashSet< Pointer<MessageId> > received;
    LinkedHashSet< Pointer<MessageId> > dlqed;

    const int MSG_COUNT = 100;
    const std::string destinationName = "testNonBlockingMessageDeleiveryWithAllRolledBack";

    destroyDestination(getBrokerURL(), destinationName);
    destroyDestination(getBrokerURL(), "ActiveMQ.DLQ");

    Pointer<ActiveMQConnectionFactory> connectionFactory(new ActiveMQConnectionFactory(getBrokerURL()));
    connectionFactory->getRedeliveryPolicy()->setMaximumRedeliveries(5);
    connectionFactory->getRedeliveryPolicy()->setInitialRedeliveryDelay(TimeUnit::SECONDS.toMillis(5));

    Pointer<Connection> connection(connectionFactory->createConnection());
    Pointer<Session> session(connection->createSession(Session::SESSION_TRANSACTED));
    Pointer<Destination> destination(session->createQueue(destinationName));
    Pointer<Destination> dlq(session->createQueue("ActiveMQ.DLQ"));
    Pointer<MessageConsumer> consumer(session->createConsumer(destination.get()));
    Pointer<MessageConsumer> dlqConsumer(session->createConsumer(dlq.get()));

    ReceivedListener dlqReceivedListener(&dlqed);
    dlqConsumer->setMessageListener(&dlqReceivedListener);

    ReceivedListener receivedListener(&received);
    consumer->setMessageListener(&receivedListener);

    sendMessages(getBrokerURL(), destinationName, MSG_COUNT);
    connection->start();

    CPPUNIT_ASSERT_MESSAGE("Pre-Rollack received size incorrect", assertTrue(received, MSG_COUNT));

    session->rollback();

    RollbacksListener rollbackListener(session);
    consumer->setMessageListener(&rollbackListener);

    CPPUNIT_ASSERT_MESSAGE("Post-Rollack DQL size incorrect", assertTrue(dlqed, MSG_COUNT));

    session->commit();
    connection->close();

    destroyDestination(getBrokerURL(), destinationName);
}
