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
                              public Comparable<Integer> {
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
         * @param the primitive value to wrap
         */
        Integer( int value );

        /**
         * @param the base 10 encoded string to decode to sn int and wrap.
         */
        Integer( const std::string& value );

        virtual ~Integer() {}

        /**
         * Compares this Integer instance with another.
         * @param b - the Integer instance to be compared
         * @return zero if this object represents the same integer value as the
         * argument; a positive value if this object represents a value greater
         * than the passed in value, and -1 if this object repesents a value
         * less than the passed in value.
         */
        virtual int compareTo( const Integer& n ) const;

        /**
         * @param the Integer object to compare against.
         * @returns true if the two Integer Objects have the same value.
         */
        bool equals( const Integer& n ) const {
            return this->value == n.value;
        }

        /**
         * @returns this Interger Object as a String Representation
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
        static Integer decode( const std::string& value )
            throw ( exceptions::NumberFormatException );

        /**
         * Returns the value obtained by reversing the order of the bytes in the
         * two's complement representation of the specified int value.
         * @param value - the int whose bytes we are to reverse
         * @return the reversed int.
         */
        static int reverseBytes( int value );

        /**
         * Parses the string argument as a signed int in the radix specified by
         * the second argument. The characters in the string must all be digits,
         * of the specified radix (as determined by whether
         * Character.digit(char, int) returns a nonnegative value) except that the
         * first character may be an ASCII minus sign '-'  ('\u002D') to indicate
         * a negative value. The resulting byte value is returned.
         *
         * An exception of type NumberFormatException is thrown if any of the
         * following situations occurs:
         *  * The first argument is null or is a string of length zero.
         *  * The radix is either smaller than Character.MIN_RADIX or larger than
         *    Character.MAX_RADIX.
         *  * Any character of the string is not a digit of the specified radix,
         *    except that the first character may be a minus sign '-' ('\u002D')
         *    provided that the string is longer than length 1.
         *  * The value represented by the string is not a value of type int.
         *
         * @param s - the String containing the int representation to be parsed
         * @param radix - the radix to be used while parsing s
         * @return the int represented by the string argument in the specified radix.
         * @throws NumberFormatException - If String does not contain a parsable int.
         */
        static int parseInt( const std::string& s, int radix )
            throw ( exceptions::NumberFormatException );

        /**
         * Parses the string argument as a signed decimal int. The characters
         * in the string must all be decimal digits, except that the first
         * character may be an ASCII minus sign '-' ('\u002D') to indicate a
         * negative value. The resulting int value is returned, exactly as if
         * the argument and the radix 10 were given as arguments to the
         * parseInteger( const std::string, int ) method.
         * @param s - String to convert to a int
         * @returns the converted int value
         * @throws NumberFormatException if the string is not a int.
         */
        static int parseInt( const std::string& s )
            throw ( exceptions::NumberFormatException );

        /**
         * Returns a Integer instance representing the specified int value.
         * @param value - the int to wrap
         * @return the new Integer object wrapping value.
         */
        static Integer valueOf( int value );

        /**
         * Returns a Integer object holding the value given by the specified std::string.
         * The argument is interpreted as representing a signed decimal int,
         * exactly as if the argument were given to the parseInteger( std::string )
         * method. The result is a Integer object that represents the int value
         * specified by the string.
         * @param value - std::string to parse as base 10
         * @return new Integer Object wrapping the primitive
         * @throws NumberFormatException if the string is not a decimal int.
         */
        static Integer valueOf( const std::string& value )
            throw ( exceptions::NumberFormatException );

        /**
         * Returns a Integer object holding the value extracted from the specified
         * std::string when parsed with the radix given by the second argument.
         * The first argument is interpreted as representing a signed int in the
         * radix specified by the second argument, exactly as if the argument were
         * given to the parseInteger( std::string, int ) method. The result is a
         * Integer object that represents the int value specified by the string.
         * @param value - std::string to parse as base ( radix )
         * @param radix - base of the string to parse.
         * @return new Integer Object wrapping the primitive
         * @throws NumberFormatException if the string is not a valid int.
         */
        static Integer valueOf( const std::string& value, int radix )
            throw ( exceptions::NumberFormatException );

        /**
         * Returns the number of one-bits in the two's complement binary
         * representation of the specified int value. This function is sometimes
         * referred to as the population count.
         * @param value - the int to count
         * @return the number of one-bits in the two's complement binary
         * representation of the specified int value.
         */
        static int bitCount( int value );

        /**
         * Converts the int to a String representation
         * @param int to convert
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
         * the ASCII minus character '-' ('\u002D'). If the first argument is not
         * negative, no sign character appears in the result.
         *
         * The remaining characters of the result represent the magnitude of the
         * first argument. If the magnitude is zero, it is represented by a single
         *  zero character '0' ('\u0030'); otherwise, the first character of the
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

    };

}}

#endif /*_DECAF_LANG_INTEGER_H_*/
