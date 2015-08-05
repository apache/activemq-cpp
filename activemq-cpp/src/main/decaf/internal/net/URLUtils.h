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

#ifndef _DECAF_INTERNAL_NET_URLUTILS_H_
#define _DECAF_INTERNAL_NET_URLUTILS_H_

#include <decaf/util/Config.h>
#include <decaf/lang/String.h>
#include <decaf/net/URL.h>

namespace decaf {
namespace internal {
namespace net {

    class DECAF_API URLUtils {
    private:

        URLUtils();

    public:

        /**
         * File based URL instance with an empty host value are always considered
         * to have a host value of "localhost".
         *
         * @param url
         *      the URL whose host value is to be returned.
         *
         * @return the host value or 'localhost' for file based protocols.
         */
        static decaf::lang::String getHost(const decaf::net::URL& url);

        /**
         * Returns the path will relative path segments like ".." and "." resolved.
         * The returned path will not necessarily start with a "/" character. This
         * handles ".." and "." segments at both the beginning and end of the path.
         *
         * @param discardRelativePrefix
         *      true to remove leading ".." segments from the path. This is appropriate
         *      for paths that are known to be absolute.
         *
         * @return the canonicalized Path value.
         */
        static decaf::lang::String canonicalizePath(const decaf::lang::String& original, bool discardRelativePrefix);

        /**
         * Returns a path that can be safely concatenated with the given authority. If the
         * authority is empty, this can be any path. Otherwise the paths run together like
         * http://activemq.apache.html.
         *
         * @param authority
         *      The authority value from a given URL.
         * @param path
         *      The path value from a given URL.
         *
         * @return a safe version of the Path value.
         */
        static decaf::lang::String authoritySafePath(const decaf::lang::String& authority,
                                                     const decaf::lang::String& path);

        /**
         * Returns true if the given char is valid for a URL scheme taking into account its
         * position in the scheme string.
         *
         * @param index
         *      The index in the scheme where the char value is from.
         * @param c
         *      The value from the given index.
         *
         * @return true if the char value is valid for the given index.
         */
        static bool isValidSchemeChar(int index, char c);

        /**
         * Returns the scheme prefix like "http" from the URL spec, or empty if the
         * spec doesn't start with a scheme. Scheme prefixes match this pattern:
         * (alpha ( alpha | digit | '+' | '-' | '.' )* ':')
         */
        static decaf::lang::String getSchemePrefix(const decaf::lang::String& spec);

        /**
         * Returns the index of the first char of the given set in the passed in String
         * bounded between start and end. This returns the end value if none of the characters
         * exist in the requested range.
         *
         * This is an optimization used in URL processing as the return value is end if
         * the chars are not found in the string so that processing can continue from the
         * returned end value no matter what the result is.
         *
         * @param string
         *      The string to search.
         * @param chars
         *      The set of characters to search for in the target String.
         * @param start
         *      The start index to search from.
         * @param end
         *      The end index to stop the search at.
         *
         * @return the first index that matches one of the chars or the end value if no matches..
         */
        static int findFirstOf(const decaf::lang::String& string,
                               const decaf::lang::String& chars, int start, int end);

    };

}}}

#endif /* _DECAF_INTERNAL_NET_URLUTILS_H_ */
