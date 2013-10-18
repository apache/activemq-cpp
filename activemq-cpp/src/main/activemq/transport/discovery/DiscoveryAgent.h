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

#ifndef _ACTIVEMQ_TRANSPORT_DISCOVERY_DISCOVERYAGENT_H_
#define _ACTIVEMQ_TRANSPORT_DISCOVERY_DISCOVERYAGENT_H_

#include <activemq/util/Config.h>
#include <activemq/util/Service.h>
#include <decaf/net/URI.h>
#include <decaf/util/Properties.h>

#include <activemq/commands/DiscoveryEvent.h>

namespace activemq {
namespace transport {
namespace discovery {

    class DiscoveryListener;

    class AMQCPP_API DiscoveryAgent : public activemq::util::Service {
    public:

        virtual ~DiscoveryAgent();

        /**
         * Sets the URI that was used to create this discovery agent.  This URI can
         * be used to configure the discovery agent.
         */
        virtual void setDiscoveryURI(const decaf::net::URI& discoveryURI) = 0;

        /**
         * Sets the URI that was used to create this discovery agent.  This URI can
         * be used to configure the discovery agent.
         */
        virtual decaf::net::URI getDiscoveryURI() const = 0;

        /**
         * Sets the discovery listener which will be notified on the add or remove of
         * a discovered service.
         *
         * @param listener
         *      Pointer to a listener instance that will be notified, or null to reset.
         */
        virtual void setDiscoveryListener(DiscoveryListener* listener) = 0;

        /**
         * Register a service with this DiscoveryAgent.  If the agent supports advertisement
         * then the new service will be broadcast.
         *
         * @param service
         *      The service to register and advertise.
         *
         * @throws IOException if an error occurs.
         */
        virtual void registerService(const std::string& name) = 0;

        /**
         * A process actively using a service may see it go down before the DiscoveryAgent
         * notices the service's failure.  That process can use this method to notify the
         * DiscoveryAgent of the failure so that other listeners of this DiscoveryAgent can
         * also be made aware of the failure.
         *
         * @paran event
         *      A DiscoveryEvent that contains information on the failed service.
         *
         * @throws IOException if an error occurs processing the failure event.
         */
        virtual void serviceFailed(const activemq::commands::DiscoveryEvent& event) = 0;

        /**
         * Returns a descriptive string that represents this discovery agent.
         *
         * @return a string that descibes this discovery agent.
         */
        virtual std::string toString() const = 0;

    };

}}}

#endif /* _ACTIVEMQ_TRANSPORT_DISCOVERY_DISCOVERYAGENT_H_ */
