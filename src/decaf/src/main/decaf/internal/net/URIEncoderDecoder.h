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

#ifndef _DECAF_INTERNAL_NET_URIENCODERDECODER_H_
#define _DECAF_INTERNAL_NET_URIENCODERDECODER_H_

#include <decaf/util/Config.h>
#include <decaf/net/URISyntaxException>
#include <string>

namespace decaf{
namespace internal{
namespace net{

    class URIEncoderDecoder {
    public:

        URIEncoderDecoder();
        virtual ~URIEncoderDecoder() {}

        /**
         * Validate a string by checking if it contains any characters other than:
         *
         * 1. letters ('a'..'z', 'A'..'Z')
         * 2. numbers ('0'..'9')
         * 3. characters in the legalset parameter
         * 4. characters that are not ISO Control or are not ISO Space characters)
         *
         * @param s - the string to be validated
         * @param legal - the characters allowed in the string s
         */
        static void validate( const std::string& s, const std::string& legal )
            throw ( URISyntaxException );

        /**
         * Validate a string by checking if it contains any characters other than:
         *
         * 1. letters ('a'..'z', 'A'..'Z')
         * 2. numbers ('0'..'9')
         * 3. characters in the legalset parameter
         *
         * @param s - the string to be validated
         * @param legal - the characters allowed in the string s
         */
        static void validateSimple( const std::string& s, const std::string& legal )
            throw ( URISyntaxException );

    };

}}}

#endif /*_DECAF_INTERNAL_NET_URIENCODERDECODER_H_*/
