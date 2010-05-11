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

#include "OpenSSLSocket.h"

#ifdef HAVE_OPENSSL
    #include <openssl/ssl.h>
#endif

#include <decaf/net/SocketImpl.h>
#include <decaf/io/IOException.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/exceptions/IndexOutOfBoundsException.h>
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

    class SocketData {
    public:

#ifdef HAVE_OPENSSL
        SSL* ssl;
#endif
        bool needsClientAuth;
        bool wantsClientAuth;

    public:

        SocketData() : ssl( NULL ), needsClientAuth( false ), wantsClientAuth( false ) {
        }

        ~SocketData() {
            try{
#ifdef HAVE_OPENSSL
                if( ssl ) {
                    SSL_free( ssl );
                }
#endif
            } catch(...) {}
        }

    };

}}}}}

////////////////////////////////////////////////////////////////////////////////
OpenSSLSocket::OpenSSLSocket( void* ssl ) : SSLSocket(), data( new SocketData() ) {

    if( ssl == NULL ) {
        throw NullPointerException(
            __FILE__, __LINE__, "The OpenSSL SSL object instance passed was NULL." );
    }

#ifdef HAVE_OPENSSL
    this->data->ssl = static_cast<SSL*>( ssl );
#endif
}

////////////////////////////////////////////////////////////////////////////////
OpenSSLSocket::~OpenSSLSocket() {
    try{

        SSLSocket::close();

#ifdef HAVE_OPENSSL
        if( this->data->ssl ) {
            SSL_set_shutdown( this->data->ssl, SSL_SENT_SHUTDOWN | SSL_RECEIVED_SHUTDOWN );
            SSL_shutdown( this->data->ssl );
        }
#endif

        delete data;
    }
    DECAF_CATCH_NOTHROW( Exception )
    DECAF_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
std::vector<std::string> OpenSSLSocket::getSupportedCipherSuites() const {

    return std::vector<std::string>();
}

////////////////////////////////////////////////////////////////////////////////
std::vector<std::string> OpenSSLSocket::getSupportedProtocols() const {

    return std::vector<std::string>();
}

////////////////////////////////////////////////////////////////////////////////
std::vector<std::string> OpenSSLSocket::getEnabledCipherSuites() const {

    return std::vector<std::string>();
}

////////////////////////////////////////////////////////////////////////////////
void OpenSSLSocket::setEnabledCipherSuites( const std::vector<std::string>& suites DECAF_UNUSED ) {

}

////////////////////////////////////////////////////////////////////////////////
std::vector<std::string> OpenSSLSocket::getEnabledProtocols() const {

    return std::vector<std::string>();
}

////////////////////////////////////////////////////////////////////////////////
void OpenSSLSocket::setEnabledProtocols( const std::vector<std::string>& protocols DECAF_UNUSED ) {

}

////////////////////////////////////////////////////////////////////////////////
bool OpenSSLSocket::getWantClientAuth() const {
    return false;
}

////////////////////////////////////////////////////////////////////////////////
void OpenSSLSocket::setWantClientAuth( bool value DECAF_UNUSED ) {
}

////////////////////////////////////////////////////////////////////////////////
bool OpenSSLSocket::getNeedClientAuth() const {
    return false;
}

////////////////////////////////////////////////////////////////////////////////
void OpenSSLSocket::setNeedClientAuth( bool value DECAF_UNUSED ) {
}

////////////////////////////////////////////////////////////////////////////////
int OpenSSLSocket::read( unsigned char* buffer, int size, int offset, int length ) {

    try{
        if( this->isClosed() ){
            throw IOException(
                __FILE__, __LINE__, "The Stream has been closed" );
        }

        if( this->isInputShutdown() == true ) {
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

        // Read data from the socket.
        int result = SSL_read( this->data->ssl, buffer + offset, length );

        switch( SSL_get_error( this->data->ssl, result ) ) {
            case SSL_ERROR_NONE:
                return result;
            case SSL_ERROR_ZERO_RETURN:
                if( !isClosed() ) {
                    this->shutdownInput();
                    return -1;
                }
            default:
                throw OpenSSLSocketException( __FILE__, __LINE__ );
        }
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void OpenSSLSocket::write( const unsigned char* buffer, int size, int offset, int length ) {

    try{

        if( length == 0 ) {
            return;
        }

        if( buffer == NULL ) {
            throw NullPointerException(
                __FILE__, __LINE__,
                "TcpSocketOutputStream::write - passed buffer is null" );
        }

        if( isClosed() ) {
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

        std::size_t remaining = length;

        while( remaining > 0 && !isClosed() ) {

            int written = SSL_write( this->data->ssl, buffer + offset, remaining );

            switch( SSL_get_error( this->data->ssl, written ) ) {
                case SSL_ERROR_NONE:
                    offset += written;
                    remaining -= written;
                    break;
                case SSL_ERROR_ZERO_RETURN:
                    throw SocketException(
                        __FILE__, __LINE__,
                        "The connection was broken unexpectedly." );
                default:
                    throw OpenSSLSocketException( __FILE__, __LINE__ );
            }
        }
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
int OpenSSLSocket::available() {

    try{

        if( !isClosed() ) {
            return SSL_pending( this->data->ssl );
        }

        return -1;
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}
