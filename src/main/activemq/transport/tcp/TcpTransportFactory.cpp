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

////////////////////////////////////////////////////////////////////////////////
Pointer<Transport> TcpTransportFactory::create( const decaf::net::URI& location ) {

    try{

        Properties properties =
            activemq::util::URISupport::parseQuery( location.getQuery() );

        Pointer<WireFormat> wireFormat = this->createWireFormat( properties );

        // Create the initial Transport, then wrap it in the normal Filters
        Pointer<Transport> transport( doCreateComposite( location, wireFormat, properties ) );

        // Create the Transport for response correlator
        transport.reset( new ResponseCorrelator( transport ) );

        return transport;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Transport> TcpTransportFactory::createComposite( const decaf::net::URI& location ) {

    try{

        Properties properties =
            activemq::util::URISupport::parseQuery( location.getQuery() );

        Pointer<WireFormat> wireFormat = this->createWireFormat( properties );

        // Create the initial Transport, then wrap it in the normal Filters
        return doCreateComposite( location, wireFormat, properties );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Transport> TcpTransportFactory::doCreateComposite( const decaf::net::URI& location,
                                                           const Pointer<wireformat::WireFormat>& wireFormat,
                                                           const decaf::util::Properties& properties ) {

    try {

        Pointer<Transport> transport( new TcpTransport(
            Pointer<Transport>( new IOTransport( wireFormat ) ) ) );

        // Initialize the Transport, creates Sockets and configures defaults.
        transport.dynamicCast<TcpTransport>()->connect( location, properties );

        if( properties.getProperty( "transport.useInactivityMonitor", "true" ) == "true" ) {
            transport.reset( new InactivityMonitor( transport, properties, wireFormat ) );
        }

        // If command tracing was enabled, wrap the transport with a logging transport.
        // We support the old CMS value, the ActiveMQ trace value and the NMS useLogging
        // value in order to be more friendly.
        if( properties.getProperty( "transport.commandTracingEnabled", "false" ) == "true" ||
            properties.getProperty( "transport.useLogging", "false" ) == "true"  ||
            properties.getProperty( "transport.trace", "false" ) == "true" ) {

            // Create the Transport for response correlator
            transport.reset( new LoggingTransport( transport ) );
        }

        // If there is a negotiator need then we create and wrap here.
        if( wireFormat->hasNegotiator() ) {
            transport = wireFormat->createNegotiator( transport );
        }

        return transport;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}
