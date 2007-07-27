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

#ifndef _DECAF_LANG_INTEGER_H_
#define _DECAF_LANG_INTEGER_H_

#include <decaf/util/Config.h>
#include <decaf/lang/Number.h>
#include <decaf/lang/Comparable.h>
#include <string>

namespace decaf{
namespace lang{

    class DECAF_API Integer : public Number {
    private:

        // The primitve Integer value.
        int value;

    public:

        /** The size in bits of the primitive int type */
        static const int SIZE = 32;

        /** The maximum value that the primitive type can hold */
        static const int MAX_VALUE = (int)0x7FFFFFFF;

        /** The minimum value that the primitive type can hold */
        static const int MIN_VALUE = (int)0x80000000;

    public:

        Integer() {}

        virtual ~Integer() {}

    public:  // Statics

        /**
         * Parses the String passed and extracts an int.
         * @param String to parse
         * @return int value
         */
        static int parseInt( const std::string& value );

        /**
         * Converts the int to a String representation
         * @param int to convert
         * @return string representation
         */
        static std::string toString( int value );

    };

}}

#endif /*_DECAF_LANG_INTEGER_H_*/
