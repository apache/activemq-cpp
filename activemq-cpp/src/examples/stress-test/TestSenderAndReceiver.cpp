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

#include <stdio.h>

#ifdef __SUNPRO_CC
#include <stdlib.h>
#endif

using namespace decaf::lang;
using namespace cms;
using namespace cms::stress;
using namespace decaf::util;
using namespace decaf::util::concurrent;

extern bool VERBOSE;
extern TESTINFO TestResults;

////////////////////////////////////////////////////////////////////////////////
TestSenderAndReceiver::TestSenderAndReceiver(const std::string& url, const std::string& queueOrTopicName,
                                             const std::string& headerName, bool isTopic,
                                             bool isDeliveryPersistent, BrokerMonitor* monitor,
                                             CountDownLatch* quit, int timeToLive, int receiveTimeout,
                                             int identifier, bool useThreadPool, int sleep, int seed) :
    sender(NULL),
    receiver(NULL),
    senderThread(NULL),
    monitor(monitor),
    header(headerName),
    closing(false),
    sendIndex(0),
    id(identifier),
    sleep(sleep),
    seed(seed),
    quit(quit),
    random(seed) {

    sender = new Sender(url, queueOrTopicName, isTopic, isDeliveryPersistent, timeToLive);
    receiver = new Receiver(url, queueOrTopicName, isTopic, monitor, quit, receiveTimeout, useThreadPool);
    ErrorCode errorCode = CMS_SUCCESS;

    std::string selector("");
    if (headerName != "") {
        std::stringstream sID;
        selector = headerName;
        selector.append("='");
        sID << identifier;
        selector.append(sID.str());
        selector.append("'");
    }

    receiver->registerMessageListener(this, errorCode, selector, identifier);
}

////////////////////////////////////////////////////////////////////////////////
TestSenderAndReceiver::~TestSenderAndReceiver() {
    close();
}

////////////////////////////////////////////////////////////////////////////////
void TestSenderAndReceiver::init() {
    char buffer[512];
    sprintf(buffer, "TestSender-%d", id);
    senderThread = new Thread(this, buffer);
    senderThread->start();
}

////////////////////////////////////////////////////////////////////////////////
void TestSenderAndReceiver::onMessage(const std::string& message) {

    int index = (int) message.find(";");
    std::string msg;
    int thrdidx;
    int thrdseq, curseq, diffseq;

    if (index <= 0) {
        // i for invalid message
        if (VERBOSE) {
            printf("%c", SYM_BAD_MSG);
        }
        TestResults.invalidMessages.incrementAndGet();
    } else {
        thrdidx = atoi(message.substr(0, index).c_str());
        msg = message.substr(index + 1);

        if (thrdidx > (int) TestResults.threadCount) {
            if (VERBOSE) {
                printf("%c", SYM_BAD_MSG);
            }
            TestResults.invalidMessages.incrementAndGet();
        } else {
            index = (int) msg.find(";");
            if (index <= 0) {
                if (VERBOSE) {
                    printf("%c", SYM_BAD_MSG);
                }
                TestResults.invalidMessages.incrementAndGet();
            } else {
                TestResults.received.incrementAndGet();
                thrdseq = Integer::parseInt(msg.substr(0, index));
                msg = msg.substr(index + 1);
                curseq = TestResults.lastSequence[thrdidx].incrementAndGet();
                if (thrdseq == curseq) {
                    if (VERBOSE) {
                        // Smiley face for good message
                        printf("%c", SYM_GOOD_SEQ);
                    }
                } else {
                    TestResults.lastSequence[thrdidx].set(thrdseq);
                    TestResults.badSequenceMessages.incrementAndGet();
                    if (thrdseq > curseq) {
                        diffseq = thrdseq - curseq;
                    } else {
                        diffseq = curseq - thrdseq;
                    }
                    TestResults.sequenceDifferences.addAndGet(diffseq);
                    if (VERBOSE) {
                        if ((diffseq > 0) && (diffseq < 10)) {
                            printf("%d", diffseq);
                        } else {
                            printf("%c", SYM_BIG_DIFF);
                        }
                    }
                }
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void TestSenderAndReceiver::run() {
    ErrorCode errorReturn;
    int i, j;
    bool result;

    // Seed the random numbers - time if zero
    if (seed == 0) {
        random.setSeed(System::currentTimeMillis());
    } else {
        random.setSeed(seed);
    }

    // If randomizing sleeps - stagger start by up to 1 second
    if (sleep == -1) {
        Thread::sleep(random.nextInt(1000));
    }

    while (!closing) {
        std::stringstream sID;
        std::stringstream sSeq;
        std::stringstream sHdr;
        std::string message;

        // Add id to messages
        sID << id;
        sID >> message;
        sHdr << id;
        message.append(";");

        // Add sequence to messages
        sSeq << sendIndex;
        message.append(sSeq.str());
        message.append(";");

        // Add variable payload
        j = random.nextInt(1024);
        for (i = 0; i < j; i++) {
            message += std::string(1, (char) (65 + (random.nextInt(24))));
        }

        errorReturn = CMS_SUCCESS;
        sender->SendMessage(message, errorReturn, header, sHdr.str());
        if (errorReturn == CMS_SUCCESS) {
            sendIndex++;
            if (VERBOSE) {
                printf("%c", SYM_GOOD_SEND);
            }
            TestResults.sent.incrementAndGet();
        } else {
            if (VERBOSE) {
                // Exclamation point for error
                printf("%c", SYM_BAD_SEND);
            }
            TestResults.sendErrors.incrementAndGet();
        }

        if (sleep) {
            if (sleep == -1) {
                result = quit->await(random.nextInt(1000));
            } else {
                result = quit->await(random.nextInt(sleep));
            }
        } else {
            result = quit->getCount() == 0;
        }

        if (result) {
            closing = true;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void TestSenderAndReceiver::close() {
    closing = true;

    if (senderThread) {
        senderThread->join();
        delete senderThread;
        senderThread = NULL;
    }

    if (sender) {
        delete sender;
        sender = NULL;
    }

    if (receiver) {
        receiver->close();
        delete receiver;
        receiver = NULL;
    }
}
