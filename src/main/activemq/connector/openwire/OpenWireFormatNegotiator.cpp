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

#include "OpenWireFormatNegotiator.h"

using namespace activemq;
using namespace activemq::exceptions;
using namespace activemq::transport;
using namespace activemq::connector;
using namespace activemq::connector::openwire;

////////////////////////////////////////////////////////////////////////////////
OpenWireFormatNegotiator::OpenWireFormatNegotiator( OpenWireFormat* openWireFormat,
                                                    Transport* next,
                                                    const bool own ) :
    TransportFilter( next, own )
{
    this->firstTime = true;
    this->openWireFormat = openWireFormat;
    this->closed = true;
}

////////////////////////////////////////////////////////////////////////////////
OpenWireFormatNegotiator::~OpenWireFormatNegotiator()
{
    // Close the transport and destroy it.
    close();
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireFormatNegotiator::oneway( Command* command )
    throw( CommandIOException, exceptions::UnsupportedOperationException )
{

    try{

        if( closed || next == NULL ){
            throw CommandIOException( __FILE__, __LINE__,
                "transport already closed" );
        }

        next->oneway( command );
    }
    AMQ_CATCH_RETHROW( exceptions::UnsupportedOperationException )
    AMQ_CATCH_RETHROW( CommandIOException )
    AMQ_CATCH_EXCEPTION_CONVERT( exceptions::ActiveMQException, CommandIOException )
    AMQ_CATCHALL_THROW( CommandIOException )
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireFormatNegotiator::onCommand( Command* command ){


}

////////////////////////////////////////////////////////////////////////////////
void OpenWireFormatNegotiator::start() throw( cms::CMSException ){

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

    if( firstTime = true ) {

        try {

            // The First Time is now over with
            firstTime = false;

            // We first send the WireFormat that we'd prefer.
            //next.Oneway( wireFormat->getPreferedWireFormatInfo() );

        } catch( ActiveMQException& ex ) {

            //wireInfoSentDownLatch.countDown();
            ex.setMark( __FILE__, __LINE__ );
            throw ex;
        }
    }

    // Mark it as open.
    closed = false;
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireFormatNegotiator::close() throw( cms::CMSException ){

    if( !closed && next != NULL ){
        next->close();
    }

    closed = true;
}

