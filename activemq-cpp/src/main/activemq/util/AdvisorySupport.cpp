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

#include "AdvisorySupport.h"

#include <activemq/commands/ActiveMQDestination.h>
#include <activemq/commands/ActiveMQTopic.h>
#include <activemq/util/ActiveMQMessageTransformation.h>
#include <activemq/exceptions/ActiveMQException.h>

#include <decaf/lang/exceptions/IllegalArgumentException.h>
#include <decaf/util/ArrayList.h>

using namespace activemq;
using namespace activemq::util;
using namespace activemq::commands;
using namespace decaf::util;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
const std::string AdvisorySupport::ADVISORY_TOPIC_PREFIX = "ActiveMQ.Advisory.";
const std::string AdvisorySupport::PRODUCER_ADVISORY_TOPIC_PREFIX = ADVISORY_TOPIC_PREFIX + "Producer.";
const std::string AdvisorySupport::QUEUE_PRODUCER_ADVISORY_TOPIC_PREFIX = PRODUCER_ADVISORY_TOPIC_PREFIX + "Queue.";
const std::string AdvisorySupport::TOPIC_PRODUCER_ADVISORY_TOPIC_PREFIX = PRODUCER_ADVISORY_TOPIC_PREFIX + "Topic.";
const std::string AdvisorySupport::CONSUMER_ADVISORY_TOPIC_PREFIX = ADVISORY_TOPIC_PREFIX + "Consumer.";
const std::string AdvisorySupport::QUEUE_CONSUMER_ADVISORY_TOPIC_PREFIX = CONSUMER_ADVISORY_TOPIC_PREFIX + "Queue.";
const std::string AdvisorySupport::TOPIC_CONSUMER_ADVISORY_TOPIC_PREFIX = CONSUMER_ADVISORY_TOPIC_PREFIX + "Topic.";
const std::string AdvisorySupport::EXPIRED_TOPIC_MESSAGES_TOPIC_PREFIX = ADVISORY_TOPIC_PREFIX + "Expired.Topic.";
const std::string AdvisorySupport::EXPIRED_QUEUE_MESSAGES_TOPIC_PREFIX = ADVISORY_TOPIC_PREFIX + "Expired.Queue.";
const std::string AdvisorySupport::NO_TOPIC_CONSUMERS_TOPIC_PREFIX = ADVISORY_TOPIC_PREFIX + "NoConsumer.Topic.";
const std::string AdvisorySupport::NO_QUEUE_CONSUMERS_TOPIC_PREFIX = ADVISORY_TOPIC_PREFIX + "NoConsumer.Queue.";
const std::string AdvisorySupport::SLOW_CONSUMER_TOPIC_PREFIX = ADVISORY_TOPIC_PREFIX + "SlowConsumer.";
const std::string AdvisorySupport::FAST_PRODUCER_TOPIC_PREFIX = ADVISORY_TOPIC_PREFIX + "FastProducer.";
const std::string AdvisorySupport::MESSAGE_DISCAREDED_TOPIC_PREFIX = ADVISORY_TOPIC_PREFIX + "MessageDiscarded.";
const std::string AdvisorySupport::FULL_TOPIC_PREFIX = ADVISORY_TOPIC_PREFIX + "FULL.";
const std::string AdvisorySupport::MESSAGE_DELIVERED_TOPIC_PREFIX = ADVISORY_TOPIC_PREFIX + "MessageDelivered.";
const std::string AdvisorySupport::MESSAGE_CONSUMED_TOPIC_PREFIX = ADVISORY_TOPIC_PREFIX + "MessageConsumed.";
const std::string AdvisorySupport::MESSAGE_DLQ_TOPIC_PREFIX = ADVISORY_TOPIC_PREFIX + "MessageDLQd.";
const std::string AdvisorySupport::MASTER_BROKER_TOPIC_PREFIX = ADVISORY_TOPIC_PREFIX + "MasterBroker";
const std::string AdvisorySupport::NETWORK_BRIDGE_TOPIC_PREFIX = ADVISORY_TOPIC_PREFIX + "NetworkBridge";
const std::string AdvisorySupport::AGENT_TOPIC = "ActiveMQ.Agent";
const std::string AdvisorySupport::ADIVSORY_MESSAGE_TYPE = "Advisory";
const std::string AdvisorySupport::MSG_PROPERTY_ORIGIN_BROKER_ID = "originBrokerId";
const std::string AdvisorySupport::MSG_PROPERTY_ORIGIN_BROKER_NAME = "originBrokerName";
const std::string AdvisorySupport::MSG_PROPERTY_ORIGIN_BROKER_URL = "originBrokerURL";
const std::string AdvisorySupport::MSG_PROPERTY_USAGE_NAME = "usageName";
const std::string AdvisorySupport::MSG_PROPERTY_CONSUMER_ID = "consumerId";
const std::string AdvisorySupport::MSG_PROPERTY_PRODUCER_ID = "producerId";
const std::string AdvisorySupport::MSG_PROPERTY_MESSAGE_ID = "orignalMessageId";
const std::string AdvisorySupport::MSG_PROPERTY_CONSUMER_COUNT = "consumerCount";
const std::string AdvisorySupport::MSG_PROPERTY_DISCARDED_COUNT = "discardedCount";

