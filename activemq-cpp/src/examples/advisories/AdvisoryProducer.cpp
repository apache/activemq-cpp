/**
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "AdvisoryProducer.h"

#include <cms/Topic.h>
#include <cms/Message.h>
#include <cms/TextMessage.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/Integer.h>

using namespace std;
using namespace activemqcpp;
using namespace activemqcpp::examples;
using namespace activemqcpp::examples::advisories;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
AdvisoryProducer::AdvisoryProducer( cms::Session* session ) : consumerOnline(false),
                                                              shutdown(false),
                                                              shutdownLatch(1),
                                                              session(session),
                                                              consumer(),
                                                              producer() {

    if( session == NULL ) {
        throw NullPointerException(
            __FILE__, __LINE__, "Session Object passed was Null." );
    }

    std::auto_ptr<cms::Topic> destination( session->createTopic(
        "HEART-BEAT-CHANNEL" ) );
    std::auto_ptr<cms::Topic> advisories( session->createTopic(
        "ActiveMQ.Advisory.Consumer.Topic.HEART-BEAT-CHANNEL" ) );

    this->producer.reset( session->createProducer( destination.get() ) );
    this->consumer.reset( session->createConsumer( advisories.get() ) );
    this->consumer->setMessageListener( this );
}

////////////////////////////////////////////////////////////////////////////////
AdvisoryProducer::~AdvisoryProducer() {
}

////////////////////////////////////////////////////////////////////////////////
void AdvisoryProducer::stop() {
    this->shutdown = true;
    this->shutdownLatch.await( 3000 );
}

////////////////////////////////////////////////////////////////////////////////
void AdvisoryProducer::run() {

    while( !this->shutdown ) {

        if( this->consumerOnline ) {

            std::auto_ptr<cms::TextMessage> message(
                this->session->createTextMessage( "Alive" ) );

            this->producer->send( message.get() );

            Thread::sleep( 1000 );
        }
    }

    this->shutdownLatch.countDown();
}

////////////////////////////////////////////////////////////////////////////////
void AdvisoryProducer::onMessage( const cms::Message* message ) {

    if( message->getCMSType() == "Advisory" ) {

        std::cout << "Received an Advisory Message!" << std::endl;

        if( message->propertyExists( "consumerCount" ) ) {

            std::string consumerCount = message->getStringProperty( "consumerCount" );
            std::cout << "Number of Consumers = " << consumerCount << std::endl;
            this->consumerOnline = Integer::parseInt( consumerCount ) > 0 ? true : false;
        }

    } else {
        std::cout << "Received a Non-Advisory Message!" << std::endl;
    }
}
