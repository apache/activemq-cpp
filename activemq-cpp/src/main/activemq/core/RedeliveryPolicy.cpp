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
void RedeliveryPolicy::configure( const decaf::util::Properties& properties ) {

    try{

        if( properties.hasProperty( "cms.RedeliveryPolicy.backOffMultiplier" ) ) {
            this->setBackOffMultiplier( Double::parseDouble(
                properties.getProperty( "cms.RedeliveryPolicy.backOffMultiplier" ) ) );
        }
        if( properties.hasProperty( "cms.RedeliveryPolicy.collisionAvoidancePercent" ) ) {
            this->setCollisionAvoidancePercent( Short::parseShort(
                properties.getProperty( "cms.RedeliveryPolicy.collisionAvoidancePercent" ) ) );
        }
        if( properties.hasProperty( "cms.RedeliveryPolicy.initialRedeliveryDelay" ) ) {
            this->setInitialRedeliveryDelay( Long::parseLong(
                properties.getProperty( "cms.RedeliveryPolicy.initialRedeliveryDelay" ) ) );
        }
        if( properties.hasProperty( "cms.RedeliveryPolicy.redeliveryDelay" ) ) {
            this->setRedeliveryDelay( Long::parseLong(
                properties.getProperty( "cms.RedeliveryPolicy.redeliveryDelay" ) ) );
        }
        if( properties.hasProperty( "cms.RedeliveryPolicy.maximumRedeliveries" ) ) {
            this->setMaximumRedeliveries( Integer::parseInt(
                properties.getProperty( "cms.RedeliveryPolicy.maximumRedeliveries" ) ) );
        }
        if( properties.hasProperty( "cms.RedeliveryPolicy.useCollisionAvoidance" ) ) {
            this->setUseCollisionAvoidance( Boolean::parseBoolean(
                properties.getProperty( "cms.RedeliveryPolicy.useCollisionAvoidance" ) ) );
        }
        if( properties.hasProperty( "cms.RedeliveryPolicy.useExponentialBackOff" ) ) {
            this->setUseExponentialBackOff( Boolean::parseBoolean(
                properties.getProperty( "cms.RedeliveryPolicy.useExponentialBackOff" ) ) );
        }
    }
    DECAF_CATCH_RETHROW( Exception )
    DECAF_CATCHALL_THROW( Exception )
}
