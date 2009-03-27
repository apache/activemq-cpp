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

#include "Long.h"
#include <decaf/lang/Character.h>
#include <sstream>

using namespace decaf;
using namespace decaf::lang;

////////////////////////////////////////////////////////////////////////////////
Long::Long( long long value ) {
    this->value = value;
}

////////////////////////////////////////////////////////////////////////////////
Long::Long( const std::string& value ) throw( exceptions::NumberFormatException ) {
    this->value = parseLong( value );
}

////////////////////////////////////////////////////////////////////////////////
int Long::compareTo( const Long& l ) const {
    return this->value < l.value ? -1 : this->value == l.value ? 0 : 1;
}

////////////////////////////////////////////////////////////////////////////////
int Long::compareTo( const long long& l ) const {
    return this->value < l ? -1 : this->value == l ? 0 : 1;
}

////////////////////////////////////////////////////////////////////////////////
int Long::bitCount( long long value ) {

    if( value == 0 ) {
        return 0;
    }

    unsigned long long uvalue = (unsigned long long)value;

    uvalue = (uvalue & 0x5555555555555555LL) + ((uvalue >> 1) & 0x5555555555555555LL);
    uvalue = (uvalue & 0x3333333333333333LL) + ((uvalue >> 2) & 0x3333333333333333LL);
    // adjust for 64-bit integer
    unsigned int i = (unsigned int)( (uvalue >> 32) + uvalue );
    i = (i & 0x0F0F0F0F) + ((i >> 4) & 0x0F0F0F0F);
    i = (i & 0x00FF00FF) + ((i >> 8) & 0x00FF00FF);
    i = (i & 0x0000FFFF) + ((i >> 16) & 0x0000FFFF);
    return i;
}

////////////////////////////////////////////////////////////////////////////////
Long Long::decode( const std::string& value )
    throw ( exceptions::NumberFormatException ) {

    int length = (int)value.length(), i = 0;
    if( length == 0 ) {
        throw exceptions::NumberFormatException(
            __FILE__, __LINE__,
            "Long::decode - Zero length string given." );
    }

    char firstDigit = value[i];
    bool negative = firstDigit == '-';
    if( negative ) {
        if( length == 1 ) {
            throw exceptions::NumberFormatException(
                __FILE__, __LINE__,
                "Long::decode - Invalid length string given.",
                value.c_str() );
        }
        firstDigit = value[++i];
    }

    int base = 10;
    if( firstDigit == '0' ) {
        if( ++i == length ) {
            return valueOf( 0LL );
        }
        if( ( firstDigit = value[i] ) == 'x' || firstDigit == 'X' ) {
            if ( i == length ) {
                throw exceptions::NumberFormatException(
                    __FILE__, __LINE__,
                    "Long::decode - Invalid length string given.",
                    value.c_str() );
            }
            i++;
            base = 16;
        } else {
            base = 8;
        }
    } else if( firstDigit == '#' ) {
        if( i == length ) {
            throw exceptions::NumberFormatException(
                __FILE__, __LINE__,
                "Long::decode - Invalid length string given.",
                value.c_str() );
        }
        i++;
        base = 16;
    }

    long long result = parse( value, i, base, negative );
    return valueOf( result );
}

////////////////////////////////////////////////////////////////////////////////
long long Long::highestOneBit( long long value ) {

    if( value == 0 ) {
        return 0;
    }

    unsigned long long uvalue = (unsigned long long)value;

    uvalue |= (uvalue >> 1);
    uvalue |= (uvalue >> 2);
    uvalue |= (uvalue >> 4);
    uvalue |= (uvalue >> 8);
    uvalue |= (uvalue >> 16);
    uvalue |= (uvalue >> 32);
    return ( uvalue & ~( uvalue >> 1 ) );
}

////////////////////////////////////////////////////////////////////////////////
long long Long::lowestOneBit( long long value ) {
    if( value == 0 ) {
        return 0;
    }

    unsigned long long uvalue = (unsigned long long)value;
    return ( uvalue & (-1 * uvalue) );
}

////////////////////////////////////////////////////////////////////////////////
int Long::numberOfLeadingZeros( long long value ) {

    if( value == 0 ) {
        return 0;
    }

    unsigned long long uvalue = (unsigned long long)value;

    value |= value >> 1;
    value |= value >> 2;
    value |= value >> 4;
    value |= value >> 8;
    value |= value >> 16;
    value |= value >> 32;
    return Long::bitCount( ~uvalue );
}

