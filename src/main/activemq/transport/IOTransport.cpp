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

#include "IOTransport.h"

#include <decaf/util/concurrent/Concurrent.h>
#include <decaf/lang/exceptions/UnsupportedOperationException.h>
#include <activemq/wireformat/WireFormat.h>
#include <activemq/exceptions/ActiveMQException.h>
#include <activemq/util/Config.h>
#include <typeinfo>

using namespace activemq;
using namespace activemq::transport;
using namespace activemq::exceptions;
using namespace activemq::commands;
using namespace activemq::wireformat;
using namespace decaf;
using namespace decaf::io;
using namespace decaf::lang;
using namespace decaf::util::concurrent;

////////////////////////////////////////////////////////////////////////////////
LOGDECAF_INITIALIZE( logger, IOTransport, "activemq.transport.IOTransport" )

////////////////////////////////////////////////////////////////////////////////
IOTransport::IOTransport() : wireFormat(),
                             listener(NULL),
                             inputStream(NULL),
                             outputStream(NULL),
                             thread(),
                             closed(false) {
}

////////////////////////////////////////////////////////////////////////////////
IOTransport::IOTransport( const Pointer<WireFormat>& wireFormat ) : wireFormat(wireFormat),
                                                                    listener(NULL),
                                                                    inputStream(NULL),
                                                                    outputStream(NULL),
                                                                    thread(),
                                                                    closed(false) {
}

////////////////////////////////////////////////////////////////////////////////
IOTransport::~IOTransport(){
    try{
        close();
    }
    AMQ_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void IOTransport::fire( decaf::lang::Exception& ex ){

    if( this->listener != NULL && !this->closed ){

        try{
            this->listener->onException( ex );
        }catch( ... ){}
    }
}

////////////////////////////////////////////////////////////////////////////////
void IOTransport::fire( const Pointer<Command>& command ){

    try{
        // Since the listener is responsible for freeing the memory,
        // if there is no listener - free the command here.  Also if
        // we have been closed then we don't deliver any messages that
        // might have sneaked in while we where closing.
        if( this->listener == NULL || this->closed == true ){
            return;
        }

        this->listener->onCommand( command );

    }
    AMQ_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void IOTransport::oneway( const Pointer<Command>& command ) {

    try{

        if( closed ){
            throw IOException( __FILE__, __LINE__,
                "IOTransport::oneway() - transport is closed!" );
        }

        // Make sure the thread has been started.
        if( thread == NULL ){
            throw IOException(
                __FILE__, __LINE__,
                "IOTransport::oneway() - transport is not started" );
        }

        // Make sure the command object is valid.
        if( command == NULL ){
            throw IOException(
                __FILE__, __LINE__,
                "IOTransport::oneway() - attempting to write NULL command" );
        }

        // Make sure we have an output stream to write to.
        if( outputStream == NULL ){
            throw IOException(
                __FILE__, __LINE__,
                "IOTransport::oneway() - invalid output stream" );
        }

        synchronized( outputStream ){
            // Write the command to the output stream.
            this->wireFormat->marshal( command, this, this->outputStream );
            this->outputStream->flush();
        }
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void IOTransport::start() {

    try{

        // Can't restart a closed transport.
        if( closed ){
            throw IOException(
                __FILE__, __LINE__,
                "IOTransport::start() - transport is already closed - cannot restart" );
        }

        // If it's already started, do nothing.
        if( thread != NULL ){
            return;
        }

        // Make sure all variables that we need have been set.
        if( inputStream == NULL || outputStream == NULL || wireFormat.get() == NULL ){
            throw IOException(
                __FILE__, __LINE__,
                "IOTransport::start() - "
                "IO streams and wireFormat instances must be set before calling start" );
        }

        // Start the polling thread.
        thread.reset( new Thread( this ) );
        thread->start();
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void IOTransport::stop() {

    try{
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void IOTransport::close() {

    try{

        if( closed ){
            return;
        }

        // Mark this transport as closed.
        closed = true;

        // We have to close the input stream before
        // we stop the thread.  this will force us to
        // wake up the thread if it's stuck in a read
        // (which is likely).  Otherwise, the join that
        // follows will block forever.
        if( inputStream != NULL ){
            inputStream->close();
            inputStream = NULL;
        }

        // Wait for the thread to die.
        if( thread != NULL ){
            thread->join();
            thread.reset( NULL );
        }

        // Close the output stream.
        if( outputStream != NULL ){
            outputStream->close();
            outputStream = NULL;
        }

        // Clear the WireFormat so we can't use it anymore
        this->wireFormat.reset( NULL );
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void IOTransport::run() {

    try{

        while( !closed ){

            // Read the next command from the input stream.
            Pointer<Command> command( wireFormat->unmarshal( this, this->inputStream ) );

            // Notify the listener.
            fire( command );
        }
    }
    catch( exceptions::ActiveMQException& ex ){
        ex.setMark( __FILE__, __LINE__ );
        fire( ex );
    }
    catch( decaf::lang::Exception& ex ){
        exceptions::ActiveMQException exl( ex );
        exl.setMark( __FILE__, __LINE__ );
        fire( exl );
    }
    catch( ... ){

        exceptions::ActiveMQException ex(
            __FILE__, __LINE__,
            "IOTransport::run - caught unknown exception" );

        LOGDECAF_WARN(logger, ex.getStackTraceString() );

        fire( ex );
    }
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Response> IOTransport::request( const Pointer<Command>& command AMQCPP_UNUSED ) {

    throw decaf::lang::exceptions::UnsupportedOperationException(
        __FILE__, __LINE__,
        "IOTransport::request() - unsupported operation" );
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Response> IOTransport::request( const Pointer<Command>& command AMQCPP_UNUSED, unsigned int timeout AMQCPP_UNUSED ) {

    throw decaf::lang::exceptions::UnsupportedOperationException(
        __FILE__, __LINE__,
        "IOTransport::request() - unsupported operation" );
}
