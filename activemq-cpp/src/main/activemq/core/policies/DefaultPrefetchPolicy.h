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

#ifndef _ACTIVEMQ_CORE_POLICIES_DEFAULTPREFETCHPOLICY_H_
#define _ACTIVEMQ_CORE_POLICIES_DEFAULTPREFETCHPOLICY_H_

#include <activemq/util/Config.h>

#include <activemq/core/PrefetchPolicy.h>

namespace activemq {
namespace core {
namespace policies {

    class AMQCPP_API DefaultPrefetchPolicy : public PrefetchPolicy {
    private:

        int durableTopicPrefetch;
        int queuePrefetch;
        int queueBrowserPrefetch;
        int topicPrefetch;

    public:

        static int MAX_PREFETCH_SIZE;
        static int DEFAULT_DURABLE_TOPIC_PREFETCH;
        static int DEFAULT_QUEUE_PREFETCH;
        static int DEFAULT_QUEUE_BROWSER_PREFETCH;
        static int DEFAULT_TOPIC_PREFETCH;

    private:

        DefaultPrefetchPolicy( const DefaultPrefetchPolicy& );
        DefaultPrefetchPolicy& operator= ( DefaultPrefetchPolicy& );

    public:

        DefaultPrefetchPolicy();

        virtual ~DefaultPrefetchPolicy();

        virtual void setDurableTopicPrefetch( int value ) {
            this->durableTopicPrefetch = getMaxPrefetchLimit( value );
        }

        virtual int getDurableTopicPrefetch() const {
            return this->durableTopicPrefetch;
        }

        virtual void setQueuePrefetch( int value ) {
            this->queuePrefetch = getMaxPrefetchLimit( value );
        }

        virtual int getQueuePrefetch() const {
            return this->queuePrefetch;
        }

        virtual void setQueueBrowserPrefetch( int value ) {
            this->queueBrowserPrefetch = getMaxPrefetchLimit( value );
        }

        virtual int getQueueBrowserPrefetch() const {
            return this->queueBrowserPrefetch;
        }

        virtual void setTopicPrefetch( int value ) {
            this->topicPrefetch = getMaxPrefetchLimit( value );
        }

        virtual int getTopicPrefetch() const {
            return this->topicPrefetch;
        }

        virtual int getMaxPrefetchLimit( int value ) const {
            return value < MAX_PREFETCH_SIZE ? value : MAX_PREFETCH_SIZE;
        }

        virtual PrefetchPolicy* clone() const;

    };

}}}

#endif /* _ACTIVEMQ_CORE_POLICIES_DEFAULTPREFETCHPOLICY_H_ */
