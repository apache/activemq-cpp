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

#ifndef _DECAF_UTIL_ABSTRACTSEQUENTIALLIST_H_
#define _DECAF_UTIL_ABSTRACTSEQUENTIALLIST_H_

#include <decaf/util/Config.h>
#include <decaf/lang/exceptions/UnsupportedOperationException.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>
#include <decaf/lang/Iterable.h>
#include <decaf/util/Iterator.h>
#include <decaf/util/AbstractList.h>
#include <memory>

namespace decaf {
namespace util {

    /**
     * This class provides a skeletal implementation of the List  interface to minimize
     * the effort required to implement this interface backed by a "sequential access"
     * data store (such as a linked list). For random access data (such as an array),
     * AbstractList should be used in preference to this class.
     *
     * This class is the opposite of the AbstractList class in the sense that it implements
     * the "random access" methods (get(int index), set(int index, E element), add(int index,
     * E element) and remove(int index)) on top of the list's list iterator, instead of the
     * other way around.
     *
     * To implement a list the programmer needs only to extend this class and provide
     * implementations for the listIterator and size methods. For an unmodifiable list, the
     * programmer need only implement the list iterator's hasNext, next, hasPrevious,
     * previous and index methods.
     *
     * For a modifiable list the programmer should additionally implement the list iterator's
     * set method. For a variable-size list the programmer should additionally implement the
     * list iterator's remove and add methods.
     *
     * The programmer should generally provide a void (no argument) and collection constructor,
     * as per the recommendation in the Collection interface specification.
     *
     * @since 1.0
     */
    template< typename E >
    class AbstractSequentialList : public decaf::util::AbstractList {
    public:

        virtual ~AbstractSequentialList() {}

    };

}}

#endif /* _DECAF_UTIL_ABSTRACTSEQUENTIALLIST_H_ */
