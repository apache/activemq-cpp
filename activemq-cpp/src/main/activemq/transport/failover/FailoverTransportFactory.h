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

#ifndef _ACTIVE_TRANSPORT_FAILOVER_FAILOVERTRANSPORTFACTORY_H_
#define _ACTIVE_TRANSPORT_FAILOVER_FAILOVERTRANSPORTFACTORY_H_

#include <activemq/util/Config.h>

#include <activemq/transport/AbstractTransportFactory.h>
#include <activemq/transport/Transport.h>
#include <activemq/exceptions/ActiveMQException.h>
#include <activemq/wireformat/WireFormat.h>
#include <decaf/net/URI.h>
#include <decaf/util/Properties.h>

namespace activemq {
namespace transport {
namespace failover {

    using decaf::lang::Pointer;

    /**
     * Creates an instance of a FailoverTransport.
     *
     * @since 3.0
     */
    class AMQCPP_API FailoverTransportFactory : public AbstractTransportFactory {
    public:

        virtual ~FailoverTransportFactory() {}

        virtual Pointer<Transport> create( const decaf::net::URI& location );

        virtual Pointer<Transport> createComposite( const decaf::net::URI& location );

    protected:

        /**
         * Creates a slimed down Transport instance which can be used in composite
         * transport instances.
         *
         * @param location - URI location to connect to.
         * @param properties - Properties to apply to the transport.
         *
         * @return Pointer to a new FailoverTransport instance.
         * @throws ActiveMQexception if an error occurs
         */
        virtual Pointer<Transport> doCreateComposite( const decaf::net::URI& location,
                                                      const decaf::util::Properties& properties );

    };

}}}

#endif /* _ACTIVE_TRANSPORT_FAILOVER_FAILOVERTRANSPORTFACTORY_H_ */
