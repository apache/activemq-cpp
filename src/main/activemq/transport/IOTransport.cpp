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
#include "CommandReader.h"
#include "CommandWriter.h"

#include <decaf/util/concurrent/Concurrent.h>
#include <decaf/lang/exceptions/UnsupportedOperationException.h>
#include <activemq/exceptions/ActiveMQException.h>
#include <activemq/util/Config.h>

using namespace activemq;
using namespace activemq::transport;
using namespace activemq::exceptions;
using namespace decaf::lang;
using namespace decaf::util::concurrent;

////////////////////////////////////////////////////////////////////////////////
LOGDECAF_INITIALIZE( logger, IOTransport, "activemq.transport.IOTransport" )

////////////////////////////////////////////////////////////////////////////////
IOTransport::IOTransport(){

    listener = NULL;
    reader = NULL;
    writer = NULL;
    exceptionListener = NULL;
    inputStream = NULL;
    outputStream = NULL;
    closed = false;
    thread = NULL;
}

////////////////////////////////////////////////////////////////////////////////
IOTransport::~IOTransport(){
    try{
        close();
    }
    AMQ_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void IOTransport::oneway( Command* command )
    throw( CommandIOException, decaf::lang::exceptions::UnsupportedOperationException ) {

    try{

        if( closed ){
            throw CommandIOException( __FILE__, __LINE__,
                "IOTransport::oneway() - transport is closed!" );
        }

        // Make sure the thread has been started.
        if( thread == NULL ){
            throw CommandIOException(
                __FILE__, __LINE__,
                "IOTransport::oneway() - transport is not started" );
        }

        // Make sure the command object is valid.
        if( command == NULL ){
            throw CommandIOException(
                __FILE__, __LINE__,
                "IOTransport::oneway() - attempting to write NULL command" );
        }

        // Make sure we have an output strema to write to.
        if( outputStream == NULL ){
            throw CommandIOException(
                __FILE__, __LINE__,
                "IOTransport::oneway() - invalid output stream" );
        }

        synchronized( outputStream ){
            // Write the command to the output stream.
            writer->writeCommand( command );
        }
    }
    AMQ_CATCH_RETHROW( CommandIOException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, CommandIOException )
    AMQ_CATCHALL_THROW( CommandIOException )
}

////////////////////////////////////////////////////////////////////////////////
void IOTransport::start() throw( cms::CMSException ){

    try{

        // Can't restart a closed transport.
        if( closed ){
            throw ActiveMQException(
                __FILE__, __LINE__,
                "IOTransport::start() - transport is already closed - cannot restart" );
        }

        // If it's already started, do nothing.
        if( thread != NULL ){
            return;
        }

        // Make sure all variables that we need have been set.
        if( inputStream == NULL || outputStream == NULL ||
            reader == NULL || writer == NULL ){
            throw ActiveMQException(
                __FILE__, __LINE__,
                "IOTransport::start() - "
                "IO sreams and reader/writer must be set before calling start" );
        }

        // Init the Command Reader and Writer with the Streams
        reader->setInputStream( inputStream );
        writer->setOutputStream( outputStream );

        // Start the polling thread.
        thread = new Thread( this );
        thread->start();
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void IOTransport::close() throw( cms::CMSException ){

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
            delete thread;
            thread = NULL;
        }

        // Close the output stream.
        if( outputStream != NULL ){

            outputStream->close();
            outputStream = NULL;
        }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void IOTransport::run(){

    try{

        while( !closed ){

            // Read the next command from the input stream.
            Command* command = reader->readCommand();

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
Response* IOTransport::request( Command* command AMQCPP_UNUSED )
    throw( CommandIOException, decaf::lang::exceptions::UnsupportedOperationException ){

    throw decaf::lang::exceptions::UnsupportedOperationException(
        __FILE__, __LINE__,
        "IOTransport::request() - unsupported operation" );
}

////////////////////////////////////////////////////////////////////////////////
Response* IOTransport::request( Command* command AMQCPP_UNUSED, unsigned int timeout AMQCPP_UNUSED )
    throw( CommandIOException, decaf::lang::exceptions::UnsupportedOperationException ){

    throw decaf::lang::exceptions::UnsupportedOperationException(
        __FILE__, __LINE__,
        "IOTransport::request() - unsupported operation" );
}
