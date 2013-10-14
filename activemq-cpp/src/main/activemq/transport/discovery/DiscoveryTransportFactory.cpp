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

#include "DiscoveryTransportFactory.h"

#include <activemq/transport/discovery/DiscoveryTransport.h>
#include <activemq/transport/discovery/DiscoveryAgentFactory.h>
#include <activemq/transport/discovery/DiscoveryAgentRegistry.h>
#include <activemq/transport/failover/FailoverTransport.h>
#include <activemq/transport/correlator/ResponseCorrelator.h>
#include <activemq/util/CompositeData.h>
#include <activemq/util/URISupport.h>
#include <activemq/exceptions/ActiveMQException.h>

#include <decaf/lang/Boolean.h>
#include <decaf/lang/Integer.h>
#include <decaf/lang/Long.h>

using namespace decaf;
using namespace decaf::util;
using namespace decaf::lang;
using namespace activemq;
using namespace activemq::exceptions;
using namespace activemq::util;
using namespace activemq::transport;
using namespace activemq::transport::discovery;
using namespace activemq::transport::failover;
using namespace activemq::transport::correlator;

////////////////////////////////////////////////////////////////////////////////
DiscoveryTransportFactory::~DiscoveryTransportFactory() {
}

///////////////////////////////////////////////////////////////////////////////
Pointer<Transport> DiscoveryTransportFactory::create(const decaf::net::URI& location) {

    try {

        // Create the initial Transport, then wrap it in the normal Filters
        Pointer<Transport> transport(doCreateTransport(location));

        // Create the Transport for response correlator
        transport.reset(new ResponseCorrelator(transport));

        return transport;
    }
    AMQ_CATCH_RETHROW(ActiveMQException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, ActiveMQException)
    AMQ_CATCHALL_THROW(ActiveMQException)
}

///////////////////////////////////////////////////////////////////////////////
Pointer<Transport> DiscoveryTransportFactory::createComposite(const decaf::net::URI& location) {
    try {
        return doCreateTransport(location);
    }
    AMQ_CATCH_RETHROW(ActiveMQException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, ActiveMQException)
    AMQ_CATCHALL_THROW(ActiveMQException)
}

///////////////////////////////////////////////////////////////////////////////
Pointer<Transport> DiscoveryTransportFactory::doCreateTransport(const decaf::net::URI& location) {

    try {
        CompositeData composite = URISupport::parseComposite(location);

        // TODO create using factory and pass in params.
        Pointer<CompositeTransport> failover(new FailoverTransport());

        Pointer<DiscoveryTransport> transport(new DiscoveryTransport(failover));

        // TODO set all discovery options on the transport.

        URI agentURI = composite.getComponents().get(0);

        DiscoveryAgentFactory* agentFactory =
            DiscoveryAgentRegistry::getInstance().findFactory(agentURI.getScheme());

        // TODO error?

        Pointer<DiscoveryAgent> agent = agentFactory->createAgent(agentURI);
        transport->setDiscoveryAgent(agent);

        return transport;
    }
    AMQ_CATCH_RETHROW(ActiveMQException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, ActiveMQException)
    AMQ_CATCHALL_THROW(ActiveMQException)
}
