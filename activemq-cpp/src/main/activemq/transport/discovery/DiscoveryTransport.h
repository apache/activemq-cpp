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

#ifndef _ACTIVEMQ_TRANSPORT_DISCOVERY_DISCOVERYTRANSPORT_H_
#define _ACTIVEMQ_TRANSPORT_DISCOVERY_DISCOVERYTRANSPORT_H_

#include <activemq/util/Config.h>
#include <activemq/transport/CompositeTransport.h>
#include <activemq/transport/TransportFilter.h>
#include <activemq/transport/discovery/DiscoveryListener.h>
#include <activemq/transport/discovery/DiscoveryAgent.h>
#include <decaf/net/URI.h>
#include <decaf/util/Properties.h>

namespace activemq {
namespace transport {
namespace discovery {

    class DiscoveryTransportData;

    class AMQCPP_API DiscoveryTransport : public TransportFilter, public DiscoveryListener {
    public:

        static const std::string DISCOVERED_OPTION_PREFIX;

    private:

        DiscoveryTransport(const DiscoveryTransport&);
        DiscoveryTransport& operator=(const DiscoveryTransport&);

    private:

        DiscoveryTransportData* impl;

    public:

        DiscoveryTransport(Pointer<CompositeTransport> next);

        virtual ~DiscoveryTransport();

        virtual void start();

        virtual void stop();

        /**
         * Sets the Discovery Agent that this transport will use to discover new Brokers.
         *
         * @param agent
         *      The Discovery Agent to use in this transport.
         */
        void setDiscoveryAgent(decaf::lang::Pointer<DiscoveryAgent> agent);

        /**
         * Returns the currently configured Discovery Agent
         *
         * @return the pointer to the currently configured agent or NULL if not set.
         */
        Pointer<DiscoveryAgent> getDiscoveryAgent() const;

        /**
         * Sets the properties that are used for configuration of discovered brokers.
         *
         * @param properties
         *      The supplied properties to use to configure new services.
         */
        void setParameters(const decaf::util::Properties& properties);

        /**
         * Gets the currently set parameters that are applied to newly discovered services URIs.
         *
         * @return the currently set Properties to apply to new service URIs.
         */
        decaf::util::Properties getParameters() const;

    public:

        virtual void onServiceAdd(const activemq::commands::DiscoveryEvent* event);

        virtual void onServiceRemove(const activemq::commands::DiscoveryEvent* event);

        virtual void transportInterrupted();

        virtual void transportResumed();

    protected:

        virtual void doClose();

    };

}}}

#endif /* _ACTIVEMQ_TRANSPORT_DISCOVERY_DISCOVERYTRANSPORT_H_ */
