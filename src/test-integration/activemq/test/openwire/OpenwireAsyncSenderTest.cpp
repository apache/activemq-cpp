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

#include "OpenwireAsyncSenderTest.h"

#include <activemq/core/ActiveMQConnectionFactory.h>
#include <activemq/core/ActiveMQConnection.h>

using namespace std;
using namespace cms;
using namespace activemq;
using namespace activemq::test;
using namespace activemq::test::openwire;
using namespace activemq::core;
using namespace activemq::util;

////////////////////////////////////////////////////////////////////////////////
OpenwireAsyncSenderTest::OpenwireAsyncSenderTest() {
}

////////////////////////////////////////////////////////////////////////////////
OpenwireAsyncSenderTest::~OpenwireAsyncSenderTest() {
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireAsyncSenderTest::testOpenWireConnector() {

    try{

        auto_ptr<ActiveMQConnectionFactory> connectionFactory(
            new ActiveMQConnectionFactory( this->getBrokerURL() ) );
        auto_ptr<cms::Connection> connection( connectionFactory->createConnection() );

        ActiveMQConnection* amqConnection =
            dynamic_cast<ActiveMQConnection*>( connection.get() );
        CPPUNIT_ASSERT( amqConnection != NULL );

        CPPUNIT_ASSERT( amqConnection->isUseAsyncSend() );
        CPPUNIT_ASSERT( !amqConnection->isAlwaysSyncSend() );

        connection->start();
        connection->stop();

        CPPUNIT_ASSERT( true );
    } catch(...) {
        CPPUNIT_ASSERT( false );
    }
}
