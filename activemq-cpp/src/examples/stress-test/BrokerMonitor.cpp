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

#include "BrokerMonitor.h"
#include "ConnectionFactoryMgr.h"
#include "TestSenderAndReceiver.h"

#include <cms/Session.h>
#include <cms/Message.h>
#include <cms/ConnectionFactory.h>

#include <activemq/cmsutil/MessageCreator.h>
#include <activemq/cmsutil/CmsTemplate.h>
#include <decaf/util/Iterator.h>

#include <stdio.h>

extern bool VERBOSE;

using namespace cms;
using namespace cms::stress;
using namespace activemq::cmsutil;
using namespace decaf::lang::exceptions;
using namespace decaf::lang;
using namespace decaf::util;
using namespace decaf::util::concurrent;

////////////////////////////////////////////////////////////////////////////////
BrokerMonitor::BrokerMonitor(const std::string& url, int interval, CountDownLatch* quit) :
    closing(false), brokerOk(false), url(url), interval(interval), brokerMonitorThread(), quit(quit) {
}

////////////////////////////////////////////////////////////////////////////////
BrokerMonitor::~BrokerMonitor() {
    try {
        close();
    } catch (...) {
    }
}

////////////////////////////////////////////////////////////////////////////////
void BrokerMonitor::close() {
    closing = true;
    if (brokerMonitorThread) {
        brokerMonitorThread->join();
        delete brokerMonitorThread;
        brokerMonitorThread = NULL;
    }
}

////////////////////////////////////////////////////////////////////////////////
void BrokerMonitor::start() {
    if (!brokerMonitorThread) {
        brokerMonitorThread = new Thread(this, "Message Broker Monitor Thread");
        brokerMonitorThread->start();
    }
}

////////////////////////////////////////////////////////////////////////////////
void BrokerMonitor::run() {
    ConnectionFactory* connectionFactory = ConnectionFactoryMgr::getConnectionFactory(url);
    CmsTemplate* cmsTemplate = createCmsTemplate(connectionFactory);

    while (!closing) {
        try {
            cmsTemplate->send(this);
            Message* message = cmsTemplate->receive();

            if (message) {
                delete message;
                if (VERBOSE) {
                    printf("%c", SYM_MON_GOOD);
                }
                brokerOk = true;
            } else {
                if (VERBOSE) {
                    printf("%c", SYM_MON_BAD);
                }
                brokerOk = false;
            }
        } catch (cms::CMSException& ex) {
            if (VERBOSE) {
                printf("%c", SYM_MON_CMS);
            }
            brokerOk = false;
        } catch (...) {
            if (VERBOSE) {
                printf("%c", SYM_MON_EXC);
            }
            brokerOk = false;
        }

        if (quit->await(interval)) {
            closing = true;
        }
    }

    delete cmsTemplate;
}

////////////////////////////////////////////////////////////////////////////////
Message* BrokerMonitor::createMessage(cms::Session* session) {
    Message* message = NULL;
    if (session) {
        message = session->createTextMessage("Heart Beat");
    }
    return message;
}

////////////////////////////////////////////////////////////////////////////////
CmsTemplate* BrokerMonitor::createCmsTemplate(ConnectionFactory* connectionFactory) {

    CmsTemplate* cmsTemplate = new CmsTemplate(connectionFactory);
    cmsTemplate->setDefaultDestinationName("cpp.CmsMessageHandler.BrokerMonitor.HeartBeatingChannel");
    cmsTemplate->setTimeToLive(1000);
    cmsTemplate->setReceiveTimeout(1000);

    return cmsTemplate;
}

////////////////////////////////////////////////////////////////////////////////
bool BrokerMonitor::isBrokerOk() {
    return brokerOk;
}
