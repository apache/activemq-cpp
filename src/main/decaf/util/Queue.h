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
#ifndef _DECAF_UTIL_QUEUE_H_
#define _DECAF_UTIL_QUEUE_H_

#include <decaf/util/Config.h>
#include <decaf/util/Iterator.h>
#include <decaf/util/AbstractCollection.h>
#include <decaf/lang/Exception.h>
#include <decaf/lang/exceptions/IndexOutOfBoundsException.h>
#include <decaf/util/NoSuchElementException.h>

namespace decaf{
namespace util{

    /**
     * A kind of collection provides advanced operations than other basic
     * collections, such as insertion, extraction, and inspection.
     *
     * Generally, a queue orders its elements by means of first-in-first-out. While
     * priority queue orders its elements according to a comparator specified or the
     * elements' natural order. Furthermore, a stack orders its elements
     * last-in-first out.
     *
     * Queue does not provide blocking queue methods, which will block until the
     * operation of the method is allowed. BlockingQueue interface defines such
     * methods.
     *
     * Unlike the Java Queue interface the methods of this class cannot return null
     * to indicate that a Queue is empty since null has no meaning for elements such
     * as classes, structs and primitive types and cannot be used in a meaningful way
     * to check for an empty queue.  Methods that would have returned null in the
     * Java Queue interface have been altered to return a boolean value indicating if
     * the operation succeeded and take single argument that is a reference to the
     * location where the returned value is to be assigned.  This implies that elements
     * in the Queue must be <em>assignable</em> in order to utilize these methods.
     *
     * @since 1.0
     */
    template <typename E>
    class Queue : public virtual decaf::util::Collection<E> {
    public:

        virtual ~Queue() {}

        /**
         * Inserts the specified element into the queue provided that the condition
         * allows such an operation. The method is generally preferable to the
         * collection.add(E), since the latter might throw an exception if the
         * operation fails.
         *
         * @param value
         *        the specified element to insert into the queue.
         *
         * @return true if the operation succeeds and false if it fails.
         *
         * @throws NullPointerException if the Queue implementation does not allow Null values to
         *         be inserted into the Queue.
         * @throws IllegalArgumentException if some property of the specified
         *         element prevents it from being added to this queue
         */
        virtual bool offer( const E& value ) = 0;

        /**
         * Gets and removes the element in the head of the queue.  If the operation succeeds the
         * value of the element at the head of the Queue is assigned to the result parameter and
         * the method returns true.  If the operation fails the method returns false and the value
         * of the result parameter is undefined.
         *
         * @param result
         *        Reference to an instance of the contained type to assigned the removed value to.
         *
         * @return true if the element at the head of the queue was removed and assigned to the
         *         result parameter.
         */
        virtual bool poll( E& result ) = 0;

        /**
         * Gets and removes the element in the head of the queue. Throws a
         * NoSuchElementException if there is no element in the queue.
         *
         * @return the element in the head of the queue.
         *
         * @throws NoSuchElementException
         *         if there is no element in the queue.
         */
        virtual E remove() = 0;

        /**
         * Gets but not removes the element in the head of the queue.  The result if successful is
         * assigned to the result parameter.
         *
         * @param result
         *        Reference to an instance of the contained type to assigned the removed value to.
         *
         * @return true if the element at the head of the queue was removed and assigned to the
         *         result parameter.
         */
        virtual bool peek( E& result ) const = 0;

        /**
         * Gets but not removes the element in the head of the queue. Throws a
         * NoSuchElementException if there is no element in the queue.
         *
         * @return the element in the head of the queue.
         *
         * @throws NoSuchElementException if there is no element in the queue.
         */
        virtual E element() const = 0;

    };

}}

#endif /*_DECAF_UTIL_QUEUE_H_*/
