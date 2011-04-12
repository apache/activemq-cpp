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

#ifndef _DECAF_UTIL_SET_H_
#define _DECAF_UTIL_SET_H_

#include <decaf/util/NoSuchElementException.h>
#include <decaf/util/concurrent/Synchronizable.h>
#include <decaf/util/concurrent/Mutex.h>
#include <decaf/util/Iterator.h>
#include <decaf/util/AbstractCollection.h>

namespace decaf{
namespace util{

    /**
     * A collection that contains no duplicate elements. More formally, sets contain no
     * pair of elements e1 and e2 such that e1 == e2, and at most one null element.
     * As implied by its name, this interface models the mathematical set abstraction.
     *
     * The additional stipulation on constructors is, not surprisingly, that all constructors
     * must create a set that contains no duplicate elements (as defined above).
     *
     * Note: Great care must be exercised if mutable objects are used as set elements.
     * The behavior of a set is not specified if the value of an object is changed in a
     * manner that affects equals comparisons while the object is an element in the set.
     *
     * @since 1.0
     */
    template <typename E>
    class Set : public virtual decaf::util::Collection<E> {
    public:

        virtual ~Set() {}

    };

}}

#endif /*_DECAF_UTIL_SET_H_*/
