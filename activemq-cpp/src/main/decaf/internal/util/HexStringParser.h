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

#ifndef _DECAF_INTERNAL_UTIL_HEXSTRINGPARSER_H_
#define _DECAF_INTERNAL_UTIL_HEXSTRINGPARSER_H_

#include <decaf/util/Config.h>
#include <string>

namespace decaf{
namespace internal{
namespace util{

    class HexStringParser {
    private:

        static const unsigned int DOUBLE_EXPONENT_WIDTH = 11;
        static const unsigned int DOUBLE_MANTISSA_WIDTH = 52;
        static const unsigned int FLOAT_EXPONENT_WIDTH = 8;
        static const unsigned int FLOAT_MANTISSA_WIDTH = 23;
        static const unsigned int HEX_RADIX = 16;
        static const unsigned int MAX_SIGNIFICANT_LENGTH = 15;

        static const std::string HEX_SIGNIFICANT;
        static const std::string BINARY_EXPONENT;
        static const std::string FLOAT_TYPE_SUFFIX;
        static const std::string HEX_PATTERN;

        // TODO
        //static final Pattern PATTERN = Pattern.compile(HEX_PATTERN);

    private:

        int EXPONENT_WIDTH;
        int MANTISSA_WIDTH;
        long long EXPONENT_BASE;
        long long MAX_EXPONENT;
        long long MIN_EXPONENT;
        long long MANTISSA_MASK;
        long long sign;
        long long exponent;
        long long mantissa;
        std::string abandonedNumber;

    public:

        /**
         * Create a new HexParser
         * @param exponentWidth - Width of the exponent for the type to parse
         * @param mantissaWidth - Width of the mantissa for the type to parse
         */
        HexStringParser( int exponentWidth, int mantissaWidth );

        virtual ~HexStringParser() {}

        /**
         * Parses a hex string using the specs given in the constructor
         * and returns a long long with the bits of the parsed string, the
         * caller can then convert those to a float or doulbe as needed.
         * @param hexString - string to parse
         * @returns the bits parsed from the string
         */
        long long parse( const std::string& hexString );

    private:

        /*
         * Parses the sign field.
         * @param sign string to parse
         */
        void parseHexSign( const std::string& signStr ) {
            this->sign = signStr.compare("-") == 0 ? 1 : 0;
        }

        /*
         * Parses the exponent field.
         * @param exponent string to parse
         */
        void parseExponent( const std::string& exponentStr );

        /*
         * Parses the mantissa field.
         * @param mantissa string to parse
         */
        void parseMantissa( const std::string& significantStr );

        void setInfinite() {
            exponent = MAX_EXPONENT;
            mantissa = 0;
        }

        void setZero() {
            exponent = 0;
            mantissa = 0;
        }

        /*
         * Sets the exponent variable to Long::MAX_VALUE or -Long::MAX_VALUE if
         * overflow or underflow happens.
         * @param the offset to set
         */
        void checkedAddExponent( long long offset );

        void processNormalNumber();
        void processSubNormalNumber();
        int countBitsLength( long long value );

        /*
         * Adjusts the mantissa to desired width for further analysis.
         */
        void fitMantissaInDesiredWidth( int desiredWidth );

        /*
         * Stores the discarded bits to abandonedNumber.
         */
        void discardTrailingBits( long long num );

        /*
         * The value is rounded up or down to the nearest infinitely precise result.
         * If the value is exactly halfway between two infinitely precise results,
         * then it should be rounded up to the nearest infinitely precise even.
         */
        void round();

        /*
         * Returns the normalized significand after removing the leading zeros.
         */
        std::string getNormalizedSignificand( const std::string& strIntegerPart,
                                              const std::string& strDecimalPart );

        /*
         * Calculates the offset between the normalized number and unnormalized
         * number. In a normalized representation, significand is represented by the
         * characters "0x1." followed by a lower-case hexadecimal representation of
         * the rest of the significand as a fraction.
         */
        int getOffset( const std::string& strIntegerPart,
                       const std::string& strDecimalPart );

    public:  // Statics

        /*
         * Parses the hex string to a double number.
         * @param hexString - string to parse
         * @returns the parsed double value
         */
        static double parseDouble( const std::string& hexString );

        /*
         * Parses the hex string to a float number.
         * @param hexString - string to parse
         * @returns the parsed float value
         */
        static float parseFloat( const std::string& hexString );


    private:  // Static

        /*
         * Analyzes the hex string and extracts the sign and digit segments.
         * @param hexString - string to parse
         * @returns array of three strings holding the segments caller owns
         */
        static std::string* getSegmentsFromHexString( const std::string& hexString );

        std::string& replaceFirst( std::string& target,
                                   const std::string& find,
                                   const std::string& replace ) {

            std::string::size_type pos = std::string::npos;

            if( ( pos = target.find_first_of( find, 0 ) ) != std::string::npos ) {
                return target.replace( pos, find.length(), replace );
            }

            return target;
        }

        std::string& replaceAll( std::string& target,
                                 const std::string& find,
                                 const std::string& replace ) {

            std::string::size_type pos = std::string::npos;
            while( ( pos = target.find( find ) ) != std::string::npos ) {
                target.replace( pos, find.length(), replace );
            }

            return target;
        }

    };

}}}

#endif /*_DECAF_INTERNAL_UTIL_HEXSTRINGPARSER_H_*/
