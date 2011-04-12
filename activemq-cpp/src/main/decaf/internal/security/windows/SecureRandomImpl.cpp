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

#include "SecureRandomImpl.h"

#include <decaf/lang/Exception.h>
#include <decaf/lang/exceptions/RuntimeException.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>
#include <decaf/util/Random.h>

#undef _WIN32_WINNT
#define _WIN32_WINNT 0x0500

#include <windows.h>
#include <wincrypt.h>

using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::util;
using namespace decaf::security;
using namespace decaf::internal;
using namespace decaf::internal::security;

////////////////////////////////////////////////////////////////////////////////
namespace decaf {
namespace internal {
namespace security {

    class SRNGData {
    private:

        SRNGData( const SRNGData& );
        SRNGData operator= ( const SRNGData& );

    public:

        std::auto_ptr<Random> random;

        SRNGData() : random() {
        }

    };

}}}

////////////////////////////////////////////////////////////////////////////////
SecureRandomImpl::SecureRandomImpl() {

    this->config = new SRNGData();

    try{

        HCRYPTPROV provider;
        int result = CryptAcquireContext( &provider, NULL, NULL, PROV_DSS, CRYPT_VERIFYCONTEXT );

        // Defaults to the Decaf version.
        if( result == 0 ) {
            this->config->random.reset( new Random() );
        } else {
            CryptReleaseContext( provider, 0 );
        }
    }
    DECAF_CATCH_RETHROW( Exception )
    DECAF_CATCHALL_THROW( Exception )
}

////////////////////////////////////////////////////////////////////////////////
SecureRandomImpl::~SecureRandomImpl() {
    try{
        delete this->config;
    }
    DECAF_CATCH_NOTHROW( Exception )
    DECAF_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void SecureRandomImpl::providerSetSeed( const unsigned char* seed, int size ) {

    // Only seed the default random, the other sources don't need a seed.
    if( this->config->random.get() != NULL ) {

        for( int i = 0; i < size; i++ ) {
            this->config->random->setSeed( (long long)seed[i] );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void SecureRandomImpl::providerNextBytes( unsigned char* bytes, int numBytes ) {

    if( bytes == NULL ) {
        throw NullPointerException(
            __FILE__, __LINE__, "Byte Buffer passed cannot be NULL." );
    }

    if( numBytes < 0 ) {
        throw IllegalArgumentException(
            __FILE__, __LINE__, "Number of bytes to read was negative: %d", numBytes );
    }

    if( this->config->random.get() == NULL ) {

        HCRYPTPROV provider;

        int result;

        result = CryptAcquireContext( &provider, NULL, NULL, PROV_DSS, CRYPT_VERIFYCONTEXT );

        if ( result == 0 ) {
            throw RuntimeException(
                __FILE__, __LINE__, "Failed to acquire the system cryptographic provider." );
        }

        result = CryptGenRandom( provider, numBytes, bytes );

        if( result == 0 ) {
            throw RuntimeException(
                __FILE__, __LINE__, "Failed to get random bytes from the cryptographic provider." );
        }

        CryptReleaseContext( provider, 0 );

    } else {
        this->config->random->nextBytes( bytes, numBytes );
    }
}

////////////////////////////////////////////////////////////////////////////////
unsigned char* SecureRandomImpl::providerGenerateSeed( int numBytes ) {

    if( numBytes == 0 ) {
        return NULL;
    }

    unsigned char* buffer = new unsigned char[numBytes];
    providerNextBytes( buffer, numBytes );
    return buffer;
}
