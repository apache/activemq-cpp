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

#include <activemq/transport/DummyTransport.h>
#include <activemq/support/LibraryInit.h>

using namespace activemq;
using namespace activemq::transport;
using namespace activemq::exceptions;

////////////////////////////////////////////////////////////////////////////////
DummyTransport* DummyTransport::instance = NULL;

////////////////////////////////////////////////////////////////////////////////
DummyTransport::DummyTransport( ResponseBuilder* responseBuilder ,
                                bool own,
                                bool useDefOutgoing ){

    this->responseBuilder = NULL;
    this->commandListener = NULL;
    this->outgoingCommandListener = NULL;
    this->exceptionListener = NULL;
    this->responseBuilder = responseBuilder;
    this->own = own;
    this->nextCommandId = 0;
    this->instance = this;
    if( useDefOutgoing )
    {
        this->outgoingCommandListener = &defaultListener;
        this->defaultListener.setTransport( this );
        this->defaultListener.setResponseBuilder( responseBuilder );
    }
}

////////////////////////////////////////////////////////////////////////////////
DummyTransport::~DummyTransport(){

    if( own ){
        delete responseBuilder;
    }
}

////////////////////////////////////////////////////////////////////////////////
unsigned int DummyTransport::getNextCommandId() throw ( exceptions::ActiveMQException ) {

    try{
        synchronized( &commandIdMutex ){
            return ++nextCommandId;
        }

        // Should never get here, but some compilers aren't
        // smart enough to figure out we'll never get here.
        return 0;
    }
    AMQ_CATCHALL_THROW( transport::CommandIOException )
}

////////////////////////////////////////////////////////////////////////////////
void DummyTransport::oneway( Command* command )
        throw(CommandIOException, exceptions::UnsupportedOperationException)
{
    if( outgoingCommandListener != NULL ){

        //command->setCommandId( getNextCommandId() );
        //command->setResponseRequired( false );
        outgoingCommandListener->onCommand( command );
        return;
    }
}

////////////////////////////////////////////////////////////////////////////////
Response* DummyTransport::request( Command* command )
    throw(CommandIOException,
          exceptions::UnsupportedOperationException)
{
    if( responseBuilder != NULL ){
        command->setCommandId( getNextCommandId() );
        command->setResponseRequired( true );
        return responseBuilder->buildResponse( command );
    }

    throw CommandIOException( __FILE__, __LINE__,
        "no response builder available" );
}
