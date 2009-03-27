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

#include "Float.h"
#include <decaf/lang/Integer.h>
#include <limits>
#include <string.h>

using namespace std;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
const float Float::MAX_VALUE = 3.40282346638528860e+38f;
const float Float::MIN_VALUE = 1.40129846432481707e-45f;
const float Float::NaN = std::numeric_limits<float>::quiet_NaN();
const float Float::POSITIVE_INFINITY = std::numeric_limits<float>::infinity();
const float Float::NEGATIVE_INFINITY = -std::numeric_limits<float>::infinity();

////////////////////////////////////////////////////////////////////////////////
Float::Float( float value ) {
    this->value = value;
}

////////////////////////////////////////////////////////////////////////////////
Float::Float( double value ) {
    this->value = (float)value;
}

////////////////////////////////////////////////////////////////////////////////
Float::Float( const std::string& value ) throw( exceptions::NumberFormatException ) {
    this->value = Float::parseFloat( value );
}

////////////////////////////////////////////////////////////////////////////////
int Float::compareTo( const Float& f ) const {
    return Float::compare( this->value, f.value );
}

////////////////////////////////////////////////////////////////////////////////
int Float::compareTo( const float& f ) const {
    return Float::compare( this->value, f );
}

////////////////////////////////////////////////////////////////////////////////
std::string Float::toString() const {
    return Float::toString( this->value );
}

////////////////////////////////////////////////////////////////////////////////
bool Float::isInfinite() const {
    return Float::isInfinite( this->value );
}

////////////////////////////////////////////////////////////////////////////////
bool Float::isNaN() const {
    return Float::isNaN( this->value );
}

////////////////////////////////////////////////////////////////////////////////
int Float::compare( float f1, float f2 ) {

    int i1, i2 = 0;
    long NaNbits = Float::floatToIntBits( Float::NaN );

    if( ( i1 = Float::floatToIntBits( f1 ) ) == NaNbits ) {
        if( Float::floatToIntBits( f2 ) == NaNbits ) {
            return 0;
        }
        return 1;
    }

    if( ( i2 = Float::floatToIntBits( f2 ) ) == NaNbits ) {
        return -1;
    }

    if( f1 == f2 ) {
        if( i1 == i2 ) {
            return 0;
        }

        // check for -0
        return i1 > i2 ? 1 : -1;
    }

    return f1 > f2 ? 1 : -1;
}

////////////////////////////////////////////////////////////////////////////////
int Float::floatToIntBits( float value ) {

    int intValue = 0;
    memcpy( &intValue, &value, sizeof( float ) );

    if( ( intValue & SINGLE_EXPONENT_MASK ) == SINGLE_EXPONENT_MASK )
    {
        if( intValue & SINGLE_MANTISSA_MASK )
        {
            return SINGLE_NAN_BITS;
        }
    }

    return intValue;
}

////////////////////////////////////////////////////////////////////////////////
int Float::floatToRawIntBits( float value ) {

    int intValue = 0;
    memcpy( &intValue, &value, sizeof( float ) );
    return intValue;
}

////////////////////////////////////////////////////////////////////////////////
float Float::intBitsToFloat( int bits ) {

    float floatValue = 0;
    memcpy( &floatValue, &bits, sizeof( int ) );
    return floatValue;
}

////////////////////////////////////////////////////////////////////////////////
bool Float::isInfinite( float value ) {
    return ( value == POSITIVE_INFINITY ) || ( value == NEGATIVE_INFINITY );
}

////////////////////////////////////////////////////////////////////////////////
bool Float::isNaN( float value ) {
    return value != value;
}

////////////////////////////////////////////////////////////////////////////////
float Float::parseFloat( const std::string& value )
    throw ( exceptions::NumberFormatException ) {

    // TODO - This is not going to parse the formats we say we do.
    float result = 0.0;
    istringstream stream( value );
    stream >> result;
    return result;
}

////////////////////////////////////////////////////////////////////////////////
std::string Float::toHexString( float value ) {
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

    unsigned int bitValue = Float::floatToIntBits( value );

    bool negative = ( bitValue & 0x80000000 ) != 0;
    // mask exponent bits and shift down
    unsigned int exponent = ( bitValue & 0x7f800000 ) >> 23;
    // mask significand bits and shift up
    // significand is 23-bits, so we shift to treat it like 24-bits
    unsigned int significand = ( bitValue & 0x007FFFFF ) << 1;

    if( exponent == 0 && significand == 0 ) {
        return ( negative ? "-0x0.0p0" : "0x0.0p0" );
    }

    // Start with the correct sign and Hex indicator
    std::string hexString( negative ? "-0x" : "0x" );

    if( exponent == 0 ) {
        // denormal (subnormal) value
        hexString.append( "0." );
        // significand is 23-bits, so there can be 6 hex digits
        unsigned int fractionDigits = 6;
        // remove trailing hex zeros, so Integer.toHexString() won't print
        // them
        while( ( significand != 0 ) && ( ( significand & 0xF ) == 0 ) ) {
            significand >>= 4;
            fractionDigits--;
        }
        // this assumes Integer.toHexString() returns lowercase characters
        std::string hexSignificand = Integer::toHexString( significand );

        // if there are digits left, then insert some '0' chars first
        if( significand != 0 && fractionDigits > hexSignificand.length() ) {
            unsigned int digitDiff = fractionDigits - hexSignificand.length();
            while( digitDiff-- != 0 ) {
                hexString.append( "0" );
            }
        }
        hexString.append( hexSignificand );
        hexString.append( "p-126" );
    } else {
        // normal value
        hexString.append( "1." );
        // significand is 23-bits, so there can be 6 hex digits
        unsigned int fractionDigits = 6;
        // remove trailing hex zeros, so Integer.toHexString() won't print
        // them
        while( (significand != 0) && ((significand & 0xF ) == 0 ) ) {
            significand >>= 4;
            fractionDigits--;
        }
        // this assumes Integer.toHexString() returns lowercase characters
        std::string hexSignificand = Integer::toHexString( significand );

        // if there are digits left, then insert some '0' chars first
        if( significand != 0 && fractionDigits > hexSignificand.length() ) {
            unsigned int digitDiff = fractionDigits - hexSignificand.length();
            while( digitDiff-- != 0 ) {
                hexString.append( "0" );
            }
        }
        hexString.append( hexSignificand );
        hexString.append( "p" );
        // remove exponent's 'bias' and convert to a string
        hexString.append( Integer::toString( exponent - 127 ) );
    }

    return hexString;
}

////////////////////////////////////////////////////////////////////////////////
std::string Float::toString( float value ) {

    // TODO - This is not going to support the formats we say we do.
    ostringstream stream;
    stream << value;
    return stream.str();
}

////////////////////////////////////////////////////////////////////////////////
Float Float::valueOf( float value ) {
    return Float( value );
}

////////////////////////////////////////////////////////////////////////////////
Float Float::valueOf( const std::string& value )
    throw ( exceptions::NumberFormatException ) {

    return valueOf( parseFloat( value ) );
}
