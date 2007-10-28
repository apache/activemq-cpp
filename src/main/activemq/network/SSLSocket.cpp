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

#include "SSLSocket.h"
#include "SSLSocketInputStream.h"
#include "SSLSocketOutputStream.h"
#include "SSLError.h"
#include "SocketError.h"

#ifdef AMQ_HAVE_OPENSSL

using namespace activemq::network;
using namespace activemq::io;

///////////////////////////////////////////////////////////////////////////////
SSLSocket::SSLSocket() throw (SocketException)
:
TcpSocket(),
ctx( SSL_CTX_new( SSLv3_client_method() )),
ssl( 0 ),
verify_peer( false )
{
    if( !ctx )
    throw SocketException( __FILE__, __LINE__,
            "Failed to allocate SSL context");
}

///////////////////////////////////////////////////////////////////////////////
SSLSocket::SSLSocket( SocketHandle socketHandle ) throw (SocketException)
:
TcpSocket(),
ctx( SSL_CTX_new( SSLv3_client_method() )),
ssl( 0 ),
verify_peer( false )
{
    if( !ctx )
    throw SocketException( __FILE__, __LINE__,
            "Failed to allocate SSL context - %s",
            SSLError::getErrorString().c_str());

    initializeSSL( socketHandle );

    try {
        this->socketHandle = socketHandle;
        this->inputStream = new SSLSocketInputStream( socketHandle, ssl );
        this->outputStream = new SSLSocketOutputStream( socketHandle, ssl );
    }
    AMQ_CATCH_RETHROW( SocketException )
    AMQ_CATCHALL_THROW( SocketException )
}

///////////////////////////////////////////////////////////////////////////////
SSLSocket::~SSLSocket()
{
    // Pretend shutdown, we do not want the destructor to block.
    TcpSocket::close();

    if( ssl != 0 ) {
        if( SSL_get_shutdown( ssl ) == 0 )
        SSL_set_shutdown( ssl, SSL_SENT_SHUTDOWN | SSL_RECEIVED_SHUTDOWN );
        SSL_shutdown( ssl );
    }

    if( ssl ) SSL_free( ssl );
    if( ctx ) SSL_CTX_free( ctx );
}

