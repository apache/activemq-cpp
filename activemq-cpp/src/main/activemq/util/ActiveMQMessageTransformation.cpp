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

#include "ActiveMQMessageTransformation.h"

#include <cms/Topic.h>
#include <cms/TemporaryTopic.h>
#include <cms/Queue.h>
#include <cms/TemporaryQueue.h>
#include <cms/Message.h>
#include <cms/BytesMessage.h>
#include <cms/MapMessage.h>
#include <cms/StreamMessage.h>
#include <cms/ObjectMessage.h>
#include <activemq/core/ActiveMQConnection.h>
#include <activemq/commands/ActiveMQDestination.h>
#include <activemq/commands/ActiveMQQueue.h>
#include <activemq/commands/ActiveMQTopic.h>
#include <activemq/commands/ActiveMQTempQueue.h>
#include <activemq/commands/ActiveMQTempTopic.h>
#include <activemq/commands/ActiveMQMessage.h>
#include <activemq/commands/ActiveMQMapMessage.h>
#include <activemq/commands/ActiveMQTextMessage.h>
#include <activemq/commands/ActiveMQStreamMessage.h>
#include <activemq/commands/ActiveMQBlobMessage.h>
#include <activemq/commands/ActiveMQBytesMessage.h>
#include <activemq/commands/ActiveMQObjectMessage.h>
#include <activemq/commands/Message.h>

using namespace activemq;
using namespace activemq::util;
using namespace activemq::core;
using namespace activemq::commands;

////////////////////////////////////////////////////////////////////////////////
ActiveMQMessageTransformation::ActiveMQMessageTransformation() {
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQMessageTransformation::~ActiveMQMessageTransformation() {
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQMessageTransformation::transformDestination(const cms::Destination* destination, const ActiveMQDestination** amqDestination) {

    if (amqDestination == NULL) {
        throw cms::CMSException("Provided target ActiveMQMessage pointer was NULL");
    }

    *amqDestination = dynamic_cast<const ActiveMQDestination*>(destination);

    if (*amqDestination == NULL) {

        if (dynamic_cast<const cms::TemporaryQueue*>(destination) != NULL) {
            *amqDestination = new ActiveMQTempQueue(
                dynamic_cast<const cms::TemporaryQueue*>(destination)->getQueueName());
        } else if (dynamic_cast<const cms::TemporaryTopic*>(destination) != NULL) {
            *amqDestination = new ActiveMQTempTopic(
                dynamic_cast<const cms::TemporaryTopic*>(destination)->getTopicName());
        } else if (dynamic_cast<const cms::Queue*>(destination) != NULL) {
            *amqDestination = new ActiveMQQueue(
                dynamic_cast<const cms::Queue*>(destination)->getQueueName());
        } else if (dynamic_cast<const cms::Topic*>(destination) != NULL) {
            *amqDestination = new ActiveMQTopic(
                dynamic_cast<const cms::Topic*>(destination)->getTopicName());
        }
    } else {
        return false;
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQMessageTransformation::transformMessage(cms::Message* message, ActiveMQConnection* connection, Message** amqMessage) {

    if (amqMessage == NULL) {
        throw cms::CMSException("Provided target ActiveMQMessage pointer was NULL");
    }

    *amqMessage = dynamic_cast<Message*>(message);

    if (*amqMessage != NULL) {
        return false;
    } else {

        if (dynamic_cast<cms::BytesMessage*>(message) != NULL) {
            cms::BytesMessage* bytesMsg = dynamic_cast<cms::BytesMessage*>(message);
            bytesMsg->reset();
            ActiveMQBytesMessage* msg = new ActiveMQBytesMessage();
            msg->setConnection(connection);
            try {
                for (;;) {
                    // Reads a byte from the message stream until the stream is empty
                    msg->writeByte(bytesMsg->readByte());
                }
            } catch (cms::MessageEOFException& e) {
                // if an end of message stream as expected
            } catch (cms::CMSException& e) {
            }

//            *amqMessage = msg;
        } else if (dynamic_cast<cms::MapMessage*>(message) != NULL) {
//            cms::MapMessage* mapMsg = dynamic_cast<cms::MapMessage*>(message);
//            ActiveMQMapMessage* msg = new ActiveMQMapMessage();
//            msg->setConnection(connection);
            // TODO Need type infos for map elements
//            Enumeration iter = mapMsg->getMapNames();
//
//            while (iter.hasMoreElements()) {
//                String name = iter.nextElement().toString();
//                msg.setObject(name, mapMsg.getObject(name));
//            }
//
//            *amqMessage = msg;
        } else if (dynamic_cast<cms::ObjectMessage*>(message) != NULL) {
//            cms::ObjectMessage* objMsg = dynamic_cast<cms::ObjectMessage*>(message);
            ActiveMQObjectMessage* msg = new ActiveMQObjectMessage();
            msg->setConnection(connection);
//            *amqMessage = msg;
        } else if (dynamic_cast<cms::StreamMessage*>(message) != NULL) {
//            cms::StreamMessage* streamMessage = dynamic_cast<cms::StreamMessage*>(message);
//            streamMessage->reset();
//            ActiveMQStreamMessage* msg = new ActiveMQStreamMessage();
//            msg->setConnection(connection);
//            Object obj = NULL;

            // TODO Need element enumeration for StreamMessage
//            try {
//                while ((obj = streamMessage->readObject()) != NULL) {
//                    msg->writeObject(obj);
//                }
//            } catch (MessageEOFException e) {
//                // if an end of message stream as expected
//            } catch (JMSException e) {
//            }

//            *amqMessage = msg;
        } else if (dynamic_cast<cms::TextMessage*>(message) != NULL) {
            cms::TextMessage* textMsg = dynamic_cast<cms::TextMessage*>(message);
            ActiveMQTextMessage* msg = new ActiveMQTextMessage();
            msg->setConnection(connection);
            msg->setText(textMsg->getText());
//            *amqMessage = msg;
        } else {
            *amqMessage = new ActiveMQMessage();
            (*amqMessage)->setConnection(connection);
        }

        ActiveMQMessageTransformation::copyProperties(message, dynamic_cast<cms::Message*>(*amqMessage));
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMessageTransformation::copyProperties(const cms::Message* fromMessage, cms::Message* toMessage) {

    toMessage->setCMSMessageID(fromMessage->getCMSMessageID());
    toMessage->setCMSCorrelationID(fromMessage->getCMSCorrelationID());
//    toMessage->setCMSReplyTo(transformDestination(fromMessage->getCMSReplyTo()));
//    toMessage->setCMSDestination(transformDestination(fromMessage->getCMSDestination()));
//    toMessage->setCMSDeliveryMode(fromMessage->getCMSDeliveryMode());
//    toMessage->setCMSRedelivered(fromMessage->getCMSRedelivered());
//    toMessage->setCMSType(fromMessage->getCMSType());
//    toMessage->setCMSExpiration(fromMessage->getCMSExpiration());
//    toMessage->setCMSPriority(fromMessage->getCMSPriority());
//    toMessage->setCMSTimestamp(fromMessage->getCMSTimestamp());
//
//    std::vector<std::string> propertyNames = fromMessage->getPropertyNames();
//
//    std::vector<std::string>::const_iterator iter = propertyNames.begin();
//    for(; iter != propertyNames.end(); ++iter) {
//        std::string name = *iter;
        // TODO Need type values for properties
//        Object obj = fromMessage.getObjectProperty(name);
//        toMessage->setObjectProperty(name, obj);
//    }
}
