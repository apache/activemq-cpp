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

#include <decaf/lang/Pointer.h>

using namespace activemq;
using namespace activemq::util;
using namespace activemq::commands;
using namespace decaf;
using namespace decaf::lang;

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
        CPPUNIT_ASSERT(topic->isTemporary() == false);
        CPPUNIT_ASSERT(topic->getPhysicalName().find(AdvisorySupport::ADVISORY_TOPIC_PREFIX) == 0);
    }

    CPPUNIT_ASSERT_EQUAL(topics.size(), topics2.size());
}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testGetConnectionAdvisoryTopic() {

    Pointer<ActiveMQDestination> topic(AdvisorySupport::getConnectionAdvisoryTopic());
    CPPUNIT_ASSERT(topic != NULL);
    CPPUNIT_ASSERT(topic->isComposite());
    CPPUNIT_ASSERT(topic->isTopic());
    CPPUNIT_ASSERT(topic->isTemporary() == false);
    CPPUNIT_ASSERT(topic->getPhysicalName().find(AdvisorySupport::ADVISORY_TOPIC_PREFIX) == 0);
    CPPUNIT_ASSERT(topic->getPhysicalName().find(".Connection") != std::string::npos);
}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testGetQueueAdvisoryTopic() {

    Pointer<ActiveMQDestination> topic(AdvisorySupport::getQueueAdvisoryTopic());
    CPPUNIT_ASSERT(topic != NULL);
    CPPUNIT_ASSERT(topic->isComposite());
    CPPUNIT_ASSERT(topic->isTopic());
    CPPUNIT_ASSERT(topic->isTemporary() == false);
    CPPUNIT_ASSERT(topic->getPhysicalName().find(AdvisorySupport::ADVISORY_TOPIC_PREFIX) == 0);
    CPPUNIT_ASSERT(topic->getPhysicalName().find(".Queue") != std::string::npos);
}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testGetTopicAdvisoryTopic() {

    Pointer<ActiveMQDestination> topic(AdvisorySupport::getTopicAdvisoryTopic());
    CPPUNIT_ASSERT(topic != NULL);
    CPPUNIT_ASSERT(topic->isComposite());
    CPPUNIT_ASSERT(topic->isTopic());
    CPPUNIT_ASSERT(topic->isTemporary() == false);
    CPPUNIT_ASSERT(topic->getPhysicalName().find(AdvisorySupport::ADVISORY_TOPIC_PREFIX) == 0);
    CPPUNIT_ASSERT(topic->getPhysicalName().find(".Topic") != std::string::npos);
}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testGetTempQueueAdvisoryTopic() {

    Pointer<ActiveMQDestination> topic(AdvisorySupport::getTempQueueAdvisoryTopic());
    CPPUNIT_ASSERT(topic != NULL);
    CPPUNIT_ASSERT(topic->isComposite());
    CPPUNIT_ASSERT(topic->isTopic());
    CPPUNIT_ASSERT(topic->isTemporary() == false);
    CPPUNIT_ASSERT(topic->getPhysicalName().find(AdvisorySupport::ADVISORY_TOPIC_PREFIX) == 0);
    CPPUNIT_ASSERT(topic->getPhysicalName().find(".TempQueue") != std::string::npos);
}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testGetTempTopicAdvisoryTopic() {

    Pointer<ActiveMQDestination> topic(AdvisorySupport::getTempTopicAdvisoryTopic());
    CPPUNIT_ASSERT(topic != NULL);
    CPPUNIT_ASSERT(topic->isComposite());
    CPPUNIT_ASSERT(topic->isTopic());
    CPPUNIT_ASSERT(topic->isTemporary() == false);
    CPPUNIT_ASSERT(topic->getPhysicalName().find(AdvisorySupport::ADVISORY_TOPIC_PREFIX) == 0);
    CPPUNIT_ASSERT(topic->getPhysicalName().find(".TempTopic") != std::string::npos);
}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testGetConsumerAdvisoryTopic() {

}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testGetProducerAdvisoryTopic() {

}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testGetExpiredMessageTopic() {

}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testGetExpiredTopicMessageAdvisoryTopic() {

}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testGetExpiredQueueMessageAdvisoryTopic() {

}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testGetNoConsumersAdvisoryTopic() {

}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testGetNoTopicConsumersAdvisoryTopic() {

}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testGetNoQueueConsumersAdvisoryTopic() {

}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testGetSlowConsumerAdvisoryTopic() {

}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testGetFastProducerAdvisoryTopic() {

}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testGetMessageDiscardedAdvisoryTopic() {

}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testGetMessageDeliveredAdvisoryTopic() {

}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testGetMessageConsumedAdvisoryTopic() {

}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testGetMessageDLQdAdvisoryTopic() {

}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testGetMasterBrokerAdvisoryTopic() {

}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testGetNetworkBridgeAdvisoryTopic() {

}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testGetFullAdvisoryTopic() {

}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testGetDestinationAdvisoryTopic() {

}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testIsDestinationAdvisoryTopic() {

}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testIsTempDestinationAdvisoryTopic() {

}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testIsAdvisoryTopic() {

}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testIsConnectionAdvisoryTopic() {

}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testIsProducerAdvisoryTopic() {

}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testIsConsumerAdvisoryTopic() {

}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testIsSlowConsumerAdvisoryTopic() {

}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testIsFastProducerAdvisoryTopic() {

}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testIsMessageConsumedAdvisoryTopic() {

}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testIsMasterBrokerAdvisoryTopic() {

}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testIsMessageDeliveredAdvisoryTopic() {

}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testIsMessageDiscardedAdvisoryTopic() {

}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testIsMessageDLQdAdvisoryTopic() {

}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testIsFullAdvisoryTopic() {

}

////////////////////////////////////////////////////////////////////////////////
void AdvisorySupportTest::testIsNetworkBridgeAdvisoryTopic() {

}
