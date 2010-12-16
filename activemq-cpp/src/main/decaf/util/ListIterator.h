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

#ifndef _DECAF_UTIL_LISTITERATOR_H_
#define _DECAF_UTIL_LISTITERATOR_H_

#include <decaf/util/Iterator.h>
#include <decaf/util/Config.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>

namespace decaf{
namespace util{

    /**
     * An iterator for lists that allows the programmer to traverse the list
     * in either direction, modify the list during iteration, and obtain the
     * iterator's current position in the list.
     *
     * Note that the remove() and set(Object) methods are not defined in terms
     * of the cursor position; they are defined to operate on the last element
     * returned by a call to next() or previous().
     */
    template< typename E>
    class ListIterator : public decaf::util::Iterator<E> {
    public:

        virtual ~ListIterator() {}

        /**
         * Inserts the specified element into the list (optional operation). The
         * element is inserted immediately before the next element that would be
         * returned by next, if any, and after the next element that would be
         * returned by previous, if any. (If the list contains no elements, the
         * new element becomes the sole element on the list.) The new element is
         * inserted before the implicit cursor: a subsequent call to next would
         * be unaffected, and a subsequent call to previous would return the new
         * element. (This call increases by one the value that would be returned
         * by a call to nextIndex or previousIndex.)
         *
         * @param e
         *      The element to insert into the List.
         *
         * @throw UnsupportedOperationException if the add method is not
         *        supported by this list iterator.
         * @throw IllegalArgumentException if some aspect of this element
         *        prevents it from being added to this list.
         */
        virtual void add( const E& e ) = 0;

        /**
         * Replaces the last element returned by next or previous with the
         * specified element (optional operation). This call can be made only
         * if neither ListIterator.remove nor ListIterator.add have been
         * called after the last call to next or previous.
         *
         * @param e
         *      The element with which to replace the last element returned
         *      by next or previous.
         *
         * @throw UnsupportedOperationException if the add method is not
         *        supported by this list iterator.
         * @throw IllegalArgumentException if some aspect of this element
         *        prevents it from being added to this list.
         * @throw IllegalStateException if neither next nor previous have been
         *        called, or remove or add have been called after the last call to next
         *        or previous.
         */
        virtual void set( const E& e ) = 0;

        /**
         * Returns true if this list iterator has more elements when traversing the
         * list in the reverse direction. (In other words, returns true if previous
         * would return an element rather than throwing an exception.)
         *
         * @return true if the list iterator has more elements when traversing the list
         *         in the reverse direction.
         */
        virtual bool hasPrevious() const = 0;

        /**
         * Returns the previous element in the list. This method may be called
         * repeatedly to iterate through the list backwards, or intermixed with
         * calls to next to go back and forth. (Note that alternating calls to
         * next and previous will return the same element repeatedly.)
         *
         * @return the previous element in the list.
         *
         * @throw NoSuchElementException if the iteration has no previous element.
         */
        virtual E previous() = 0;

        /**
         * Returns the index of the element that would be returned by a
         * subsequent call to next. (Returns list size if the list iterator
         * is at the end of the list.)
         *
         * @return the index of the element that would be returned by a
         *         subsequent call to next, or list size if list iterator is
         *         at end of list.
         */
        virtual int nextIndex() const = 0;

        /**
         * Returns the index of the element that would be returned by a
         * subsequent call to previous. (Returns -1 if the list iterator is
         * at the beginning of the list.)
         *
         * @return the index of the element that would be returned by a
         *         subsequent call to previous, or -1 if list iterator is
         *         at beginning of list.
         */
        virtual int previousIndex() const = 0;

    };

}}

#endif /*_DECAF_UTIL_LISTITERATOR_H_*/
