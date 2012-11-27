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

#include "MessagePriorityTest.h"

#include <activemq/util/CMSListener.h>
#include <activemq/exceptions/ActiveMQException.h>

#include <decaf/lang/Thread.h>
#include <decaf/util/UUID.h>
#include <decaf/lang/Pointer.h>

using namespace std;
using namespace cms;
using namespace activemq;
using namespace activemq::test;
using namespace activemq::util;
using namespace activemq::exceptions;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::util;

////////////////////////////////////////////////////////////////////////////////
namespace {

    class ProducerThread : public Thread {
    private:

        Session* session;
        Destination* destination;
        int num;
        int priority;

    private:

        ProducerThread(const ProducerThread&);
        ProducerThread& operator= (const ProducerThread&);

    public:

        ProducerThread( Session* session, Destination* destination, int num, int priority ) :
            session(session), destination(destination), num(num), priority(priority) {
        }

        virtual ~ProducerThread() {}

        virtual void run() {

            Pointer<MessageProducer> producer( session->createProducer( destination ) );
            producer->setDeliveryMode( cms::DeliveryMode::NON_PERSISTENT );
            producer->setPriority( priority );

            for( int i = 0; i < num; ++i ) {
                Pointer<TextMessage> message( session->createTextMessage( "Test Message") );
                producer->send( message.get() );
            }
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
MessagePriorityTest::MessagePriorityTest() {
}

////////////////////////////////////////////////////////////////////////////////
MessagePriorityTest::~MessagePriorityTest() {
}

////////////////////////////////////////////////////////////////////////////////
void MessagePriorityTest::testMessagePrioritySendReceive() {

    static const int MSG_COUNT = 25;

    // Create CMS Object for Comms
    cms::Session* session( cmsProvider->getSession() );

    cms::MessageConsumer* consumer = cmsProvider->getConsumer();

    Destination* destination = cmsProvider->getDestination();

    ProducerThread thread1( session, destination, MSG_COUNT, 9 );
    ProducerThread thread2( session, destination, MSG_COUNT, 1 );

    thread1.start();
    thread2.start();

    thread1.join();
    thread2.join();

    Thread::sleep( 3000 );

    for( int i = 0; i < MSG_COUNT * 2; ++i ) {
        Pointer<cms::Message> message( consumer->receive( 2000 ) );
        CPPUNIT_ASSERT( message != NULL );
        CPPUNIT_ASSERT( message->getCMSPriority() == ( i < MSG_COUNT ? 9 : 1 ) );
    }
}
