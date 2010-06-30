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

#ifndef _DECAF_LANG_FLOAT_H_
#define _DECAF_LANG_FLOAT_H_

#include <decaf/util/Config.h>
#include <decaf/lang/Number.h>
#include <decaf/lang/Comparable.h>
#include <decaf/lang/exceptions/NumberFormatException.h>
#include <string>

namespace decaf{
namespace lang{

    class DECAF_API Float : public Number,
                            public Comparable<Float>,
                            public Comparable<float> {
    private:

        float value;

    public:

        /** The size in bits of the primitive int type */
        static const int SIZE = 32;

        /** The maximum value that the primitive type can hold */
        static const float MAX_VALUE;

        /** The minimum value that the primitive type can hold */
        static const float MIN_VALUE;

        /** Constant for the Not a Number Value */
        static const float NaN;

        /** Constant for Positive Infinity */
        static const float POSITIVE_INFINITY;

        /** Constant for Negative Infinity */
        static const float NEGATIVE_INFINITY;

    public:

        /**
         * @param value - the primitive type to wrap
         */
        Float( float value );

        /**
         * @param value - the primitive type to wrap
         */
        Float( double value );

        /**
         * @param value - the string to convert to a primitive type to wrap
         */
        Float( const std::string& value );

        virtual ~Float() {}

        /**
         * Compares this Float instance with another.
         * @param f - the Float instance to be compared
         * @return zero if this object represents the same integer value as the
         * argument; a positive value if this object represents a value greater
         * than the passed in value, and -1 if this object represents a value
         * less than the passed in value.
         */
        virtual int compareTo( const Float& f ) const;

        /**
         * @param f - the Float object to compare against.
         * @returns true if the two Float Objects have the same value.
         */
        bool equals( const Float& f ) const {
            return this->value == f.value;
        }

        /**
         * Compares equality between this object and the one passed.
         * @param f - the value to be compared to this one.
         * @return true if this object is equal to the one passed.
         */
        virtual bool operator==( const Float& f ) const {
            return this->value == f.value;
        }

        /**
         * Compares this object to another and returns true if this object
         * is considered to be less than the one passed.  This
         * @param f - the value to be compared to this one.
         * @return true if this object is equal to the one passed.
         */
        virtual bool operator<( const Float& f ) const {
            return this->value < f.value;
        }

        /**
         * Compares this Float instance with another.
         * @param f - the Float instance to be compared
         * @return zero if this object represents the same integer value as the
         * argument; a positive value if this object represents a value greater
         * than the passed in value, and -1 if this object represents a value
         * less than the passed in value.
         */
        virtual int compareTo( const float& f ) const;

        /**
         * @param f - the Float object to compare against.
         * @returns true if the two Float Objects have the same value.
         */
        bool equals( const float& f ) const {
            return this->value == f;
        }

        /**
         * Compares equality between this object and the one passed.
         * @param f - the value to be compared to this one.
         * @return true if this object is equal to the one passed.
         */
        virtual bool operator==( const float& f ) const {
            return this->value == f;
        }

        /**
         * Compares this object to another and returns true if this object
         * is considered to be less than the one passed.  This
         * @param f - the value to be compared to this one.
         * @return true if this object is equal to the one passed.
         */
        virtual bool operator<( const float& f ) const {
            return this->value < f;
        }

        /**
         * @returns this Float Object as a String Representation
         */
        std::string toString() const;

        /**
         * Answers the double value which the receiver represents
         * @return double the value of the receiver.
         */
        virtual double doubleValue() const {
            return (double)this->value;
        }

        /**
         * Answers the float value which the receiver represents
         * @return float the value of the receiver.
         */
        virtual float floatValue() const {
            return this->value;
        }

        /**
         * Answers the byte value which the receiver represents
         * @return byte the value of the receiver.
         */
        virtual unsigned char byteValue() const {
            return (unsigned char)this->value;
        }

        /**
         * Answers the short value which the receiver represents
         * @return short the value of the receiver.
         */
        virtual short shortValue() const {
            return (short)this->value;
        }

        /**
         * Answers the int value which the receiver represents
         * @return int the value of the receiver.
         */
        virtual int intValue() const {
            return (int)this->value;
        }

        /**
         * Answers the long value which the receiver represents
         * @return long the value of the receiver.
         */
        virtual long long longValue() const {
            return (long long)this->value;
        }

        /**
         * @returns true if the float is equal to positive infinity.
         */
        bool isInfinite() const;

        /**
         * @returns true if the float is equal to NaN.
         */
        bool isNaN() const;

    public:   // Statics

        /**
         * Compares the two specified double values. The sign of the integer value
         * returned is the same as that of the integer that would be returned by the
         * call: Float( f1 ).compareTo( Float( f2) )
         * @param f1 - the first double to compare
         * @param f2 - the second double to compare
         * @returns the value 0 if d1 is numerically equal to f2; a value less than
         * 0 if f1 is numerically less than f2; and a value greater than 0  if f1 is
         * numerically greater than f2.
         */
        static int compare( float f1, float f2 );

        /**
         * Returns a representation of the specified floating-point value according
         * to the IEEE 754 floating-point "single format" bit layout.
         *
         * Bit 31 (the bit that is selected by the mask 0x80000000) represents the
         * sign of the floating-point number. Bits 30-23 (the bits that are selected
         * by the mask 0x7f800000) represent the exponent. Bits 22-0 (the bits that
         * are selected by the mask 0x007fffff) represent the significand (sometimes
         * called the mantissa) of the floating-point number.
         *
         * If the argument is positive infinity, the result is 0x7f800000.
         * If the argument is negative infinity, the result is 0xff800000.
         * If the argument is NaN, the result is 0x7fc00000.
         *
         * In all cases, the result is an integer that, when given to the
         * intBitsToFloat(int) method, will produce a floating-point value the
         * same as the argument to floatToIntBits (except all NaN values are
         * collapsed to a single "canonical" NaN value).
         * @param value - the float to convert to int bits
         * @returns the int that holds the float's value
         */
        static int floatToIntBits( float value );

        /**
         * Returns a representation of the specified floating-point value according
         * to the IEEE 754 floating-point "single format" bit layout, preserving
         * Not-a-Number (NaN) values.
         *
         * Bit 31 (the bit that is selected by the mask 0x80000000) represents the
         * sign of the floating-point number. Bits 30-23 (the bits that are selected
         * by the mask 0x7f800000) represent the exponent. Bits 22-0 (the bits that
         * are selected by the mask 0x007fffff) represent the significand (sometimes
         * called the mantissa) of the floating-point number.
         *
         * If the argument is positive infinity, the result is 0x7f800000.
         * If the argument is negative infinity, the result is 0xff800000.
         * If the argument is NaN, the result is the integer representing the
         * actual NaN value. Unlike the floatToIntBits method, intToRawIntBits
         * does not collapse all the bit patterns encoding a NaN to a single
         * "canonical" NaN value.
         *
         * In all cases, the result is an integer that, when given to the
         * intBitsToFloat(int) method, will produce a floating-point value the same
         * as the argument to floatToRawIntBits.
         * @param value
         *      The float to convert to a raw int.
         * @returns the raw int value of the float
         */
        static int floatToRawIntBits( float value );

        /**
         * Returns the float value corresponding to a given bit representation. The
         * argument is considered to be a representation of a floating-point value
         * according to the IEEE 754 floating-point "single format" bit layout.
         *
         * If the argument is 0x7f800000, the result is positive infinity.
         * If the argument is 0xff800000, the result is negative infinity.
         * If the argument is any value in the range 0x7f800001 through 0x7fffffff
         * or in the range 0xff800001 through 0xffffffff, the result is a NaN. No
         * IEEE 754 floating-point operation provided by C++ can distinguish
         * between two NaN values of the same type with different bit patterns.
         * Distinct values of NaN are only distinguishable by use of the
         * Float::floatToRawIntBits method.
         *
         * @param bits - the bits of the float encoded as a float
         * @return a new float created from the int bits.
         */
        static float intBitsToFloat( int bits );

        /**
         * @param value - The float to check.
         * @returns true if the float is equal to infinity.
         */
        static bool isInfinite( float value );

        /**
         * @param value - The float to check.
         * @returns true if the float is equal to NaN.
         */
        static bool isNaN( float value );

        /**
         * Returns a new float initialized to the value represented by the
         * specified string, as performed by the valueOf method of class Float.
         * @param value - the string to parse
         * @returns a float parsed from the string
         * @throw NumberFormatException
         */
        static float parseFloat( const std::string& value );

        /**
         * Returns a hexadecimal string representation of the float argument. All
         * characters mentioned below are ASCII characters.
         *
         *  * If the argument is NaN, the result is the string "NaN".
         *  * Otherwise, the result is a string that represents the sign and magnitude
         *    (absolute value) of the argument. If the sign is negative, the first
         *    character of the result is '-'; if the sign is positive, no sign
         *    character appears in the result. As for the magnitude m:
         *      o If m is infinity, it is represented by the string "Infinity"; thus,
         *        positive infinity produces the result "Infinity" and negative
         *        infinity produces the result "-Infinity".
         *      o If m is zero, it is represented by the string "0x0.0p0"; thus,
         *        negative zero produces the result "-0x0.0p0" and positive zero
         *        produces the result "0x0.0p0".
         *      o If m is a float value with a normalized representation, substrings
         *        are used to represent the significand and exponent fields. The
         *        significand is represented by the characters "0x1." followed by a
         *        lowercase hexadecimal representation of the rest of the
         *        significand as a fraction. Trailing zeros in the hexadecimal
         *        representation are removed unless all the digits are zero, in which
         *        case a single zero is used. Next, the exponent is represented by
         *        "p" followed by a decimal string of the unbiased exponent as if
         *        produced by a call to Integer.toString on the exponent value.
         *      o If m is a float value with a subnormal representation, the
         *        significand is represented by the characters "0x0." followed by a
         *        hexadecimal representation of the rest of the significand as a
         *        fraction. Trailing zeros in the hexadecimal representation are
         *        removed. Next, the exponent is represented by "p-126". Note that
         *        there must be at least one nonzero digit in a subnormal significand.
         *
         * @param value - The float to convert to a string
         * @returns the Hex formatted float string.
         */
        static std::string toHexString( float value );

        /**
         * Returns a string representation of the float  argument. All characters
         * mentioned below are ASCII characters.
         *
         * If the argument is NaN, the result is the string "NaN".
         * Otherwise, the result is a string that represents the sign and magnitude
         * (absolute value) of the argument. If the sign is negative, the first
         * character of the result is '-'; if the sign is positive, no
         * sign character appears in the result. As for the magnitude m:
         *  o If m is infinity, it is represented by the characters "Infinity"; thus,
         *    positive infinity produces the result "Infinity" and negative infinity
         *    produces the result "-Infinity".
         *  o If m is zero, it is represented by the characters "0.0"; thus, negative
         *    zero produces the result "-0.0" and positive zero produces the result
         *    "0.0".
         *  o If m is greater than or equal to 10-3 but less than 107, then it is
         *    represented as the integer part of m, in decimal form with no leading
         *    zeroes, followed by '.', followed by one or more decimal digits
         *    representing the fractional part of m.
         *  o If m is less than 10-3 or greater than or equal to 107, then it is
         *    represented in so-called "computerized scientific notation." Let n be
         *    the unique integer such that 10n <= m < 10n+1; then let a be the
         *    mathematically exact quotient of m and 10n so that 1 <= a < 10.
         *    The magnitude is then represented as the integer part of a, as a
         *    single decimal digit, followed by '.', followed by decimal digits
         *    representing the fractional part of a, followed by the letter 'E',
         *    followed by a representation of n as a decimal integer, as produced
         *    by the method Integer.toString(int).
         * @param value - The float to convert to a string
         * @returns the formatted float string.
         */
        static std::string toString( float value );

        /**
         * Returns a Float instance representing the specified float value.
         * @param value - float to wrap
         * @returns new Float instance wrapping the primitive value
         */
        static Float valueOf( float value );

        /**
         * Returns a Float instance that wraps a primitive float which is parsed
         * from the string value passed.
         *
         * @param value - the string to parse
         * @returns a new Float instance wrapping the float parsed from value
         * @throws NumberFormatException on error.
         */
        static Float valueOf( const std::string& value );

    private:

        static const unsigned int SINGLE_EXPONENT_MASK = 0x7F800000;
        static const unsigned int SINGLE_MANTISSA_MASK = 0x007FFFFF;
        static const unsigned int SINGLE_NAN_BITS = (SINGLE_EXPONENT_MASK | 0x00400000);

    };

}}

#endif /*_DECAF_LANG_FLOAT_H_*/
