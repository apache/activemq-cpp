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

#include <activemq/core/ActiveMQConnectionFactory.h>
#include <activemq/core/ActiveMQConnection.h>
#include <activemq/core/ActiveMQSession.h>
#include <activemq/commands/Message.h>
#include <activemq/commands/ConnectionInfo.h>
#include <activemq/exceptions/ActiveMQException.h>

#include <decaf/lang/exceptions/ClassCastException.h>
#include <decaf/lang/Pointer.h>
#include <decaf/lang/Thread.h>
#include <decaf/lang/Runnable.h>
#include <decaf/util/concurrent/TimeUnit.h>
#include <decaf/util/UUID.h>

#include <cms/ConnectionFactory.h>
#include <cms/Connection.h>
#include <cms/Session.h>
#include <cms/MessageConsumer.h>
#include <cms/MessageProducer.h>
#include <cms/MessageListener.h>
#include <cms/ConnectionFactory.h>
#include <cms/Connection.h>
#include <cms/Message.h>
#include <cms/TextMessage.h>

#include <memory>
#include "StompAdvisoryTest.h"

using namespace cms;
using namespace std;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace activemq;
using namespace activemq::core;
using namespace activemq::commands;
using namespace activemq::exceptions;
using namespace activemq::test;
using namespace activemq::test::stomp;

////////////////////////////////////////////////////////////////////////////////
StompAdvisoryTest::StompAdvisoryTest() {
}

////////////////////////////////////////////////////////////////////////////////
StompAdvisoryTest::~StompAdvisoryTest() {
}
