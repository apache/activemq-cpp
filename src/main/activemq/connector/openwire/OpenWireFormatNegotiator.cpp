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

#include <activemq/connector/openwire/commands/DataStructure.h>
#include <activemq/connector/openwire/commands/WireFormatInfo.h>
#include <activemq/transport/IOTransport.h>

using namespace std;
using namespace activemq;
using namespace activemq::exceptions;
using namespace activemq::transport;
using namespace activemq::connector;
using namespace activemq::connector::openwire;
using namespace activemq::connector::openwire::commands;
using namespace decaf::util::concurrent;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
OpenWireFormatNegotiator::OpenWireFormatNegotiator( OpenWireFormat* openWireFormat,
                                                    Transport* next,
                                                    bool own ) :
    TransportFilter( next, own ),
    wireInfoSentDownLatch(1),
    readyCountDownLatch(1)
{
    this->firstTime.set( true );
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
    throw( CommandIOException, UnsupportedOperationException ) {

    try{

        if( closed || next == NULL ){
            throw CommandIOException(
                __FILE__, __LINE__,
                "OpenWireFormatNegotiator::oneway - transport already closed" );
        }

        if( !readyCountDownLatch.await( negotiationTimeout ) ) {
            throw CommandIOException(
                __FILE__,
                __LINE__,
                "OpenWireFormatNegotiator::oneway"
                "Wire format negotiation timeout: peer did not "
                "send his wire format." );
        }

        next->oneway( command );
    }
    AMQ_CATCH_RETHROW( UnsupportedOperationException )
    AMQ_CATCH_RETHROW( CommandIOException )
    AMQ_CATCH_EXCEPTION_CONVERT( exceptions::ActiveMQException, CommandIOException )
    AMQ_CATCHALL_THROW( CommandIOException )
}

////////////////////////////////////////////////////////////////////////////////
Response* OpenWireFormatNegotiator::request( Command* command )
    throw( CommandIOException, UnsupportedOperationException ) {

    try{

        if( closed || next == NULL ){
            throw CommandIOException(
                __FILE__, __LINE__,
                "OpenWireFormatNegotiator::request - transport already closed" );
        }

        if( !readyCountDownLatch.await( negotiationTimeout ) ) {
            throw CommandIOException(
                __FILE__,
                __LINE__,
                "OpenWireFormatNegotiator::request"
                "Wire format negotiation timeout: peer did not "
                "send his wire format." );
        }

        return next->request( command );
    }
    AMQ_CATCH_RETHROW( UnsupportedOperationException )
    AMQ_CATCH_RETHROW( CommandIOException )
    AMQ_CATCH_EXCEPTION_CONVERT( exceptions::ActiveMQException, CommandIOException )
    AMQ_CATCHALL_THROW( CommandIOException )
}

////////////////////////////////////////////////////////////////////////////////
Response* OpenWireFormatNegotiator::request( Command* command, unsigned int timeout )
    throw( CommandIOException, UnsupportedOperationException ) {

    try{

        if( closed || next == NULL ){
            throw CommandIOException(
                __FILE__, __LINE__,
                "OpenWireFormatNegotiator::request - transport already closed" );
        }

        if( !readyCountDownLatch.await( negotiationTimeout ) ) {
            throw CommandIOException(
                __FILE__,
                __LINE__,
                "OpenWireFormatNegotiator::request"
                "Wire format negotiation timeout: peer did not "
                "send his wire format." );
        }

        return next->request( command, timeout );
    }
    AMQ_CATCH_RETHROW( UnsupportedOperationException )
    AMQ_CATCH_RETHROW( CommandIOException )
    AMQ_CATCH_EXCEPTION_CONVERT( exceptions::ActiveMQException, CommandIOException )
    AMQ_CATCHALL_THROW( CommandIOException )
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireFormatNegotiator::onCommand( Command* command ) {

    DataStructure* dataStructure =
        dynamic_cast<DataStructure*>( command );

    if( dataStructure != NULL &&
        dataStructure->getDataStructureType() == WireFormatInfo::ID_WIREFORMATINFO ) {

        WireFormatInfo* info = dynamic_cast<WireFormatInfo*>( dataStructure );

        try {

            if( !info->isValid() ) {
                throw CommandIOException(
                    __FILE__,
                    __LINE__,
                    "OpenWireFormatNegotiator::onCommand"
                    "Remote wire format magic is invalid" );
            }

            wireInfoSentDownLatch.await( negotiationTimeout );
            openWireFormat->renegotiateWireFormat( info );

            readyCountDownLatch.countDown();

        } catch( exceptions::ActiveMQException& ex ) {

            readyCountDownLatch.countDown();
            fire( ex );
        }
    }

    // Send along to the next interested party.
    fire( command );
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireFormatNegotiator::onTransportException(
    Transport* source AMQCPP_UNUSED,
    const decaf::lang::Exception& ex ) {

    readyCountDownLatch.countDown();
    fire( ex );
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
        throw exceptions::ActiveMQException(
            __FILE__, __LINE__,
            "OpenWireFormatNegotiator::start - "
            "commandListener is invalid" );
    }

    if( exceptionListener == NULL ){
        throw exceptions::ActiveMQException(
            __FILE__, __LINE__,
            "OpenWireFormatNegotiator::start - "
            "exceptionListener is invalid" );
    }

    if( next == NULL ){
        throw exceptions::ActiveMQException(
            __FILE__, __LINE__,
            "OpenWireFormatNegotiator::start - "
            "next transport is NULL" );
    }

    if( openWireFormat == NULL ){
        throw exceptions::ActiveMQException(
            __FILE__, __LINE__,
            "OpenWireFormatNegotiator::start - "
            "openWireFormat is NULL" );
    }

    // Start the delegate transport object.
    next->start();

    if( firstTime.compareAndSet( true, false ) ) {

        try {

            // Circumvent all other Transport filters and go straight for the base
            // IOTransport, this should guarantee that there's no funny business done
            // like async dispatch etc.  If it can't be found just use next and hope that
            // there's nothing that will break the necessary thread locking that protects
            // the message as it marshaled out to the wire
            Transport* transport = this->next->narrow( typeid( transport::IOTransport ) );
            if( transport == NULL ) {
                transport = this->next;
            }

            // We first send the WireFormat that we'd prefer.
            transport->oneway( openWireFormat->getPreferedWireFormatInfo() );

            // Mark the latch
            wireInfoSentDownLatch.countDown();

        } catch( ActiveMQException& ex ) {

            // Mark the latch
            wireInfoSentDownLatch.countDown();
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

