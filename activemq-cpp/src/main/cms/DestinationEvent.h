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

#ifndef _CMS_DESTINATIONEVENT_H_
#define _CMS_DESTINATIONEVENT_H_

#include <cms/Config.h>
#include <cms/Destination.h>

namespace cms {

    /**
     * An event class that is used to wrap information related to Destination add and
     * remove events from the CMS Provider.
     *
     * @since 3.2
     */
    class CMS_API DestinationEvent {
    public:

        virtual ~DestinationEvent();

        /**
         * Returns the destination that this event is related to, the returned
         * destination remains the property of this event and should be cloned
         * if the caller wishes to store it beyond the lifetime of this event
         * object.
         *
         * @returns a cms::Destination instance that this event relates to.
         */
        virtual const cms::Destination* getDestination() const = 0;

        /**
         * Returns true if this events represents the addition of a Destination.
         *
         * @returns true if this events represents the addition of a Destination.
         */
        virtual bool isAddOperation() const = 0;

        /**
         * Returns true if this events represents the removal of a Destination.
         *
         * @returns true if this events represents the removal of a Destination.
         */
        virtual bool isRemoveOperation() const = 0;

    };
}

#endif /* _CMS_DESTINATIONEVENT_H_ */
