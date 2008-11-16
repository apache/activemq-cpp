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

#include "TransportBuilder.h"

#include <decaf/util/StringTokenizer.h>
#include <activemq/transport/TransportFactory.h>
#include <activemq/transport/TransportFactoryMap.h>
#include <activemq/exceptions/ActiveMQException.h>
#include <activemq/util/URISupport.h>

using namespace std;
using namespace activemq;
using namespace activemq::exceptions;
using namespace activemq::transport;
using namespace activemq::util;
using namespace decaf;
using namespace decaf::util;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
Transport* TransportBuilder::buildTransport( const std::string& url,
                                             decaf::util::Properties& properties )
    throw ( cms::CMSException ) {

    try{

        Transport* transport = NULL;

        // Parse out the properties from the URI
        URISupport::parseURL( url, properties );

        // Create the Base IO Transport
        transport = this->createTransport(
            "transport.IOTransport", properties );

        // Create the Transport for our protocol
        transport = this->createTransport(
            properties.getProperty( "transport.protocol", "tcp" ),
            properties,
            transport );

        // Create the Transport for response correlator
        transport = this->createTransport(
            "transport.filters.ResponseCorrelator", properties, transport );

        // If command tracing was enabled, wrap the transport with a logging transport.
        if( properties.getProperty( "transport.commandTracingEnabled", "false" ) == "true" ) {
            // Create the Transport for response correlator
            transport = this->createTransport(
                "transport.filters.LoggingTransport", properties, transport );
        }

        return transport;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
Transport* TransportBuilder::createTransport( const std::string& name,
                                              const decaf::util::Properties& properties,
                                              Transport* next )
    throw ( cms::CMSException ) {

    try{

        // Create the Transport that the Connector will use.
        TransportFactory* factory =
            TransportFactoryMap::getInstance().lookup( name );

        if( factory == NULL ){
            throw ActiveMQException(
                __FILE__, __LINE__,
                ( string( "TransportBuilder::createTransport - " ) +
                  string( "unknown transport factory" ) + name ).c_str() );
        }

        // Create the transport.
        Transport* transport = factory->createTransport( properties, next, true );
        if( transport == NULL ){
            throw ActiveMQException(
                __FILE__, __LINE__,
                ( string( "TransportBuilder::createTransport - " ) +
                  string( "failed creating new Transport" ) + name ).c_str() );
        }

        return transport;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}
