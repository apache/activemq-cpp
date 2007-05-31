/**
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _DECAF_LANG_MATH_H_
#define _DECAF_LANG_MATH_H_

#undef min
#undef max

namespace decaf{
namespace lang{

    /**
     * The class <code>Math</code> contains methods for performing basic
     * numeric operations such as the elementary exponential, logarithm,
     * square root, and trigonometric functions.
     */
    class Math
    {
    public:

        Math();
        virtual ~Math();

    public:

        /**
         * Returns the smaller of two <code>short</code> values. That is,
         * the result the argument closer to the value of
         * <code>Short::MIN_VALUE</code>.  If the arguments have the same
         * value, the result is that same value.
         * @param a - an argument.
         * @param b - another argument.
         * @return the smaller of <code>a</code> and <code>b</code>.
         */
        static short min( short a, short b ) {
            return ( a <= b ? a : b );
        }

        /**
         * Returns the smaller of two <code>int</code> values. That is,
         * the result the argument closer to the value of
         * <code>Integer::MIN_VALUE</code>.  If the arguments have the same
         * value, the result is that same value.
         * @param a - an argument.
         * @param b - another argument.
         * @return the smaller of <code>a</code> and <code>b</code>.
         */
        static int min( int a, int b ) {
            return ( a <= b ? a : b );
        }

        /**
         * Returns the larger of two <code>short</code> values. That is,
         * the result the argument closer to the value of
         * <code>Short::MAX_VALUE</code>.  If the arguments have the same
         * value, the result is that same value.
         * @param a - an argument.
         * @param b - another argument.
         * @return the larger of <code>a</code> and <code>b</code>.
         */
        static short max( short a, short b ) {
            return ( a >= b ? a : b );
        }

        /**
         * Returns the larger of two <code>int</code> values. That is,
         * the result the argument closer to the value of
         * <code>Integer::MAX_VALUE</code>.  If the arguments have the same
         * value, the result is that same value.
         * @param a - an argument.
         * @param b - another argument.
         * @return the larger of <code>a</code> and <code>b</code>.
         */
        static int max( int a, int b ) {
            return ( a >= b ? a : b );
        }

    };

}}

#endif /*_DECAF_LANG_MATH_H_*/
