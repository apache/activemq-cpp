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

#include <decaf/lang/Thread.h>
#include <decaf/lang/Runnable.h>
#include <decaf/util/concurrent/CountDownLatch.h>
#include <decaf/lang/Long.h>
#include <decaf/util/Date.h>
#include <activemq/core/ActiveMQConnectionFactory.h>
#include <activemq/util/Config.h>
#include <activemq/library/ActiveMQCPP.h>
#include <activemq/cmsutil/CmsTemplate.h>
#include <activemq/cmsutil/MessageCreator.h>
#include <cms/Connection.h>
#include <cms/Session.h>
#include <cms/TextMessage.h>
#include <cms/BytesMessage.h>
#include <cms/MapMessage.h>
#include <cms/ExceptionListener.h>
#include <cms/MessageListener.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <memory>

using namespace activemq;
using namespace activemq::core;
using namespace activemq::cmsutil;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace cms;
using namespace std;

////////////////////////////////////////////////////////////////////////////////
namespace{

    class TextMessageCreator: public activemq::cmsutil::MessageCreator {
    public:

        TextMessageCreator() : MessageCreator() {}

        virtual ~TextMessageCreator() {}

        virtual cms::Message* createMessage( cms::Session* session ) {

            cms::Message* message = NULL;

            if( session != NULL ) {
                message = session->createTextMessage("test text message");
            }

            return message;
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
int main(int argc AMQCPP_UNUSED, char* argv[] AMQCPP_UNUSED) {

    std::cout << "=====================================================\n";
    std::cout << "Starting the MessageSender application..." << std::endl;

    // initialize ActiveMQCPP library
    activemq::library::ActiveMQCPP::initializeLibrary();

    ActiveMQConnectionFactory* connectionFactory =
        new ActiveMQConnectionFactory( "failover://(tcp://127.0.0.1:61616)" );

    CmsTemplate* cmsTemplate = new CmsTemplate( connectionFactory );
    cmsTemplate->setDefaultDestinationName( "CMSTemplateExamples" );
    cmsTemplate->setPubSubDomain( true );

    TextMessageCreator* textMessageCreator = new TextMessageCreator();

    for( int i = 0; i < 100; i++ ) {

        std::cout << "Iteration " << i << ". Sending 3 messages..." << std::endl;

        for( int j = 0; j < 3; j++ ) {

            try {
                cmsTemplate->send( textMessageCreator );
                std::cout << "Sent a message successfully." << endl;
            } catch( cms::CMSException& ex ) {
                std::cout << "Caught CMSException:  ";
                std::cout << ex.getMessage().c_str() << endl;
            }
        }

        std::cout << "Sleeping 15000 milliseconds..." << std::endl;
        Thread::sleep( 15000 );
    }

    std::cout << "Shutting down MessageSender..." << std::endl;

    //shut down ActiveMQCPP library
    delete textMessageCreator;
    delete cmsTemplate;
    delete connectionFactory;

    // terminates the ActiveMQCPP library
    activemq::library::ActiveMQCPP::shutdownLibrary();
}
