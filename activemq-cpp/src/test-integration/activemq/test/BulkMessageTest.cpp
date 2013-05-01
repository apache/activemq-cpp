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

#include "BulkMessageTest.h"

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

        std::auto_ptr<activemq::util::CMSProvider> cmsProducerProvider;
        int num;
        int size;

    private:

        ProducerThread(const ProducerThread&);
        ProducerThread& operator= (const ProducerThread&);

    public:

        ProducerThread(const std::string& brokerURL, const std::string& destinationName, const std::string& subscription, int num, int size) :
            cmsProducerProvider(new activemq::util::CMSProvider(brokerURL, destinationName, subscription)), num(num), size(size) {
        }

        virtual ~ProducerThread() {}

        virtual void run() {

            cms::Session* session( cmsProducerProvider->getSession() );
            Destination* destination = cmsProducerProvider->getDestination();
            Pointer<MessageProducer> producer( session->createProducer( destination ) );
            producer->setDeliveryMode( cms::DeliveryMode::NON_PERSISTENT );

            std::string DATA = "abcdefghijklmnopqrstuvwxyz";
            std::string body = "";
            for( int i=0; i < size; i ++) {
                body += DATA.at(i % DATA.length());
            }

            Pointer<BytesMessage> message;

            for( int i = 0; i < num; ++i ) {
                message.reset( session->createBytesMessage( (const unsigned char*) body.c_str(), body.length() ) );
                producer->send( message.get() );
            }
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
BulkMessageTest::BulkMessageTest() {
}

////////////////////////////////////////////////////////////////////////////////
BulkMessageTest::~BulkMessageTest() {
}

////////////////////////////////////////////////////////////////////////////////
void BulkMessageTest::testBulkMessageSendReceive() {

    static const int MSG_COUNT = 5000;
    static const int MSG_SIZE = 8192;

    // Create CMS Object for consumer Comms
    cms::Session* session( cmsProvider->getSession() );
    cms::MessageConsumer* consumer = cmsProvider->getConsumer();
    Destination* destination = cmsProvider->getDestination();

    ProducerThread thread( this->getBrokerURL(), cmsProvider->getDestinationName(), cmsProvider->getSubscription(), MSG_COUNT, MSG_SIZE );
    thread.start();

    Pointer<cms::Message> message;

    for( int i = 0; i < MSG_COUNT ; ++i ) {
        CPPUNIT_ASSERT_NO_THROW( message.reset( consumer->receive( 2000 ) ) );
        CPPUNIT_ASSERT( message.get() != NULL );
    }
}
