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

#include "HexStringParser.h"

#include <decaf/lang/Character.h>
#include <decaf/lang/Integer.h>
#include <decaf/lang/Long.h>
#include <decaf/lang/Double.h>
#include <decaf/lang/Float.h>
#include <decaf/util/StringTokenizer.h>
#include <decaf/lang/exceptions/NumberFormatException.h>
#include <apr_pools.h>
#include <apr_strmatch.h>

using namespace decaf;
using namespace decaf::util;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::internal;
using namespace decaf::internal::util;

////////////////////////////////////////////////////////////////////////////////
const std::string HexStringParser::HEX_SIGNIFICANT =
    "0[xX](\\p{XDigit}+\\.?|\\p{XDigit}*\\.\\p{XDigit}+)";
const std::string HexStringParser::BINARY_EXPONENT =
    "[pP]([+-]?\\d+)";
const std::string HexStringParser::FLOAT_TYPE_SUFFIX =
    "[fFdD]?";
const std::string HexStringParser::HEX_PATTERN =
    "[\\x00-\\x20]*([+-]?)" + HEX_SIGNIFICANT + BINARY_EXPONENT + FLOAT_TYPE_SUFFIX + "[\\x00-\\x20]*";

////////////////////////////////////////////////////////////////////////////////
HexStringParser::HexStringParser( int exponentWidth, int mantissaWidth ) {

    this->EXPONENT_WIDTH = exponentWidth;
    this->MANTISSA_WIDTH = mantissaWidth;

    this->EXPONENT_BASE = ~( -1 << (exponentWidth - 1) );
    this->MAX_EXPONENT = ~( -1 << exponentWidth );
    this->MIN_EXPONENT = -( MANTISSA_WIDTH + 1 );
    this->MANTISSA_MASK = ~( -1 << mantissaWidth );
}

////////////////////////////////////////////////////////////////////////////////
double HexStringParser::parseDouble( const std::string& hexString ) {

    HexStringParser parser( DOUBLE_EXPONENT_WIDTH, DOUBLE_MANTISSA_WIDTH );
    long long result = parser.parse( hexString );
    return Double::longBitsToDouble( result );
}

////////////////////////////////////////////////////////////////////////////////
float HexStringParser::parseFloat( const std::string& hexString ) {

    HexStringParser parser( FLOAT_EXPONENT_WIDTH, FLOAT_MANTISSA_WIDTH );
    int result = (int)parser.parse( hexString );
    return Float::intBitsToFloat( result );
}

////////////////////////////////////////////////////////////////////////////////
long long HexStringParser::parse( const std::string& hexString ) {

    std::string* hexSegments = getSegmentsFromHexString( hexString );
    std::string signStr = hexSegments[0];
    std::string significantStr = hexSegments[1];
    std::string exponentStr = hexSegments[2];
    delete hexSegments;

    parseHexSign( signStr );
    parseExponent( exponentStr );
    parseMantissa( significantStr );

    sign <<= ( MANTISSA_WIDTH + EXPONENT_WIDTH );
    exponent <<= MANTISSA_WIDTH;
    return sign | exponent | mantissa;
}

////////////////////////////////////////////////////////////////////////////////
std::string* HexStringParser::getSegmentsFromHexString( DECAF_UNUSED const std::string& hexString ) {

//    apr_pool_t* thePool = NULL;
//    apr_pool_create( &thePool, NULL );
//    apr_strmatch_pattern* pattern =
//        apr_strmatch_precompile( thePool, HEX_PATTERN.c_str(), 0 );
//
//    std::vector<std::string> hexSegments;
//


    // TODO
//    Matcher matcher = PATTERN.matcher(hexString);
//    if( !matcher.matches() ) {
//        throw NumberFormatException(
//            __FILE__, __LINE__,
//            "HexStringParser::getSegmentsFromHexString"
//            "Invalid hex string:", hexString.c_str() );
//    }
//
//    std::string* hexSegments = new std::string[3];
//    hexSegments[0] = matcher.group(1);
//    hexSegments[1] = matcher.group(2);
//    hexSegments[2] = matcher.group(3);
//
//    return hexSegments;

    return NULL;
}

////////////////////////////////////////////////////////////////////////////////
void HexStringParser::parseExponent(const std::string& exponentStr) {

    std::string newExponentStr = exponentStr;

    char leadingChar = newExponentStr.at(0);
    int expSign = ( leadingChar == '-' ? -1 : 1 );
    if( !Character::isDigit( leadingChar ) ) {
        newExponentStr = newExponentStr.substr( 1, newExponentStr.size() );
    }

    try {
        exponent = expSign * Long::parseLong( exponentStr );
        checkedAddExponent( EXPONENT_BASE );
    } catch( exceptions::NumberFormatException& e ) {
        exponent = expSign * Long::MAX_VALUE;
    }
}

