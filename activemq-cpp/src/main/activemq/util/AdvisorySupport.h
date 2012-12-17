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

#ifndef _ACTIVEMQ_UTIL_ADVISORYSUPPORT_H_
#define _ACTIVEMQ_UTIL_ADVISORYSUPPORT_H_

#include <activemq/util/Config.h>

#include <string>
#include <vector>

namespace cms {
    class Destination;
}
namespace activemq {
namespace commands {
    class ActiveMQDestination;
}
namespace util {

    /**
     * Support class that provides various static constants for use in constructing
     * Destination names for the ActiveMQ advisory destinations.  Methods that can create
     * Advisory Topic instances for commonly used Advisory messages are also provided
     * here.
     */
    class AMQCPP_API AdvisorySupport {
    private:

        AdvisorySupport();

    public:

        static const std::string ADVISORY_TOPIC_PREFIX;
        static const std::string PRODUCER_ADVISORY_TOPIC_PREFIX;
        static const std::string QUEUE_PRODUCER_ADVISORY_TOPIC_PREFIX;
        static const std::string TOPIC_PRODUCER_ADVISORY_TOPIC_PREFIX;
        static const std::string CONSUMER_ADVISORY_TOPIC_PREFIX;
        static const std::string QUEUE_CONSUMER_ADVISORY_TOPIC_PREFIX;
        static const std::string TOPIC_CONSUMER_ADVISORY_TOPIC_PREFIX;
        static const std::string EXPIRED_TOPIC_MESSAGES_TOPIC_PREFIX;
        static const std::string EXPIRED_QUEUE_MESSAGES_TOPIC_PREFIX;
        static const std::string NO_TOPIC_CONSUMERS_TOPIC_PREFIX;
        static const std::string NO_QUEUE_CONSUMERS_TOPIC_PREFIX;
        static const std::string SLOW_CONSUMER_TOPIC_PREFIX;
        static const std::string FAST_PRODUCER_TOPIC_PREFIX;
        static const std::string MESSAGE_DISCAREDED_TOPIC_PREFIX;
        static const std::string FULL_TOPIC_PREFIX;
        static const std::string MESSAGE_DELIVERED_TOPIC_PREFIX;
        static const std::string MESSAGE_CONSUMED_TOPIC_PREFIX;
        static const std::string MESSAGE_DLQ_TOPIC_PREFIX;
        static const std::string MASTER_BROKER_TOPIC_PREFIX;
        static const std::string NETWORK_BRIDGE_TOPIC_PREFIX;
        static const std::string AGENT_TOPIC;
        static const std::string ADIVSORY_MESSAGE_TYPE;
        static const std::string MSG_PROPERTY_ORIGIN_BROKER_ID;
        static const std::string MSG_PROPERTY_ORIGIN_BROKER_NAME;
        static const std::string MSG_PROPERTY_ORIGIN_BROKER_URL;
        static const std::string MSG_PROPERTY_USAGE_NAME;
        static const std::string MSG_PROPERTY_CONSUMER_ID;
        static const std::string MSG_PROPERTY_PRODUCER_ID;
        static const std::string MSG_PROPERTY_MESSAGE_ID;
        static const std::string MSG_PROPERTY_CONSUMER_COUNT;
        static const std::string MSG_PROPERTY_DISCARDED_COUNT;

    public:

        ~AdvisorySupport();

    public:

        /**
         * Returns a new Pointer to an Destination that will consume the advisory messages for both
         * Temporary Topic and Temporary Queue creation on the broker.
         *
         * @return Pointer to the requested Advisory Topic destination.
         */
        static commands::ActiveMQDestination* getTempDestinationCompositeAdvisoryTopic();

        /**
         * Returns a new Pointer to an Destination that will consume the advisory messages for all
         * Destinations created by the Broker, Queue, Topic, Temporary Queue and Temporary Topic.
         *
         * @return Pointer to the requested Advisory Topic destination.
         */
        static commands::ActiveMQDestination* getAllDestinationsCompositeAdvisoryTopic();

        /**
         * Returns a new vector that contains pointers to all the available advisory topics for the given
         * destination.
         *
         * @return vector of all advisory topics that will receive events for the given destination..
         */
        static std::vector<commands::ActiveMQDestination*> getAllDestinationAdvisoryTopics(const cms::Destination* destination);

