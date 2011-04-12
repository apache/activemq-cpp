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

#ifndef _DECAF_LANG_INTEGER_H_
#define _DECAF_LANG_INTEGER_H_

#include <decaf/util/Config.h>
#include <decaf/lang/Number.h>
#include <decaf/lang/Comparable.h>
#include <string>
#include <decaf/lang/exceptions/NumberFormatException.h>

namespace decaf{
namespace lang{

    class DECAF_API Integer : public Number,
                              public Comparable<Integer>,
                              public Comparable<int> {
    private:

        // The primitve Integer value.
        int value;

    public:

        /** The size in bits of the primitive int type */
        static const int SIZE = 32;

        /** The maximum value that the primitive type can hold */
        static const int MAX_VALUE = (int)0x7FFFFFFF;

        /** The minimum value that the primitive type can hold */
        static const int MIN_VALUE = (int)0x80000000;

    public:

        /**
         * @param value
         *      The primitive value to wrap in an <code>Integer</code> instance.
         */
        Integer( int value );

        /**
         * Constructs a new Integer and attempts to convert the given string to an int
         * value, assigning it to the new object is successful or throwing a
         * NumberFormatException if the string is not a properly formatted int.
         *
         * @param value
         *      The string to convert to a primitive type to wrap.
         *
         * @throws NumberFormatException if the string is not a a valid integer.
         */
        Integer( const std::string& value );

        virtual ~Integer() {}

        /**
         * Compares this Integer instance with another.
         * @param i - the Integer instance to be compared
         * @return zero if this object represents the same integer value as the
         * argument; a positive value if this object represents a value greater
         * than the passed in value, and -1 if this object represents a value
         * less than the passed in value.
         */
        virtual int compareTo( const Integer& i ) const;

        /**
         * @param i - the Integer object to compare against.
         * @returns true if the two Integer Objects have the same value.
         */
        bool equals( const Integer& i ) const {
            return this->value == i.value;
        }

        /**
         * Compares equality between this object and the one passed.
         * @param i - the value to be compared to this one.
         * @return true if this object is equal to the one passed.
         */
        virtual bool operator==( const Integer& i ) const {
            return this->value == i.value;
        }

        /**
         * Compares this object to another and returns true if this object
         * is considered to be less than the one passed.  This
         * @param i - the value to be compared to this one.
         * @return true if this object is equal to the one passed.
         */
        virtual bool operator<( const Integer& i ) const {
            return this->value < i.value;
        }

        /**
         * Compares this Integer instance with another.
         * @param i - the Integer instance to be compared
         * @return zero if this object represents the same integer value as the
         * argument; a positive value if this object represents a value greater
         * than the passed in value, and -1 if this object represents a value
         * less than the passed in value.
         */
        virtual int compareTo( const int& i ) const;

        /**
         * @param i - the Integer object to compare against.
         * @returns true if the two Integer Objects have the same value.
         */
        bool equals( const int& i ) const {
            return this->value == i;
        }

        /**
         * Compares equality between this object and the one passed.
         * @param i - the value to be compared to this one.
         * @return true if this object is equal to the one passed.
         */
        virtual bool operator==( const int& i ) const {
            return this->value == i;
        }

        /**
         * Compares this object to another and returns true if this object
         * is considered to be less than the one passed.  This
         * @param i - the value to be compared to this one.
         * @return true if this object is equal to the one passed.
         */
        virtual bool operator<( const int& i ) const {
            return this->value < i;
        }

        /**
         * @returns this <code>Integer</code> Object as a String Representation
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
            return this->value;
        }

        /**
         * Answers the long value which the receiver represents
         * @return long the value of the receiver.
         */
        virtual long long longValue() const {
            return (long long)this->value;
        }

    public:  // Statics

        /**
         * Decodes a String into a Integer. Accepts decimal, hexadecimal, and octal
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
         * @returns a Integer object containing the decoded value
         * @throws NumberFomatException if the string is not formatted correctly.
         */
        static Integer decode( const std::string& value );

        /**
         * Returns the value obtained by reversing the order of the bytes in the
         * two's complement representation of the specified int value.
         * @param value - the int whose bytes we are to reverse
         * @return the reversed int.
         */
        static int reverseBytes( int value );

        /**
         * Returns the value obtained by reversing the order of the bits in the
         * two's complement binary representation of the specified int  value.
         * @param value - the value whose bits are to be reversed
         * @returns the reversed bits int.
         */
        static int reverse( int value );

        /**
         * Parses the string argument as a signed int in the radix specified by
         * the second argument. The characters in the string must all be digits,
         * of the specified radix (as determined by whether
         * Character.digit(char, int) returns a nonnegative value) except that the
         * first character may be an ASCII minus sign '-' to indicate a negative
         * value. The resulting byte value is returned.
         *
         * An exception of type NumberFormatException is thrown if any of the
         * following situations occurs:
         *  * The first argument is null or is a string of length zero.
         *  * The radix is either smaller than Character.MIN_RADIX or larger than
         *    Character.MAX_RADIX.
         *  * Any character of the string is not a digit of the specified radix,
         *    except that the first character may be a minus sign '-' provided
         *    that the string is longer than length 1.
         *  * The value represented by the string is not a value of type int.
         *
         * @param s - the String containing the int representation to be parsed
         * @param radix - the radix to be used while parsing s
         * @return the int represented by the string argument in the specified radix.
         * @throws NumberFormatException - If String does not contain a parsable int.
         */
        static int parseInt( const std::string& s, int radix );

        /**
         * Parses the string argument as a signed decimal int. The characters
         * in the string must all be decimal digits, except that the first
         * character may be an ASCII minus sign '-' to indicate a
         * negative value. The resulting int value is returned, exactly as if
         * the argument and the radix 10 were given as arguments to the
         * parseInteger( const std::string, int ) method.
         * @param s - String to convert to a int
         * @returns the converted int value
         * @throws NumberFormatException if the string is not a int.
         */
        static int parseInt( const std::string& s );

        /**
         * Returns a Integer instance representing the specified int value.
         * @param value - the int to wrap
         * @return the new Integer object wrapping value.
         */
        static Integer valueOf( int value ) {
            return Integer( value );
        }

        /**
         * Returns a Integer object holding the value given by the specified
         * std::string.  The argument is interpreted as representing a signed
         * decimal int, exactly as if the argument were given to the
         * parseInt( std::string ) method. The result is a Integer object that
         * represents the int value specified by the string.
         * @param value - std::string to parse as base 10
         * @return new Integer Object wrapping the primitive
         * @throws NumberFormatException if the string is not a decimal int.
         */
        static Integer valueOf( const std::string& value );

        /**
         * Returns a Integer object holding the value extracted from the specified
         * std::string when parsed with the radix given by the second argument.
         * The first argument is interpreted as representing a signed int in the
         * radix specified by the second argument, exactly as if the argument were
         * given to the parseInt( std::string, int ) method. The result is a
         * Integer object that represents the int value specified by the string.
         * @param value - std::string to parse as base ( radix )
         * @param radix - base of the string to parse.
         * @return new Integer Object wrapping the primitive
         * @throws NumberFormatException if the string is not a valid int.
         */
        static Integer valueOf( const std::string& value, int radix );

        /**
         * Returns the number of one-bits in the two's complement binary
         * representation of the specified int value. This function is sometimes
         * referred to as the population count.
         * @param value - the int to count
         * @return the number of one-bits in the two's complement binary
         *         representation of the specified int value.
         */
        static int bitCount( int value );

        /**
         * Converts the int to a String representation
         * @param value
         *      The int to convert to a <code>std::string</code> instance.
         * @return string representation
         */
        static std::string toString( int value );

        /**
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
         * @param value - the int to convert to a string
         * @param radix - the radix to format the string in
         * @returns an int formatted to the string value of the radix given.
         */
        static std::string toString( int value, int radix );

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
         * @param value - the int to be translated to an Octal string
         * @returns the unsigned int value as a Octal string
         */
        static std::string toHexString( int value );

        /**
         * Returns a string representation of the integer argument as an unsigned
         * integer in base 8.
         *
         * The unsigned integer value is the argument plus 2^32 if the argument is
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
         * @param value - the int to be translated to an Octal string
         * @returns the unsigned int value as a Octal string
         */
        static std::string toOctalString( int value );

        /**
         * Returns a string representation of the integer argument as an unsigned
         * integer in base 2.
         * <p>
         * The unsigned integer value is the argument plus 2^32 if the argument is
         * negative; otherwise it is equal to the argument. This value is converted
         * to a string of ASCII digits in binary (base 2) with no extra leading 0s.
         * If the unsigned magnitude is zero, it is represented by a single zero
         * character '0'; otherwise, the first character of the representation
         * of the unsigned magnitude will not be the zero character.
         * <p>
         * The characters '0' and '1' are used as binary
         * digits.
         *
         * @param value - the int to be translated to a binary string
         * @returns the unsigned int value as a binary string
         */
        static std::string toBinaryString( int value );

        /**
         * Returns an int value with at most a single one-bit, in the position of
         * the highest-order ("leftmost") one-bit in the specified int value.
         * Returns zero if the specified value has no one-bits in its two's
         * complement binary representation, that is, if it is equal to zero.
         * @param value - the int to be inspected
         * @return an int value with a single one-bit, in the position of the
         * highest-order one-bit in the specified value, or zero if the specified
         * value is itself equal to zero.
         */
        static int highestOneBit( int value );

        /**
         * Returns an int value with at most a single one-bit, in the position of
         * the lowest-order ("rightmost") one-bit in the specified int value.
         * Returns zero if the specified value has no one-bits in its two's
         * complement binary representation, that is, if it is equal to zero.
         * @param value - the int to be inspected
         * @return an int value with a single one-bit, in the position of the
         * lowest-order one-bit in the specified value, or zero if the specified
         * value is itself equal to zero.
         */
        static int lowestOneBit( int value );

        /**
         * Returns the number of zero bits preceding the highest-order ("leftmost")
         * one-bit in the two's complement binary representation of the specified
         * int value. Returns 32 if the specified value has no one-bits in its two's
         * complement representation, in other words if it is equal to zero.
         *
         * Note that this method is closely related to the logarithm base 2. For
         * all positive int values x:
         *
         *     * floor( log2(x)) = 31 - numberOfLeadingZeros(x)
         *     * ceil( log2(x)) = 32 - numberOfLeadingZeros(x - 1)
         *
         * @param value - the int to be inspected
         * @return the number of zero bits preceding the highest-order ("leftmost")
         * one-bit in the two's complement binary representation of the specified
         * int value, or 32 if the value is equal to zero.
         */
        static int numberOfLeadingZeros( int value );

        /**
         * Returns the number of zero bits following the lowest-order ("rightmost")
         * one-bit in the two's complement binary representation of the specified
         * int value. Returns 32 if the specified value has no one-bits in its
         * two's complement representation, in other words if it is equal to zero.
         * @param value - the int to be inspected
         * @return the number of zero bits following the lowest-order ("rightmost")
         * one-bit in the two's complement binary representation of the specified
         * int value, or 32 if the value is equal to zero.
         */
        static int numberOfTrailingZeros( int value );

        /**
         * Returns the value obtained by rotating the two's complement binary
         * representation of the specified int value left by the specified number
         * of bits. (Bits shifted out of the left hand, or high-order, side reenter
         * on the right, or low-order.)
         *
         * Note that left rotation with a negative distance is equivalent to right
         * rotation: rotateLeft(val, -distance) == rotateRight(val, distance). Note
         * also that rotation by any multiple of 32 is a no-op, so all but the last
         * five bits of the rotation distance can be ignored, even if the distance
         * is negative: rotateLeft(val, distance) == rotateLeft(val, distance & 0x1F).
         * @param value - the int to be inspected
         * @param distance - the number of bits to rotate
         * @return the value obtained by rotating the two's complement binary
         * representation of the specified int value left by the specified number
         * of bits.
         */
        static int rotateLeft( int value, int distance );

        /**
         * Returns the value obtained by rotating the two's complement binary
         * representation of the specified int value right by the specified number
         * of bits. (Bits shifted out of the right hand, or low-order, side reenter
         * on the left, or high-order.)
         *
         * Note that right rotation with a negative distance is equivalent to left
         * rotation: rotateRight(val, -distance) == rotateLeft(val, distance). Note
         * also that rotation by any multiple of 32 is a no-op, so all but the last
         * five bits of the rotation distance can be ignored, even if the distance is
         * negative: rotateRight(val, distance) == rotateRight(val, distance & 0x1F).
         * @param value - the int to be inspected
         * @param distance - the number of bits to rotate
         * @return the value obtained by rotating the two's complement binary
         * representation of the specified int value right by the specified number
         * of bits.
         */
        static int rotateRight( int value, int distance );

        /**
         * Returns the signum function of the specified int value. (The return value
         * is -1 if the specified value is negative; 0 if the specified value is zero;
         * and 1 if the specified value is positive.)
         * @param value - the int to be inspected
         * @return the signum function of the specified int value.
         */
        static int signum( int value );

    private:

        static int parse( const std::string& value, int offset,
                          int radix, bool negative );

    };

}}

#endif /*_DECAF_LANG_INTEGER_H_*/
