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
#include <activemq/transport/TransportFactoryMap.h>
#include <activemq/exceptions/ActiveMQException.h>

using namespace std;
using namespace activemq;
using namespace activemq::exceptions;
using namespace activemq::transport;
using namespace decaf;
using namespace decaf::util;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
TransportBuilder::TransportBuilder() {
}

////////////////////////////////////////////////////////////////////////////////
TransportBuilder::~TransportBuilder() {
}

////////////////////////////////////////////////////////////////////////////////
Transport* TransportBuilder::buildTransport( const std::string& url,
                                             util::Properties& properties )
    throw ( cms::CMSException ) {

    try{

        Transport* transport = NULL;

        // Parse out the properties from the URI
        parseURL( url, properties );

        // Create the Base IO Transport
        transport = this->createTransport(
            "transport.IOTransport", properties );

        // Create the Transport for our protocol
        transport = this->createTransport(
            properties.getProperty( "transport.protocol", "tcp" ),
            properties,
            transport );

        // If async sends are enabled, wrap the transport with a AsyncSendTransport
        // do this before the response correlator so that all commands go out on the
        // send message queue, otherwise messages could get sent out of order.
        if( properties.getProperty( "transport.useAsyncSend", "false" ) == "true" ) {
            // Create the Transport for response correlator
            transport = this->createTransport(
                "transport.filters.AsyncSendTransport", properties, transport );
        }

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
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void TransportBuilder::parseURL( const std::string& URI,
                                 util::Properties& properties )
    throw ( decaf::lang::exceptions::IllegalArgumentException ) {

    try
    {
        StringTokenizer tokenizer( URI, ":/" );

        std::vector<std::string> tokens;

        // Require that there be three tokens at the least, these are
        // transport, url, port.
        if( tokenizer.countTokens() < 3 )
        {
            throw decaf::lang::exceptions::IllegalArgumentException(
                __FILE__, __LINE__,
                (string( "TransportBuilder::parseURL - "
                         "Marlformed URI: ") + URI).c_str() );
        }

        // First element should be the Transport Type, following that is the
        // URL and any params.
        properties.setProperty( "transport.protocol", tokenizer.nextToken() );

        // Parse URL and Port as one item, optional params follow the ?
        // and then each param set is delimited with & we extract first
        // three chars as they are the left over ://
        properties.setProperty( "transport.uri", tokenizer.nextToken("&?").substr(3) );

        // Now get all the optional parameters and store them as properties
        int count = tokenizer.toArray( tokens );

        for( int i = 0; i < count; ++i )
        {
            tokenizer.reset( tokens[i], "=" );

            if( tokenizer.countTokens() != 2 )
            {
                throw decaf::lang::exceptions::IllegalArgumentException(
                    __FILE__, __LINE__,
                    ( string( "TransportBuilder::parseURL - "
                              "Marlformed Parameter = " ) + tokens[i] ).c_str() );
            }

            // Get them in order, passing both as nextToken calls in the
            // set Property can cause reversed order.
            string key   = tokenizer.nextToken();
            string value = tokenizer.nextToken();

            // Store this param as a property
            properties.setProperty( key, value );
        }
    }
    AMQ_CATCH_RETHROW( IllegalArgumentException )
    AMQ_CATCH_EXCEPTION_CONVERT( ActiveMQException, IllegalArgumentException )
    AMQ_CATCHALL_THROW( IllegalArgumentException )
}

////////////////////////////////////////////////////////////////////////////////
Transport* TransportBuilder::createTransport( const std::string& name,
                                              const util::Properties& properties,
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
    AMQ_CATCHALL_THROW( ActiveMQException )
}