        /**
         * Returns a new vector that contains pointers to all the available advisory topics for the given
         * destination.
         *
         * @return vector of all advisory topics that will receive events for the given destination..
         */
        static std::vector<commands::ActiveMQDestination*> getAllDestinationAdvisoryTopics(const commands::ActiveMQDestination* destination);

        /**
         * Returns a new Pointer to an Destination that will consume advisory messages for Connections.
         *
         * @return Pointer to the requested Advisory Topic destination.
         */
        static commands::ActiveMQDestination* getConnectionAdvisoryTopic();

        /**
         * Returns a new Pointer to an Destination that will consume advisory messages for Queues.
         *
         * @return Pointer to the requested Advisory Topic destination.
         */
        static commands::ActiveMQDestination* getQueueAdvisoryTopic();

        /**
         * Returns a new Pointer to an Destination that will consume advisory messages for Topics.
         *
         * @return Pointer to the requested Advisory Topic destination.
         */
        static commands::ActiveMQDestination* getTopicAdvisoryTopic();

        /**
         * Returns a new Pointer to an Destination that will consume advisory messages for Temporary Queues.
         *
         * @return Pointer to the requested Advisory Topic destination.
         */
        static commands::ActiveMQDestination* getTempQueueAdvisoryTopic();

        /**
         * Returns a new Pointer to an Destination that will consume advisory messages for Temporary Topics.
         *
         * @return Pointer to the requested Advisory Topic destination.
         */
        static commands::ActiveMQDestination* getTempTopicAdvisoryTopic();

        /**
         * Returns a new Pointer to an Destination that will consume advisory messages for Consumer events.
         *
         * @return Pointer to the requested Advisory Topic destination.
         */
        static commands::ActiveMQDestination* getConsumerAdvisoryTopic(const cms::Destination* destination);

        /**
         * Returns a new Pointer to an Destination that will consume advisory messages for Consumer events.
         *
         * @return Pointer to the requested Advisory Topic destination.
         */
        static commands::ActiveMQDestination* getConsumerAdvisoryTopic(const commands::ActiveMQDestination* destination);

        /**
         * Returns a new Pointer to an Destination that will consume advisory messages for Producer events.
         *
         * @return Pointer to the requested Advisory Topic destination.
         */
        static commands::ActiveMQDestination* getProducerAdvisoryTopic(const cms::Destination* destination);

        /**
         * Returns a new Pointer to an Destination that will consume advisory messages for Producer events.
         *
         * @return Pointer to the requested Advisory Topic destination.
         */
        static commands::ActiveMQDestination* getProducerAdvisoryTopic(const commands::ActiveMQDestination* destination);

        /**
         * Returns a new Pointer to an Destination that will consume advisory messages for expiration events
         * for messages on the specified destination.
         *
         * @return Pointer to the requested Advisory Topic destination.
         */
        static commands::ActiveMQDestination* getExpiredMessageTopic(const cms::Destination* destination);

        /**
         * Returns a new Pointer to an Destination that will consume advisory messages for expiration events
         * for messages on the specified destination.
         *
         * @return Pointer to the requested Advisory Topic destination.
         */
        static commands::ActiveMQDestination* getExpiredMessageTopic(const commands::ActiveMQDestination* destination);

        /**
         * Returns a new Pointer to an Destination that will consume advisory messages for expiration events
         * for messages on the specified destination.
         *
         * @return Pointer to the requested Advisory Topic destination.
         */
        static commands::ActiveMQDestination* getExpiredTopicMessageAdvisoryTopic(const cms::Destination* destination);

        /**
         * Returns a new Pointer to an Destination that will consume advisory messages for expiration events
         * for messages on the specified destination.
         *
         * @return Pointer to the requested Advisory Topic destination.
         */
        static commands::ActiveMQDestination* getExpiredTopicMessageAdvisoryTopic(const commands::ActiveMQDestination* destination);

        /**
         * Returns a new Pointer to an Destination that will consume advisory messages for expiration events
         * for messages on the specified destination.
         *
         * @return Pointer to the requested Advisory Topic destination.
         */
        static commands::ActiveMQDestination* getExpiredQueueMessageAdvisoryTopic(const cms::Destination* destination);

