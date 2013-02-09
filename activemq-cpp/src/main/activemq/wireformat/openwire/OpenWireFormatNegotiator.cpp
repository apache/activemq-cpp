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

#include "OpenWireFormatNegotiator.h"

#include <activemq/commands/DataStructure.h>
#include <activemq/commands/WireFormatInfo.h>
#include <activemq/transport/IOTransport.h>

using namespace std;
using namespace activemq;
using namespace activemq::commands;
using namespace activemq::exceptions;
using namespace activemq::wireformat;
using namespace activemq::wireformat::openwire;
using namespace activemq::transport;
using namespace activemq::commands;
using namespace decaf::util::concurrent;
using namespace decaf::io;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
const int OpenWireFormatNegotiator::negotiationTimeout = 15000;

////////////////////////////////////////////////////////////////////////////////
OpenWireFormatNegotiator::OpenWireFormatNegotiator(OpenWireFormat* wireFormat, const Pointer<Transport> next ) :
    WireFormatNegotiator( next ),
    firstTime(true),
    wireInfoSentDownLatch(1),
    readyCountDownLatch(1),
    openWireFormat(wireFormat) {
}

////////////////////////////////////////////////////////////////////////////////
OpenWireFormatNegotiator::~OpenWireFormatNegotiator() {
    try {
        close();
    }
    AMQ_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireFormatNegotiator::oneway(const Pointer<Command> command) {

    try {

        checkClosed();

        if (!readyCountDownLatch.await(negotiationTimeout)) {
            throw IOException(__FILE__, __LINE__, "OpenWireFormatNegotiator::oneway"
                    "Wire format negotiation timeout: peer did not "
                    "send his wire format.");
        }

        next->oneway(command);
    }
    AMQ_CATCH_RETHROW(UnsupportedOperationException)
    AMQ_CATCH_RETHROW(IOException)
    AMQ_CATCH_EXCEPTION_CONVERT(exceptions::ActiveMQException, IOException)
    AMQ_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Response> OpenWireFormatNegotiator::request(const Pointer<Command> command) {

    try {

        checkClosed();

        if (!readyCountDownLatch.await(negotiationTimeout)) {
            throw IOException(__FILE__, __LINE__, "OpenWireFormatNegotiator::request"
                    "Wire format negotiation timeout: peer did not "
                    "send his wire format.");
        }

        return next->request(command);
    }
    AMQ_CATCH_RETHROW(UnsupportedOperationException)
    AMQ_CATCH_RETHROW(IOException)
    AMQ_CATCH_EXCEPTION_CONVERT(exceptions::ActiveMQException, IOException)
    AMQ_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Response> OpenWireFormatNegotiator::request(const Pointer<Command> command, unsigned int timeout) {

    try {

        checkClosed();

        if (!readyCountDownLatch.await(negotiationTimeout)) {
            throw IOException(__FILE__, __LINE__, "OpenWireFormatNegotiator::request"
                    "Wire format negotiation timeout: peer did not "
                    "send his wire format.");
        }

        return next->request(command, timeout);
    }
    AMQ_CATCH_RETHROW(UnsupportedOperationException)
    AMQ_CATCH_RETHROW(IOException)
    AMQ_CATCH_EXCEPTION_CONVERT(exceptions::ActiveMQException, IOException)
    AMQ_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireFormatNegotiator::onCommand(const Pointer<Command> command) {

    if (command->isWireFormatInfo()) {

        WireFormatInfo* info = dynamic_cast<WireFormatInfo*>(command.get());

        try {

            if (!info->isValid()) {
                throw IOException(__FILE__, __LINE__, "OpenWireFormatNegotiator::onCommand"
                        "Remote wire format magic is invalid");
            }

            wireInfoSentDownLatch.await(negotiationTimeout);
            openWireFormat->renegotiateWireFormat(*info);
            readyCountDownLatch.countDown();
        } catch (exceptions::ActiveMQException& ex) {
            readyCountDownLatch.countDown();
            TransportFilter::onCommand(command);
        }
    }

    // Send along to the next interested party.
    TransportFilter::onCommand(command);
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireFormatNegotiator::onException(const decaf::lang::Exception& ex) {
    readyCountDownLatch.countDown();
    TransportFilter::onException(ex);
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireFormatNegotiator::afterNextIsStopped() {
    readyCountDownLatch.countDown();
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireFormatNegotiator::afterNextIsStarted() {

    if (firstTime.compareAndSet(true, false)) {
        try {

            // We first send the WireFormat that we'd prefer.
            this->next->oneway(openWireFormat->getPreferedWireFormatInfo());

            // Mark the latch
            wireInfoSentDownLatch.countDown();

        } catch (decaf::lang::Exception& ex) {
            // Mark the latch
            wireInfoSentDownLatch.countDown();
            ex.setMark(__FILE__, __LINE__);
            throw;
        }
    }
}
