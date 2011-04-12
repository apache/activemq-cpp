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

#ifndef _DECAF_LANG_BYTE_H_
#define _DECAF_LANG_BYTE_H_

#include <decaf/util/Config.h>
#include <decaf/lang/Number.h>
#include <decaf/lang/Comparable.h>
#include <decaf/lang/exceptions/NumberFormatException.h>
#include <string>

namespace decaf{
namespace lang{

    class DECAF_API Byte : public Number,
                           public Comparable<Byte>,
                           public Comparable<unsigned char> {
    private:

        unsigned char value;

    public:

        /** The minimum value that a unsigned char can take on. */
        static const unsigned char MIN_VALUE = 0x7F;

        /** The maximum value that a unsigned char can take on. */
        static const unsigned char MAX_VALUE = 0x80;

        /** The size of the primitive charactor in bits. */
        static const int SIZE = 8;

    public:

        /**
         * @param value - the primitive value to wrap
         */
        Byte( unsigned char value );

        /**
         * Creates a new Byte instance from the given string.
         *
         * @param value
         *      The string to convert to an unsigned char
         *
         * @throws NumberFormatException if the string is not a valid byte.
         */
        Byte( const std::string& value );

        virtual ~Byte() {}

        /**
         * Compares this Byte instance with another.
         * @param c - the Byte instance to be compared
         * @return zero if this object represents the same char value as the
         * argument; a positive value if this object represents a value greater
         * than the passed in value, and -1 if this object repesents a value
         * less than the passed in value.
         */
        virtual int compareTo( const Byte& c ) const {
            return this->value < c.value ? -1 : (this->value > c.value) ? 1 : 0;
        }

        /**
         * Compares equality between this object and the one passed.
         * @param c - the value to be compared to this one.
         * @return true if this object is equal to the one passed.
         */
        virtual bool operator==( const Byte& c ) const {
            return this->value == c.value;
        }

        /**
         * Compares this object to another and returns true if this object
         * is considered to be less than the one passed.  This
         * @param c - the value to be compared to this one.
         * @return true if this object is equal to the one passed.
         */
        virtual bool operator<( const Byte& c ) const {
            return this->value < c.value;
        }

        /**
         * Compares this Byte instance with a char type.
         * @param c - the char instance to be compared
         * @return zero if this object represents the same char value as the
         * argument; a positive value if this object represents a value greater
         * than the passed in value, and -1 if this object repesents a value
         * less than the passed in value.
         */
        virtual int compareTo( const unsigned char& c ) const {
            return this->value < c ? -1 : (this->value > c) ? 1 : 0;
        }

        /**
         * Compares equality between this object and the one passed.
         * @param c - the value to be compared to this one.
         * @return true if this object is equal to the one passed.
         */
        virtual bool operator==( const unsigned char& c ) const {
            return this->value == c;
        }

        /**
         * Compares this object to another and returns true if this object
         * is considered to be less than the one passed.  This
         * @param c - the value to be compared to this one.
         * @return true if this object is equal to the one passed.
         */
        virtual bool operator<( const unsigned char& c ) const {
            return this->value < c;
        }

        /**
         * @returns true if the two Byte Objects have the same value.
         */
        bool equals( const Byte& c ) const {
            return this->value == c.value;
        }

        /**
         * @returns true if the two Bytes have the same value.
         */
        bool equals( const unsigned char& c ) const {
            return this->value == c;
        }

        /**
         * @returns this Byte Object as a String Representation
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
         * @return byte the value of the receiver.
         */
        virtual unsigned char byteValue() const {
            return this->value;
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
         * @return long long the value of the receiver.
         */
        virtual long long longValue() const {
            return (long long)this->value;
        }

    public:    // statics

        /**
         * @returns a string representing the primitive value as Base 10
         */
        static std::string toString( unsigned char value );

        /**
         * Decodes a String into a Byte. Accepts decimal, hexadecimal, and octal
         * numbers given by the following grammar:
         *
         * The sequence of characters following an (optional) negative sign and/or
         * radix specifier ("0x", "0X", "#", or leading zero) is parsed as by the
         * Byte::parseByte method with the indicated radix (10, 16, or 8). This
         * sequence of characters must represent a positive value or a
         * NumberFormatException will be thrown. The result is negated if first
         * character of the specified String is the minus sign. No whitespace
         * characters are permitted in the string.
         * @param value - The string to decode
         * @returns a Byte object containing the decoded value
         * @throws NumberFomatException if the string is not formatted correctly.
         */
        static Byte decode( const std::string& value );

        /**
         * Parses the string argument as a signed unsigned char in the radix specified by
         * the second argument. The characters in the string must all be digits,
         * of the specified radix (as determined by whether
         * Character.digit(char, int) returns a nonnegative value) except that the
         * first character may be an ASCII minus sign '-' to indicate
         * a negative value. The resulting byte value is returned.
         *
         * An exception of type NumberFormatException is thrown if any of the
         * following situations occurs:
         *  * The first argument is null or is a string of length zero.
         *  * The radix is either smaller than Character.MIN_RADIX or larger than
         *    Character::MAX_RADIX.
         *  * Any character of the string is not a digit of the specified radix,
         *    except that the first character may be a minus sign '-' provided
         *    that the string is longer than length 1.
         *  * The value represented by the string is not a value of type unsigned char.
         *
         * @param s - the String containing the unsigned char to be parsed
         * @param radix - the radix to be used while parsing s
         * @return the unsigned char represented by the string argument in the
         * 	       specified radix.
         * @throws NumberFormatException - If String does not contain a parsable
         *         unsigned char.
         */
        static unsigned char parseByte( const std::string& s, int radix );

        /**
         * Parses the string argument as a signed decimal unsigned char. The
         * characters in the string must all be decimal digits, except that the
         * first character may be an ASCII minus sign '-' to indicate a
         * negative value. The resulting unsigned char value is returned, exactly as
         * if the argument and the radix 10 were given as arguments to the
         * parseByte( const std::string, int ) method.
         * @param s - String to convert to a unsigned char
         * @returns the converted unsigned char value
         * @throws NumberFormatException if the string is not a unsigned char.
         */
        static unsigned char parseByte( const std::string& s );

        /**
         * Returns a Character instance representing the specified char value.
         * @param value - the primitive char to wrap.
         * @returns a new Character instance that wraps this value.
         */
        static Byte valueOf( unsigned char value ) {
            return Byte( value );
        }

        /**
         * Returns a Byte object holding the value given by the specified std::string.
         * The argument is interpreted as representing a signed decimal unsigned char,
         * exactly as if the argument were given to the parseByte( std::string )
         * method. The result is a Byte object that represents the unsigned char value
         * specified by the string.
         * @param value - std::string to parse as base 10
         * @return new Byte Object wrapping the primitive
         * @throws NumberFormatException if the string is not a decimal unsigned char.
         */
        static Byte valueOf( const std::string& value );

        /**
         * Returns a Byte object holding the value extracted from the specified
         * std::string when parsed with the radix given by the second argument.
         * The first argument is interpreted as representing a signed unsigned char
         * in the radix specified by the second argument, exactly as if the argument
         * were given to the parseByte( std::string, int ) method. The result is a
         * Byte object that represents the unsigned char value specified by the
         * string.
         * @param value - std::string to parse as base ( radix )
         * @param radix - base of the string to parse.
         * @return new Byte Object wrapping the primitive
         * @throws NumberFormatException if the string is not a valid unsigned char.
         */
        static Byte valueOf( const std::string& value, int radix );

    };

}}

#endif /*_DECAF_LANG_BYTE_H_*/
