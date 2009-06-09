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

#ifndef _ACTIVEMQ_TRANSPORT_TCP_TCPTRANSPORTFACTORY_H_
#define _ACTIVEMQ_TRANSPORT_TCP_TCPTRANSPORTFACTORY_H_

#include <activemq/util/Config.h>
#include <activemq/transport/AbstractTransportFactory.h>
#include <activemq/exceptions/ActiveMQException.h>

namespace activemq{
namespace transport{
namespace tcp{

    using decaf::lang::Pointer;

    /**
     * Factory Responsible for creating the TcpTransport.
     */
    class AMQCPP_API TcpTransportFactory : public AbstractTransportFactory {
    public:

        virtual ~TcpTransportFactory() {}

        /**
         * Creates a fully configured Transport instance which could be a chain
         * of filters and transports.
         * @param location - URI location to connect to plus any properties to assign.
         * @throws ActiveMQexception if an error occurs
         */
        virtual Pointer<Transport> create( const decaf::net::URI& location )
            throw ( exceptions::ActiveMQException );

        /**
         * Creates a slimed down Transport instance which can be used in composite
         * transport instances.
         * @param location - URI location to connect to plus any properties to assign.
         * @throws ActiveMQexception if an error occurs
         */
        virtual Pointer<Transport> createComposite( const decaf::net::URI& location )
            throw ( exceptions::ActiveMQException );

    protected:

        /**
         * Creates a slimed down Transport instance which can be used in composite
         * transport instances.
         *
         * @param location - URI location to connect to.
         * @param wireFormat - the assigned WireFormat for the new Transport.
         * @param properties - Properties to apply to the transport.
         *
         * @return new Pointer to a TcpTransport.
         * @throws ActiveMQexception if an error occurs
         */
        virtual Pointer<Transport> doCreateComposite( const decaf::net::URI& location,
                                                      const Pointer<wireformat::WireFormat>& wireFormat,
                                                      const decaf::util::Properties& properties )
            throw ( exceptions::ActiveMQException );

    };

}}}

#endif /*_ACTIVEMQ_TRANSPORT_TCP_TCPTRANSPORTFACTORY_H_*/
