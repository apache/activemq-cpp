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

#ifndef _DECAF_LANG_SHORT_H_
#define _DECAF_LANG_SHORT_H_

#include <decaf/lang/Number.h>
#include <decaf/lang/Comparable.h>
#include <decaf/lang/exceptions/NumberFormatException.h>
#include <string>

namespace decaf{
namespace lang{

    class Short : public Number,
                  public Comparable<Short> {
    private:

        // The short value
        short value;

    public:

        /**
         * Max Value for this Object's primitive type
         */
        static const short MAX_VALUE = ((1 << sizeof(short)) - 1);

        /**
         * Max Value for this Object's primitive type
         */
        static const short MIN_VALUE = (-(1 << sizeof(short)));

        /**
         * Size of this objects primitive type
         */
        static const int SIZE = sizeof( short );


    public:

        /**
         * @param value - short to wrap
         */
        Short( short value );

        /**
         * @param value - string value to convert to short and wrap
         */
        Short( const std::string& value );

        virtual ~Short() {}

        /**
         * Compares this Short instance with another.
         * @param b - the Short instance to be compared
         * @return zero if this object represents the same short value as the
         * argument; a positive value if this object represents a value greater
         * than the passed in value, and -1 if this object repesents a value
         * less than the passed in value.
         */
        virtual int compareTo( const Short& b ) const;

        /**
         * @returns true if the two Short Objects have the same value.
         */
        bool equals( const Short& s ) const {
            return this->value == s.value;
        }

        /**
         * @returns this Short Object as a String Representation
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
            return this->value;
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

    public:   // statics

    	/**
    	 * @returns a string representing the primitive value as Base 10
    	 */
    	static std::string toString( short value );
    	
        /**
         * Decodes a String into a Short. Accepts decimal, hexadecimal, and octal
         * numbers given by the following grammar:
         *
         * The sequence of characters following an (optional) negative sign and/or
         * radix specifier ("0x", "0X", "#", or leading zero) is parsed as by the
         * Short.parseShort method with the indicated radix (10, 16, or 8). This
         * sequence of characters must represent a positive value or a
         * NumberFormatException will be thrown. The result is negated if first
         * character of the specified String is the minus sign. No whitespace
         * characters are permitted in the string.
         * @param value - The string to decode
         * @returns a Short object containing the decoded value
         * @throws NumberFomatException if the string is not formatted correctly.
         */
        static Short decode( const std::string& value )
            throw ( exceptions::NumberFormatException );

        /**
         * Returns the value obtained by reversing the order of the bytes in the
         * two's complement representation of the specified short value.
         */
        static short reverseBytes( short value );

        /**
         * Parses the string argument as a signed short in the radix specified by
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
         *  * The value represented by the string is not a value of type short.
         *
         * @param s - the String containing the short representation to be parsed
         * @param radix - the radix to be used while parsing s
         * @return the short represented by the string argument in the specified radix.
         * @throws NumberFormatException - If String does not contain a parsable short.
         */
        static short parseShort( const std::string& s, int radix )
            throw ( exceptions::NumberFormatException );

        /**
         * Parses the string argument as a signed decimal short. The characters
         * in the string must all be decimal digits, except that the first
         * character may be an ASCII minus sign '-' ('\u002D') to indicate a
         * negative value. The resulting short value is returned, exactly as if
         * the argument and the radix 10 were given as arguments to the
         * parseShort( const std::string, int ) method.
         * @param s - String to convert to a short
         * @returns the converted short value
         * @throws NumberFormatException if the string is not a short.
         */
        static short parseShort( const std::string& s )
            throw ( exceptions::NumberFormatException );

        /**
         * Returns a Short instance representing the specified short value.
         * @param value - the short to wrap
         * @return the new Short object wrapping value.
         */
        static Short valueOf( short value );

        /**
         * Returns a Short object holding the value given by the specified std::string.
         * The argument is interpreted as representing a signed decimal short,
         * exactly as if the argument were given to the parseShort( std::string )
         * method. The result is a Short object that represents the short value
         * specified by the string.
         * @param value - std::string to parse as base 10
         * @return new Short Object wrapping the primitive
         * @throws NumberFormatException if the string is not a decimal short.
         */
        static Short valueOf( const std::string& value )
            throw ( exceptions::NumberFormatException );

        /**
         * Returns a Short object holding the value extracted from the specified
         * std::string when parsed with the radix given by the second argument.
         * The first argument is interpreted as representing a signed short in the
         * radix specified by the second argument, exactly as if the argument were
         * given to the parseShort( std::string, int ) method. The result is a
         * Short object that represents the short value specified by the string.
         * @param value - std::string to parse as base ( radix )
         * @param radix - base of the string to parse.
         * @return new Short Object wrapping the primitive
         * @throws NumberFormatException if the string is not a valid short.
         */
        static Short valueOf( const std::string& value, int radix )
            throw ( exceptions::NumberFormatException );

    };

}}

#endif /*_DECAF_LANG_SHORT_H_*/
