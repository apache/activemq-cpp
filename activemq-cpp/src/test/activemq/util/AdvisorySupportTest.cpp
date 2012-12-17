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

#include "AdvisorySupportTest.h"

#include <activemq/util/AdvisorySupport.h>
#include <activemq/commands/ActiveMQDestination.h>
#include <activemq/commands/ActiveMQTopic.h>
#include <activemq/commands/ActiveMQQueue.h>
#include <activemq/commands/ActiveMQTempTopic.h>
#include <activemq/commands/ActiveMQTempQueue.h>

#include <decaf/lang/Pointer.h>
#include <decaf/lang/exceptions/NullPointerException.h>

using namespace activemq;
using namespace activemq::util;
using namespace activemq::commands;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
AdvisorySupportTest::AdvisorySupportTest() {
}

////////////////////////////////////////////////////////////////////////////////
AdvisorySupportTest::~AdvisorySupportTest() {
}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testGetTempDestinationCompositeAdvisoryTopic() {

    Pointer<ActiveMQDestination> topic(AdvisorySupport::getTempDestinationCompositeAdvisoryTopic());
    CPPUNIT_ASSERT(topic != NULL);
    CPPUNIT_ASSERT(topic->isComposite());
    CPPUNIT_ASSERT(topic->isTopic());
    CPPUNIT_ASSERT(topic->isTemporary() == false);
    CPPUNIT_ASSERT(topic->isAdvisory() == true);

    CPPUNIT_ASSERT(topic->getPhysicalName().find(".TempTopic") != std::string::npos);
    CPPUNIT_ASSERT(topic->getPhysicalName().find(".TempQueue") != std::string::npos);
}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testGetAllDestinationsCompositeAdvisoryTopic() {

    Pointer<ActiveMQDestination> topic(AdvisorySupport::getAllDestinationsCompositeAdvisoryTopic());
    CPPUNIT_ASSERT(topic != NULL);
    CPPUNIT_ASSERT(topic->isComposite());
    CPPUNIT_ASSERT(topic->isTopic());
    CPPUNIT_ASSERT(topic->isTemporary() == false);
    CPPUNIT_ASSERT(topic->isAdvisory() == true);

    CPPUNIT_ASSERT(topic->getPhysicalName().find(".TempTopic") != std::string::npos);
    CPPUNIT_ASSERT(topic->getPhysicalName().find(".TempQueue") != std::string::npos);
    CPPUNIT_ASSERT(topic->getPhysicalName().find(".Topic") != std::string::npos);
    CPPUNIT_ASSERT(topic->getPhysicalName().find(".Queue") != std::string::npos);
}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testGetAllDestinationAdvisoryTopics() {

    Pointer<ActiveMQDestination> target(new ActiveMQTopic("Test.Topic"));

    std::vector<ActiveMQDestination*> topics = AdvisorySupport::getAllDestinationAdvisoryTopics(target.get());
    CPPUNIT_ASSERT(topics.size() > 0);

    std::vector<ActiveMQDestination*>::iterator iter = topics.begin();
    for (; iter != topics.end(); ++iter) {
        Pointer<ActiveMQDestination> topic(*iter);
        CPPUNIT_ASSERT(topic->isTopic());
        CPPUNIT_ASSERT(topic->isAdvisory() == true);
        CPPUNIT_ASSERT(topic->isTemporary() == false);
        CPPUNIT_ASSERT(topic->getPhysicalName().find(AdvisorySupport::ADVISORY_TOPIC_PREFIX) == 0);
    }

    Pointer<cms::Destination> cmsDest = target.dynamicCast<cms::Destination>();

    std::vector<ActiveMQDestination*> topics2 = AdvisorySupport::getAllDestinationAdvisoryTopics(cmsDest.get());
    CPPUNIT_ASSERT(topics2.size() > 0);

    std::vector<ActiveMQDestination*>::iterator iter2 = topics2.begin();
    for (; iter2 != topics2.end(); ++iter2) {
        Pointer<ActiveMQDestination> topic(*iter2);
        CPPUNIT_ASSERT(topic->isTopic());
        CPPUNIT_ASSERT(topic->isAdvisory() == true);
        CPPUNIT_ASSERT(topic->isTemporary() == false);
        CPPUNIT_ASSERT(topic->getPhysicalName().find(AdvisorySupport::ADVISORY_TOPIC_PREFIX) == 0);
    }

    CPPUNIT_ASSERT_EQUAL(topics.size(), topics2.size());

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException when passed a NULL destination",
        AdvisorySupport::getAllDestinationAdvisoryTopics((ActiveMQDestination*)NULL),
        NullPointerException);
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException when passed a NULL destination",
        AdvisorySupport::getAllDestinationAdvisoryTopics((cms::Destination*)NULL),
        NullPointerException);
}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testGetConnectionAdvisoryTopic() {

    Pointer<ActiveMQDestination> topic(AdvisorySupport::getConnectionAdvisoryTopic());
    CPPUNIT_ASSERT(topic != NULL);
    CPPUNIT_ASSERT(topic->isAdvisory() == true);
    CPPUNIT_ASSERT(topic->isComposite() == false);
    CPPUNIT_ASSERT(topic->isTopic());
    CPPUNIT_ASSERT(topic->isTemporary() == false);
    CPPUNIT_ASSERT(topic->getPhysicalName().find(AdvisorySupport::ADVISORY_TOPIC_PREFIX) == 0);
    CPPUNIT_ASSERT(topic->getPhysicalName().find(".Connection") != std::string::npos);
}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testGetQueueAdvisoryTopic() {

    Pointer<ActiveMQDestination> topic(AdvisorySupport::getQueueAdvisoryTopic());
    CPPUNIT_ASSERT(topic != NULL);
    CPPUNIT_ASSERT(topic->isComposite() == false);
    CPPUNIT_ASSERT(topic->isAdvisory() == true);
    CPPUNIT_ASSERT(topic->isTopic());
    CPPUNIT_ASSERT(topic->isTemporary() == false);
    CPPUNIT_ASSERT(topic->getPhysicalName().find(AdvisorySupport::ADVISORY_TOPIC_PREFIX) == 0);
    CPPUNIT_ASSERT(topic->getPhysicalName().find(".Queue") != std::string::npos);
}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testGetTopicAdvisoryTopic() {

    Pointer<ActiveMQDestination> topic(AdvisorySupport::getTopicAdvisoryTopic());
    CPPUNIT_ASSERT(topic != NULL);
    CPPUNIT_ASSERT(topic->isComposite() == false);
    CPPUNIT_ASSERT(topic->isTopic());
    CPPUNIT_ASSERT(topic->isAdvisory() == true);
    CPPUNIT_ASSERT(topic->isTemporary() == false);
    CPPUNIT_ASSERT(topic->getPhysicalName().find(AdvisorySupport::ADVISORY_TOPIC_PREFIX) == 0);
    CPPUNIT_ASSERT(topic->getPhysicalName().find(".Topic") != std::string::npos);
}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testGetTempQueueAdvisoryTopic() {

    Pointer<ActiveMQDestination> topic(AdvisorySupport::getTempQueueAdvisoryTopic());
    CPPUNIT_ASSERT(topic != NULL);
    CPPUNIT_ASSERT(topic->isComposite() == false);
    CPPUNIT_ASSERT(topic->isAdvisory() == true);
    CPPUNIT_ASSERT(topic->isTopic());
    CPPUNIT_ASSERT(topic->isTemporary() == false);
    CPPUNIT_ASSERT(topic->getPhysicalName().find(AdvisorySupport::ADVISORY_TOPIC_PREFIX) == 0);
    CPPUNIT_ASSERT(topic->getPhysicalName().find(".TempQueue") != std::string::npos);
}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testGetTempTopicAdvisoryTopic() {

    Pointer<ActiveMQDestination> topic(AdvisorySupport::getTempTopicAdvisoryTopic());
    CPPUNIT_ASSERT(topic != NULL);
    CPPUNIT_ASSERT(topic->isAdvisory() == true);
    CPPUNIT_ASSERT(topic->isComposite() == false);
    CPPUNIT_ASSERT(topic->isTopic());
    CPPUNIT_ASSERT(topic->isTemporary() == false);
    CPPUNIT_ASSERT(topic->getPhysicalName().find(AdvisorySupport::ADVISORY_TOPIC_PREFIX) == 0);
    CPPUNIT_ASSERT(topic->getPhysicalName().find(".TempTopic") != std::string::npos);
}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testGetConsumerAdvisoryTopic() {

    Pointer<ActiveMQDestination> target(new ActiveMQTopic("Test.Topic"));

    Pointer<ActiveMQDestination> topic(AdvisorySupport::getConsumerAdvisoryTopic(target.get()));
    CPPUNIT_ASSERT(topic != NULL);
    CPPUNIT_ASSERT(topic->isComposite() == false);
    CPPUNIT_ASSERT(topic->isAdvisory() == true);
    CPPUNIT_ASSERT(topic->isTopic());
    CPPUNIT_ASSERT(topic->isTemporary() == false);
    CPPUNIT_ASSERT(topic->getPhysicalName().find(AdvisorySupport::CONSUMER_ADVISORY_TOPIC_PREFIX) == 0);
    CPPUNIT_ASSERT(topic->getPhysicalName().find(".Test.Topic") != std::string::npos);

    Pointer<cms::Destination> cmsDest = target.dynamicCast<cms::Destination>();

    Pointer<ActiveMQDestination> cmstopic(AdvisorySupport::getConsumerAdvisoryTopic(cmsDest.get()));
    CPPUNIT_ASSERT(cmstopic != NULL);
    CPPUNIT_ASSERT(cmstopic->isComposite() == false);
    CPPUNIT_ASSERT(cmstopic->isAdvisory() == true);
    CPPUNIT_ASSERT(cmstopic->isTopic());
    CPPUNIT_ASSERT(cmstopic->isTemporary() == false);
    CPPUNIT_ASSERT(cmstopic->getPhysicalName().find(AdvisorySupport::CONSUMER_ADVISORY_TOPIC_PREFIX) == 0);
    CPPUNIT_ASSERT(cmstopic->getPhysicalName().find(".Test.Topic") != std::string::npos);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException when passed a NULL destination",
        AdvisorySupport::getConsumerAdvisoryTopic((ActiveMQDestination*)NULL),
        NullPointerException);
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException when passed a NULL destination",
        AdvisorySupport::getConsumerAdvisoryTopic((cms::Destination*)NULL),
        NullPointerException);
}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testGetProducerAdvisoryTopic() {

    Pointer<ActiveMQDestination> target(new ActiveMQTopic("Test.Topic"));

    Pointer<ActiveMQDestination> topic(AdvisorySupport::getProducerAdvisoryTopic(target.get()));
    CPPUNIT_ASSERT(topic != NULL);
    CPPUNIT_ASSERT(topic->isComposite() == false);
    CPPUNIT_ASSERT(topic->isAdvisory() == true);
    CPPUNIT_ASSERT(topic->isTopic());
    CPPUNIT_ASSERT(topic->isTemporary() == false);
    CPPUNIT_ASSERT(topic->getPhysicalName().find(AdvisorySupport::PRODUCER_ADVISORY_TOPIC_PREFIX) == 0);
    CPPUNIT_ASSERT(topic->getPhysicalName().find(".Test.Topic") != std::string::npos);

    Pointer<cms::Destination> cmsDest = target.dynamicCast<cms::Destination>();

    Pointer<ActiveMQDestination> cmstopic(AdvisorySupport::getProducerAdvisoryTopic(cmsDest.get()));
    CPPUNIT_ASSERT(cmstopic != NULL);
    CPPUNIT_ASSERT(cmstopic->isComposite() == false);
    CPPUNIT_ASSERT(cmstopic->isAdvisory() == true);
    CPPUNIT_ASSERT(cmstopic->isTopic());
    CPPUNIT_ASSERT(cmstopic->isTemporary() == false);
    CPPUNIT_ASSERT(cmstopic->getPhysicalName().find(AdvisorySupport::PRODUCER_ADVISORY_TOPIC_PREFIX) == 0);
    CPPUNIT_ASSERT(cmstopic->getPhysicalName().find(".Test.Topic") != std::string::npos);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException when passed a NULL destination",
        AdvisorySupport::getProducerAdvisoryTopic((ActiveMQDestination*)NULL),
        NullPointerException);
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException when passed a NULL destination",
        AdvisorySupport::getProducerAdvisoryTopic((cms::Destination*)NULL),
        NullPointerException);
}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testGetExpiredMessageTopic() {

    {
        Pointer<ActiveMQDestination> target(new ActiveMQTopic("Test.Topic"));

        Pointer<ActiveMQDestination> topic(AdvisorySupport::getExpiredMessageTopic(target.get()));
        CPPUNIT_ASSERT(topic != NULL);
        CPPUNIT_ASSERT(topic->isComposite() == false);
        CPPUNIT_ASSERT(topic->isAdvisory() == true);
        CPPUNIT_ASSERT(topic->isTopic());
        CPPUNIT_ASSERT(topic->isTemporary() == false);
        CPPUNIT_ASSERT(topic->getPhysicalName().find(AdvisorySupport::EXPIRED_TOPIC_MESSAGES_TOPIC_PREFIX) == 0);
        CPPUNIT_ASSERT(topic->getPhysicalName().find(".Test.Topic") != std::string::npos);

        Pointer<cms::Destination> cmsDest = target.dynamicCast<cms::Destination>();

        Pointer<ActiveMQDestination> cmstopic(AdvisorySupport::getExpiredMessageTopic(cmsDest.get()));
        CPPUNIT_ASSERT(cmstopic != NULL);
        CPPUNIT_ASSERT(cmstopic->isComposite() == false);
        CPPUNIT_ASSERT(cmstopic->isAdvisory() == true);
        CPPUNIT_ASSERT(cmstopic->isTopic());
        CPPUNIT_ASSERT(cmstopic->isTemporary() == false);
        CPPUNIT_ASSERT(cmstopic->getPhysicalName().find(AdvisorySupport::EXPIRED_TOPIC_MESSAGES_TOPIC_PREFIX) == 0);
        CPPUNIT_ASSERT(cmstopic->getPhysicalName().find(".Test.Topic") != std::string::npos);
    }
    {
        Pointer<ActiveMQDestination> target(new ActiveMQQueue("Test.Queue"));

        Pointer<ActiveMQDestination> topic(AdvisorySupport::getExpiredMessageTopic(target.get()));
        CPPUNIT_ASSERT(topic != NULL);
        CPPUNIT_ASSERT(topic->isComposite() == false);
        CPPUNIT_ASSERT(topic->isAdvisory() == true);
        CPPUNIT_ASSERT(topic->isTopic());
        CPPUNIT_ASSERT(topic->isTemporary() == false);
        CPPUNIT_ASSERT(topic->getPhysicalName().find(AdvisorySupport::EXPIRED_QUEUE_MESSAGES_TOPIC_PREFIX) == 0);
        CPPUNIT_ASSERT(topic->getPhysicalName().find(".Test.Queue") != std::string::npos);

        Pointer<cms::Destination> cmsDest = target.dynamicCast<cms::Destination>();

        Pointer<ActiveMQDestination> cmstopic(AdvisorySupport::getExpiredMessageTopic(cmsDest.get()));
        CPPUNIT_ASSERT(cmstopic != NULL);
        CPPUNIT_ASSERT(cmstopic->isComposite() == false);
        CPPUNIT_ASSERT(cmstopic->isAdvisory() == true);
        CPPUNIT_ASSERT(cmstopic->isTopic());
        CPPUNIT_ASSERT(cmstopic->isTemporary() == false);
        CPPUNIT_ASSERT(cmstopic->getPhysicalName().find(AdvisorySupport::EXPIRED_QUEUE_MESSAGES_TOPIC_PREFIX) == 0);
        CPPUNIT_ASSERT(cmstopic->getPhysicalName().find(".Test.Queue") != std::string::npos);
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException when passed a NULL destination",
        AdvisorySupport::getExpiredMessageTopic((ActiveMQDestination*)NULL),
        NullPointerException);
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException when passed a NULL destination",
        AdvisorySupport::getExpiredMessageTopic((cms::Destination*)NULL),
        NullPointerException);
}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testGetExpiredTopicMessageAdvisoryTopic() {

    Pointer<ActiveMQDestination> target(new ActiveMQTopic("Test.Topic"));

    Pointer<ActiveMQDestination> topic(AdvisorySupport::getExpiredTopicMessageAdvisoryTopic(target.get()));
    CPPUNIT_ASSERT(topic != NULL);
    CPPUNIT_ASSERT(topic->isComposite() == false);
    CPPUNIT_ASSERT(topic->isAdvisory() == true);
    CPPUNIT_ASSERT(topic->isTopic());
    CPPUNIT_ASSERT(topic->isTemporary() == false);
    CPPUNIT_ASSERT(topic->getPhysicalName().find(AdvisorySupport::EXPIRED_TOPIC_MESSAGES_TOPIC_PREFIX) == 0);
    CPPUNIT_ASSERT(topic->getPhysicalName().find(".Test.Topic") != std::string::npos);

    Pointer<cms::Destination> cmsDest = target.dynamicCast<cms::Destination>();

    Pointer<ActiveMQDestination> cmstopic(AdvisorySupport::getExpiredTopicMessageAdvisoryTopic(cmsDest.get()));
    CPPUNIT_ASSERT(cmstopic != NULL);
    CPPUNIT_ASSERT(cmstopic->isComposite() == false);
    CPPUNIT_ASSERT(cmstopic->isAdvisory() == true);
    CPPUNIT_ASSERT(cmstopic->isTopic());
    CPPUNIT_ASSERT(cmstopic->isTemporary() == false);
    CPPUNIT_ASSERT(cmstopic->getPhysicalName().find(AdvisorySupport::EXPIRED_TOPIC_MESSAGES_TOPIC_PREFIX) == 0);
    CPPUNIT_ASSERT(cmstopic->getPhysicalName().find(".Test.Topic") != std::string::npos);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException when passed a NULL destination",
        AdvisorySupport::getExpiredTopicMessageAdvisoryTopic((ActiveMQDestination*)NULL),
        NullPointerException);
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException when passed a NULL destination",
        AdvisorySupport::getExpiredTopicMessageAdvisoryTopic((cms::Destination*)NULL),
        NullPointerException);
}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testGetExpiredQueueMessageAdvisoryTopic() {

    Pointer<ActiveMQDestination> target(new ActiveMQQueue("Test.Queue"));

    Pointer<ActiveMQDestination> topic(AdvisorySupport::getExpiredQueueMessageAdvisoryTopic(target.get()));
    CPPUNIT_ASSERT(topic != NULL);
    CPPUNIT_ASSERT(topic->isComposite() == false);
    CPPUNIT_ASSERT(topic->isAdvisory() == true);
    CPPUNIT_ASSERT(topic->isTopic());
    CPPUNIT_ASSERT(topic->isTemporary() == false);
    CPPUNIT_ASSERT(topic->getPhysicalName().find(AdvisorySupport::EXPIRED_QUEUE_MESSAGES_TOPIC_PREFIX) == 0);
    CPPUNIT_ASSERT(topic->getPhysicalName().find(".Test.Queue") != std::string::npos);

    Pointer<cms::Destination> cmsDest = target.dynamicCast<cms::Destination>();

    Pointer<ActiveMQDestination> cmstopic(AdvisorySupport::getExpiredQueueMessageAdvisoryTopic(cmsDest.get()));
    CPPUNIT_ASSERT(cmstopic != NULL);
    CPPUNIT_ASSERT(cmstopic->isComposite() == false);
    CPPUNIT_ASSERT(cmstopic->isAdvisory() == true);
    CPPUNIT_ASSERT(cmstopic->isTopic());
    CPPUNIT_ASSERT(cmstopic->isTemporary() == false);
    CPPUNIT_ASSERT(cmstopic->getPhysicalName().find(AdvisorySupport::EXPIRED_QUEUE_MESSAGES_TOPIC_PREFIX) == 0);
    CPPUNIT_ASSERT(cmstopic->getPhysicalName().find(".Test.Queue") != std::string::npos);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException when passed a NULL destination",
        AdvisorySupport::getExpiredQueueMessageAdvisoryTopic((ActiveMQDestination*)NULL),
        NullPointerException);
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException when passed a NULL destination",
        AdvisorySupport::getExpiredQueueMessageAdvisoryTopic((cms::Destination*)NULL),
        NullPointerException);
}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testGetNoConsumersAdvisoryTopic() {

    {
        Pointer<ActiveMQDestination> target(new ActiveMQTopic("Test.Topic"));

        Pointer<ActiveMQDestination> topic(AdvisorySupport::getNoConsumersAdvisoryTopic(target.get()));
        CPPUNIT_ASSERT(topic != NULL);
        CPPUNIT_ASSERT(topic->isComposite() == false);
        CPPUNIT_ASSERT(topic->isAdvisory() == true);
        CPPUNIT_ASSERT(topic->isTopic());
        CPPUNIT_ASSERT(topic->isTemporary() == false);
        CPPUNIT_ASSERT(topic->getPhysicalName().find(AdvisorySupport::NO_TOPIC_CONSUMERS_TOPIC_PREFIX) == 0);
        CPPUNIT_ASSERT(topic->getPhysicalName().find(".Test.Topic") != std::string::npos);

        Pointer<cms::Destination> cmsDest = target.dynamicCast<cms::Destination>();

        Pointer<ActiveMQDestination> cmstopic(AdvisorySupport::getNoConsumersAdvisoryTopic(cmsDest.get()));
        CPPUNIT_ASSERT(cmstopic != NULL);
        CPPUNIT_ASSERT(cmstopic->isComposite() == false);
        CPPUNIT_ASSERT(cmstopic->isAdvisory() == true);
        CPPUNIT_ASSERT(cmstopic->isTopic());
        CPPUNIT_ASSERT(cmstopic->isTemporary() == false);
        CPPUNIT_ASSERT(cmstopic->getPhysicalName().find(AdvisorySupport::NO_TOPIC_CONSUMERS_TOPIC_PREFIX) == 0);
        CPPUNIT_ASSERT(cmstopic->getPhysicalName().find(".Test.Topic") != std::string::npos);
    }
    {
        Pointer<ActiveMQDestination> target(new ActiveMQQueue("Test.Queue"));

        Pointer<ActiveMQDestination> topic(AdvisorySupport::getNoConsumersAdvisoryTopic(target.get()));
        CPPUNIT_ASSERT(topic != NULL);
        CPPUNIT_ASSERT(topic->isComposite() == false);
        CPPUNIT_ASSERT(topic->isAdvisory() == true);
        CPPUNIT_ASSERT(topic->isTopic());
        CPPUNIT_ASSERT(topic->isTemporary() == false);
        CPPUNIT_ASSERT(topic->getPhysicalName().find(AdvisorySupport::NO_QUEUE_CONSUMERS_TOPIC_PREFIX) == 0);
        CPPUNIT_ASSERT(topic->getPhysicalName().find(".Test.Queue") != std::string::npos);

        Pointer<cms::Destination> cmsDest = target.dynamicCast<cms::Destination>();

        Pointer<ActiveMQDestination> cmstopic(AdvisorySupport::getNoConsumersAdvisoryTopic(cmsDest.get()));
        CPPUNIT_ASSERT(cmstopic != NULL);
        CPPUNIT_ASSERT(cmstopic->isComposite() == false);
        CPPUNIT_ASSERT(cmstopic->isAdvisory() == true);
        CPPUNIT_ASSERT(cmstopic->isTopic());
        CPPUNIT_ASSERT(cmstopic->isTemporary() == false);
        CPPUNIT_ASSERT(cmstopic->getPhysicalName().find(AdvisorySupport::NO_QUEUE_CONSUMERS_TOPIC_PREFIX) == 0);
        CPPUNIT_ASSERT(cmstopic->getPhysicalName().find(".Test.Queue") != std::string::npos);
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException when passed a NULL destination",
        AdvisorySupport::getNoConsumersAdvisoryTopic((ActiveMQDestination*)NULL),
        NullPointerException);
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException when passed a NULL destination",
        AdvisorySupport::getNoConsumersAdvisoryTopic((cms::Destination*)NULL),
        NullPointerException);
}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testGetNoTopicConsumersAdvisoryTopic() {

    Pointer<ActiveMQDestination> target(new ActiveMQTopic("Test.Topic"));

    Pointer<ActiveMQDestination> topic(AdvisorySupport::getNoTopicConsumersAdvisoryTopic(target.get()));
    CPPUNIT_ASSERT(topic != NULL);
    CPPUNIT_ASSERT(topic->isComposite() == false);
    CPPUNIT_ASSERT(topic->isAdvisory() == true);
    CPPUNIT_ASSERT(topic->isTopic());
    CPPUNIT_ASSERT(topic->isTemporary() == false);
    CPPUNIT_ASSERT(topic->getPhysicalName().find(AdvisorySupport::NO_TOPIC_CONSUMERS_TOPIC_PREFIX) == 0);
    CPPUNIT_ASSERT(topic->getPhysicalName().find(".Test.Topic") != std::string::npos);

    Pointer<cms::Destination> cmsDest = target.dynamicCast<cms::Destination>();

    Pointer<ActiveMQDestination> cmstopic(AdvisorySupport::getNoTopicConsumersAdvisoryTopic(cmsDest.get()));
    CPPUNIT_ASSERT(cmstopic != NULL);
    CPPUNIT_ASSERT(cmstopic->isComposite() == false);
    CPPUNIT_ASSERT(cmstopic->isAdvisory() == true);
    CPPUNIT_ASSERT(cmstopic->isTopic());
    CPPUNIT_ASSERT(cmstopic->isTemporary() == false);
    CPPUNIT_ASSERT(cmstopic->getPhysicalName().find(AdvisorySupport::NO_TOPIC_CONSUMERS_TOPIC_PREFIX) == 0);
    CPPUNIT_ASSERT(cmstopic->getPhysicalName().find(".Test.Topic") != std::string::npos);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException when passed a NULL destination",
        AdvisorySupport::getNoTopicConsumersAdvisoryTopic((ActiveMQDestination*)NULL),
        NullPointerException);
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException when passed a NULL destination",
        AdvisorySupport::getNoTopicConsumersAdvisoryTopic((cms::Destination*)NULL),
        NullPointerException);
}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testGetNoQueueConsumersAdvisoryTopic() {

    Pointer<ActiveMQDestination> target(new ActiveMQQueue("Test.Queue"));

    Pointer<ActiveMQDestination> topic(AdvisorySupport::getNoQueueConsumersAdvisoryTopic(target.get()));
    CPPUNIT_ASSERT(topic != NULL);
    CPPUNIT_ASSERT(topic->isComposite() == false);
    CPPUNIT_ASSERT(topic->isAdvisory() == true);
    CPPUNIT_ASSERT(topic->isTopic());
    CPPUNIT_ASSERT(topic->isTemporary() == false);
    CPPUNIT_ASSERT(topic->getPhysicalName().find(AdvisorySupport::NO_QUEUE_CONSUMERS_TOPIC_PREFIX) == 0);
    CPPUNIT_ASSERT(topic->getPhysicalName().find(".Test.Queue") != std::string::npos);

    Pointer<cms::Destination> cmsDest = target.dynamicCast<cms::Destination>();

    Pointer<ActiveMQDestination> cmstopic(AdvisorySupport::getNoQueueConsumersAdvisoryTopic(cmsDest.get()));
    CPPUNIT_ASSERT(cmstopic != NULL);
    CPPUNIT_ASSERT(cmstopic->isComposite() == false);
    CPPUNIT_ASSERT(cmstopic->isAdvisory() == true);
    CPPUNIT_ASSERT(cmstopic->isTopic());
    CPPUNIT_ASSERT(cmstopic->isTemporary() == false);
    CPPUNIT_ASSERT(cmstopic->getPhysicalName().find(AdvisorySupport::NO_QUEUE_CONSUMERS_TOPIC_PREFIX) == 0);
    CPPUNIT_ASSERT(cmstopic->getPhysicalName().find(".Test.Queue") != std::string::npos);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException when passed a NULL destination",
        AdvisorySupport::getNoQueueConsumersAdvisoryTopic((ActiveMQDestination*)NULL),
        NullPointerException);
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException when passed a NULL destination",
        AdvisorySupport::getNoQueueConsumersAdvisoryTopic((cms::Destination*)NULL),
        NullPointerException);
}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testGetSlowConsumerAdvisoryTopic() {

    Pointer<ActiveMQDestination> target(new ActiveMQTopic("Test.Topic"));

    Pointer<ActiveMQDestination> topic(AdvisorySupport::getSlowConsumerAdvisoryTopic(target.get()));
    CPPUNIT_ASSERT(topic != NULL);
    CPPUNIT_ASSERT(topic->isComposite() == false);
    CPPUNIT_ASSERT(topic->isAdvisory() == true);
    CPPUNIT_ASSERT(topic->isTopic());
    CPPUNIT_ASSERT(topic->isTemporary() == false);
    CPPUNIT_ASSERT(topic->getPhysicalName().find(AdvisorySupport::SLOW_CONSUMER_TOPIC_PREFIX) == 0);
    CPPUNIT_ASSERT(topic->getPhysicalName().find(".Test.Topic") != std::string::npos);

    Pointer<cms::Destination> cmsDest = target.dynamicCast<cms::Destination>();

    Pointer<ActiveMQDestination> cmstopic(AdvisorySupport::getSlowConsumerAdvisoryTopic(cmsDest.get()));
    CPPUNIT_ASSERT(cmstopic != NULL);
    CPPUNIT_ASSERT(cmstopic->isComposite() == false);
    CPPUNIT_ASSERT(cmstopic->isAdvisory() == true);
    CPPUNIT_ASSERT(cmstopic->isTopic());
    CPPUNIT_ASSERT(cmstopic->isTemporary() == false);
    CPPUNIT_ASSERT(cmstopic->getPhysicalName().find(AdvisorySupport::SLOW_CONSUMER_TOPIC_PREFIX) == 0);
    CPPUNIT_ASSERT(cmstopic->getPhysicalName().find(".Test.Topic") != std::string::npos);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException when passed a NULL destination",
        AdvisorySupport::getSlowConsumerAdvisoryTopic((ActiveMQDestination*)NULL),
        NullPointerException);
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException when passed a NULL destination",
        AdvisorySupport::getSlowConsumerAdvisoryTopic((cms::Destination*)NULL),
        NullPointerException);
}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testGetFastProducerAdvisoryTopic() {

    Pointer<ActiveMQDestination> target(new ActiveMQTopic("Test.Topic"));

    Pointer<ActiveMQDestination> topic(AdvisorySupport::getFastProducerAdvisoryTopic(target.get()));
    CPPUNIT_ASSERT(topic != NULL);
    CPPUNIT_ASSERT(topic->isComposite() == false);
    CPPUNIT_ASSERT(topic->isAdvisory() == true);
    CPPUNIT_ASSERT(topic->isTopic());
    CPPUNIT_ASSERT(topic->isTemporary() == false);
    CPPUNIT_ASSERT(topic->getPhysicalName().find(AdvisorySupport::FAST_PRODUCER_TOPIC_PREFIX) == 0);
    CPPUNIT_ASSERT(topic->getPhysicalName().find(".Test.Topic") != std::string::npos);

    Pointer<cms::Destination> cmsDest = target.dynamicCast<cms::Destination>();

    Pointer<ActiveMQDestination> cmstopic(AdvisorySupport::getFastProducerAdvisoryTopic(cmsDest.get()));
    CPPUNIT_ASSERT(cmstopic != NULL);
    CPPUNIT_ASSERT(cmstopic->isComposite() == false);
    CPPUNIT_ASSERT(cmstopic->isAdvisory() == true);
    CPPUNIT_ASSERT(cmstopic->isTopic());
    CPPUNIT_ASSERT(cmstopic->isTemporary() == false);
    CPPUNIT_ASSERT(cmstopic->getPhysicalName().find(AdvisorySupport::FAST_PRODUCER_TOPIC_PREFIX) == 0);
    CPPUNIT_ASSERT(cmstopic->getPhysicalName().find(".Test.Topic") != std::string::npos);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException when passed a NULL destination",
        AdvisorySupport::getFastProducerAdvisoryTopic((ActiveMQDestination*)NULL),
        NullPointerException);
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException when passed a NULL destination",
        AdvisorySupport::getFastProducerAdvisoryTopic((cms::Destination*)NULL),
        NullPointerException);
}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testGetMessageDiscardedAdvisoryTopic() {

    Pointer<ActiveMQDestination> target(new ActiveMQTopic("Test.Topic"));

    Pointer<ActiveMQDestination> topic(AdvisorySupport::getMessageDiscardedAdvisoryTopic(target.get()));
    CPPUNIT_ASSERT(topic != NULL);
    CPPUNIT_ASSERT(topic->isComposite() == false);
    CPPUNIT_ASSERT(topic->isAdvisory() == true);
    CPPUNIT_ASSERT(topic->isTopic());
    CPPUNIT_ASSERT(topic->isTemporary() == false);
    CPPUNIT_ASSERT(topic->getPhysicalName().find(AdvisorySupport::MESSAGE_DISCAREDED_TOPIC_PREFIX) == 0);
    CPPUNIT_ASSERT(topic->getPhysicalName().find(".Test.Topic") != std::string::npos);

    Pointer<cms::Destination> cmsDest = target.dynamicCast<cms::Destination>();

    Pointer<ActiveMQDestination> cmstopic(AdvisorySupport::getMessageDiscardedAdvisoryTopic(cmsDest.get()));
    CPPUNIT_ASSERT(cmstopic != NULL);
    CPPUNIT_ASSERT(cmstopic->isComposite() == false);
    CPPUNIT_ASSERT(cmstopic->isAdvisory() == true);
    CPPUNIT_ASSERT(cmstopic->isTopic());
    CPPUNIT_ASSERT(cmstopic->isTemporary() == false);
    CPPUNIT_ASSERT(cmstopic->getPhysicalName().find(AdvisorySupport::MESSAGE_DISCAREDED_TOPIC_PREFIX) == 0);
    CPPUNIT_ASSERT(cmstopic->getPhysicalName().find(".Test.Topic") != std::string::npos);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException when passed a NULL destination",
        AdvisorySupport::getMessageDiscardedAdvisoryTopic((ActiveMQDestination*)NULL),
        NullPointerException);
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException when passed a NULL destination",
        AdvisorySupport::getMessageDiscardedAdvisoryTopic((cms::Destination*)NULL),
        NullPointerException);
}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testGetMessageDeliveredAdvisoryTopic() {

    Pointer<ActiveMQDestination> target(new ActiveMQTopic("Test.Topic"));

    Pointer<ActiveMQDestination> topic(AdvisorySupport::getMessageDeliveredAdvisoryTopic(target.get()));
    CPPUNIT_ASSERT(topic != NULL);
    CPPUNIT_ASSERT(topic->isComposite() == false);
    CPPUNIT_ASSERT(topic->isAdvisory() == true);
    CPPUNIT_ASSERT(topic->isTopic());
    CPPUNIT_ASSERT(topic->isTemporary() == false);
    CPPUNIT_ASSERT(topic->getPhysicalName().find(AdvisorySupport::MESSAGE_DELIVERED_TOPIC_PREFIX) == 0);
    CPPUNIT_ASSERT(topic->getPhysicalName().find(".Test.Topic") != std::string::npos);

    Pointer<cms::Destination> cmsDest = target.dynamicCast<cms::Destination>();

    Pointer<ActiveMQDestination> cmstopic(AdvisorySupport::getMessageDeliveredAdvisoryTopic(cmsDest.get()));
    CPPUNIT_ASSERT(cmstopic != NULL);
    CPPUNIT_ASSERT(cmstopic->isComposite() == false);
    CPPUNIT_ASSERT(cmstopic->isAdvisory() == true);
    CPPUNIT_ASSERT(cmstopic->isTopic());
    CPPUNIT_ASSERT(cmstopic->isTemporary() == false);
    CPPUNIT_ASSERT(cmstopic->getPhysicalName().find(AdvisorySupport::MESSAGE_DELIVERED_TOPIC_PREFIX) == 0);
    CPPUNIT_ASSERT(cmstopic->getPhysicalName().find(".Test.Topic") != std::string::npos);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException when passed a NULL destination",
        AdvisorySupport::getMessageDeliveredAdvisoryTopic((ActiveMQDestination*)NULL),
        NullPointerException);
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException when passed a NULL destination",
        AdvisorySupport::getMessageDeliveredAdvisoryTopic((cms::Destination*)NULL),
        NullPointerException);
}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testGetMessageConsumedAdvisoryTopic() {

    Pointer<ActiveMQDestination> target(new ActiveMQTopic("Test.Topic"));

    Pointer<ActiveMQDestination> topic(AdvisorySupport::getMessageConsumedAdvisoryTopic(target.get()));
    CPPUNIT_ASSERT(topic != NULL);
    CPPUNIT_ASSERT(topic->isComposite() == false);
    CPPUNIT_ASSERT(topic->isAdvisory() == true);
    CPPUNIT_ASSERT(topic->isTopic());
    CPPUNIT_ASSERT(topic->isTemporary() == false);
    CPPUNIT_ASSERT(topic->getPhysicalName().find(AdvisorySupport::MESSAGE_CONSUMED_TOPIC_PREFIX) == 0);
    CPPUNIT_ASSERT(topic->getPhysicalName().find(".Test.Topic") != std::string::npos);

    Pointer<cms::Destination> cmsDest = target.dynamicCast<cms::Destination>();

    Pointer<ActiveMQDestination> cmstopic(AdvisorySupport::getMessageConsumedAdvisoryTopic(cmsDest.get()));
    CPPUNIT_ASSERT(cmstopic != NULL);
    CPPUNIT_ASSERT(cmstopic->isComposite() == false);
    CPPUNIT_ASSERT(cmstopic->isAdvisory() == true);
    CPPUNIT_ASSERT(cmstopic->isTopic());
    CPPUNIT_ASSERT(cmstopic->isTemporary() == false);
    CPPUNIT_ASSERT(cmstopic->getPhysicalName().find(AdvisorySupport::MESSAGE_CONSUMED_TOPIC_PREFIX) == 0);
    CPPUNIT_ASSERT(cmstopic->getPhysicalName().find(".Test.Topic") != std::string::npos);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException when passed a NULL destination",
        AdvisorySupport::getMessageConsumedAdvisoryTopic((ActiveMQDestination*)NULL),
        NullPointerException);
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException when passed a NULL destination",
        AdvisorySupport::getMessageConsumedAdvisoryTopic((cms::Destination*)NULL),
        NullPointerException);
}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testGetMessageDLQdAdvisoryTopic() {

    Pointer<ActiveMQDestination> target(new ActiveMQTopic("Test.Topic"));

    Pointer<ActiveMQDestination> topic(AdvisorySupport::getMessageDLQdAdvisoryTopic(target.get()));
    CPPUNIT_ASSERT(topic != NULL);
    CPPUNIT_ASSERT(topic->isComposite() == false);
    CPPUNIT_ASSERT(topic->isAdvisory() == true);
    CPPUNIT_ASSERT(topic->isTopic());
    CPPUNIT_ASSERT(topic->isTemporary() == false);
    CPPUNIT_ASSERT(topic->getPhysicalName().find(AdvisorySupport::MESSAGE_DLQ_TOPIC_PREFIX) == 0);
    CPPUNIT_ASSERT(topic->getPhysicalName().find(".Test.Topic") != std::string::npos);

    Pointer<cms::Destination> cmsDest = target.dynamicCast<cms::Destination>();

    Pointer<ActiveMQDestination> cmstopic(AdvisorySupport::getMessageDLQdAdvisoryTopic(cmsDest.get()));
    CPPUNIT_ASSERT(cmstopic != NULL);
    CPPUNIT_ASSERT(cmstopic->isComposite() == false);
    CPPUNIT_ASSERT(cmstopic->isAdvisory() == true);
    CPPUNIT_ASSERT(cmstopic->isTopic());
    CPPUNIT_ASSERT(cmstopic->isTemporary() == false);
    CPPUNIT_ASSERT(cmstopic->getPhysicalName().find(AdvisorySupport::MESSAGE_DLQ_TOPIC_PREFIX) == 0);
    CPPUNIT_ASSERT(cmstopic->getPhysicalName().find(".Test.Topic") != std::string::npos);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException when passed a NULL destination",
        AdvisorySupport::getMessageDLQdAdvisoryTopic((ActiveMQDestination*)NULL),
        NullPointerException);
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException when passed a NULL destination",
        AdvisorySupport::getMessageDLQdAdvisoryTopic((cms::Destination*)NULL),
        NullPointerException);
}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testGetMasterBrokerAdvisoryTopic() {

    Pointer<ActiveMQDestination> topic(AdvisorySupport::getMasterBrokerAdvisoryTopic());
    CPPUNIT_ASSERT(topic != NULL);
    CPPUNIT_ASSERT(topic->isComposite() == false);
    CPPUNIT_ASSERT(topic->isAdvisory() == true);
    CPPUNIT_ASSERT(topic->isTopic());
    CPPUNIT_ASSERT(topic->isTemporary() == false);
    CPPUNIT_ASSERT(topic->getPhysicalName().find(AdvisorySupport::MASTER_BROKER_TOPIC_PREFIX) == 0);
}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testGetNetworkBridgeAdvisoryTopic() {

    Pointer<ActiveMQDestination> topic(AdvisorySupport::getNetworkBridgeAdvisoryTopic());
    CPPUNIT_ASSERT(topic != NULL);
    CPPUNIT_ASSERT(topic->isComposite() == false);
    CPPUNIT_ASSERT(topic->isAdvisory() == true);
    CPPUNIT_ASSERT(topic->isTopic());
    CPPUNIT_ASSERT(topic->isTemporary() == false);
    CPPUNIT_ASSERT(topic->getPhysicalName().find(AdvisorySupport::NETWORK_BRIDGE_TOPIC_PREFIX) == 0);
}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testGetFullAdvisoryTopic() {

    Pointer<ActiveMQDestination> target(new ActiveMQTopic("Test.Topic"));

    Pointer<ActiveMQDestination> topic(AdvisorySupport::getFullAdvisoryTopic(target.get()));
    CPPUNIT_ASSERT(topic != NULL);
    CPPUNIT_ASSERT(topic->isComposite() == false);
    CPPUNIT_ASSERT(topic->isAdvisory() == true);
    CPPUNIT_ASSERT(topic->isTopic());
    CPPUNIT_ASSERT(topic->isTemporary() == false);
    CPPUNIT_ASSERT(topic->getPhysicalName().find(AdvisorySupport::FULL_TOPIC_PREFIX) == 0);
    CPPUNIT_ASSERT(topic->getPhysicalName().find(".Test.Topic") != std::string::npos);

    Pointer<cms::Destination> cmsDest = target.dynamicCast<cms::Destination>();

    Pointer<ActiveMQDestination> cmstopic(AdvisorySupport::getFullAdvisoryTopic(cmsDest.get()));
    CPPUNIT_ASSERT(cmstopic != NULL);
    CPPUNIT_ASSERT(cmstopic->isComposite() == false);
    CPPUNIT_ASSERT(cmstopic->isAdvisory() == true);
    CPPUNIT_ASSERT(cmstopic->isTopic());
    CPPUNIT_ASSERT(cmstopic->isTemporary() == false);
    CPPUNIT_ASSERT(cmstopic->getPhysicalName().find(AdvisorySupport::FULL_TOPIC_PREFIX) == 0);
    CPPUNIT_ASSERT(cmstopic->getPhysicalName().find(".Test.Topic") != std::string::npos);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException when passed a NULL destination",
        AdvisorySupport::getFullAdvisoryTopic((ActiveMQDestination*)NULL),
        NullPointerException);
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException when passed a NULL destination",
        AdvisorySupport::getFullAdvisoryTopic((cms::Destination*)NULL),
        NullPointerException);
}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testGetDestinationAdvisoryTopic() {

    Pointer<ActiveMQDestination> target(new ActiveMQTopic("Test.Topic"));

    Pointer<ActiveMQDestination> topic(AdvisorySupport::getDestinationAdvisoryTopic(target.get()));
    CPPUNIT_ASSERT(topic != NULL);
    CPPUNIT_ASSERT(topic->isComposite() == false);
    CPPUNIT_ASSERT(topic->isAdvisory() == true);
    CPPUNIT_ASSERT(topic->isTopic());
    CPPUNIT_ASSERT(topic->isTemporary() == false);
    CPPUNIT_ASSERT(topic->getPhysicalName().find(AdvisorySupport::ADVISORY_TOPIC_PREFIX) == 0);
    CPPUNIT_ASSERT(topic->getPhysicalName().find(".Topic") != std::string::npos);

    Pointer<cms::Destination> cmsDest = target.dynamicCast<cms::Destination>();

    Pointer<ActiveMQDestination> cmstopic(AdvisorySupport::getDestinationAdvisoryTopic(cmsDest.get()));
    CPPUNIT_ASSERT(cmstopic != NULL);
    CPPUNIT_ASSERT(cmstopic->isComposite() == false);
    CPPUNIT_ASSERT(cmstopic->isAdvisory() == true);
    CPPUNIT_ASSERT(cmstopic->isTopic());
    CPPUNIT_ASSERT(cmstopic->isTemporary() == false);
    CPPUNIT_ASSERT(cmstopic->getPhysicalName().find(AdvisorySupport::ADVISORY_TOPIC_PREFIX) == 0);
    CPPUNIT_ASSERT(cmstopic->getPhysicalName().find(".Topic") != std::string::npos);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException when passed a NULL destination",
        AdvisorySupport::getDestinationAdvisoryTopic((ActiveMQDestination*)NULL),
        NullPointerException);
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException when passed a NULL destination",
        AdvisorySupport::getDestinationAdvisoryTopic((cms::Destination*)NULL),
        NullPointerException);
}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testIsDestinationAdvisoryTopic() {

    Pointer<ActiveMQDestination> target(new ActiveMQTopic("Test.Topic"));
    Pointer<ActiveMQDestination> topic(AdvisorySupport::getDestinationAdvisoryTopic(target.get()));
    CPPUNIT_ASSERT(!AdvisorySupport::isDestinationAdvisoryTopic(target.get()));
    CPPUNIT_ASSERT(AdvisorySupport::isDestinationAdvisoryTopic(topic.get()));

    Pointer<cms::Destination> cmsDest = target.dynamicCast<cms::Destination>();
    Pointer<ActiveMQDestination> cmstopic(AdvisorySupport::getDestinationAdvisoryTopic(cmsDest.get()));
    CPPUNIT_ASSERT(!AdvisorySupport::isDestinationAdvisoryTopic(cmsDest.get()));
    CPPUNIT_ASSERT(AdvisorySupport::isDestinationAdvisoryTopic(cmstopic.get()));

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException when passed a NULL destination",
        AdvisorySupport::isDestinationAdvisoryTopic((ActiveMQDestination*)NULL),
        NullPointerException);
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException when passed a NULL destination",
        AdvisorySupport::isDestinationAdvisoryTopic((cms::Destination*)NULL),
        NullPointerException);
}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testIsTempDestinationAdvisoryTopic() {

    Pointer<ActiveMQDestination> target(new ActiveMQTempTopic("Test.Topic"));
    Pointer<ActiveMQDestination> topic(AdvisorySupport::getDestinationAdvisoryTopic(target.get()));
    CPPUNIT_ASSERT(!AdvisorySupport::isTempDestinationAdvisoryTopic(target.get()));
    CPPUNIT_ASSERT(AdvisorySupport::isTempDestinationAdvisoryTopic(topic.get()));

    Pointer<cms::Destination> cmsDest = target.dynamicCast<cms::Destination>();
    Pointer<ActiveMQDestination> cmstopic(AdvisorySupport::getDestinationAdvisoryTopic(cmsDest.get()));
    CPPUNIT_ASSERT(!AdvisorySupport::isTempDestinationAdvisoryTopic(cmsDest.get()));
    CPPUNIT_ASSERT(AdvisorySupport::isTempDestinationAdvisoryTopic(cmstopic.get()));

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException when passed a NULL destination",
        AdvisorySupport::isTempDestinationAdvisoryTopic((ActiveMQDestination*)NULL),
        NullPointerException);
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException when passed a NULL destination",
        AdvisorySupport::isTempDestinationAdvisoryTopic((cms::Destination*)NULL),
        NullPointerException);
}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testIsAdvisoryTopic() {

    Pointer<ActiveMQDestination> target(new ActiveMQTempTopic("Test.Topic"));
    Pointer<ActiveMQDestination> topic(AdvisorySupport::getConsumerAdvisoryTopic(target.get()));
    CPPUNIT_ASSERT(!AdvisorySupport::isAdvisoryTopic(target.get()));
    CPPUNIT_ASSERT(AdvisorySupport::isAdvisoryTopic(topic.get()));

    Pointer<cms::Destination> cmsDest = target.dynamicCast<cms::Destination>();
    Pointer<ActiveMQDestination> cmstopic(AdvisorySupport::getConsumerAdvisoryTopic(cmsDest.get()));
    CPPUNIT_ASSERT(!AdvisorySupport::isAdvisoryTopic(cmsDest.get()));
    CPPUNIT_ASSERT(AdvisorySupport::isAdvisoryTopic(cmstopic.get()));

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException when passed a NULL destination",
        AdvisorySupport::isAdvisoryTopic((ActiveMQDestination*)NULL),
        NullPointerException);
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException when passed a NULL destination",
        AdvisorySupport::isAdvisoryTopic((cms::Destination*)NULL),
        NullPointerException);
}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testIsConnectionAdvisoryTopic() {

    Pointer<ActiveMQDestination> target(new ActiveMQTempTopic("Test.Topic"));
    Pointer<ActiveMQDestination> topic(AdvisorySupport::getConnectionAdvisoryTopic());
    CPPUNIT_ASSERT(!AdvisorySupport::isConnectionAdvisoryTopic(target.get()));
    CPPUNIT_ASSERT(AdvisorySupport::isConnectionAdvisoryTopic(topic.get()));

    Pointer<cms::Destination> cmsDest = target.dynamicCast<cms::Destination>();
    Pointer<ActiveMQDestination> cmstopic(AdvisorySupport::getConnectionAdvisoryTopic());
    CPPUNIT_ASSERT(!AdvisorySupport::isConnectionAdvisoryTopic(cmsDest.get()));
    CPPUNIT_ASSERT(AdvisorySupport::isConnectionAdvisoryTopic(cmstopic.get()));

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException when passed a NULL destination",
        AdvisorySupport::isConnectionAdvisoryTopic((ActiveMQDestination*)NULL),
        NullPointerException);
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException when passed a NULL destination",
        AdvisorySupport::isConnectionAdvisoryTopic((cms::Destination*)NULL),
        NullPointerException);
}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testIsProducerAdvisoryTopic() {

    Pointer<ActiveMQDestination> target(new ActiveMQTempTopic("Test.Topic"));
    Pointer<ActiveMQDestination> topic(AdvisorySupport::getProducerAdvisoryTopic(target.get()));
    CPPUNIT_ASSERT(!AdvisorySupport::isProducerAdvisoryTopic(target.get()));
    CPPUNIT_ASSERT(AdvisorySupport::isProducerAdvisoryTopic(topic.get()));

    Pointer<cms::Destination> cmsDest = target.dynamicCast<cms::Destination>();
    Pointer<ActiveMQDestination> cmstopic(AdvisorySupport::getProducerAdvisoryTopic(cmsDest.get()));
    CPPUNIT_ASSERT(!AdvisorySupport::isProducerAdvisoryTopic(cmsDest.get()));
    CPPUNIT_ASSERT(AdvisorySupport::isProducerAdvisoryTopic(cmstopic.get()));

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException when passed a NULL destination",
        AdvisorySupport::isProducerAdvisoryTopic((ActiveMQDestination*)NULL),
        NullPointerException);
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException when passed a NULL destination",
        AdvisorySupport::isProducerAdvisoryTopic((cms::Destination*)NULL),
        NullPointerException);
}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testIsConsumerAdvisoryTopic() {

    Pointer<ActiveMQDestination> target(new ActiveMQTempTopic("Test.Topic"));
    Pointer<ActiveMQDestination> topic(AdvisorySupport::getConsumerAdvisoryTopic(target.get()));
    CPPUNIT_ASSERT(!AdvisorySupport::isConsumerAdvisoryTopic(target.get()));
    CPPUNIT_ASSERT(AdvisorySupport::isConsumerAdvisoryTopic(topic.get()));

    Pointer<cms::Destination> cmsDest = target.dynamicCast<cms::Destination>();
    Pointer<ActiveMQDestination> cmstopic(AdvisorySupport::getConsumerAdvisoryTopic(cmsDest.get()));
    CPPUNIT_ASSERT(!AdvisorySupport::isConsumerAdvisoryTopic(cmsDest.get()));
    CPPUNIT_ASSERT(AdvisorySupport::isConsumerAdvisoryTopic(cmstopic.get()));

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException when passed a NULL destination",
        AdvisorySupport::isConsumerAdvisoryTopic((ActiveMQDestination*)NULL),
        NullPointerException);
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException when passed a NULL destination",
        AdvisorySupport::isConsumerAdvisoryTopic((cms::Destination*)NULL),
        NullPointerException);
}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testIsSlowConsumerAdvisoryTopic() {

    Pointer<ActiveMQDestination> target(new ActiveMQTempTopic("Test.Topic"));
    Pointer<ActiveMQDestination> topic(AdvisorySupport::getSlowConsumerAdvisoryTopic(target.get()));
    CPPUNIT_ASSERT(!AdvisorySupport::isSlowConsumerAdvisoryTopic(target.get()));
    CPPUNIT_ASSERT(AdvisorySupport::isSlowConsumerAdvisoryTopic(topic.get()));

    Pointer<cms::Destination> cmsDest = target.dynamicCast<cms::Destination>();
    Pointer<ActiveMQDestination> cmstopic(AdvisorySupport::getSlowConsumerAdvisoryTopic(cmsDest.get()));
    CPPUNIT_ASSERT(!AdvisorySupport::isSlowConsumerAdvisoryTopic(cmsDest.get()));
    CPPUNIT_ASSERT(AdvisorySupport::isSlowConsumerAdvisoryTopic(cmstopic.get()));

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException when passed a NULL destination",
        AdvisorySupport::isSlowConsumerAdvisoryTopic((ActiveMQDestination*)NULL),
        NullPointerException);
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException when passed a NULL destination",
        AdvisorySupport::isSlowConsumerAdvisoryTopic((cms::Destination*)NULL),
        NullPointerException);
}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testIsFastProducerAdvisoryTopic() {

    Pointer<ActiveMQDestination> target(new ActiveMQTempTopic("Test.Topic"));
    Pointer<ActiveMQDestination> topic(AdvisorySupport::getFastProducerAdvisoryTopic(target.get()));
    CPPUNIT_ASSERT(!AdvisorySupport::isFastProducerAdvisoryTopic(target.get()));
    CPPUNIT_ASSERT(AdvisorySupport::isFastProducerAdvisoryTopic(topic.get()));

    Pointer<cms::Destination> cmsDest = target.dynamicCast<cms::Destination>();
    Pointer<ActiveMQDestination> cmstopic(AdvisorySupport::getFastProducerAdvisoryTopic(cmsDest.get()));
    CPPUNIT_ASSERT(!AdvisorySupport::isFastProducerAdvisoryTopic(cmsDest.get()));
    CPPUNIT_ASSERT(AdvisorySupport::isFastProducerAdvisoryTopic(cmstopic.get()));

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException when passed a NULL destination",
        AdvisorySupport::isFastProducerAdvisoryTopic((ActiveMQDestination*)NULL),
        NullPointerException);
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException when passed a NULL destination",
        AdvisorySupport::isFastProducerAdvisoryTopic((cms::Destination*)NULL),
        NullPointerException);
}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testIsMessageConsumedAdvisoryTopic() {

    Pointer<ActiveMQDestination> target(new ActiveMQTempTopic("Test.Topic"));
    Pointer<ActiveMQDestination> topic(AdvisorySupport::getMessageConsumedAdvisoryTopic(target.get()));
    CPPUNIT_ASSERT(!AdvisorySupport::isMessageConsumedAdvisoryTopic(target.get()));
    CPPUNIT_ASSERT(AdvisorySupport::isMessageConsumedAdvisoryTopic(topic.get()));

    Pointer<cms::Destination> cmsDest = target.dynamicCast<cms::Destination>();
    Pointer<ActiveMQDestination> cmstopic(AdvisorySupport::getMessageConsumedAdvisoryTopic(cmsDest.get()));
    CPPUNIT_ASSERT(!AdvisorySupport::isMessageConsumedAdvisoryTopic(cmsDest.get()));
    CPPUNIT_ASSERT(AdvisorySupport::isMessageConsumedAdvisoryTopic(cmstopic.get()));

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException when passed a NULL destination",
        AdvisorySupport::isMessageConsumedAdvisoryTopic((ActiveMQDestination*)NULL),
        NullPointerException);
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException when passed a NULL destination",
        AdvisorySupport::isMessageConsumedAdvisoryTopic((cms::Destination*)NULL),
        NullPointerException);
}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testIsMasterBrokerAdvisoryTopic() {

    Pointer<ActiveMQDestination> target(new ActiveMQTempTopic("Test.Topic"));
    Pointer<ActiveMQDestination> topic(AdvisorySupport::getMasterBrokerAdvisoryTopic());
    CPPUNIT_ASSERT(!AdvisorySupport::isMasterBrokerAdvisoryTopic(target.get()));
    CPPUNIT_ASSERT(AdvisorySupport::isMasterBrokerAdvisoryTopic(topic.get()));

    Pointer<cms::Destination> cmsDest = target.dynamicCast<cms::Destination>();
    Pointer<ActiveMQDestination> cmstopic(AdvisorySupport::getMasterBrokerAdvisoryTopic());
    CPPUNIT_ASSERT(!AdvisorySupport::isMasterBrokerAdvisoryTopic(cmsDest.get()));
    CPPUNIT_ASSERT(AdvisorySupport::isMasterBrokerAdvisoryTopic(cmstopic.get()));

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException when passed a NULL destination",
        AdvisorySupport::isMasterBrokerAdvisoryTopic((ActiveMQDestination*)NULL),
        NullPointerException);
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException when passed a NULL destination",
        AdvisorySupport::isMasterBrokerAdvisoryTopic((cms::Destination*)NULL),
        NullPointerException);
}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testIsMessageDeliveredAdvisoryTopic() {

    Pointer<ActiveMQDestination> target(new ActiveMQTempTopic("Test.Topic"));
    Pointer<ActiveMQDestination> topic(AdvisorySupport::getMessageDeliveredAdvisoryTopic(target.get()));
    CPPUNIT_ASSERT(!AdvisorySupport::isMessageDeliveredAdvisoryTopic(target.get()));
    CPPUNIT_ASSERT(AdvisorySupport::isMessageDeliveredAdvisoryTopic(topic.get()));

    Pointer<cms::Destination> cmsDest = target.dynamicCast<cms::Destination>();
    Pointer<ActiveMQDestination> cmstopic(AdvisorySupport::getMessageDeliveredAdvisoryTopic(cmsDest.get()));
    CPPUNIT_ASSERT(!AdvisorySupport::isMessageDeliveredAdvisoryTopic(cmsDest.get()));
    CPPUNIT_ASSERT(AdvisorySupport::isMessageDeliveredAdvisoryTopic(cmstopic.get()));

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException when passed a NULL destination",
        AdvisorySupport::isMessageDeliveredAdvisoryTopic((ActiveMQDestination*)NULL),
        NullPointerException);
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException when passed a NULL destination",
        AdvisorySupport::isMessageDeliveredAdvisoryTopic((cms::Destination*)NULL),
        NullPointerException);
}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testIsMessageDiscardedAdvisoryTopic() {

    Pointer<ActiveMQDestination> target(new ActiveMQTempTopic("Test.Topic"));
    Pointer<ActiveMQDestination> topic(AdvisorySupport::getMessageDiscardedAdvisoryTopic(target.get()));
    CPPUNIT_ASSERT(!AdvisorySupport::isMessageDiscardedAdvisoryTopic(target.get()));
    CPPUNIT_ASSERT(AdvisorySupport::isMessageDiscardedAdvisoryTopic(topic.get()));

    Pointer<cms::Destination> cmsDest = target.dynamicCast<cms::Destination>();
    Pointer<ActiveMQDestination> cmstopic(AdvisorySupport::getMessageDiscardedAdvisoryTopic(cmsDest.get()));
    CPPUNIT_ASSERT(!AdvisorySupport::isMessageDiscardedAdvisoryTopic(cmsDest.get()));
    CPPUNIT_ASSERT(AdvisorySupport::isMessageDiscardedAdvisoryTopic(cmstopic.get()));

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException when passed a NULL destination",
        AdvisorySupport::isMessageDiscardedAdvisoryTopic((ActiveMQDestination*)NULL),
        NullPointerException);
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException when passed a NULL destination",
        AdvisorySupport::isMessageDiscardedAdvisoryTopic((cms::Destination*)NULL),
        NullPointerException);
}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testIsMessageDLQdAdvisoryTopic() {

    Pointer<ActiveMQDestination> target(new ActiveMQTempTopic("Test.Topic"));
    Pointer<ActiveMQDestination> topic(AdvisorySupport::getMessageDLQdAdvisoryTopic(target.get()));
    CPPUNIT_ASSERT(!AdvisorySupport::isMessageDLQdAdvisoryTopic(target.get()));
    CPPUNIT_ASSERT(AdvisorySupport::isMessageDLQdAdvisoryTopic(topic.get()));

    Pointer<cms::Destination> cmsDest = target.dynamicCast<cms::Destination>();
    Pointer<ActiveMQDestination> cmstopic(AdvisorySupport::getMessageDLQdAdvisoryTopic(cmsDest.get()));
    CPPUNIT_ASSERT(!AdvisorySupport::isMessageDLQdAdvisoryTopic(cmsDest.get()));
    CPPUNIT_ASSERT(AdvisorySupport::isMessageDLQdAdvisoryTopic(cmstopic.get()));

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException when passed a NULL destination",
        AdvisorySupport::isMessageDLQdAdvisoryTopic((ActiveMQDestination*)NULL),
        NullPointerException);
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException when passed a NULL destination",
        AdvisorySupport::isMessageDLQdAdvisoryTopic((cms::Destination*)NULL),
        NullPointerException);
}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testIsFullAdvisoryTopic() {

    Pointer<ActiveMQDestination> target(new ActiveMQTempTopic("Test.Topic"));
    Pointer<ActiveMQDestination> topic(AdvisorySupport::getFullAdvisoryTopic(target.get()));
    CPPUNIT_ASSERT(!AdvisorySupport::isFullAdvisoryTopic(target.get()));
    CPPUNIT_ASSERT(AdvisorySupport::isFullAdvisoryTopic(topic.get()));

    Pointer<cms::Destination> cmsDest = target.dynamicCast<cms::Destination>();
    Pointer<ActiveMQDestination> cmstopic(AdvisorySupport::getFullAdvisoryTopic(cmsDest.get()));
    CPPUNIT_ASSERT(!AdvisorySupport::isFullAdvisoryTopic(cmsDest.get()));
    CPPUNIT_ASSERT(AdvisorySupport::isFullAdvisoryTopic(cmstopic.get()));

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException when passed a NULL destination",
        AdvisorySupport::isFullAdvisoryTopic((ActiveMQDestination*)NULL),
        NullPointerException);
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException when passed a NULL destination",
        AdvisorySupport::isFullAdvisoryTopic((cms::Destination*)NULL),
        NullPointerException);
}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testIsNetworkBridgeAdvisoryTopic() {

    Pointer<ActiveMQDestination> target(new ActiveMQTempTopic("Test.Topic"));
    Pointer<ActiveMQDestination> topic(AdvisorySupport::getNetworkBridgeAdvisoryTopic());
    CPPUNIT_ASSERT(!AdvisorySupport::isNetworkBridgeAdvisoryTopic(target.get()));
    CPPUNIT_ASSERT(AdvisorySupport::isNetworkBridgeAdvisoryTopic(topic.get()));

    Pointer<cms::Destination> cmsDest = target.dynamicCast<cms::Destination>();
    Pointer<ActiveMQDestination> cmstopic(AdvisorySupport::getNetworkBridgeAdvisoryTopic());
    CPPUNIT_ASSERT(!AdvisorySupport::isNetworkBridgeAdvisoryTopic(cmsDest.get()));
    CPPUNIT_ASSERT(AdvisorySupport::isNetworkBridgeAdvisoryTopic(cmstopic.get()));

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException when passed a NULL destination",
        AdvisorySupport::isNetworkBridgeAdvisoryTopic((ActiveMQDestination*)NULL),
        NullPointerException);
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NullPointerException when passed a NULL destination",
        AdvisorySupport::isNetworkBridgeAdvisoryTopic((cms::Destination*)NULL),
        NullPointerException);
}
