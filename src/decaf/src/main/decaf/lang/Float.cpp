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

using namespace std;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
const float Float::MAX_VALUE = 3.40282347e+38F;
const float Float::MIN_VALUE = 1.17549435e-38F;
const float Float::NaN = 0.0f / 0.0f;
const float Float::POSITIVE_INFINITY = 1.0f / 0.0f;
const float Float::NEGATIVE_INFINITY = -1.0f / 0.0f;

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

    return 0.0; // TODO
}

////////////////////////////////////////////////////////////////////////////////
std::string Float::toHexString( float value ) {
    return ""; //TODO
}

////////////////////////////////////////////////////////////////////////////////
std::string Float::toString( float value ) {
    return ""; //TODO
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
