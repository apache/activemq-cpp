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

#ifndef _DECAF_UTIL_ITERATOR_H_
#define _DECAF_UTIL_ITERATOR_H_

#include <decaf/util/NoSuchElementException.h>
#include <decaf/lang/exceptions/IllegalStateException.h>
#include <decaf/lang/exceptions/UnsupportedOperationException.h>

namespace decaf{
namespace util{

    /**
     * Defines an object that can be used to iterate over the elements of a
     * collection.  The iterator provides a way to access and remove elements
     * with well defined semantics.
     */
    template< typename E>
    class Iterator {
    public:

        virtual ~Iterator() {}

        /**
         * Returns the next element in the iteration. Calling this method
         * repeatedly until the hasNext() method returns false will return
         * each element in the underlying collection exactly once.
         *
         * @returns the next element in the iteration of elements.
         *
         * @throws NoSuchElementException if the iteration has no more elements.
         */
        virtual E next() = 0;

        /**
         * Returns true if the iteration has more elements.  Returns false if
         * the next call to next would result in an NoSuchElementException to
         * be thrown.
         *
         * @returns true if there are more elements available for iteration.
         */
        virtual bool hasNext() const = 0;

        /**
         * Removes from the underlying collection the last element returned
         * by the iterator (optional operation). This method can be called
         * only once per call to next. The behavior of an iterator is
         * unspecified if the underlying collection is modified while the
         * iteration is in progress in any way other than by calling this
         * method.
         *
         * @throws UnsupportedOperationException if the remove  operation is
         *         not supported by this Iterator.
         * @throws IllegalStateException if the next method has not yet been
         *         called, or the remove method has already been called after
         *         the last call to the next method.
         */
        virtual void remove() = 0;

    };

}}

#endif /*_DECAF_UTIL_ITERATOR_H_*/
