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

#include "StompHelperTest.h"

#include <activemq/commands/ActiveMQTopic.h>

#include <activemq/wireformat/stomp/StompFrame.h>
#include <activemq/wireformat/stomp/StompHelper.h>
#include <activemq/wireformat/stomp/StompWireFormat.h>

using namespace activemq;
using namespace activemq::commands;
using namespace activemq::wireformat;
using namespace activemq::wireformat::stomp;

////////////////////////////////////////////////////////////////////////////////
StompHelperTest::StompHelperTest() {
}

////////////////////////////////////////////////////////////////////////////////
StompHelperTest::~StompHelperTest() {
}

////////////////////////////////////////////////////////////////////////////////
void StompHelperTest::testConvertDestinationFromString() {

    StompWireFormat wireformat;
    wireformat.setTopicPrefix("mytopics://");
    StompHelper helper(&wireformat);

    Pointer<ActiveMQDestination> destination = helper.convertDestination("mytopics://SomeTopic");

    CPPUNIT_ASSERT_EQUAL(std::string("SomeTopic"), destination->getPhysicalName());
}

////////////////////////////////////////////////////////////////////////////////
void StompHelperTest::testConvertDestinationFromCommand() {

    StompWireFormat wireformat;
    wireformat.setTopicPrefix("mytopics://");
    StompHelper helper(&wireformat);

    Pointer<ActiveMQDestination> destination(new ActiveMQTopic("SomeTopic"));

    std::string result = helper.convertDestination(destination);

    CPPUNIT_ASSERT_EQUAL(std::string("mytopics://SomeTopic"), result);
}
