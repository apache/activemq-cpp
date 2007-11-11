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

#include <activemq/connector/openwire/OpenWireConnectorFactory.h>
#include <activemq/connector/openwire/OpenWireConnector.h>
#include <activemq/connector/Connector.h>
#include <activemq/transport/Transport.h>

using namespace activemq;
using namespace activemq::util;
using namespace activemq::transport;
using namespace activemq::connector;
using namespace activemq::connector::openwire;

////////////////////////////////////////////////////////////////////////////////
OpenWireConnector* OpenWireConnectorFactory::createConnector(
    const decaf::util::Properties& properties,
    activemq::transport::Transport* transport ) {

    return new OpenWireConnector( transport, properties );
}

////////////////////////////////////////////////////////////////////////////////
ConnectorFactory& OpenWireConnectorFactory::getInstance() {

    // Create a static instance of the registrar and return a reference to
    // its internal instance of this class.
    static ConnectorFactoryMapRegistrar registrar(
        "openwire", new OpenWireConnectorFactory() );

    return registrar.getFactory();
}
