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

#ifndef _DECAF_LANG_ITERABLE_H_
#define _DECAF_LANG_ITERABLE_H_

#include <decaf/util/Config.h>
#include <decaf/util/Iterator.h>

namespace decaf{
namespace lang{

    /**
     * Implementing this interface allows an object to be cast to an Iterable
     * type for generic collections API calls.
     */
    template< typename E >
    class Iterable {
    public:

        virtual ~Iterable() {}

        /**
         * @returns an iterator over a set of elements of type T.
         */
        virtual decaf::util::Iterator<E>* iterator() = 0;
        virtual decaf::util::Iterator<E>* iterator() const = 0;

    };

}}

#endif /*_DECAF_LANG_ITERABLE_H_*/
