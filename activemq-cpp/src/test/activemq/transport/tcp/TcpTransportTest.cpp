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

#include "TcpTransportTest.h"

#include <activemq/transport/tcp/TcpTransportFactory.h>
#include <activemq/transport/tcp/TcpTransport.h>

#include <activemq/wireformat/openwire/OpenWireFormat.h>

#include <decaf/lang/Pointer.h>
#include <decaf/net/Socket.h>
#include <decaf/net/SocketFactory.h>
#include <decaf/net/ServerSocket.h>
#include <decaf/io/InputStream.h>
#include <decaf/io/OutputStream.h>
#include <decaf/util/Random.h>

using namespace decaf;
using namespace decaf::lang;
using namespace decaf::net;
using namespace decaf::io;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace activemq;
using namespace activemq::wireformat;
using namespace activemq::wireformat::openwire;
using namespace activemq::transport;
using namespace activemq::transport::tcp;

////////////////////////////////////////////////////////////////////////////////
TcpTransportTest::TcpTransportTest() {
}

////////////////////////////////////////////////////////////////////////////////
TcpTransportTest::~TcpTransportTest() {
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class TestServer : public lang::Thread{
    private:

        bool done;
        bool error;
        Pointer<ServerSocket> server;
        Pointer<OpenWireFormat> wireFormat;
        CountDownLatch started;

    public:

        TestServer() : Thread(), done(false), error(false), server(), started(1) {
            server.reset(new ServerSocket(0));

            Properties properties;
            this->wireFormat.reset(new OpenWireFormat(properties));
        }

        virtual ~TestServer() {
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

                    std::auto_ptr<Socket> socket(server->accept());
                    socket->setSoLinger(false, 0);

                    InputStream* is = socket->getInputStream();
                    DataInputStream ds(is);

                    socket->close();
                }

            } catch (IOException& ex) {
                error = true;
            } catch (...) {
                error = true;
            }
        }
    };

    TestServer* server;
}

////////////////////////////////////////////////////////////////////////////////
void TcpTransportTest::setUp() {

    server = new TestServer();
    server->start();
    server->waitUntilStarted();
}

////////////////////////////////////////////////////////////////////////////////
void TcpTransportTest::tearDown() {

    if (server == NULL) {
        return;
    }

    server->stop();
    server->waitUntilStopped();
}

////////////////////////////////////////////////////////////////////////////////
void TcpTransportTest::testTransportCreateWithRadomFailures() {

    Properties properties;
    OpenWireFormat wireFormat(properties);
    TcpTransportFactory factory;
}
