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

#ifndef _ACTIVEMQ_TRANSPORT_TRANSPORTFACTORY_H_
#define _ACTIVEMQ_TRANSPORT_TRANSPORTFACTORY_H_

#include <activemq/util/Config.h>
#include <activemq/exceptions/ActiveMQException.h>
#include <decaf/util/Properties.h>

namespace activemq{
namespace transport{

    class Transport;

    /**
     * Defines the interface for Factories that create Transports or
     * TransportFilters.  Since Transports can be chained, the create
     * method takes a pointer to the next transport in the list, and
     * wether the newly create transport owns the next and should delete
     * it on its own destruction.
     */
    class AMQCPP_API TransportFactory{
    public:

        virtual ~TransportFactory() {}

        /**
         * Creates a Transport instance.
         * @param properties - Object that will hold transport config values
         * @param next - the next transport in the chain, or NULL
         * @param own - does the new Transport own the next
         * @throws ActiveMQexception if an error occurs
         */
        virtual Transport* createTransport(
            const decaf::util::Properties& properties,
            Transport* next = NULL,
            bool own = true ) throw ( exceptions::ActiveMQException ) = 0;

    };

}}

#endif /*_ACTIVEMQ_TRANSPORT_TRANSPORTFACTORY_H_*/
