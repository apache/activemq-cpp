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

#ifndef _DECAF_LANG_BOOLEAN_H_
#define _DECAF_LANG_BOOLEAN_H_

#include <string>
#include <decaf/lang/Comparable.h>
#include <decaf/util/Config.h>

namespace decaf{
namespace lang{

    class DECAF_API Boolean : public Comparable<Boolean>,
                              public Comparable<bool> {
    private:

        // This objects boolean value
        bool value;

    public:

        /**
         * The Class object representing the primitive false boolean.
         */
        static const Boolean _FALSE;

        /**
         * The Class object representing the primitive type boolean.
         */
        static const Boolean _TRUE;

    public:

        /**
         * @param value - primitive boolean to wrap.
         */
        Boolean( bool value );

        /**
         * @param value - String value to convert to a boolean.
         */
        Boolean( const std::string& value );

        virtual ~Boolean() {}

        /**
         * @returns the primitive boolean value of this object
         */
        bool booleanValue() const {
            return value;
        }

        /**
         * @returns the string representation of this Booleans value.
         */
        std::string toString() const;

        /**
         * Compares this Boolean instance with another.
         * @param b - the Boolean instance to be compared
         * @return zero if this object represents the same boolean value as the
         * argument; a positive value if this object represents true and the
         * argument represents false; and a negative value if this object
         * represents false and the argument represents true
         */
        virtual int compareTo( const Boolean& b ) const;

        /**
         * Compares equality between this object and the one passed.
         * @param value - the value to be compared to this one.
         * @return true if this object is equal to the one passed.
         */
        virtual bool operator==( const Boolean& value ) const;

        /**
         * Compares this object to another and returns true if this object
         * is considered to be less than the one passed.  This
         * @param value - the value to be compared to this one.
         * @return true if this object is equal to the one passed.
         */
        virtual bool operator<( const Boolean& value ) const;

        /**
         * @returns true if the two Boolean Objects have the same value.
         */
        bool equals( const Boolean& b ) const {
            return this->value == b.value;
        }

        /**
         * Compares this Boolean instance with another.
         * @param b - the Boolean instance to be compared
         * @return zero if this object represents the same boolean value as the
         * argument; a positive value if this object represents true and the
         * argument represents false; and a negative value if this object
         * represents false and the argument represents true
         */
        virtual int compareTo( const bool& b ) const;

        /**
         * Compares equality between this object and the one passed.
         * @param value - the value to be compared to this one.
         * @return true if this object is equal to the one passed.
         */
        virtual bool operator==( const bool& value ) const;

        /**
         * Compares this object to another and returns true if this object
         * is considered to be less than the one passed.  This
         * @param value - the value to be compared to this one.
         * @return true if this object is equal to the one passed.
         */
        virtual bool operator<( const bool& value ) const;

        /**
         * @returns true if the two Boolean Objects have the same value.
         */
        bool equals( const bool& b ) const {
            return this->value == b;
        }

    public:  // Statics

        /**
         * @param value
         *      The bool value to convert to a <code>Boolean</code> instance.
         *
         * @returns a Boolean instance of the primitive boolean value
         */
        static Boolean valueOf( bool value );

        /**
         * @param value
         *      The std::string value to convert to a <code>Boolean</code> instance.
         *
         * @returns a Boolean instance of the string value
         */
        static Boolean valueOf( const std::string& value );

        /**
         * Parses the String passed and extracts an bool.
         *
         * @param value
         *      The std::string value to parse
         * @return bool value
         */
        static bool parseBoolean( const std::string& value );

        /**
         * Converts the bool to a String representation.
         *
         * @param value The bool value to convert.
         *
         * @return std::string representation of the bool value passed.
         */
        static std::string toString( bool value );

    };

}}

#endif /*_DECAF_LANG_BOOLEAN_H_*/