        /**
         * Returns a new Pointer to an Destination that will consume advisory messages for expiration events
         * for messages on the specified destination.
         *
         * @return Pointer to the requested Advisory Topic destination.
         */
        static commands::ActiveMQDestination* getExpiredQueueMessageAdvisoryTopic(const commands::ActiveMQDestination* destination);

        /**
         * Returns a new Pointer to an Destination that will consume advisory messages for no consumer events
         * for messages on the specified destination.
         *
         * @return Pointer to the requested Advisory Topic destination.
         */
        static commands::ActiveMQDestination* getNoConsumersAdvisoryTopic(const cms::Destination* destination);

        /**
         * Returns a new Pointer to an Destination that will consume advisory messages for no consumer events
         * for messages on the specified destination.
         *
         * @return Pointer to the requested Advisory Topic destination.
         */
        static commands::ActiveMQDestination* getNoConsumersAdvisoryTopic(const commands::ActiveMQDestination* destination);

        /**
         * Returns a new Pointer to an Destination that will consume advisory messages for no consumer events
         * for messages on the specified destination.
         *
         * @return Pointer to the requested Advisory Topic destination.
         */
        static commands::ActiveMQDestination* getNoTopicConsumersAdvisoryTopic(const cms::Destination* destination);

        /**
         * Returns a new Pointer to an Destination that will consume advisory messages for no consumer events
         * for messages on the specified destination.
         *
         * @return Pointer to the requested Advisory Topic destination.
         */
        static commands::ActiveMQDestination* getNoTopicConsumersAdvisoryTopic(const commands::ActiveMQDestination* destination);

        /**
         * Returns a new Pointer to an Destination that will consume advisory messages for no consumer events
         * for messages on the specified destination.
         *
         * @return Pointer to the requested Advisory Topic destination.
         */
        static commands::ActiveMQDestination* getNoQueueConsumersAdvisoryTopic(const cms::Destination* destination);

        /**
         * Returns a new Pointer to an Destination that will consume advisory messages for no consumer events
         * for messages on the specified destination.
         *
         * @return Pointer to the requested Advisory Topic destination.
         */
        static commands::ActiveMQDestination* getNoQueueConsumersAdvisoryTopic(const commands::ActiveMQDestination* destination);

        /**
         * Returns a new Pointer to an Destination that will consume advisory messages for slow consumers
         * of messages on the specified destination.
         *
         * @return Pointer to the requested Advisory Topic destination.
         */
        static commands::ActiveMQDestination* getSlowConsumerAdvisoryTopic(const cms::Destination* destination);

        /**
         * Returns a new Pointer to an Destination that will consume advisory messages for slow consumers
         * of messages on the specified destination.
         *
         * @return Pointer to the requested Advisory Topic destination.
         */
        static commands::ActiveMQDestination* getSlowConsumerAdvisoryTopic(const commands::ActiveMQDestination* destination);

        /**
         * Returns a new Pointer to an Destination that will consume advisory messages for fast producers
         * of messages on the specified destination.
         *
         * @return Pointer to the requested Advisory Topic destination.
         */
        static commands::ActiveMQDestination* getFastProducerAdvisoryTopic(const cms::Destination* destination);

        /**
         * Returns a new Pointer to an Destination that will consume advisory messages for fast producers
         * of messages on the specified destination.
         *
         * @return Pointer to the requested Advisory Topic destination.
         */
        static commands::ActiveMQDestination* getFastProducerAdvisoryTopic(const commands::ActiveMQDestination* destination);

        /**
         * Returns a new Pointer to an Destination that will consume advisory messages when a message is
         * discarded on the specified destination
         *
         * @return Pointer to the requested Advisory Topic destination.
         */
        static commands::ActiveMQDestination* getMessageDiscardedAdvisoryTopic(const cms::Destination* destination);

        /**
         * Returns a new Pointer to an Destination that will consume advisory messages when a message is
         * discarded on the specified destination
         *
         * @return Pointer to the requested Advisory Topic destination.
         */
        static commands::ActiveMQDestination* getMessageDiscardedAdvisoryTopic(const commands::ActiveMQDestination* destination);

        /**
         * Returns a new Pointer to an Destination that will consume advisory messages when a message is
         * dispatched to the specified destination
         *
         * @return Pointer to the requested Advisory Topic destination.
         */
        static commands::ActiveMQDestination* getMessageDeliveredAdvisoryTopic(const cms::Destination* destination);

