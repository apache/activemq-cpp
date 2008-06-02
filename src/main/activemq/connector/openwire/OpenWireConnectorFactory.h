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

#ifndef _ACTIVEMQ_CONNECTOR_OPENWIRE_OPENWIRECONNECTORFACTORY_H_
#define _ACTIVEMQ_CONNECTOR_OPENWIRE_OPENWIRECONNECTORFACTORY_H_

#include <activemq/util/Config.h>
#include <activemq/connector/ConnectorFactory.h>
#include <activemq/connector/ConnectorFactoryMapRegistrar.h>
#include <activemq/connector/openwire/OpenWireConnector.h>

namespace activemq{
namespace connector{
namespace openwire{

    class AMQCPP_API OpenWireConnectorFactory : public connector::ConnectorFactory {
    public:

        virtual ~OpenWireConnectorFactory() {}

        /**
         * Creates a StompConnector
         * @param properties The Properties that the new connector is
         * configured with
         * @param transport the Transport instance this connector uses.
         */
        virtual OpenWireConnector* createConnector(
            const decaf::util::Properties& properties,
            activemq::transport::Transport* transport );

        /**
         * Returns an instance of this Factory by reference
         * @return StompConnectorFactory reference
         */
        static ConnectorFactory& getInstance();

    };

}}}

#endif /*_ACTIVEMQ_CONNECTOR_OPENWIRE_OPENWIRECONNECTORFACTORY_H_*/
