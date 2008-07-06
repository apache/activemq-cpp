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

#ifndef _DECAF_INTERNAL_NET_URIHELPER_H_
#define _DECAF_INTERNAL_NET_URIHELPER_H_

#include <string>

namespace decaf {
namespace internal {
namespace net {

    /**
     * Helper class used by the URI classes in encoding and decoding of URI's.
     */
    class URIHelper {
    public:

        URIHelper();
        virtual ~URIHelper() {}

        /**
         * Check is the string passed contains a Valid IPv4 word, which is
         * an integer in the range of 0 to 255.
         * @param word - string value to check.
         * @return true if the word is a valid IPv4 word.
         */
        bool isValidIP4Word( const std::string& word );

        /**
         * Determines if the given char is a valid Hex char.  Valid
         * chars are A-F (upper or lower case) and 0-9.
         * @param c - char to inspect
         * @return true if c is a valid hex char.
         */
        bool isValidHexChar( char c );

    };

}}}

#endif /* _DECAF_INTERNAL_NET_URIHELPER_H_ */