////////////////////////////////////////////////////////////////////////////////
int Long::numberOfTrailingZeros( long long value ) {
    if( value == 0 ) {
        return 0;
    }

    unsigned long long uvalue = (unsigned long long)value;
    return Long::bitCount( (uvalue & (-1 * uvalue)) - 1 );
}

////////////////////////////////////////////////////////////////////////////////
long long Long::parseLong( const std::string& value )
    throw ( exceptions::NumberFormatException ) {

    return Long::parseLong( value, 10 );
}

////////////////////////////////////////////////////////////////////////////////
long long Long::parseLong( const std::string& value, int radix )
    throw ( exceptions::NumberFormatException ) {

    if( radix < Character::MIN_RADIX ||
        radix > Character::MAX_RADIX ) {
        throw exceptions::NumberFormatException(
            __FILE__, __LINE__,
            "Long::parseLong - Given Radix is out of range." );
    }

    int length = (int)value.length();
    int i = 0;

    if( length == 0 ) {
        throw exceptions::NumberFormatException(
            __FILE__, __LINE__,
            "Long::parseLong - Zero length string is illegal." );
    }

    bool negative = value[i] == '-';
    if( negative && ++i == length ) {
        throw exceptions::NumberFormatException(
            __FILE__, __LINE__,
            "Long::parseLong - Only a minus given, string is invalid." );
    }

    return Long::parse( value, i, radix, negative );
}

