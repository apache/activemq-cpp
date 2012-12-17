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

#include "StompWireFormatTest.h"

#include <activemq/wireformat/stomp/StompFrame.h>
#include <activemq/wireformat/stomp/StompHelper.h>
#include <activemq/wireformat/stomp/StompWireFormat.h>

using namespace activemq;
using namespace activemq::wireformat;
using namespace activemq::wireformat::stomp;

////////////////////////////////////////////////////////////////////////////////
StompWireFormatTest::StompWireFormatTest() {
}

////////////////////////////////////////////////////////////////////////////////
StompWireFormatTest::~StompWireFormatTest() {
}

////////////////////////////////////////////////////////////////////////////////
void StompWireFormatTest::testChangeDestinationPrefix() {

    StompWireFormat wireformat;
    wireformat.setTopicPrefix("mytopics://");
    StompHelper helper(&wireformat);

    StompFrame frame;
    frame.setCommand("MESSAGE");
    frame.setProperty("destination", "mytopics://SomeTopic");
    frame.setProperty("subscription", "connection:1:1:0:1");
    frame.setProperty("message-id", "connection:1:1:0:1");
}
