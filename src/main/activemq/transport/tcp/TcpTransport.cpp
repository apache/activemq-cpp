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

#include <activemq/transport/IOTransport.h>
#include <activemq/transport/TransportFactory.h>

#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>
#include <decaf/lang/Integer.h>
#include <decaf/lang/Boolean.h>
#include <decaf/net/SocketFactory.h>

#include <memory>

using namespace std;
using namespace activemq;
using namespace activemq::io;
using namespace activemq::transport;
using namespace activemq::transport::tcp;
using namespace activemq::exceptions;
using namespace decaf;
using namespace decaf::net;
using namespace decaf::util;
using namespace decaf::io;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
TcpTransport::TcpTransport( const Pointer<Transport>& next ) :
    TransportFilter(next), connectTimeout(0), closed(false), socket(), dataInputStream(), dataOutputStream() {

}

////////////////////////////////////////////////////////////////////////////////
TcpTransport::~TcpTransport() {

    try {
        close();
    }
    AMQ_CATCH_NOTHROW( ActiveMQException )
    AMQ_CATCH_NOTHROW( Exception )
    AMQ_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void TcpTransport::close() {

    try {

        this->closed = true;

        // Close the socket.
        if( socket.get() != NULL ) {
            socket->close();
        }

        // Invoke the paren't close first.
        TransportFilter::close();
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void TcpTransport::connect( const decaf::net::URI& uri,
                            const decaf::util::Properties& properties ) {

    try {

        socket.reset( this->createSocket() );

        // Set all Socket Options from the URI options.
        this->configureSocket( socket.get(), properties );

        // Ensure something is actually passed in for the URI
        if( uri.getAuthority() == "" ) {
            throw SocketException( __FILE__, __LINE__,
                "Connection URI was not provided or is invalid: %s", uri.toString().c_str() );
        }

        // Connect the socket.
        string host = uri.getHost();
        int port = uri.getPort();

        socket->connect( host, port, connectTimeout );

        // Cast it to an IO transport so we can wire up the socket
        // input and output streams.
        IOTransport* ioTransport = dynamic_cast<IOTransport*>( next.get() );
        if( ioTransport == NULL ){
            throw ActiveMQException(
                __FILE__, __LINE__,
                "TcpTransport::TcpTransport - "
                "transport must be of type IOTransport");
        }

        // Get the read buffer size.
        int inputBufferSize = Integer::parseInt(
            properties.getProperty( "inputBufferSize", "8192" ) );

        // Get the write buffer size.
        int outputBufferSize = Integer::parseInt(
            properties.getProperty( "outputBufferSize", "8192" ) );

        InputStream* inputStream = socket->getInputStream();
        OutputStream* outputStream = socket->getOutputStream();

        // If tcp tracing was enabled, wrap the iostreams with logging streams
        if( properties.getProperty( "transport.tcpTracingEnabled", "false" ) == "true" ) {

            // Wrap with logging stream, we don't own the wrapped streams
            inputStream = new LoggingInputStream( inputStream );
            outputStream = new LoggingOutputStream( outputStream );

            // Now wrap with the Buffered streams, we own the source streams
            inputStream = new BufferedInputStream( inputStream, inputBufferSize, true );
            outputStream = new BufferedOutputStream( outputStream, outputBufferSize, true );

        } else {

            // Wrap with the Buffered streams, we don't own the source streams
            inputStream = new BufferedInputStream( inputStream, inputBufferSize );
            outputStream = new BufferedOutputStream( outputStream, outputBufferSize );
        }

        // Now wrap the Buffered Streams with DataInput based streams.  We own
        // the Source streams, all the streams in the chain that we own are
        // destroyed when these are.
        this->dataInputStream.reset( new DataInputStream( inputStream, true ) );
        this->dataOutputStream.reset( new DataOutputStream( outputStream, true ) );

        // Give the IOTransport the streams.
        ioTransport->setInputStream( dataInputStream.get() );
        ioTransport->setOutputStream( dataOutputStream.get() );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
Socket* TcpTransport::createSocket() {

    try {
        SocketFactory* factory = SocketFactory::getDefault();
        return factory->createSocket();
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void TcpTransport::configureSocket( Socket* socket, const Properties& properties ) {

    try {

        // Get the linger flag.
        int soLinger = Integer::parseInt(
            properties.getProperty( "soLinger", "-1" ) );

        // Get the keepAlive flag.
        bool soKeepAlive = Boolean::parseBoolean(
            properties.getProperty( "soKeepAlive", "false" ) );

        // Get the socket receive buffer size.
        int soReceiveBufferSize = Integer::parseInt(
            properties.getProperty( "soReceiveBufferSize", "-1" ) );

        // Get the socket send buffer size.
        int soSendBufferSize = Integer::parseInt(
            properties.getProperty( "soSendBufferSize", "-1" ) );

        // Get the socket TCP_NODELAY flag.
        bool tcpNoDelay = Boolean::parseBoolean(
            properties.getProperty( "tcpNoDelay", "true" ) );

        // Get the socket connect timeout in microseconds. (default to infinite wait).
        this->connectTimeout = Integer::parseInt(
            properties.getProperty( "soConnectTimeout", "0" ) );

        // Set the socket options.
        socket->setKeepAlive( soKeepAlive );
        socket->setTcpNoDelay( tcpNoDelay );

        if( soLinger > 0 ) {
            socket->setSoLinger( true, soLinger );
        }

        if( soReceiveBufferSize > 0 ){
            socket->setReceiveBufferSize( soReceiveBufferSize );
        }

        if( soSendBufferSize > 0 ){
            socket->setSendBufferSize( soSendBufferSize );
        }
    }
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCH_RETHROW( IllegalArgumentException )
    DECAF_CATCH_RETHROW( SocketException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, SocketException )
    DECAF_CATCHALL_THROW( SocketException )
}
