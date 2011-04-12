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

#include "TcpSocket.h"

#include <decaf/internal/net/SocketFileDescriptor.h>
#include <decaf/internal/net/tcp/TcpSocketInputStream.h>
#include <decaf/internal/net/tcp/TcpSocketOutputStream.h>

#include <decaf/net/SocketError.h>
#include <decaf/net/SocketOptions.h>
#include <decaf/lang/Character.h>
#include <decaf/lang/exceptions/UnsupportedOperationException.h>
#include <stdlib.h>
#include <string>
#include <stdio.h>
#include <iostream>

#include <apr_portable.h>

#if !defined(HAVE_WINSOCK2_H)
    #include <sys/select.h>
    #include <sys/socket.h>
#else
    #include <Winsock2.h>
#endif

#ifdef HAVE_SYS_IOCTL_H
#define BSD_COMP /* Get FIONREAD on Solaris2. */
#include <sys/ioctl.h>
#include <unistd.h>
#endif

// Pick up FIONREAD on Solaris 2.5.
#ifdef HAVE_SYS_FILIO_H
#include <sys/filio.h>
#endif

using namespace decaf;
using namespace decaf::internal;
using namespace decaf::internal::net;
using namespace decaf::internal::net::tcp;
using namespace decaf::net;
using namespace decaf::io;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
TcpSocket::TcpSocket() : apr_pool(),
                         socketHandle( NULL ),
                         localAddress( NULL ),
                         remoteAddress( NULL ),
                         inputStream( NULL ),
                         outputStream( NULL ),
                         inputShutdown( false ),
                         outputShutdown( false ),
                         closed( false ),
                         trafficClass( 0 ),
                         soTimeout( -1 ),
                         soLinger( -1 ) {
}

