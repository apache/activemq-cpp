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

#include "DefaultPrefetchPolicy.h"

#include <decaf/lang/Short.h>

using namespace activemq;
using namespace activemq::core;
using namespace activemq::core::policies;
using namespace decaf;
using namespace decaf::lang;

////////////////////////////////////////////////////////////////////////////////
int DefaultPrefetchPolicy::MAX_PREFETCH_SIZE = Short::MAX_VALUE - 1;
int DefaultPrefetchPolicy::DEFAULT_DURABLE_TOPIC_PREFETCH = 100;
int DefaultPrefetchPolicy::DEFAULT_QUEUE_PREFETCH = 1000;
int DefaultPrefetchPolicy::DEFAULT_QUEUE_BROWSER_PREFETCH = 500;
int DefaultPrefetchPolicy::DEFAULT_TOPIC_PREFETCH = MAX_PREFETCH_SIZE;

////////////////////////////////////////////////////////////////////////////////
DefaultPrefetchPolicy::DefaultPrefetchPolicy() :
    durableTopicPrefetch( DEFAULT_DURABLE_TOPIC_PREFETCH ),
    queuePrefetch( DEFAULT_QUEUE_PREFETCH ),
    queueBrowserPrefetch( DEFAULT_QUEUE_BROWSER_PREFETCH ),
    topicPrefetch( DEFAULT_TOPIC_PREFETCH ) {
}

////////////////////////////////////////////////////////////////////////////////
DefaultPrefetchPolicy::~DefaultPrefetchPolicy() {
}

////////////////////////////////////////////////////////////////////////////////
PrefetchPolicy* DefaultPrefetchPolicy::clone() const {

    DefaultPrefetchPolicy* copy = new DefaultPrefetchPolicy;

    copy->setDurableTopicPrefetch(this->getDurableTopicPrefetch());
    copy->setTopicPrefetch(this->getTopicPrefetch());
    copy->setQueueBrowserPrefetch(this->getQueueBrowserPrefetch());
    copy->setQueuePrefetch(this->getQueuePrefetch());

    return copy;
}
