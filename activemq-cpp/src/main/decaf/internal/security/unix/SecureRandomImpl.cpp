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

#include <decaf/internal/AprPool.h>

#include <memory>
#include <apr_file_io.h>

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

        AprPool pool;
        apr_file_t* randFile;
        std::auto_ptr<Random> random;

        SRNGData() : pool(), randFile( NULL ), random() {
        }

    };

}}}

////////////////////////////////////////////////////////////////////////////////
SecureRandomImpl::SecureRandomImpl() : config( new SRNGData() ) {

    try{

        const char* files[] = { "/dev/urandom", "/dev/random" };
        int index = 0;
        apr_status_t result = APR_SUCCESS;

        do {
            // Attempt to find an OS source for secure random bytes.
            result = apr_file_open( &config->randFile, files[index],
                                    APR_READ, APR_OS_DEFAULT,
                                    config->pool.getAprPool() );
        } while( index < 2 && result != APR_SUCCESS );

        // Defaults to the Decaf version.
        if( result != APR_SUCCESS ) {
            this->config->random.reset( new Random() );
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

    if( this->config->randFile != NULL ) {

        apr_status_t result = APR_EOF;
        apr_size_t bytesRead = 0;

        // Instruct APR to read it all.
        result = apr_file_read_full( this->config->randFile, (void*)bytes, numBytes, &bytesRead );

        // Since the dev random files are special OS random sources we should never get
        // an EOF or other error, if so its bad.
        if( result != APR_SUCCESS ) {
            throw RuntimeException(
                __FILE__, __LINE__,
                "Unexpected error while reading random bytes from system resources." );
        }

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
