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

    public:

        /**
         * @param value - the primitve type to wrap
         */
        Float( float value );

        /**
         * @param value - the primitve type to wrap
         */
        Float( double value );

        /**
         * @param value - the string to convert to a primitve type to wrap
         */
        Float( const std::string& value ) throw( exceptions::NumberFormatException );

        virtual ~Float() {}

        /**
         * Compares this Float instance with another.
         * @param f - the Float instance to be compared
         * @return zero if this object represents the same integer value as the
         * argument; a positive value if this object represents a value greater
         * than the passed in value, and -1 if this object repesents a value
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
         * than the passed in value, and -1 if this object repesents a value
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

    };

}}

#endif /*_DECAF_LANG_FLOAT_H_*/
