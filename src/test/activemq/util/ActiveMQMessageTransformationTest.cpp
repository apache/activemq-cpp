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

#include "ActiveMQMessageTransformationTest.h"

#include <activemq/commands/ActiveMQDestination.h>
#include <activemq/commands/ActiveMQTopic.h>
#include <activemq/commands/ActiveMQQueue.h>
#include <activemq/util/ActiveMQProperties.h>
#include <activemq/util/ActiveMQMessageTransformation.h>

#include <cms/Destination.h>
#include <cms/Destination.h>
#include <cms/Destination.h>

#include <decaf/lang/Pointer.h>
#include <decaf/lang/exceptions/NullPointerException.h>

using namespace activemq;
using namespace activemq::util;
using namespace activemq::commands;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
namespace {

    class CustomCmsTopic : public cms::Topic {
    private:

        std::string name;
        activemq::util::ActiveMQProperties props;

    public:

        CustomCmsTopic() : cms::Topic(), name("TEST-CMS-TOPIC"), props() {

        }

        virtual std::string getTopicName() const {
            return name;
        }

        virtual cms::Destination::DestinationType getDestinationType() const {
            return cms::Destination::TOPIC;
        }

        virtual cms::Destination* clone() const {
            return new CustomCmsTopic();
        }

        virtual void copy(const cms::Destination& source) {
        }

        virtual bool equals(const cms::Destination& other) const {

            if (dynamic_cast<const CustomCmsTopic*>(&other) != NULL) {
                return true;
            }

            return false;
        }

        virtual const cms::CMSProperties& getCMSProperties() const {
            return props;
        }

    };

}

////////////////////////////////////////////////////////////////////////////////
ActiveMQMessageTransformationTest::ActiveMQMessageTransformationTest() {
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQMessageTransformationTest::~ActiveMQMessageTransformationTest() {
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMessageTransformationTest::testTransformDestination() {

    CustomCmsTopic customTopic;
    const ActiveMQDestination* transformed;

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an NullPointerException",
        ActiveMQMessageTransformation::transformDestination(&customTopic, NULL),
        NullPointerException);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an NullPointerException",
        ActiveMQMessageTransformation::transformDestination(NULL, NULL),
        NullPointerException);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an NullPointerException",
        ActiveMQMessageTransformation::transformDestination(NULL, &transformed),
        NullPointerException);

    CPPUNIT_ASSERT(ActiveMQMessageTransformation::transformDestination(&customTopic, &transformed));
    CPPUNIT_ASSERT(transformed != NULL);
    CPPUNIT_ASSERT(transformed->isTopic());
    CPPUNIT_ASSERT(!transformed->isQueue());
    CPPUNIT_ASSERT(!transformed->isAdvisory());
    CPPUNIT_ASSERT(!transformed->isComposite());
    CPPUNIT_ASSERT(!transformed->isTemporary());
    CPPUNIT_ASSERT(!transformed->isWildcard());

    CPPUNIT_ASSERT_EQUAL(customTopic.getTopicName(), transformed->getPhysicalName());
    delete transformed;

    ActiveMQQueue queue("ActiveMQ.Test.Queue");

    CPPUNIT_ASSERT(!ActiveMQMessageTransformation::transformDestination(&queue, &transformed));
    CPPUNIT_ASSERT(transformed != NULL);
    CPPUNIT_ASSERT(!transformed->isTopic());
    CPPUNIT_ASSERT(transformed->isQueue());
}
