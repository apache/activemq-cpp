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

#include "AsyncSendTransportFactory.h"

#include <activemq/transport/filters/AsyncSendTransport.h>
#include <decaf/lang/Integer.h>

using namespace activemq;
using namespace activemq::transport;
using namespace activemq::transport::filters;
using namespace activemq::exceptions;
using namespace decaf;
using namespace decaf::lang;

////////////////////////////////////////////////////////////////////////////////
TransportFactory& AsyncSendTransportFactory::getInstance() {

    // Create the one and only instance of the registrar
    static TransportFactoryMapRegistrar registrar(
        "transport.filters.AsyncSendTransport",
        new AsyncSendTransportFactory() );

    return registrar.getFactory();
}

////////////////////////////////////////////////////////////////////////////////
Transport* AsyncSendTransportFactory::createTransport(
    const decaf::util::Properties& properties AMQCPP_UNUSED,
    Transport* next,
    bool own ) throw ( ActiveMQException ) {

    try{

        unsigned int maxBacklog = Integer::parseInt(
            properties.getProperty( "transport.maxAsyncSendBacklog", "0" ) );

        return new AsyncSendTransport( next, maxBacklog, own );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}
