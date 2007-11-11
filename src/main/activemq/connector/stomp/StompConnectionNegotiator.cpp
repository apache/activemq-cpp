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

#include "StompConnectionNegotiator.h"

#include <activemq/connector/stomp/commands/ConnectCommand.h>
#include <activemq/connector/stomp/commands/ConnectedCommand.h>
#include <activemq/exceptions/ActiveMQException.h>

using namespace std;
using namespace activemq;
using namespace activemq::exceptions;
using namespace activemq::transport;
using namespace activemq::connector;
using namespace activemq::connector::stomp;
using namespace activemq::connector::stomp::commands;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::util;
using namespace decaf::util::concurrent;

////////////////////////////////////////////////////////////////////////////////
StompConnectionNegotiator::StompConnectionNegotiator( Transport* next, bool own ) :
    TransportFilter( next, own ),
    readyCountDownLatch(1) {

    this->connected = false;
    this->closed = true;
    this->connectedCmd = NULL;
}

////////////////////////////////////////////////////////////////////////////////
StompConnectionNegotiator::~StompConnectionNegotiator() {

    try{
        // Close the transport and destroy it.
        close();
    }
    AMQ_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void StompConnectionNegotiator::oneway( Command* command )
    throw( CommandIOException,
           decaf::lang::exceptions::UnsupportedOperationException ) {

    try{

        if( closed || next == NULL ){
            throw CommandIOException(
                __FILE__, __LINE__,
                "StompConnectionNegotiator::oneway - transport already closed" );
        }

        next->oneway( command );
    }
    AMQ_CATCH_RETHROW( UnsupportedOperationException )
    AMQ_CATCH_RETHROW( CommandIOException )
    AMQ_CATCH_EXCEPTION_CONVERT( ActiveMQException, CommandIOException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, CommandIOException )
    AMQ_CATCHALL_THROW( CommandIOException )
}

////////////////////////////////////////////////////////////////////////////////
Response* StompConnectionNegotiator::request( Command* command )
    throw( CommandIOException, decaf::lang::exceptions::UnsupportedOperationException ) {

    try{

        if( closed || next == NULL ){
            throw CommandIOException(
                __FILE__, __LINE__,
                "StompConnectionNegotiator::request - transport already closed" );
        }

        // Once connected we just pass through all requests.
        if( connected ) {
            return next->request( command );
        } else {

            ConnectCommand* connect = dynamic_cast<ConnectCommand*>( command );

            if( connect == NULL ) {
                throw CommandIOException(
                    __FILE__,
                    __LINE__,
                    "StompConnectionNegotiator::request"
                    "Invalid Command Received: only a connect command "
                    "can be sent before connected." );
            }

            // Send the connect request
            next->oneway( command );

            if( !readyCountDownLatch.await( negotiationTimeout ) ) {
                throw CommandIOException(
                    __FILE__,
                    __LINE__,
                    "StompConnectionNegotiator::request"
                    "Connection Negotiate timeout: peer did not "
                    "send a connected command." );
            }

            // return the connected command
            return connectedCmd;
        }
    }
    AMQ_CATCH_RETHROW( UnsupportedOperationException )
    AMQ_CATCH_RETHROW( CommandIOException )
    AMQ_CATCH_EXCEPTION_CONVERT( ActiveMQException, CommandIOException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, CommandIOException )
    AMQ_CATCHALL_THROW( CommandIOException )
}

////////////////////////////////////////////////////////////////////////////////
void StompConnectionNegotiator::onCommand( Command* command ) {

    ConnectedCommand* response =
        dynamic_cast<ConnectedCommand*>( command );

    if( response != NULL && !this->connected ) {

        // Store for the main thread.
        this->connectedCmd = new ConnectedWrapper( *response );
        this->connected = true;

        // Done with this, we have a copy of its internals.
        delete response;

        readyCountDownLatch.countDown();
        return;
    }

    // Send along to the next interested party.
    fire( command );
}

////////////////////////////////////////////////////////////////////////////////
void StompConnectionNegotiator::onTransportException(
    Transport* source AMQCPP_UNUSED,
    const decaf::lang::Exception& ex ) {

    readyCountDownLatch.countDown();
    fire( ex );
}

////////////////////////////////////////////////////////////////////////////////
void StompConnectionNegotiator::start() throw( cms::CMSException ){

    /**
     * We're already started.
     */
    if( !closed ){
        return;
    }

    if( commandlistener == NULL ){
        throw exceptions::ActiveMQException(
            __FILE__, __LINE__,
            "StompConnectionNegotiator::start - "
            "commandListener is invalid" );
    }

    if( exceptionListener == NULL ){
        throw exceptions::ActiveMQException(
            __FILE__, __LINE__,
            "StompConnectionNegotiator::start - "
            "exceptionListener is invalid" );
    }

    if( next == NULL ){
        throw exceptions::ActiveMQException(
            __FILE__, __LINE__,
            "StompConnectionNegotiator::start - "
            "next transport is NULL" );
    }

    try{

        // Start the delegate transport object.
        next->start();

        // Mark it as open.
        closed = false;
        connected = false;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void StompConnectionNegotiator::close() throw( cms::CMSException ){

    try{

        if( !closed && next != NULL ){
            next->close();
        }

        closed = true;
        connected = false;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}
