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

#include "TcpTransportFactory.h"

#include <activemq/transport/IOTransport.h>
#include <activemq/transport/tcp/TcpTransport.h>
#include <activemq/transport/correlator/ResponseCorrelator.h>
#include <activemq/transport/logging/LoggingTransport.h>
#include <activemq/transport/inactivity/InactivityMonitor.h>
#include <activemq/util/URISupport.h>
#include <activemq/wireformat/WireFormat.h>
#include <decaf/util/Properties.h>
#include <decaf/lang/Integer.h>
#include <decaf/lang/Boolean.h>

using namespace activemq;
using namespace activemq::util;
using namespace activemq::wireformat;
using namespace activemq::transport;
using namespace activemq::transport::tcp;
using namespace activemq::transport::correlator;
using namespace activemq::transport::logging;
using namespace activemq::transport::inactivity;
using namespace activemq::exceptions;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::util;

////////////////////////////////////////////////////////////////////////////////
Pointer<Transport> TcpTransportFactory::create(const decaf::net::URI& location) {

    try {

        Properties properties = activemq::util::URISupport::parseQuery(location.getQuery());

        Pointer<WireFormat> wireFormat = this->createWireFormat(properties);

        // Create the initial Composite Transport, then wrap it in the normal Filters
        // for a non-composite Transport which right now is just a ResponseCorrelator
        Pointer<Transport> transport(doCreateComposite(location, wireFormat, properties));

        transport.reset(new ResponseCorrelator(transport));

        return transport;
    }
    AMQ_CATCH_RETHROW(ActiveMQException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, ActiveMQException)
    AMQ_CATCHALL_THROW(ActiveMQException)
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Transport> TcpTransportFactory::createComposite(const decaf::net::URI& location) {

    try {

        Properties properties = activemq::util::URISupport::parseQuery(location.getQuery());

        Pointer<WireFormat> wireFormat = this->createWireFormat(properties);

        // Create the initial Transport, then wrap it in the normal Filters
        return doCreateComposite(location, wireFormat, properties);
    }
    AMQ_CATCH_RETHROW(ActiveMQException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, ActiveMQException)
    AMQ_CATCHALL_THROW(ActiveMQException)
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Transport> TcpTransportFactory::doCreateComposite(const decaf::net::URI& location,
                                                          const Pointer<wireformat::WireFormat> wireFormat,
                                                          const decaf::util::Properties& properties) {

    try {

        Pointer<Transport> transport(new IOTransport(wireFormat));

        transport.reset(new TcpTransport(transport, location));

        // Give this class and any derived classes a chance to apply value that
        // are set in the properties object.
        doConfigureTransport(transport, properties);

        if (properties.getProperty("transport.useInactivityMonitor", "true") == "true") {
            transport.reset(new InactivityMonitor(transport, properties, wireFormat));
        }

        // If command tracing was enabled, wrap the transport with a logging transport.
        // We support the old CMS value, the ActiveMQ trace value and the NMS useLogging
        // value in order to be more friendly.
        if (properties.getProperty("transport.commandTracingEnabled", "false") == "true" ||
            properties.getProperty("transport.useLogging", "false") == "true" ||
            properties.getProperty("transport.trace", "false") == "true") {

            transport.reset(new LoggingTransport(transport));
        }

        if (wireFormat->hasNegotiator()) {
            transport = wireFormat->createNegotiator(transport);
        }

        return transport;
    }
    AMQ_CATCH_RETHROW(ActiveMQException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, ActiveMQException)
    AMQ_CATCHALL_THROW(ActiveMQException)
}

////////////////////////////////////////////////////////////////////////////////
void TcpTransportFactory::doConfigureTransport(Pointer<Transport> transport,
                                               const decaf::util::Properties& properties) {

    try {

        Pointer<TcpTransport> tcp = transport.dynamicCast<TcpTransport>();

        tcp->setInputBufferSize(Integer::parseInt(properties.getProperty("inputBufferSize", "8192")));
        tcp->setOutputBufferSize(Integer::parseInt(properties.getProperty("outputBufferSize", "8192")));
        tcp->setTrace(Boolean::parseBoolean(properties.getProperty("transport.tcpTracingEnabled", "false")));
        tcp->setLinger(Integer::parseInt(properties.getProperty("soLinger", "-1")));
        tcp->setKeepAlive(Boolean::parseBoolean(properties.getProperty("soKeepAlive", "false")));
        tcp->setReceiveBufferSize(Integer::parseInt(properties.getProperty("soReceiveBufferSize", "-1")));
        tcp->setSendBufferSize(Integer::parseInt(properties.getProperty("soSendBufferSize", "-1")));
        tcp->setTcpNoDelay(Boolean::parseBoolean(properties.getProperty("tcpNoDelay", "true")));
        tcp->setConnectTimeout(Integer::parseInt(properties.getProperty("soConnectTimeout", "0")));
    }
    AMQ_CATCH_RETHROW(ActiveMQException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, ActiveMQException)
    AMQ_CATCHALL_THROW(ActiveMQException)
}
