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

#include "ActiveMQTempQueueTest.h"

#include <decaf/util/UUID.h>
#include <activemq/commands/ActiveMQTempQueue.h>

using namespace std;
using namespace activemq;
using namespace activemq::util;
using namespace activemq::commands;

////////////////////////////////////////////////////////////////////////////////
void ActiveMQTempQueueTest::test()
{
    ActiveMQTempQueue myQueue;

    CPPUNIT_ASSERT( myQueue.getDestinationType() == cms::Destination::TEMPORARY_QUEUE );

    myQueue.setPhysicalName("ID:SomeValue:0:1");
    std::string connectionId = myQueue.getConnectionId();

    CPPUNIT_ASSERT_EQUAL_MESSAGE("ConnectionId did not parse correctly",
        std::string("ID:SomeValue:0"), myQueue.getConnectionId());

    myQueue.setPhysicalName("");
    myQueue.setPhysicalName("ID:SomeValue:0:A");
    connectionId = myQueue.getConnectionId();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("ConnectionId should not have parsed",
        std::string("ID:SomeValue:0"), myQueue.getConnectionId());

    myQueue.setPhysicalName("");
    myQueue.setPhysicalName("SomeValueThatWillNotParse");
    connectionId = myQueue.getConnectionId();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("ConnectionId should not have parsed",
        std::string(""), myQueue.getConnectionId());
}
