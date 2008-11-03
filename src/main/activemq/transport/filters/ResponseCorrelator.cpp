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

#include "ResponseCorrelator.h"

using namespace activemq;
using namespace activemq::transport;
using namespace activemq::transport::filters;
using namespace activemq::exceptions;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
ResponseCorrelator::ResponseCorrelator( Transport* next, bool own )
 :  TransportFilter( next, own ) {

    nextCommandId.set(1);
    // Start in the closed state.
    closed = true;
}

////////////////////////////////////////////////////////////////////////////////
ResponseCorrelator::~ResponseCorrelator(){

    // Close the transport and destroy it.
    close();

    // Don't do anything with the future responses -
    // they should be cleaned up by each requester.
}

////////////////////////////////////////////////////////////////////////////////
void ResponseCorrelator::oneway( Command* command )
    throw( CommandIOException, decaf::lang::exceptions::UnsupportedOperationException ) {

    try{
        command->setCommandId( nextCommandId.getAndIncrement() );
        command->setResponseRequired( false );

        if( closed || next == NULL ){
            throw CommandIOException( __FILE__, __LINE__,
                "transport already closed" );
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
Response* ResponseCorrelator::request( Command* command )
    throw( CommandIOException, decaf::lang::exceptions::UnsupportedOperationException ) {

    try{
        command->setCommandId( nextCommandId.getAndIncrement() );
        command->setResponseRequired( true );

        // Add a future response object to the map indexed by this
        // command id.
        // TODO = This might not get deleted if an exception is thrown.
        FutureResponse* futureResponse = new FutureResponse();

        synchronized( &mapMutex ){
            requestMap[command->getCommandId()] = futureResponse;
        }

        // Wait to be notified of the response via the futureResponse
        // object.
        Response* response = NULL;

        // Send the request.
        next->oneway( command );

        // Get the response.
        response = futureResponse->getResponse();

        // Perform cleanup on the map.
        synchronized( &mapMutex ){

            // We've done our waiting - get this thing out
            // of the map.
            requestMap.erase( command->getCommandId() );

            // Destroy the futureResponse.  It is safe to
            // do this now because the other thread only
            // accesses the futureResponse within a lock on
            // the map.
            delete futureResponse;
            futureResponse = NULL;
        }

        if( response == NULL ){

            throw CommandIOException( __FILE__, __LINE__,
                "No valid response received for command: %s, check broker.",
                command->toString().c_str() );
        }

        return response;
    }
    AMQ_CATCH_RETHROW( UnsupportedOperationException )
    AMQ_CATCH_RETHROW( CommandIOException )
    AMQ_CATCH_EXCEPTION_CONVERT( ActiveMQException, CommandIOException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, CommandIOException )
    AMQ_CATCHALL_THROW( CommandIOException )
}

////////////////////////////////////////////////////////////////////////////////
Response* ResponseCorrelator::request( Command* command, unsigned int timeout )
    throw( CommandIOException, decaf::lang::exceptions::UnsupportedOperationException ) {

    try{
        command->setCommandId( nextCommandId.getAndIncrement() );
        command->setResponseRequired( true );

        // Add a future response object to the map indexed by this
        // command id.
        // TODO = This might not get deleted if an exception is thrown.
        FutureResponse* futureResponse = new FutureResponse();

        synchronized( &mapMutex ){
            requestMap[command->getCommandId()] = futureResponse;
        }

        // Wait to be notified of the response via the futureResponse
        // object.
        Response* response = NULL;

        // Send the request.
        next->oneway( command );

        // Get the response.
        response = futureResponse->getResponse( timeout );

        // Perform cleanup on the map.
        synchronized( &mapMutex ){

            // We've done our waiting - get this thing out
            // of the map.
            requestMap.erase( command->getCommandId() );

            // Destroy the futureResponse.  It is safe to
            // do this now because the other thread only
            // accesses the futureResponse within a lock on
            // the map.
            delete futureResponse;
            futureResponse = NULL;
        }

        if( response == NULL ){

            throw CommandIOException( __FILE__, __LINE__,
                "No valid response received for command: %s, check broker.",
                command->toString().c_str() );
        }

        return response;
    }
    AMQ_CATCH_RETHROW( UnsupportedOperationException )
    AMQ_CATCH_RETHROW( CommandIOException )
    AMQ_CATCH_EXCEPTION_CONVERT( ActiveMQException, CommandIOException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, CommandIOException )
    AMQ_CATCHALL_THROW( CommandIOException )
}

////////////////////////////////////////////////////////////////////////////////
void ResponseCorrelator::onCommand( Command* command ) {

    // Let's see if the incoming command is a response.
    Response* response = dynamic_cast<Response*>( command );

    if( response == NULL ){

        // It's a non-response - just notify the listener.
        fire( command );
        return;
    }

    // It is a response - let's correlate ...
    synchronized( &mapMutex ){

        // Look the future request up based on the correlation id.
        std::map<unsigned int, FutureResponse*>::iterator iter =
            requestMap.find( response->getCorrelationId() );
        if( iter == requestMap.end() ){

            // This is not terrible - just log it.
            //printf("ResponseCorrelator::onCommand() - received unknown response for request: %d\n",
            //    response->getCorrelationId() );
            return;
        }

        // Get the future response (if it's in the map, it's not NULL).
        FutureResponse* futureResponse = NULL;
        futureResponse = iter->second;

        // Set the response property in the future response.
        futureResponse->setResponse( response );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ResponseCorrelator::start() throw( cms::CMSException ) {

    try{

        /**
         * We're already started.
         */
        if( !closed ){
            return;
        }

        if( commandlistener == NULL ){
            throw exceptions::ActiveMQException( __FILE__, __LINE__,
                "commandListener is invalid" );
        }

        if( exceptionListener == NULL ){
            throw exceptions::ActiveMQException( __FILE__, __LINE__,
                "exceptionListener is invalid" );
        }

        if( next == NULL ){
            throw exceptions::ActiveMQException( __FILE__, __LINE__,
                "next transport is NULL" );
        }

        // Start the delegate transport object.
        next->start();

        // Mark it as open.
        closed = false;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ResponseCorrelator::close() throw( cms::CMSException ){

    try{

        // Wake-up any outstanding requests.
        synchronized( &mapMutex ){
            std::map<unsigned int, FutureResponse*>::iterator iter = requestMap.begin();
            for( ; iter != requestMap.end(); ++iter ){
                iter->second->setResponse( NULL );
            }
        }

        if( !closed && next != NULL ){
            next->close();
        }

        closed = true;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ResponseCorrelator::onTransportException(
    Transport* source AMQCPP_UNUSED,
    const decaf::lang::Exception& ex ) {

    // Trigger each outstanding request to complete so that we don't hang
    // forever waiting for one that has been sent without timeout.
    synchronized( &mapMutex ){
        std::map<unsigned int, FutureResponse*>::iterator iter = requestMap.begin();
        for( ; iter != requestMap.end(); ++iter ){
            iter->second->setResponse( NULL );
        }
    }

    fire( ex );
}
