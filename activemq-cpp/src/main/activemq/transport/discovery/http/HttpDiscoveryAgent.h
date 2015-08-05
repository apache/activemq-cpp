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

#ifndef _ACTIVEMQ_TRANSPORT_DISCOVERY_HTTP_HTTPDISCOVERYAGENT_H_
#define _ACTIVEMQ_TRANSPORT_DISCOVERY_HTTP_HTTPDISCOVERYAGENT_H_

#include <activemq/util/Config.h>

#include <activemq/util/Suspendable.h>
#include <activemq/transport/discovery/AbstractDiscoveryAgent.h>

namespace activemq {
namespace transport {
namespace discovery {
namespace http {

    class HttpDiscoveryAgentImpl;

    /**
     * HTTP based discovery agent that reads a list of active Brokers from a GET
     * request.
     *
     * @since 3.9.0
     */
    class AMQCPP_API HttpDiscoveryAgent : public AbstractDiscoveryAgent {
    private:

        HttpDiscoveryAgent(const HttpDiscoveryAgent&);
        HttpDiscoveryAgent& operator= (const HttpDiscoveryAgent&);

    private:

        HttpDiscoveryAgentImpl* impl;

    public:

        HttpDiscoveryAgent();

        virtual ~HttpDiscoveryAgent();

        /**
         * Suspend updates from the configured HTTP registry service.
         */
        virtual void suspend();

        /**
         * Resume updates from the configured HTTP registry service.
         */
        virtual void resume();

        /**
         * Sets the amount of time the agent waits before attempting to fetch the list
         * of registered Brokers from the configured HTTP registry service.
         *
         * @param updateInterval
         *      Time in milliseconds to wait between update attempts.
         */
        void setUpdateInterval(long long updateInterval);

        /**
         * Gets the amount of time the agent waits before attempting to fetch the list
         * of registered Brokers from the configured HTTP registry service.
         *
         * @return Time in milliseconds to wait between update attempts.
         */
        long long getUpdateInterval() const;

        /**
         * Sets the URL for the Broker registry where the agent gets its updates.
         *
         * @param registryUrl
         *      The URL to poll for registry entries.
         */
        void setRegistryURL(const std::string& registryUrl);

        /**
         * Gets the URL for the Broker registry where the agent gets its updates.
         *
         * @return The URL to poll for registry entries.
         */
        std::string getRegistryURL() const;

    public:

        virtual void doStart();

        virtual void doStop();

        virtual void doAdvertizeSelf();

        virtual void doDiscovery();

        virtual std::string toString() const;

    protected:

        virtual void updateServices();

    };

}}}}

#endif /* _ACTIVEMQ_TRANSPORT_DISCOVERY_HTTP_HTTPDISCOVERYAGENT_H_ */
