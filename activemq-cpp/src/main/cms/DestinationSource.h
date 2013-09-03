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

#ifndef _CMS_DESTINATIONSOURCE_H_
#define _CMS_DESTINATIONSOURCE_H_

#include <cms/Config.h>
#include <cms/DestinationListener.h>
#include <cms/Topic.h>
#include <cms/Queue.h>
#include <cms/TemporaryQueue.h>
#include <cms/TemporaryTopic.h>
#include <cms/Startable.h>
#include <cms/Stoppable.h>

namespace cms {

    /**
     * Provides an object that will provide a snapshot view of Destinations that exist
     * on the Message provider.
     *
     * @since 3.2
     */
    class CMS_API DestinationSource : public cms::Startable, public cms::Stoppable {
    public:

        virtual ~DestinationSource();

        /**
         * Sets the current listener of Destination events.
         *
         * This class does not manage the lifetime of the configured listener, the client
         * must ensure that it deletes the listener instance at the appropriate time.
         *
         * @param listener
         *      The new listener to provide destination events to.
         */
        virtual void setListener(cms::DestinationListener* listener) = 0;

        /**
         * Gets the currently configured DestiantionListener for this event source.
         *
         * The event source instance remains active in this DestinationSource until
         * it is removed via a call to setListener(null) and should not be deleted
         * until the client is sure it will not receive any future events.
         *
         * @returns the configured DestinationListener for this event source or null if none.
         */
        virtual cms::DestinationListener* getListener() const = 0;

        /**
         * Returns a snapshot of the currently known Queues that are active on the CMS
         * provider, this state can change at any time as Destinations are added and deleted
         * from the provider.
         *
         * The destinations are cloned and placed into the returned vector, the caller is
         * responsible for deleting these cloned objects.
         *
         * @returns an STL vector containing the current list of known Queues.
         */
        virtual std::vector<cms::Queue*> getQueues() const = 0;

        /**
         * Returns a snapshot of the currently known Queues that are active on the CMS
         * provider, this state can change at any time as Destinations are added and deleted
         * from the provider.
         *
         * The destinations are cloned and placed into the returned vector, the caller is
         * responsible for deleting these cloned objects.
         *
         * @returns an STL vector containing the current list of known Queues.
         */
        virtual std::vector<cms::Topic*> getTopics() const = 0;

        /**
         * Returns a snapshot of the currently known Queues that are active on the CMS
         * provider, this state can change at any time as Destinations are added and deleted
         * from the provider.
         *
         * The destinations are cloned and placed into the returned vector, the caller is
         * responsible for deleting these cloned objects.
         *
         * @returns an STL vector containing the current list of known Queues.
         */
        virtual std::vector<cms::TemporaryQueue*> getTemporaryQueues() const = 0;

        /**
         * Returns a snapshot of the currently known Queues that are active on the CMS
         * provider, this state can change at any time as Destinations are added and deleted
         * from the provider.
         *
         * The destinations are cloned and placed into the returned vector, the caller is
         * responsible for deleting these cloned objects.
         *
         * @returns an STL vector containing the current list of known Queues.
         */
        virtual std::vector<cms::TemporaryTopic*> getTemporaryTopics() const = 0;

    };

}

#endif /* _CMS_DESTINATIONSOURCE_H_ */
