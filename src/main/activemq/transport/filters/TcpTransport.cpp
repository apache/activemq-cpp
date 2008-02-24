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

#include "TcpTransport.h"

#include <decaf/net/SocketFactory.h>
#include <activemq/transport/IOTransport.h>
#include <activemq/transport/TransportFactory.h>
#include <activemq/transport/TransportFactoryMap.h>

using namespace std;
using namespace activemq;
using namespace activemq::io;
using namespace activemq::transport;
using namespace activemq::transport::filters;
using namespace activemq::exceptions;
using namespace decaf::net;
using namespace decaf::io;
using namespace decaf::lang;

////////////////////////////////////////////////////////////////////////////////
TcpTransport::TcpTransport( const decaf::util::Properties& properties,
                            Transport* next,
                            const bool own )
:
    TransportFilter( next, own ),
    socket( NULL ),
    loggingInputStream( NULL ),
    loggingOutputStream( NULL ),
    bufferedInputStream( NULL ),
    bufferedOutputStream( NULL )
{
    try{

        if( !properties.hasProperty( "transport.uri" ) ) {
            throw ActiveMQException(
                __FILE__, __LINE__,
                "TcpTransport::TcpTransport - "
                "No URI set for this transport to connect to.");
        }

        // Create the IO device we will be communicating over the
        // wire with.  This may need to change if we add more types
        // of sockets, such as SSL.
        socket = SocketFactory::createSocket(
            properties.getProperty( "transport.uri" ), properties );

        // Cast it to an IO transport so we can wire up the socket
        // input and output streams.
        IOTransport* ioTransport = dynamic_cast<IOTransport*>( next );
        if( ioTransport == NULL ){
            throw ActiveMQException(
                __FILE__, __LINE__,
                "TcpTransport::TcpTransport - "
                "transport must be of type IOTransport");
        }

        InputStream* inputStream = socket->getInputStream();
        OutputStream* outputStream = socket->getOutputStream();

        // If tcp tracing was enabled, wrap the iostreams with logging streams
        if( properties.getProperty( "transport.tcpTracingEnabled", "false" ) == "true" ) {
            loggingInputStream = new LoggingInputStream( inputStream );
            loggingOutputStream = new LoggingOutputStream( outputStream );

            inputStream = loggingInputStream;
            outputStream = loggingOutputStream;
        }

        // Now wrap the input/output streams with buffered streams
        bufferedInputStream = new BufferedInputStream(inputStream);
        bufferedOutputStream = new BufferedOutputStream(outputStream);

        // Give the IOTransport the streams.
        ioTransport->setInputStream( bufferedInputStream );
        ioTransport->setOutputStream( bufferedOutputStream );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
TcpTransport::~TcpTransport() {

    try{

        try{
            close();
        } catch( cms::CMSException& ex ){ /* Absorb */ }

        if( socket != NULL ) {
            delete socket;
            socket = NULL;
        }

        if( loggingInputStream != NULL ) {
            delete loggingInputStream;
            loggingInputStream = NULL;
        }

        if( loggingOutputStream != NULL ) {
            delete loggingOutputStream;
            loggingOutputStream = NULL;
        }

        if( bufferedInputStream != NULL ) {
            delete bufferedInputStream;
            bufferedInputStream = NULL;
        }

        if( bufferedOutputStream != NULL ) {
            delete bufferedOutputStream;
            bufferedOutputStream = NULL;
        }
    }
    AMQ_CATCH_NOTHROW( ActiveMQException )
    AMQ_CATCH_NOTHROW( Exception )
    AMQ_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void TcpTransport::close() throw( cms::CMSException ) {

    try
    {
        // Close the socket.
        if( socket != NULL ) {
            socket->close();
        }

        // Invoke the paren't close first.
        TransportFilter::close();
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}
