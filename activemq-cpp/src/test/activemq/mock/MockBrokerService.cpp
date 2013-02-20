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

#include "MockBrokerService.h"

#include <activemq/wireformat/openwire/OpenWireFormatFactory.h>
#include <activemq/wireformat/openwire/OpenWireFormat.h>
#include <activemq/wireformat/openwire/OpenWireResponseBuilder.h>
#include <activemq/commands/Command.h>
#include <activemq/commands/Response.h>
#include <activemq/commands/WireFormatInfo.h>
#include <activemq/transport/mock/MockTransport.h>

#include <decaf/net/ServerSocket.h>
#include <decaf/net/Socket.h>
#include <decaf/lang/Integer.h>
#include <decaf/lang/Pointer.h>
#include <decaf/util/Random.h>
#include <decaf/util/Properties.h>
#include <decaf/util/concurrent/CountDownLatch.h>
#include <decaf/io/InputStream.h>
#include <decaf/io/OutputStream.h>

using namespace activemq;
using namespace activemq::mock;
using namespace activemq::commands;
using namespace activemq::wireformat;
using namespace activemq::wireformat::openwire;
using namespace activemq::transport::mock;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::io;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace decaf::net;

////////////////////////////////////////////////////////////////////////////////
namespace activemq {
namespace mock {

    class TcpServer : public lang::Thread {
    private:

        bool done;
        bool error;
        Pointer<ServerSocket> server;
        Pointer<OpenWireFormat> wireFormat;
        Pointer<OpenWireResponseBuilder> responeBuilder;
        CountDownLatch started;
        Random rand;

    public:

        TcpServer() : Thread(), done(false), error(false), server(), wireFormat(),
                      responeBuilder(), started(1), rand() {
            server.reset(new ServerSocket(0));

            Properties properties;

            this->wireFormat = OpenWireFormatFactory().createWireFormat(properties).dynamicCast<OpenWireFormat>();
            this->responeBuilder.reset(new OpenWireResponseBuilder());

            this->rand.setSeed(System::currentTimeMillis());
        }

        TcpServer(int port) : Thread(), done(false), error(false), server(), wireFormat(),
                              responeBuilder(), started(1), rand() {
            server.reset(new ServerSocket(port));

            Properties properties;
            this->wireFormat = OpenWireFormatFactory().createWireFormat(properties).dynamicCast<OpenWireFormat>();
            this->responeBuilder.reset(new OpenWireResponseBuilder());

            this->rand.setSeed(System::currentTimeMillis());
        }

        virtual ~TcpServer() {
            stop();
        }

        int getLocalPort() {
            if (this->server.get() != NULL) {
                return server->getLocalPort();
            }

            return 0;
        }

        void waitUntilStarted() {
            this->started.await();
        }

        void waitUntilStopped() {
            this->join();
        }

        void stop() {
            try {
                done = true;
                server->close();
            } catch (...) {}
        }

        virtual void run() {
            try {

                started.countDown();

                while (!done) {

                    MockTransport mock(this->wireFormat, this->responeBuilder);

                    std::auto_ptr<Socket> socket(server->accept());
                    socket->setSoLinger(false, 0);

                    Pointer<WireFormatInfo> preferred = wireFormat->getPreferedWireFormatInfo();

                    OutputStream* os = socket->getOutputStream();
                    DataOutputStream dataOut(os);

                    InputStream* is = socket->getInputStream();
                    DataInputStream dataIn(is);

                    wireFormat->marshal(preferred, &mock, &dataOut);
                    dataOut.flush();

                    while (!done) {
                        Pointer<Command> command = wireFormat->unmarshal(&mock, &dataIn);
                        Pointer<Response> response = responeBuilder->buildResponse(command);

                        if (response != NULL) {
                            wireFormat->marshal(response, &mock, &dataOut);
                        }
                    }
                }

            } catch (IOException& ex) {
                error = true;
            } catch (Exception& ex) {
                error = true;
            } catch (...) {
                error = true;
            }
        }
    };

    class MockBrokerServiceImpl {
    private:

        MockBrokerServiceImpl(const MockBrokerServiceImpl&);
        MockBrokerServiceImpl& operator= (const MockBrokerServiceImpl&);

    public:

        Pointer<TcpServer> server;
        CountDownLatch started;
        CountDownLatch stopped;

    public:

        MockBrokerServiceImpl() : server(), started(1), stopped(1) {
            this->server.reset(new TcpServer());
        }

        MockBrokerServiceImpl(int port) : server(), started(1), stopped(1) {
            this->server.reset(new TcpServer(port));
        }

    };

}}

////////////////////////////////////////////////////////////////////////////////
MockBrokerService::MockBrokerService() : impl(new MockBrokerServiceImpl()) {

}

////////////////////////////////////////////////////////////////////////////////
MockBrokerService::MockBrokerService(int port) : impl(new MockBrokerServiceImpl(port)) {

}

////////////////////////////////////////////////////////////////////////////////
MockBrokerService::~MockBrokerService() {
    try {
        stop();
    }
    AMQ_CATCHALL_NOTHROW()

    try {
        delete impl;
    }
    AMQ_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void MockBrokerService::start() {
    this->impl->server->start();
}

////////////////////////////////////////////////////////////////////////////////
void MockBrokerService::stop() {
    this->impl->server->stop();
}

////////////////////////////////////////////////////////////////////////////////
void MockBrokerService::waitUntilStarted() {
    this->impl->server->waitUntilStarted();
}

////////////////////////////////////////////////////////////////////////////////
void MockBrokerService::waitUntilStopped() {
    this->impl->server->waitUntilStopped();
}

////////////////////////////////////////////////////////////////////////////////
std::string MockBrokerService::getConnectString() const {
    int port = this->impl->server->getLocalPort();
    return std::string("tcp://localhost:") + Integer::toString(port);
}
