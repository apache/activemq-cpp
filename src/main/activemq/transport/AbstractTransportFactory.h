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
#include <decaf/util/NoSuchElementException.h>
#include <decaf/lang/Pointer.h>
#include <decaf/util/Properties.h>

namespace activemq {
namespace transport {

    using decaf::lang::Pointer;

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

    protected:

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
        virtual Pointer<wireformat::WireFormat> createWireFormat(
            const decaf::util::Properties& properties );

    };

}}

#endif /* _ACTIVEMQ_TRANSPORT_ABSTRACTTRANSPORTFACTORY_H_ */
