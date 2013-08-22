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

#include "CmsSendWithAsyncCallbackTest.h"

#include <cms/ConnectionFactory.h>
#include <cms/Connection.h>
#include <cms/Destination.h>
#include <cms/Session.h>
#include <cms/DeliveryMode.h>
#include <cms/MessageConsumer.h>

#include <activemq/util/IntegrationCommon.h>
#include <activemq/core/ActiveMQConnectionFactory.h>
#include <activemq/core/ActiveMQProducer.h>
#include <activemq/exceptions/ActiveMQException.h>

#include <decaf/util/concurrent/CountDownLatch.h>
#include <decaf/lang/Thread.h>

using namespace std;
using namespace cms;
using namespace activemq;
using namespace activemq::test;
using namespace activemq::core;
using namespace activemq::exceptions;
using namespace decaf;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace decaf::lang;

////////////////////////////////////////////////////////////////////////////////
namespace {

    std::auto_ptr<cms::ConnectionFactory> factory;
    std::auto_ptr<cms::Connection> connection;
    std::auto_ptr<cms::Destination> destination;

    class MyMessageListener: public cms::MessageListener {
    public:

        virtual ~MyMessageListener() {
        }

        virtual void onMessage(const cms::Message* message) {

        }
    };

    class MyAsyncCallback : public cms::AsyncCallback {
    private:

        CountDownLatch* latch;

    private:

        MyAsyncCallback(const MyAsyncCallback&);
        MyAsyncCallback& operator= (const MyAsyncCallback&);

    public:

        MyAsyncCallback(CountDownLatch* latch) : cms::AsyncCallback(), latch(latch) {}
        virtual ~MyAsyncCallback() {}

        virtual void onSuccess() {
            latch->countDown();
        }
        virtual void onException(const cms::CMSException& ex) {
            ex.printStackTrace();
        }
    };

    double benchmarkNonCallbackRate(int count) {
        std::auto_ptr<Session> session(connection->createSession(Session::AUTO_ACKNOWLEDGE));

        std::auto_ptr<ActiveMQProducer> producer(
                dynamic_cast<ActiveMQProducer*>(session->createProducer(destination.get())));
        producer->setDeliveryMode(DeliveryMode::PERSISTENT);

        long long start = System::currentTimeMillis();

        for (int i = 0; i < count; i++) {
            std::auto_ptr<cms::TextMessage> message(session->createTextMessage("Hello"));
            producer->send(message.get());
        }

        return 1000.0 * count / (double)((System::currentTimeMillis() - start));
    }

    double benchmarkCallbackRate(int count) {
        std::auto_ptr<Session> session(connection->createSession(Session::AUTO_ACKNOWLEDGE));
        CountDownLatch messagesSent(count);
        MyAsyncCallback onComplete(&messagesSent);

        std::auto_ptr<ActiveMQProducer> producer(
                dynamic_cast<ActiveMQProducer*>(session->createProducer(destination.get())));
        producer->setDeliveryMode(DeliveryMode::PERSISTENT);

        long long start = System::currentTimeMillis();

        for (int i = 0; i < count; i++) {
            std::auto_ptr<cms::TextMessage> message(session->createTextMessage("Hello"));
            producer->send(message.get(), &onComplete);
        }

        messagesSent.await();
        return 1000.0 * count / (double)((System::currentTimeMillis() - start));
    }
}

////////////////////////////////////////////////////////////////////////////////
CmsSendWithAsyncCallbackTest::CmsSendWithAsyncCallbackTest() {

}

////////////////////////////////////////////////////////////////////////////////
CmsSendWithAsyncCallbackTest::~CmsSendWithAsyncCallbackTest() {

}

////////////////////////////////////////////////////////////////////////////////
void CmsSendWithAsyncCallbackTest::setUp() {

    factory.reset(ConnectionFactory::createCMSConnectionFactory(getBrokerURL()));
    connection.reset(factory->createConnection());
    std::auto_ptr<cms::Session> session(connection->createSession(Session::AUTO_ACKNOWLEDGE));
    destination.reset(session->createQueue("CmsSendWithAsyncCallbackTest"));

    session->close();
}

////////////////////////////////////////////////////////////////////////////////
void CmsSendWithAsyncCallbackTest::tearDown() {

    factory.reset(NULL);
    connection.reset(NULL);
    destination.reset(NULL);
}

////////////////////////////////////////////////////////////////////////////////
void CmsSendWithAsyncCallbackTest::testAsyncCallbackIsFaster() {

    MyMessageListener listener;

    connection->start();

    std::auto_ptr<Session> session(connection->createSession(Session::AUTO_ACKNOWLEDGE));

    // setup a consumer to drain messages..
    std::auto_ptr<MessageConsumer> consumer(session->createConsumer(destination.get()));
    consumer->setMessageListener(&listener);

    // warmup...
    benchmarkNonCallbackRate(20);
    benchmarkCallbackRate(20);

    double callbackRate = benchmarkCallbackRate(30);
    double nonCallbackRate = benchmarkNonCallbackRate(30);

    CPPUNIT_ASSERT(callbackRate > 0);
    CPPUNIT_ASSERT(nonCallbackRate > 0);
}
