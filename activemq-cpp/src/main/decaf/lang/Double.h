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

#ifndef _DECAF_LANG_DOUBLE_H_
#define _DECAF_LANG_DOUBLE_H_

#include <decaf/util/Config.h>
#include <decaf/lang/Comparable.h>
#include <decaf/lang/Number.h>
#include <decaf/lang/exceptions/NumberFormatException.h>
#include <string>

namespace decaf{
namespace lang{

    class DECAF_API Double : public Number,
                             public Comparable<Double>,
                             public Comparable<double> {
    private:

        double value;

    public:

        /** The size in bits of the primitive int type */
        static const int SIZE = 64;

        /** The maximum value that the primitive type can hold */
        static const double MAX_VALUE;

        /** The minimum value that the primitive type can hold */
        static const double MIN_VALUE;

        /** Constant for the Not a Number Value */
        static const double NaN;

        /** Constant for Positive Infinity */
        static const double POSITIVE_INFINITY;

        /** Constant for Negative Infinitiy */
        static const double NEGATIVE_INFINITY;

    public:

        /**
         * Constructs a new instance of a Double object and assigns it the given value.
         *
         * @param value
         *      The primitive type to wrap.
         */
        Double( double value );

        /**
         * Constructs a new Double and attempts to convert the given string to a double
         * value, assigning it to the new object is successful or throwing a
         * NumberFormatException if the string is not a properly formatted double.
         *
         * @param value
         *      The string to convert to a primitive type to wrap.
         *
         * @throws NumberFormatException if the string is not a a valid double.
         */
        Double( const std::string& value );

        virtual ~Double() {}

        /**
         * Compares this Double instance with another.
         * @param d - the Double instance to be compared
         * @return zero if this object represents the same integer value as the
         * argument; a positive value if this object represents a value greater
         * than the passed in value, and -1 if this object repesents a value
         * less than the passed in value.
         */
        virtual int compareTo( const Double& d ) const;

        /**
         * @param d - the Double object to compare against.
         * @returns true if the two Double Objects have the same value.
         */
        bool equals( const Double& d ) const {
            return this->value == d.value;
        }

        /**
         * Compares equality between this object and the one passed.
         * @param d - the value to be compared to this one.
         * @return true if this object is equal to the one passed.
         */
        virtual bool operator==( const Double& d ) const {
            return this->value == d.value;
        }

        /**
         * Compares this object to another and returns true if this object
         * is considered to be less than the one passed.  This
         * @param d - the value to be compared to this one.
         * @return true if this object is equal to the one passed.
         */
        virtual bool operator<( const Double& d ) const {
            return this->value < d.value;
        }

        /**
         * Compares this Double instance with another.
         * @param d - the Double instance to be compared
         * @return zero if this object represents the same integer value as the
         * argument; a positive value if this object represents a value greater
         * than the passed in value, and -1 if this object represents a value
         * less than the passed in value.
         */
        virtual int compareTo( const double& d ) const;

        /**
         * @param d - the Double object to compare against.
         * @returns true if the two Double Objects have the same value.
         */
        bool equals( const double& d ) const {
            return this->value == d;
        }

        /**
         * Compares equality between this object and the one passed.
         * @param d - the value to be compared to this one.
         * @return true if this object is equal to the one passed.
         */
        virtual bool operator==( const double& d ) const {
            return this->value == d;
        }

        /**
         * Compares this object to another and returns true if this object
         * is considered to be less than the one passed.  This
         * @param d - the value to be compared to this one.
         * @return true if this object is equal to the one passed.
         */
        virtual bool operator<( const double& d ) const {
            return this->value < d;
        }

        /**
         * @returns this Double Object as a String Representation
         */
        std::string toString() const;

        /**
         * Answers the double value which the receiver represents
         * @return double the value of the receiver.
         */
        virtual double doubleValue() const {
            return this->value;
        }

        /**
         * Answers the float value which the receiver represents
         * @return float the value of the receiver.
         */
        virtual float floatValue() const {
            return (float)this->value;
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
         * @returns true if the double is equal to positive infinity.
         */
        bool isInfinite() const;

        /**
         * @returns true if the double is equal to NaN.
         */
        bool isNaN() const;

    public:  // Statics

        /**
         * Compares the two specified double values. The sign of the integer value
         * returned is the same as that of the integer that would be returned by the
         * call:
         *        new Double(d1).compareTo(new Double(d2))
         * @param d1 - the first double to compare
         * @param d2 - the second double to compare
         * @returns the value 0 if d1 is numerically equal to d2; a value less than
         * 0 if d1 is numerically less than d2; and a value greater than 0  if d1 is
         * numerically greater than d2.
         */
        static int compare( double d1, double d2 );

        /**
         * Returns a representation of the specified floating-point value according
         * to the IEEE 754 floating-point "double format" bit layout.
         *
         * Bit 63 (the bit that is selected by the mask 0x8000000000000000L)
         * represents the sign of the floating-point number. Bits 62-52 (the bits
         * that are selected by the mask 0x7ff0000000000000L) represent the exponent.
         * Bits 51-0 (the bits that are selected by the mask 0x000fffffffffffffL)
         * represent the significand (sometimes called the mantissa) of the
         * floating-point number.
         *
         * If the argument is positive infinity, the result is 0x7ff0000000000000L.
         * If the argument is negative infinity, the result is 0xfff0000000000000L.
         * If the argument is NaN, the result is 0x7ff8000000000000L.
         *
         * In all cases, the result is a long integer that, when given to the
         * longBitsToDouble(long) method, will produce a floating-point value the
         * same as the argument to doubleToLongBits (except all NaN values are
         * collapsed to a single "canonical" NaN value).
         * @param value - double to be converted
         * @returns the long long bits that make up the double
         */
        static long long doubleToLongBits( double value );

        /**
         * Returns a representation of the specified floating-point value according
         * to the IEEE 754 floating-point "double format" bit layout, preserving
         * Not-a-Number (NaN) values.
         *
         * Bit 63 (the bit that is selected by the mask 0x8000000000000000LL)
         * represents the sign of the floating-point number. Bits 62-52 (the bits
         * that are selected by the mask 0x7ff0000000000000L) represent the exponent.
         * Bits 51-0 (the bits that are selected by the mask 0x000fffffffffffffL)
         * represent the significand (sometimes called the mantissa) of the
         * floating-point number.
         *
         * If the argument is positive infinity, the result is 0x7ff0000000000000LL.
         * If the argument is negative infinity, the result is 0xfff0000000000000LL.
         * If the argument is NaN, the result is the long integer representing the
         * actual NaN value. Unlike the doubleToLongBits method, doubleToRawLongBits
         * does not collapse all the bit patterns encoding a NaN to a single
         * "canonical" NaN value.
         *
         * In all cases, the result is a long integer that, when given to the
         * longBitsToDouble(long) method, will produce a floating-point value the
         * same as the argument to doubleToRawLongBits.
         * @param value - double to be converted
         * @returns the long long bits that make up the double
         */
        static long long doubleToRawLongBits( double value );

        /**
         * @param value - The double to check.
         * @returns true if the double is equal to infinity.
         */
        static bool isInfinite( double value );

        /**
         * @param value - The double to check.
         * @returns true if the double is equal to NaN.
         */
        static bool isNaN( double value );

        /**
         * Returns the double value corresponding to a given bit representation.
         * The argument is considered to be a representation of a floating-point
         * value according to the IEEE 754 floating-point "double format" bit layout.
         *
         * If the argument is 0x7ff0000000000000L, the result is positive infinity.
         * If the argument is 0xfff0000000000000L, the result is negative infinity.
         * If the argument is any value in the range 0x7ff0000000000001L through
         * 0x7fffffffffffffffL or in the range 0xfff0000000000001L through
         * 0xffffffffffffffffL, the result is a NaN. No IEEE 754 floating-point
         * operation provided by C++ can distinguish between two NaN values of the
         * same type with different bit patterns. Distinct values of NaN are only
         * distinguishable by use of the Double.doubleToRawLongBits method.
         * @param bits - the long long bits to convert to double
         * @return the double converted from the bits
         */
        static double longBitsToDouble( long long bits );

        /**
         * Returns a new double initialized to the value represented by the
         * specified string, as performed by the valueOf method of class Double.
         * @param value - The string to parse to an double
         * @returns a double parsed from the passed string
         * @throws NumberFormatException
         */
        static double parseDouble( const std::string value );

        /**
         * Returns a hexadecimal string representation of the double argument. All
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
         *      o If m is a double value with a normalized representation, substrings
         *        are used to represent the significand and exponent fields. The
         *        significand is represented by the characters "0x1." followed by a
         *        lowercase hexadecimal representation of the rest of the
         *        significand as a fraction. Trailing zeros in the hexadecimal
         *        representation are removed unless all the digits are zero, in which
         *        case a single zero is used. Next, the exponent is represented by
         *        "p" followed by a decimal string of the unbiased exponent as if
         *        produced by a call to Integer.toString on the exponent value.
         *      o If m is a double value with a subnormal representation, the
         *        significand is represented by the characters "0x0." followed by a
         *        hexadecimal representation of the rest of the significand as a
         *        fraction. Trailing zeros in the hexadecimal representation are
         *        removed. Next, the exponent is represented by "p-126". Note that
         *        there must be at least one nonzero digit in a subnormal significand.
         *
         * @param value - The double to convert to a string
         * @returns the Hex formatted double string.
         */
        static std::string toHexString( double value );

        /**
         * Returns a string representation of the double argument. All characters
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
         * @param value - The double to convert to a string
         * @returns the formatted double string.
         */
        static std::string toString( double value );

        /**
         * Returns a Double instance representing the specified double value.
         * @param value - double to wrap
         * @returns new Double instance wrapping the primitive value
         */
        static Double valueOf( double value );

        /**
         * Returns a Double instance that wraps a primtive double which is parsed
         * from the string value passed.
         * @param value - the string to parse
         * @returns a new Double instance wrapping the double parsed from value
         * @throws NumberFormatException on error.
         */
        static Double valueOf( const std::string& value );

    private:

        static const long long DOUBLE_SIGN_MASK = 0x8000000000000000LL;
        static const long long DOUBLE_EXPONENT_MASK = 0x7FF0000000000000LL;
        static const long long DOUBLE_MANTISSA_MASK = 0x000FFFFFFFFFFFFFLL;
        static const long long DOUBLE_NAN_BITS =
            DOUBLE_EXPONENT_MASK | 0x0008000000000000LL;

    };

}}

#endif /*_DECAF_LANG_DOUBLE_H_*/