////////////////////////////////////////////////////////////////////////////////
long long Long::parse( const std::string& value, int offset,
                       int radix, bool negative )
    throw ( exceptions::NumberFormatException ) {

    long long max = Long::MIN_VALUE / radix;
    long long result = 0;
    long long length = value.length();

    while( offset < length ) {
        int digit = Character::digit( value[offset++], radix );

        if( digit == -1 ) {
            throw exceptions::NumberFormatException(
                __FILE__, __LINE__,
                "Long::parseLong - String contains no digit characters." );
        }

        if( max > result ) {
            throw exceptions::NumberFormatException(
                __FILE__, __LINE__,
                "Long::parseLong - Parsed value greater than max for radix." );
        }

        long long next = result * radix - digit;

        if( next > result ) {
            throw exceptions::NumberFormatException(
                __FILE__, __LINE__,
                "Long::parseLong - Only a minus given, string is invalid." );
        }

        result = next;
    }

    if( !negative ) {
        result = -result;
        if( result < 0 ) {
            throw exceptions::NumberFormatException(
                __FILE__, __LINE__,
                "Long::parseLong - Value less than zero, but no minus sign." );
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
long long Long::reverseBytes( long long value ) {

    if( value == 0 ) {
        return 0;
    }

    unsigned long long uvalue = (unsigned long long)value;

    long long b7 = (uvalue >> 56);
    long long b6 = (uvalue >> 40) & 0xFF00ULL;
    long long b5 = (uvalue >> 24) & 0xFF0000ULL;
    long long b4 = (uvalue >> 8) & 0xFF000000ULL;
    long long b3 = (uvalue & 0xFF000000ULL) << 8;
    long long b2 = (uvalue & 0xFF0000ULL) << 24;
    long long b1 = (uvalue & 0xFF00ULL) << 40;
    long long b0 = (uvalue << 56);
    return ( b0 | b1 | b2 | b3 | b4 | b5 | b6 | b7 );
}

////////////////////////////////////////////////////////////////////////////////
long long Long::reverse( long long value ) {

    if( value == 0 ) {
        return 0;
    }

    unsigned long long uvalue = (unsigned long long)value;

    // From Hacker's Delight, 7-1, Figure 7-1
    uvalue = ( uvalue & 0x5555555555555555ULL ) << 1 | (uvalue >> 1) &
             0x5555555555555555ULL;
    uvalue = ( uvalue & 0x3333333333333333ULL ) << 2 | (uvalue >> 2) &
             0x3333333333333333ULL;
    uvalue = ( uvalue & 0x0F0F0F0F0F0F0F0FULL ) << 4 | (uvalue >> 4) &
             0x0F0F0F0F0F0F0F0FULL;

    return reverseBytes( uvalue );
}

////////////////////////////////////////////////////////////////////////////////
long long Long::rotateLeft( long long value, int distance ) {
    unsigned long long i = (unsigned long long)value;
    int j = distance & 0x1F;
    return ( i << j ) | ( i >> (-j & 0x1F ) );
}

////////////////////////////////////////////////////////////////////////////////
long long Long::rotateRight( long long value, int distance ) {
    unsigned long long i = (unsigned long long)value;
    int j = distance & 0x1F;
    return ( i >> j ) | ( i << (-j & 0x1F ) );
}

////////////////////////////////////////////////////////////////////////////////
int Long::signum( long long value ) {
    return ( value == 0 ? 0 : ( value < 0 ? -1 : 1 ) );
}

////////////////////////////////////////////////////////////////////////////////
std::string Long::toString() const {
    return Long::toString( this->value, 10 );
}

////////////////////////////////////////////////////////////////////////////////
std::string Long::toString( long long value ) {
    return Long::toString( value, 10 );
}

////////////////////////////////////////////////////////////////////////////////
std::string Long::toString( long long value, int radix ) {

    if( radix < Character::MIN_RADIX || radix > Character::MAX_RADIX ) {
        radix = 10;
    }

    if( value == 0 ) {
        return "0";
    }

    int count = 2;
    long long j = value;
    bool negative = value < 0;

    if( !negative ) {
        count = 1;
        j = -value;
    }

    while( (value /= radix) != 0 ) {
        count++;
    }

    // Save length and allocate a new buffer for the string, add one
    // more for the null character.
    int length = count;
    char* buffer = new char[length + 1];

    do {
        int ch = (int)( 0 - ( j % radix ) );
        if( ch > 9 ) {
            ch = ch - 10 + 'a';
        } else {
            ch += '0';
        }
        buffer[--count] = (char)ch;
    } while( (j /= radix) != 0 );

    if( negative ) {
        buffer[0] = '-';
    }

    // Ensure there's a null
    buffer[length] = 0;
    std::string result( &buffer[0] );
    delete [] buffer;

    return result;
}

////////////////////////////////////////////////////////////////////////////////
std::string Long::toBinaryString( long long value ) {

    int count = 1;
    long long j = value;

    if( value < 0 ) {
        count = 32;
    } else {
        while ( (j >>= 1) != 0) {
            count++;
        }
    }

    // Save length and allocate a new buffer for the string, add one
    // more for the null character.
    int length = count;
    char* buffer = new char[length + 1];

    do {
        buffer[--count] = (char)( (value & 1) + '0' );
        value >>= 1;
    } while( count > 0 );

    // Ensure there's a null
    buffer[length] = 0;
    std::string result( &buffer[0] );
    delete [] buffer;

    return result;
}

////////////////////////////////////////////////////////////////////////////////
std::string Long::toOctalString( long long value ) {

    int count = 1;
    long long j = value;

    if( value < 0 ) {
        count = 11;
    } else {
        while ( (j >>= 3) != 0 ) {
            count++;
        }
    }

    // Save length and allocate a new buffer for the string, add one
    // more for the null character.
    int length = count;
    char* buffer = new char[length + 1];

    do {
        buffer[--count] = (char)( (value & 7) + '0' );
        value >>= 3;
    } while( count > 0 );

    // Ensure there's a null
    buffer[length] = 0;
    std::string result( &buffer[0] );
    delete [] buffer;

    return result;
}

////////////////////////////////////////////////////////////////////////////////
std::string Long::toHexString( long long value ) {

    int count = 1;
    long long j = value;

    if( value < 0 ) {
        count = 8;
    } else {
        while( (j >>= 4) != 0 ) {
            count++;
        }
    }

    // Save length and allocate a new buffer for the string, add one
    // more for the null character.
    int length = count;
    char* buffer = new char[length + 1];

    do {
        int t = (int)( value & 15 );
        if( t > 9 ) {
            t = t - 10 + 'a';
        } else {
            t += '0';
        }
        buffer[--count] = (char)t;
        value >>= 4;
    } while( count > 0 );

    // Ensure there's a null
    buffer[length] = 0;
    std::string result( &buffer[0] );
    delete [] buffer;

    return result;
}

////////////////////////////////////////////////////////////////////////////////
Long Long::valueOf( const std::string& value )
    throw ( exceptions::NumberFormatException ) {

    return Long( Long::parseLong( value ) );
}

////////////////////////////////////////////////////////////////////////////////
Long Long::valueOf( const std::string& value, int radix )
    throw ( exceptions::NumberFormatException ) {

    return Long( Long::parseLong( value, radix ) );
}
