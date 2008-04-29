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

#ifndef _DECAF_LANG_COMPARABLE_H_
#define _DECAF_LANG_COMPARABLE_H_

#include <decaf/util/Config.h>

namespace decaf{
namespace lang{

    /**
     * This interface imposes a total ordering on the objects of each class that
     * implements it. This ordering is referred to as the class's natural
     * ordering, and the class's compareTo method is referred to as its natural
     * comparison method.
     */
    template< typename T >
    class DECAF_API Comparable{
    public:

        virtual ~Comparable() {}

        /**
         * Compares this object with the specified object for order. Returns a
         * negative integer, zero, or a positive integer as this object is less
         * than, equal to, or greater than the specified object.
         *
         * In the foregoing description, the notation sgn(expression) designates
         * the mathematical signum function, which is defined to return one of
         * -1, 0, or 1 according to whether the value of expression  is negative,
         * zero or positive. The implementor must ensure sgn(x.compareTo(y)) ==
         * -sgn(y.compareTo(x)) for all x and y. (This implies that x.compareTo(y)
         * must throw an exception iff y.compareTo(x) throws an exception.)
         *
         * The implementor must also ensure that the relation is transitive:
         * (x.compareTo(y)>0 && y.compareTo(z)>0) implies x.compareTo(z)>0.
         *
         * Finally, the implementer must ensure that x.compareTo(y)==0 implies
         * that sgn(x.compareTo(z)) == sgn(y.compareTo(z)), for all z.
         *
         * It is strongly recommended, but not strictly required that
         * (x.compareTo(y)==0) == (x.equals(y)). Generally speaking, any class
         * that implements the Comparable interface and violates this condition
         * should clearly indicate this fact. The recommended language is
         * "Note: this class has a natural ordering that is inconsistent with
         * equals."
         * @param value - the Object to be compared.
         * @returns a negative integer, zero, or a positive integer as this
         * object is less than, equal to, or greater than the specified object.
         */
        virtual int compareTo( const T& value ) const = 0;

        /**
         * @return true if this value is considered equal to the passed value.
         */
        virtual bool equals( const T& value ) const = 0;

        /**
         * Compares equality between this object and the one passed.
         * @param value - the value to be compared to this one.
         * @return true if this object is equal to the one passed.
         */
        virtual bool operator==( const T& value ) const = 0;

        /**
         * Compares this object to another and returns true if this object
         * is considered to be less than the one passed.  This
         * @param value - the value to be compared to this one.
         * @return true if this object is equal to the one passed.
         */
        virtual bool operator<( const T& value ) const = 0;

    };

}}

#endif /*_DECAF_LANG_COMPARABLE_H_*/
