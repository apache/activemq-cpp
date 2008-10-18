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
#include "InitDirector.h"

#include <decaf/util/logging/LogWriter.h>
#include <decaf/lang/Runtime.h>
#include <activemq/transport/IOTransportFactory.h>
#include <activemq/transport/MockTransportFactory.h>
#include <activemq/transport/filters/AsyncSendTransportFactory.h>
#include <activemq/transport/filters/TcpTransportFactory.h>
#include <activemq/transport/filters/LoggingTransportFactory.h>
#include <activemq/transport/filters/ResponseCorrelatorFactory.h>
#include <activemq/connector/stomp/StompConnectorFactory.h>
#include <activemq/connector/openwire/OpenWireConnectorFactory.h>

using namespace activemq;
using namespace activemq::support;
using namespace decaf::util::logging;

int InitDirector::refCount;

////////////////////////////////////////////////////////////////////////////////
InitDirector::InitDirector() {

    if( refCount == 0 ) {
        decaf::lang::Runtime::getRuntime();
        connector::stomp::StompConnectorFactory::getInstance();
        connector::openwire::OpenWireConnectorFactory::getInstance();
        transport::filters::TcpTransportFactory::getInstance();
        transport::filters::AsyncSendTransportFactory::getInstance();
        transport::filters::LoggingTransportFactory::getInstance();
        transport::filters::ResponseCorrelatorFactory::getInstance();
        transport::IOTransportFactory::getInstance();
        transport::MockTransportFactory::getInstance();
    }

    refCount++;
}

////////////////////////////////////////////////////////////////////////////////
InitDirector::~InitDirector() {

    refCount--;

    if( refCount == 0 ) {
    }
}
