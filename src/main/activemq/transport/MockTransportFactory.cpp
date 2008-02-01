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

#include "MockTransportFactory.h"
#include <activemq/connector/stomp/StompResponseBuilder.h>
#include <activemq/connector/openwire/OpenWireResponseBuilder.h>
#include <activemq/transport/Transport.h>
#include <activemq/transport/MockTransport.h>
#include <activemq/transport/MockTransportFactory.h>
#include <activemq/transport/TransportFactoryMapRegistrar.h>

using namespace activemq;
using namespace activemq::transport;
using namespace activemq::exceptions;
using namespace decaf::util;
using namespace decaf::lang;

////////////////////////////////////////////////////////////////////////////////
Transport* MockTransportFactory::createTransport(
    const decaf::util::Properties& properties,
    Transport* next,
    bool own ) throw ( activemq::exceptions::ActiveMQException ) {

    try{

        // We don't use the next here, so clean it up now.
        if( own == true ) {
            delete next;
        }

        std::string wireFormat =
            properties.getProperty( "wireFormat", "stomp" );

        MockTransport::ResponseBuilder* builder = NULL;

        if( wireFormat == "stomp" ) {
            builder = new connector::stomp::StompResponseBuilder();
        } else if( wireFormat == "openwire" ) {
            builder = new connector::openwire::OpenWireResponseBuilder();
        }

        return new MockTransport( builder, true );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
TransportFactory& MockTransportFactory::getInstance() {

    // Create the one and only instance of the registrar
    static TransportFactoryMapRegistrar registrar(
        "mock", new MockTransportFactory() );

    return registrar.getFactory();
}
