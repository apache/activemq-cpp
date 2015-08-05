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

#ifndef _ACTIVEMQ_TRANSPORT_DISCOVERY_DISCOVEREDBROKERDATA_H_
#define _ACTIVEMQ_TRANSPORT_DISCOVERY_DISCOVEREDBROKERDATA_H_

#include <activemq/util/Config.h>
#include <activemq/commands/DiscoveryEvent.h>

namespace activemq {
namespace transport {
namespace discovery {

    /**
     * Enhanced DiscoveryEvent object used to store additional data about discovered
     * broker services.
     *
     * @since 3.9.0
     */
    class AMQCPP_API DiscoveredBrokerData : public activemq::commands::DiscoveryEvent {
    private:

        long long lastHeartBeatTime;
        long long nextRecoveryTime;
        int failureCount;
        bool failed;

    public:

        DiscoveredBrokerData();
        DiscoveredBrokerData(const std::string& brokerName, const std::string& service);

        virtual ~DiscoveredBrokerData();

        /**
         * Gets the time of the last heart best from this Broker
         *
         * @return the time of the last received heart beat event from this Broker
         */
        long long getLastHeartBeatTime() const {
            return this->lastHeartBeatTime;
        }

        /**
         * Sets the time of the last received heart beat event from this Broker.
         *
         * @param lastHeartBeatTime
         *      Time since last heart beat was received.
         */
        void setLastHeartBeatTime(long long lastHeartBeatTime) {
            this->lastHeartBeatTime = lastHeartBeatTime;
        }

        /**
         * @return true is this service has been marked as failed.
         */
        bool isFailed() const {
            return failed;
        }

        /**
         * Marks this service as failed.
         *
         * @param failed
         *      Set to true to mark this broker as failed.
         */
        void setFailed(bool failed) {
            this->failed = failed;
        }

        /**
         * Gets the number of times that there was a failure contacting this broker.
         *
         * @return count of the number of failures of this service.
         */
        int getFailureCount() const {
            return failureCount;
        }

        /**
         * Sets the number of failures that are recorded for this service.
         *
         * @param failureCount
         *      The new value of the failure count for this service.
         */
        void setFailureCount(int failureCount) {
            this->failureCount = failureCount;
        }

        /**
         * Gets the set time for the next recovery attempt on this service.
         *
         * @return the next set time that this service can have a recovery attempt.
         */
        long long getNextRecoveryTime() const {
            return nextRecoveryTime;
        }

        /**
         * Sets the next recovery time value for this service.
         *
         * @param recoveryTime
         *      The next time a recovery of this service should be attempted.
         */
        void setNextRecoveryTime(long long nextRecoveryTime) {
            this->nextRecoveryTime = nextRecoveryTime;
        }
    };

}}}

#endif /* _ACTIVEMQ_TRANSPORT_DISCOVERY_DISCOVEREDBROKERDATA_H_ */
