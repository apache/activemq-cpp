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

#ifndef _ACTIVEMQ_TRANSPORT_DISCOVERY_ABSTRACTDISCOVERYAGENTFACTORY_H_
#define _ACTIVEMQ_TRANSPORT_DISCOVERY_ABSTRACTDISCOVERYAGENTFACTORY_H_

#include <activemq/util/Config.h>

#include <decaf/lang/Pointer.h>
#include <decaf/util/Properties.h>
#include <decaf/net/URI.h>

#include <activemq/transport/discovery/DiscoveryAgentFactory.h>
#include <activemq/transport/discovery/AbstractDiscoveryAgent.h>

namespace activemq {
namespace transport {
namespace discovery {

    class AMQCPP_API AbstractDiscoveryAgentFactory : public DiscoveryAgentFactory {
    public:

        virtual ~AbstractDiscoveryAgentFactory();

        /**
         * Provides the implementation of the createAgent method, subclasses should implement the
         * doCreateAgent method to do the actual agent creation and override the doConfigureAgent if
         * they have additional URI options that need to be applied before returning the newly
         * created agent instance.
         *
         * @param agentURI
         *      The URI that describes the agent and any requested configuration changes.
         *
         * @return a new DiscoveryAgent instance with all URI options applied.
         */
        virtual decaf::lang::Pointer<DiscoveryAgent> createAgent(const decaf::net::URI& agentURI);

    protected:

        /**
         * Subclasses are required to implement this method and return the correct Agent instance.
         *
         * After this method is called by createAgent the configuration method will be invoked to
         * allow the URI options to be applied.
         *
         * @return a Pointer to the newly created discovery agent instance.
         *
         * @throws ActiveMQException if an error occurs while creating the agent.
         */
        virtual decaf::lang::Pointer<AbstractDiscoveryAgent> doCreateAgent() = 0;

    protected:

        /**
         * Called from createAgent after the new agent instance has been created.  The default
         * implementation of this method will configure all the known AbstractDiscoveryAgent
         * options using the URI options given.  Subclasses can override this method to apply
         * addition URI options specific to the agent type being created but should also call
         * this method to ensure that all options are applied.
         *
         * @param agent
         *      The AbstractDiscoveryAgent instance that is to be configured.
         * @param options
         *      The Properties object that contians all agent options parsed from the URI.
         *
         * @throws ActiveMQException if an error occurs while applying the options.
         */
        virtual void doConfigureAgent(decaf::lang::Pointer<AbstractDiscoveryAgent> agent,
                                      const decaf::util::Properties& options);

    };

}}}

#endif /* _ACTIVEMQ_TRANSPORT_DISCOVERY_ABSTRACTDISCOVERYAGENTFACTORY_H_ */
