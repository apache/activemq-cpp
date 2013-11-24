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

#include "ActiveMQCPP.h"

#include <decaf/lang/Runtime.h>
#include <activemq/wireformat/WireFormatRegistry.h>
#include <activemq/transport/TransportRegistry.h>

#include <activemq/util/IdGenerator.h>

#include <activemq/wireformat/stomp/StompWireFormatFactory.h>
#include <activemq/wireformat/openwire/OpenWireFormatFactory.h>

#include <activemq/transport/mock/MockTransportFactory.h>
#include <activemq/transport/tcp/TcpTransportFactory.h>
#include <activemq/transport/tcp/SslTransportFactory.h>
#include <activemq/transport/failover/FailoverTransportFactory.h>

using namespace activemq;
using namespace activemq::library;
using namespace activemq::util;
using namespace activemq::transport;
using namespace activemq::transport::tcp;
using namespace activemq::transport::mock;
using namespace activemq::transport::failover;
using namespace activemq::wireformat;

////////////////////////////////////////////////////////////////////////////////
ActiveMQCPP::ActiveMQCPP() {

}

////////////////////////////////////////////////////////////////////////////////
ActiveMQCPP::~ActiveMQCPP() {

}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQCPP::initializeLibrary(int argc, char** argv) {

    // Initialize the Decaf Library by requesting its runtime.
    decaf::lang::Runtime::initializeRuntime(argc, argv);

    // Register all WireFormats
    ActiveMQCPP::registerWireFormats();

    // Register all Transports
    ActiveMQCPP::registerTransports();

    // Start the IdGenerator Kernel
    IdGenerator::initialize();
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQCPP::initializeLibrary() {
    ActiveMQCPP::initializeLibrary(0, NULL);
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQCPP::shutdownLibrary() {

    // Shutdown the IdGenerator Kernel
    IdGenerator::shutdown();

    WireFormatRegistry::shutdown();
    TransportRegistry::shutdown();

    // Now it should be safe to shutdown Decaf.
    decaf::lang::Runtime::shutdownRuntime();
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQCPP::registerWireFormats() {

    // Each of the internally implemented WireFormat's is registered here
    // with the WireFormat Registry
    WireFormatRegistry::initialize();

    WireFormatRegistry::getInstance().registerFactory("openwire", new wireformat::openwire::OpenWireFormatFactory());
    WireFormatRegistry::getInstance().registerFactory("stomp", new wireformat::stomp::StompWireFormatFactory());
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQCPP::registerTransports() {

    // Each of the internally implemented Transports is registered here
    // with the Transport Registry
    TransportRegistry::initialize();

    TransportRegistry::getInstance().registerFactory("tcp", new TcpTransportFactory());
    TransportRegistry::getInstance().registerFactory("ssl", new SslTransportFactory());
    TransportRegistry::getInstance().registerFactory("nio", new TcpTransportFactory());
    TransportRegistry::getInstance().registerFactory("nio+ssl", new SslTransportFactory());
    TransportRegistry::getInstance().registerFactory("mock", new MockTransportFactory());
    TransportRegistry::getInstance().registerFactory("failover", new FailoverTransportFactory());
}
