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

#include "AdvisoryConsumer.h"

#include <cms/Topic.h>
#include <cms/Message.h>
#include <cms/TextMessage.h>
#include <decaf/lang/exceptions/NullPointerException.h>

#include <activemq/commands/ActiveMQMessage.h>
#include <activemq/commands/ProducerInfo.h>

using namespace std;
using namespace activemq;
using namespace activemq::commands;
using namespace activemqcpp;
using namespace activemqcpp::examples;
using namespace activemqcpp::examples::advisories;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
AdvisoryConsumer::AdvisoryConsumer(cms::Session* session) : session(session), consumer(), advisoryConsumer() {

    if (session == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "Session Object passed was Null.");
    }

    std::auto_ptr<cms::Topic> destination(session->createTopic("HEART-BEAT-CHANNEL"));
    std::auto_ptr<cms::Topic> advisories(
        session->createTopic("ActiveMQ.Advisory.Producer.Topic.HEART-BEAT-CHANNEL"));

    this->consumer.reset(session->createConsumer(destination.get()));
    this->advisoryConsumer.reset(session->createConsumer(advisories.get()));
    this->consumer->setMessageListener(this);
    this->advisoryConsumer->setMessageListener(this);
}

////////////////////////////////////////////////////////////////////////////////
AdvisoryConsumer::~AdvisoryConsumer() {
}

////////////////////////////////////////////////////////////////////////////////
void AdvisoryConsumer::close() {
    this->consumer.reset(NULL);
}

////////////////////////////////////////////////////////////////////////////////
void AdvisoryConsumer::onMessage(const cms::Message* message) {

    if (message->getCMSType() == "Advisory") {

        const ActiveMQMessage* amqMessage = dynamic_cast<const ActiveMQMessage*> (message);

        // If you want you can get the ProducerInfo for instance, you could get
        // the ConsumerInfo and ConnectionInfo.
        if (amqMessage != NULL && amqMessage->getDataStructure() != NULL) {
            const ProducerInfo* info = dynamic_cast<const ProducerInfo*> (amqMessage->getDataStructure().get());

            std::cout << "Got ProducerInfo for producer: " << info->getProducerId()->toString() << std::endl;
        }

        if (message->propertyExists("producerCount")) {
            std::string producerCount = message->getStringProperty("producerCount");
            std::cout << "Number of Producers = " << producerCount << std::endl;
        }

    } else {

        const cms::TextMessage* txtMessage = dynamic_cast<const cms::TextMessage*> (message);

        if (txtMessage != NULL) {
            std::cout << "Producer Reports Status as: " << txtMessage->getText() << std::endl;
        }
    }
}
