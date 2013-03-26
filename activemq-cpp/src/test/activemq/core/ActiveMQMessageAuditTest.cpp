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

#include "ActiveMQMessageAuditTest.h"

#include <activemq/core/ActiveMQMessageAudit.h>
#include <activemq/util/IdGenerator.h>

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
ActiveMQMessageAuditTest::ActiveMQMessageAuditTest() {
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQMessageAuditTest::~ActiveMQMessageAuditTest() {
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMessageAuditTest::testIsDuplicateString() {

    int count = 10000;
    ActiveMQMessageAudit audit;
    IdGenerator idGen;

    ArrayList<std::string> list;
    for (int i = 0; i < count; i++) {
        std::string id = idGen.generateId();
        list.add(id);
        CPPUNIT_ASSERT(!audit.isDuplicate(id));
    }

    int index = list.size() -1 -audit.getAuditDepth();
    for (; index < list.size(); index++) {
        std::string id = list.get(index);
        CPPUNIT_ASSERT_MESSAGE("duplicate, id:" + id, audit.isDuplicate(id));
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMessageAuditTest::testIsDuplicateMessageId() {

    int count = 10000;
    ActiveMQMessageAudit audit;
    ArrayList<Pointer<MessageId> > list;

    Pointer<ProducerId> pid(new ProducerId);
    pid->setConnectionId("test");
    pid->setSessionId(0);
    pid->setValue(1);

    for (int i = 0; i < count; i++) {
        Pointer<MessageId> id(new MessageId);
        id->setProducerId(pid);
        id->setProducerSequenceId(i);
        list.add(id);
        CPPUNIT_ASSERT(!audit.isDuplicate(id));
    }

    int index = list.size() -1 -audit.getAuditDepth();
    for (; index < list.size(); index++) {
        Pointer<MessageId> id = list.get(index);
        CPPUNIT_ASSERT_MESSAGE(std::string() + "duplicate msg:" + id->toString(), audit.isDuplicate(id));
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMessageAuditTest::testRollbackString() {

    int count = 10000;
    ActiveMQMessageAudit audit;
    IdGenerator idGen;

    ArrayList<std::string> list;
    for (int i = 0; i < count; i++) {
        std::string id = idGen.generateId();
        list.add(id);
        CPPUNIT_ASSERT(!audit.isDuplicate(id));
    }

    int index = list.size() -1 -audit.getAuditDepth();
    for (; index < list.size(); index++) {
        std::string id = list.get(index);
        CPPUNIT_ASSERT_MESSAGE("duplicate, id:" + id, audit.isDuplicate(id));
        audit.rollback(id);
        CPPUNIT_ASSERT_MESSAGE(std::string() + "duplicate msg:" + id, !audit.isDuplicate(id));
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMessageAuditTest::testRollbackMessageId() {

    int count = 10000;
    ActiveMQMessageAudit audit;
    ArrayList<Pointer<MessageId> > list;

    Pointer<ProducerId> pid(new ProducerId);
    pid->setConnectionId("test");
    pid->setSessionId(0);
    pid->setValue(1);

    for (int i = 0; i < count; i++) {
        Pointer<MessageId> id(new MessageId);
        id->setProducerId(pid);
        id->setProducerSequenceId(i);
        list.add(id);
        CPPUNIT_ASSERT(!audit.isDuplicate(id));
    }

    int index = list.size() -1 -audit.getAuditDepth();
    for (; index < list.size(); index++) {
        Pointer<MessageId> id = list.get(index);
        CPPUNIT_ASSERT_MESSAGE(std::string() + "duplicate msg:" + id->toString(), audit.isDuplicate(id));
        audit.rollback(id);
        CPPUNIT_ASSERT_MESSAGE(std::string() + "duplicate msg:" + id->toString(), !audit.isDuplicate(id));
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMessageAuditTest::testIsInOrderString() {

    int count = 10000;
    ActiveMQMessageAudit audit;
    IdGenerator idGen;

    ArrayList<std::string> list;
    for (int i = 0; i < count; i++) {
        std::string id = idGen.generateId();
        if (i == 0) {
            CPPUNIT_ASSERT(!audit.isDuplicate(id));
            CPPUNIT_ASSERT(audit.isInOrder(id));
        }
        if (i > 1 && i % 2 != 0) {
            list.add(id);
        }
    }

    for (int i = 0; i < list.size(); i++) {
        std::string id = list.get(i);
        CPPUNIT_ASSERT_MESSAGE(std::string("Out of order msg: ") + id, !audit.isInOrder(id));
        CPPUNIT_ASSERT(!audit.isDuplicate(id));
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMessageAuditTest::testIsInOrderMessageId() {

    int count = 10000;
    ActiveMQMessageAudit audit;
    ArrayList<Pointer<MessageId> > list;

    Pointer<ProducerId> pid(new ProducerId);
    pid->setConnectionId("test");
    pid->setSessionId(0);
    pid->setValue(1);

    for (int i = 0; i < count; i++) {
        Pointer<MessageId> id(new MessageId);
        id->setProducerId(pid);
        id->setProducerSequenceId(i);

        if (i == 0) {
            CPPUNIT_ASSERT(!audit.isDuplicate(id));
            CPPUNIT_ASSERT(audit.isInOrder(id));
        }
        if (i > 1 && i % 2 != 0) {
            list.add(id);
        }
    }

    for (int i = 0; i < list.size(); i++) {
        Pointer<MessageId> mid = list.get(i);
        CPPUNIT_ASSERT_MESSAGE(std::string("Out of order msg: ") + mid->toString(), !audit.isInOrder(mid));
        CPPUNIT_ASSERT(!audit.isDuplicate(mid));
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMessageAuditTest::testGetLastSeqId() {

    int count = 10000;
    ActiveMQMessageAudit audit;
    ArrayList<Pointer<MessageId> > list;

    Pointer<ProducerId> pid(new ProducerId);
    pid->setConnectionId("test");
    pid->setSessionId(0);
    pid->setValue(1);
    Pointer<MessageId> id(new MessageId);
    id->setProducerId(pid);

    for (int i = 0; i < count; i++) {
        id->setProducerSequenceId(i);
        list.add(id);
        CPPUNIT_ASSERT(!audit.isDuplicate(id));
        CPPUNIT_ASSERT_EQUAL((long long)i, audit.getLastSeqId(pid));
    }

}
