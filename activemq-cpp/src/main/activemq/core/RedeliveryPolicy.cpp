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

#include "RedeliveryPolicy.h"

#include <decaf/lang/Boolean.h>
#include <decaf/lang/Double.h>
#include <decaf/lang/Short.h>
#include <decaf/lang/Integer.h>
#include <decaf/lang/Long.h>

using namespace activemq;
using namespace activemq::core;
using namespace decaf;
using namespace decaf::lang;

////////////////////////////////////////////////////////////////////////////////
const long long RedeliveryPolicy::NO_MAXIMUM_REDELIVERIES = -1;

////////////////////////////////////////////////////////////////////////////////
RedeliveryPolicy::RedeliveryPolicy() {
}

////////////////////////////////////////////////////////////////////////////////
RedeliveryPolicy::~RedeliveryPolicy() {
}

////////////////////////////////////////////////////////////////////////////////
void RedeliveryPolicy::configure(const decaf::util::Properties& properties) {

    try {

        if (properties.hasProperty("cms.redeliveryPolicy.backOffMultiplier")) {
            this->setBackOffMultiplier(Double::parseDouble(
                properties.getProperty("cms.redeliveryPolicy.backOffMultiplier")));
        }
        if (properties.hasProperty("cms.redeliveryPolicy.collisionAvoidancePercent")) {
            this->setCollisionAvoidancePercent(Short::parseShort(
                properties.getProperty("cms.redeliveryPolicy.collisionAvoidancePercent")));
        }
        if (properties.hasProperty("cms.redeliveryPolicy.initialRedeliveryDelay")) {
            this->setInitialRedeliveryDelay(Long::parseLong(
                properties.getProperty("cms.redeliveryPolicy.initialRedeliveryDelay")));
        }
        if (properties.hasProperty("cms.redeliveryPolicy.redeliveryDelay")) {
            this->setRedeliveryDelay(Long::parseLong(
                properties.getProperty("cms.redeliveryPolicy.redeliveryDelay")));
        }
        if (properties.hasProperty("cms.redeliveryPolicy.maximumRedeliveries")) {
            this->setMaximumRedeliveries(Integer::parseInt(
                properties.getProperty("cms.redeliveryPolicy.maximumRedeliveries")));
        }
        if (properties.hasProperty("cms.redeliveryPolicy.useCollisionAvoidance")) {
            this->setUseCollisionAvoidance(Boolean::parseBoolean(
                properties.getProperty("cms.redeliveryPolicy.useCollisionAvoidance")));
        }
        if (properties.hasProperty("cms.redeliveryPolicy.useExponentialBackOff")) {
            this->setUseExponentialBackOff(Boolean::parseBoolean(
                properties.getProperty("cms.redeliveryPolicy.useExponentialBackOff")));
        }
    }
    DECAF_CATCH_RETHROW(Exception)
    DECAF_CATCHALL_THROW(Exception)
}
