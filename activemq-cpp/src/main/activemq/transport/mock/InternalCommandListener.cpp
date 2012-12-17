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

#include "InternalCommandListener.h"

#include <activemq/transport/mock/MockTransport.h>

using namespace activemq;
using namespace activemq::transport;
using namespace activemq::transport::mock;
using namespace activemq::exceptions;
using namespace activemq::wireformat;
using namespace decaf::io;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
InternalCommandListener::InternalCommandListener() :
    transport(NULL), responseBuilder(), done(false), startedLatch(1), inboundQueue() {

    this->start();
    startedLatch.await();
}

////////////////////////////////////////////////////////////////////////////////
InternalCommandListener::~InternalCommandListener() {
    try {
        done = true;
        synchronized(&inboundQueue) {
            inboundQueue.notifyAll();
        }
        this->join();

        inboundQueue.clear();
    }
    AMQ_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void InternalCommandListener::onCommand(const Pointer<Command> command) {
    synchronized(&inboundQueue) {
        // Create a response now before the caller has a
        // chance to destroy the command.
        responseBuilder->buildIncomingCommands(command, inboundQueue);

        // Wake up the thread, messages are dispatched from there.
        inboundQueue.notifyAll();
    }
}

////////////////////////////////////////////////////////////////////////////////
void InternalCommandListener::run() {

    try {

        synchronized(&inboundQueue) {

            while (!done) {
                startedLatch.countDown();

                while (inboundQueue.isEmpty() && !done) {
                    inboundQueue.wait();
                }

                if (done || transport == NULL) {
                    continue;
                }

                // If we created a response then send it.
                while (!inboundQueue.isEmpty()) {

                    Pointer<Command> command = inboundQueue.pop();

                    if (command->isMessage() && transport->isFailOnReceiveMessage()) {
                        transport->setNumReceivedMessages(transport->getNumReceivedMessages() + 1);

                        if (transport->getNumReceivedMessages() > transport->getNumReceivedMessageBeforeFail()) {
                            transport->fireException(IOException(__FILE__, __LINE__, "Failed to Send Message."));
                            ;
                        }
                    }

                    transport->fireCommand(command);
                }
            }
        }
    }
    AMQ_CATCHALL_NOTHROW()
}
