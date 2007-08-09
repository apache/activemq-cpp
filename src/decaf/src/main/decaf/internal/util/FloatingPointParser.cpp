/*
 *  Licensed to the Apache Software Foundation (ASF) under one or more
 *  contributor license agreements.  See the NOTICE file distributed with
 *  this work for additional information regarding copyright ownership.
 *  The ASF licenses this file to You under the Apache License, Version 2.0
 *  (the "License"); you may not use this file except in compliance with
 *  the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include "FloatingPointParser.h"
#include <decaf/lang/Math.h>
#include <decaf/lang/Integer.h>
#include <decaf/lang/Long.h>
#include <decaf/lang/Float.h>
#include <decaf/lang/Double.h>
#include <decaf/lang/Character.h>
#include <decaf/internal/util/HexStringParser.h>
#include <decaf/internal/util/BigInt.h>
#include <decaf/internal/util/BitOps.h>
#include <apr_lib.h>

using namespace std;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::internal;
using namespace decaf::internal::util;

////////////////////////////////////////////////////////////////////////////////
double FloatingPointParser::parseDblImpl( const std::string& value, int exp )
    throw ( exceptions::NumberFormatException ) {

    // assumes s is a null terminated string with at least one
    // character in it
    unsigned long long def[17];
    unsigned long long defBackup[17];
    unsigned long long* f;
    unsigned long long* fNoOverflow;
    unsigned long long* g;
    unsigned long long* tempBackup;
    unsigned int overflow = 0;
    unsigned long long result = 0;
    int index = 1;
    int unprocessedDigits = 0;
    std::string::const_iterator valItr = value.begin();

    f = def;
    fNoOverflow = defBackup;
    (*f) = 0;
    tempBackup = g = 0;

    do {

        if( *valItr >= '0' && *valItr <= '9' ) {

            // Make a back up of f before appending, so that we can
            // back out of it if there is no more room, i.e. index >
            // MAX_ACCURACY_WIDTH.

            memcpy( fNoOverflow, f, sizeof(unsigned long long) * index );
            overflow = BigInt::simpleAppendDecimalDigitHighPrecision(
                    f, index, *valItr - '0' );

            if( overflow ) {
                f[index++] = overflow;
                /* There is an overflow, but there is no more room
                 * to store the result. We really only need the top 52
                 * bits anyway, so we must back out of the overflow,
                 * and ignore the rest of the string.
                 */
                if( index >= MAX_ACCURACY_WIDTH ) {
                    index--;
                    memcpy( f, fNoOverflow, sizeof(unsigned long long) * index);
                    break;
                }

                if( tempBackup ) {
                    fNoOverflow = tempBackup;
                }
            }
        } else {
            index = -1;
        }

    } while( index > 0 && *(++valItr) != '\0' );

    /* We've broken out of the parse loop either because we've reached
     * the end of the string or we've overflowed the maximum accuracy
     * limit of a double. If we still have unprocessed digits in the
     * given string, then there are three possible results:
     *   1. (unprocessed digits + e) == 0, in which case we simply
     *      convert the existing bits that are already parsed
     *   2. (unprocessed digits + e) < 0, in which case we simply
     *      convert the existing bits that are already parsed along
     *      with the given e
     *   3. (unprocessed digits + e) > 0 indicates that the value is
     *      simply too big to be stored as a double, so return Infinity
     */
    if( ( unprocessedDigits = value.length() ) > 0 ) {

        exp += unprocessedDigits;
        if( index > -1 ) {

            if( exp == 0 ) {
                return BigInt::toDoubleHighPrecision( f, index );
            } else if( exp < 0 ) {
                return BitOps::CREATE_DOUBLE_BITS( f, index, exp );
            } else {
                result = BitOps::INFINITE_LONGBITS;
            }

        } else {

            BitOps::LOW_I32_FROM_LONG64( result ) = -1;
            BitOps::HIGH_I32_FROM_LONG64( result ) = -1;
        }
    } else {

        if( index > -1 ) {
            if( exp == 0 ) {
                return BigInt::toDoubleHighPrecision( f, index );
            } else {
                return BitOps::CREATE_DOUBLE_BITS( f, index, exp );
            }
        } else {
            BitOps::LOW_I32_FROM_LONG64( result ) = -1;
            BitOps::HIGH_I32_FROM_LONG64( result ) = -1;
        }
    }

    return Double::longBitsToDouble( result );
}

