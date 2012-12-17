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

#include "SecureRandom.h"

#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>

#ifdef HAVE_PTHREAD_H
#include <decaf/internal/security/unix/SecureRandomImpl.h>
#else
#include <decaf/internal/security/windows/SecureRandomImpl.h>
#endif

using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::security;
using namespace decaf::internal;
using namespace decaf::internal::security;

////////////////////////////////////////////////////////////////////////////////
SecureRandom::SecureRandom() : secureRandom(new SecureRandomImpl()) {
}

////////////////////////////////////////////////////////////////////////////////
SecureRandom::SecureRandom( const std::vector<unsigned char>& seed ) :
    secureRandom(new SecureRandomImpl()) {

    if( !seed.empty() ) {
        this->secureRandom->providerSetSeed( &seed[0], (int)seed.size() );
    }
}

////////////////////////////////////////////////////////////////////////////////
SecureRandom::SecureRandom( const unsigned char* seed, int size ) :
    secureRandom(new SecureRandomImpl()) {

    if( seed == NULL ) {
        throw NullPointerException( __FILE__, __LINE__, "Seed buffer pointer passed was NULL" );
    }

    if( size < 0 ) {
        throw IllegalArgumentException( __FILE__, __LINE__, "Passed buffer size was negative." );
    }

    if( size > 0 ) {
        this->secureRandom->providerSetSeed( seed, size );
    }
}

////////////////////////////////////////////////////////////////////////////////
SecureRandom::~SecureRandom() {
}

////////////////////////////////////////////////////////////////////////////////
void SecureRandom::nextBytes( std::vector<unsigned char>& buf ) {
    if( !buf.empty() ) {
        this->secureRandom->providerNextBytes( &buf[0], (int)buf.size() );
    }
}

////////////////////////////////////////////////////////////////////////////////
void SecureRandom::nextBytes( unsigned char* buf, int size ) {

    if( buf == NULL ) {
        throw NullPointerException( __FILE__, __LINE__, "Buffer pointer passed was NULL" );
    }

    if( size < 0 ) {
        throw IllegalArgumentException( __FILE__, __LINE__, "Passed buffer size was negative." );
    }

    if( size > 0 ) {
        this->secureRandom->providerNextBytes( buf, size );
    }
}

////////////////////////////////////////////////////////////////////////////////
void SecureRandom::setSeed( unsigned long long seed ) {

    if( seed == 0 ) {
        return;
    }

    unsigned char byteSeed[] = { (unsigned char)( ( seed >> 56 ) & 0xFF ),
                                 (unsigned char)( ( seed >> 48 ) & 0xFF ),
                                 (unsigned char)( ( seed >> 40 ) & 0xFF ),
                                 (unsigned char)( ( seed >> 32 ) & 0xFF ),
                                 (unsigned char)( ( seed >> 24 ) & 0xFF ),
                                 (unsigned char)( ( seed >> 16 ) & 0xFF ),
                                 (unsigned char)( ( seed >> 8 ) & 0xFF ),
                                 (unsigned char)( ( seed ) & 0xFF ) };

    this->secureRandom->providerSetSeed( byteSeed, 8 );
}

////////////////////////////////////////////////////////////////////////////////
void SecureRandom::setSeed( const std::vector<unsigned char>& seed ) {
    if( !seed.empty() ) {
        this->secureRandom->providerSetSeed( &seed[0], (int)seed.size() );
    }
}

////////////////////////////////////////////////////////////////////////////////
void SecureRandom::setSeed( const unsigned char* seed, int size ) {

    if( seed == NULL ) {
        throw NullPointerException( __FILE__, __LINE__, "Buffer pointer passed was NULL" );
    }

    if( size < 0 ) {
        throw IllegalArgumentException( __FILE__, __LINE__, "Passed buffer size was negative." );
    }

    if( size > 0 ) {
        this->secureRandom->providerSetSeed( seed, size );
    }
}

////////////////////////////////////////////////////////////////////////////////
int SecureRandom::next( int numBits ) {

    if( numBits < 0 ) {
        numBits = 0;
    } else if(numBits > 32 ) {
        numBits = 32;
    }

    int bytes = ( numBits + 7 ) / 8;
    unsigned char* next = new unsigned char[bytes];
    unsigned int ret = 0;

    this->nextBytes( next, bytes );
    for( int i = 0; i < bytes; i++ ) {
        ret = ( next[i] & 0xFF ) | ( ret << 8 );
    }

    ret = ret >> ( ( bytes * 8 ) - numBits );

    delete [] next;

    return ret;
}
