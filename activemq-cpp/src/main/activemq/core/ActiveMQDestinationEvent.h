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

#ifndef _ACTIVEMQ_CORE_ACTIVEMQDESTINATIONEVENT_H_
#define _ACTIVEMQ_CORE_ACTIVEMQDESTINATIONEVENT_H_

#include <cms/DestinationEvent.h>

#include <decaf/lang/Pointer.h>
#include <activemq/commands/DestinationInfo.h>
#include <activemq/util/Config.h>

namespace activemq {
namespace core {

    class AMQCPP_API ActiveMQDestinationEvent : public cms::DestinationEvent {
    private:

        ActiveMQDestinationEvent(const ActiveMQDestinationEvent&);
        ActiveMQDestinationEvent& operator= (const ActiveMQDestinationEvent&);

    private:

        decaf::lang::Pointer<commands::DestinationInfo> destination;

    public:

        ActiveMQDestinationEvent(decaf::lang::Pointer<commands::DestinationInfo> destination);

        virtual ~ActiveMQDestinationEvent();

        virtual const cms::Destination* getDestination() const;

        virtual bool isAddOperation() const;

        virtual bool isRemoveOperation() const;

    public:

        /**
         * Returns the DestinationInfo object that triggered this event.
         *
         * @returns the DestinationInfo object that triggered this event.
         */
        decaf::lang::Pointer<commands::DestinationInfo> getDestinationInfo() const {
            return this->destination;
        }

    };

}}

#endif /* _ACTIVEMQ_CORE_ACTIVEMQDESTINATIONEVENT_H_ */
