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

#include "SslTransportFactory.h"

#include <activemq/transport/tcp/SslTransport.h>

#include <activemq/transport/IOTransport.h>
#include <activemq/transport/inactivity/InactivityMonitor.h>
#include <activemq/transport/logging/LoggingTransport.h>

#include <memory>

using namespace std;
using namespace activemq;
using namespace activemq::io;
using namespace activemq::transport;
using namespace activemq::transport::logging;
using namespace activemq::transport::inactivity;
using namespace activemq::transport::tcp;
using namespace activemq::exceptions;
using namespace decaf;
using namespace decaf::net;
using namespace decaf::util;
using namespace decaf::io;
using namespace decaf::lang;

////////////////////////////////////////////////////////////////////////////////
SslTransportFactory::~SslTransportFactory() {
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Transport> SslTransportFactory::doCreateComposite(const decaf::net::URI& location,
                                                          const Pointer<wireformat::WireFormat> wireFormat,
                                                          const decaf::util::Properties& properties) {

    try {

        Pointer<Transport> transport(new IOTransport(wireFormat));

        transport.reset(new SslTransport(transport, location));

        // Give this class and any derived classes a chance to apply value that
        // are set in the properties object.
        doConfigureTransport(transport, properties);

        if (properties.getProperty("transport.useInactivityMonitor", "true") == "true") {
            transport.reset(new InactivityMonitor(transport, properties, wireFormat));
        }

        // If command tracing was enabled, wrap the transport with a logging transport.
        if (properties.getProperty("transport.commandTracingEnabled", "false") == "true") {
            // Create the Transport for response correlator
            transport.reset(new LoggingTransport(transport));
        }

        // If there is a negotiator need then we create and wrap here.
        if (wireFormat->hasNegotiator()) {
            transport = wireFormat->createNegotiator(transport);
        }

        return transport;
    }
    AMQ_CATCH_RETHROW(ActiveMQException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, ActiveMQException)
    AMQ_CATCHALL_THROW(ActiveMQException)
}
