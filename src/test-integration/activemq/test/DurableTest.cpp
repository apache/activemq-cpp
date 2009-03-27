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

#include "DurableTest.h"

#include <activemq/util/IntegrationCommon.h>
#include <activemq/exceptions/ActiveMQException.h>

#include <decaf/util/UUID.h>

using namespace std;
using namespace cms;
using namespace activemq;
using namespace activemq::util;
using namespace activemq::test;
using namespace activemq::exceptions;

using namespace decaf;
using namespace decaf::util;

////////////////////////////////////////////////////////////////////////////////
void DurableTest::testDurableConsumer() {

    try {

        // Create CMS Object for Comms
        cms::Session* session( cmsProvider->getSession() );
        cmsProvider->setSubscription( UUID::randomUUID().toString() );
        cmsProvider->setDurable( true );
        cms::MessageConsumer* consumer = cmsProvider->getConsumer();
        cms::MessageProducer* producer = cmsProvider->getProducer();

        // Send a text message to the consumer while its active
        auto_ptr<cms::TextMessage> txtMessage( session->createTextMessage( "TEST MESSAGE" ) );
        producer->send( txtMessage.get() );
        auto_ptr<cms::Message> received( consumer->receive( 3000 ) );

        CPPUNIT_ASSERT( received.get() != NULL );

        cmsProvider->reconnectSession();
        session = cmsProvider->getSession();
        producer = cmsProvider->getProducer();

        // Send some messages while there is no consumer active.
        for( int i = 0; i < MSG_COUNT; ++i ) {
            producer->send( txtMessage.get() );
        }

        consumer = cmsProvider->getConsumer();

        // Send some messages while there is no consumer active.
        for( int i = 0; i < MSG_COUNT; ++i ) {
            producer->send( txtMessage.get() );
        }

        for( int i = 0; i < MSG_COUNT * 2; i++ ) {
            received.reset( consumer->receive( 1000 * 5 ) );

            CPPUNIT_ASSERT_MESSAGE(
                "Failed to receive all messages in batch", received.get() != NULL );
        }

        // Remove the subscription after the consumer is forcibly closed.
        cmsProvider->unsubscribe();
    }
    catch( ActiveMQException& ex ) {
        CPPUNIT_ASSERT_MESSAGE( ex.getStackTraceString(), false );
    }
}
