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

#include "LoggingTransport.h"

using namespace std;
using namespace activemq;
using namespace activemq::exceptions;
using namespace activemq::transport;
using namespace activemq::transport::logging;
using namespace decaf::io;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
LOGDECAF_INITIALIZE( logger, LoggingTransport, "activemq.transport.logging.LoggingTransport")

////////////////////////////////////////////////////////////////////////////////
LoggingTransport::LoggingTransport( const Pointer<Transport>& next )
 :  TransportFilter( next )
{}

////////////////////////////////////////////////////////////////////////////////
void LoggingTransport::onCommand( const Pointer<Command>& command ) {

    ostringstream ostream;
    ostream << "*** BEGIN RECEIVED ASYNCHRONOUS COMMAND ***" << endl;
    ostream << command->toString() << endl;
    ostream << "*** END RECEIVED ASYNCHRONOUS COMMAND ***";

    LOGDECAF_INFO( logger, ostream.str() );

    // Delegate to the base class.
    TransportFilter::onCommand( command );
}

////////////////////////////////////////////////////////////////////////////////
void LoggingTransport::oneway( const Pointer<Command>& command )
    throw(IOException, decaf::lang::exceptions::UnsupportedOperationException) {

    try {

        ostringstream ostream;
        ostream << "*** BEGIN SENDING ONEWAY COMMAND ***" << endl;
        ostream << command->toString() << endl;
        ostream << "*** END SENDING ONEWAY COMMAND ***";

        LOGDECAF_INFO( logger, ostream.str() );

        // Delegate to the base class.
        TransportFilter::oneway( command );
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCH_RETHROW( UnsupportedOperationException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Response> LoggingTransport::request( const Pointer<Command>& command )
    throw(IOException, decaf::lang::exceptions::UnsupportedOperationException) {

    try {

        // Delegate to the base class.
        Pointer<Response> response = TransportFilter::request( command );

        ostringstream ostream;
        ostream << "*** SENDING REQUEST COMMAND ***" << endl;
        ostream << command->toString() << endl;
        ostream << "*** RECEIVED RESPONSE COMMAND ***" << endl;
        ostream << ( response == NULL? "NULL" : response->toString() );

        LOGDECAF_INFO( logger, ostream.str() );

        return response;
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCH_RETHROW( UnsupportedOperationException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Response> LoggingTransport::request( const Pointer<Command>& command, unsigned int timeout )
    throw(IOException, decaf::lang::exceptions::UnsupportedOperationException) {

    try {

        // Delegate to the base class.
        Pointer<Response> response = TransportFilter::request( command, timeout );

        ostringstream ostream;
        ostream << "*** SENDING REQUEST COMMAND: Timeout = " << timeout << " ***" << endl;
        ostream << command->toString() << endl;
        ostream << "*** RECEIVED RESPONSE COMMAND ***" << endl;
        ostream << ( response == NULL? "NULL" : response->toString() );

        LOGDECAF_INFO( logger, ostream.str() );

        return response;
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCH_RETHROW( UnsupportedOperationException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    AMQ_CATCHALL_THROW( IOException )
}
