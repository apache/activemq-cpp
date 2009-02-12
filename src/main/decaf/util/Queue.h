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
#include <decaf/util/Collection.h>
#include <decaf/lang/Exception.h>
#include <decaf/lang/exceptions/NoSuchElementException.h>
#include <decaf/lang/exceptions/IndexOutOfBoundsException.h>

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
     * Certain methods in the Queue interface return a special value instead of throwing
     * an exception if there is no element in the Queue to return, this special value
     * can be obtained by calling the Queue method <code>getEmptyMarker</code>.
     */
    template <typename E>
    class DECAF_API Queue : public decaf::util::Collection<E> {
    public:

        virtual ~Queue() {}

        /**
         * Returns a reference to the Marker value that is returned from methods that
         * do not throw an exception when there is no element in the Queue to return.
         */
        virtual const E& getEmptyMarker() const = 0;

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
         */
        virtual bool offer( const E& value ) = 0;

        /**
         * Gets and removes the element in the head of the queue, or returns null if
         * there is no element in the queue.
         *
         * @return the element in the head of the queue or null if there is no
         *         element in the queue.
         */
        virtual E poll() = 0;

        /**
         * Gets and removes the element in the head of the queue. Throws a
         * NoSuchElementException if there is no element in the queue.
         *
         * @return the element in the head of the queue.
         * @throws NoSuchElementException
         *         if there is no element in the queue.
         */
        virtual E remove() throw ( decaf::lang::exceptions::NoSuchElementException ) = 0;

        /**
         * Gets but not removes the element in the head of the queue.
         *
         * @return the element in the head of the queue or null if there is no
         *         element in the queue.
         */
        virtual const E& peek() const = 0;

        /**
         * Gets but not removes the element in the head of the queue. Throws a
         * NoSuchElementException if there is no element in the queue.
         *
         * @return the element in the head of the queue.
         * @throws NoSuchElementException
         *         if there is no element in the queue.
         */
        virtual const E& element() const
            throw( decaf::lang::exceptions::NoSuchElementException ) = 0;

    };

}}

#endif /*_DECAF_UTIL_QUEUE_H_*/
