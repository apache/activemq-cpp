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
#include <activemq/util/Config.h>

#if defined(HAVE_WINSOCK2_H)
    #include <Winsock2.h>
    #include <Ws2tcpip.h>
    #include <sys/stat.h>
    #define stat _stat
#else
    #include <unistd.h>
    #include <netdb.h>
    #include <fcntl.h>
    #include <sys/file.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <string.h>
    #include <netinet/tcp.h>
#endif

#ifndef SHUT_RDWR
    #define SHUT_RDWR 2 // Winsock2 doesn't seem to define this
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <sys/types.h>

#include "TcpSocket.h"
#include "SocketInputStream.h"
#include "SocketOutputStream.h"
#include "SocketError.h"

using namespace activemq::network;
using namespace activemq::io;

#if defined(HAVE_WINSOCK2_H)

    // Static socket initializer needed for winsock

    TcpSocket::StaticSocketInitializer::StaticSocketInitializer() {
        socketInitError = NULL;
        const WORD version_needed = MAKEWORD(2,2); // lo-order byte: major version
        WSAData temp;
        if( WSAStartup( version_needed, &temp ) ){
           clear();
           socketInitError = new SocketException ( __FILE__, __LINE__,
               "winsock.dll was not found");
        }
    }
    TcpSocket::StaticSocketInitializer::~StaticSocketInitializer() {
        clear();
        WSACleanup();
    }

    // Create static instance of the socket initializer.
    TcpSocket::StaticSocketInitializer TcpSocket::staticSocketInitializer;

#endif

////////////////////////////////////////////////////////////////////////////////
TcpSocket::TcpSocket() throw (SocketException)
:
    socketHandle( INVALID_SOCKET_HANDLE ),
    inputStream( NULL ),
    outputStream( NULL )
{

    try {

#if defined(HAVE_WINSOCK2_H)
        if( staticSocketInitializer.getSocketInitError() != NULL ) {
            throw *staticSocketInitializer.getSocketInitError();
        }
#endif
    }
    AMQ_CATCH_RETHROW( SocketException )
    AMQ_CATCHALL_THROW( SocketException )
}

////////////////////////////////////////////////////////////////////////////////
TcpSocket::TcpSocket( SocketHandle socketHandle )
:
    socketHandle( INVALID_SOCKET_HANDLE ),
    inputStream( NULL ),
    outputStream( NULL )
{
    try {

#if defined(HAVE_WINSOCK2_H)
        if( staticSocketInitializer.getSocketInitError() != NULL ) {
            throw *staticSocketInitializer.getSocketInitError();
        }
#endif

        this->socketHandle = socketHandle;
        this->inputStream = new SocketInputStream( socketHandle );
        this->outputStream = new SocketOutputStream( socketHandle );
    }
    AMQ_CATCH_RETHROW( SocketException )
    AMQ_CATCHALL_THROW( SocketException )
}

