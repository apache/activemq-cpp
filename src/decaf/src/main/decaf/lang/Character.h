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

#ifndef _DECAF_LANG_CHARACTER_H_
#define _DECAF_LANG_CHARACTER_H_

#include <decaf/util/Config.h>
#include <decaf/lang/Number.h>
#include <decaf/lang/Comparable.h>
#include <string>

namespace decaf{
namespace lang{

    class DECAF_API Character : public Number,
                                public Comparable<Character>,
                                public Comparable<char> {
    private:

        // The primitive Char value
        char value;

    public:

        /** The minimum radix available for conversion to and from strings. */
        static const int MIN_RADIX = 2;

        /** The maximum radix available for conversion to and from strings. */
        static const int MAX_RADIX = 36;

        /** The minimum value that a signed char can take on. */
        static const char MIN_VALUE = (char)0x7F;

        /** The maximum value that a signed char can take on. */
        static const char MAX_VALUE = (char)0x80;

        /** The size of the primitive charactor in bits. */
        static const int SIZE = 8;

    public:

        /**
         * @param value - char to wrap.
         */
        Character( char value );

        /**
         * Compares this Character instance with another.
         * @param c - the Character instance to be compared
         * @return zero if this object represents the same char value as the
         * argument; a positive value if this object represents a value greater
         * than the passed in value, and -1 if this object repesents a value
         * less than the passed in value.
         */
        virtual int compareTo( const Character& c ) const {
            return this->value < c.value ? -1 : (this->value > c.value) ? 1 : 0;
        }

        /**
         * Compares equality between this object and the one passed.
         * @param c - the value to be compared to this one.
         * @return true if this object is equal to the one passed.
         */
        virtual bool operator==( const Character& c ) const {
            return this->value == c.value;
        }

        /**
         * Compares this object to another and returns true if this object
         * is considered to be less than the one passed.  This
         * @param c - the value to be compared to this one.
         * @return true if this object is equal to the one passed.
         */
        virtual bool operator<( const Character& c ) const {
            return this->value < c.value;
        }

        /**
         * Compares this Character instance with a char type.
         * @param c - the char instance to be compared
         * @return zero if this object represents the same char value as the
         * argument; a positive value if this object represents a value greater
         * than the passed in value, and -1 if this object repesents a value
         * less than the passed in value.
         */
        virtual int compareTo( const char& c ) const {
            return this->value < c ? -1 : (this->value > c) ? 1 : 0;
        }

        /**
         * Compares equality between this object and the one passed.
         * @param c - the value to be compared to this one.
         * @return true if this object is equal to the one passed.
         */
        virtual bool operator==( const char& c ) const {
            return this->value == c;
        }

        /**
         * Compares this object to another and returns true if this object
         * is considered to be less than the one passed.  This
         * @param c - the value to be compared to this one.
         * @return true if this object is equal to the one passed.
         */
        virtual bool operator<( const char& c ) const {
            return this->value < c;
        }

        /**
         * @returns true if the two Character Objects have the same value.
         */
        bool equals( const Character& c ) const {
            return this->value == c.value;
        }

        /**
         * @returns true if the two Characters have the same value.
         */
        bool equals( const char& c ) const {
            return this->value == c;
        }

        /**
         * @returns this Character Object as a String Representation
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
            return (long long)this->value;
        }

    public:    // statics

        /**
         * Returns a Character instance representing the specified char value.
         * @param value - the primitive char to wrap.
         * @returns a new Charactor instance that wraps this value.
         */
        static Character valueOf( char value ) {
            return Character( value );
        }

        /**
         * Indicates whether or not the given character is considered
         * whitespace.
         */
        static bool isWhitespace( char c ){
            switch ( c )
            {
                case '\n':
                case '\t':
                case '\r':
                case '\f':
                case ' ':
                    return true;
            }

            return false;
        }

        /**
         * Indicates whether or not the given character is
         * a digit.
         */
        static bool isDigit( char c ){
            return c >= '0' && c <= '9';
        }

        /**
         * Indicates whether or not the given character is
         * a lower case character.
         */
        static bool isLowerCase( char c ){
            return c >= 'a' && c <= 'z';
        }

        /**
         * Indicates whether or not the given character is
         * a upper case character.
         */
        static bool isUpperCase( char c ){
            return c >= 'A' && c <= 'Z';
        }

        /**
         * Indicates whether or not the given character is
         * a letter.
         */
        static bool isLetter( char c ){
            return isUpperCase(c) || isLowerCase(c);
        }

        /**
         * Indicates whether or not the given character is
         * either a letter or a digit.
         */
        static bool isLetterOrDigit( char c ){
            return isLetter(c) || isDigit(c);
        }

        /**
         * Answers whether the character is an ISO control character, which
         * is a char that lays in the range of 0 to 1f and 7f to 9f
         * @param c - the character, including supplementary characters
         * @return true if the char is an ISO control character
         */
        static bool isISOControl( char c ) {
            return ( c >= 0 && c <= 0x1f ) ||
                   ( (unsigned char)c >= 0x7f && (unsigned char)c <= 0x9f );
        }

        /**
         * Returns the numeric value of the character ch in the specified radix.
         *
         * If the radix is not in the range MIN_RADIX <= radix <= MAX_RADIX or if
         * the value of ch is not a valid digit in the specified radix, -1 is
         * returned. A character is a valid digit if at least one of the following
         * is true:
         *
         *    * The method isDigit is true of the character and the single-character
         *      decomposition is less than the specified radix. In this case the
         *      decimal digit value is returned.
         *    * The character is one of the uppercase Latin letters 'A' through 'Z'
         *      and its code is less than radix + 'A' - 10. In this case,
         *      ch - 'A' + 10 is returned.
         *    * The character is one of the lowercase Latin letters 'a' through 'z'
         *      and its code is less than radix + 'a' - 10. In this case,
         *      ch - 'a' + 10 is returned.
         *
         * @param c - the char to be converted
         * @param radix - the radix of the number
         * @returns the numeric value of the number represented in the given radix
         */
        static int digit( char c, int radix );

    };

}}

#endif /*_DECAF_LANG_CHARACTER_H_*/
