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
#include <decaf/net/SocketFactory.h>
#include <decaf/net/BufferedSocket.h>
#include <decaf/net/TcpSocket.h>
#include <decaf/util/Properties.h>
#include <stdio.h>

using namespace std;
using namespace decaf;
using namespace decaf::util;
using namespace decaf::net;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
Socket* SocketFactory::createSocket(
    const std::string& uri,
    const Properties& properties)
        throw ( SocketException ) {

    try {

        // Ensure something is actually passed in for the URI
        if( uri == "" ) {
            throw SocketException( __FILE__, __LINE__,
                "SocketTransport::start() - uri not provided" );
        }

        string dummy = uri;

        // Extract the port.
        std::size_t portIx = dummy.find( ':' );
        if( portIx == string::npos ) {
            throw SocketException( __FILE__, __LINE__,
                "SocketTransport::start() - uri malformed - port not specified: %s", uri.c_str() );
        }
        string host = dummy.substr( 0, portIx );
        string portString = dummy.substr( portIx + 1 );
        int port;
        if( sscanf( portString.c_str(), "%d", &port) != 1 ) {
            throw SocketException( __FILE__, __LINE__,
               "SocketTransport::start() - unable to extract port from uri: %s", uri.c_str() );
        }

        // Get the read buffer size.
        int inputBufferSize = 10000;
        dummy = properties.getProperty( "inputBufferSize", "10000" );
        sscanf( dummy.c_str(), "%d", &inputBufferSize );

        // Get the write buffer size.
        int outputBufferSize = 10000;
        dummy = properties.getProperty( "outputBufferSize", "10000" );
        sscanf( dummy.c_str(), "%d", &outputBufferSize );

        // Get the linger flag.
        int soLinger = 0;
        dummy = properties.getProperty( "soLinger", "0" );
        sscanf( dummy.c_str(), "%d", &soLinger );

        // Get the keepAlive flag.
        bool soKeepAlive =
            properties.getProperty( "soKeepAlive", "false" ) == "true";

        // Get the socket receive buffer size.
        int soReceiveBufferSize = -1;
        dummy = properties.getProperty( "soReceiveBufferSize", "-1" );
        sscanf( dummy.c_str(), "%d", &soReceiveBufferSize );

        // Get the socket send buffer size.
        int soSendBufferSize = -1;
        dummy = properties.getProperty( "soSendBufferSize", "-1" );
        sscanf( dummy.c_str(), "%d", &soSendBufferSize );

        // Get the socket TCP_NODELAY flag.
        bool tcpNoDelay =
            properties.getProperty( "tcpNoDelay", "true" ) == "true";

        // Get the socket connect timeout in microseconds.
        int connectTimeout = -1;
        dummy = properties.getProperty( "soConnectTimeout", "-1" );
        sscanf( dummy.c_str(), "%d", &connectTimeout );

        // Now that we have all the elements that we wanted - let's do it!
        // Create a TCP Socket and then Wrap it in a buffered socket
        // so that users get the benefit of buffered reads and writes.
        // The buffered socket will own the TcpSocket instance, and will
        // clean it up when it is cleaned up.
        TcpSocket* tcpSocket = new TcpSocket();

        try {

            // Connect the socket.
            tcpSocket->connect( host.c_str(), port, connectTimeout );

            // Set the socket options.
            tcpSocket->setSoLinger( soLinger );
            tcpSocket->setKeepAlive( soKeepAlive );
            tcpSocket->setTcpNoDelay( tcpNoDelay );

            if( soReceiveBufferSize > 0 ){
                tcpSocket->setReceiveBufferSize( soReceiveBufferSize );
            }

            if( soSendBufferSize > 0 ){
                tcpSocket->setSendBufferSize( soSendBufferSize );
            }
        } catch ( SocketException& ex ) {
            ex.setMark( __FILE__, __LINE__ );
            try{
                delete tcpSocket;
            } catch( SocketException& ex2 ){ /* Absorb */ }

            throw ex;
        }

        return tcpSocket;
    }
    DECAF_CATCH_RETHROW( SocketException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, SocketException )
    DECAF_CATCHALL_THROW( SocketException )
}
