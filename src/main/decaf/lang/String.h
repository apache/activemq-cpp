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

#ifndef _DECAF_LANG_STRING_H_
#define _DECAF_LANG_STRING_H_

#include <decaf/util/Config.h>

#include <decaf/lang/CharSequence.h>
#include <decaf/lang/Comparable.h>

#include <string>

namespace decaf {
namespace lang {

    class StringContents;

    /**
     * The String class represents an immutable sequence of chars.
     *
     * @since 1.0
     */
    class DECAF_API String {
    private:

        StringContents* contents;

    public:

        /**
         * Creates a new empty String object.
         */
        String();

        /**
         * Create a new String object that represents the given STL string
         *
         * @param source
         *      The string to copy into this new String object.
         */
        String( const std::string& source );

        virtual ~String();

    };

}}

#endif /* _DECAF_LANG_STRING_H_ */
