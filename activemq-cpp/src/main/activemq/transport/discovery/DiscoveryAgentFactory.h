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

#ifndef _ACTIVEMQ_TRANSPORT_DISCOVERY_DISCOVERYAGENTFACTORY_H_
#define _ACTIVEMQ_TRANSPORT_DISCOVERY_DISCOVERYAGENTFACTORY_H_

#include <activemq/util/Config.h>

#include <decaf/lang/Pointer.h>
#include <decaf/net/URI.h>

#include <activemq/transport/discovery/DiscoveryAgent.h>

namespace activemq {
namespace transport {
namespace discovery {

    /**
     * Factory class for creating discovery agents.  All agents are required to
     * provide a factory class that can be registered in the DiscoveryAgentRegistry.
     *
     * @since 3.9.0
     */
    class AMQCPP_API DiscoveryAgentFactory {
    public:

        virtual ~DiscoveryAgentFactory();

        /**
         * Creates and returns a new DiscoveryAgentFactory instance that can be used to
         * create the agent referred to in the given URI.  The factory should apply all
         * configuration options to the agent prior to returning it.
         *
         * @param agentURI
         *      The URI that defines the agent to create along with it configuration options.
         *
         * @return a new DiscoveryAgent instance for the given URI.
         *
         * @throws IOException if an error occurs creating the given agent.
         */
        virtual decaf::lang::Pointer<DiscoveryAgent> createAgent(const decaf::net::URI& agentURI) = 0;

    };

}}}

#endif /* _ACTIVEMQ_TRANSPORT_DISCOVERY_DISCOVERYAGENTFACTORY_H_ */