////////////////////////////////////////////////////////////////////////////////
float FloatingPointParser::parseFltImpl( const std::string& value, int exp )
    throw ( exceptions::NumberFormatException ) {

    return 0.0f;
}

////////////////////////////////////////////////////////////////////////////////
FloatingPointParser::StringExponentPair FloatingPointParser::initialParse(
    const std::string& value, int length ) throw ( exceptions::NumberFormatException ){

    std::string newValue = value;
    bool negative = false;
    char c;
    int start = 0;
    int end = 0;
    int decimal = 0;
    int e = 0;

    if( length == 0 ) {
        throw exceptions::NumberFormatException(
            __FILE__, __LINE__,
            "FloatingPointParser::initialParse - Invalid zero length string");
    }

    c = newValue[length - 1];
    if( c == 'D' || c == 'd' || c == 'F' || c == 'f') {
        length--;
        if( length == 0 ) {
            throw exceptions::NumberFormatException(
                __FILE__, __LINE__,
                "FloatingPointParser::initialParse - Invalid zero length string");
        }
    }

    end = Math::max( (int)newValue.find_first_of( 'E' ),
                     (int)newValue.find_first_of( 'e' ) );

    if( end > -1 ) {

        if( end + 1 == length ) {
            if( length == 0 ) {
                throw exceptions::NumberFormatException(
                    __FILE__, __LINE__,
                    "FloatingPointParser::initialParse - Invalid no data after e",
                    newValue.c_str() );
            }
        }

        int exponent_offset = end + 1;

        if( newValue[exponent_offset] == '+' ) {
            if( newValue[exponent_offset + 1] == '-' ) {
                if( length == 0 ) {
                    throw exceptions::NumberFormatException(
                        __FILE__, __LINE__,
                        "FloatingPointParser::initialParse - "
                        "Invalid length string");
                }
            }
            exponent_offset++; // skip the plus sign
        }

        try {
            e = Integer::parseInt( newValue.substr( exponent_offset, length ) );
        } catch( exceptions::NumberFormatException ex ) {
            // ex contains the exponent substring only so throw a
            // new exception with the correct string
            throw exceptions::NumberFormatException(
                __FILE__, __LINE__,
                "FloatingPointParser::initialParse - "
                "exponent string is not valid" );
        }

    } else {
        end = length;
    }

    if( length == 0 ) {
        throw exceptions::NumberFormatException(
            __FILE__, __LINE__,
            "FloatingPointParser::initialParse - "
            "invalid length string", newValue.c_str() );
    }

    c = newValue[start];

    if( c == '-') {
        ++start;
        --length;
        negative = true;
    } else if( c == '+' ) {
        ++start;
        --length;
    }

    if( length == 0 ) {
        throw exceptions::NumberFormatException(
            __FILE__, __LINE__,
            "FloatingPointParser::initialParse - "
            "invalid length string", newValue.c_str() );
    }

    decimal = newValue.find_first_of( '.' );
    if( decimal > -1 ) {
        e -= end - decimal - 1;
        newValue = newValue.substr( start, decimal ) + newValue.substr( decimal + 1, end );
    } else {
        newValue = newValue.substr( start, end );
    }

    if( ( length = newValue.length() ) == 0 ) {
        throw exceptions::NumberFormatException(
            __FILE__, __LINE__,
            "FloatingPointParser::initialParse - "
            "invalid length string", newValue.c_str() );
    }

    end = length;
    while( end > 1 && newValue[end - 1] == '0' ) {
        --end;
    }

    start = 0;
    while( start < end - 1 && newValue[start] == '0' ) {
        start++;
    }

    if( end != length || start != 0 ) {
        e += length - end;
        newValue = newValue.substr( start, end );
    }

    return StringExponentPair( newValue, e, negative );
}

