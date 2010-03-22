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

#include "TempDestinationAdvisoryConsumer.h"

#include <cms/Topic.h>
#include <cms/Message.h>
#include <cms/TextMessage.h>
#include <activemq/core/ActiveMQConstants.h>
#include <activemq/commands/ActiveMQMessage.h>
#include <activemq/commands/DestinationInfo.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/exceptions/ClassCastException.h>
#include <decaf/lang/Integer.h>

using namespace std;
using namespace activemqcpp;
using namespace activemqcpp::examples;
using namespace activemqcpp::examples::advisories;
using namespace activemq;
using namespace activemq::commands;
using namespace activemq::core;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
TempDestinationAdvisoryConsumer::TempDestinationAdvisoryConsumer( cms::Session* session )
    : session( session ), consumer() {

    if( session == NULL ) {
        throw NullPointerException(
            __FILE__, __LINE__, "Session Object passed was Null." );
    }

    std::auto_ptr<cms::Topic> advisories( session->createTopic(
        "ActiveMQ.Advisory.TempTopic,ActiveMQ.Advisory.TempQueue" ) );

    this->consumer.reset( session->createConsumer( advisories.get() ) );
    this->consumer->setMessageListener( this );
}

////////////////////////////////////////////////////////////////////////////////
TempDestinationAdvisoryConsumer::~TempDestinationAdvisoryConsumer() {
}

////////////////////////////////////////////////////////////////////////////////
void TempDestinationAdvisoryConsumer::onMessage( const cms::Message* message ) {

    if( message->getCMSType() == "Advisory" ) {

        std::cout << "Received an Advisory Message!" << std::endl;

        const ActiveMQMessage* amqMessage =
            dynamic_cast<const ActiveMQMessage*>( message );

        if( amqMessage != NULL && amqMessage->getDataStructure() != NULL ) {
            std::cout << "Advisory Message contains a Command Object!" << std::endl;

            try {

                Pointer<DestinationInfo> info =
                    amqMessage->getDataStructure().dynamicCast<DestinationInfo>();

                unsigned char operationType = info->getOperationType();

                if( operationType == ActiveMQConstants::DESTINATION_REMOVE_OPERATION ) {
                    std::cout << "Temporary Destination {"
                              << info->getDestination()->getPhysicalName()
                              << "} Removed."
                              << std::endl;
                } else if( operationType == ActiveMQConstants::DESTINATION_ADD_OPERATION ) {
                    std::cout << "Temporary Destination {"
                              << info->getDestination()->getPhysicalName()
                              << "} Added."
                              << std::endl;
                } else {
                    std::cout << "ERROR: I have no Idea what just happened!"
                              << std::endl;
                }

            } catch( ClassCastException& ex ) {
                std::cout << "ERROR: Expected the Command to be a DestinationInfo, "
                          << "it wasn't so PANIC!!"
                          << std::endl;
            }
        }

    } else {
        std::cout << "Received a Non-Advisory Message!" << std::endl;
    }
}
