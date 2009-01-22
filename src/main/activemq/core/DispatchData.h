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

#ifndef _ACTIVEMQ_CORE_DISPATCHDATA_H_
#define _ACTIVEMQ_CORE_DISPATCHDATA_H_

#include <stdlib.h>
#include <memory>
#include <activemq/util/Config.h>
#include <activemq/commands/ConsumerId.h>

namespace activemq {
namespace core {

    class ActiveMQMessage;

    /**
     * Contains information about dispatching to a particular consumer.
     */
    class AMQCPP_API DispatchData {
    private:

        commands::ConsumerId* consumerId;
        ActiveMQMessage* message;

    public:

        DispatchData(){
            consumerId = NULL;
            message = NULL;
        }

        DispatchData( commands::ConsumerId* consumer, ActiveMQMessage* message ) {
            this->consumerId = consumer;
            this->message = message;
        }

        DispatchData( const DispatchData& d ) {
            (*this) = d;
        }

        DispatchData& operator =( const DispatchData& d ) {
            this->consumerId = d.consumerId;
            this->message = d.message;
            return *this;
        }

        commands::ConsumerId* getConsumerId() {
            return consumerId;
        }

        ActiveMQMessage* getMessage() {
            return message;
        }

    };
}}

#endif /*_ACTIVEMQ_CORE_DISPATCHDATA_H_*/
