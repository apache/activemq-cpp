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
    #include <openssl/x509.h>
    #include <openssl/x509v3.h>
    #include <openssl/bio.h>
#endif

#include <apr_strings.h>

#include <decaf/net/SocketImpl.h>
#include <decaf/io/IOException.h>
#include <decaf/net/SocketException.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/exceptions/IndexOutOfBoundsException.h>
#include <decaf/internal/net/SocketFileDescriptor.h>
#include <decaf/internal/net/ssl/openssl/OpenSSLSocketException.h>
#include <decaf/internal/net/ssl/openssl/OpenSSLSocketInputStream.h>
#include <decaf/internal/net/ssl/openssl/OpenSSLSocketOutputStream.h>

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
#else
        void* ssl;
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

#ifdef HAVE_OPENSSL
        static int verifyCallback( int verified, X509_STORE_CTX* store ) {

            if( !verified ) {

                // Trap debug info here about why the Certificate failed to validate.
            }

            return verified;
        }
#endif

    };

}}}}}

////////////////////////////////////////////////////////////////////////////////
OpenSSLSocket::OpenSSLSocket( void* ssl ) :
    SSLSocket(), data( new SocketData() ), input( NULL ), output( NULL ) {

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
        delete input;
        delete output;
    }
    DECAF_CATCH_NOTHROW( Exception )
    DECAF_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void OpenSSLSocket::connect( const std::string& host, int port, int timeout )
    throw( decaf::io::IOException,
           decaf::lang::exceptions::IllegalArgumentException ) {

    try{

#ifdef HAVE_OPENSSL
        SSLSocket::connect( host, port, timeout );
        if( isConnected() ) {

            BIO* bio = BIO_new( BIO_s_socket() );
            if( !bio ) {
                throw SocketException(
                    __FILE__, __LINE__, "Failed to create SSL IO Bindings");
            }

            const SocketFileDescriptor* fd =
                dynamic_cast<const SocketFileDescriptor*>( this->impl->getFileDescriptor() );

            if( fd == NULL ) {
                throw SocketException(
                    __FILE__, __LINE__, "Invalid File Descriptor returned from Socket" );
            }

            BIO_set_fd( bio, (int)fd->getValue(), BIO_NOCLOSE );
            SSL_set_bio( this->data->ssl, bio, bio );

            // Since we are a client we want to enforce peer verification, we set a
            // callback so we can collect data on why a verify failed for debugging.
            SSL_set_verify( this->data->ssl, SSL_VERIFY_PEER, SocketData::verifyCallback );

            int result = SSL_connect( this->data->ssl );

            // Checks the error status, when things go right we still perform a deeper
            // check on the provided certificate to ensure that it matches the host name
            // that we connected to, this prevents someone from using any certificate
            // signed by a signing authority that we trust.
            switch( SSL_get_error( this->data->ssl, result ) ) {
                case SSL_ERROR_NONE:
                    verifyServerCert( host );
                    return;
                case SSL_ERROR_SSL:
                case SSL_ERROR_ZERO_RETURN:
                case SSL_ERROR_SYSCALL:
                    SSLSocket::close();
                    throw OpenSSLSocketException( __FILE__, __LINE__ );
            }
        }
#else
        throw SocketException( __FILE__, __LINE__, "Not Supported" );
#endif
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_RETHROW( IllegalArgumentException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void OpenSSLSocket::close() throw( decaf::io::IOException ) {

    try{

        if( isClosed() ) {
            return;
        }

        SSLSocket::close();

        if( this->input != NULL ) {
            this->input->close();
        }
        if( this->output != NULL ) {
            this->output->close();
        }
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
decaf::io::InputStream* OpenSSLSocket::getInputStream() throw( decaf::io::IOException ) {

    checkClosed();

    try{
        if( this->input == NULL ) {
            this->input = new OpenSSLSocketInputStream( this );
        }

        return this->input;
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
decaf::io::OutputStream* OpenSSLSocket::getOutputStream() throw( decaf::io::IOException ) {

    checkClosed();

    try{
        if( this->output == NULL ) {
            this->output = new OpenSSLSocketOutputStream( this );
        }

        return this->output;
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void OpenSSLSocket::shutdownInput() throw( decaf::io::IOException ) {

    throw SocketException(
        __FILE__, __LINE__, "Not supported for SSL Sockets" );
}

////////////////////////////////////////////////////////////////////////////////
void OpenSSLSocket::shutdownOutput() throw( decaf::io::IOException ) {

    throw SocketException(
        __FILE__, __LINE__, "Not supported for SSL Sockets" );
}

////////////////////////////////////////////////////////////////////////////////
void OpenSSLSocket::setOOBInline( bool value DECAF_UNUSED ) throw( SocketException ) {

    throw SocketException(
        __FILE__, __LINE__, "Not supported for SSL Sockets" );
}

////////////////////////////////////////////////////////////////////////////////
void OpenSSLSocket::sendUrgentData( int data DECAF_UNUSED ) throw( decaf::io::IOException ) {

    throw SocketException(
        __FILE__, __LINE__, "Not supported for SSL Sockets" );
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

#ifdef HAVE_OPENSSL
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
#else
        throw SocketException( __FILE__, __LINE__, "Not Supported" );
#endif
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

#ifdef HAVE_OPENSSL
        int remaining = length;

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
#else
        throw SocketException( __FILE__, __LINE__, "Not Supported" );
#endif
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
int OpenSSLSocket::available() {

    try{

#ifdef HAVE_OPENSSL
        if( !isClosed() ) {
            return SSL_pending( this->data->ssl );
        }
#else
        throw SocketException( __FILE__, __LINE__, "Not Supported" );
#endif

        return -1;
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void OpenSSLSocket::verifyServerCert( const std::string& serverName ) {

#ifdef HAVE_OPENSSL
    X509* cert = SSL_get_peer_certificate( this->data->ssl );

    if( cert == NULL ) {
        this->close();
        throw OpenSSLSocketException(
            __FILE__, __LINE__,
            "No server certificate for verify for host: %s", serverName.c_str() );
    }

    class Finalizer {
    private:

        X509* cert;

    public:

        Finalizer( X509* cert ) : cert( cert ) {
        }

        ~Finalizer() {
            if( cert != NULL ) {
                X509_free( cert );
            }
        }
    };

    // Store the Certificate to be cleaned up when the method returns
    Finalizer final( cert );

    // We check the extensions first since newer x509v3 Certificates are recommended
    // to store the FQDN in the dsnName field of the subjectAltName extension.  If we
    // don't find it there then we can check the commonName field which is where older
    // Certificates placed the FQDN.
    int extensions = X509_get_ext_count( cert );

    for( int ix = 0; ix < extensions; ix++ ) {

        X509_EXTENSION* extension = X509_get_ext( cert, ix );
        const char* extensionName = OBJ_nid2sn( OBJ_obj2nid( X509_EXTENSION_get_object( extension ) ) );

        if( apr_strnatcmp( "subjectAltName", extensionName ) == 0 ) {

            const X509V3_EXT_METHOD* method = X509V3_EXT_get( extension );
            if( method == NULL ) {
                break;
            }

            const unsigned char* data = extension->value->data;
            STACK_OF(CONF_VALUE)* confValue =
                method->i2v( method, method->d2i( NULL, &data, extension->value->length ), NULL );

            CONF_VALUE* value = NULL;

            for( int iy = 0; iy < sk_CONF_VALUE_num( confValue ); iy++ ) {
                value = sk_CONF_VALUE_value( confValue, iy );
                if( ( apr_strnatcmp( value->name, "DNS" ) == 0 ) &&
                      apr_strnatcmp( value->value, serverName.c_str() ) == 0 ) {

                    // Found it.
                    return;
                }
            }
        }
    }

    X509_NAME* subject = X509_get_subject_name( cert );
    char buffer[256];

    if( subject != NULL && X509_NAME_get_text_by_NID( subject, NID_commonName, buffer, 256 ) > 0 ) {
        buffer[255] = 0;
        if( apr_strnatcmp( buffer, serverName.c_str() ) == 0 ) {
            return;
        }
    }

    // We got here so no match to serverName in the Certificate
    throw OpenSSLSocketException(
        __FILE__, __LINE__, "Server Certificate Name doesn't match the URI Host Name value." );
#endif
}