///////////////////////////////////////////////////////////////////////////////
void SSLSocket::connect(const char* host, int port) throw ( SocketException )
{
    if( ssl ) {
        throw SocketException( __FILE__, __LINE__,
                "SSLSocket::connect - Socket already connected."
                " host: %s, port: %d", host, port );
    }

    TcpSocket::connect (host, port);
    if( isConnected() ) {
        while( true ) {

            int result = SSL_connect(ssl);

            switch (SSL_get_error (ssl, result))
            {
                case SSL_ERROR_NONE:
                // Apart from verification we are done.
                verifyPeerCertificate( host );
                return;

                case SSL_ERROR_SSL:
                case SSL_ERROR_ZERO_RETURN:
                TcpSocket::close();
                throw SocketException( __FILE__, __LINE__,
                        SSLError::getErrorString().c_str());

                case SSL_ERROR_WANT_READ:
                case SSL_ERROR_WANT_WRITE:
                // Repeat the operation.
                break;

                case SSL_ERROR_SYSCALL:
                TcpSocket::close();
                throw SocketException( __FILE__, __LINE__,
                        SocketError::getErrorString().c_str() );
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void SSLSocket::verifyPeerCertificate( const char* host )
throw( SocketException )
{
    // Verify the certificate name.
    if( verify_peer ) {
        X509 *peer_cert = SSL_get_peer_certificate( ssl );

        if( !peer_cert ) {
            TcpSocket::close();
            throw SocketException( __FILE__, __LINE__,
                    "No peer certificate for verify");
        }

        X509_NAME *subject = X509_get_subject_name( peer_cert );

        std::string need_name (verify_name);
        if( need_name.empty() ) {
            need_name = host;
        }

        bool match = false;
        std::string peer_name;

        int i = -1;
        do {
            i = X509_NAME_get_index_by_NID( subject, NID_commonName, i );
            if( i >= 0 ) {
                X509_NAME_ENTRY *name = X509_NAME_get_entry( subject, i );

                unsigned char *cn;
                if( ASN1_STRING_to_UTF8( &cn, name->value ) >= 0 ) {
                    peer_name = reinterpret_cast<char*> (cn);
                    std::free( cn );

                    if( peer_name == need_name ) {
                        match = true;
                    }
                }
            }
        }while( i >= 0 && !match );

        if( !match ) {
            TcpSocket::close();

            if( peer_name.length() ) {
                throw SocketException( __FILE__, __LINE__,
                        "Peer certificate mismatch for %s"
                        " - peer name %s",
                        verify_name.c_str(),
                        peer_name.c_str() );
            } else {
                throw SocketException( __FILE__, __LINE__,
                        "Unable to find certificate CN");
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void SSLSocket::initialize() throw( SocketException )
{
    // Destroy the input stream.
    if( inputStream != NULL ) {
        delete inputStream;
        inputStream = NULL;
    }

    // Destroy the output stream.
    if( outputStream != NULL ) {
        delete outputStream;
        outputStream = NULL;
    }

    if( ssl ) {
        SSL_free( ssl );
        ssl = 0;
    }
    initializeSSL( socketHandle );

    // Create an input/output stream for this socket.
    inputStream = new SSLSocketInputStream( socketHandle, ssl );
    outputStream = new SSLSocketOutputStream( socketHandle, ssl );
}

///////////////////////////////////////////////////////////////////////////////
void SSLSocket::initializeSSL( SocketHandle handle )
throw (SocketException)
{
    ssl = SSL_new( ctx );
    if( !ssl ) {
        throw SocketException( __FILE__, __LINE__,
                "Failed to allocate SSL - %s",
                SSLError::getErrorString().c_str());
    }

    BIO* bio = BIO_new( BIO_s_socket() );
    if( !bio ) {
        throw SocketException( __FILE__, __LINE__,
                "Failed to create BIO for SSL");
    }

    BIO_set_fd( bio, handle, BIO_NOCLOSE );
    SSL_set_bio( ssl, bio, bio );
}

///////////////////////////////////////////////////////////////////////////////
void SSLSocket::close() throw( cms::CMSException )
{
    // Close the input stream.
    if( inputStream != NULL ) {
        inputStream->close();
    }

    // Close the output stream.
    if( outputStream != NULL ) {
        outputStream->close();
    }

    if( ssl ) {
        // The ssl can not yet be free'd, it is still use in the streams.
        SSL_shutdown(ssl);
    }
    TcpSocket::close ();
}

///////////////////////////////////////////////////////////////////////////////
void SSLSocket::setCAFilePath( std::string const& file,
        std::string const& path )
throw( SocketException )
{
    if( file.length() || path.length() ) {

        const char *c_file = file.length() ? file.c_str() : 0;
        const char *c_path = path.length() ? path.c_str() : 0;

        if( !SSL_CTX_load_verify_locations( ctx, c_file, c_path )) {
            throw SocketException( __FILE__, __LINE__,
                    "Failed to load verify locations - %s",
                    SSLError::getErrorString().c_str());
        }
    }

    // Should we do this automatically? 
    if( !SSL_CTX_set_default_verify_paths( ctx )) {
        throw SocketException( __FILE__, __LINE__,
                "Failed to load default verify locations - %s",
                SSLError::getErrorString().c_str());
    }
}

///////////////////////////////////////////////////////////////////////////////
void SSLSocket::setCertFile( std::string const& cert_file,
        std::string const& key_file )
throw( SocketException )
{
    if( cert_file.length() ) {
        if( !SSL_CTX_use_certificate_file( ctx, cert_file.c_str (),
                        SSL_FILETYPE_PEM )) {
            throw SocketException( __FILE__, __LINE__,
                    "Failed to load certificate %s - %s",
                    cert_file.c_str(),
                    SSLError::getErrorString().c_str());
        }

        const char *c_key_file
        = key_file.length() ? key_file.c_str() : cert_file.c_str();

        if( !SSL_CTX_use_PrivateKey_file( ctx, c_key_file,
                        SSL_FILETYPE_PEM )) {
            throw SocketException( __FILE__, __LINE__,
                    "Failed to load private key %s - %s",
                    c_key_file,
                    SSLError::getErrorString().c_str());
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void SSLSocket::setPassword( std::string const& password )
throw( SocketException )
{
    if( password.length() ) {
        this->password = password;
        void* data = static_cast<void*> (&this->password);

        SSL_CTX_set_default_passwd_cb( ctx, password_cb );
        SSL_CTX_set_default_passwd_cb_userdata( ctx, data );
    }
}

///////////////////////////////////////////////////////////////////////////////
int SSLSocket::password_cb( char* buffer, int size, int, void* data)
{
    std::string *password = static_cast<std::string*>( data );
    std::strncpy( buffer, password->c_str(), size );
    return password->length();
}

///////////////////////////////////////////////////////////////////////////////
void SSLSocket::setVerifyPeer( bool value, std::string const& name )
throw( SocketException )
{
    verify_peer = value;
    if( verify_peer ) {
        verify_name = name;
        SSL_CTX_set_verify( ctx, SSL_VERIFY_PEER, 0);
    }
}

///////////////////////////////////////////////////////////////////////////////
void SSLSocket::setCiphers( std::string const& ciphers )
throw( SocketException )
{
    if( ciphers.length() ) {
        if( SSL_CTX_set_cipher_list( ctx, ciphers.c_str() ) == 0) {
            throw SocketException( __FILE__, __LINE__,
                    "Failed to set ciphers %s - %s",
                    ciphers.c_str(),
                    SSLError::getErrorString().c_str());
        }
    }
}

#endif /* AMQ_HAVE_OPENSSL */
