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

#include "LoggingTransportFactory.h"

#include <activemq/transport/filters/LoggingTransport.h>

using namespace decaf::lang;
using namespace activemq;
using namespace activemq::transport;
using namespace activemq::transport::filters;
using namespace activemq::exceptions;

////////////////////////////////////////////////////////////////////////////////
TransportFactory& LoggingTransportFactory::getInstance() {

    // Create the one and only instance of the registrar
    static TransportFactoryMapRegistrar registrar(
        "transport.filters.LoggingTransport",
        new LoggingTransportFactory() );

    return registrar.getFactory();
}

////////////////////////////////////////////////////////////////////////////////
Transport* LoggingTransportFactory::createTransport(
    const decaf::util::Properties& properties AMQCPP_UNUSED,
    Transport* next,
    bool own ) throw ( ActiveMQException ) {

    try {
        return new LoggingTransport( next, own );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}
