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

#include "Double.h"
#include <decaf/lang/Long.h>
#include <limits>
#include <string.h>

using namespace std;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
const double Double::MAX_VALUE = 1.7976931348623157e+308;
const double Double::MIN_VALUE = 5e-324;
const double Double::NaN = std::numeric_limits<double>::quiet_NaN();
const double Double::POSITIVE_INFINITY = std::numeric_limits<double>::infinity();
const double Double::NEGATIVE_INFINITY = -std::numeric_limits<double>::infinity();

////////////////////////////////////////////////////////////////////////////////
Double::Double( double value ) {
    this->value = value;
}

////////////////////////////////////////////////////////////////////////////////
Double::Double( const std::string& value ) throw( exceptions::NumberFormatException ) {
    this->value = Double::parseDouble( value );
}

////////////////////////////////////////////////////////////////////////////////
int Double::compareTo( const Double& d ) const {
    return Double::compare( this->value, d.value );
}

////////////////////////////////////////////////////////////////////////////////
int Double::compareTo( const double& d ) const {
    return Double::compare( this->value, d );
}

////////////////////////////////////////////////////////////////////////////////
std::string Double::toString() const {
    return Double::toString( this->value );
}

////////////////////////////////////////////////////////////////////////////////
bool Double::isInfinite() const {
    return Double::isInfinite( this->value );
}

////////////////////////////////////////////////////////////////////////////////
bool Double::isNaN() const {
    return Double::isNaN( this->value );
}

////////////////////////////////////////////////////////////////////////////////
int Double::compare( double d1, double d2 ) {

    long long l1, l2 = 0;
    long long NaNbits = Double::doubleToLongBits( Double::NaN );

    if( ( l1 = Double::doubleToLongBits( d1 ) ) == NaNbits ) {
        if( Double::doubleToLongBits( d2 ) == NaNbits ) {
            return 0;
        }
        return 1;
    }

    if( ( l2 = Double::doubleToLongBits( d2 ) ) == NaNbits ) {
        return -1;
    }

    if( d1 == d2 ) {
        if( l1 == l2 ) {
            return 0;
        }

        // check for -0
        return l1 > l2 ? 1 : -1;
    }

    return d1 > d2 ? 1 : -1;
}

////////////////////////////////////////////////////////////////////////////////
long long Double::doubleToLongBits( double value ) {

    long long longValue = 0;
    memcpy( &longValue, &value, sizeof( double ) );

    if( ( longValue & DOUBLE_EXPONENT_MASK ) == DOUBLE_EXPONENT_MASK ) {
        if( longValue & DOUBLE_MANTISSA_MASK ) {
            return DOUBLE_NAN_BITS;
        }
    }

    return longValue;
}

////////////////////////////////////////////////////////////////////////////////
long long Double::doubleToRawLongBits( double value ) {

    long long longValue = 0;
    memcpy( &longValue, &value, sizeof( double ) );
    return longValue;
}

////////////////////////////////////////////////////////////////////////////////
bool Double::isInfinite( double value ) {
    return ( value == POSITIVE_INFINITY ) || ( value == NEGATIVE_INFINITY );
}

////////////////////////////////////////////////////////////////////////////////
bool Double::isNaN( double value ) {
    return value != value;
}

////////////////////////////////////////////////////////////////////////////////
double Double::longBitsToDouble( long long bits ) {
    double result = 0;
    memcpy( &result, &bits, sizeof( long long ) );
    return result;
}

////////////////////////////////////////////////////////////////////////////////
double Double::parseDouble( DECAF_UNUSED const std::string value )
    throw ( exceptions::NumberFormatException ) {

    return 0; // TODO
}

////////////////////////////////////////////////////////////////////////////////
std::string Double::toHexString( double value ) {
    /*
     * Reference: http://en.wikipedia.org/wiki/IEEE_754
     */
    if( value != value ) {
        return "NaN";
    }
    if( value == POSITIVE_INFINITY ) {
        return "Infinity";
    }
    if( value == NEGATIVE_INFINITY ) {
        return "-Infinity";
    }

    unsigned long long bitValue = Double::doubleToLongBits( value );

    bool negative = ( bitValue & 0x8000000000000000LL ) != 0;
    // mask exponent bits and shift down
    unsigned long long exponent = ( bitValue & 0x7FF0000000000000LL ) >> 52;
    // mask significand bits and shift up
    unsigned long long significand = bitValue & 0x000FFFFFFFFFFFFFLL;

    if( exponent == 0 && significand == 0 ) {
        return ( negative ? "-0x0.0p0" : "0x0.0p0" );
    }

    // Start with sign and hex indicator
    std::string hexString( negative ? "-0x" : "0x" );

    if( exponent == 0 ) {
        // denormal (subnormal) value
        hexString.append("0.");
        // significand is 52-bits, so there can be 13 hex digits
        unsigned int fractionDigits = 13;
        // remove trailing hex zeros, so Integer.toHexString() won't print
        // them
        while( ( significand != 0 ) && ( ( significand & 0xF ) == 0 ) ) {
            significand >>= 4;
            fractionDigits--;
        }
        // this assumes Integer.toHexString() returns lowercase characters
        std::string hexSignificand = Long::toHexString( significand );

        // if there are digits left, then insert some '0' chars first
        if( significand != 0 && fractionDigits > hexSignificand.length() ) {
            int digitDiff = fractionDigits - hexSignificand.length();
            while( digitDiff-- != 0 ) {
                hexString.append( "0" );
            }
        }

        hexString.append( hexSignificand );
        hexString.append( "p-1022" );
    } else {
        // normal value
        hexString.append( "1." );
        // significand is 52-bits, so there can be 13 hex digits
        unsigned int fractionDigits = 13;
        // remove trailing hex zeros, so Integer.toHexString() won't print
        // them
        while( (significand != 0 ) && ( ( significand & 0xF ) == 0 ) ) {
            significand >>= 4;
            fractionDigits--;
        }
        // this assumes Integer.toHexString() returns lowercase characters
        std::string hexSignificand = Long::toHexString( significand );

        // if there are digits left, then insert some '0' chars first
        if( significand != 0 && fractionDigits > hexSignificand.length() ) {
            int digitDiff = fractionDigits - hexSignificand.length();
            while( digitDiff-- != 0 ) {
                hexString.append( "0" );
            }
        }

        hexString.append( hexSignificand );
        hexString.append( "p" );
        // remove exponent's 'bias' and convert to a string
        hexString.append( Long::toString( exponent - 1023 ) );
    }

    return hexString;
}

////////////////////////////////////////////////////////////////////////////////
std::string Double::toString( DECAF_UNUSED double value ) {
    return ""; //TODO
}

////////////////////////////////////////////////////////////////////////////////
Double Double::valueOf( double value ) {
    return Double( value );
}

////////////////////////////////////////////////////////////////////////////////
Double Double::valueOf( const std::string& value )
    throw ( exceptions::NumberFormatException ) {

    return valueOf( parseDouble( value ) );
}
