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

#ifndef _DECAF_LANG_APPENDABLE_H_
#define _DECAF_LANG_APPENDABLE_H_

#include <decaf/lang/Exception.h>
#include <decaf/util/Config.h>

namespace decaf{
namespace lang{

    class CharSequence;

    /**
     * An object to which char sequences and values can be appended. The Appendable interface
     * must be implemented by any class whose instances are intended to receive formatted
     * output from a Formatter.
     *
     * TODO
     * The characters to be appended should be valid Unicode characters as described in Unicode
     * Character Representation. Note that supplementary characters may be composed of multiple
     * 16-bit char values.
     *
     * Appendables are not necessarily safe for multithreaded access. Thread safety is the
     * responsibility of classes that extend and implement this interface.
     *
     * Since this interface may be implemented by existing classes with different styles of error
     * handling there is no guarantee that errors will be propagated to the invoker.
     *
     * @since 1.0
     */
    class DECAF_API Appendable {
    public:

        virtual ~Appendable() {}

        /**
         * Appends the specified character to this Appendable.
         *
         * @param value
         *      The character to append.
         *
         * @returns a Reference to this Appendable
         *
         * @throws Exception if an error occurs.
         */
        virtual Appendable& append( char value ) = 0;

        /**
         * Appends the specified character sequence to this Appendable.
         *
         * @param csq
         *      The character sequence from which a subsequence will be appended.
         *      If csq is NULL, then characters will be appended as if csq contained the
         *      string "null".
         *
         * @returns a Reference to this Appendable.
         *
         * @throws Exception if an error occurs.
         */
        virtual Appendable& append( const CharSequence* csq ) = 0;

        /**
         * Appends a subsequence of the specified character sequence to this Appendable.
         * @param csq - The character sequence from which a subsequence will be appended.
         * If csq is NULL, then characters will be appended as if csq contained the
         * string "null".
         *
         * @param start
         *      The index of the first character in the subsequence.
         * @param end
         *      The index of the character following the last character in the subsequence.
         *
         * @returns a Reference to this Appendable
         *
         * @throws Exception if an error occurs.
         * @throws IndexOutOfBoundsException start is greater than end, or end is
         *         greater than csq.length()
         */
        virtual Appendable& append( const CharSequence* csq, int start, int end ) = 0;

    };

}}

#endif /*_DECAF_LANG_APPENDABLE_H_*/
