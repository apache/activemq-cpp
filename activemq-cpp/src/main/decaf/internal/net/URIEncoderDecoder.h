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
#include <decaf/net/URISyntaxException.h>
#include <string>

namespace decaf{
namespace internal{
namespace net{

    class DECAF_API URIEncoderDecoder {
    private:

        static const std::string digits;

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
         *
         * @throws URISyntaxException if the uri string is not well formed.
         */
        static void validate( const std::string& s, const std::string& legal );

        /**
         * Validate a string by checking if it contains any characters other than:
         *
         * 1. letters ('a'..'z', 'A'..'Z')
         * 2. numbers ('0'..'9')
         * 3. characters in the legalset parameter
         *
         * @param s - the string to be validated
         * @param legal - the characters allowed in the string s
         *
         * @throws URISyntaxException if the uri string is not well formed.
         */
        static void validateSimple( const std::string& s, const std::string& legal );

        /**
         * All characters except letters ('a'..'z', 'A'..'Z') and numbers ('0'..'9')
         * and legal characters are converted into their hexidecimal value prepended
         * by '%'.
         * <p>
         * For example: '#' -> %23
         * <p>
         * Other characters, which are chars that are not US-ASCII, and are
         * not ISO Control or are not ISO Space chars, are preserved.
         * @param s - the string to be converted
         * @param legal - the characters allowed to be preserved in the string s
         * @return converted string
         */
        static std::string quoteIllegal( const std::string& s, const std::string& legal );

        /**
         * Other characters, which are chars that are not US-ASCII, and are
         * not ISO Control or are not ISO Space chars are not preserved. They are
         * converted into their hexidecimal value prepended by '%'.
         * <p>
         * For example: Euro currency symbol -> "%E2%82%AC".
         * @param s - the string to be converted
         * @return the converted string
         */
        static std::string encodeOthers( const std::string& s );

        /**
         * Decodes the string argument which is assumed to be encoded in the
         * <code>x-www-form-urlencoded</code> MIME content type using the UTF-8
         * encoding scheme.
         * <p>
         * '%' and two following hex digit characters are converted to the
         * equivalent byte value. All other characters are passed through
         * unmodified.
         * <p>
         * e.g. "A%20B%20C %24%25" -> "A B C $%"
         * @param s - The encoded string.
         * @return The decoded version.
         */
        static std::string decode( const std::string& s );

    };

}}}

#endif /*_DECAF_INTERNAL_NET_URIENCODERDECODER_H_*/
