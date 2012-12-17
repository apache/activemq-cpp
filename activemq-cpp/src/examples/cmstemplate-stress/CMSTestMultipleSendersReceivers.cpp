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

#include <iostream>
#include <string>
#include <cstdio>
#include <vector>

#include <decaf/lang/Thread.h>
#include <decaf/lang/Integer.h>
#include <activemq/library/ActiveMQCPP.h>

#include "TestSenderAndReceiver.h"
#include "ConnectionFactoryMgr.h"

using namespace std;
using namespace cms;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace activemq::library;
using namespace cmstemplate;

////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv) {

    printf("Test Started\n");
    int cnt = 25;
    int done = 240;

    if (argc > 1) {
        cnt = Integer::parseInt(argv[1]);
    }

    if (argc > 2) {
        done = Integer::parseInt(argv[2]);
    }

    string url = "tcp://127.0.0.1:61616?connection.sendTimeout=1000";

    ActiveMQCPP::initializeLibrary();
    ConnectionFactoryMgr::initialize();
    int maxThreads = 30;
    int reservedThreads = 3;
    Receiver::initialize(reservedThreads, maxThreads);

    std::vector<TestSenderAndReceiver*> sar;
    sar.resize(cnt);

    for (int i = 0; i < cnt; i++) {
        string topic("topic");
        stringstream str;
        str << i;
        topic += str.str();
        sar[i] = new TestSenderAndReceiver(url, topic, true, false, 50, 1000);
        sar[i]->initialize();
    }

    Thread::sleep(done * 1000);

    for (int i = 0; i < cnt; i++) {
        try {
            sar[i]->close();
        } catch(...) {}
        delete sar[i];
    }

    Receiver::unInitialize();
    ConnectionFactoryMgr::unInitialize();

    ActiveMQCPP::shutdownLibrary();

    printf("\nTest Completed!\n");
}
