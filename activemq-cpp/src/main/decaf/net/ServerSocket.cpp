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

#include <decaf/net/SocketImpl.h>
#include <decaf/net/Socket.h>
#include <decaf/net/SocketOptions.h>

#include <decaf/internal/net/tcp/TcpSocket.h>

#include <memory>

using namespace decaf;
using namespace decaf::io;
using namespace decaf::net;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::internal;
using namespace decaf::internal::net;
using namespace decaf::internal::net::tcp;

////////////////////////////////////////////////////////////////////////////////
SocketImplFactory* ServerSocket::factory = NULL;

////////////////////////////////////////////////////////////////////////////////
ServerSocket::ServerSocket() : impl(NULL), created(false), closed(false), bound(false) {

    this->impl = this->factory != NULL ? factory->createSocketImpl() : new TcpSocket();
}

////////////////////////////////////////////////////////////////////////////////
ServerSocket::ServerSocket( int port )
    throw( decaf::io::IOException, decaf::lang::exceptions::IllegalArgumentException ) :
        impl(NULL), created(false), closed(false), bound(false) {

    this->impl = this->factory != NULL ? factory->createSocketImpl() : new TcpSocket();

    if( port < 0 ) {
        throw IllegalArgumentException(
            __FILE__, __LINE__, "Port value was invalid: %d", port );
    }

    this->setupSocketImpl( "", port, getDefaultBacklog() );
}

////////////////////////////////////////////////////////////////////////////////
ServerSocket::ServerSocket( int port, int backlog )
    throw( decaf::io::IOException, decaf::lang::exceptions::IllegalArgumentException ) :
        impl(NULL), created(false), closed(false), bound(false) {

    this->impl = this->factory != NULL ? factory->createSocketImpl() : new TcpSocket();

    if( port < 0 ) {
        throw IllegalArgumentException(
            __FILE__, __LINE__, "Port value was invalid: %d", port );
    }

    this->setupSocketImpl( "", port, backlog );
}

////////////////////////////////////////////////////////////////////////////////
ServerSocket::ServerSocket( const std::string& ipAddress, int port, int backlog )
    throw( decaf::io::IOException, decaf::lang::exceptions::IllegalArgumentException ) :
        impl(NULL), created(false), closed(false), bound(false) {

    this->impl = this->factory != NULL ? factory->createSocketImpl() : new TcpSocket();

    if( port < 0 ) {
        throw IllegalArgumentException(
            __FILE__, __LINE__, "Port value was invalid: %d", port );
    }

    this->setupSocketImpl( ipAddress, port, backlog );
}