////////////////////////////////////////////////////////////////////////////////
TcpSocket::~TcpSocket() {

    try{

        // No shutdown, just close - don't want a blocking destructor.
        close();

        // Destroy the input stream.
        if( inputStream != NULL ){
            delete inputStream;
            inputStream = NULL;
        }

        // Destroy the output stream.
        if( outputStream != NULL ){
            delete outputStream;
            outputStream = NULL;
        }
    }
    DECAF_CATCH_NOTHROW( Exception )
    DECAF_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void TcpSocket::create() {

    try{

        if( this->socketHandle != NULL ) {
            throw IOException(
                __FILE__, __LINE__, "The System level socket has already been created." );
        }

        // Create the actual socket.
        checkResult( apr_socket_create( &socketHandle, AF_INET, SOCK_STREAM,
                                        APR_PROTO_TCP, apr_pool.getAprPool() ) );

        // Initialize the Socket's FileDescriptor
        apr_os_sock_t osSocket = -1;
        apr_os_sock_get( &osSocket, socketHandle );
        this->fd = new SocketFileDescriptor( osSocket );
    }
    DECAF_CATCH_RETHROW( decaf::io::IOException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, decaf::io::IOException )
    DECAF_CATCHALL_THROW( decaf::io::IOException )
}

////////////////////////////////////////////////////////////////////////////////
void TcpSocket::accept( SocketImpl* socket ) {

    try{

        if( socket == NULL ) {
            throw IOException(
                __FILE__, __LINE__, "SocketImpl instance passed was null." );
        }

        TcpSocket* impl = dynamic_cast<TcpSocket*>( socket );
        if( impl == NULL ) {
            throw IOException(
                __FILE__, __LINE__, "SocketImpl instance passed was not a TcpSocket." );
        }

        apr_status_t result = APR_SUCCESS;

        // Loop to ignore any signal interruptions that occur during the operation.
        do {
            result = apr_socket_accept( &impl->socketHandle, socketHandle, apr_pool.getAprPool() );
        } while( result == APR_EINTR );

        if( result != APR_SUCCESS ) {
            throw SocketException(
                  __FILE__, __LINE__,
                  "ServerSocket::accept - %s",
                  SocketError::getErrorString().c_str() );
        }
    }
    DECAF_CATCH_RETHROW( decaf::io::IOException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, decaf::io::IOException )
    DECAF_CATCHALL_THROW( decaf::io::IOException )
}

////////////////////////////////////////////////////////////////////////////////
InputStream* TcpSocket::getInputStream() {

    if( this->socketHandle == NULL || this->closed ) {
        throw IOException( __FILE__, __LINE__, "The Socket is not Connected." );
    }

    if( this->inputShutdown ) {
        throw IOException( __FILE__, __LINE__, "Input has been shut down on this Socket." );
    }

    try{

        if( this->inputStream == NULL ) {
            this->inputStream = new TcpSocketInputStream( this );
        }

        return inputStream;
    }
    DECAF_CATCH_RETHROW( decaf::io::IOException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, decaf::io::IOException )
    DECAF_CATCHALL_THROW( decaf::io::IOException )
}

////////////////////////////////////////////////////////////////////////////////
OutputStream* TcpSocket::getOutputStream() {

    if( this->socketHandle == NULL || this->closed ) {
        throw IOException( __FILE__, __LINE__, "The Socket is not Connected." );
    }

    if( this->outputShutdown ) {
        throw IOException( __FILE__, __LINE__, "Output has been shut down on this Socket." );
    }

    try{

        if( this->outputStream == NULL ) {
            this->outputStream = new TcpSocketOutputStream( this );
        }

        return outputStream;
    }
    DECAF_CATCH_RETHROW( decaf::io::IOException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, decaf::io::IOException )
    DECAF_CATCHALL_THROW( decaf::io::IOException )
}

////////////////////////////////////////////////////////////////////////////////
void TcpSocket::bind( const std::string& ipaddress, int port ) {

    try{

        const char* host = ipaddress.empty() ? NULL : ipaddress.c_str();

        // Create the Address Info for the Socket
        apr_status_t result = apr_sockaddr_info_get(
            &localAddress, host, APR_INET, (apr_port_t)port, 0, apr_pool.getAprPool() );

        if( result != APR_SUCCESS ) {
            socketHandle = NULL;
            throw SocketException(
                  __FILE__, __LINE__,
                  SocketError::getErrorString().c_str() );
        }

        // Set the socket to reuse the address and default as blocking with no timeout.
        apr_socket_opt_set( socketHandle, APR_SO_REUSEADDR, 1 );
        apr_socket_opt_set( socketHandle, APR_SO_NONBLOCK, 0 );
        apr_socket_timeout_set( socketHandle, -1 );

        // Bind to the Socket, this may be where we find out if the port is in use.
        result = apr_socket_bind( socketHandle, localAddress );

        if( result != APR_SUCCESS ) {
            close();
            throw SocketException(
                  __FILE__, __LINE__,
                  "ServerSocket::bind - %s",
                  SocketError::getErrorString().c_str() );
        }

        // Only incur the overhead of a lookup if we don't already know the local port.
        if( port != 0 ) {
            this->localPort = port;
        } else {
            apr_sockaddr_t* localAddress;
            checkResult( apr_socket_addr_get( &localAddress, APR_LOCAL, socketHandle ) );
            this->localPort = localAddress->port;
        }
    }
    DECAF_CATCH_RETHROW( decaf::io::IOException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, decaf::io::IOException )
    DECAF_CATCHALL_THROW( decaf::io::IOException )
}

////////////////////////////////////////////////////////////////////////////////
void TcpSocket::connect( const std::string& hostname, int port, int timeout ) {

    try{

        if( port < 0 || port > 65535 ) {
            throw IllegalArgumentException(
                __FILE__, __LINE__, "Given port is out of range: %d", port );
        }

        if( this->socketHandle == NULL ) {
            throw IOException(
                __FILE__, __LINE__, "The socket was not yet created." );
        }

        // Create the Address data
        checkResult( apr_sockaddr_info_get(
            &remoteAddress, hostname.c_str(), APR_INET, (apr_port_t)port, 0, apr_pool.getAprPool() ) );

        int oldNonblockSetting = 0;
        apr_interval_time_t oldTimeoutSetting = 0;

        // Record the old settings.
        apr_socket_opt_get( socketHandle, APR_SO_NONBLOCK, &oldNonblockSetting );
        apr_socket_timeout_get( socketHandle, &oldTimeoutSetting );

        // Temporarily make it what we want, blocking.
        apr_socket_opt_set( socketHandle, APR_SO_NONBLOCK, 0 );

        // Timeout and non-timeout case require very different logic.
        if( timeout <= 0 ) {
            apr_socket_timeout_set( socketHandle, -1 );
        } else {
            apr_socket_timeout_set( socketHandle, timeout * 1000 );
        }

        // try to Connect to the provided address.
        checkResult( apr_socket_connect( socketHandle, remoteAddress ) );

        // Now that we are connected, we want to go back to old settings.
        apr_socket_opt_set( socketHandle, APR_SO_NONBLOCK, oldNonblockSetting );
        apr_socket_timeout_set( socketHandle, oldTimeoutSetting );

        // Now that we connected, cache the port value for later lookups.
        this->port = port;

    } catch( IOException& ex ) {
        ex.setMark( __FILE__, __LINE__);
        try{ close(); } catch( lang::Exception& cx){ /* Absorb */ }
        throw ex;
    } catch( IllegalArgumentException& ex ) {
        ex.setMark( __FILE__, __LINE__);
        try{ close(); } catch( lang::Exception& cx){ /* Absorb */ }
        throw ex;
    } catch( Exception& ex ) {
        try{ close(); } catch( lang::Exception& cx){ /* Absorb */ }
        throw SocketException( &ex );
    } catch( ... ) {
        try{ close(); } catch( lang::Exception& cx){ /* Absorb */ }
        throw SocketException(
            __FILE__, __LINE__,
            "TcpSocket::connect() - caught unknown exception" );
    }
}

////////////////////////////////////////////////////////////////////////////////
std::string TcpSocket::getLocalAddress() const {

    if( !isClosed() ) {
        SocketAddress addr;
        checkResult( apr_socket_addr_get( &addr, APR_LOCAL, this->socketHandle ) );
        char ipStr[20] = {0};
        checkResult( apr_sockaddr_ip_getbuf( ipStr, 20, addr ) );

        return std::string( ipStr, 20 );
    }

    return "0.0.0.0";
}

////////////////////////////////////////////////////////////////////////////////
void TcpSocket::listen( int backlog ) {

    try{

        if( isClosed() ){
            throw IOException(
                __FILE__, __LINE__, "The stream is closed" );
        }

        // Setup the listen for incoming connection requests
        apr_status_t result = apr_socket_listen( socketHandle, backlog );

        if( result != APR_SUCCESS ) {
            close();
            throw SocketException(
                __FILE__, __LINE__, "Error on Bind - %s",
                SocketError::getErrorString().c_str() );
        }
    }
    DECAF_CATCH_RETHROW( decaf::io::IOException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, decaf::io::IOException )
    DECAF_CATCHALL_THROW( decaf::io::IOException )
}

////////////////////////////////////////////////////////////////////////////////
int TcpSocket::available() {

    if( isClosed() ){
        throw IOException(
            __FILE__, __LINE__, "The stream is closed" );
    }

    // Convert to an OS level socket.
    apr_os_sock_t oss;
    apr_os_sock_get( (apr_os_sock_t*)&oss, socketHandle );

// The windows version
#if defined(HAVE_WINSOCK2_H)

    unsigned long numBytes = 0;

    if( ::ioctlsocket( oss, FIONREAD, &numBytes ) == SOCKET_ERROR ){
        throw SocketException( __FILE__, __LINE__, "ioctlsocket failed" );
    }

    return numBytes;

#else // !defined(HAVE_WINSOCK2_H)

    // If FIONREAD is defined - use ioctl to find out how many bytes
    // are available.
    #if defined(FIONREAD)

        int numBytes = 0;
        if( ::ioctl( oss, FIONREAD, &numBytes ) != -1 ){
            return numBytes;
        }

    #endif

    // If we didn't get anything we can use select.  This is a little
    // less functional.  We will poll on the socket - if there is data
    // available, we'll return 1, otherwise we'll return zero.
    #if defined(HAVE_SELECT)

        fd_set rd;
        FD_ZERO(&rd);
        FD_SET( oss, &rd );
        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 0;
        int returnCode = ::select( oss+1, &rd, NULL, NULL, &tv );
        if( returnCode == -1 ){
            throw IOException(
                __FILE__, __LINE__,
                SocketError::getErrorString().c_str() );
        }
        return (returnCode == 0) ? 0 : 1;

    #else

        return 0;

    #endif /* HAVE_SELECT */

#endif // !defined(HAVE_WINSOCK2_H)
}

////////////////////////////////////////////////////////////////////////////////
void TcpSocket::close() {

    try{

        if( this->closed ) {
            return;
        }

        this->closed = true;

        // Destroy the input stream.
        if( inputStream != NULL ){
            inputStream->close();
        }

        // Destroy the output stream.
        if( outputStream != NULL ){
            outputStream->close();
        }

        // When connected we first shutdown, which breaks our reads and writes
        // then we close to free APR resources.
        if( isConnected() ) {
            // Destory the allocated resources.
            apr_socket_shutdown( socketHandle, APR_SHUTDOWN_READWRITE );
            apr_socket_close( socketHandle );
            delete this->fd;

            // Clear out the member data.
            this->socketHandle = NULL;
            this->port = 0;
            this->localPort = 0;
        }
    }
    DECAF_CATCH_RETHROW( decaf::io::IOException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, decaf::io::IOException )
    DECAF_CATCHALL_THROW( decaf::io::IOException )
}

////////////////////////////////////////////////////////////////////////////////
void TcpSocket::shutdownInput() {

    if( isClosed() ){
        throw IOException(
            __FILE__, __LINE__, "The stream is closed" );
    }

    this->inputShutdown = true;
    apr_socket_shutdown( socketHandle, APR_SHUTDOWN_READ );
}

////////////////////////////////////////////////////////////////////////////////
void TcpSocket::shutdownOutput() {

    if( isClosed() ){
        throw IOException(
            __FILE__, __LINE__, "The stream is closed" );
    }

    this->outputShutdown = true;
    apr_socket_shutdown( socketHandle, APR_SHUTDOWN_WRITE );
}

////////////////////////////////////////////////////////////////////////////////
int TcpSocket::getOption( int option ) const {

    try{

        if( isClosed() ) {
            throw IOException(
                __FILE__, __LINE__, "The Socket is closed." );
        }

        apr_int32_t aprId = 0;
        apr_int32_t value = 0;

        if( option == SocketOptions::SOCKET_OPTION_TIMEOUT ) {

            // Time in APR on socket is stored in microseconds.
            apr_interval_time_t tvalue = 0;
            checkResult( apr_socket_timeout_get( socketHandle, &tvalue ) );
            return (int)( tvalue / 1000 );
        } else if( option == SocketOptions::SOCKET_OPTION_LINGER ) {

            checkResult( apr_socket_opt_get( socketHandle, APR_SO_LINGER, &value ) );

            // In case the socket linger is on by default we reset to match,
            // we just use one since we really don't know what the linger time is
            // with APR.
            if( value == 1 && this->soLinger == -1 ) {
                this->soLinger = 1;
            }

            return this->soLinger;
        }

        if( option == SocketOptions::SOCKET_OPTION_REUSEADDR ) {
            aprId = APR_SO_REUSEADDR;
        } else if( option == SocketOptions::SOCKET_OPTION_SNDBUF ) {
            aprId = APR_SO_SNDBUF;
        } else if( option == SocketOptions::SOCKET_OPTION_RCVBUF ) {
            aprId = APR_SO_RCVBUF;
        } else if( option == SocketOptions::SOCKET_OPTION_TCP_NODELAY ) {
            aprId = APR_TCP_NODELAY;
        } else if( option == SocketOptions::SOCKET_OPTION_KEEPALIVE ) {
            aprId = APR_SO_KEEPALIVE;
        } else {
            throw IOException(
                __FILE__, __LINE__,
                "Socket Option is not valid for this Socket type." );
        }

        checkResult( apr_socket_opt_get( socketHandle, aprId, &value ) );

        return (int)value;
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void TcpSocket::setOption( int option, int value ) {

    try{

        if( isClosed() ) {
            throw IOException(
                __FILE__, __LINE__, "The Socket is closed." );
        }

        apr_int32_t aprId = 0;

        if( option == SocketOptions::SOCKET_OPTION_TIMEOUT ) {
            checkResult( apr_socket_opt_set( socketHandle, APR_SO_NONBLOCK, 0 ) );
            // Time in APR for sockets is in microseconds so multiply by 1000.
            checkResult( apr_socket_timeout_set( socketHandle, value * 1000 ) );
            this->soTimeout = value;
            return;
        } else if( option == SocketOptions::SOCKET_OPTION_LINGER ) {

            // Store the real setting for later.
            this->soLinger = value;

            // Now use the APR API to set it to the boolean state that APR expects
            value = value <= 0 ? 0 : 1;
            checkResult( apr_socket_opt_set( socketHandle, APR_SO_LINGER, (apr_int32_t)value ) );
            return;
        }

        if( option == SocketOptions::SOCKET_OPTION_REUSEADDR ) {
            aprId = APR_SO_REUSEADDR;
        } else if( option == SocketOptions::SOCKET_OPTION_SNDBUF ) {
            aprId = APR_SO_SNDBUF;
        } else if( option == SocketOptions::SOCKET_OPTION_RCVBUF ) {
            aprId = APR_SO_RCVBUF;
        } else if( option == SocketOptions::SOCKET_OPTION_TCP_NODELAY ) {
            aprId = APR_TCP_NODELAY;
        } else if( option == SocketOptions::SOCKET_OPTION_KEEPALIVE ) {
            aprId = APR_SO_KEEPALIVE;
        } else {
            throw IOException(
                __FILE__, __LINE__,
                "Socket Option is not valid for this Socket type." );
        }

        checkResult( apr_socket_opt_set( socketHandle, aprId, (apr_int32_t)value ) );
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void TcpSocket::checkResult( apr_status_t value ) const {

    if( value != APR_SUCCESS ){
        throw SocketException(
            __FILE__, __LINE__,
            SocketError::getErrorString().c_str() );
    }
}

////////////////////////////////////////////////////////////////////////////////
int TcpSocket::read( unsigned char* buffer, int size, int offset, int length ) {

    try{
        if( this->isClosed() ){
            throw IOException(
                __FILE__, __LINE__, "The Stream has been closed" );
        }

        if( this->inputShutdown == true ) {
            return -1;
        }

        if( length == 0 ) {
            return 0;
        }

        if( buffer == NULL ) {
            throw NullPointerException(
                __FILE__, __LINE__, "Buffer passed is Null" );
        }

        if( size < 0 ) {
            throw IndexOutOfBoundsException(
                __FILE__, __LINE__, "size parameter out of Bounds: %d.", size );
        }

        if( offset > size || offset < 0 ) {
            throw IndexOutOfBoundsException(
                __FILE__, __LINE__, "offset parameter out of Bounds: %d.", offset );
        }

        if( length < 0 || length > size - offset ) {
            throw IndexOutOfBoundsException(
                __FILE__, __LINE__, "length parameter out of Bounds: %d.", length );
        }

        apr_size_t aprSize = (apr_size_t)length;
        apr_status_t result = APR_SUCCESS;

        // Read data from the socket, size on input is size of buffer, when done
        // size is the number of bytes actually read, can be <= bufferSize.
        result = apr_socket_recv( socketHandle, (char*)buffer + offset, &aprSize );

        // Check for EOF, on windows we only get size==0 so check that to, if we
        // were closed though then we throw an IOException so the caller knows we
        // aren't usable anymore.
        if( ( APR_STATUS_IS_EOF( result ) || aprSize == 0 ) && !closed ) {
            this->inputShutdown = true;
            return -1;
        }

        if( isClosed() ){
            throw IOException(
                __FILE__, __LINE__, "The connection is closed" );
        }

        if( result != APR_SUCCESS ){
            throw IOException(
                __FILE__, __LINE__,
                "Socket Read Error - %s",
                SocketError::getErrorString().c_str() );
        }

        return (int)aprSize;
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void TcpSocket::write( const unsigned char* buffer, int size, int offset, int length ) {

    try{

        if( length == 0 ) {
            return;
        }

        if( buffer == NULL ) {
            throw NullPointerException(
                __FILE__, __LINE__,
                "TcpSocketOutputStream::write - passed buffer is null" );
        }

        if( closed ) {
            throw IOException(
                __FILE__, __LINE__,
                "TcpSocketOutputStream::write - This Stream has been closed." );
        }

        if( size < 0 ) {
            throw IndexOutOfBoundsException(
                __FILE__, __LINE__, "size parameter out of Bounds: %d.", size );
        }

        if( offset > size || offset < 0 ) {
            throw IndexOutOfBoundsException(
                __FILE__, __LINE__, "offset parameter out of Bounds: %d.", offset );
        }

        if( length < 0 || length > size - offset ) {
            throw IndexOutOfBoundsException(
                __FILE__, __LINE__, "length parameter out of Bounds: %d.", length );
        }

        apr_size_t remaining = (apr_size_t)length;
        apr_status_t result = APR_SUCCESS;

        const unsigned char* lbuffer = buffer + offset;

        while( remaining > 0 && !closed ) {

            // On input remaining is the bytes to send, after return remaining
            // is the amount actually sent.
            result = apr_socket_send( socketHandle, (const char*)lbuffer, &remaining );

            if( result != APR_SUCCESS || closed ) {
                throw IOException(
                    __FILE__, __LINE__,
                    "decaf::net::TcpSocketOutputStream::write - %s",
                    SocketError::getErrorString().c_str() );
            }

            // move us to next position to write, or maybe end.
            lbuffer += remaining;
            remaining = length - remaining;
        }
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCHALL_THROW( IOException )
}
