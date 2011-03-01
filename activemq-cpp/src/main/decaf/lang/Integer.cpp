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

#ifdef _WIN32
#pragma warning( disable: 4146 )
#endif

#include <decaf/lang/Integer.h>
#include <decaf/lang/Character.h>
#include <sstream>

using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
Integer::Integer( int value ) {
    this->value = value;
}

////////////////////////////////////////////////////////////////////////////////
Integer::Integer( const std::string& value ) throw( exceptions::NumberFormatException ) {
    this->value = parseInt( value );
}

////////////////////////////////////////////////////////////////////////////////
int Integer::compareTo( const Integer& i ) const {
    return this->value < i.value ? -1 : this->value == i.value ? 0 : 1;
}

////////////////////////////////////////////////////////////////////////////////
int Integer::compareTo( const int& i ) const {
    return this->value < i ? -1 : this->value == i ? 0 : 1;
}

////////////////////////////////////////////////////////////////////////////////
int Integer::bitCount( int value ) {

    if( value == 0 ) {
        return 0;
    }

    unsigned int uvalue = (unsigned int)value;

    // 32-bit recursive reduction using SWAR...
    // but first step is mapping 2-bit values
    // into sum of 2 1-bit values in sneaky way
    uvalue -= ((uvalue >> 1) & 0x55555555);
    uvalue = (((uvalue >> 2) & 0x33333333) + (uvalue & 0x33333333));
    uvalue = (((uvalue >> 4) + uvalue) & 0x0F0F0F0F);
    uvalue += (uvalue >> 8);
    uvalue += (uvalue >> 16);
    return(uvalue & 0x0000003F);
}

////////////////////////////////////////////////////////////////////////////////
int Integer::reverseBytes( int value ) {

    if( value == 0 ) {
        return 0;
    }

    unsigned int uvalue = (unsigned int)value;

    unsigned int b3 = uvalue >> 24;
    unsigned int b2 = (uvalue >> 8) & 0xFF00;
    unsigned int b1 = (uvalue & 0xFF00) << 8;
    unsigned int b0 = uvalue << 24;
    return (b0 | b1 | b2 | b3);
}

////////////////////////////////////////////////////////////////////////////////
int Integer::reverse( int value ) {

    if( value == 0 ) {
        return 0;
    }

    unsigned int uvalue = (unsigned int)value;

    uvalue = (((uvalue & 0xAAAAAAAA) >> 1) | ((uvalue & 0x55555555) << 1));
    uvalue = (((uvalue & 0xCCCCCCCC) >> 2) | ((uvalue & 0x33333333) << 2));
    uvalue = (((uvalue & 0xF0F0F0F0) >> 4) | ((uvalue & 0x0F0F0F0F) << 4));
    uvalue = (((uvalue & 0xFF00FF00) >> 8) | ((uvalue & 0x00FF00FF) << 8));
    return ((uvalue >> 16) | (uvalue << 16));
}

////////////////////////////////////////////////////////////////////////////////
std::string Integer::toString() const {
    return Integer::toString( this->value, 10 );
}

////////////////////////////////////////////////////////////////////////////////
std::string Integer::toString( int value ) {
    return Integer::toString( value, 10 );
}

