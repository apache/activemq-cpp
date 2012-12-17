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

#include <decaf/lang/exceptions/NullPointerException.h>

using namespace activemq;
using namespace activemq::util;
using namespace activemq::core;
using namespace activemq::commands;

using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
ActiveMQMessageTransformation::ActiveMQMessageTransformation() {
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQMessageTransformation::~ActiveMQMessageTransformation() {
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQMessageTransformation::transformDestination(const cms::Destination* destination, const ActiveMQDestination** amqDestination) {

    if (destination == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "Provided source cms::Destination pointer was NULL");
    }

    if (amqDestination == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "Provided target ActiveMQDestination pointer was NULL");
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

    if (message == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "Provided source cms::Message pointer was NULL");
    }

    if (amqMessage == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "Provided target commands::Message pointer was NULL");
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

            *amqMessage = msg;
        } else if (dynamic_cast<cms::MapMessage*>(message) != NULL) {
            cms::MapMessage* mapMsg = dynamic_cast<cms::MapMessage*>(message);
            ActiveMQMapMessage* msg = new ActiveMQMapMessage();
            msg->setConnection(connection);

            std::vector<std::string> elements = mapMsg->getMapNames();
            std::vector<std::string>::iterator iter = elements.begin();
            for(; iter != elements.end() ; ++iter) {
                std::string key = *iter;
                cms::Message::ValueType elementType = mapMsg->getValueType(key);

                switch(elementType) {
                    case cms::Message::BOOLEAN_TYPE:
                        msg->setBoolean(key, mapMsg->getBoolean(key));
                        break;
                    case cms::Message::BYTE_TYPE:
                        msg->setByte(key, mapMsg->getByte(key));
                        break;
                    case cms::Message::BYTE_ARRAY_TYPE:
                        msg->setBytes(key, mapMsg->getBytes(key));
                        break;
                    case cms::Message::CHAR_TYPE:
                        msg->setChar(key, mapMsg->getChar(key));
                        break;
                    case cms::Message::SHORT_TYPE:
                        msg->setShort(key, mapMsg->getShort(key));
                        break;
                    case cms::Message::INTEGER_TYPE:
                        msg->setInt(key, mapMsg->getInt(key));
                        break;
                    case cms::Message::LONG_TYPE:
                        msg->setLong(key, mapMsg->getLong(key));
                        break;
                    case cms::Message::FLOAT_TYPE:
                        msg->setFloat(key, mapMsg->getFloat(key));
                        break;
                    case cms::Message::DOUBLE_TYPE:
                        msg->setDouble(key, mapMsg->getDouble(key));
                        break;
                    case cms::Message::STRING_TYPE:
                        msg->setString(key, mapMsg->getString(key));
                        break;
                    default:
                        break;
                }
            }

            *amqMessage = msg;
        } else if (dynamic_cast<cms::ObjectMessage*>(message) != NULL) {
            cms::ObjectMessage* objMsg = dynamic_cast<cms::ObjectMessage*>(message);
            ActiveMQObjectMessage* msg = new ActiveMQObjectMessage();
            msg->setConnection(connection);
            msg->setObjectBytes(objMsg->getObjectBytes());
            *amqMessage = msg;
        } else if (dynamic_cast<cms::StreamMessage*>(message) != NULL) {
            cms::StreamMessage* streamMessage = dynamic_cast<cms::StreamMessage*>(message);
            streamMessage->reset();
            ActiveMQStreamMessage* msg = new ActiveMQStreamMessage();
            msg->setConnection(connection);

            try {
                while(true) {
                    cms::Message::ValueType elementType = streamMessage->getNextValueType();
                    int result = -1;
                    std::vector<unsigned char> buffer(255);

                    switch(elementType) {
                        case cms::Message::BOOLEAN_TYPE:
                            msg->writeBoolean(streamMessage->readBoolean());
                            break;
                        case cms::Message::BYTE_TYPE:
                            msg->writeBoolean(streamMessage->readBoolean());
                            break;
                        case cms::Message::BYTE_ARRAY_TYPE:
                            while ((result = streamMessage->readBytes(buffer)) != -1) {
                                msg->writeBytes(&buffer[0], 0, result);
                                buffer.clear();
                            }
                            break;
                        case cms::Message::CHAR_TYPE:
                            msg->writeChar(streamMessage->readChar());
                            break;
                        case cms::Message::SHORT_TYPE:
                            msg->writeShort(streamMessage->readShort());
                            break;
                        case cms::Message::INTEGER_TYPE:
                            msg->writeInt(streamMessage->readInt());
                            break;
                        case cms::Message::LONG_TYPE:
                            msg->writeLong(streamMessage->readLong());
                            break;
                        case cms::Message::FLOAT_TYPE:
                            msg->writeFloat(streamMessage->readFloat());
                            break;
                        case cms::Message::DOUBLE_TYPE:
                            msg->writeDouble(streamMessage->readDouble());
                            break;
                        case cms::Message::STRING_TYPE:
                            msg->writeString(streamMessage->readString());
                            break;
                        default:
                            break;
                    }
                }
            } catch (cms::MessageEOFException& e) {
                // if an end of message stream as expected
            } catch (cms::CMSException& e) {
            }

            *amqMessage = msg;
        } else if (dynamic_cast<cms::TextMessage*>(message) != NULL) {
            cms::TextMessage* textMsg = dynamic_cast<cms::TextMessage*>(message);
            ActiveMQTextMessage* msg = new ActiveMQTextMessage();
            msg->setConnection(connection);
            msg->setText(textMsg->getText());
            *amqMessage = msg;
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

    if (fromMessage == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "Provided source cms::Message pointer was NULL");
    }

    if (toMessage == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "Provided destination cms::Message pointer was NULL");
    }

    if (fromMessage->getCMSDestination() != NULL) {
        const ActiveMQDestination* transformed = NULL;
        if (transformDestination(fromMessage->getCMSDestination(), &transformed)) {
            toMessage->setCMSDestination(dynamic_cast<const cms::Destination*>(transformed));
            delete transformed;
        } else {
            toMessage->setCMSDestination(fromMessage->getCMSDestination());
        }
    }

    if (fromMessage->getCMSReplyTo() != NULL) {
        const ActiveMQDestination* transformed = NULL;
        if (transformDestination(fromMessage->getCMSReplyTo(), &transformed)) {
            toMessage->setCMSReplyTo(dynamic_cast<const cms::Destination*>(transformed));
            delete transformed;
        } else {
            toMessage->setCMSReplyTo(fromMessage->getCMSReplyTo());
        }
    }

    toMessage->setCMSMessageID(fromMessage->getCMSMessageID());
    toMessage->setCMSCorrelationID(fromMessage->getCMSCorrelationID());
    toMessage->setCMSDeliveryMode(fromMessage->getCMSDeliveryMode());
    toMessage->setCMSRedelivered(fromMessage->getCMSRedelivered());
    toMessage->setCMSType(fromMessage->getCMSType());
    toMessage->setCMSExpiration(fromMessage->getCMSExpiration());
    toMessage->setCMSPriority(fromMessage->getCMSPriority());
    toMessage->setCMSTimestamp(fromMessage->getCMSTimestamp());

    std::vector<std::string> propertyNames = fromMessage->getPropertyNames();
    std::vector<std::string>::iterator iter = propertyNames.begin();
    for(; iter != propertyNames.end() ; ++iter) {
        std::string name = *iter;
        cms::Message::ValueType propertyType = fromMessage->getPropertyValueType(name);

        switch(propertyType) {
            case cms::Message::BOOLEAN_TYPE:
                toMessage->setBooleanProperty(name, fromMessage->getBooleanProperty(name));
                break;
            case cms::Message::BYTE_TYPE:
                toMessage->setByteProperty(name, fromMessage->getByteProperty(name));
                break;
            case cms::Message::SHORT_TYPE:
                toMessage->setShortProperty(name, fromMessage->getShortProperty(name));
                break;
            case cms::Message::INTEGER_TYPE:
                toMessage->setIntProperty(name, fromMessage->getIntProperty(name));
                break;
            case cms::Message::LONG_TYPE:
                toMessage->setLongProperty(name, fromMessage->getLongProperty(name));
                break;
            case cms::Message::FLOAT_TYPE:
                toMessage->setFloatProperty(name, fromMessage->getFloatProperty(name));
                break;
            case cms::Message::DOUBLE_TYPE:
                toMessage->setDoubleProperty(name, fromMessage->getDoubleProperty(name));
                break;
            case cms::Message::STRING_TYPE:
                toMessage->setStringProperty(name, fromMessage->getStringProperty(name));
                break;
            default:
                break;
        }
    }
}