////////////////////////////////////////////////////////////////////////////////
ServerSocket::~ServerSocket() {
    try{
        close();

        delete this->impl;
    }
    DECAF_CATCH_NOTHROW( Exception )
    DECAF_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void ServerSocket::setupSocketImpl( const std::string ipAddress, int port, int backlog ) {

    try{

        this->impl->create();
        this->created = true;

        std::string bindAddr = ipAddress.empty() ? "0.0.0.0" : ipAddress;

        try {
            this->impl->bind( bindAddr, port );
            this->bound = true;
            this->impl->listen( backlog > 0 ? backlog : getDefaultBacklog() );
        } catch( IOException& ex ) {
            close();
            throw ex;
        }
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_RETHROW( IllegalArgumentException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void ServerSocket::bind( const std::string& host, int port )
    throw( decaf::io::IOException, decaf::lang::exceptions::IllegalArgumentException ) {

    try{
        this->bind( host, port, getDefaultBacklog() );
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_RETHROW( IllegalArgumentException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void ServerSocket::bind( const std::string& address, int port, int backlog )
    throw( decaf::io::IOException, decaf::lang::exceptions::IllegalArgumentException ) {

    checkClosed();

    if( port < 0 || port > 65535 ) {
        throw IllegalArgumentException(
            __FILE__, __LINE__, "Specified port value is out of range: %d", port );
    }

    if( isBound() ) {
        throw IOException(
            __FILE__, __LINE__, "ServerSocket is already bound." );
    }

    try{

        ensureCreated();

        try {
            this->impl->bind( address, port );
            this->bound = true;
            this->impl->listen( backlog > 0 ? backlog : getDefaultBacklog() );
        } catch( IOException& ex ) {
            close();
            throw ex;
        }
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_RETHROW( IllegalArgumentException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void ServerSocket::close() throw ( decaf::io::IOException ){

    try{
        if( !this->closed ) {
            this->closed = true;
            this->impl->close();
        }
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
bool ServerSocket::isBound() const {
    return this->bound;
}

////////////////////////////////////////////////////////////////////////////////
bool ServerSocket::isClosed() const {
    return this->closed;
}

////////////////////////////////////////////////////////////////////////////////
Socket* ServerSocket::accept() throw( decaf::io::IOException ) {

    checkClosed();

    if( !isBound() ) {
        throw IOException(
            __FILE__, __LINE__, "The ServerSocket has not yet been bound." );
    }

    try{
        ensureCreated();
        std::auto_ptr<Socket> newSocket( new Socket() );
        this->implAccept( newSocket.get() );
        return newSocket.release();
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void ServerSocket::implAccept( Socket* socket ) throw( decaf::io::IOException ) {

    try{
        this->impl->accept( socket->impl );
        socket->accepted();
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
int ServerSocket::getReceiveBufferSize() const throw( SocketException ) {

    checkClosed();

    try{
        ensureCreated();
        return this->impl->getOption( SocketOptions::SOCKET_OPTION_RCVBUF );
    }
    DECAF_CATCH_RETHROW( SocketException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, SocketException )
    DECAF_CATCHALL_THROW( SocketException )
}

////////////////////////////////////////////////////////////////////////////////
void ServerSocket::setReceiveBufferSize( int size )
    throw( SocketException, decaf::lang::exceptions::IllegalArgumentException ) {

    checkClosed();

    if( size <= 0 ) {
        throw IllegalArgumentException(
            __FILE__, __LINE__, "Buffer size given was invalid: %d", size );
    }

    try{
        ensureCreated();
        this->impl->setOption( SocketOptions::SOCKET_OPTION_RCVBUF, size );
    }
    DECAF_CATCH_RETHROW( SocketException )
    DECAF_CATCH_RETHROW( IllegalArgumentException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, SocketException )
    DECAF_CATCHALL_THROW( SocketException )
}

////////////////////////////////////////////////////////////////////////////////
bool ServerSocket::getReuseAddress() const throw( SocketException ) {

    checkClosed();

    try{
        ensureCreated();
        return this->impl->getOption( SocketOptions::SOCKET_OPTION_REUSEADDR ) == 0 ? false : true;
    }
    DECAF_CATCH_RETHROW( SocketException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, SocketException )
    DECAF_CATCHALL_THROW( SocketException )
}

////////////////////////////////////////////////////////////////////////////////
void ServerSocket::setReuseAddress( bool reuse ) throw( SocketException ) {

    checkClosed();

    try{
        ensureCreated();
        this->impl->setOption( SocketOptions::SOCKET_OPTION_REUSEADDR, reuse ? 1 : 0 );
    }
    DECAF_CATCH_RETHROW( SocketException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, SocketException )
    DECAF_CATCHALL_THROW( SocketException )
}

////////////////////////////////////////////////////////////////////////////////
int ServerSocket::getSoTimeout() const throw( SocketException ) {

    checkClosed();

    try{
        ensureCreated();
        return this->impl->getOption( SocketOptions::SOCKET_OPTION_TIMEOUT );
    }
    DECAF_CATCH_RETHROW( SocketException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, SocketException )
    DECAF_CATCHALL_THROW( SocketException )
}

////////////////////////////////////////////////////////////////////////////////
void ServerSocket::setSoTimeout( int timeout )
    throw( SocketException, decaf::lang::exceptions::IllegalArgumentException ) {

    checkClosed();

    if( timeout < 0 ) {
        throw IllegalArgumentException(
            __FILE__, __LINE__, "Socket timeout given was invalid: %d", timeout );
    }

    try{
        ensureCreated();
        this->impl->setOption( SocketOptions::SOCKET_OPTION_TIMEOUT, timeout );
    }
    DECAF_CATCH_RETHROW( SocketException )
    DECAF_CATCH_RETHROW( IllegalArgumentException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, SocketException )
    DECAF_CATCHALL_THROW( SocketException )
}

////////////////////////////////////////////////////////////////////////////////
int ServerSocket::getLocalPort() const {

    if( this->impl == NULL ) {
        return -1;
    }

    return this->impl->getLocalPort();
}

////////////////////////////////////////////////////////////////////////////////
void ServerSocket::setSocketImplFactory( SocketImplFactory* factory )
    throw( decaf::io::IOException,
           decaf::net::SocketException ) {

    if( Socket::factory != NULL ) {
        throw SocketException(
            __FILE__, __LINE__, "A SocketInplFactory was already set." );
    }

    Socket::factory = factory;
}

////////////////////////////////////////////////////////////////////////////////
std::string ServerSocket::toString() const {

    std::ostringstream str;

    str << "ServerSocket[";

    if( !isBound() ) {
        str << "unbound]";
    } else {

    }

    return str.str();
}

////////////////////////////////////////////////////////////////////////////////
void ServerSocket::checkClosed() const throw( decaf::io::IOException ) {
    if( this->closed ) {
        throw IOException( __FILE__, __LINE__, "ServerSocket already closed." );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ServerSocket::ensureCreated() const throw( decaf::io::IOException ) {

    try{
        if( !this->created ) {
            this->impl->create();
            this->created = true;
        }
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
int ServerSocket::getDefaultBacklog() {
    return 50;
}
