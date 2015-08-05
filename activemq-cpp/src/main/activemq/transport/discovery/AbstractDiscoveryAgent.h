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

        virtual void setDiscoveryURI(const decaf::net::URI& discoveryURI);
        virtual decaf::net::URI getDiscoveryURI() const;

        /**
         * @return true if this agent is currently started.
         */
        bool isStarted() const;

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
         * @return the configured service to publish.
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
         * @return Time in milliseconds for the keep alive interval.
         */
        long long getKeepAliveInterval() const;

        /**
         * Sets the agents reconnect backoff multiplier.
         *
         * @param multiplier
         *      The back multiplier to use when calculating the next recovery time.
         */
        void setBackOffMultiplier(long long multiplier);

        /**
         * Gets the configured backoff multiplier for calculating the next recovery time.
         *
         * @return the configured backoff multiplier for calculating the next recovery time.
         */
        long long getBackOffMultiplier() const;

        /**
         * Sets the initial amount of time the agent should wait before attempt a reconnect on
         * a discovered service.
         *
         * @param initialReconnectDelay
         *      The time to wait before the initial reconnect attempt in milliseconds.
         */
        void setInitialReconnectDelay(long long initialReconnectDelay);

        /**
         * Gets the initial amount of time the agent should wait before attempt a reconnect on
         * a discovered service.
         *
         * @return The time to wait before the initial reconnect attempt in milliseconds.
         */
        long long getInitialReconnectDelay() const;

        /**
         * Sets the maximum number of reconnect attempts that occur before a service is considered to
         * be failed and removed.
         *
         * @param maxReconnectAttempts
         *      The maximum number of reconnect attempts allowed or zero for no limit.
         */
        void setMaxReconnectAttempts(int maxReconnectAttempts);

        /**
         * Returns the maximum number of reconnect attempts that will be attempted before a
         * service is considered to be failed.
         *
         * @return the maximum number of allowed reconnect attempts.
         */
        int getMaxReconnectAttempts() const;

        /**
         * The maximum time that a reconnect delay is allowed to grow to, in Milliseconds.
         *
         * @param maxReconnectDelay
         *      The maximum time in milliseconds to limit the reconnect delays.
         */
        void setMaxReconnectDelay(long long maxReconnectDelay);

        /**
         * Returns the maximum time allowed between reconnect attempts.  This limits the amount
         * of time that reconnect will wait when exponential backoff is enabled.
         *
         * @return the maximum reconnect delay in milliseconds.
         */
        long long getMaxReconnectDelay() const;

        /**
         * Sets whether reconnect attempts are delayed exponentially until a new connection is made
         * or the maximum number of attempts is exceeded.  Using an exponential back-off can
         * reduce CPU overhead but can also lead to a longer time to reconnect to a service that
         * is known to have short drop outs.
         *
         * @param useExponentialBackOff
         *     When true it indicates that the delay between reconnect attempts grows exponentially.
         */
        void setUseExponentialBackOff(bool useExponentialBackOff);

        /**
         * Returns true if successive attempts to reconnect to a discovered service are delayed
         * by an exponentially growing time factor.
         *
         * @return whether exponential back-off is enabled.
         */
        bool isUseExponentialBackOff() const;

        /**
         * Sets the discovery group that this agent is assigned.  The default value for this
         * group is "default".
         *
         * @param group
         *      The assigned group for the discovery agent.
         */
        void setGroup(const std::string& group);

        /**
         * Returns the assigned group for this discovery agent.
         *
         * @return the assigned group for this discovery agent.
         */
        std::string getGroup() const;

    protected:

        /**
         * Default implementation of the DiscoveryAgent's background worker thread processing.
         *
         * In general there is no need for an agent to override this method, however some agents
         * may require special handling of their discovery mechanisms so it is allowable for this
         * method to be overridden.  Implementors of this method must ensure that the run method
         * will return quickly once the started value becomes false or the Thread is interrupted.
         */
        virtual void run();

        virtual void processLiveService(const std::string& brokerName, const std::string& service);
        virtual void processDeadService(const std::string& service);

        virtual void fireServiceAddedEvent(decaf::lang::Pointer<DiscoveredBrokerData> event);
        virtual void fireServiceRemovedEvent(decaf::lang::Pointer<DiscoveredBrokerData> event);

    protected:

        /**
         * The real agent class must implement this method to perform any necessary resource allocation
         * prior to the completion of the start call.
         */
        virtual void doStart() = 0;

        /**
         * The real agent class must implement this method to perform any necessary resource cleanup
         * prior to the completion of the stop call.
         */
        virtual void doStop() = 0;

        /**
         * Optional method that allows the agent to perform an advertisement of this clients
         * service.  Not all agents can do this so this method should be a no-op if this is
         * not supported.
         */
        virtual void doAdvertizeSelf() = 0;

        /**
         * Performs the actual discovery operation for this agent.  This method is called in the
         * worker thread context of the AbstractDiscoveryAgent and should not block forever.  The
         * method will be called repeatedly to allow the agent to continually probe for new services.
         */
        virtual void doDiscovery() = 0;

    private:

        void doExpireOldServices();
        void doTimeKeepingServices();

    };

}}}

#endif /* _ACTIVEMQ_TRANSPORT_DISCOVERY_ABSTRACTDISCOVERYAGENT_H_ */
