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

#ifndef _ACTIVEMQ_CORE_PREFETCHPOLICY_H_
#define _ACTIVEMQ_CORE_PREFETCHPOLICY_H_

#include <activemq/util/Config.h>

#include <decaf/util/Properties.h>

namespace activemq {
namespace core {

    /**
     * Interface for a Policy object that controls message Prefetching on various
     * destination types in ActiveMQ-CPP.
     *
     * @since 3.2.0
     */
    class AMQCPP_API PrefetchPolicy {
    private:

        PrefetchPolicy( const PrefetchPolicy& );
        PrefetchPolicy& operator= ( const PrefetchPolicy& );

    protected:

        PrefetchPolicy();

    public:

        virtual ~PrefetchPolicy();

        /**
         * Sets the amount of prefetched messages for a Durable Topic.
         *
         * @param value
         *      The number of messages to prefetch.
         */
        virtual void setDurableTopicPrefetch( int value ) = 0;

        /**
         * Gets the amount of messages to prefetch for a Durable Topic.
         *
         * @returns value of the number of messages to prefetch.
         */
        virtual int getDurableTopicPrefetch() const = 0;

        /**
         * Sets the amount of prefetched messages for a Queue.
         *
         * @param value
         *      The number of messages to prefetch.
         */
        virtual void setQueuePrefetch( int value ) = 0;

        /**
         * Gets the amount of messages to prefetch for a Queue.
         *
         * @returns value of the number of messages to prefetch.
         */
        virtual int getQueuePrefetch() const = 0;

        /**
         * Sets the amount of prefetched messages for a Queue Browser.
         *
         * @param value
         *      The number of messages to prefetch.
         */
        virtual void setQueueBrowserPrefetch( int value ) = 0;

        /**
         * Gets the amount of messages to prefetch for a Queue Browser.
         *
         * @returns value of the number of messages to prefetch.
         */
        virtual int getQueueBrowserPrefetch() const = 0;

        /**
         * Sets the amount of prefetched messages for a Topic.
         *
         * @param value
         *      The number of messages to prefetch.
         */
        virtual void setTopicPrefetch( int value ) = 0;

        /**
         * Gets the amount of messages to prefetch for a Topic.
         *
         * @returns value of the number of messages to prefetch.
         */
        virtual int getTopicPrefetch() const = 0;

        /**
         * Given a requested value for a new prefetch limit, compare it against some max
         * prefetch value and return either the requested value or the maximum allowable
         * value for prefetch.
         *
         * @returns the allowable value for a prefetch limit, either requested or the max.
         */
        virtual int getMaxPrefetchLimit( int value ) const = 0;

        /**
         * Clone the Policy and return a new pointer to that clone.
         *
         * @return pointer to a new PrefetchPolicy instance that is a clone of this one.
         */
        virtual PrefetchPolicy* clone() const = 0;

        /**
         * Checks the supplied properties object for properties matching the configurable
         * settings of this class.  The default implementation looks for properties named
         * with the prefix cms.PrefetchPolicy.XXX where XXX is the name of a property with
         * a public setter method.  For instance cms.PrefetchPolicy.topicPrefetch will be
         * used to set the value of the topic prefetch limit.
         *
         * Subclasses can override this method to add more configuration options or to exclude
         * certain parameters from being set via the properties object.
         *
         * @param properties
         *      The Properties object used to configure this object.
         *
         * @throws NumberFormatException if a property that is numeric cannot be converted
         * @throws IllegalArgumentException if a property can't be converted to the correct type.
         */
        virtual void configure( const decaf::util::Properties& properties );

    };

}}

#endif /* _ACTIVEMQ_CORE_PREFETCHPOLICY_H_ */
