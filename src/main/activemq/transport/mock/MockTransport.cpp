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

#include <activemq/transport/mock/MockTransport.h>
#include <activemq/exceptions/ActiveMQException.h>

using namespace activemq;
using namespace activemq::transport;
using namespace activemq::transport::mock;
using namespace activemq::exceptions;
using namespace activemq::wireformat;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
MockTransport* MockTransport::instance = NULL;

////////////////////////////////////////////////////////////////////////////////
MockTransport::MockTransport( WireFormat* wireFormat, ResponseBuilder* responseBuilder, bool own ){

    this->wireFormat = wireFormat;
    this->responseBuilder = NULL;
    this->outgoingListener = NULL;
    this->listener = NULL;
    this->responseBuilder = responseBuilder;
    this->own = own;
    this->nextCommandId.set( 0 );
    this->instance = this;

    // Configure the Internal Listener this is the Fake Broker.
    this->internalListener.setTransport( this );
    this->internalListener.setResponseBuilder( responseBuilder );
}

////////////////////////////////////////////////////////////////////////////////
MockTransport::~MockTransport(){
    try{

        if( this->own ){
            delete this->responseBuilder;
        }

        delete this->wireFormat;
    }
    AMQ_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void MockTransport::oneway( commands::Command* command )
        throw( CommandIOException,
               decaf::lang::exceptions::UnsupportedOperationException) {

    try{

        // Process and send any new Commands back.
        internalListener.onCommand( command );

        // Notify external Client of command that we "sent"
        if( outgoingListener != NULL ){
            outgoingListener->onCommand( command );
            return;
        }
    }
    AMQ_CATCH_RETHROW( CommandIOException )
    AMQ_CATCH_RETHROW( UnsupportedOperationException )
    AMQ_CATCH_EXCEPTION_CONVERT( ActiveMQException, CommandIOException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, CommandIOException )
    AMQ_CATCHALL_THROW( CommandIOException )
}

////////////////////////////////////////////////////////////////////////////////
commands::Response* MockTransport::request( commands::Command* command )
    throw( CommandIOException,
           decaf::lang::exceptions::UnsupportedOperationException)
{
    try{

        if( responseBuilder != NULL ){

            // Notify external Client of command that we "sent"
            if( outgoingListener != NULL ){
                outgoingListener->onCommand( command );
            }

            command->setCommandId( this->nextCommandId.incrementAndGet() );
            command->setResponseRequired( true );
            return responseBuilder->buildResponse( command );
        }

        throw CommandIOException(
            __FILE__, __LINE__,
            "MockTransport::request - no response builder available" );
    }
    AMQ_CATCH_RETHROW( CommandIOException )
    AMQ_CATCH_RETHROW( UnsupportedOperationException )
    AMQ_CATCH_EXCEPTION_CONVERT( ActiveMQException, CommandIOException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, CommandIOException )
    AMQ_CATCHALL_THROW( CommandIOException )
}

////////////////////////////////////////////////////////////////////////////////
commands::Response* MockTransport::request( commands::Command* command,
                                            unsigned int timeout AMQCPP_UNUSED )
    throw( CommandIOException,
           decaf::lang::exceptions::UnsupportedOperationException)
{
    try{
        return this->request( command );
    }
    AMQ_CATCH_RETHROW( CommandIOException )
    AMQ_CATCH_RETHROW( UnsupportedOperationException )
    AMQ_CATCH_EXCEPTION_CONVERT( ActiveMQException, CommandIOException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, CommandIOException )
    AMQ_CATCHALL_THROW( CommandIOException )
}
