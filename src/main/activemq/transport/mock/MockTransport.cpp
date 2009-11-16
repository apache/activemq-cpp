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
using namespace decaf::io;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
MockTransport* MockTransport::instance = NULL;

////////////////////////////////////////////////////////////////////////////////
MockTransport::MockTransport( const Pointer<WireFormat>& wireFormat,
                              const Pointer<ResponseBuilder>& responseBuilder ){

    this->wireFormat = wireFormat;
    this->outgoingListener = NULL;
    this->listener = NULL;
    this->responseBuilder = responseBuilder;
    this->nextCommandId.set( 0 );
    this->instance = this;

    // Script Properties.
    this->failOnSendMessage = false;
    this->numSentMessageBeforeFail = 0;
    this->numSentMessages = 0;
    this->failOnReceiveMessage = false;
    this->numReceivedMessageBeforeFail = 0;
    this->numReceivedMessages = 0;
    this->failOnKeepAliveSends = false;
    this->numSentKeepAlivesBeforeFail = 0;
    this->numSentKeepAlives = 0;
    this->failOnStart = false;
    this->failOnStop = false;
    this->failOnClose = false;

    // Configure the Internal Listener this is the Fake Broker.
    this->internalListener.setTransport( this );
    this->internalListener.setResponseBuilder( responseBuilder );
}

////////////////////////////////////////////////////////////////////////////////
void MockTransport::oneway( const Pointer<Command>& command )
        throw( IOException,
               decaf::lang::exceptions::UnsupportedOperationException) {

    try{

        if( command->isMessage() && this->failOnSendMessage ) {
            this->numSentMessages++;

            if( this->numSentMessages > this->numSentMessageBeforeFail ) {
                throw IOException(
                    __FILE__, __LINE__, "Failed to Send Message.");
            }
        }

        if( command->isKeepAliveInfo() && this->failOnKeepAliveSends ) {
            this->numSentKeepAlives++;

            if( this->numSentKeepAlives > this->numSentKeepAlivesBeforeFail ) {
                throw IOException(
                    __FILE__, __LINE__, "Failed to Send Message.");
            }
        }

        // Process and send any new Commands back.
        internalListener.onCommand( command );

        // Notify external Client of command that we "sent"
        if( outgoingListener != NULL ){
            outgoingListener->onCommand( command );
            return;
        }
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCH_RETHROW( UnsupportedOperationException )
    AMQ_CATCH_EXCEPTION_CONVERT( ActiveMQException, IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Response> MockTransport::request( const Pointer<Command>& command )
    throw( IOException,
           decaf::lang::exceptions::UnsupportedOperationException)
{
    try{

        if( responseBuilder != NULL ){

            if( command->isMessage() && this->failOnSendMessage ) {
                this->numSentMessages++;

                if( this->numSentMessages > this->numSentMessageBeforeFail ) {
                    throw IOException(
                        __FILE__, __LINE__, "Failed to Send Message.");
                }
            }

            // Notify external Client of command that we "sent"
            if( outgoingListener != NULL ){
                outgoingListener->onCommand( command );
            }

            command->setCommandId( this->nextCommandId.incrementAndGet() );
            command->setResponseRequired( true );
            return responseBuilder->buildResponse( command );
        }

        throw IOException(
            __FILE__, __LINE__,
            "MockTransport::request - no response builder available" );
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCH_RETHROW( UnsupportedOperationException )
    AMQ_CATCH_EXCEPTION_CONVERT( ActiveMQException, IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Response> MockTransport::request( const Pointer<Command>& command,
                                          unsigned int timeout AMQCPP_UNUSED )
    throw( IOException,
           decaf::lang::exceptions::UnsupportedOperationException)
{
    try{
        return this->request( command );
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCH_RETHROW( UnsupportedOperationException )
    AMQ_CATCH_EXCEPTION_CONVERT( ActiveMQException, IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void MockTransport::start() throw( decaf::io::IOException ) {

    if( this->failOnStart ) {
        throw IOException( __FILE__, __LINE__, "Failed to Start MockTransport." );
    }
}

////////////////////////////////////////////////////////////////////////////////
void MockTransport::stop() throw( decaf::io::IOException ) {

    if( this->failOnStop ) {
        throw IOException( __FILE__, __LINE__, "Failed to Stop MockTransport." );
    }
}

////////////////////////////////////////////////////////////////////////////////
void MockTransport::close() throw( decaf::io::IOException ) {

    if( this->failOnClose ) {
        throw IOException( __FILE__, __LINE__, "Failed to Close MockTransport." );
    }
}
