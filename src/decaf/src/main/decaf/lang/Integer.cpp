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

#include "Integer.h"
#include <sstream>

using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
Integer::Integer( int value ) {
    this->value = value;
}

////////////////////////////////////////////////////////////////////////////////
Integer::Integer( const std::string& value ) {
    this->value = parseInt( value );
}

////////////////////////////////////////////////////////////////////////////////
int Integer::compareTo( const Integer& n ) const {
    return this->value < n.value ? -1 : this->value == n.value ? 0 : 1;
}

////////////////////////////////////////////////////////////////////////////////
int Integer::bitCount( int value ) {

    if( value == 0 ) {
        return 0;
    }

    // 32-bit recursive reduction using SWAR...
    // but first step is mapping 2-bit values
    // into sum of 2 1-bit values in sneaky way
    value -= ((value >> 1) & 0x55555555);
    value = (((value >> 2) & 0x33333333) + (value & 0x33333333));
    value = (((value >> 4) + value) & 0x0F0F0F0F);
    value += (value >> 8);
    value += (value >> 16);
    return(value & 0x0000003F);
}

////////////////////////////////////////////////////////////////////////////////
std::string Integer::toString( int value )
{
    std::ostringstream ostream;
    ostream << value;
    return ostream.str();
}

////////////////////////////////////////////////////////////////////////////////
int Integer::parseInt( const std::string& value )
    throw ( exceptions::NumberFormatException ) {

    int ret = 0;
    std::istringstream istream(value);
    istream.clear();
    istream >> ret;
    return ret;
}

////////////////////////////////////////////////////////////////////////////////
int Integer::parse( const std::string& value, int offset,
                    int radix, bool negative )
    throw ( exceptions::NumberFormatException ) {

    int max = Integer::MIN_VALUE / radix;
    int result = 0, length = value.size();

    while( offset < length ) {
        //TODO
        int digit = 0;//Character.digit(string.charAt(offset++), radix);
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
