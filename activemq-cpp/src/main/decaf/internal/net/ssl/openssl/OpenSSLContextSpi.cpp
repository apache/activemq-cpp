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

#include "OpenSSLContextSpi.h"

#include <decaf/net/SocketFactory.h>
#include <decaf/net/ssl/SSLParameters.h>
#include <decaf/security/SecureRandom.h>
#include <decaf/security/KeyManagementException.h>
#include <decaf/lang/Thread.h>
#include <decaf/lang/Pointer.h>
#include <decaf/lang/ArrayPointer.h>
#include <decaf/lang/exceptions/IllegalStateException.h>
#include <decaf/util/concurrent/Mutex.h>
#include <decaf/internal/net/ssl/openssl/OpenSSLSocketException.h>
#include <decaf/internal/net/ssl/openssl/OpenSSLSocketFactory.h>

#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/rand.h>
#define SSL_LOCK_MUTEX CRYPTO_LOCK
#else
#define SSL_LOCK_MUTEX 1
#endif

using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::net;
using namespace decaf::net::ssl;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace decaf::security;
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

    class ContextData {
    public:

        Mutex monitor;
        Pointer<SocketFactory> clientSocketFactory;
        Pointer<SecureRandom> random;
        std::string password;

        static Mutex* locks;
        static std::string defaultCipherList;

#ifdef HAVE_OPENSSL
        SSL_CTX* openSSLContext;
#endif

    public:

        ContextData( int size ) {
            ContextData::locks = new Mutex[size];
        }

        ~ContextData() {

#ifdef HAVE_OPENSSL
            try{
                SSL_CTX_free( this->openSSLContext );
            } catch(...) {}
#endif
            delete [] locks;
        }

        // Used to allow OpenSSL to tell us when to lock / unlock a mutex for it.
        static void lockCallback( int mode, int type, const char *, int ) {

            if( mode & SSL_LOCK_MUTEX ) {
                locks[type].lock();
            } else {
                locks[type].unlock();
            }
        }

        // Returns to OpenSSL the password for a Certificate.
        static int passwordCallback( char* buffer, int size, int, void* data ) {

            ContextData* ctxData = static_cast<ContextData*>( data );

            if( !ctxData->password.empty() ) {

                size = size < (int)ctxData->password.size() ? size : (int)ctxData->password.size();
                strncpy( buffer, ctxData->password.c_str(), size );
                return size;
            }

            return 0;
        }

        static unsigned long getThreadId() {
            return (unsigned long)Thread::getId();
        }

    };

    Mutex* ContextData::locks = NULL;
    std::string ContextData::defaultCipherList = "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH";

}}}}}

////////////////////////////////////////////////////////////////////////////////
OpenSSLContextSpi::OpenSSLContextSpi() : data( NULL ) {
}

////////////////////////////////////////////////////////////////////////////////
OpenSSLContextSpi::~OpenSSLContextSpi() {
    try{

#ifdef HAVE_OPENSSL
        // Clean up all the OpenSSL resources.
        CRYPTO_set_locking_callback( 0 );
        EVP_cleanup();
#endif

        delete this->data;
    }
    DECAF_CATCH_NOTHROW( Exception )
    DECAF_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void OpenSSLContextSpi::providerInit( SecureRandom* random ) {

    if( random == NULL ) {
        throw NullPointerException(
            __FILE__, __LINE__, "SecureRandom instance passed was NULL." );
    }

    try{

#ifdef HAVE_OPENSSL

        // General library initialization.
        CRYPTO_malloc_init();
        SSL_load_error_strings();
        SSL_library_init();
        OpenSSL_add_all_algorithms();

        // Initialize our data object and store the RNG for later.
        this->data = new ContextData( CRYPTO_num_locks() );
        this->data->random.reset( random );
        this->data->openSSLContext = SSL_CTX_new( SSLv23_method() );

        // Setup the Crypto Library Thread callbacks.
        CRYPTO_set_id_callback( &ContextData::getThreadId );
        CRYPTO_set_locking_callback( &ContextData::lockCallback );

        // Load the Library default CA paths and Context Options.
        SSL_CTX_set_default_verify_paths( this->data->openSSLContext );
        SSL_CTX_set_options( this->data->openSSLContext, SSL_OP_ALL | SSL_OP_NO_SSLv2 );
        SSL_CTX_set_mode( this->data->openSSLContext, SSL_MODE_AUTO_RETRY );

        // The Password Callback for cases where we need to open a Cert.
        SSL_CTX_set_default_passwd_cb( this->data->openSSLContext, &ContextData::passwordCallback );
        SSL_CTX_set_default_passwd_cb_userdata( this->data->openSSLContext, (void*)this->data );

        // Sets the Default set of allowed Ciphers, this could be overridden by the system
        // property: decaf.net.ssl.CipherSuites
        if( SSL_CTX_set_cipher_list( this->data->openSSLContext, ContextData::defaultCipherList.c_str() ) != 1 ) {
            throw OpenSSLSocketException( __FILE__, __LINE__ );
        }

        // Here we load the configured KeyStore and TrustStore files
        std::string keyStorePath = System::getenv( "decaf.net.ssl.keyStore" );
        std::string keyStoreFile = System::getenv( "decaf.net.ssl.keyStoreFile" );
        std::string keyStorePassword = System::getenv( "decaf.net.ssl.keyStorePassword" );
        std::string trustStorePath = System::getenv( "decaf.net.ssl.trustStore" );
        std::string trustStoreFile = System::getenv( "decaf.net.ssl.trustStoreFile" );
        std::string trustStorePassword = System::getenv( "decaf.net.ssl.trustStorePassword" );

        // Now seed the OpenSSL RNG.
        std::vector<unsigned char> seed( 128 );
        random->nextBytes( seed );
        RAND_seed( (void*)( &seed[0] ), (int)seed.size() );

#endif
    }
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCH_RETHROW( KeyManagementException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, KeyManagementException )
    DECAF_CATCHALL_THROW( KeyManagementException )
}

////////////////////////////////////////////////////////////////////////////////
SocketFactory* OpenSSLContextSpi::providerGetSocketFactory() {

    try{

        synchronized( &( this->data->monitor ) ) {
            if( this->data->clientSocketFactory == NULL ) {
                this->data->clientSocketFactory.reset( new OpenSSLSocketFactory( this ) );
            }
        }

        return this->data->clientSocketFactory.get();
    }
    DECAF_CATCH_RETHROW( IllegalStateException )
    DECAF_CATCH_RETHROW( Exception )
    DECAF_CATCHALL_THROW( Exception )
}

////////////////////////////////////////////////////////////////////////////////
std::vector<std::string> OpenSSLContextSpi::getDefaultCipherSuites() {

    return std::vector<std::string>();
}

////////////////////////////////////////////////////////////////////////////////
std::vector<std::string> OpenSSLContextSpi::getSupportedCipherSuites() {

    return std::vector<std::string>();
}

////////////////////////////////////////////////////////////////////////////////
void* OpenSSLContextSpi::getOpenSSLCtx() {

    return (void*)this->data->openSSLContext;
}
