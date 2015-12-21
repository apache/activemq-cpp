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

#include <activemq/core/ActiveMQConnectionFactory.h>
#include <activemq/core/ActiveMQConnection.h>
#include <activemq/core/ActiveMQSession.h>
#include <activemq/commands/Message.h>
#include <activemq/commands/ConnectionInfo.h>
#include <activemq/exceptions/ActiveMQException.h>

#include <decaf/lang/exceptions/ClassCastException.h>
#include <decaf/lang/Pointer.h>
#include <decaf/lang/Thread.h>
#include <decaf/lang/Runnable.h>
#include <decaf/util/concurrent/TimeUnit.h>
#include <decaf/util/UUID.h>

#include <cms/ConnectionFactory.h>
#include <cms/Connection.h>
#include <cms/Session.h>
#include <cms/MessageConsumer.h>
#include <cms/MessageProducer.h>
#include <cms/MessageListener.h>
#include <cms/ConnectionFactory.h>
#include <cms/Connection.h>
#include <cms/Message.h>
#include <cms/TextMessage.h>

#include <memory>
#include "OpenwireAdvisoryTest.h"

using namespace cms;
using namespace std;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace activemq;
using namespace activemq::core;
using namespace activemq::commands;
using namespace activemq::exceptions;
using namespace activemq::test;
using namespace activemq::test::openwire;

////////////////////////////////////////////////////////////////////////////////
OpenwireAdvisoryTest::OpenwireAdvisoryTest() {
}

////////////////////////////////////////////////////////////////////////////////
OpenwireAdvisoryTest::~OpenwireAdvisoryTest() {
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireAdvisoryTest::testConnectionAdvisories() {

    std::auto_ptr<ConnectionFactory> factory(ConnectionFactory::createCMSConnectionFactory(getBrokerURL()));
    CPPUNIT_ASSERT(factory.get() != NULL);

    std::auto_ptr<Connection> connection(factory->createConnection());
    CPPUNIT_ASSERT(connection.get() != NULL);

    std::auto_ptr<Session> session(connection->createSession());
    CPPUNIT_ASSERT(session.get() != NULL);

    std::auto_ptr<Destination> destination(session->createTopic("ActiveMQ.Advisory.Connection"));
    std::auto_ptr<MessageConsumer> consumer(session->createConsumer(destination.get()));

    connection->start();

    std::auto_ptr<Connection> otherConnection(factory->createConnection());
    CPPUNIT_ASSERT(otherConnection.get() != NULL);
    otherConnection->start();

    std::auto_ptr<cms::Message> message;
    int connectionInfoCount = 0;

    do {
        message.reset(consumer->receive(3000));

        commands::Message* amqMessage = dynamic_cast<commands::Message*>(message.get());
        if (amqMessage != NULL) {
            try {
                Pointer<ConnectionInfo> connectionInfo =
                    amqMessage->getDataStructure().dynamicCast<commands::ConnectionInfo>();

                if (connectionInfo != NULL) {
                    connectionInfoCount++;
                }

            } catch (ClassCastException& ex) {
            }
        }

    } while (message.get() != NULL);

    CPPUNIT_ASSERT(connectionInfoCount >= 2);

    otherConnection->close();
    connection->close();
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class ConnectionLoadThread : public Thread {
    private:

        ConnectionFactory* factory;
        bool noErrors;

    public:

        ConnectionLoadThread(ConnectionFactory* factory) :
            Thread(), factory(factory), noErrors(true) {
        }

        virtual ~ConnectionLoadThread() {}

        bool isNoErrors() const {
            return this->noErrors;
        }

        virtual void run() {

            try {
                for (unsigned int i = 0; i < 50; ++i) {
                    auto_ptr<Connection> connection(factory->createConnection());
                    connection->start();
                    auto_ptr<Session> session(connection->createSession(Session::AUTO_ACKNOWLEDGE));

                    for (unsigned int j = 0; j < 100; ++j) {
                        auto_ptr<Queue> queue(session->createTemporaryQueue());
                        auto_ptr<MessageProducer> producer(session->createProducer(queue.get()));
                    }

                    TimeUnit::MILLISECONDS.sleep(20);
                    connection->close();
                }
            } catch(...) {
                noErrors = false;
            }
        }
    };

}

////////////////////////////////////////////////////////////////////////////////
void OpenwireAdvisoryTest::testConcurrentTempDestCreation() {

    std::auto_ptr<ConnectionFactory> factory(
        ConnectionFactory::createCMSConnectionFactory(getBrokerURL()));

    ConnectionLoadThread thread1(factory.get());
    ConnectionLoadThread thread2(factory.get());

    thread1.start();
    thread2.start();

    thread1.join();
    thread2.join();

    CPPUNIT_ASSERT(thread1.isNoErrors());
    CPPUNIT_ASSERT(thread2.isNoErrors());
}
