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

#include "ServerSocket.h"
#include "SocketError.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <sys/types.h>
#include <assert.h>
#include <string>

using namespace decaf;
using namespace decaf::net;

////////////////////////////////////////////////////////////////////////////////
ServerSocket::ServerSocket() {
    socketHandle = (apr_socket_t*)Socket::INVALID_SOCKET_HANDLE;
}

////////////////////////////////////////////////////////////////////////////////
ServerSocket::~ServerSocket() {
    // No shutdown, just close - dont want blocking destructor.
    close();
}

////////////////////////////////////////////////////////////////////////////////
void ServerSocket::bind( const char* host, int port ) throw ( SocketException ) {
    this->bind( host, port, SOMAXCONN );
}

////////////////////////////////////////////////////////////////////////////////
void ServerSocket::bind( const char* host,
                         int port,
                         int backlog ) throw ( SocketException ) {

    apr_status_t result = APR_SUCCESS;

    if( isBound() ) {
        throw SocketException ( __FILE__, __LINE__,
            "ServerSocket::bind - Socket already bound" );
    }

    // Verify the port value.
    if( port <= 0 || port > 65535 ) {
        throw SocketException(
            __FILE__, __LINE__,
            "ServerSocket::bind - Port out of range: %d", port );
    }

    // Create the Address Info for the Socket
    result = apr_sockaddr_info_get(
        &socketAddress, host, APR_INET, port, 0, apr_pool.getAprPool() );

    if( result != APR_SUCCESS ) {
        socketHandle = (apr_socket_t*)Socket::INVALID_SOCKET_HANDLE;
        throw SocketException(
              __FILE__, __LINE__,
              SocketError::getErrorString().c_str() );
    }

    // Create the socket.
    result = apr_socket_create(
        &socketHandle, APR_INET, SOCK_STREAM, APR_PROTO_TCP, apr_pool.getAprPool() );

    if( result != APR_SUCCESS ) {
        socketHandle = (apr_socket_t*)Socket::INVALID_SOCKET_HANDLE;
        throw SocketException(
              __FILE__, __LINE__,
              SocketError::getErrorString().c_str() );
    }

    // Set the socket to reuse the address and default as blocking
    apr_socket_opt_set( socketHandle, APR_SO_REUSEADDR, 1 );
    apr_socket_opt_set( socketHandle, APR_SO_NONBLOCK, 0);
    apr_socket_timeout_set( socketHandle, -1 );

    // Bind to the Socket, this may be where we find out if the port is in use.
    result = apr_socket_bind( socketHandle, socketAddress );

    if( result != APR_SUCCESS ) {
        close();
        throw SocketException(
              __FILE__, __LINE__,
              "ServerSocket::bind - %s",
              SocketError::getErrorString().c_str() );
    }

    // Setup the listen for incoming connection requests
    result = apr_socket_listen( socketHandle, backlog );

    if( result != APR_SUCCESS ) {
        close();
        throw SocketException(
              __FILE__, __LINE__,
              "ServerSocket::bind - %s",
              SocketError::getErrorString().c_str() );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ServerSocket::close() throw ( lang::Exception ){

    if( isBound() ) {
        apr_socket_close( socketHandle );
        socketHandle = (apr_socket_t*)Socket::INVALID_SOCKET_HANDLE;
    }
}

////////////////////////////////////////////////////////////////////////////////
bool ServerSocket::isBound() const {
    return this->socketHandle != (apr_socket_t*)Socket::INVALID_SOCKET_HANDLE;
}

////////////////////////////////////////////////////////////////////////////////
Socket* ServerSocket::accept() throw (SocketException)
{
    SocketHandle incoming = NULL;
    apr_status_t result = APR_SUCCESS;

    // Loop to ignore any signal interruptions that occur during the operation.
    do {
        result = apr_socket_accept( &incoming, socketHandle, apr_pool.getAprPool() );
    } while( result == APR_EINTR );

    if( result != APR_SUCCESS ) {
        std::cout << "Failed to accept New Connection:" << std::endl;
        throw SocketException(
              __FILE__, __LINE__,
              "ServerSocket::accept - %s",
              SocketError::getErrorString().c_str() );
    }

    return new TcpSocket( incoming );
}
