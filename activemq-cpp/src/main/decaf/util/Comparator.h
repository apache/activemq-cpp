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

#ifndef _DECAF_UTIL_COMPARATOR_H_
#define _DECAF_UTIL_COMPARATOR_H_

#include <decaf/util/Config.h>

namespace decaf{
namespace util{

    /**
     * A comparison function, which imposes a total ordering on some collection
     * of objects. Comparators can be passed to a sort method (such as
     * Collections.sort) to allow precise control over the sort order. Comparators
     * can also be used to control the order of certain data structures.
     *
     * The ordering imposed by a Comparator c on a set of elements S is said to
     * be consistent with equals if and only if
     * ( compare( e1, e2) == 0 ) has the same boolean value as ( e1 == e2 ) for
     * every e1 and e2 in S.
     *
     * @since 1.0
     */
    template<typename T>
    class Comparator {
    public:

        virtual ~Comparator() {}

        /**
         * Implementation of the Binary function interface as a means of allowing
         * a Comparator to be passed to an STL Map for use as the sorting criteria.
         *
         * @param left
         *      The Left hand side operand.
         * @param right
         *      The Right hand side operand.
         *
         * @return true if the vale of left is less than the value of right.
         */
        virtual bool operator() ( const T& left, const T& right ) const = 0;

        /**
         * Compares its two arguments for order. Returns a negative integer, zero,
         * or a positive integer as the first argument is less than, equal to, or
         * greater than the second.
         *
         * The implementor must ensure that
         * sgn( compare(x, y)) == -sgn(compare(y, x) ) for all x and y.
         * (This implies that compare(x, y) must throw an exception if and only
         * if compare(y, x) throws an exception.)
         *
         * The implementor must also ensure that the relation is transitive:
         * ((compare(x, y)>0) && (compare(y, z)>0)) implies compare(x, z)>0.
         *
         * Finally, the implementer must ensure that compare(x, y)==0 implies
         * that sgn(compare(x, z))==sgn(compare(y, z)) for all z.
         *
         * It is generally the case, but not strictly required that
         * (compare(x, y)==0) == ( x == y) ). Generally speaking, any comparator
         * that violates this condition should clearly indicate this fact. The
         * recommended language is "Note: this comparator imposes orderings that
         * are inconsistent with equals."
         *
         * @param o1
         *      The first object to be compared
         * @param o2
         *      The second object to be compared
         *
         * @returns a negative integer, zero, or a positive integer as the first
         *          argument is less than, equal to, or greater than the second.
         */
        virtual int compare( const T& o1, const T& o2 ) const = 0;

    };

}}

#endif /*_DECAF_UTIL_COMPARATOR_H_*/
