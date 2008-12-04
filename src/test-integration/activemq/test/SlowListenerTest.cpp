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

#include "SlowListenerTest.h"

#include <decaf/lang/Thread.h>
#include <decaf/util/concurrent/Mutex.h>
#include <decaf/util/Date.h>
#include <decaf/util/Set.h>

#include <activemq/exceptions/ActiveMQException.h>

using namespace std;
using namespace cms;
using namespace activemq;
using namespace activemq::test;
using namespace activemq::util;
using namespace activemq::exceptions;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::util;
using namespace decaf::util::concurrent;

////////////////////////////////////////////////////////////////////////////////
namespace activemq{
namespace test{

    class SlowListener: public cms::MessageListener {
    public:

        unsigned int count;
        decaf::util::Set<long long> threadIds;

        SlowListener() { count = 0; }

        void onMessage( const cms::Message* message ) {

            synchronized( &threadIds ) {
                count++;
                threadIds.add( Thread::getId() );
            }

            Thread::sleep( 20 );
        }
    };

}}

////////////////////////////////////////////////////////////////////////////////
void SlowListenerTest::testSlowListener() {

    try {

        SlowListener listener;

        cms::Session* session = cmsProvider->getSession();

        cms::MessageProducer* producer = cmsProvider->getProducer();
        producer->setDeliveryMode( DeliveryMode::NON_PERSISTENT );

        const unsigned int numConsumers = 5;
        cms::MessageConsumer* consumers[numConsumers];

        // Create several consumers for the same destination.
        for( unsigned int i = 0; i < numConsumers; i++ ) {
            consumers[i] = session->createConsumer( cmsProvider->getDestination() );
            consumers[i]->setMessageListener( &listener );
        }

        auto_ptr<cms::BytesMessage> message( session->createBytesMessage() );

        unsigned int msgCount = 50;
        for( unsigned int i = 0; i < msgCount; i++ ) {
            producer->send( message.get() );
        }

        // Wait no more than 10 seconds for all the messages to come in.
        waitForMessages( msgCount * numConsumers, 10000, &listener );

        synchronized( &listener.threadIds ) {

            // Make sure that the listener was always accessed by the same thread
            // and that it received all the messages from all consumers.
            CPPUNIT_ASSERT_EQUAL( 1, (int)listener.threadIds.size() );
            CPPUNIT_ASSERT_EQUAL( (msgCount * numConsumers), listener.count );
        }

        for( unsigned int i = 0; i < numConsumers; i++ ) {
            delete consumers[i];
        }

    } catch( ActiveMQException& ex ) {
        ex.printStackTrace();
        throw ex;
    }
}

////////////////////////////////////////////////////////////////////////////////
void SlowListenerTest::waitForMessages(
    unsigned int count, long long maxWaitTime, SlowListener* l ) {

    long long startTime = Date::getCurrentTimeMilliseconds();

    synchronized( &( l->threadIds ) ) {

        while( l->count < count ) {

            long long curTime = Date::getCurrentTimeMilliseconds();
            if( ( curTime - startTime ) >= maxWaitTime ) {
                return;
            }

            l->threadIds.wait( 500 );
        }
    }
}