////////////////////////////////////////////////////////////////////////////////
std::string Integer::toString( int value, int radix ) {

    if( radix < Character::MIN_RADIX || radix > Character::MAX_RADIX ) {
        radix = 10;
    }

    if( value == 0 ) {
        return "0";
    }

    int count = 2, j = value;
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
        int ch = 0 - ( j % radix );
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
std::string Integer::toBinaryString( int value ) {

    int count = 1;
    int j = value;

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
std::string Integer::toOctalString( int value ) {

    int count = 1, j = value;

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
std::string Integer::toHexString( int value ) {

    int count = 1;
    int j = value;

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
        int t = value & 15;
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
int Integer::parseInt( const std::string& value )
    throw ( exceptions::NumberFormatException ) {

    return Integer::parseInt( value, 10 );
}

////////////////////////////////////////////////////////////////////////////////
int Integer::parseInt( const std::string& value, int radix )
    throw ( exceptions::NumberFormatException ) {

    if( radix < Character::MIN_RADIX ||
        radix > Character::MAX_RADIX ) {
        throw NumberFormatException(
            __FILE__, __LINE__,
            "Integer:decode - Invalid radix" );
    }

    int length = (int)value.length(), i = 0;
    if( length == 0 ) {
        throw NumberFormatException(
            __FILE__, __LINE__,
            "Integer:decode - Invalid: zero length string");
    }

    bool negative = value[i] == '-';
    if( negative && ++i == length ) {
        throw NumberFormatException(
            __FILE__, __LINE__,
            "Integer:decode - Invalid only a minus sign given");
    }

    return Integer::parse( value, i, radix, negative );
}

////////////////////////////////////////////////////////////////////////////////
Integer Integer::valueOf( const std::string& value )
    throw ( exceptions::NumberFormatException ) {

    return Integer( Integer::parseInt( value ) );
}

////////////////////////////////////////////////////////////////////////////////
Integer Integer::valueOf( const std::string& value, int radix )
    throw ( exceptions::NumberFormatException ) {

    return Integer( Integer::parseInt( value, radix ) );
}

////////////////////////////////////////////////////////////////////////////////
Integer Integer::decode( const std::string& value )
    throw ( exceptions::NumberFormatException ) {

    int length = (int)value.length(), i = 0;
    if( length == 0 ) {
        throw NumberFormatException(
            __FILE__, __LINE__,
            "Integer:decode - Invalid zero size string");
    }

    char firstDigit = value[i];
    bool negative = firstDigit == '-';
    if( negative ) {
        if( length == 1 ) {
            throw NumberFormatException(
                __FILE__, __LINE__,
                "Integer:decode - Invalid zero string, minus only");
        }

        firstDigit = value[++i];
    }

    int base = 10;
    if( firstDigit == '0' ) {
        if( ++i == length ) {
            return valueOf( 0 );
        }

        if( ( firstDigit = value[i] ) == 'x' || firstDigit == 'X' ) {
            if( i == length ) {
                throw NumberFormatException(
                    __FILE__, __LINE__,
                    "Integer:decode - Invalid zero string, minus only");
            }
            i++;
            base = 16;
        } else {
            base = 8;
        }
    } else if( firstDigit == '#' ) {
        if( i == length ) {
            throw NumberFormatException(
                __FILE__, __LINE__,
                "Integer:decode - Invalid zero string, minus only");
        }
        i++;
        base = 16;
    }

    int result = parse( value, i, base, negative );
    return valueOf( result );
}

////////////////////////////////////////////////////////////////////////////////
int Integer::parse( const std::string& value, int offset,
                    int radix, bool negative )
    throw ( exceptions::NumberFormatException ) {

    int max = Integer::MIN_VALUE / radix;
    int result = 0, length = (int)value.size();

    while( offset < length ) {
        int digit = Character::digit( value[offset++], radix );
        if( digit == -1 ) {
            throw NumberFormatException(
                __FILE__, __LINE__,
                "Integer::parse - number string is invalid: ",
                value.c_str() );
        }
        if( max > result ) {
            throw NumberFormatException(
                __FILE__, __LINE__,
                "Integer::parse - number string is invalid: ",
                value.c_str() );
        }
        int next = result * radix - digit;
        if( next > result ) {
            throw NumberFormatException(
                __FILE__, __LINE__,
                "Integer::parse - number string is invalid: ",
                value.c_str() );
        }
        result = next;
    }
    if( !negative ) {
        result = -result;
        if( result < 0 ) {
            throw NumberFormatException(
                __FILE__, __LINE__,
                "Integer::parse - number string is invalid: ",
                value.c_str() );
        }
    }
    return result;
}

////////////////////////////////////////////////////////////////////////////////
int Integer::highestOneBit( int value ) {

    if( value == 0 ) {
        return 0;
    }

    unsigned int uvalue = (unsigned int)value;

    uvalue |= (uvalue >> 1);
    uvalue |= (uvalue >> 2);
    uvalue |= (uvalue >> 4);
    uvalue |= (uvalue >> 8);
    uvalue |= (uvalue >> 16);
    return ( uvalue & ~(uvalue >> 1));
}

////////////////////////////////////////////////////////////////////////////////
int Integer::lowestOneBit( int value ) {
    if( value == 0 ) {
        return 0;
    }

    unsigned int uvalue = (unsigned int)value;
    return ( uvalue & (-uvalue) );
}

////////////////////////////////////////////////////////////////////////////////
int Integer::numberOfLeadingZeros( int value ) {

    if( value == 0 ) {
        return 0;
    }

    unsigned int uvalue = (unsigned int)value;

    value |= value >> 1;
    value |= value >> 2;
    value |= value >> 4;
    value |= value >> 8;
    value |= value >> 16;
    return Integer::bitCount( ~uvalue );
}

////////////////////////////////////////////////////////////////////////////////
int Integer::numberOfTrailingZeros( int value ) {
    if( value == 0 ) {
        return 0;
    }

    unsigned int uvalue = (unsigned int)value;
    return bitCount( (uvalue & -uvalue) - 1 );
}

////////////////////////////////////////////////////////////////////////////////
int Integer::rotateLeft( int value, int distance ) {
    unsigned int i = (unsigned int)value;
    int j = distance & 0x1F;
    return ( i << j ) | ( i >> (-j & 0x1F ) );
}

////////////////////////////////////////////////////////////////////////////////
int Integer::rotateRight( int value, int distance ) {
    unsigned int i = (unsigned int)value;
    int j = distance & 0x1F;
    return ( i >> j ) | ( i << (-j & 0x1F ) );
}

////////////////////////////////////////////////////////////////////////////////
int Integer::signum( int value ) {
    return ( value == 0 ? 0 : ( value < 0 ? -1 : 1 ) );
}
