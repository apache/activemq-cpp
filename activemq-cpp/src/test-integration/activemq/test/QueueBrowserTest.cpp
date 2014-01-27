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

#include "QueueBrowserTest.h"

#include <activemq/core/ActiveMQConnectionFactory.h>
#include <activemq/core/ActiveMQConnection.h>
#include <activemq/util/CMSListener.h>
#include <activemq/exceptions/ActiveMQException.h>

#include <cms/Message.h>
#include <cms/TextMessage.h>
#include <cms/Session.h>

#include <decaf/lang/Integer.h>

#include <memory>

using namespace activemq;
using namespace activemq::test;
using namespace activemq::core;
using namespace decaf;
using namespace decaf::lang;

////////////////////////////////////////////////////////////////////////////////
QueueBrowserTest::QueueBrowserTest() {
}

////////////////////////////////////////////////////////////////////////////////
QueueBrowserTest::~QueueBrowserTest() {
}

////////////////////////////////////////////////////////////////////////////////
void QueueBrowserTest::testReceiveBrowseReceive() {

    cms::Session* session(cmsProvider->getSession());

    std::auto_ptr<cms::Queue> queue(session->createQueue("testReceiveBrowseReceive"));

    std::auto_ptr<cms::MessageConsumer> consumer(session->createConsumer(queue.get()));
    std::auto_ptr<cms::MessageProducer> producer(session->createProducer(queue.get()));

    std::auto_ptr<cms::TextMessage> message1(session->createTextMessage("First Message"));
    std::auto_ptr<cms::TextMessage> message2(session->createTextMessage("Second Message"));
    std::auto_ptr<cms::TextMessage> message3(session->createTextMessage("Third Message"));

    // lets consume any outstanding messages from previous test runs
    cms::Message* message;
    while ((message = consumer->receive(1000)) != NULL) {
        delete message;
    }

    producer->send(message1.get());
    producer->send(message2.get());
    producer->send(message3.get());

    // Get the first.
    std::auto_ptr<cms::TextMessage> inbound(dynamic_cast<cms::TextMessage*>(consumer->receive(1000)));
    CPPUNIT_ASSERT(inbound.get() != NULL);
    CPPUNIT_ASSERT_EQUAL(message1->getText(), inbound->getText());
    consumer->close();

    std::auto_ptr<cms::QueueBrowser> browser(session->createBrowser(queue.get()));
    cms::MessageEnumeration* enumeration = browser->getEnumeration();

    // browse the second
    CPPUNIT_ASSERT_MESSAGE("should have received the second message", enumeration->hasMoreMessages());
    inbound.reset(dynamic_cast<cms::TextMessage*>(enumeration->nextMessage()));
    CPPUNIT_ASSERT(inbound.get() != NULL);
    CPPUNIT_ASSERT_EQUAL(message2->getText(), inbound->getText());

    // browse the third.
    CPPUNIT_ASSERT_MESSAGE("should have received the third message", enumeration->hasMoreMessages());
    inbound.reset(dynamic_cast<cms::TextMessage*>(enumeration->nextMessage()));
    CPPUNIT_ASSERT(inbound.get() != NULL);
    CPPUNIT_ASSERT_EQUAL(message3->getText(), inbound->getText());

    // There should be no more.
    bool tooMany = false;
    while (enumeration->hasMoreMessages()) {
        tooMany = true;
    }
    CPPUNIT_ASSERT_MESSAGE("Should not have browsed any more messages", !tooMany);
    browser->close();

    // Re-open the consumer
    consumer.reset(session->createConsumer(queue.get()));
    // Receive the second.
    inbound.reset(dynamic_cast<cms::TextMessage*>(consumer->receive(1000)));
    CPPUNIT_ASSERT(inbound.get() != NULL);
    CPPUNIT_ASSERT_EQUAL(message2->getText(), inbound->getText());
    // Receive the third.
    inbound.reset(dynamic_cast<cms::TextMessage*>(consumer->receive(1000)));
    CPPUNIT_ASSERT(inbound.get() != NULL);
    CPPUNIT_ASSERT_EQUAL(message3->getText(), inbound->getText());

    consumer->close();
    browser->close();
    producer->close();
    cmsProvider->close();
}

////////////////////////////////////////////////////////////////////////////////
void QueueBrowserTest::testBrowseReceive() {

    std::auto_ptr<cms::TextMessage> inbound;

    cms::Session* session(cmsProvider->getSession());
    std::auto_ptr<cms::Queue> queue(session->createQueue("testBrowseReceive"));
    std::auto_ptr<cms::TextMessage> message1(session->createTextMessage("First Message"));
    std::auto_ptr<cms::MessageProducer> producer(session->createProducer(queue.get()));

    producer->send(message1.get());

    // create browser first
    std::auto_ptr<cms::QueueBrowser> browser(session->createBrowser(queue.get()));
    cms::MessageEnumeration* enumeration = browser->getEnumeration();

    // create consumer
    std::auto_ptr<cms::MessageConsumer> consumer(session->createConsumer(queue.get()));

    // browse the first message
    CPPUNIT_ASSERT_MESSAGE("should have received the first message", enumeration->hasMoreMessages());
    inbound.reset(dynamic_cast<cms::TextMessage*>(enumeration->nextMessage()));
    CPPUNIT_ASSERT(inbound.get() != NULL);
    CPPUNIT_ASSERT_EQUAL(message1->getText(), inbound->getText());

    // Receive the first message.
    inbound.reset(dynamic_cast<cms::TextMessage*>(consumer->receive(1000)));
    CPPUNIT_ASSERT(inbound.get() != NULL);
    CPPUNIT_ASSERT_EQUAL(message1->getText(), inbound->getText());

    consumer->close();
    browser->close();
    producer->close();
}

