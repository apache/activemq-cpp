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

#include "OpenSSLServerSocket.h"

#ifdef HAVE_OPENSSL
    #include <openssl/ssl.h>
    #include <openssl/x509.h>
    #include <openssl/x509v3.h>
    #include <openssl/bio.h>
#endif

#include <decaf/net/SocketImpl.h>
#include <decaf/io/IOException.h>
#include <decaf/net/SocketException.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/exceptions/IndexOutOfBoundsException.h>
#include <decaf/internal/net/SocketFileDescriptor.h>
#include <decaf/internal/net/ssl/openssl/OpenSSLParameters.h>
#include <decaf/internal/net/ssl/openssl/OpenSSLSocket.h>
#include <decaf/internal/net/ssl/openssl/OpenSSLSocketException.h>

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
namespace decaf {
namespace internal {
namespace net {
namespace ssl {
namespace openssl {

    class ServerSocketData {
    public:

        ServerSocketData() {
        }

        ~ServerSocketData() {
            try{
            } catch(...) {}
        }

    };

}}}}}

////////////////////////////////////////////////////////////////////////////////
OpenSSLServerSocket::OpenSSLServerSocket( OpenSSLParameters* parameters ) :
    SSLServerSocket(), data( new ServerSocketData() ), parameters( parameters ) {

    if( parameters == NULL ) {
        throw NullPointerException(
            __FILE__, __LINE__, "The OpenSSL Parameters object instance passed was NULL." );
    }
}

////////////////////////////////////////////////////////////////////////////////
OpenSSLServerSocket::~OpenSSLServerSocket() {

    try{
        delete data;
        delete parameters;
    }
    DECAF_CATCH_NOTHROW( Exception )
    DECAF_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
std::vector<std::string> OpenSSLServerSocket::getSupportedCipherSuites() const {
    return this->parameters->getSupportedCipherSuites();
}

////////////////////////////////////////////////////////////////////////////////
std::vector<std::string> OpenSSLServerSocket::getSupportedProtocols() const {
    return this->parameters->getSupportedProtocols();
}

////////////////////////////////////////////////////////////////////////////////
std::vector<std::string> OpenSSLServerSocket::getEnabledCipherSuites() const {
    return this->parameters->getEnabledCipherSuites();
}

////////////////////////////////////////////////////////////////////////////////
void OpenSSLServerSocket::setEnabledCipherSuites( const std::vector<std::string>& suites ) {
    this->parameters->setEnabledCipherSuites( suites );
}

////////////////////////////////////////////////////////////////////////////////
std::vector<std::string> OpenSSLServerSocket::getEnabledProtocols() const {
    return this->parameters->getEnabledProtocols();
}

////////////////////////////////////////////////////////////////////////////////
void OpenSSLServerSocket::setEnabledProtocols( const std::vector<std::string>& protocols ) {
    this->parameters->setEnabledProtocols( protocols );
}

////////////////////////////////////////////////////////////////////////////////
bool OpenSSLServerSocket::getNeedClientAuth() const {
    return this->parameters->getNeedClientAuth();
}

////////////////////////////////////////////////////////////////////////////////
void OpenSSLServerSocket::setNeedClientAuth( bool value ) {
    this->parameters->setNeedClientAuth( value );
}

////////////////////////////////////////////////////////////////////////////////
bool OpenSSLServerSocket::getWantClientAuth() const {
    return this->parameters->getWantClientAuth();
}

////////////////////////////////////////////////////////////////////////////////
void OpenSSLServerSocket::setWantClientAuth( bool value ) {
    this->parameters->setWantClientAuth( value );
}

////////////////////////////////////////////////////////////////////////////////
Socket* OpenSSLServerSocket::accept() {

    try{

        std::auto_ptr<OpenSSLSocket> socket( new OpenSSLSocket( this->parameters->clone() ) );
        this->implAccept( socket.get() );
        socket->startHandshake();

        return socket.release();
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    DECAF_CATCHALL_THROW( IOException )
}
