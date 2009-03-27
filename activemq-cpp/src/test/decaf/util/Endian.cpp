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

#include "Endian.h"
#include <decaf/util/Config.h>
#include <string.h>

using namespace decaf;
using namespace decaf::util;

////////////////////////////////////////////////////////////////////////////////
void Endian::byteSwap(unsigned char* data, int dataLength) {

    #ifdef WORDS_BIGENDIAN
        return;
    #endif

    for (int i = 0; i<dataLength/2; i++) {
        unsigned char temp = data[i];
        data[i] = data[dataLength-1-i];
        data[dataLength-1-i] = temp;
    }
}

////////////////////////////////////////////////////////////////////////////////
unsigned char Endian::byteSwap( unsigned char value ){

    #ifdef WORDS_BIGENDIAN
        return value;
    #endif

    return value;
}

////////////////////////////////////////////////////////////////////////////////
unsigned short Endian::byteSwap( unsigned short value ){

    #ifdef WORDS_BIGENDIAN
        return value;
    #endif

    return (((unsigned short)value & 0xFF00 ) >> 8 ) |
           (((unsigned short)value & 0x00FF ) << 8 );
}

////////////////////////////////////////////////////////////////////////////////
unsigned int Endian::byteSwap( unsigned int value ){

    #ifdef WORDS_BIGENDIAN
        return value;
    #endif

    return (((unsigned int)value & 0xFF000000 ) >> 24 ) |
           (((unsigned int)value & 0x00FF0000 ) >> 8 )  |
           (((unsigned int)value & 0x0000FF00 ) << 8 )  |
           (((unsigned int)value & 0x000000FF ) << 24 );
}

////////////////////////////////////////////////////////////////////////////////
unsigned long long Endian::byteSwap( unsigned long long value ){

    #ifdef WORDS_BIGENDIAN
        return value;
    #endif

    return (((unsigned long long)value & 0xFF00000000000000ULL ) >> 56 ) |
           (((unsigned long long)value & 0x00FF000000000000ULL ) >> 40 ) |
           (((unsigned long long)value & 0x0000FF0000000000ULL ) >> 24 ) |
           (((unsigned long long)value & 0x000000FF00000000ULL ) >> 8 )  |
           (((unsigned long long)value & 0x00000000FF000000ULL ) << 8 )  |
           (((unsigned long long)value & 0x0000000000FF0000ULL ) << 24 ) |
           (((unsigned long long)value & 0x000000000000FF00ULL ) << 40 ) |
           (((unsigned long long)value & 0x00000000000000FFULL ) << 56 );
}

////////////////////////////////////////////////////////////////////////////////
float Endian::byteSwap( float value ){

    #ifdef WORDS_BIGENDIAN
        return value;
    #endif

    unsigned int lvalue = 0;
    memcpy( &lvalue, &value, sizeof( float ) );
    lvalue = byteSwap( lvalue );
    memcpy( &value, &lvalue, sizeof( unsigned int ) );
    return value;
}

////////////////////////////////////////////////////////////////////////////////
double Endian::byteSwap( double value ){

    #ifdef WORDS_BIGENDIAN
        return value;
    #endif

    unsigned long long lvalue = 0;
    memcpy( &lvalue, &value, sizeof( double ) );
    lvalue = byteSwap( lvalue );
    memcpy( &value, &lvalue, sizeof( unsigned long long ) );
    return value;
}