////////////////////////////////////////////////////////////////////////////////
double FloatingPointParser::parseDblName(
    const std::string& namedDouble, int length )
        throw( exceptions::NumberFormatException ) {

    // Valid strings are only +Nan, NaN, -Nan, +Infinity, Infinity,
    // -Infinity.
    if( (length != 3) && ( length != 4) && ( length != 8) && ( length != 9 ) ) {
        throw exceptions::NumberFormatException(
            __FILE__, __LINE__,
            "FloatingPointParser::parseDblName - "
            "invalid name string", namedDouble.c_str() );
    }

    bool negative = namedDouble.at(0) == '-' ? true : false;

    if( namedDouble.find_first_of( "Infinity" ) != string::npos ) {
        return negative ? Double::NEGATIVE_INFINITY : Float::POSITIVE_INFINITY;
    }

    if( namedDouble.find_first_of( "NaN" ) != string::npos ) {
        return Double::NaN;
    }

    throw exceptions::NumberFormatException(
        __FILE__, __LINE__,
        "FloatingPointParser::parseDblName - "
        "invalid name string", namedDouble.c_str() );
}

////////////////////////////////////////////////////////////////////////////////
float FloatingPointParser::parseFltName(
    const std::string& namedFloat, int length )
        throw( exceptions::NumberFormatException ) {

    // Valid strings are only +Nan, NaN, -Nan, +Infinity, Infinity,
    // -Infinity.
    if( ( length != 3 ) && ( length != 4 ) && ( length != 8 ) && ( length != 9 ) ) {
        throw exceptions::NumberFormatException(
            __FILE__, __LINE__,
            "FloatingPointParser::parseFltName - "
            "invalid name string", namedFloat.c_str() );
    }

    bool negative = namedFloat.at(0) == '-' ? true : false;

    if( namedFloat.find_first_of( "Infinitiy" ) != string::npos ) {
        return negative ? Float::NEGATIVE_INFINITY : Float::POSITIVE_INFINITY;
    }

    if( namedFloat.find_first_of( "NaN" ) != string::npos ) {
        return Float::NaN;
    }

    throw exceptions::NumberFormatException(
        __FILE__, __LINE__,
        "FloatingPointParser::parseFltName - "
        "invalid name string", namedFloat.c_str() );
}

////////////////////////////////////////////////////////////////////////////////
double FloatingPointParser::parseDouble( const std::string& value )
    throw( exceptions::NumberFormatException ) {

    std::string newValue = value;
    FloatingPointParser::trim( newValue );
    int length = newValue.length();

    if( length == 0 ) {
        throw exceptions::NumberFormatException(
            __FILE__, __LINE__,
            "FloatingPointParser::parseDouble - "
            "invalid length string", value.c_str() );
    }

    // See if this could be a named double
    char last = newValue[length - 1];

    if( (last == 'y') || (last == 'N') ) {
        return parseDblName( newValue, length );
    }

    // See if it could be a hexadecimal representation
    if( toLowerCase( newValue ).find_first_of( "0x" ) != string::npos ) {
        return HexStringParser::parseDouble(value);
    }

    StringExponentPair info = initialParse(value, length);

    double result = parseDblImpl( info.value, info.exp );

    if( info.negative ) {
        result = -result;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
float FloatingPointParser::parseFloat( const std::string& value )
    throw( exceptions::NumberFormatException ) {

    std::string newValue = value;
    FloatingPointParser::trim( newValue );

    int length = newValue.length();

    if( length == 0 ) {
        throw exceptions::NumberFormatException(
            __FILE__, __LINE__,
            "FloatingPointParser::parseFloat - "
            "invalid length string", value.c_str() );
    }

    // See if this could be a named float
    char last = newValue[length - 1];
    if( (last == 'y') || (last == 'N') ) {
        return parseFltName( value, length );
    }

    // See if it could be a hexadecimal representation
    if( toLowerCase( newValue ).find_first_of( "0x" ) != string::npos ) {
        return HexStringParser::parseFloat( newValue );
    }

    StringExponentPair info = initialParse( newValue, length );

    float result = parseFltImpl( info.value, info.exp );
    if( info.negative ) {
        result = -result;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
std::string& FloatingPointParser::trim( std::string& value ) {

    char const* delims = " \t\r\n";

    // trim leading whitespace
    string::size_type notwhite = value.find_first_not_of( delims );
    value.erase( 0, notwhite );

    // trim trailing whitespace
    notwhite = value.find_last_not_of( delims );
    value.erase( notwhite+1 );

    return value;
}

////////////////////////////////////////////////////////////////////////////////
std::string& FloatingPointParser::toLowerCase( std::string& value ) {

    string::size_type index = 0;

    for( ; index < value.size(); ++index ) {
        if( Character::isLetter( value[index] ) ) {
            value[index] = apr_tolower( value[index] );
        }
    }

    return value;
}
