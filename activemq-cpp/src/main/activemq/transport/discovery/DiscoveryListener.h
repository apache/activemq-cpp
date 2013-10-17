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

#ifndef _ACTIVEMQ_TRANSPORT_DISCOVERY_DISCOVERYLISTENER_H_
#define _ACTIVEMQ_TRANSPORT_DISCOVERY_DISCOVERYLISTENER_H_

#include <activemq/util/Config.h>

#include <activemq/commands/DiscoveryEvent.h>

namespace activemq {
namespace transport {
namespace discovery {

    class AMQCPP_API DiscoveryListener {
    public:

        virtual ~DiscoveryListener();

        /**
         * Called when an discovery agent becomes aware of a new service.
         *
         * @param event
         *      A DiscoveryEvent that contains information on the newly discovered service.
         */
        virtual void onServiceAdd(const activemq::commands::DiscoveryEvent* event) = 0;

        /**
         * Called when an discovery agent determines that a service is no longer available.
         *
         * @param event
         *      A DiscoveryEvent that contains information on the removed service.
         */
        virtual void onServiceRemove(const activemq::commands::DiscoveryEvent* event) = 0;

    };

}}}

#endif /* _ACTIVEMQ_TRANSPORT_DISCOVERY_DISCOVERYLISTENER_H_ */
