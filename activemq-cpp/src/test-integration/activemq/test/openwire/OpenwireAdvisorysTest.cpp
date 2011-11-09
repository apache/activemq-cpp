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

#include "OpenwireAdvisorysTest.h"

#include <activemq/core/ActiveMQConnectionFactory.h>
#include <activemq/core/ActiveMQConnection.h>
#include <activemq/core/ActiveMQSession.h>
#include <activemq/commands/Message.h>
#include <activemq/commands/ConnectionInfo.h>
#include <activemq/exceptions/ActiveMQException.h>

#include <decaf/lang/exceptions/ClassCastException.h>
#include <decaf/lang/Pointer.h>
#include <decaf/lang/Thread.h>
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

using namespace cms;
using namespace std;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::util;
using namespace activemq;
using namespace activemq::core;
using namespace activemq::commands;
using namespace activemq::exceptions;
using namespace activemq::test;
using namespace activemq::test::openwire;

////////////////////////////////////////////////////////////////////////////////
OpenwireAdvisorysTest::OpenwireAdvisorysTest() {
}

////////////////////////////////////////////////////////////////////////////////
OpenwireAdvisorysTest::~OpenwireAdvisorysTest() {
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireAdvisorysTest::testConnectionAdvisories() {

    std::auto_ptr<ConnectionFactory> factory(
        ConnectionFactory::createCMSConnectionFactory( getBrokerURL() ) );
    CPPUNIT_ASSERT( factory.get() != NULL );

    std::auto_ptr<Connection> connection( factory->createConnection() );
    CPPUNIT_ASSERT( connection.get() != NULL );

    std::auto_ptr<Session> session( connection->createSession() );
    CPPUNIT_ASSERT( session.get() != NULL );

    std::auto_ptr<Destination> destination( session->createTopic("ActiveMQ.Advisory.Connection") );
    std::auto_ptr<MessageConsumer> consumer( session->createConsumer( destination.get() ) );

    connection->start();

    std::auto_ptr<Connection> otherConnection( factory->createConnection() );
    CPPUNIT_ASSERT( otherConnection.get() != NULL );
    otherConnection->start();

    std::auto_ptr<cms::Message> message;
    int connectionInfoCount = 0;

    do {
        message.reset( consumer->receive(3000) );

        commands::Message* amqMessage = dynamic_cast<commands::Message*>( message.get() );
        if(amqMessage != NULL) {
            try {
                Pointer<ConnectionInfo> connectionInfo =
                    amqMessage->getDataStructure().dynamicCast<commands::ConnectionInfo>();

                if(connectionInfo != NULL) {
                    connectionInfoCount++;
                }

            } catch(ClassCastException& ex) {
            }
        }

    } while(message.get() != NULL);

    CPPUNIT_ASSERT_EQUAL(2, connectionInfoCount);

    otherConnection->close();
    connection->close();
}