////////////////////////////////////////////////////////////////////////////////
void HexStringParser::parseMantissa(const std::string& significantStr) {

    StringTokenizer tokenizer( significantStr, "\\." );
    std::vector<std::string> strings;

    tokenizer.toArray( strings );

    std::string strIntegerPart = strings[0];
    std::string strDecimalPart = strings.size() > 1 ? strings[1] : "";

    std::string significand =
        getNormalizedSignificand( strIntegerPart, strDecimalPart) ;

    if( significand == "0" ) {
        setZero();
        return;
    }

    int offset = getOffset( strIntegerPart, strDecimalPart );
    checkedAddExponent( offset );

    if( exponent >= MAX_EXPONENT ) {
        setInfinite();
        return;
    }

    if( exponent <= MIN_EXPONENT ) {
        setZero();
        return;
    }

    if( significand.length() > MAX_SIGNIFICANT_LENGTH ) {
        abandonedNumber = significand.substr( MAX_SIGNIFICANT_LENGTH );
        significand = significand.substr( 0, MAX_SIGNIFICANT_LENGTH );
    }

    mantissa = Long::parseLong( significand, HEX_RADIX );

    if( exponent >= 1 ) {
        processNormalNumber();
    } else{
        processSubNormalNumber();
    }

}

////////////////////////////////////////////////////////////////////////////////
void HexStringParser::checkedAddExponent( long long offset ) {

    long long result = exponent + offset;
    int expSign = Long::signum( exponent );

    if( expSign * Long::signum( offset ) > 0 &&
        expSign * Long::signum( result ) < 0 ) {

        exponent = expSign * Long::MAX_VALUE;
    } else {
        exponent = result;
    }
}

////////////////////////////////////////////////////////////////////////////////
void HexStringParser::processNormalNumber() {
    int desiredWidth = MANTISSA_WIDTH + 2;
    fitMantissaInDesiredWidth( desiredWidth );
    round();
    mantissa = mantissa & MANTISSA_MASK;
}

////////////////////////////////////////////////////////////////////////////////
void HexStringParser::processSubNormalNumber() {
    int desiredWidth = MANTISSA_WIDTH + 1;
    desiredWidth += (int)exponent;//lends bit from mantissa to exponent
    exponent = 0;
    fitMantissaInDesiredWidth( desiredWidth );
    round();
    mantissa = mantissa & MANTISSA_MASK;
}

////////////////////////////////////////////////////////////////////////////////
void HexStringParser::fitMantissaInDesiredWidth(int desiredWidth){
    int bitLength = countBitsLength( mantissa );
    if( bitLength > desiredWidth ) {
        discardTrailingBits( bitLength - desiredWidth );
    } else {
        mantissa <<= ( desiredWidth - bitLength );
    }
}

////////////////////////////////////////////////////////////////////////////////
void HexStringParser::discardTrailingBits( long long num ) {
    long long mask = ~( -1L << num );
    abandonedNumber += (char)( mantissa & mask );
    mantissa >>= num;
}

////////////////////////////////////////////////////////////////////////////////
void HexStringParser::round() {

    std::string result = abandonedNumber;
    replaceAll( result, "0+", "" );

    bool moreThanZero = ( result.length() > 0 ? true : false );

    int lastDiscardedBit = (int)( mantissa & 1L );
    mantissa >>= 1;
    int tailBitInMantissa = (int)( mantissa & 1L );

    if( lastDiscardedBit == 1 && ( moreThanZero || tailBitInMantissa == 1 ) ) {

        int oldLength = countBitsLength( mantissa );
        mantissa += 1L;
        int newLength = countBitsLength( mantissa );

        //Rounds up to exponent when whole bits of mantissa are one-bits.
        if( oldLength >= MANTISSA_WIDTH && newLength > oldLength ) {
            checkedAddExponent( 1 );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
std::string HexStringParser::getNormalizedSignificand(
    const std::string& strIntegerPart, const std::string& strDecimalPart ) {

    std::string significand = strIntegerPart + strDecimalPart;

    replaceFirst( significand, "^0x", "" );

    if( significand.length() == 0 ) {
        significand = "0";
    }
    return significand;
}

////////////////////////////////////////////////////////////////////////////////
int HexStringParser::getOffset(
    const std::string& strIntegerPart, const std::string& strDecimalPart ) {

    std::string strIntegerPart2 = strIntegerPart;

    replaceFirst( strIntegerPart2, "^0+", "" );

    //If the Integer part is a nonzero number.
    if( strIntegerPart.length() != 0 ) {
        std::string leadingNumber = strIntegerPart.substr( 0, 1 );
        return (int)( ( strIntegerPart.length() - 1) * 4 +
               countBitsLength(Long::parseLong( leadingNumber,HEX_RADIX ) ) - 1 );
    }

    //If the Integer part is a zero number.
    int i;
    for( i = 0; (std::size_t)i < strDecimalPart.length() && strDecimalPart.at(i) == '0'; i++ );

    if( (std::size_t)i == strDecimalPart.length() ) {
        return 0;
    }

    std::string leadingNumber = strDecimalPart.substr( i,i + 1 );

    return (-i - 1) * 4 +
        countBitsLength( Long::parseLong( leadingNumber, HEX_RADIX) ) - 1;
}

////////////////////////////////////////////////////////////////////////////////
int HexStringParser::countBitsLength(long long value) {
    int leadingZeros = Long::numberOfLeadingZeros( value );
    return Long::SIZE - leadingZeros;
}
