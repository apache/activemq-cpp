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

#include "OpenwireTempDestinationTest.h"

#include <decaf/lang/Thread.h>
#include <decaf/util/concurrent/Mutex.h>
#include <decaf/util/concurrent/CountDownLatch.h>
#include <decaf/util/UUID.h>
#include <activemq/exceptions/ActiveMQException.h>

using namespace std;
using namespace cms;
using namespace activemq;
using namespace activemq::test;
using namespace activemq::test::openwire;
using namespace activemq::util;
using namespace activemq::exceptions;
using namespace decaf;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace decaf::lang;

namespace activemq{
namespace test{
namespace openwire{

    class Requester : public cms::MessageListener,
                      public decaf::lang::Runnable {
    private:

        auto_ptr<CMSProvider> cmsProvider;
        auto_ptr<cms::MessageConsumer> tempTopicConsumer;

        unsigned int numReceived;
        unsigned int messageCount;

        decaf::util::concurrent::CountDownLatch ready;
        decaf::util::concurrent::CountDownLatch responses;

    public:

        Requester( const std::string& url,
                   const std::string& destination,
                   unsigned int messageCount )
        : messageCount( messageCount ), ready( 1 ), responses( messageCount ) {

            this->cmsProvider.reset( new CMSProvider( url ) );
            this->cmsProvider->setDestinationName( destination );

            this->cmsProvider->getProducer()->setDeliveryMode( DeliveryMode::NON_PERSISTENT );

            this->tempTopicConsumer.reset(
                cmsProvider->getSession()->createConsumer(
                    cmsProvider->getTempDestination() ) );
            this->tempTopicConsumer->setMessageListener( this );

            this->numReceived = 0;
        }

        virtual ~Requester() {}

        virtual unsigned int getNumReceived() const {
            return this->numReceived;
        }

        virtual void waitUnitReady() {
            this->ready.await();
        }

        virtual void awaitAllResponses() {
            this->responses.await( 2000 * this->messageCount );
        }

        virtual void run() {

            try {

                auto_ptr<cms::TextMessage> message(
                    this->cmsProvider->getSession()->createTextMessage() );
                message->setCMSReplyTo( this->cmsProvider->getTempDestination() );

                this->ready.countDown();

                for( unsigned int i = 0; i < messageCount; ++i ) {
                    this->cmsProvider->getProducer()->send( message.get() );
                }

            } catch( CMSException& e ) {
                e.printStackTrace();
            }
        }

        virtual void onMessage( const cms::Message* message ) {

            try {

                this->numReceived++;
                this->responses.countDown();

            } catch( CMSException& e ) {
                e.printStackTrace();
            }
        }
    };

    class Responder : public cms::MessageListener {
    private:

        auto_ptr<CMSProvider> cmsProvider;

        unsigned int numReceived;
        unsigned int messageCount;

        decaf::util::concurrent::CountDownLatch requests;

    public:

        Responder( const std::string& url,
                   const std::string& destination,
                   unsigned int messageCount )
        : messageCount( messageCount ), requests( messageCount ) {

            this->cmsProvider.reset( new CMSProvider( url ) );

            this->cmsProvider->setDestinationName( destination );
            this->cmsProvider->getNoDestProducer()->setDeliveryMode(
                DeliveryMode::NON_PERSISTENT );
            this->cmsProvider->getConsumer()->setMessageListener( this );

            this->numReceived = 0;
        }

        virtual ~Responder() {}

        virtual unsigned int getNumReceived() const {
            return this->numReceived;
        }

        virtual void awaitAllRequests() {
            this->requests.await( 2000 * this->messageCount );
        }

        virtual void onMessage( const cms::Message* message ) {

            try {

                if( message->getCMSReplyTo() != NULL ) {

                    auto_ptr<cms::Message> response(
                        cmsProvider->getSession()->createMessage() );

                    // Send it back to the replyTo Destination
                    this->cmsProvider->getNoDestProducer()->send(
                        message->getCMSReplyTo(), response.get() );

                    this->requests.countDown();
                }

                this->numReceived++;

            } catch( CMSException& e ) {
                e.printStackTrace();
            }
        }
    };

}}}

///////////////////////////////////////////////////////////////////////////////
void OpenwireTempDestinationTest::testBasics() {

    try{

        auto_ptr<cms::MessageConsumer> tempConsumer(
            cmsProvider->getSession()->createConsumer(
                cmsProvider->getTempDestination() ) );

        auto_ptr<TextMessage> message(
            cmsProvider->getSession()->createTextMessage() );

        // Fire a message to the temporary topic
        cmsProvider->getNoDestProducer()->send(
            cmsProvider->getTempDestination(), message.get() );

        auto_ptr<cms::Message> received( tempConsumer->receive( 3000 ) );

        CPPUNIT_ASSERT( received.get() != NULL );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

///////////////////////////////////////////////////////////////////////////////
void OpenwireTempDestinationTest::testTwoConnections() {

    try {

        std::string destination = "REQUEST-TOPIC";

        auto_ptr<Requester> requester(
            new Requester( cmsProvider->getBrokerURL(), destination, 10 ) );
        auto_ptr<Responder> responder(
            new Responder( cmsProvider->getBrokerURL(), destination, 10 ) );

        // Launch the Consumers in new Threads.
        Thread requestorThread( requester.get() );
        requestorThread.start();

        // Responder should get all its requests first
        responder->awaitAllRequests();

        // Now the Requester should get all its responses.
        requester->awaitAllResponses();

        // Check that the responder received all the required requests
        CPPUNIT_ASSERT( responder->getNumReceived() == 10 );

        // Check that the requester received all the required responses
        CPPUNIT_ASSERT( requester->getNumReceived() == 10 );

        // Shutdown the Requester.
        requestorThread.join();
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}
