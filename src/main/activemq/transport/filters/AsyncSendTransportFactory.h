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

#ifndef _ACTIVEMQ_TRANSPORT_FILTERS_ASYNCSENDTRANSPORTFACTORY_H_
#define _ACTIVEMQ_TRANSPORT_FILTERS_ASYNCSENDTRANSPORTFACTORY_H_

#include <activemq/util/Config.h>
#include <activemq/transport/TransportFactory.h>
#include <activemq/transport/TransportFactoryMapRegistrar.h>
#include <activemq/exceptions/ActiveMQException.h>

namespace activemq{
namespace transport{
namespace filters{

    /**
     * Factory Responsible for creating the AsyncSendTransport.
     */
    class AMQCPP_API AsyncSendTransportFactory : public TransportFactory {
    public:

        virtual ~AsyncSendTransportFactory() {}

        /**
         * Creates a Transport instance.
         * @param properties - Object that will hold transport config values
         * @param next - the next transport in the chain, or NULL
         * @param own - does the new Transport own the next
         * @throws ActiveMQException if an error occurs.
         */
        virtual Transport* createTransport(
            const decaf::util::Properties& properties,
            Transport* next,
            bool own ) throw ( exceptions::ActiveMQException );

        /**
         * Returns a reference to this TransportFactory
         * @returns TransportFactory Reference
         */
        static TransportFactory& getInstance();

    };

}}}

#endif /*_ACTIVEMQ_TRANSPORT_FILTERS_ASYNCSENDTRANSPORTFACTORY_H_*/
