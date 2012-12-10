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

#ifndef _CMSTEMPLATE_TESTSENDERANDRECEIVER_H_
#define _CMSTEMPLATE_TESTSENDERANDRECEIVER_H_

#include <decaf/lang/Runnable.h>
#include <decaf/util/concurrent/CountDownLatch.h>
#include <decaf/lang/exceptions/RuntimeException.h>

#include "Sender.h"
#include "Receiver.h"
#include "CmsMessageHandlerDefinitions.h"

namespace cmstemplate {

    class TestSenderAndReceiver : public decaf::lang::Runnable,
                                  public ReceiverListener {
    private:

        std::auto_ptr<Sender> sender;
        std::auto_ptr<Receiver> receiver;
        std::auto_ptr<decaf::lang::Thread> senderThread;
        bool closing;
        int sendIndex;
        int receiveIndex;

    private:

        TestSenderAndReceiver(const TestSenderAndReceiver&);
        TestSenderAndReceiver& operator= (const TestSenderAndReceiver&);

    public:

        TestSenderAndReceiver(const std::string& url, const std::string& queueOrTopicName,
                              bool isTopic, bool isDeliveryPersistent, int timeToLive, int receiveTimeout);

        virtual ~TestSenderAndReceiver();

        void initialize();

        virtual void run();

        void close();

        void waitUntilReady();

    public:

        virtual void onMessage(const std::string& message);

    };
}

#endif /** _CMSTEMPLATE_TESTSENDERANDRECEIVER_H_ */
