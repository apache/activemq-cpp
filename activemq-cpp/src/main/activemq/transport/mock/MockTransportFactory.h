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

#ifndef _ACTIVEMQ_TRANSPORT_MOCK_MOCKTRANSPORTFACTORY_H_
#define _ACTIVEMQ_TRANSPORT_MOCK_MOCKTRANSPORTFACTORY_H_

#include <activemq/util/Config.h>
#include <activemq/transport/AbstractTransportFactory.h>

namespace activemq{
namespace transport{
namespace mock{

    using decaf::lang::Pointer;

    /**
     * Manufactures MockTransports, which are objects that
     * read from input streams and write to output streams.
     */
    class AMQCPP_API MockTransportFactory : public AbstractTransportFactory {
    public:

        virtual ~MockTransportFactory() {}

        /**
         * Creates a fully configured Transport instance which could be a chain
         * of filters and transports.
         * @param location - URI location to connect to plus any properties to assign.
         * @throws ActiveMQexception if an error occurs
         */
        virtual Pointer<Transport> create( const decaf::net::URI& location );

        /**
         * Creates a slimed down Transport instance which can be used in composite
         * transport instances.
         * @param location - URI location to connect to plus any properties to assign.
         * @throws ActiveMQexception if an error occurs
         */
        virtual Pointer<Transport> createComposite( const decaf::net::URI& location );

    protected:

        /**
         * Creates a slimed down Transport instance which can be used in composite
         * transport instances.
         *
         * @param location - URI location to connect to.
         * @param wireFormat - the assigned WireFormat for the new Transport.
         * @param properties - Properties to apply to the transport.
         *
         * @return Pointer to a new Transport instance.
         * @throws ActiveMQexception if an error occurs
         */
        virtual Pointer<Transport> doCreateComposite( const decaf::net::URI& location,
                                                      const Pointer<wireformat::WireFormat>& wireFormat,
                                                      const decaf::util::Properties& properties );

    };

}}}

#endif /*_ACTIVEMQ_TRANSPORT_MOCK_MOCKTRANSPORTFACTORY_H_*/
