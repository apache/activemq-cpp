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

#ifndef _CMS_STRESS_TESTSENDERANDRECEIVER_H_
#define _CMS_STRESS_TESTSENDERANDRECEIVER_H_

#include <decaf/util/Config.h>

#include <decaf/lang/Runnable.h>
#include <decaf/util/concurrent/atomic/AtomicInteger.h>
#include <decaf/util/concurrent/CountDownLatch.h>
#include <decaf/lang/exceptions/RuntimeException.h>
#include <decaf/util/Random.h>

#include "Sender.h"
#include "Receiver.h"
#include "CmsMessageHandlerDefinitions.h"

namespace cms {
namespace stress {

    class TestSenderAndReceiver: public decaf::lang::Runnable,
                                 public ReceiverListener {
    private:

        Sender* sender;
        Receiver* receiver;
        decaf::lang::Thread* senderThread;
        BrokerMonitor* monitor;
        std::string header;
        bool closing;
        int sendIndex;
        int id;
        int sleep;
        unsigned int seed;
        decaf::util::concurrent::CountDownLatch* quit;
        decaf::util::Random random;

    public:

        TestSenderAndReceiver(const std::string& url, const std::string& queueOrTopicName,
                              const std::string& headerName, bool isTopic, bool isDeliveryPersistent,
                              BrokerMonitor *monitor, decaf::util::concurrent::CountDownLatch* quit,
                              int timeToLive, int receiveTimeout, int identifier,
                              bool useThreadPool = true, int sleep = -1, int seed = 0);

        virtual ~TestSenderAndReceiver();

        void init();

        virtual void run();

        void close();

        void waitUntilReady();

    public:

        virtual void onMessage(const std::string& message);

    };

    typedef struct {
        int threadCount;
        long long startTime;
        long long endTime;
        decaf::util::concurrent::atomic::AtomicInteger sent;
        decaf::util::concurrent::atomic::AtomicInteger received;
        decaf::util::concurrent::atomic::AtomicInteger sendErrors;
        decaf::util::concurrent::atomic::AtomicInteger receiveErrors;
        decaf::util::concurrent::atomic::AtomicInteger invalidMessages;
        decaf::util::concurrent::atomic::AtomicInteger badSequenceMessages;
        decaf::util::concurrent::atomic::AtomicInteger sequenceDifferences;
        decaf::util::concurrent::atomic::AtomicInteger* lastSequence;
    } TESTINFO;

}}

#endif /** _CMS_STRESS_TESTSENDERANDRECEIVER_H_ */
