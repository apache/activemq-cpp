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

#ifndef ACTIVEMQ_CORE_DISPATCHER_H_
#define ACTIVEMQ_CORE_DISPATCHER_H_

#include <activemq/commands/MessageDispatch.h>
#include <activemq/util/Config.h>
#include <decaf/lang/Pointer.h>

namespace activemq{
namespace core{

    using decaf::lang::Pointer;
    using activemq::commands::MessageDispatch;

    /**
     * Interface for an object responsible for dispatching messages to
     * consumers.
     */
    class AMQCPP_API Dispatcher {
    public:

        virtual ~Dispatcher(){}

        /**
         * Dispatches a message to a particular consumer.
         *
         * @param message
         *      The message to be dispatched to a waiting consumer.
         */
        virtual void dispatch( const Pointer<MessageDispatch>& message ) = 0;

    };

}}

#endif /*ACTIVEMQ_CORE_DISPATCHER_H_*/
