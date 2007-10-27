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

#ifndef _DECAF_INTERNAL_UTIL_FLOATINGPOINTPARSER_H_
#define _DECAF_INTERNAL_UTIL_FLOATINGPOINTPARSER_H_

#include <decaf/util/Config.h>
#include <decaf/lang/exceptions/NumberFormatException.h>
#include <string>

namespace decaf{
namespace internal{
namespace util{

    /**
     * Used to parse a string and return either a single or double precision
     * floating point number.
     */
    class FloatingPointParser {
    private:

        class StringExponentPair {
        public:

            std::string value;
            int exp;
            bool negative;

            StringExponentPair( const std::string value, int exp, bool negative ) {
                this->value = value;
                this->exp = exp;
                this->negative = negative;
            }
        };

        static const int MAX_ACCURACY_WIDTH = 17;
        static const int DEFAULT_WIDTH = MAX_ACCURACY_WIDTH;
        static const int LOG5_OF_TWO_TO_THE_N = 11;

        static const int tens[11];

    public:

        virtual ~FloatingPointParser() {}

    public:

        /**
         * Returns the closest double value to the real number in the string.
         * @param value - the String that will be parsed to a floating point
         * @return the double closest to the real number
         * @exception NumberFormatException if value is not a valid Number
         */
        static double parseDouble( const std::string& value )
            throw ( lang::exceptions::NumberFormatException );

        /**
         * Returns the closest float value to the real number in the string.
         * @param value - the String that will be parsed to a floating point
         * @return the float closest to the real number
         * @exception NumberFormatException if value is not a valid Number
         */
        static float parseFloat( const std::string& value )
            throw ( lang::exceptions::NumberFormatException );

    private:

        static int sizeOfTenToTheE( int e) {
            return ( e / 19 ) + 1;
        }

        /**
         * Takes a String and an integer exponent. The String should hold a positive
         * integer value (or zero). The exponent will be used to calculate the
         * floating point number by taking the positive integer the String
         * represents and multiplying by 10 raised to the power of the of the
         * exponent. Returns the closest double value to the real number
         * @param value - the String that will be parsed to a floating point
         * @param exp - an int represent the 10 to part
         * @return the double closest to the real number
         * @exception NumberFormatException if string isn't a valid number
         */
        static double parseDblImpl( const std::string& value, int exp )
            throw ( lang::exceptions::NumberFormatException );

        /**
         * Takes a String and an integer exponent. The String should hold a positive
         * integer value (or zero). The exponent will be used to calculate the
         * floating point number by taking the positive integer the String
         * represents and multiplying by 10 raised to the power of the of the
         * exponent. Returns the closest float value to the real number
         * @param value - the String that will be parsed to a floating point
         * @param exp - an int represent the 10 to part
         * @return the float closest to the real number
         * @exception NumberFormatException fi the string is not a valid number
         */
        static float parseFltImpl( const std::string& value, int exp )
            throw ( lang::exceptions::NumberFormatException );

        /**
         * Takes a String and does some initial parsing. Should return a
         * StringExponentPair containing a String with no leading or trailing white
         * space and trailing zeroes eliminated. The exponent of the
         * StringExponentPair will be used to calculate the floating point number by
         * taking the positive integer the String represents and multiplying by 10
         * raised to the power of the of the exponent.
         * @param value - the String that will be parsed to a floating point
         * @param length - the length of s
         * @return a StringExponentPair with necessary values
         * @exception NumberFormatException
         */
        static StringExponentPair initialParse( const std::string& value, int length )
            throw ( lang::exceptions::NumberFormatException );

        /*
         * Assumes the string is trimmed.
         * @param value - the String that will be parsed to a floating point
         * @param length - the length of s
         * @return the parsed double
         * @exception NumberFormatException
         */
        static double parseDblName( const std::string& namedDouble, int length )
            throw ( lang::exceptions::NumberFormatException );

        /*
         * Assumes the string is trimmed.
         * @param value - the String that will be parsed to a floating point
         * @param length - the length of s
         * @return the parsed double
         * @exception NumberFormatException
         */
        static float parseFltName( const std::string& namedFloat, int length )
            throw ( lang::exceptions::NumberFormatException );

        /**
         * Trims the leading and trailing whitespace from a string
         * @param value - reference to the string to trim
         * @param returns a reference to the string passed for chaining
         */
        static std::string& trim( std::string& value );

        /**
         * Turns all letters in the string into lowercase.
         * @param value - reference to the string to trim
         * @param returns a reference to the string passed for chaining
         */
        static std::string& toLowerCase( std::string& value );

        /**
         * Create a float from the given string and exponent
         */
        static float createFloat( const std::string& s, int exp );

        /**
         * Create a float from the given long long array and exponent
         */
        static float createFloat1( unsigned long long* f, int length, int exp );

        /**
         * The algorithm for the function floatAlgorithm() below can be found
         * in:
         *
         *      "How to Read Floating-Point Numbers Accurately", William D.
         *      Clinger, Proceedings of the ACM SIGPLAN '90 Conference on
         *      Programming Language Design and Implementation, June 20-22,
         *      1990, pp. 92-101.
         *
         * There is a possibility that the function will end up in an endless
         * loop if the given approximating floating-point number (a very small
         * floating-point whose value is very close to zero) straddles between
         * two approximating integer values. We modified the algorithm slightly
         * to detect the case where it oscillates back and forth between
         * incrementing and decrementing the floating-point approximation. It
         * is currently set such that if the oscillation occurs more than twice
         * then return the original approximation.
         * @returns a newly parse float.
         */
        static float floatAlgorithm( unsigned long long* f, int length,
                                     int e, float z );

        static int tenToTheE( int exp ) {
            return *(tens + exp);
        }

    };

}}}

#endif /*_DECAF_INTERNAL_UTIL_FLOATINGPOINTPARSER_H_*/
