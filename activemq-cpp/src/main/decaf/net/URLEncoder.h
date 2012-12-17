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

#ifndef _DECAF_NET_URLENCODER_H_
#define _DECAF_NET_URLENCODER_H_

#include <decaf/util/Config.h>
#include <string>

namespace decaf{
namespace net{

    class DECAF_API URLEncoder {
    private:

        static const std::string digits;

    private:

        URLEncoder();

    public:

        virtual ~URLEncoder() {}

        /**
         * This class contains a utility method for converting a string to the
         * format required by the <code>application/x-www-form-urlencoded</code>
         * MIME content type.
         * <p>
         * All characters except letters ('a'..'z', 'A'..'Z') and numbers ('0'..'9')
         * and characters '.', '-', '*', '_' are converted into their hexidecimal
         * value prepended by '%'.
         * <p>
         * For example: '#' -> %23
         * <p>
         * In addition, spaces are substituted by '+'
         *
         * @param value - the string to be converted
         * @returns the converted string
         */
        static std::string encode( const std::string& value );

    };

}}

#endif /*_DECAF_NET_URLENCODER_H_*/
