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

#ifndef _ACTIVEMQ_TRANSPORT_DISCOVERY_ABSTRACTDISCOVERYAGENT_H_
#define _ACTIVEMQ_TRANSPORT_DISCOVERY_ABSTRACTDISCOVERYAGENT_H_

#include <activemq/util/Config.h>

#include <activemq/transport/discovery/DiscoveryAgent.h>
#include <activemq/transport/discovery/DiscoveredBrokerData.h>

#include <decaf/lang/Runnable.h>
#include <decaf/lang/Pointer.h>

namespace activemq {
namespace transport {
namespace discovery {

    class AbstractDiscoveryAgentImpl;

    /**
     * Abstract base class that provides all the basic implementation needed to create
     * a DiscoveryAgent instance without needing to implement a lot of boilerplate code.
     *
     * @since 3.9.0
     */
    class AMQCPP_API AbstractDiscoveryAgent : public DiscoveryAgent, public decaf::lang::Runnable {
    private:

        AbstractDiscoveryAgentImpl* impl;

    public:

        static const int DEFAULT_INITIAL_RECONNECT_DELAY;
        static const int DEFAULT_BACKOFF_MULTIPLIER;
        static const int DEFAULT_MAX_RECONNECT_DELAY;
        static const int WORKER_KILL_TIME_SECONDS;
        static const int HEARTBEAT_MISS_BEFORE_DEATH;
        static const int DEFAULT_KEEPALIVE_INTERVAL;

    private:

        AbstractDiscoveryAgent(const AbstractDiscoveryAgent&);
        AbstractDiscoveryAgent& operator= (const AbstractDiscoveryAgent&);

    public:

        AbstractDiscoveryAgent();
        virtual ~AbstractDiscoveryAgent();

        virtual void start();
        virtual void stop();

        virtual void registerService(const std::string& name);
        virtual void serviceFailed(const activemq::commands::DiscoveryEvent& event);

        virtual void setDiscoveryListener(DiscoveryListener* listener);
        virtual DiscoveryListener* getDiscoveryListener() const;

        /**
         * Sets the service that is publish by this agent if it supports publishing.
         *
         * @param name
         *      The service name to publish, typically the URI.
         */
        void setServiceName(const std::string& name);

        /**
         * Gets the configured service to publish, not all agents can publish so this value
         * may not mean that an actual service advertisement is ever done.
         *
         * @returns the configured service to publish.
         */
        std::string getServiceName() const;

        /**
         * Sets the keep alive interval used to control how long an service that has not been
         * seen is kept in the list of discovered services before being idle to long.  Also this
         * value controls how often this service will advertise itself if it supports that.
         *
         * @param interval
         *      Time in milliseconds for the keep alive interval.
         */
        void setKeepAliveInterval(long long interval);

        /**
         * Gets the keep alive interval used to control how long an service that has not been
         * seen is kept in the list of discovered services before being idle to long.  Also this
         * value controls how often this service will advertise itself if it supports that.
         *
         * @returns Time in milliseconds for the keep alive interval.
         */
        long long getKeepAliveInterval() const;

        /**
         * Sets the agents reconnect backoff multiplier.
         *
         * @param multiplier
         *      The back multiplier to use when calculating the next recovery time.
         */
        void getBackOffMultiplier(long long multiplier);

        /**
         * Gets the configured backoff multiplier for calculating the next recovery time.
         *
         * @returns the configured backoff multiplier for calculating the next recovery time.
         */
        long long getBackOffMultiplier() const;

        void setInitialReconnectDelay(long long initialReconnectDelay);

        long long getInitialReconnectDelay() const;

        void setMaxReconnectAttempts(int maxReconnectAttempts);

        int getMaxReconnectAttempts() const;

        void setMaxReconnectDelay(long long maxReconnectDelay);

        long long getMaxReconnectDelay() const;

        void setUseExponentialBackOff(bool useExponentialBackOff);

        bool isUseExponentialBackOff() const;

        void setGroup(const std::string& group);

        std::string getGroup() const;

    protected:

        /**
         * Default implementation of the DiscoveryAgent's background worker thread processing.
         */
        virtual void run();

        virtual void processLiveService(const std::string& brokerName, const std::string& service);
        virtual void processDeadService(const std::string& service);

        virtual void fireServiceAddedEvent(decaf::lang::Pointer<DiscoveredBrokerData> event);
        virtual void fireServiceRemovedEvent(decaf::lang::Pointer<DiscoveredBrokerData> event);

    protected:

        /**
         * The real agent will implement this method to perform any necessary resource allocation
         * prior to the completion of the start call.
         */
        virtual void doStart() = 0;

        virtual void doStop() = 0;
        virtual void doAdvertizeSelf() = 0;
        virtual void doDiscovery() = 0;

    private:

        void doExpireOldServices();
        void doTimeKeepingServices();

    };

}}}

#endif /* _ACTIVEMQ_TRANSPORT_DISCOVERY_ABSTRACTDISCOVERYAGENT_H_ */
