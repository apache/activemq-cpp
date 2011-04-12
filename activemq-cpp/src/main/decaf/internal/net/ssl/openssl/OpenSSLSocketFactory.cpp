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

#include "OpenSSLSocketFactory.h"

#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/exceptions/UnsupportedOperationException.h>

#include <decaf/internal/net/ssl/openssl/OpenSSLSocket.h>
#include <decaf/internal/net/ssl/openssl/OpenSSLParameters.h>
#include <decaf/internal/net/ssl/openssl/OpenSSLContextSpi.h>

#include <memory>

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#endif

using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::io;
using namespace decaf::net;
using namespace decaf::net::ssl;
using namespace decaf::internal;
using namespace decaf::internal::net;
using namespace decaf::internal::net::ssl;
using namespace decaf::internal::net::ssl::openssl;

////////////////////////////////////////////////////////////////////////////////
OpenSSLSocketFactory::OpenSSLSocketFactory( OpenSSLContextSpi* parent ) : SSLSocketFactory(), parent( parent ) {

    if( parent == NULL ) {
        throw NullPointerException(
            __FILE__, __LINE__, "Parent Pointer was NULL." );
    }
}

////////////////////////////////////////////////////////////////////////////////
OpenSSLSocketFactory::~OpenSSLSocketFactory() {
}

////////////////////////////////////////////////////////////////////////////////
std::vector<std::string> OpenSSLSocketFactory::getDefaultCipherSuites() {

    return std::vector<std::string>();
}

////////////////////////////////////////////////////////////////////////////////
std::vector<std::string> OpenSSLSocketFactory::getSupportedCipherSuites() {

    return std::vector<std::string>();
}

////////////////////////////////////////////////////////////////////////////////
Socket* OpenSSLSocketFactory::createSocket() {

    try{

#ifdef HAVE_OPENSSL
        // Create a new SSL object for the Socket then create a new unconnected Socket.
        SSL_CTX* ctx = static_cast<SSL_CTX*>( this->parent->getOpenSSLCtx() );
        std::auto_ptr<OpenSSLParameters> parameters( new OpenSSLParameters( ctx ) );
        return new OpenSSLSocket( parameters.release() );
#else
        return NULL;
#endif
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
Socket* OpenSSLSocketFactory::createSocket( const decaf::net::InetAddress* host, int port ) {

    try{

#ifdef HAVE_OPENSSL
        // Create a new SSL object for the Socket then create a new unconnected Socket.
        SSL_CTX* ctx = static_cast<SSL_CTX*>( this->parent->getOpenSSLCtx() );
        std::auto_ptr<OpenSSLParameters> parameters( new OpenSSLParameters( ctx ) );
        std::auto_ptr<SSLSocket> socket( new OpenSSLSocket( parameters.release(), host, port ) );
        return socket.release();
#else
        return NULL;
#endif
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
Socket* OpenSSLSocketFactory::createSocket( const decaf::net::InetAddress* host, int port,
                                            const decaf::net::InetAddress* ifAddress, int localPort ) {

    try{

#ifdef HAVE_OPENSSL
        // Create a new SSL object for the Socket then create a new unconnected Socket.
        SSL_CTX* ctx = static_cast<SSL_CTX*>( this->parent->getOpenSSLCtx() );
        std::auto_ptr<OpenSSLParameters> parameters( new OpenSSLParameters( ctx ) );
        std::auto_ptr<SSLSocket> socket(
            new OpenSSLSocket( parameters.release(), host, port, ifAddress, localPort ) );
        return socket.release();
#else
        return NULL;
#endif
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
Socket* OpenSSLSocketFactory::createSocket( const std::string& hostname, int port ) {

    try{

#ifdef HAVE_OPENSSL
        // Create a new SSL object for the Socket then create a new unconnected Socket.
        SSL_CTX* ctx = static_cast<SSL_CTX*>( this->parent->getOpenSSLCtx() );
        std::auto_ptr<OpenSSLParameters> parameters( new OpenSSLParameters( ctx ) );
        std::auto_ptr<SSLSocket> socket( new OpenSSLSocket( parameters.release(), hostname, port ) );
        return socket.release();
#else
        return NULL;
#endif
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
Socket* OpenSSLSocketFactory::createSocket( const std::string& hostname, int port,
                                            const InetAddress* ifAddress, int localPort ) {

    try{

#ifdef HAVE_OPENSSL
        // Create a new SSL object for the Socket then create a new unconnected Socket.
        SSL_CTX* ctx = static_cast<SSL_CTX*>( this->parent->getOpenSSLCtx() );
        std::auto_ptr<OpenSSLParameters> parameters( new OpenSSLParameters( ctx ) );
        std::auto_ptr<SSLSocket> socket(
            new OpenSSLSocket( parameters.release(), hostname, port, ifAddress, localPort ) );
        return socket.release();
#else
        return NULL;
#endif
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
Socket* OpenSSLSocketFactory::createSocket( Socket* socket DECAF_UNUSED,
                                            std::string host DECAF_UNUSED,
                                            int port DECAF_UNUSED,
                                            bool autoClose DECAF_UNUSED ) {

    throw UnsupportedOperationException(
        __FILE__, __LINE__, "Wrapped Sockets not yet Supported." );
}