////////////////////////////////////////////////////////////////////////////////
void QueueBrowserTest::testQueueBrowserWith2Consumers() {

    static const int numMessages = 100;

    ActiveMQConnection* connection = dynamic_cast<ActiveMQConnection*>(cmsProvider->getConnection());
    CPPUNIT_ASSERT(connection != NULL);
    connection->setAlwaysSyncSend(false);

    std::auto_ptr<cms::Session> session(connection->createSession(cms::Session::CLIENT_ACKNOWLEDGE));
    std::auto_ptr<cms::Queue> queue(session->createQueue("testQueueBrowserWith2Consumers"));

    std::auto_ptr<cms::Queue> queuePrefetch10(
        session->createQueue("testQueueBrowserWith2Consumers?consumer.prefetchSize=10"));
    std::auto_ptr<cms::Queue> queuePrefetch1(
        session->createQueue("testQueueBrowserWith2Consumers?consumer.prefetchSize=1"));

    std::auto_ptr<ActiveMQConnectionFactory> factory(new ActiveMQConnectionFactory(cmsProvider->getBrokerURL()));
    std::auto_ptr<ActiveMQConnection> connection2(dynamic_cast<ActiveMQConnection*>(factory->createConnection()));
    connection2->start();

    std::auto_ptr<cms::Session> session2(connection2->createSession(cms::Session::AUTO_ACKNOWLEDGE));

    std::auto_ptr<cms::MessageProducer> producer(session->createProducer(queue.get()));
    std::auto_ptr<cms::MessageConsumer> consumer(session->createConsumer(queuePrefetch10.get()));

    producer->setDeliveryMode(cms::DeliveryMode::NON_PERSISTENT);

    for (int i = 0; i < numMessages; i++) {
        std::auto_ptr<cms::TextMessage> message(
                session->createTextMessage(std::string("Message: ") + Integer::toString(i)));
        producer->send(message.get());
    }

    std::auto_ptr<cms::QueueBrowser> browser(session2->createBrowser(queuePrefetch1.get()));
    cms::MessageEnumeration* browserView = browser->getEnumeration();

    std::vector<cms::Message*> messages;
    for (int i = 0; i < numMessages; i++) {
        cms::Message* m1 = consumer->receive(5000);
        CPPUNIT_ASSERT_MESSAGE(std::string("m1 is null for index: ") + Integer::toString(i), m1 != NULL);
        messages.push_back(m1);
    }

    for (int i = 0; i < numMessages && browserView->hasMoreMessages(); i++) {
        cms::Message* m1 = messages[i];
        cms::Message* m2 = browserView->nextMessage();
        CPPUNIT_ASSERT_MESSAGE(std::string("m2 is null for index: ") + Integer::toString(i), m2 != NULL);
        CPPUNIT_ASSERT(m1->getCMSMessageID() == m2->getCMSMessageID());
        delete m2;
    }

    CPPUNIT_ASSERT_MESSAGE("nothing left in the browser", !browserView->hasMoreMessages());
    CPPUNIT_ASSERT_MESSAGE("consumer finished", consumer->receiveNoWait() == NULL);

    for (std::size_t ix = 0; ix < messages.size(); ++ix) {
        cms::Message* msg = messages[ix];
        msg->acknowledge();
        delete msg;
    }
}

////////////////////////////////////////////////////////////////////////////////
void QueueBrowserTest::testRepeatedQueueBrowserCreateDestroy() {

    ActiveMQConnection* connection = dynamic_cast<ActiveMQConnection*>(cmsProvider->getConnection());
    CPPUNIT_ASSERT(connection != NULL);

    std::auto_ptr<cms::Session> session(connection->createSession(cms::Session::SESSION_TRANSACTED));
    std::auto_ptr<cms::Queue> queue(session->createTemporaryQueue());

    std::auto_ptr<cms::MessageProducer> producer(session->createProducer(queue.get()));
    std::auto_ptr<cms::TextMessage> textMessage(session->createTextMessage("Test"));
    producer->setDeliveryMode(cms::DeliveryMode::NON_PERSISTENT);
    producer->send(textMessage.get());
    session->commit();

    connection->start();

    std::auto_ptr<cms::QueueBrowser> browser(session->createBrowser(queue.get()));

    for (int i = 0; i < 200; i++) {
        browser.reset(session->createBrowser(queue.get()));
        cms::MessageEnumeration* browserView = browser->getEnumeration();

        if (browserView->hasMoreMessages()) {
            std::auto_ptr<cms::Message> message(browserView->nextMessage());
            CPPUNIT_ASSERT(message.get() != NULL);
        }

        browser.reset(NULL);
    }
}
