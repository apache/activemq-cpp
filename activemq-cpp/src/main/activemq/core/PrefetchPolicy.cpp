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

#include "PrefetchPolicy.h"

#include <decaf/lang/Integer.h>

using namespace activemq;
using namespace activemq::core;
using namespace decaf;
using namespace decaf::lang;

////////////////////////////////////////////////////////////////////////////////
PrefetchPolicy::PrefetchPolicy() {
}

////////////////////////////////////////////////////////////////////////////////
PrefetchPolicy::~PrefetchPolicy() {
}

////////////////////////////////////////////////////////////////////////////////
void PrefetchPolicy::configure( const decaf::util::Properties& properties ) {

    try{

        if( properties.hasProperty( "cms.PrefetchPolicy.durableTopicPrefetch" ) ) {
            this->setDurableTopicPrefetch( Integer::parseInt(
                properties.getProperty( "cms.PrefetchPolicy.durableTopicPrefetch" ) ) );
        }
        if( properties.hasProperty( "cms.PrefetchPolicy.queueBrowserPrefetch" ) ) {
            this->setQueueBrowserPrefetch( Integer::parseInt(
                properties.getProperty( "cms.PrefetchPolicy.queueBrowserPrefetch" ) ) );
        }
        if( properties.hasProperty( "cms.PrefetchPolicy.queuePrefetch" ) ) {
            this->setQueuePrefetch( Integer::parseInt(
                properties.getProperty( "cms.PrefetchPolicy.queuePrefetch" ) ) );
        }
        if( properties.hasProperty( "cms.PrefetchPolicy.topicPrefetch" ) ) {
            this->setTopicPrefetch( Integer::parseInt(
                properties.getProperty( "cms.PrefetchPolicy.topicPrefetch" ) ) );
        }

        if( properties.hasProperty( "cms.PrefetchPolicy.all" ) ) {
            int value = Integer::parseInt( properties.getProperty( "cms.PrefetchPolicy.all" ) );

            this->setDurableTopicPrefetch( value );
            this->setQueueBrowserPrefetch( value );
            this->setQueuePrefetch( value );
            this->setTopicPrefetch( value );
        }
    }
    DECAF_CATCH_RETHROW( Exception )
    DECAF_CATCHALL_THROW( Exception )
}
