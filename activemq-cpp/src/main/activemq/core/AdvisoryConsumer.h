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

#ifndef _ACTIVEMQ_CORE_ADVISORYCONSUMER_H_
#define _ACTIVEMQ_CORE_ADVISORYCONSUMER_H_

#include <activemq/util/Config.h>
#include <activemq/core/Dispatcher.h>
#include <activemq/core/ActiveMQConnection.h>
#include <activemq/commands/ConsumerId.h>
#include <activemq/commands/DestinationInfo.h>

#include <decaf/lang/Pointer.h>

namespace activemq {
namespace core {

    class AdvisoryConsumerConfig;

    using decaf::lang::Pointer;

    class AMQCPP_API AdvisoryConsumer : Dispatcher {
    private:

        AdvisoryConsumerConfig* config;
        ActiveMQConnection* connection;

    private:

        AdvisoryConsumer(const AdvisoryConsumer&);
        AdvisoryConsumer& operator= (const AdvisoryConsumer&);

    public:

        AdvisoryConsumer(ActiveMQConnection* connection, Pointer<commands::ConsumerId> consumerId);
        virtual ~AdvisoryConsumer();

    public:

        void dispose();

        virtual void dispatch(const Pointer<MessageDispatch>& message);

    private:

        void processDestinationInfo(Pointer<commands::DestinationInfo> destination);

    };

}}

#endif /* _ACTIVEMQ_CORE_ADVISORYCONSUMER_H_ */
