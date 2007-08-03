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
    //TODO this->value = Float::parseFloat( value );
}

////////////////////////////////////////////////////////////////////////////////
int Float::compareTo( const Float& f ) const {
    // TODO
    return this->value < f.value ? -1 : this->value == f.value ? 0 : 1;
}

////////////////////////////////////////////////////////////////////////////////
int Float::compareTo( const float& f ) const {
    // TODO
    return this->value < f ? -1 : this->value == f ? 0 : 1;
}

////////////////////////////////////////////////////////////////////////////////
std::string Float::toString() const {
    return ""; //TODO Float::toString( this->value, 10 );
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
