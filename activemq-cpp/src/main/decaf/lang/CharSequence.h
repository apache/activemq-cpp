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

#ifndef _DECAF_LANG_CHARSEQUENCE_H_
#define _DECAF_LANG_CHARSEQUENCE_H_

#include <decaf/lang/exceptions/IndexOutOfBoundsException.h>
#include <decaf/util/Config.h>

namespace decaf{
namespace lang{

    /**
     * A CharSequence is a readable sequence of char values. This interface provides
     * uniform, read-only access to many different kinds of char sequences.
     *
     * This interface does not define that a CharSequence should implement comparable,
     * it is therefore up to the dervied classes that implement this interface to define
     * equality, which implies that comparison of two CharSequences does not have a
     * contract on equality.
     */
    class DECAF_API CharSequence {
    public:

        virtual ~CharSequence() {}

        /**
         * @returns the length of the underlying character sequence.
         */
        virtual int length() const = 0;

        /**
         * Returns the Char at the specified index so long as the index is not
         * greater than the length of the sequence.
         *
         * @param index
         *      The position to return the char at.
         *
         * @returns the char at the given position.
         *
         * @throws IndexOutOfBoundsException if index is > than length() or negative
         */
        virtual char charAt( int index ) const = 0;

        /**
         * Returns a new CharSequence that is a subsequence of this sequence. The
         * subsequence starts with the char value at the specified index and ends with
         * the char value at index end - 1. The length (in chars) of the returned
         * sequence is end - start, so if start == end then an empty sequence is returned.
         *
         * @param start
         *      The start index, inclusive.
         * @param end
         *      The end index, exclusive.
         *
         * @returns a new CharSequence
         *
         * @throws IndexOutOfBoundsException if start or end > length() or start or end are negative.
         */
        virtual CharSequence* subSequence( int start, int end ) const = 0;

        /**
         * @returns the string representation of this CharSequence
         */
        virtual std::string toString() const = 0;

    };

}}

#endif /*_DECAF_LANG_CHARSEQUENCE_H_*/
