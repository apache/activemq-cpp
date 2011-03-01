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

#ifndef _DECAF_UTIL_ABSTRACTLIST_H_
#define _DECAF_UTIL_ABSTRACTLIST_H_

#include <decaf/util/Config.h>
#include <decaf/lang/exceptions/UnsupportedOperationException.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>
#include <decaf/lang/Iterable.h>
#include <decaf/util/Iterator.h>
#include <decaf/util/List.h>
#include <memory>

namespace decaf {
namespace util {

    /**
     * This class provides a skeletal implementation of the List  interface to minimize
     * the effort required to implement this interface backed by a "random access" data
     * store (such as an array). For sequential access data (such as a linked list),
     * AbstractSequentialList should be used in preference to this class.
     *
     * To implement an unmodifiable list, the programmer needs only to extend this class
     * and provide implementations for the get(int) and size() methods.
     *
     * To implement a modifiable list, the programmer must additionally override the
     * set(int, E) method (which otherwise throws an UnsupportedOperationException). If
     * the list is variable-size the programmer must additionally override the add(int, E)
     * and remove(int) methods.
     *
     * The programmer should generally provide a void (no argument) and collection
     * constructor, as per the recommendation in the Collection interface specification.
     *
     * Unlike the other abstract collection implementations, the programmer does not have
     * to provide an iterator implementation; the iterator and list iterator are implemented
     * by this class, on top of the "random access" methods: get(int), set(int, E),
     * add(int, E) and remove(int).
     *
     * The documentation for each non-abstract method in this class describes its
     * implementation in detail. Each of these methods may be overridden if the collection
     * being implemented admits a more efficient implementation.
     *
     * @since 1.0
     */
    template< typename E >
    class AbstractList : public decaf::util::List<E> {
    public:

        virtual ~AbstractList() {}

    };

}}

#endif /* _DECAF_UTIL_ABSTRACTLIST_H_ */
