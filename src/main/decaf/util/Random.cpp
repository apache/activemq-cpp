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

#include "Random.h"

#include <decaf/lang/System.h>

using namespace decaf;
using namespace decaf::util;
using namespace decaf::lang;

unsigned long long Random::multiplier = 0x5deece66dLL;

////////////////////////////////////////////////////////////////////////////////
Random::Random() {
    setSeed( System::currentTimeMillis() );
}

////////////////////////////////////////////////////////////////////////////////
Random::Random( unsigned long long seed ) {
    setSeed( seed );
}

////////////////////////////////////////////////////////////////////////////////
bool Random::nextBoolean() {
    return next( 1 ) != 0;
}

////////////////////////////////////////////////////////////////////////////////
void Random::nextBytes( std::vector<unsigned char>& buf ) {
    int rand = 0;
    std::size_t count = 0, loop = 0;
    while( count < buf.size() ) {
        if( loop == 0 ) {
            rand = nextInt();
            loop = 3;
        } else {
            loop--;
        }
        buf[count++] = (unsigned char)rand;
        rand >>= 8;
    }
}

////////////////////////////////////////////////////////////////////////////////
double Random::nextDouble() {
    long long divisor = 1LL;
    divisor <<= 31;
    divisor <<= 22;
    return ((((long long) next(26) << 27) + next(27)) / (double) divisor);
}

////////////////////////////////////////////////////////////////////////////////
float Random::nextFloat() {
    return ( next(24) / 16777216.0f );
}

////////////////////////////////////////////////////////////////////////////////
double Random::nextGaussian() {

    if( haveNextNextGaussian ) {
        // if X1 has been returned, return the second Gaussian
        haveNextNextGaussian = false;
        return nextNextGaussian;
    }

    double v1, v2, s;
    do {
        // Generates two independent random variables U1, U2
        v1 = 2 * nextDouble() - 1;
        v2 = 2 * nextDouble() - 1;
        s = v1 * v1 + v2 * v2;
    } while( s >= 1 );
    double norm = std::sqrt( -2 * std::log(s) / s );
    // should that not be norm instead of multiplier ?
    nextNextGaussian = v2 * norm;
    haveNextNextGaussian = true;
    // should that not be norm instead of multiplier ?
    return v1 * norm;
}

////////////////////////////////////////////////////////////////////////////////
int Random::nextInt() {
    return next( 32 );
}

////////////////////////////////////////////////////////////////////////////////
int Random::nextInt( int n ) throw( exceptions::IllegalArgumentException ) {

    if( n > 0 ) {

        if( ( n & -n ) == n ) {
            return (int)( ( n * (long long)next( 31 ) ) >> 31 );
        }

        int bits, val;

        do {
            bits = next( 31 );
            val = bits % n;
        } while( bits - val + ( n - 1 ) < 0 );

        return val;
    }

    throw exceptions::IllegalArgumentException(
        __FILE__, __LINE__,
        "Value passed cannot be less than or equal to zero." );
}

////////////////////////////////////////////////////////////////////////////////
long long Random::nextLong() {
    return ( (long long) next(32) << 32 ) + next(32);
}

////////////////////////////////////////////////////////////////////////////////
void Random::setSeed( unsigned long long seed ) {
    unsigned long long mask = 1ULL;
    mask <<= 31;
    mask <<= 17;
    this->seed = ( seed ^ multiplier ) & ( mask - 1 );
    haveNextNextGaussian = false;
}

////////////////////////////////////////////////////////////////////////////////
int Random::next( int bits ) {
    long long mask = 1L;
    mask <<= 31;
    mask <<= 17;
    seed = ( seed * multiplier + 0xbL ) & ( mask - 1 );
    // was: return (int) (seed >>> (48 - bits));
    return (int)( seed >> (48 - bits) );
}