////////////////////////////////////////////////////////////////////////////////
TcpSocket::~TcpSocket()
{
    // No shutdown, just close - dont want blocking destructor.
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

////////////////////////////////////////////////////////////////////////////////
InputStream* TcpSocket::getInputStream(){
    return inputStream;
}

////////////////////////////////////////////////////////////////////////////////
OutputStream* TcpSocket::getOutputStream(){
    return outputStream;
}

////////////////////////////////////////////////////////////////////////////////
void TcpSocket::connect(const char* host, int port) throw ( SocketException )
{
    try{

        if( isConnected() ) {
            throw SocketException( __FILE__, __LINE__,
                "Socket::connect - Socket already connected.  host: %s, port: %d", host, port );
        }

        // Create the socket.
        checkResult( (int)(socketHandle = ::socket(AF_INET, SOCK_STREAM, 0)) );

        // Check port value.
        if( port <= 0 || port > 65535 ) {
            close();
            throw SocketException ( __FILE__, __LINE__,
                "Socket::connect- Port out of range: %d", port );
        }

#ifdef SO_NOSIGPIPE // Don't want to get a SIGPIPE on FreeBSD and Mac OS X

        int optval = 1;
        checkResult( ::setsockopt( socketHandle, SOL_SOCKET, SO_NOSIGPIPE, (char*)&optval, sizeof(optval)) );

#endif

        sockaddr_in target_addr;
        target_addr.sin_family = AF_INET;
        target_addr.sin_port = htons( ( short ) port );
        target_addr.sin_addr.s_addr = 0; // To be set later down...
        memset( &target_addr.sin_zero, 0, sizeof( target_addr.sin_zero ) );

        // Resolve name
#if defined(HAVE_STRUCT_ADDRINFO)
        addrinfo hints;
        memset( &hints, 0, sizeof(addrinfo) );
        hints.ai_family = PF_INET;
        struct addrinfo *res_ptr = NULL;

        checkResult( ::getaddrinfo( host, NULL, &hints, &res_ptr ) );

        if( res_ptr == NULL || res_ptr->ai_addr->sa_family != AF_INET ){
            throw SocketException(
                __FILE__, __LINE__,
                "TcpSocket::connect - getaddrinfo failed, is hostname correct?" );
        }

        // Porting: On both 32bit and 64 bit systems that we compile to soo far, sin_addr
        // is a 32 bit value, not an unsigned long.
        assert( sizeof( ( ( sockaddr_in* )res_ptr->ai_addr )->sin_addr.s_addr ) == 4 );
        target_addr.sin_addr.s_addr = ( ( sockaddr_in* )res_ptr->ai_addr )->sin_addr.s_addr;
        freeaddrinfo( res_ptr );
#else
        struct ::hostent *he = ::gethostbyname(host);
        if( he == NULL ) {
            throw SocketException( __FILE__, __LINE__, "Failed to resolve hostname" );
        }
        target_addr.sin_addr.s_addr = *((in_addr_t *)he->h_addr);
#endif

        // Attempt the connection to the server.
        checkResult( ::connect( socketHandle,
                            ( const sockaddr * )&target_addr,
                            sizeof( target_addr ) ) );

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

        // Create an input/output stream for this socket.
        inputStream = new SocketInputStream( socketHandle );
        outputStream = new SocketOutputStream( socketHandle );
    }
    catch( SocketException& ex ) {
        ex.setMark( __FILE__, __LINE__);
        try{ close(); } catch( cms::CMSException& cx){ /* Absorb */ }
        throw ex;
    }
    catch( ... ){
        try{ close(); } catch( cms::CMSException& cx){ /* Absorb */ }
        throw SocketException( __FILE__, __LINE__, "connect() caught unknown exception");
    }
}

////////////////////////////////////////////////////////////////////////////////
void TcpSocket::close() throw( cms::CMSException )
{
    // Close the input stream.
    if( inputStream != NULL ){
        inputStream->close();
    }

    // Close the output stream.
    if( outputStream != NULL ){
        outputStream->close();
    }

    if( isConnected() )
    {
        ::shutdown( socketHandle, SHUT_RDWR );

        #if !defined(HAVE_WINSOCK2_H)
            ::close( socketHandle );
        #else
           ::closesocket( socketHandle );
        #endif

       socketHandle = INVALID_SOCKET_HANDLE;
    }
}

////////////////////////////////////////////////////////////////////////////////
int TcpSocket::getSoLinger() const throw( SocketException ){

   try{
        linger value;
        socklen_t length = sizeof( value );
        checkResult(::getsockopt( socketHandle, SOL_SOCKET, SO_LINGER, (char*)&value, &length ));

        return value.l_onoff? value.l_linger : 0;
    }
    AMQ_CATCH_RETHROW( SocketException )
    AMQ_CATCHALL_THROW( SocketException )
}

////////////////////////////////////////////////////////////////////////////////
void TcpSocket::setSoLinger( int dolinger ) throw( SocketException ){

    try{
        linger value;
        value.l_onoff = dolinger != 0;
        value.l_linger = dolinger;
        checkResult(::setsockopt( socketHandle, SOL_SOCKET, SO_LINGER, (char*)&value, sizeof(value) ));
    }
    AMQ_CATCH_RETHROW( SocketException )
    AMQ_CATCHALL_THROW( SocketException )
}

////////////////////////////////////////////////////////////////////////////////
bool TcpSocket::getKeepAlive() const throw( SocketException ){

    try{
        int value;
        socklen_t length = sizeof( int );
        checkResult(::getsockopt( socketHandle, SOL_SOCKET, SO_KEEPALIVE, (char*)&value, &length ));
        return value != 0;
    }
    AMQ_CATCH_RETHROW( SocketException )
    AMQ_CATCHALL_THROW( SocketException )
}

////////////////////////////////////////////////////////////////////////////////
void TcpSocket::setKeepAlive( const bool keepAlive ) throw( SocketException ){

    try{
        int value = keepAlive? 1 : 0;
        checkResult(::setsockopt(socketHandle, SOL_SOCKET, SO_KEEPALIVE, (char*)&value, sizeof(int)) );
    }
    AMQ_CATCH_RETHROW( SocketException )
    AMQ_CATCHALL_THROW( SocketException )
}

////////////////////////////////////////////////////////////////////////////////
int TcpSocket::getReceiveBufferSize() const throw( SocketException ){

    try{
        int value;
        socklen_t length = sizeof( value );
        checkResult(::getsockopt( socketHandle, SOL_SOCKET, SO_RCVBUF, (char*)&value, &length ));
        return value;
    }
    AMQ_CATCH_RETHROW( SocketException )
    AMQ_CATCHALL_THROW( SocketException )
}

////////////////////////////////////////////////////////////////////////////////
void TcpSocket::setReceiveBufferSize( int size ) throw( SocketException ){

    try{
        checkResult(::setsockopt( socketHandle, SOL_SOCKET, SO_RCVBUF, (char*)&size, sizeof(size) ));
    }
    AMQ_CATCH_RETHROW( SocketException )
    AMQ_CATCHALL_THROW( SocketException )
}

////////////////////////////////////////////////////////////////////////////////
bool TcpSocket::getReuseAddress() const throw( SocketException ){

    try{
        int value;
        socklen_t length = sizeof( int );
        checkResult(::getsockopt( socketHandle, SOL_SOCKET, SO_REUSEADDR, (char*)&value, &length ));
        return value != 0;
    }
    AMQ_CATCH_RETHROW( SocketException )
    AMQ_CATCHALL_THROW( SocketException )
}

////////////////////////////////////////////////////////////////////////////////
void TcpSocket::setReuseAddress( bool reuse ) throw( SocketException ){

    try{
        int value = reuse? 1 : 0;
        checkResult(::setsockopt( socketHandle, SOL_SOCKET, SO_REUSEADDR, (char*)&value, sizeof(int) ));
    }
    AMQ_CATCH_RETHROW( SocketException )
    AMQ_CATCHALL_THROW( SocketException )
}

////////////////////////////////////////////////////////////////////////////////
int TcpSocket::getSendBufferSize() const throw( SocketException ){

    try{
        int value;
        socklen_t length = sizeof( value );
        checkResult(::getsockopt( socketHandle, SOL_SOCKET, SO_SNDBUF, (char*)&value, &length ));
        return value;
    }
    AMQ_CATCH_RETHROW( SocketException )
    AMQ_CATCHALL_THROW( SocketException )
}

////////////////////////////////////////////////////////////////////////////////
void TcpSocket::setSendBufferSize( int size ) throw( SocketException ){

    try{
        checkResult(::setsockopt( socketHandle, SOL_SOCKET, SO_SNDBUF, (char*)&size, sizeof(size) ));
    }
    AMQ_CATCH_RETHROW( SocketException )
    AMQ_CATCHALL_THROW( SocketException )
}

////////////////////////////////////////////////////////////////////////////////
void TcpSocket::setSoTimeout ( const int millisecs ) throw ( SocketException )
{
    try{

#if !defined(HAVE_WINSOCK2_H)
        timeval timot;
        timot.tv_sec = millisecs / 1000;
        timot.tv_usec = (millisecs % 1000) * 1000;
#else
        int timot = millisecs;
#endif

        checkResult(::setsockopt( socketHandle, SOL_SOCKET, SO_RCVTIMEO, (const char*) &timot, sizeof (timot) ));
        checkResult(::setsockopt( socketHandle, SOL_SOCKET, SO_SNDTIMEO, (const char*) &timot, sizeof (timot) ));
    }
    AMQ_CATCH_RETHROW( SocketException )
    AMQ_CATCHALL_THROW( SocketException )
}

////////////////////////////////////////////////////////////////////////////////
int TcpSocket::getSoTimeout() const throw( SocketException )
{
    try{

#if !defined(HAVE_WINSOCK2_H)
        timeval timot;
        timot.tv_sec = 0;
        timot.tv_usec = 0;
        socklen_t size = sizeof(timot);
#else
        int timot = 0;
        int size = sizeof(timot);
#endif

        checkResult(::getsockopt(socketHandle, SOL_SOCKET, SO_RCVTIMEO, (char*) &timot, &size));

#if !defined(HAVE_WINSOCK2_H)
        return (timot.tv_sec * 1000) + (timot.tv_usec / 1000);
#else
        return timot;
#endif

    }
    AMQ_CATCH_RETHROW( SocketException )
    AMQ_CATCHALL_THROW( SocketException )

}

////////////////////////////////////////////////////////////////////////////////
bool TcpSocket::getTcpNoDelay() const throw ( cms::CMSException ) {

    try{
        int value;
        socklen_t length = sizeof( int );
        checkResult(::getsockopt( socketHandle, IPPROTO_TCP, TCP_NODELAY, (char*)&value, &length ));
        return value != 0;
    }
    AMQ_CATCH_RETHROW( SocketException )
    AMQ_CATCHALL_THROW( SocketException )
}

////////////////////////////////////////////////////////////////////////////////
void TcpSocket::setTcpNoDelay( bool value ) throw ( cms::CMSException ) {

    try{
        int ivalue = value ? 1 : 0;
        checkResult(::setsockopt( socketHandle, IPPROTO_TCP, TCP_NODELAY, (char*)&ivalue, sizeof(int) ));
    }
    AMQ_CATCH_RETHROW( SocketException )
    AMQ_CATCHALL_THROW( SocketException )
}

////////////////////////////////////////////////////////////////////////////////
void TcpSocket::checkResult( int value ) const throw (SocketException) {

    if( value < 0 ){
        throw SocketException( __FILE__, __LINE__,
            SocketError::getErrorString().c_str() );
    }
}


