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

#ifndef _DECAF_LANG_LONG_H_
#define _DECAF_LANG_LONG_H_

#include <decaf/lang/Number.h>
#include <decaf/lang/Comparable.h>
#include <string>

namespace decaf{
namespace lang{

    class DECAF_API Long : public Number{
    private:

        // The primitive long long value
        long long value;

    public:

        /** The size in bits of the primitive long long type */
        static const int SIZE = 64;

        /** The maximum value that the primitive type can hold */
        static const long long MAX_VALUE = (long long)0x7FFFFFFFFFFFFFFFLL;

        /** The minimum value that the primitive type can hold */
        static const long long MIN_VALUE = (long long)0x8000000000000000LL;

    public:

        Long() {}
        virtual ~Long() {}

        /**
         * Parses the String passed and extracts an long.
         * @param String to parse
         * @return long value
         */
        static long long parseLong( const std::string& value );

        /**
         * Converts the long to a String representation
         * @param long to convert
         * @return string representation
         */
        static std::string toString( long long value );
    };

}}

#endif /*_DECAF_LANG_LONG_H_*/
