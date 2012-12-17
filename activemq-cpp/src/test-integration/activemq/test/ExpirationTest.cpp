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

#include "ExpirationTest.h"

#include <decaf/lang/Runnable.h>
#include <decaf/lang/Thread.h>
#include <decaf/lang/Long.h>
#include <decaf/util/UUID.h>

#include <sstream>

using namespace std;
using namespace cms;
using namespace activemq;
using namespace activemq::test;
using namespace activemq::util;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::util;

////////////////////////////////////////////////////////////////////////////////
namespace activemq {
namespace test {

    class Producer : public decaf::lang::Runnable {
    private:

        auto_ptr<CMSProvider> cmsProvider;
        int numMessages;
        long long timeToLive;
        bool disableTimeStamps;

    public:

        Producer(const std::string& brokerURL, const std::string& destination,
                 int numMessages, long long timeToLive ) : Runnable(),
                                                           cmsProvider(),
                                                           numMessages(numMessages),
                                                           timeToLive(timeToLive),
                                                           disableTimeStamps(false) {

            this->cmsProvider.reset( new CMSProvider( brokerURL ) );
            this->cmsProvider->setDestinationName( destination );
            this->cmsProvider->setTopic( false );
        }

        virtual ~Producer(){
        }

        virtual bool getDisableTimeStamps() const {
            return this->disableTimeStamps;
        }

        virtual void setDisableTimeStamps( bool value ){
            this->disableTimeStamps = value;
        }

        virtual void run() {
            try {

                cms::Session* session = cmsProvider->getSession();
                cms::MessageProducer* producer = cmsProvider->getProducer();
                producer->setDeliveryMode( DeliveryMode::NON_PERSISTENT );
                producer->setDisableMessageTimeStamp( disableTimeStamps );

                if( !this->disableTimeStamps ) {
                    producer->setTimeToLive( timeToLive );
                }

                // Create the Thread Id String
                string threadIdStr = Long::toString( Thread::currentThread()->getId() );

                // Create a messages
                string text = (string)"Hello world! from thread " + threadIdStr;

                for( int ix=0; ix<numMessages; ++ix ){
                    TextMessage* message = session->createTextMessage( text );
                    producer->send( message );
                    delete message;
                }

            } catch ( CMSException& e ) {
                e.printStackTrace();
            }
        }
    };

    class Consumer : public cms::MessageListener, public decaf::lang::Runnable {
    private:

        auto_ptr<CMSProvider> cmsProvider;
        long waitMillis;
        int numReceived;

    public:

        Consumer( const std::string& brokerURL, const std::string& destination, long waitMillis ) :
            Runnable(), cmsProvider(), waitMillis(waitMillis), numReceived(0) {

            this->cmsProvider.reset( new CMSProvider( brokerURL ) );
            this->cmsProvider->setTopic( false );
            this->cmsProvider->setDestinationName( destination );
        }

        virtual ~Consumer() {}

        virtual int getNumReceived() const{
            return numReceived;
        }

        virtual void run(){

            try {

                cms::MessageConsumer* consumer = cmsProvider->getConsumer();
                consumer->setMessageListener( this );

                // Sleep while asynchronous messages come in.
                Thread::sleep( waitMillis );

            } catch (CMSException& e) {
                e.printStackTrace();
            }
        }

        virtual void onMessage( const cms::Message* message ) {

            try{
                const TextMessage* textMessage =
                    dynamic_cast< const TextMessage* >( message );
                textMessage->getText();
                numReceived++;
            } catch( CMSException& e ) {
                e.printStackTrace();
            }
        }
    };
}}

////////////////////////////////////////////////////////////////////////////////
void ExpirationTest::testExpired() {

    string destination = UUID::randomUUID().toString();
    Producer producer( this->getBrokerURL(), destination, 1, 1 );
    Thread producerThread( &producer );
    producerThread.start();
    producerThread.join();

    Consumer consumer( this->getBrokerURL(), destination, 2000 );
    Thread consumerThread( &consumer );
    consumerThread.start();
    consumerThread.join();

    CPPUNIT_ASSERT_EQUAL( 0, consumer.getNumReceived() );
}

////////////////////////////////////////////////////////////////////////////////
void ExpirationTest::testNotExpired() {

    string destination = UUID::randomUUID().toString();
    Producer producer( this->getBrokerURL(), destination, 2, 2000 );
    producer.setDisableTimeStamps( true );
    Thread producerThread( &producer );
    producerThread.start();
    producerThread.join();

    Consumer consumer( this->getBrokerURL(), destination, 3000 );
    Thread consumerThread( &consumer );
    consumerThread.start();
    consumerThread.join();

    CPPUNIT_ASSERT_EQUAL( 2, consumer.getNumReceived() );
}
