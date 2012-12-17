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

#include "StompWireFormatFactoryTest.h"

#include <decaf/util/Properties.h>

#include <activemq/wireformat/stomp/StompFrame.h>
#include <activemq/wireformat/stomp/StompWireFormat.h>
#include <activemq/wireformat/stomp/StompWireFormatFactory.h>

using namespace decaf;
using namespace decaf::util;
using namespace activemq;
using namespace activemq::wireformat;
using namespace activemq::wireformat::stomp;

////////////////////////////////////////////////////////////////////////////////
StompWireFormatFactoryTest::StompWireFormatFactoryTest() {
}

////////////////////////////////////////////////////////////////////////////////
StompWireFormatFactoryTest::~StompWireFormatFactoryTest() {
}

////////////////////////////////////////////////////////////////////////////////
void StompWireFormatFactoryTest::testCreateWireFormat() {

    StompWireFormatFactory factory;

    Properties properties;
    properties.setProperty("wireFormat.topicPrefix", "/test-topic/");
    properties.setProperty("wireFormat.queuePrefix", "/test-queue/");
    properties.setProperty("wireFormat.tempTopicPrefix", "/test-temp-topic/");
    properties.setProperty("wireFormat.tempQueuePrefix", "/test-temp-queue/");

    Pointer<WireFormat> format(factory.createWireFormat(properties));

    Pointer<StompWireFormat> stomp = format.dynamicCast<StompWireFormat>();

    CPPUNIT_ASSERT_EQUAL(std::string("/test-topic/"), stomp->getTopicPrefix());
    CPPUNIT_ASSERT_EQUAL(std::string("/test-queue/"), stomp->getQueuePrefix());
    CPPUNIT_ASSERT_EQUAL(std::string("/test-temp-topic/"), stomp->getTempTopicPrefix());
    CPPUNIT_ASSERT_EQUAL(std::string("/test-temp-queue/"), stomp->getTempQueuePrefix());
}