        /**
         * Returns a new Pointer to an Destination that will consume advisory messages when a message is
         * dispatched to the specified destination
         *
         * @return Pointer to the requested Advisory Topic destination.
         */
        static commands::ActiveMQDestination* getMessageDeliveredAdvisoryTopic(const commands::ActiveMQDestination* destination);

        /**
         * Returns a new Pointer to an Destination that will consume advisory messages when a message is
         * consumed from the specified destination
         *
         * @return Pointer to the requested Advisory Topic destination.
         */
        static commands::ActiveMQDestination* getMessageConsumedAdvisoryTopic(const cms::Destination* destination);

        /**
         * Returns a new Pointer to an Destination that will consume advisory messages when a message is
         * consumed from the specified destination
         *
         * @return Pointer to the requested Advisory Topic destination.
         */
        static commands::ActiveMQDestination* getMessageConsumedAdvisoryTopic(const commands::ActiveMQDestination* destination);

        /**
         * Returns a new Pointer to an Destination that will consume advisory messages when a message is
         * sent to the DLQ from the specified destination
         *
         * @return Pointer to the requested Advisory Topic destination.
         */
        static commands::ActiveMQDestination* getMessageDLQdAdvisoryTopic(const cms::Destination* destination);

        /**
         * Returns a new Pointer to an Destination that will consume advisory messages when a message is
         * sent to the DLQ from the specified destination
         *
         * @return Pointer to the requested Advisory Topic destination.
         */
        static commands::ActiveMQDestination* getMessageDLQdAdvisoryTopic(const commands::ActiveMQDestination* destination);

        /**
         * Returns a new Pointer to an Destination that will consume advisory messages for Master Brokers.
         *
         * @return Pointer to the requested Advisory Topic destination.
         */
        static commands::ActiveMQDestination* getMasterBrokerAdvisoryTopic();

        /**
         * Returns a new Pointer to an Destination that will consume advisory messages for Network Bridges.
         *
         * @return Pointer to the requested Advisory Topic destination.
         */
        static commands::ActiveMQDestination* getNetworkBridgeAdvisoryTopic();

        /**
         * Returns a new Pointer to an Destination that will consume advisory messages when the given
         * destination has become full and cannot receive more messages.
         *
         * @return Pointer to the requested Advisory Topic destination.
         */
        static commands::ActiveMQDestination* getFullAdvisoryTopic(const cms::Destination* destination);

        /**
         * Returns a new Pointer to an Destination that will consume advisory messages when the given
         * destination has become full and cannot receive more messages.
         *
         * @return Pointer to the requested Advisory Topic destination.
         */
        static commands::ActiveMQDestination* getFullAdvisoryTopic(const commands::ActiveMQDestination* destination);

        /**
         * Returns a new Pointer to an Destination that will consume advisory messages for events related
         * to Destination such as create and delete.
         *
         * @return Pointer to the requested Advisory Topic destination.
         */
        static commands::ActiveMQDestination* getDestinationAdvisoryTopic(const cms::Destination* destination);

        /**
         * Returns a new Pointer to an Destination that will consume advisory messages for events related
         * to Destination such as create and delete.
         *
         * @return Pointer to the requested Advisory Topic destination.
         */
        static commands::ActiveMQDestination* getDestinationAdvisoryTopic(const commands::ActiveMQDestination* destination);

    public:

        /**
         * @returns true if the specified destination is a Destination advisory topic.
         */
        static bool isDestinationAdvisoryTopic(const cms::Destination* destination);

        /**
         * @returns true if the specified destination is a Destination advisory topic.
         */
        static bool isDestinationAdvisoryTopic(const commands::ActiveMQDestination* destination);

        /**
         * @returns true if the specified destination is a Temporary Destination advisory topic.
         */
        static bool isTempDestinationAdvisoryTopic(const cms::Destination* destination);

        /**
         * @returns true if the specified destination is a Temporary Destination advisory topic.
         */
        static bool isTempDestinationAdvisoryTopic(const commands::ActiveMQDestination* destination);

        /**
         * @returns true if the specified destination is an advisory topic.
         */
        static bool isAdvisoryTopic(const cms::Destination* destination);

        /**
         * @returns true if the specified destination is an advisory topic.
         */
        static bool isAdvisoryTopic(const commands::ActiveMQDestination* destination);

