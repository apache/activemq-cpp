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

#include "ResponseCorrelatorFactory.h"

#include <activemq/transport/filters/ResponseCorrelator.h>
#include <activemq/util/Long.h>

using namespace activemq;
using namespace activemq::transport;
using namespace activemq::transport::filters;
using namespace activemq::exceptions;
using namespace activemq::util;

////////////////////////////////////////////////////////////////////////////////
TransportFactory& ResponseCorrelatorFactory::getInstance(void)
{
    // Create the one and only instance of the registrar
    static TransportFactoryMapRegistrar registrar(
        "transport.filters.ResponseCorrelator",
        new ResponseCorrelatorFactory() );

    return registrar.getFactory();
}

////////////////////////////////////////////////////////////////////////////////
Transport* ResponseCorrelatorFactory::createTransport(
    const activemq::util::Properties& properties AMQCPP_UNUSED,
    Transport* next,
    bool own ) throw ( ActiveMQException ) {

    try {

        ResponseCorrelator* transport = new ResponseCorrelator( next, own );

        transport->setMaxResponseWaitTime(
            (unsigned long)Long::parseLong( 
                properties.getProperty( 
                    "transport.ResponseCorrelator.maxResponseWaitTime", "3000" ) ) );

        return transport;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}
