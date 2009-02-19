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

#include "FailoverTransportFactory.h"

#include <activemq/transport/failover/FailoverTransport.h>

#include <decaf/lang/Boolean.h>
#include <decaf/lang/Integer.h>
#include <decaf/lang/Long.h>
#include <memory>

using namespace activemq;
using namespace activemq::transport;
using namespace activemq::transport::failover;
using namespace activemq::exceptions;
using namespace decaf;
using namespace decaf::util;
using namespace decaf::lang;

////////////////////////////////////////////////////////////////////////////////
Pointer<Transport> FailoverTransportFactory::doCreateComposite(
    const decaf::net::URI& location,
    const Pointer<wireformat::WireFormat>& wireFormat,
    const decaf::util::Properties& properties )
        throw ( exceptions::ActiveMQException ) {

    try {

        Pointer<FailoverTransport> transport( new FailoverTransport() );

        transport->setInitialReconnectDelay(
            Long::parseLong( properties.getProperty( "initialReconnectDelay", "10" ) ) );
        transport->setMaxReconnectDelay(
            Long::parseLong( properties.getProperty( "maxReconnectDelay", "30000" ) ) );
        transport->setUseExponentialBackOff(
            Boolean::parseBoolean( properties.getProperty( "useExponentialBackOff", "true" ) ) );
        transport->setMaxReconnectAttempts(
            Integer::parseInt( properties.getProperty( "maxReconnectAttempts", "0" ) ) );
        transport->setRandomize(
            Boolean::parseBoolean( properties.getProperty( "randomize", "true" ) ) );
        transport->setBackup(
            Boolean::parseBoolean( properties.getProperty( "backup", "false" ) ) );
        transport->setBackupPoolSize(
            Integer::parseInt( properties.getProperty( "backupPoolSize", "1" ) ) );
        transport->setTimeout(
            Long::parseLong( properties.getProperty( "timeout", "-1" ) ) );
        transport->setTrackMessages(
            Boolean::parseBoolean( properties.getProperty( "trackMessages", "false" ) ) );
        transport->setMaxCacheSize(
            Integer::parseInt( properties.getProperty( "maxCacheSize", "131072" ) ) );

        return transport;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}
