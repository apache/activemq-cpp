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

#ifndef _ACTIVEMQ_TRANSPORT_DISCOVERY_DISCOVERYTRANSPORTFACTORY_H_
#define _ACTIVEMQ_TRANSPORT_DISCOVERY_DISCOVERYTRANSPORTFACTORY_H_

#include <activemq/util/Config.h>
#include <activemq/transport/AbstractTransportFactory.h>
#include <activemq/transport/Transport.h>
#include <decaf/net/URI.h>
#include <decaf/util/Properties.h>

namespace activemq {
namespace transport {
namespace discovery {

    /**
     * Creates an instance of a DiscoveryTransport.
     *
     * @since 3.9
     */
    class AMQCPP_API DiscoveryTransportFactory : public AbstractTransportFactory {
    public:

        virtual ~DiscoveryTransportFactory();

        virtual Pointer<Transport> create(const decaf::net::URI& location);

        virtual Pointer<Transport> createComposite(const decaf::net::URI& location);

    protected:

        virtual Pointer<Transport> doCreateTransport(const decaf::net::URI& location);

    };

}}}

#endif /* _ACTIVEMQ_TRANSPORT_DISCOVERY_DISCOVERYTRANSPORTFACTORY_H_ */
