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

#ifndef _DECAF_INTERNAL_UTIL_STRINGUTILS_H_
#define STRINGUTILS_H_

#include <decaf/util/Config.h>

namespace decaf {
namespace internal {
namespace util {

    class StringUtils {
    private:

        StringUtils(const StringUtils&);
        StringUtils operator= (const StringUtils&);

    private:

        StringUtils() {}

    public:

        virtual ~StringUtils() {}

        /**
         * Perform a comparison between two strings using natural ordering and ignoring case.
         *
         * @param left
         *      The left-hand string of the comparison.
         * @param right
         *      The right-hand string of the comparison.
         *
         * @returns a negative integer, zero, or a positive integer as the specified string
         *          is greater than, equal to, or less than this String, ignoring case considerations.
         */
        static int compareIgnoreCase(const char* left, const char* right);

        /**
         * Perform a comparison between two strings using natural ordering case is not ignored
         * here, so two otherwise equal string will not match if case differes.
         *
         * @param left
         *      The left-hand string of the comparison.
         * @param right
         *      The right-hand string of the comparison.
         *
         * @returns a negative integer, zero, or a positive integer as the specified string
         *          is greater than, equal to, or less than this String, ignoring case considerations.
         */
        static int compare(const char* left, const char* right);

    };

}}}

#endif /* _DECAF_INTERNAL_UTIL_STRINGUTILS_H_ */
