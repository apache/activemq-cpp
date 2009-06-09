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

#ifndef _DECAF_UTIL_LIST_H_
#define _DECAF_UTIL_LIST_H_

#include <decaf/lang/exceptions/NoSuchElementException.h>
#include <decaf/lang/exceptions/IndexOutOfBoundsException.h>
#include <decaf/util/concurrent/Synchronizable.h>
#include <decaf/util/Config.h>
#include <decaf/util/Iterator.h>
#include <decaf/util/AbstractCollection.h>
#include <decaf/util/ListIterator.h>

namespace decaf{
namespace util{

    /**
     * An ordered collection (also known as a sequence). The user of this interface has
     * precise control over where in the list each element is inserted. The user can
     * access elements by their integer index (position in the list), and search for
     * elements in the list.
     *
     * Unlike sets, lists typically allow duplicate elements. More formally, lists
     * typically allow pairs of elements e1 and e2 such that e1.equals(e2), and they
     * typically allow multiple null elements if they allow null elements at all.
     * It is not inconceivable that someone might wish to implement a list that
     * prohibits duplicates, by throwing runtime exceptions when the user attempts
     * to insert them, but we expect this usage to be rare.
     */
    template <typename E>
    class DECAF_API List : public decaf::util::AbstractCollection<E> {
    public:

        virtual ~List() {}

        /**
         * @return a list iterator over the elements in this list (in proper sequence).
         */
        virtual ListIterator<E>* listIterator() = 0;
        virtual ListIterator<E>* listIterator() const = 0;

        /**
         * @param index index of first element to be returned from the list iterator
         *              (by a call to the next method).
         *
         * @return a list iterator of the elements in this list (in proper sequence),
         *         starting at the specified position in this list. The specified index
         *         indicates the first element that would be returned by an initial call
         *         to next. An initial call to previous would return the element with the
         *         specified index minus one.
         *
         * @throws IndexOutOfBoundsException if the index is out of range
         *         (index < 0 || index > size())
         */
        virtual ListIterator<E>* listIterator( std::size_t index )
            throw( decaf::lang::exceptions::IndexOutOfBoundsException ) = 0;
        virtual ListIterator<E>* listIterator( std::size_t index ) const
            throw( decaf::lang::exceptions::IndexOutOfBoundsException ) = 0;

        /**
         * Returns the index of the first occurrence of the specified element in
         * this list, or -1 if this list does not contain the element. More
         * formally, returns the lowest index i such that get(i) == value, or
         * -1 if there is no such index.
         *
         * @param value - element to search for
         * @return the index of the first occurrence of the specified element in
         * this list,
         * @throw NoSuchElementException if value is not in the list
         */
        virtual std::size_t indexOf( const E& value )
            throw ( decaf::lang::exceptions::NoSuchElementException ) = 0;

        /**
         * Returns the index of the last occurrence of the specified element in
         * this list, or -1 if this list does not contain the element. More
         * formally, returns the highest index i such that get(i) == value
         * or -1 if there is no such index.
         *
         * @param value - element to search for
         * @return the index of the last occurrence of the specified element in
         * this list.
         * @throw NoSuchElementException if value is not in the list
         */
        virtual size_t lastIndexOf( const E& value )
            throw ( decaf::lang::exceptions::NoSuchElementException ) = 0;

        /**
         * Gets the element contained at position passed
         * @param index - position to get
         * @return value at index
         */
        virtual E get( std::size_t index ) const
            throw ( decaf::lang::exceptions::IndexOutOfBoundsException ) = 0;

        /**
         * Replaces the element at the specified position in this list with the
         * specified element.
         *
         * @param index - index of the element to replace
         * @param element - element to be stored at the specified position
         * @return the element previously at the specified position
         * @throw IndexOutOfBoundsException - if the index is greater than size
         */
        virtual E set( std::size_t index, const E& element )
            throw ( decaf::lang::exceptions::IndexOutOfBoundsException ) = 0;

        /**
         * Inserts the specified element at the specified position in this list.
         * Shifts the element currently at that position (if any) and any
         * subsequent elements to the right (adds one to their indices).
         *
         * @param index - index at which the specified element is to be inserted
         * @param element - element to be inserted
         *
         * @throw IndexOutOfBoundsException - if the index is greater than size
         * @throw UnsupportedOperationException - If the collection is non-modifiable.
         */
        virtual void add( std::size_t index, const E& element )
            throw ( decaf::lang::exceptions::UnsupportedOperationException,
                    decaf::lang::exceptions::IndexOutOfBoundsException ) = 0;

        /**
         * Inserts all of the elements in the specified collection into this list at
         * the specified position (optional operation). Shifts the element currently at
         * that position (if any) and any subsequent elements to the right (increases
         * their indices). The new elements will appear in this list in the order that
         * they are returned by the specified collection's iterator. The behavior of this
         * operation is undefined if the specified collection is modified while the
         * operation is in progress. (Note that this will occur if the specified collection
         * is this list, and it's nonempty.)
         *
         * @param index
         *      The index at which to insert the first element from the specified collection
         * @param source
         *      The Collection containing elements to be added to this list
         *
         * @return true if this list changed as a result of the call
         *
         * @throw IndexOutOfBoundsException - if the index is greater than size
         * @throw UnsupportedOperationException - If the collection is non-modifiable.
         */
        virtual bool addAll( std::size_t index, const Collection<E>& source )
            throw ( decaf::lang::exceptions::UnsupportedOperationException,
                    decaf::lang::exceptions::IndexOutOfBoundsException ) = 0;

        /**
         * Removes the element at the specified position in this list.
         * Shifts any subsequent elements to the left (subtracts one from their
         * indices). Returns the element that was removed from the list.
         *
         * @param index - the index of the element to be removed
         * @return the element previously at the specified position
         * @throw IndexOutOfBoundsException - if the index is greater than size
         * @throw UnsupportedOperationException - If the collection is non-modifiable.
         */
        virtual E remove( std::size_t index )
            throw ( decaf::lang::exceptions::UnsupportedOperationException,
                    decaf::lang::exceptions::IndexOutOfBoundsException ) = 0;

    };

}}

#endif /*_DECAF_UTIL_LIST_H_*/
