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

#ifndef _ACTIVEMQ_TRANSPORT_ABSTRACTTRANSPORTFACTORY_H_
#define _ACTIVEMQ_TRANSPORT_ABSTRACTTRANSPORTFACTORY_H_

#include <activemq/util/Config.h>
#include <activemq/transport/Transport.h>
#include <activemq/transport/TransportFactory.h>
#include <activemq/wireformat/WireFormat.h>
#include <decaf/lang/exceptions/NoSuchElementException.h>
#include <decaf/util/Properties.h>

namespace activemq {
namespace transport {

    /**
     * Abstract implementation of the TransportFactory interface, providing
     * the base functionality that's common to most of the TransportFactory
     * instances.
     *
     * @since 3.0
     */
    class AMQCPP_API AbstractTransportFactory : public TransportFactory {
    public:

        virtual ~AbstractTransportFactory() {}

        /**
         * Creates a fully configured Transport instance which could be a chain
         * of filters and transports.
         * @param location - URI location to connect to plus any properties to assign.
         * @throws ActiveMQexception if an error occurs
         */
        virtual Transport* create( const decaf::net::URI& location )
            throw ( exceptions::ActiveMQException );

        /**
         * Creates a slimed down Transport instance which can be used in composite
         * transport instances.
         * @param location - URI location to connect to plus any properties to assign.
         * @throws ActiveMQexception if an error occurs
         */
        virtual Transport* createComposite( const decaf::net::URI& location )
            throw ( exceptions::ActiveMQException );

    protected:

        /**
         * Creates a slimed down Transport instance which can be used in composite
         * transport instances or as the basis for a fully wrapped Transport.  This
         * method must be implemented by the actual TransportFactory that extends this
         * abstract base class.
         * @param location - URI location to connect to.
         * @param wireformat - the assigned WireFormat for the new Transport.
         * @param properties - Properties to apply to the transport.
         * @throws ActiveMQexception if an error occurs
         */
        virtual Transport* doCreateComposite( const decaf::net::URI& location,
                                              wireformat::WireFormat* wireFormat,
                                              const decaf::util::Properties& properties )
            throw ( exceptions::ActiveMQException ) = 0;

        /**
         * Creates the WireFormat that is configured for this Transport and returns it.
         * The default WireFormat is Openwire.
         *
         * @param properties
         *        The properties that were configured on the URI.
         *
         * @returns a pointer to a WireFormat instance that the caller then owns.
         *
         * @throws NoSuchElementException if the configured WireFormat is not found.
         */
        virtual wireformat::WireFormat* createWireFormat(
            const decaf::util::Properties& properties )
                throw( decaf::lang::exceptions::NoSuchElementException );

    };

}}

#endif /* _ACTIVEMQ_TRANSPORT_ABSTRACTTRANSPORTFACTORY_H_ */
