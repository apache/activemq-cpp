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

#include "ConnectionAuditTest.h"

#include <activemq/core/ConnectionAudit.h>
#include <activemq/core/Dispatcher.h>
#include <activemq/core/ActiveMQMessageAudit.h>
#include <activemq/util/IdGenerator.h>
#include <activemq/commands/Message.h>
#include <activemq/commands/ActiveMQDestination.h>
#include <activemq/commands/ActiveMQQueue.h>

#include <decaf/util/ArrayList.h>

using namespace std;
using namespace activemq;
using namespace activemq::core;
using namespace activemq::commands;
using namespace activemq::util;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::util;

////////////////////////////////////////////////////////////////////////////////
namespace {

    class MyDispatcher : public Dispatcher {
    public:

        virtual ~MyDispatcher() {}

        virtual void dispatch(const Pointer<commands::MessageDispatch>& message) {

        }

        virtual int getHashCode() const {
            return 1;
        }

    };
}

////////////////////////////////////////////////////////////////////////////////
ConnectionAuditTest::ConnectionAuditTest() {
}

////////////////////////////////////////////////////////////////////////////////
ConnectionAuditTest::~ConnectionAuditTest() {
}

////////////////////////////////////////////////////////////////////////////////
void ConnectionAuditTest::testConstructor1() {

    ConnectionAudit audit;
    CPPUNIT_ASSERT(audit.isCheckForDuplicates());
    CPPUNIT_ASSERT(audit.getAuditDepth() == ActiveMQMessageAudit::DEFAULT_WINDOW_SIZE);
    CPPUNIT_ASSERT(audit.getAuditMaximumProducerNumber() == ActiveMQMessageAudit::MAXIMUM_PRODUCER_COUNT);
}

////////////////////////////////////////////////////////////////////////////////
void ConnectionAuditTest::testConstructor2() {

    ConnectionAudit audit(100, 200);
    CPPUNIT_ASSERT(audit.isCheckForDuplicates());
    CPPUNIT_ASSERT(audit.getAuditDepth() == 100);
    CPPUNIT_ASSERT(audit.getAuditMaximumProducerNumber() == 200);
}

////////////////////////////////////////////////////////////////////////////////
void ConnectionAuditTest::testIsDuplicate() {

    int count = 10000;
    ConnectionAudit audit;
    ArrayList<Pointer<MessageId> > list;
    Pointer<MyDispatcher> dispatcher(new MyDispatcher);

    Pointer<ProducerId> pid(new ProducerId);
    pid->setConnectionId("test");
    pid->setSessionId(0);
    pid->setValue(1);

    Pointer<Message> message(new Message());

    for (int i = 0; i < count; i++) {
        Pointer<ActiveMQDestination> destination(new ActiveMQQueue("TEST.QUEUE"));
        message->setDestination(destination);

        Pointer<MessageId> id(new MessageId);
        id->setProducerId(pid);
        id->setProducerSequenceId(i);
        list.add(id);

        message->setMessageId(id);
        CPPUNIT_ASSERT(!audit.isDuplicate(dispatcher.get(), message));
    }

    int index = list.size() -1 -audit.getAuditDepth();
    for (; index < list.size(); index++) {
        Pointer<MessageId> id = list.get(index);
        message->setMessageId(id);
        CPPUNIT_ASSERT_MESSAGE(std::string() + "duplicate msg:" + id->toString(),
                               audit.isDuplicate(dispatcher.get(), message));
    }
}

////////////////////////////////////////////////////////////////////////////////
void ConnectionAuditTest::testRollbackDuplicate() {

    int count = 10000;
    ConnectionAudit audit;
    ArrayList<Pointer<MessageId> > list;
    Pointer<MyDispatcher> dispatcher(new MyDispatcher);

    Pointer<ProducerId> pid(new ProducerId);
    pid->setConnectionId("test");
    pid->setSessionId(0);
    pid->setValue(1);

    Pointer<ActiveMQDestination> destination(new ActiveMQQueue("TEST.QUEUE"));
    Pointer<Message> message(new Message());
    message->setDestination(destination);

    for (int i = 0; i < count; i++) {
        Pointer<MessageId> id(new MessageId);
        id->setProducerId(pid);
        id->setProducerSequenceId(i);
        list.add(id);

        message->setMessageId(id);
        CPPUNIT_ASSERT(!audit.isDuplicate(dispatcher.get(), message));
    }

    int index = list.size() -1 -audit.getAuditDepth();
    for (; index < list.size(); index++) {
        Pointer<MessageId> id = list.get(index);
        message->setMessageId(id);
        CPPUNIT_ASSERT_MESSAGE(std::string() + "duplicate msg:" + id->toString(),
                               audit.isDuplicate(dispatcher.get(), message));
        audit.rollbackDuplicate(dispatcher.get(), message);
        CPPUNIT_ASSERT_MESSAGE(std::string() + "duplicate msg:" + id->toString(),
                               !audit.isDuplicate(dispatcher.get(), message));
    }
}
