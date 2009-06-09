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

#include "ActiveMQConnectionSupport.h"

#include <activemq/exceptions/ActiveMQException.h>
#include <activemq/core/ActiveMQConstants.h>

#include <decaf/lang/Boolean.h>
#include <decaf/lang/Integer.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>

using namespace std;
using namespace activemq;
using namespace activemq::core;
using namespace activemq::exceptions;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
ActiveMQConnectionSupport::ActiveMQConnectionSupport(
    const Pointer<transport::Transport>& transport,
    const Pointer<decaf::util::Properties>& properties ) {

    if( transport  == NULL ) {
        throw decaf::lang::exceptions::IllegalArgumentException(
            __FILE__, __LINE__,
            "ActiveMQConnectionSupport::ActiveMQConnectionSupport - "
            "Required Parameter 'transport' was NULL.");
    }

    this->properties = properties;
    this->transport = transport;

    // Check the connection options
    this->setAlwaysSyncSend( Boolean::parseBoolean(
        properties->getProperty(
            core::ActiveMQConstants::toString(
                core::ActiveMQConstants::CONNECTION_ALWAYSSYNCSEND ), "false" ) ) );

    this->setUseAsyncSend( Boolean::parseBoolean(
        properties->getProperty(
            core::ActiveMQConstants::toString(
                core::ActiveMQConstants::CONNECTION_USEASYNCSEND ), "false" ) ) );

    this->setProducerWindowSize( decaf::lang::Integer::parseInt(
        properties->getProperty(
            core::ActiveMQConstants::toString(
                core::ActiveMQConstants::CONNECTION_PRODUCERWINDOWSIZE ), "0" ) ) );

    this->setSendTimeout( decaf::lang::Integer::parseInt(
        properties->getProperty(
            core::ActiveMQConstants::toString(
                core::ActiveMQConstants::CONNECTION_SENDTIMEOUT ), "0" ) ) );

    this->setCloseTimeout( decaf::lang::Integer::parseInt(
        properties->getProperty(
            core::ActiveMQConstants::toString(
                core::ActiveMQConstants::CONNECTION_CLOSETIMEOUT ), "15000" ) ) );

    this->setClientId( properties->getProperty(
        core::ActiveMQConstants::toString(
            core::ActiveMQConstants::PARAM_CLIENTID ), "" ) );

    this->setUsername( properties->getProperty(
        core::ActiveMQConstants::toString(
            core::ActiveMQConstants::PARAM_USERNAME ), "" ) );

    this->setPassword( properties->getProperty(
        core::ActiveMQConstants::toString(
            core::ActiveMQConstants::PARAM_PASSWORD ), "" ) );
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQConnectionSupport::~ActiveMQConnectionSupport() {
    try{
        this->shutdownTransport();
    }
    AMQ_CATCH_NOTHROW( ActiveMQException )
    AMQ_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnectionSupport::startupTransport() throw( decaf::lang::Exception ) {
    try {

        if( this->transport.get()  == NULL ) {
            throw decaf::lang::exceptions::IllegalArgumentException(
                __FILE__, __LINE__,
                "ActiveMQConnectionSupport::startupTransport - "
                "Required Object 'transport' was NULL.");
        }

        this->transport->start();
    }
    AMQ_CATCH_RETHROW( decaf::lang::Exception )
    AMQ_CATCHALL_THROW( decaf::lang::Exception )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnectionSupport::shutdownTransport() throw( decaf::lang::Exception ) {

    bool hasException = false;
    exceptions::ActiveMQException e;

    try {

        if( transport.get() != NULL ){

            try{
                transport->close();
            }catch( exceptions::ActiveMQException& ex ){
                if( !hasException ){
                    hasException = true;
                    ex.setMark(__FILE__, __LINE__ );
                    e = ex;
                }
            }

            try{
                transport.reset( NULL );
            }catch( exceptions::ActiveMQException& ex ){
                if( !hasException ){
                    hasException = true;
                    ex.setMark(__FILE__, __LINE__ );
                    e = ex;
                }
            }
        }

        // If we encountered an exception - throw the first one we encountered.
        // This will preserve the stack trace for logging purposes.
        if( hasException ){
            throw e;
        }
    }
    AMQ_CATCH_RETHROW( decaf::lang::Exception )
    AMQ_CATCHALL_THROW( decaf::lang::Exception )
}