        /**
         * @returns true if the specified destination is an Connection advisory topic.
         */
        static bool isConnectionAdvisoryTopic(const cms::Destination* destination);

        /**
         * @returns true if the specified destination is an Connection advisory topic.
         */
        static bool isConnectionAdvisoryTopic(const commands::ActiveMQDestination* destination);

        /**
         * @returns true if the specified destination is an Producer advisory topic.
         */
        static bool isProducerAdvisoryTopic(const cms::Destination* destination);

        /**
         * @returns true if the specified destination is an Producer advisory topic.
         */
        static bool isProducerAdvisoryTopic(const commands::ActiveMQDestination* destination);

        /**
         * @returns true if the specified destination is an Consumer advisory topic.
         */
        static bool isConsumerAdvisoryTopic(const cms::Destination* destination);

        /**
         * @returns true if the specified destination is an Consumer advisory topic.
         */
        static bool isConsumerAdvisoryTopic(const commands::ActiveMQDestination* destination);

        /**
         * @returns true if the specified destination is an Slow Consumer advisory topic.
         */
        static bool isSlowConsumerAdvisoryTopic(const cms::Destination* destination);

        /**
         * @returns true if the specified destination is an Slow Consumer advisory topic.
         */
        static bool isSlowConsumerAdvisoryTopic(const commands::ActiveMQDestination* destination);

        /**
         * @returns true if the specified destination is an Fast Producer advisory topic.
         */
        static bool isFastProducerAdvisoryTopic(const cms::Destination* destination);

        /**
         * @returns true if the specified destination is an Fast Producer advisory topic.
         */
        static bool isFastProducerAdvisoryTopic(const commands::ActiveMQDestination* destination);

        /**
         * @returns true if the specified destination is an Message Consumed advisory topic.
         */
        static bool isMessageConsumedAdvisoryTopic(const cms::Destination* destination);

        /**
         * @returns true if the specified destination is an Message Consumed advisory topic.
         */
        static bool isMessageConsumedAdvisoryTopic(const commands::ActiveMQDestination* destination);

        /**
         * @returns true if the specified destination is an Master Broker advisory topic.
         */
        static bool isMasterBrokerAdvisoryTopic(const cms::Destination* destination);

        /**
         * @returns true if the specified destination is an Master Broker Consumed advisory topic.
         */
        static bool isMasterBrokerAdvisoryTopic(const commands::ActiveMQDestination* destination);

        /**
         * @returns true if the specified destination is an Message Delivered advisory topic.
         */
        static bool isMessageDeliveredAdvisoryTopic(const cms::Destination* destination);

        /**
         * @returns true if the specified destination is an Message Delivered advisory topic.
         */
        static bool isMessageDeliveredAdvisoryTopic(const commands::ActiveMQDestination* destination);

        /**
         * @returns true if the specified destination is an Message Discarded advisory topic.
         */
        static bool isMessageDiscardedAdvisoryTopic(const cms::Destination* destination);

        /**
         * @returns true if the specified destination is an Message Discarded advisory topic.
         */
        static bool isMessageDiscardedAdvisoryTopic(const commands::ActiveMQDestination* destination);

        /**
         * @returns true if the specified destination is an Message DLQ'd advisory topic.
         */
        static bool isMessageDLQdAdvisoryTopic(const cms::Destination* destination);

        /**
         * @returns true if the specified destination is an Message DLQ'd advisory topic.
         */
        static bool isMessageDLQdAdvisoryTopic(const commands::ActiveMQDestination* destination);

        /**
         * @returns true if the specified destination is an Destination Full advisory topic.
         */
        static bool isFullAdvisoryTopic(const cms::Destination* destination);

        /**
         * @returns true if the specified destination is an Destination Full advisory topic.
         */
        static bool isFullAdvisoryTopic(const commands::ActiveMQDestination* destination);

        /**
         * @returns true if the specified destination is an Network Bridge advisory topic.
         */
        static bool isNetworkBridgeAdvisoryTopic(const cms::Destination* destination);

        /**
         * @returns true if the specified destination is an Network Bridge advisory topic.
         */
        static bool isNetworkBridgeAdvisoryTopic(const commands::ActiveMQDestination* destination);

    };

}}

#endif /* _ACTIVEMQ_UTIL_ADVISORYSUPPORT_H_ */