////////////////////////////////////////////////////////////////////////////////
AdvisorySupport::AdvisorySupport() {
}

////////////////////////////////////////////////////////////////////////////////
AdvisorySupport::~AdvisorySupport() {
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQDestination* AdvisorySupport::getTempDestinationCompositeAdvisoryTopic() {

    ActiveMQTopic queues(ADVISORY_TOPIC_PREFIX + "TempQueue");
    ActiveMQTopic topics(ADVISORY_TOPIC_PREFIX + "TempTopic");

    std::string name = queues.getPhysicalName() + "," + topics.getPhysicalName();
    return new ActiveMQTopic(name);
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQDestination* AdvisorySupport::getAllDestinationsCompositeAdvisoryTopic() {

    ActiveMQTopic queues(ADVISORY_TOPIC_PREFIX + "Queue");
    ActiveMQTopic topics(ADVISORY_TOPIC_PREFIX + "Topic");
    ActiveMQTopic tempQueues(ADVISORY_TOPIC_PREFIX + "TempQueue");
    ActiveMQTopic tempTopics(ADVISORY_TOPIC_PREFIX + "TempTopic");

    std::string name = queues.getPhysicalName() + "," + topics.getPhysicalName() + "," +
                       tempQueues.getPhysicalName() + "," + tempTopics.getPhysicalName();
    return new ActiveMQTopic(name);
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQDestination* AdvisorySupport::getConnectionAdvisoryTopic() {
    return new ActiveMQTopic(ADVISORY_TOPIC_PREFIX + "Connection");
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQDestination* AdvisorySupport::getTopicAdvisoryTopic() {
    return new ActiveMQTopic(ADVISORY_TOPIC_PREFIX + "Topic");
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQDestination* AdvisorySupport::getQueueAdvisoryTopic() {
    return new ActiveMQTopic(ADVISORY_TOPIC_PREFIX + "Queue");
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQDestination* AdvisorySupport::getTempTopicAdvisoryTopic() {
    return new ActiveMQTopic(ADVISORY_TOPIC_PREFIX + "TempTopic");
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQDestination* AdvisorySupport::getTempQueueAdvisoryTopic() {
    return new ActiveMQTopic(ADVISORY_TOPIC_PREFIX + "TempQueue");
}


////////////////////////////////////////////////////////////////////////////////
std::vector<ActiveMQDestination*> AdvisorySupport::getAllDestinationAdvisoryTopics(const cms::Destination* destination) {

    const ActiveMQDestination* transformed = NULL;

    bool doDelete = ActiveMQMessageTransformation::transformDestination(destination, &transformed);
    std::vector<ActiveMQDestination*> destinations = getAllDestinationAdvisoryTopics(transformed);

    if (doDelete) {
        delete transformed;
    }

    return destinations;
}

////////////////////////////////////////////////////////////////////////////////
std::vector<ActiveMQDestination*> AdvisorySupport::getAllDestinationAdvisoryTopics(const ActiveMQDestination* destination) {

    std::vector<ActiveMQDestination*> destinations;

    destinations.push_back(getConsumerAdvisoryTopic(destination));
    destinations.push_back(getProducerAdvisoryTopic(destination));
    destinations.push_back(getExpiredMessageTopic(destination));
    destinations.push_back(getNoConsumersAdvisoryTopic(destination));
    destinations.push_back(getSlowConsumerAdvisoryTopic(destination));
    destinations.push_back(getFastProducerAdvisoryTopic(destination));
    destinations.push_back(getMessageDiscardedAdvisoryTopic(destination));
    destinations.push_back(getMessageDeliveredAdvisoryTopic(destination));
    destinations.push_back(getMessageConsumedAdvisoryTopic(destination));
    destinations.push_back(getMessageDLQdAdvisoryTopic(destination));
    destinations.push_back(getFullAdvisoryTopic(destination));

    return destinations;
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQDestination* AdvisorySupport::getConsumerAdvisoryTopic(const cms::Destination* destination) {

    const ActiveMQDestination* transformed = NULL;

    bool doDelete = ActiveMQMessageTransformation::transformDestination(destination, &transformed);
    ActiveMQDestination* advisoryDest = getConsumerAdvisoryTopic(transformed);

    if (doDelete) {
        delete transformed;
    }

    return advisoryDest;
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQDestination* AdvisorySupport::getConsumerAdvisoryTopic(const ActiveMQDestination* destination) {

    if (destination == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "The passed ActiveMQDestination cannot be NULL");
    }

    if (destination->isQueue()) {
        return new ActiveMQTopic(QUEUE_CONSUMER_ADVISORY_TOPIC_PREFIX + destination->getPhysicalName());
    } else {
        return new ActiveMQTopic(TOPIC_CONSUMER_ADVISORY_TOPIC_PREFIX + destination->getPhysicalName());
    }
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQDestination* AdvisorySupport::getProducerAdvisoryTopic(const cms::Destination* destination) {

    const ActiveMQDestination* transformed = NULL;

    bool doDelete = ActiveMQMessageTransformation::transformDestination(destination, &transformed);
    ActiveMQDestination* advisoryDest = getProducerAdvisoryTopic(transformed);

    if (doDelete) {
        delete transformed;
    }

    return advisoryDest;
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQDestination* AdvisorySupport::getProducerAdvisoryTopic(const ActiveMQDestination* destination) {

    if (destination == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "The passed ActiveMQDestination cannot be NULL");
    }

    if (destination->isQueue()) {
        return new ActiveMQTopic(QUEUE_PRODUCER_ADVISORY_TOPIC_PREFIX + destination->getPhysicalName());
    } else {
        return new ActiveMQTopic(TOPIC_PRODUCER_ADVISORY_TOPIC_PREFIX + destination->getPhysicalName());
    }
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQDestination* AdvisorySupport::getExpiredMessageTopic(const cms::Destination* destination) {

    const ActiveMQDestination* transformed = NULL;

    bool doDelete = ActiveMQMessageTransformation::transformDestination(destination, &transformed);
    ActiveMQDestination* advisoryDest = getExpiredMessageTopic(transformed);

    if (doDelete) {
        delete transformed;
    }

    return advisoryDest;
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQDestination* AdvisorySupport::getExpiredMessageTopic(const ActiveMQDestination* destination) {

    if (destination == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "The passed ActiveMQDestination cannot be NULL");
    }

    if (destination->isQueue()) {
        return getExpiredQueueMessageAdvisoryTopic(destination);
    }

    return getExpiredTopicMessageAdvisoryTopic(destination);
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQDestination* AdvisorySupport::getExpiredTopicMessageAdvisoryTopic(const cms::Destination* destination) {

    const ActiveMQDestination* transformed = NULL;

    bool doDelete = ActiveMQMessageTransformation::transformDestination(destination, &transformed);
    ActiveMQDestination* advisoryDest = getExpiredTopicMessageAdvisoryTopic(transformed);

    if (doDelete) {
        delete transformed;
    }

    return advisoryDest;
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQDestination* AdvisorySupport::getExpiredTopicMessageAdvisoryTopic(const ActiveMQDestination* destination) {
    if (destination == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "The passed ActiveMQDestination cannot be NULL");
    }

    return new ActiveMQTopic(EXPIRED_TOPIC_MESSAGES_TOPIC_PREFIX + destination->getPhysicalName());
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQDestination* AdvisorySupport::getExpiredQueueMessageAdvisoryTopic(const cms::Destination* destination) {

    const ActiveMQDestination* transformed = NULL;

    bool doDelete = ActiveMQMessageTransformation::transformDestination(destination, &transformed);
    ActiveMQDestination* advisoryDest = getExpiredQueueMessageAdvisoryTopic(transformed);

    if (doDelete) {
        delete transformed;
    }

    return advisoryDest;
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQDestination* AdvisorySupport::getExpiredQueueMessageAdvisoryTopic(const ActiveMQDestination* destination) {
    if (destination == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "The passed ActiveMQDestination cannot be NULL");
    }

    return new ActiveMQTopic(EXPIRED_QUEUE_MESSAGES_TOPIC_PREFIX + destination->getPhysicalName());
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQDestination* AdvisorySupport::getNoConsumersAdvisoryTopic(const cms::Destination* destination) {

    const ActiveMQDestination* transformed = NULL;

    bool doDelete = ActiveMQMessageTransformation::transformDestination(destination, &transformed);
    ActiveMQDestination* advisoryDest = getNoConsumersAdvisoryTopic(transformed);

    if (doDelete) {
        delete transformed;
    }

    return advisoryDest;
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQDestination* AdvisorySupport::getNoConsumersAdvisoryTopic(const ActiveMQDestination* destination) {

    if (destination == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "The passed ActiveMQDestination cannot be NULL");
    }

    if (destination->isQueue()) {
        return getNoQueueConsumersAdvisoryTopic(destination);
    }
    return getNoTopicConsumersAdvisoryTopic(destination);
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQDestination* AdvisorySupport::getNoTopicConsumersAdvisoryTopic(const cms::Destination* destination) {

    const ActiveMQDestination* transformed = NULL;

    bool doDelete = ActiveMQMessageTransformation::transformDestination(destination, &transformed);
    ActiveMQDestination* advisoryDest = getNoTopicConsumersAdvisoryTopic(transformed);

    if (doDelete) {
        delete transformed;
    }

    return advisoryDest;
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQDestination* AdvisorySupport::getNoTopicConsumersAdvisoryTopic(const ActiveMQDestination* destination) {
    if (destination == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "The passed ActiveMQDestination cannot be NULL");
    }

    return new ActiveMQTopic(NO_TOPIC_CONSUMERS_TOPIC_PREFIX + destination->getPhysicalName());
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQDestination* AdvisorySupport::getNoQueueConsumersAdvisoryTopic(const cms::Destination* destination) {

    const ActiveMQDestination* transformed = NULL;

    bool doDelete = ActiveMQMessageTransformation::transformDestination(destination, &transformed);
    ActiveMQDestination* advisoryDest = getNoQueueConsumersAdvisoryTopic(transformed);

    if (doDelete) {
        delete transformed;
    }

    return advisoryDest;
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQDestination* AdvisorySupport::getNoQueueConsumersAdvisoryTopic(const ActiveMQDestination* destination) {
    if (destination == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "The passed ActiveMQDestination cannot be NULL");
    }

    return new ActiveMQTopic(NO_QUEUE_CONSUMERS_TOPIC_PREFIX + destination->getPhysicalName());
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQDestination* AdvisorySupport::getSlowConsumerAdvisoryTopic(const cms::Destination* destination) {

    const ActiveMQDestination* transformed = NULL;

    bool doDelete = ActiveMQMessageTransformation::transformDestination(destination, &transformed);
    ActiveMQDestination* advisoryDest = getSlowConsumerAdvisoryTopic(transformed);

    if (doDelete) {
        delete transformed;
    }

    return advisoryDest;
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQDestination* AdvisorySupport::getSlowConsumerAdvisoryTopic(const ActiveMQDestination* destination) {
    if (destination == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "The passed ActiveMQDestination cannot be NULL");
    }

    std::string name = SLOW_CONSUMER_TOPIC_PREFIX +
        destination->getDestinationTypeAsString() + "." + destination->getPhysicalName();
    return new ActiveMQTopic(name);
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQDestination* AdvisorySupport::getFastProducerAdvisoryTopic(const cms::Destination* destination) {

    const ActiveMQDestination* transformed = NULL;

    bool doDelete = ActiveMQMessageTransformation::transformDestination(destination, &transformed);
    ActiveMQDestination* advisoryDest = getFastProducerAdvisoryTopic(transformed);

    if (doDelete) {
        delete transformed;
    }

    return advisoryDest;
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQDestination* AdvisorySupport::getFastProducerAdvisoryTopic(const ActiveMQDestination* destination) {
    if (destination == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "The passed ActiveMQDestination cannot be NULL");
    }

    std::string name = FAST_PRODUCER_TOPIC_PREFIX +
        destination->getDestinationTypeAsString() + "." + destination->getPhysicalName();
    return new ActiveMQTopic(name);
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQDestination* AdvisorySupport::getMessageDiscardedAdvisoryTopic(const cms::Destination* destination) {

    const ActiveMQDestination* transformed = NULL;

    bool doDelete = ActiveMQMessageTransformation::transformDestination(destination, &transformed);
    ActiveMQDestination* advisoryDest = getMessageDiscardedAdvisoryTopic(transformed);

    if (doDelete) {
        delete transformed;
    }

    return advisoryDest;
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQDestination* AdvisorySupport::getMessageDiscardedAdvisoryTopic(const ActiveMQDestination* destination) {
    if (destination == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "The passed ActiveMQDestination cannot be NULL");
    }

    std::string name = MESSAGE_DISCAREDED_TOPIC_PREFIX +
        destination->getDestinationTypeAsString() + "." + destination->getPhysicalName();
    return new ActiveMQTopic(name);
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQDestination* AdvisorySupport::getMessageDeliveredAdvisoryTopic(const cms::Destination* destination) {

    const ActiveMQDestination* transformed = NULL;

    bool doDelete = ActiveMQMessageTransformation::transformDestination(destination, &transformed);
    ActiveMQDestination* advisoryDest = getMessageDeliveredAdvisoryTopic(transformed);

    if (doDelete) {
        delete transformed;
    }

    return advisoryDest;
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQDestination* AdvisorySupport::getMessageDeliveredAdvisoryTopic(const ActiveMQDestination* destination) {
    if (destination == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "The passed ActiveMQDestination cannot be NULL");
    }

    std::string name = MESSAGE_DELIVERED_TOPIC_PREFIX +
        destination->getDestinationTypeAsString() + "." + destination->getPhysicalName();
    return new ActiveMQTopic(name);
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQDestination* AdvisorySupport::getMessageConsumedAdvisoryTopic(const cms::Destination* destination) {

    const ActiveMQDestination* transformed = NULL;

    bool doDelete = ActiveMQMessageTransformation::transformDestination(destination, &transformed);
    ActiveMQDestination* advisoryDest = getMessageConsumedAdvisoryTopic(transformed);

    if (doDelete) {
        delete transformed;
    }

    return advisoryDest;
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQDestination* AdvisorySupport::getMessageConsumedAdvisoryTopic(const ActiveMQDestination* destination) {
    if (destination == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "The passed ActiveMQDestination cannot be NULL");
    }

    std::string name = MESSAGE_CONSUMED_TOPIC_PREFIX +
        destination->getDestinationTypeAsString() + "." + destination->getPhysicalName();
    return new ActiveMQTopic(name);
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQDestination* AdvisorySupport::getMessageDLQdAdvisoryTopic(const cms::Destination* destination) {

    const ActiveMQDestination* transformed = NULL;

    bool doDelete = ActiveMQMessageTransformation::transformDestination(destination, &transformed);
    ActiveMQDestination* advisoryDest = getMessageDLQdAdvisoryTopic(transformed);

    if (doDelete) {
        delete transformed;
    }

    return advisoryDest;
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQDestination* AdvisorySupport::getMessageDLQdAdvisoryTopic(const ActiveMQDestination* destination) {
    if (destination == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "The passed ActiveMQDestination cannot be NULL");
    }

    std::string name = MESSAGE_DLQ_TOPIC_PREFIX + destination->getDestinationTypeAsString() + "."
            + destination->getPhysicalName();
    return new ActiveMQTopic(name);
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQDestination* AdvisorySupport::getMasterBrokerAdvisoryTopic() {
    return new ActiveMQTopic(MASTER_BROKER_TOPIC_PREFIX);
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQDestination* AdvisorySupport::getNetworkBridgeAdvisoryTopic() {
    return new ActiveMQTopic(NETWORK_BRIDGE_TOPIC_PREFIX);
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQDestination* AdvisorySupport::getFullAdvisoryTopic(const cms::Destination* destination) {

    const ActiveMQDestination* transformed = NULL;

    bool doDelete = ActiveMQMessageTransformation::transformDestination(destination, &transformed);
    ActiveMQDestination* advisoryDest = getFullAdvisoryTopic(transformed);

    if (doDelete) {
        delete transformed;
    }

    return advisoryDest;
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQDestination* AdvisorySupport::getFullAdvisoryTopic(const ActiveMQDestination* destination) {
    if (destination == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "The passed ActiveMQDestination cannot be NULL");
    }

    std::string name = FULL_TOPIC_PREFIX +
        destination->getDestinationTypeAsString() + "." + destination->getPhysicalName();
    return new ActiveMQTopic(name);
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQDestination* AdvisorySupport::getDestinationAdvisoryTopic(const cms::Destination* destination) {

    const ActiveMQDestination* transformed = NULL;

    bool doDelete = ActiveMQMessageTransformation::transformDestination(destination, &transformed);
    ActiveMQDestination* advisoryDest = getDestinationAdvisoryTopic(transformed);

    if (doDelete) {
        delete transformed;
    }

    return advisoryDest;
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQDestination* AdvisorySupport::getDestinationAdvisoryTopic(const ActiveMQDestination* destination) {

    if (destination == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "The passed ActiveMQDestination cannot be NULL");
    }

    switch (destination->getDestinationType()) {
        case cms::Destination::QUEUE:
            return getQueueAdvisoryTopic();
        case cms::Destination::TOPIC:
            return getTopicAdvisoryTopic();
        case cms::Destination::TEMPORARY_QUEUE:
            return getTempQueueAdvisoryTopic();
        case cms::Destination::TEMPORARY_TOPIC:
            return getTempTopicAdvisoryTopic();
        default:
            throw new IllegalArgumentException(
                __FILE__, __LINE__, "Unknown destination type: " + destination->getDestinationType());
    }
}

////////////////////////////////////////////////////////////////////////////////
bool AdvisorySupport::isDestinationAdvisoryTopic(const cms::Destination* destination) {

    const ActiveMQDestination* transformed = NULL;

    bool doDelete = ActiveMQMessageTransformation::transformDestination(destination, &transformed);
    bool result = isDestinationAdvisoryTopic(transformed);

    if (doDelete) {
        delete transformed;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
bool AdvisorySupport::isDestinationAdvisoryTopic(const ActiveMQDestination* destination) {
    if (destination == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "The passed ActiveMQDestination cannot be NULL");
    }

    if (destination->isComposite()) {
        ArrayList< Pointer<ActiveMQDestination> > compositeDestinations = destination->getCompositeDestinations();
        for (int i = 0; i < compositeDestinations.size(); i++) {
            if (!isDestinationAdvisoryTopic(compositeDestinations.get(i).get())) {
                return false;
            }
        }
        return false;
    } else {
        std::string name = destination->getPhysicalName();
        return name == ADVISORY_TOPIC_PREFIX + "TempQueue" || name == ADVISORY_TOPIC_PREFIX + "TempTopic" ||
               name == ADVISORY_TOPIC_PREFIX + "Queue" || name == ADVISORY_TOPIC_PREFIX + "Topic";
    }
}

////////////////////////////////////////////////////////////////////////////////
bool AdvisorySupport::isTempDestinationAdvisoryTopic(const cms::Destination* destination) {

    const ActiveMQDestination* transformed = NULL;

    bool doDelete = ActiveMQMessageTransformation::transformDestination(destination, &transformed);
    bool result = isTempDestinationAdvisoryTopic(transformed);

    if (doDelete) {
        delete transformed;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
bool AdvisorySupport::isTempDestinationAdvisoryTopic(const ActiveMQDestination* destination) {
    if (destination == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "The passed ActiveMQDestination cannot be NULL");
    }

    if (destination->isComposite()) {
        ArrayList< Pointer<ActiveMQDestination> > compositeDestinations = destination->getCompositeDestinations();
        for (int i = 0; i < compositeDestinations.size(); i++) {
            if (!isTempDestinationAdvisoryTopic(compositeDestinations.get(i).get())) {
                return false;
            }
        }
        return true;
    } else {
        std::string name = destination->getPhysicalName();
        return name == ADVISORY_TOPIC_PREFIX + "TempQueue" || name == ADVISORY_TOPIC_PREFIX + "TempTopic";
    }
}

////////////////////////////////////////////////////////////////////////////////
bool AdvisorySupport::isAdvisoryTopic(const cms::Destination* destination) {

    const ActiveMQDestination* transformed = NULL;

    bool doDelete = ActiveMQMessageTransformation::transformDestination(destination, &transformed);
    bool result = isAdvisoryTopic(transformed);

    if (doDelete) {
        delete transformed;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
bool AdvisorySupport::isAdvisoryTopic(const ActiveMQDestination* destination) {
    if (destination == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "The passed ActiveMQDestination cannot be NULL");
    }

    if (destination->isComposite()) {
        ArrayList< Pointer<ActiveMQDestination> > compositeDestinations = destination->getCompositeDestinations();
        for (int i = 0; i < compositeDestinations.size(); i++) {
            if (isAdvisoryTopic(compositeDestinations.get(i).get())) {
                return true;
            }
        }
        return false;
    } else {
        return destination->isTopic() &&
               destination->getPhysicalName().find(ADVISORY_TOPIC_PREFIX) == 0;
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////
bool AdvisorySupport::isConnectionAdvisoryTopic(const cms::Destination* destination) {

    const ActiveMQDestination* transformed = NULL;

    bool doDelete = ActiveMQMessageTransformation::transformDestination(destination, &transformed);
    bool result = isConnectionAdvisoryTopic(transformed);

    if (doDelete) {
        delete transformed;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
bool AdvisorySupport::isConnectionAdvisoryTopic(const ActiveMQDestination* destination) {
    if (destination == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "The passed ActiveMQDestination cannot be NULL");
    }

    if (destination->isComposite()) {
        ArrayList< Pointer<ActiveMQDestination> > compositeDestinations = destination->getCompositeDestinations();
        for (int i = 0; i < compositeDestinations.size(); i++) {
            if (isConnectionAdvisoryTopic(compositeDestinations.get(i).get())) {
                return true;
            }
        }
        return false;
    } else {
        return destination->getPhysicalName() == ADVISORY_TOPIC_PREFIX + "Connection";
    }
}

////////////////////////////////////////////////////////////////////////////////
bool AdvisorySupport::isProducerAdvisoryTopic(const cms::Destination* destination) {

    const ActiveMQDestination* transformed = NULL;

    bool doDelete = ActiveMQMessageTransformation::transformDestination(destination, &transformed);
    bool result = isProducerAdvisoryTopic(transformed);

    if (doDelete) {
        delete transformed;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
bool AdvisorySupport::isProducerAdvisoryTopic(const ActiveMQDestination* destination) {
    if (destination == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "The passed ActiveMQDestination cannot be NULL");
    }

    if (destination->isComposite()) {
        ArrayList< Pointer<ActiveMQDestination> > compositeDestinations = destination->getCompositeDestinations();
        for (int i = 0; i < compositeDestinations.size(); i++) {
            if (isProducerAdvisoryTopic(compositeDestinations.get(i).get())) {
                return true;
            }
        }
        return false;
    } else {
        return destination->isTopic() &&
               destination->getPhysicalName().find(PRODUCER_ADVISORY_TOPIC_PREFIX) == 0;
    }
}

////////////////////////////////////////////////////////////////////////////////
bool AdvisorySupport::isConsumerAdvisoryTopic(const cms::Destination* destination) {

    const ActiveMQDestination* transformed = NULL;

    bool doDelete = ActiveMQMessageTransformation::transformDestination(destination, &transformed);
    bool result = isConsumerAdvisoryTopic(transformed);

    if (doDelete) {
        delete transformed;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
bool AdvisorySupport::isConsumerAdvisoryTopic(const ActiveMQDestination* destination) {
    if (destination == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "The passed ActiveMQDestination cannot be NULL");
    }

    if (destination->isComposite()) {
        ArrayList< Pointer<ActiveMQDestination> > compositeDestinations = destination->getCompositeDestinations();
        for (int i = 0; i < compositeDestinations.size(); i++) {
            if (isConsumerAdvisoryTopic(compositeDestinations.get(i).get())) {
                return true;
            }
        }
        return false;
    } else {
        return destination->isTopic() &&
               destination->getPhysicalName().find(CONSUMER_ADVISORY_TOPIC_PREFIX) == 0;
    }
}

////////////////////////////////////////////////////////////////////////////////
bool AdvisorySupport::isSlowConsumerAdvisoryTopic(const cms::Destination* destination) {

    const ActiveMQDestination* transformed = NULL;

    bool doDelete = ActiveMQMessageTransformation::transformDestination(destination, &transformed);
    bool result = isSlowConsumerAdvisoryTopic(transformed);

    if (doDelete) {
        delete transformed;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
bool AdvisorySupport::isSlowConsumerAdvisoryTopic(const ActiveMQDestination* destination) {
    if (destination == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "The passed ActiveMQDestination cannot be NULL");
    }

    if (destination->isComposite()) {
        ArrayList< Pointer<ActiveMQDestination> > compositeDestinations = destination->getCompositeDestinations();
        for (int i = 0; i < compositeDestinations.size(); i++) {
            if (isSlowConsumerAdvisoryTopic(compositeDestinations.get(i).get())) {
                return true;
            }
        }
        return false;
    } else {
        return destination->isTopic() &&
               destination->getPhysicalName().find(SLOW_CONSUMER_TOPIC_PREFIX) == 0;
    }
}

////////////////////////////////////////////////////////////////////////////////
bool AdvisorySupport::isFastProducerAdvisoryTopic(const cms::Destination* destination) {

    const ActiveMQDestination* transformed = NULL;

    bool doDelete = ActiveMQMessageTransformation::transformDestination(destination, &transformed);
    bool result = isFastProducerAdvisoryTopic(transformed);

    if (doDelete) {
        delete transformed;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
bool AdvisorySupport::isFastProducerAdvisoryTopic(const ActiveMQDestination* destination) {
    if (destination == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "The passed ActiveMQDestination cannot be NULL");
    }

    if (destination->isComposite()) {
        ArrayList< Pointer<ActiveMQDestination> > compositeDestinations = destination->getCompositeDestinations();
        for (int i = 0; i < compositeDestinations.size(); i++) {
            if (isFastProducerAdvisoryTopic(compositeDestinations.get(i).get())) {
                return true;
            }
        }
        return false;
    } else {
        return destination->isTopic() &&
               destination->getPhysicalName().find(FAST_PRODUCER_TOPIC_PREFIX) == 0;
    }
}

////////////////////////////////////////////////////////////////////////////////
bool AdvisorySupport::isMessageConsumedAdvisoryTopic(const cms::Destination* destination) {

    const ActiveMQDestination* transformed = NULL;

    bool doDelete = ActiveMQMessageTransformation::transformDestination(destination, &transformed);
    bool result = isMessageConsumedAdvisoryTopic(transformed);

    if (doDelete) {
        delete transformed;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
bool AdvisorySupport::isMessageConsumedAdvisoryTopic(const ActiveMQDestination* destination) {
    if (destination == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "The passed ActiveMQDestination cannot be NULL");
    }

    if (destination->isComposite()) {
        ArrayList< Pointer<ActiveMQDestination> > compositeDestinations = destination->getCompositeDestinations();
        for (int i = 0; i < compositeDestinations.size(); i++) {
            if (isMessageConsumedAdvisoryTopic(compositeDestinations.get(i).get())) {
                return true;
            }
        }
        return false;
    } else {
        return destination->isTopic() &&
               destination->getPhysicalName().find(MESSAGE_CONSUMED_TOPIC_PREFIX) == 0;
    }
}

////////////////////////////////////////////////////////////////////////////////
bool AdvisorySupport::isMasterBrokerAdvisoryTopic(const cms::Destination* destination) {

    const ActiveMQDestination* transformed = NULL;

    bool doDelete = ActiveMQMessageTransformation::transformDestination(destination, &transformed);
    bool result = isMasterBrokerAdvisoryTopic(transformed);

    if (doDelete) {
        delete transformed;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
bool AdvisorySupport::isMasterBrokerAdvisoryTopic(const ActiveMQDestination* destination) {
    if (destination == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "The passed ActiveMQDestination cannot be NULL");
    }

    if (destination->isComposite()) {
        ArrayList< Pointer<ActiveMQDestination> > compositeDestinations = destination->getCompositeDestinations();
        for (int i = 0; i < compositeDestinations.size(); i++) {
            if (isMasterBrokerAdvisoryTopic(compositeDestinations.get(i).get())) {
                return true;
            }
        }
        return false;
    } else {
        return destination->isTopic() &&
               destination->getPhysicalName().find(MASTER_BROKER_TOPIC_PREFIX) == 0;
    }
}

////////////////////////////////////////////////////////////////////////////////
bool AdvisorySupport::isMessageDeliveredAdvisoryTopic(const cms::Destination* destination) {

    const ActiveMQDestination* transformed = NULL;

    bool doDelete = ActiveMQMessageTransformation::transformDestination(destination, &transformed);
    bool result = isMessageDeliveredAdvisoryTopic(transformed);

    if (doDelete) {
        delete transformed;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
bool AdvisorySupport::isMessageDeliveredAdvisoryTopic(const ActiveMQDestination* destination) {
    if (destination == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "The passed ActiveMQDestination cannot be NULL");
    }

    if (destination->isComposite()) {
        ArrayList< Pointer<ActiveMQDestination> > compositeDestinations = destination->getCompositeDestinations();
        for (int i = 0; i < compositeDestinations.size(); i++) {
            if (isMessageDeliveredAdvisoryTopic(compositeDestinations.get(i).get())) {
                return true;
            }
        }
        return false;
    } else {
        return destination->isTopic() &&
               destination->getPhysicalName().find(MESSAGE_DELIVERED_TOPIC_PREFIX) == 0;
    }
}

////////////////////////////////////////////////////////////////////////////////
bool AdvisorySupport::isMessageDiscardedAdvisoryTopic(const cms::Destination* destination) {

    const ActiveMQDestination* transformed = NULL;

    bool doDelete = ActiveMQMessageTransformation::transformDestination(destination, &transformed);
    bool result = isMessageDiscardedAdvisoryTopic(transformed);

    if (doDelete) {
        delete transformed;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
bool AdvisorySupport::isMessageDiscardedAdvisoryTopic(const ActiveMQDestination* destination) {
    if (destination == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "The passed ActiveMQDestination cannot be NULL");
    }

    if (destination->isComposite()) {
        ArrayList< Pointer<ActiveMQDestination> > compositeDestinations = destination->getCompositeDestinations();
        for (int i = 0; i < compositeDestinations.size(); i++) {
            if (isMessageDiscardedAdvisoryTopic(compositeDestinations.get(i).get())) {
                return true;
            }
        }
        return false;
    } else {
        return destination->isTopic() &&
               destination->getPhysicalName().find(MESSAGE_DISCAREDED_TOPIC_PREFIX) == 0;
    }
}

////////////////////////////////////////////////////////////////////////////////
bool AdvisorySupport::isMessageDLQdAdvisoryTopic(const cms::Destination* destination) {

    const ActiveMQDestination* transformed = NULL;

    bool doDelete = ActiveMQMessageTransformation::transformDestination(destination, &transformed);
    bool result = isMessageDLQdAdvisoryTopic(transformed);

    if (doDelete) {
        delete transformed;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
bool AdvisorySupport::isMessageDLQdAdvisoryTopic(const ActiveMQDestination* destination) {
    if (destination == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "The passed ActiveMQDestination cannot be NULL");
    }

    if (destination->isComposite()) {
        ArrayList< Pointer<ActiveMQDestination> > compositeDestinations = destination->getCompositeDestinations();
        for (int i = 0; i < compositeDestinations.size(); i++) {
            if (isMessageDLQdAdvisoryTopic(compositeDestinations.get(i).get())) {
                return true;
            }
        }
        return false;
    } else {
        return destination->isTopic() &&
               destination->getPhysicalName().find(MESSAGE_DLQ_TOPIC_PREFIX) == 0;
    }
}

////////////////////////////////////////////////////////////////////////////////
bool AdvisorySupport::isFullAdvisoryTopic(const cms::Destination* destination) {

    const ActiveMQDestination* transformed = NULL;

    bool doDelete = ActiveMQMessageTransformation::transformDestination(destination, &transformed);
    bool result = isFullAdvisoryTopic(transformed);

    if (doDelete) {
        delete transformed;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
bool AdvisorySupport::isFullAdvisoryTopic(const ActiveMQDestination* destination) {
    if (destination == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "The passed ActiveMQDestination cannot be NULL");
    }

    if (destination->isComposite()) {
        ArrayList< Pointer<ActiveMQDestination> > compositeDestinations = destination->getCompositeDestinations();
        for (int i = 0; i < compositeDestinations.size(); i++) {
            if (isFullAdvisoryTopic(compositeDestinations.get(i).get())) {
                return true;
            }
        }
        return false;
    } else {
        return destination->isTopic() &&
               destination->getPhysicalName().find(FULL_TOPIC_PREFIX) == 0;
    }
}

////////////////////////////////////////////////////////////////////////////////
bool AdvisorySupport::isNetworkBridgeAdvisoryTopic(const cms::Destination* destination) {

    const ActiveMQDestination* transformed = NULL;

    bool doDelete = ActiveMQMessageTransformation::transformDestination(destination, &transformed);
    bool result = isNetworkBridgeAdvisoryTopic(transformed);

    if (doDelete) {
        delete transformed;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
bool AdvisorySupport::isNetworkBridgeAdvisoryTopic(const ActiveMQDestination* destination) {
    if (destination == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "The passed ActiveMQDestination cannot be NULL");
    }

    if (destination->isComposite()) {
        ArrayList< Pointer<ActiveMQDestination> > compositeDestinations = destination->getCompositeDestinations();
        for (int i = 0; i < compositeDestinations.size(); i++) {
            if (isNetworkBridgeAdvisoryTopic(compositeDestinations.get(i).get())) {
                return true;
            }
        }
        return false;
    } else {
        return destination->isTopic() &&
               destination->getPhysicalName().find(NETWORK_BRIDGE_TOPIC_PREFIX) == 0;
    }
}

