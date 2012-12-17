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

#ifndef _DECAF_LANG_LONG_H_
#define _DECAF_LANG_LONG_H_

#include <decaf/lang/Number.h>
#include <decaf/lang/Comparable.h>
#include <decaf/lang/exceptions/NumberFormatException.h>
#include <string>

namespace decaf{
namespace lang{

    class DECAF_API Long : public Number,
                           public Comparable<Long>,
                           public Comparable<long long> {
    private:

        // The primitive long long value
        long long value;

    public:

        /** The size in bits of the primitive long long type */
        static const int SIZE = 64;

        /** The maximum value that the primitive type can hold */
        static const long long MAX_VALUE = (long long)0x7FFFFFFFFFFFFFFFLL;

        /** The minimum value that the primitive type can hold */
        static const long long MIN_VALUE = (long long)0x8000000000000000LL;

    public:

        /**
         * @param value - the primitive long long to wrap
         */
        Long( long long value );

        /**
         * Constructs a new Long and attempts to convert the given string to an long long
         * value, assigning it to the new object is successful or throwing a
         * NumberFormatException if the string is not a properly formatted long long.
         *
         * @param value
         *      The string to convert to a primitive type to wrap.
         *
         * @throws NumberFormatException if the string is not a a valid 64bit long.
         */
        Long( const std::string& value );

        virtual ~Long() {}

        /**
         * Compares this Long instance with another.
         * @param l - the Long instance to be compared
         * @return zero if this object represents the same integer value as the
         * argument; a positive value if this object represents a value greater
         * than the passed in value, and -1 if this object represents a value
         * less than the passed in value.
         */
        virtual int compareTo( const Long& l ) const;

        /**
         * @param l - the Long object to compare against.
         * @returns true if the two Integer Objects have the same value.
         */
        bool equals( const Long& l ) const {
            return this->value == l.value;
        }

        /**
         * Compares equality between this object and the one passed.
         * @param l - the value to be compared to this one.
         * @return true if this object is equal to the one passed.
         */
        virtual bool operator==( const Long& l ) const {
            return this->value == l.value;
        }

        /**
         * Compares this object to another and returns true if this object
         * is considered to be less than the one passed.  This
         * @param l - the value to be compared to this one.
         * @return true if this object is equal to the one passed.
         */
        virtual bool operator<( const Long& l ) const {
            return this->value < l.value;
        }

        /**
         * Compares this Long instance with another.
         * @param l - the Integer instance to be compared
         * @return zero if this object represents the same integer value as the
         * argument; a positive value if this object represents a value greater
         * than the passed in value, and -1 if this object represents a value
         * less than the passed in value.
         */
        virtual int compareTo( const long long& l ) const;

        /**
         * @param l - the Long object to compare against.
         * @returns true if the two Integer Objects have the same value.
         */
        bool equals( const long long& l ) const {
            return this->value == l;
        }

        /**
         * Compares equality between this object and the one passed.
         * @param l - the value to be compared to this one.
         * @return true if this object is equal to the one passed.
         */
        virtual bool operator==( const long long& l ) const {
            return this->value == l;
        }

        /**
         * Compares this object to another and returns true if this object
         * is considered to be less than the one passed.  This
         * @param l - the value to be compared to this one.
         * @return true if this object is equal to the one passed.
         */
        virtual bool operator<( const long long& l ) const {
            return this->value < l;
        }

        /**
         * @returns this Long Object as a String Representation
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
            return (float)this->value;
        }

        /**
         * Answers the byte value which the receiver represents
         * @return int the value of the receiver.
         */
        virtual unsigned char byteValue() const {
            return (unsigned char)this->value;
        }

        /**
         * Answers the short value which the receiver represents
         * @return int the value of the receiver.
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
            return this->value;
        }

    public:

        /**
         * Returns the number of one-bits in the two's complement binary
         * representation of the specified int value. This function is sometimes
         * referred to as the population count.
         * @param value - the long long to count
         * @return the number of one-bits in the two's complement binary
         * representation of the specified long long value.
         */
        static int bitCount( long long value );

        /**
         * Decodes a String into a Long. Accepts decimal, hexadecimal, and octal
         * numbers given by the following grammar:
         *
         * The sequence of characters following an (optional) negative sign and/or
         * radix specifier ("0x", "0X", "#", or leading zero) is parsed as by the
         * Integer.parseInteger method with the indicated radix (10, 16, or 8). This
         * sequence of characters must represent a positive value or a
         * NumberFormatException will be thrown. The result is negated if first
         * character of the specified String is the minus sign. No whitespace
         * characters are permitted in the string.
         * @param value - The string to decode
         * @returns a Long object containing the decoded value
         * @throws NumberFomatException if the string is not formatted correctly.
         */
        static Long decode( const std::string& value );

        /**
         * Returns an long long value with at most a single one-bit, in the position
         * of the highest-order ("leftmost") one-bit in the specified int value.
         * Returns zero if the specified value has no one-bits in its two's
         * complement binary representation, that is, if it is equal to zero.
         * @param value - the long long to be inspected
         * @return an long long value with a single one-bit, in the position of the
         * highest-order one-bit in the specified value, or zero if the specified
         * value is itself equal to zero.
         */
        static long long highestOneBit( long long value );

        /**
         * Returns an long long value with at most a single one-bit, in the position
         * of the lowest-order ("rightmost") one-bit in the specified int value.
         * Returns zero if the specified value has no one-bits in its two's
         * complement binary representation, that is, if it is equal to zero.
         * @param value - the long long to be inspected
         * @return an long long value with a single one-bit, in the position of the
         * lowest-order one-bit in the specified value, or zero if the specified
         * value is itself equal to zero.
         */
        static long long lowestOneBit( long long value );

        /**
         * Returns the number of zero bits preceding the highest-order ("leftmost")
         * one-bit in the two's complement binary representation of the specified
         * long long value. Returns 64 if the specified value has no one-bits in its
         * two's complement representation, in other words if it is equal to zero.
         *
         * Note that this method is closely related to the logarithm base 2. For
         * all positive int values x:
         *
         *     * floor( log2(x)) = 63 - numberOfLeadingZeros(x)
         *     * ceil( log2(x)) = 64 - numberOfLeadingZeros(x - 1)
         *
         * @param value - the long long to be inspected
         * @return the number of zero bits preceding the highest-order ("leftmost")
         * one-bit in the two's complement binary representation of the specified
         * long long value, or 64 if the value is equal to zero.
         */
        static int numberOfLeadingZeros( long long value );

        /**
         * Returns the number of zero bits following the lowest-order ("rightmost")
         * one-bit in the two's complement binary representation of the specified
         * long long value. Returns 64 if the specified value has no one-bits in its
         * two's complement representation, in other words if it is equal to zero.
         * @param value - the int to be inspected
         * @return the number of zero bits following the lowest-order ("rightmost")
         * one-bit in the two's complement binary representation of the specified
         * long long value, or 64 if the value is equal to zero.
         */
        static int numberOfTrailingZeros( long long value );

        /**
         * Parses the string argument as a signed decimal long. The characters in the
         * string must all be decimal digits, except that the first character may be
         * an ASCII minus sign '-' to indicate a negative value. The resulting long
         * value is returned, exactly as if the argument and the radix 10 were given
         * as arguments to the parseLong(java.lang.String, int) method.
         *
         * Note that the characters LL or ULL are not permitted to appear at the end
         * of this string as would normally be permitted in a C++ program.
         * @param value - String to parse
         * @return long long value
         * @throws NumberFormatException on invalid string value
         */
        static long long parseLong( const std::string& value );

        /**
         * Returns a Long object holding the value extracted from the specified
         * string when parsed with the radix given by the second argument. The
         * first argument is interpreted as representing a signed long in the radix
         * specified by the second argument, exactly as if the arguments were
         * given to the parseLong(std::string, int) method. The result is a
         * Long object that represents the long long value specified by the string.
         * @param value - String to parse
         * @param radix - the base encoding of the string
         * @return long long value
         * @throws NumberFormatException on invalid string value
         */
        static long long parseLong( const std::string& value, int radix );

        /**
         * Returns the value obtained by reversing the order of the bytes in the
         * two's complement representation of the specified long long value.
         * @param value - the long long whose bytes we are to reverse
         * @return the reversed long long.
         */
        static long long reverseBytes( long long value );

        /**
         * Returns the value obtained by reversing the order of the bits in the
         * two's complement binary representation of the specified long long value.
         * @param value - the value whose bits are to be reversed
         * @returns the reversed bits long long.
         */
        static long long reverse( long long value );

        /**
         * Returns the value obtained by rotating the two's complement binary
         * representation of the specified value left by the specified number
         * of bits. (Bits shifted out of the left hand, or high-order, side reenter
         * on the right, or low-order.)
         *
         * Note that left rotation with a negative distance is equivalent to right
         * rotation: rotateLeft(val, -distance) == rotateRight(val, distance). Note
         * also that rotation by any multiple of 32 is a no-op, so all but the last
         * five bits of the rotation distance can be ignored, even if the distance
         * is negative: rotateLeft(val, distance) == rotateLeft(val, distance & 0x1F).
         * @param value - the long long to be inspected
         * @param distance - the number of bits to rotate
         * @return the value obtained by rotating the two's complement binary
         * representation of the specified value left by the specified number
         * of bits.
         */
        static long long rotateLeft( long long value, int distance );

        /**
         * Returns the value obtained by rotating the two's complement binary
         * representation of the specified value right by the specified number
         * of bits. (Bits shifted out of the right hand, or low-order, side reenter
         * on the left, or high-order.)
         *
         * Note that right rotation with a negative distance is equivalent to left
         * rotation: rotateRight(val, -distance) == rotateLeft(val, distance). Note
         * also that rotation by any multiple of 32 is a no-op, so all but the last
         * five bits of the rotation distance can be ignored, even if the distance is
         * negative: rotateRight(val, distance) == rotateRight(val, distance & 0x1F).
         * @param value - the long long to be inspected
         * @param distance - the number of bits to rotate
         * @return the value obtained by rotating the two's complement binary
         * representation of the specified value right by the specified number
         * of bits.
         */
        static long long rotateRight( long long value, int distance );

        /**
         * Returns the signum function of the specified value. (The return value
         * is -1 if the specified value is negative; 0 if the specified value is zero;
         * and 1 if the specified value is positive.)
         * @param value - the long long to be inspected
         * @return the signum function of the specified long long value.
         */
        static int signum( long long value );

        /**
         * Converts the long to a String representation
         * @param value The long to convert to a std::string.
         * @return string representation
         */
        static std::string toString( long long value );

        /*
         * Returns a string representation of the first argument in the radix
         * specified by the second argument.
         *
         * If the radix is smaller than Character.MIN_RADIX or larger than
         * Character.MAX_RADIX, then the radix 10 is used instead.
         *
         * If the first argument is negative, the first element of the result is
         * the ASCII minus character '-'. If the first argument is not
         * negative, no sign character appears in the result.
         *
         * The remaining characters of the result represent the magnitude of the
         * first argument. If the magnitude is zero, it is represented by a single
         *  zero character '0'; otherwise, the first character of the
         * representation of the magnitude will not be the zero character. The
         * following ASCII characters are used as digits:
         *
         *    0123456789abcdefghijklmnopqrstuvwxyz
         *
         * @param value - the long long to convert to a string
         * @param radix - the radix to format the string in
         * @returns an long long formatted to the string value of the radix given.
         */
        static std::string toString( long long value, int radix );

        /**
         * Returns a string representation of the integer argument as an unsigned
         * integer in base 16.
         *
         * The unsigned integer value is the argument plus 2^32 if the argument is
         * negative; otherwise, it is equal to the argument. This value is converted
         * to a string of ASCII digits in hexadecimal (base 16) with no extra leading
         * 0s. If the unsigned magnitude is zero, it is represented by a single zero
         * character '0'; otherwise, the first character of the representation of the
         * unsigned magnitude will not be the zero character. The following characters
         * are used as hexadecimal digits:
         *
         * 		0123456789abcdef
         *
         * If uppercase letters are desired, the toUpperCase() method may be called
         * on the result:
         * @param value - the long long to be translated to an Octal string
         * @returns the unsigned long long value as a Octal string
         */
        static std::string toHexString( long long value );

        /**
         * Returns a string representation of the long long argument as an unsigned
         * long long in base 8.
         *
         * The unsigned long long value is the argument plus 2^32 if the argument is
         * negative; otherwise, it is equal to the argument. This value is converted
         * to a string of ASCII digits in octal (base 8) with no extra leading 0s.
         *
         * If the unsigned magnitude is zero, it is represented by a single zero
         * character '0'; otherwise, the first character of the representation
         * of the unsigned magnitude will not be the zero character. The following
         * characters are used as octal digits:
         *
         *      01234567
         *
         * @param value - the long long to be translated to an Octal string
         * @returns the unsigned long long value as a Octal string
         */
        static std::string toOctalString( long long value );

        /**
         * Returns a string representation of the long long argument as an unsigned
         * long long in base 2.
         *
         * The unsigned long long value is the argument plus 2^32 if the argument is
         * negative; otherwise it is equal to the argument. This value is converted
         * to a string of ASCII digits in binary (base 2) with no extra leading 0s.
         * If the unsigned magnitude is zero, it is represented by a single zero
         * character '0'; otherwise, the first character of the
         * representation of the unsigned magnitude will not be the zero character.
         * The characters '0' and '1' are used as binary
         * digits.
         * @param value - the long long to be translated to a binary string
         * @returns the unsigned long long value as a binary string
         */
        static std::string toBinaryString( long long value );

        /**
         * Returns a Long instance representing the specified int value.
         * @param value - the long long to wrap
         * @return the new Integer object wrapping value.
         */
        static Long valueOf( long long value ) {
            return Long( value );
        }

        /**
         * Returns a Long object holding the value given by the specified
         * std::string.  The argument is interpreted as representing a signed
         * decimal long long, exactly as if the argument were given to the
         * parseLong( std::string ) method. The result is a Integer object that
         * represents the long long value specified by the string.
         * @param value - std::string to parse as base 10
         * @return new Long Object wrapping the primitive
         * @throws NumberFormatException if the string is not a decimal long long.
         */
        static Long valueOf( const std::string& value );

        /**
         * Returns a Long object holding the value extracted from the specified
         * std::string when parsed with the radix given by the second argument.
         * The first argument is interpreted as representing a signed long long in the
         * radix specified by the second argument, exactly as if the argument were
         * given to the parseLong( std::string, int ) method. The result is a
         * Long object that represents the long long value specified by the string.
         * @param value - std::string to parse as base ( radix )
         * @param radix - base of the string to parse.
         * @return new Long Object wrapping the primitive
         * @throws NumberFormatException if the string is not a valid long long.
         */
        static Long valueOf( const std::string& value, int radix );

    private:

        static long long parse( const std::string& value, int offset,
                                int radix, bool negative );

    };

}}

#endif /*_DECAF_LANG_LONG_H_*/
