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

#include "TestSenderAndReceiver.h"

#include <decaf/lang/Thread.h>
#include <decaf/util/Random.h>
#include <bitset>
#include <stdio.h>

using namespace std;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::util;
using namespace cmstemplate;

////////////////////////////////////////////////////////////////////////////////
void TestSenderAndReceiver::onMessage(const string& message) {
    int index = (int) message.find(";");
    if (index <= 0) {
        // i for invalid message
        printf("%c", 105);
    } else {
        // Smiley face for good message
        printf("%c", 2);
    }
}

////////////////////////////////////////////////////////////////////////////////
TestSenderAndReceiver::TestSenderAndReceiver(const string& url, const string& queueOrTopicName, bool isTopic,
                                             bool isDeliveryPersistent, int timeToLive, int receiveTimeout) :
    sender(NULL), receiver(NULL), senderThread(NULL), closing(false), sendIndex(0), receiveIndex(0) {

    sender = new Sender(url, queueOrTopicName, isTopic, isDeliveryPersistent, timeToLive);
    receiver = new Receiver(url, queueOrTopicName, isTopic, receiveTimeout, true);
    ErrorCode errorCode = CMS_SUCCESS;
    receiver->RegisterMessageListener(onMessage, errorCode);
}

////////////////////////////////////////////////////////////////////////////////
TestSenderAndReceiver::~TestSenderAndReceiver() {
    try {
        close();
    } catch(...) {}
}

////////////////////////////////////////////////////////////////////////////////
void TestSenderAndReceiver::init() {
    senderThread = new Thread(this, "TestSenderAndReceiver");
    senderThread->start();
}

////////////////////////////////////////////////////////////////////////////////
void TestSenderAndReceiver::run() {
    ErrorCode errorReturn;
    Random random;
    int i, j;

    j = (int) ((random.nextInt() * 50) / RAND_MAX);

    while (!closing) {
        std::stringstream str;
        str << sendIndex;

        string message;
        str >> message;
        message.append(";");

        // Add variable payload
        j = (int) ((rand() * 1024) / RAND_MAX);
        for (i = 0; i < j; i++) {
            message += string(1, (char) (65 + ((rand() * 26) / RAND_MAX)));
        }

        errorReturn = CMS_SUCCESS;
        sender->SendMessage(message, errorReturn);
        if (errorReturn == CMS_SUCCESS) {
            sendIndex++;
        } else {
            // Exclamation point for error
            printf("%c", 33);
        }

        // Sleep for up to 1 second between sends
        Thread::sleep(((random.nextInt() * 1000) / RAND_MAX));
    }
}

////////////////////////////////////////////////////////////////////////////////
void TestSenderAndReceiver::close() {
    if (!closing) {
        closing = true;

        if (senderThread) {
            senderThread->join();
            delete senderThread;
            senderThread = NULL;
        }

        delete sender;
        sender = NULL;

        try {
            receiver->Close();
        } catch(Exception& ex) {}

        delete receiver;
        receiver = NULL;
    }
}
