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

#ifndef _DECAF_UTIL_DEQUE_H_
#define _DECAF_UTIL_DEQUE_H_

#include <decaf/util/NoSuchElementException.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>
#include <decaf/lang/exceptions/IllegalStateException.h>
#include <decaf/util/Config.h>
#include <decaf/util/Queue.h>

namespace decaf {
namespace util {

    /**
     * Defines a 'Double ended Queue' interface that allows for insertion and removal
     * of elements from both ends.  Generally there is no limit on the number of
     * elements that can be placed into a Deque.
     *
     * Unlike a List the Deque doesn't provide index element based access, however
     * methods are provided to grant access to interior elements.
     *
     * @since 1.0
     */
    template<typename E>
    class Deque : public Queue<E> {
    public:

        virtual ~Deque() {}

        /**
         * Inserts an element onto the front of the Deque if possible without violating
         * the implementations capacity restrictions.  For a capacity restricted Deque it
         * is preferable to call offerFirst instead.
         *
         * @param element
         *      The element to be placed at the front of the Deque.
         *
         * @throws IllegalStateException if the element cannot be added at this time due to
         *         capacity restrictions
         * @throws NullPointerException if the specified element is NULL and this deque is a Collection
         *         of pointers and does not permit null elements.
         * @throws IllegalArgumentException if some property of the specified element prevents it
         *         from being added to this deque.
         */
        virtual void addFirst( const E& element ) = 0;

        /**
         * Inserts an element onto the end of the Deque if possible without violating
         * the implementations capacity restrictions.  For a capacity restricted Deque it
         * is preferable to call offerLast instead.
         *
         * @param element
         *      The element to be placed at the end of the Deque.
         *
         * @throws IllegalStateException if the element cannot be added at this time due to
         *         capacity restrictions
         * @throws NullPointerException if the specified element is NULL and this deque is a Collection
         *         of pointers and does not permit null elements.
         * @throws IllegalArgumentException if some property of the specified element prevents it
         *         from being added to this deque.
         */
        virtual void addLast( const E& element ) = 0;

        /**
         * This method attempts to insert the given element into the Deque at the front end.
         * Unlike the addFirst method that throws an exception if it cannot insert the element due
         * to capacity restrictions etc this method returns false if it cannot insert the element.
         *
         * @param element
         *      The element to add to this Deque.
         *
         * @returns true if the element was added, false otherwise.
         *
         * @throws NullPointerException if the specified element is NULL and this deque is a Collection
         *         of pointers and does not permit null elements.
         * @throws IllegalArgumentException if some property of the specified element prevents it
         *         from being added to this deque.
         */
        virtual bool offerFirst( const E& element ) = 0;

        /**
         * This method attempts to insert the given element into the Deque at the end. Unlike
         * the addLast method that throws an exception if it cannot insert the element due
         * to capacity restrictions etc this method returns false if it cannot insert the element.
         *
         * @param element
         *      The element to add to this Deque.
         *
         * @returns true if the element was added, false otherwise.
         *
         * @throws NullPointerException if the specified element is NULL and this deque is a Collection
         *         of pointers and does not permit null elements.
         * @throws IllegalArgumentException if some property of the specified element prevents it
         *         from being added to this deque.
         */
        virtual bool offerLast( const E& element ) = 0;

        /**
         * Removes the topmost element from the Deque and returns it.  Unlike the pollFirst
         * method this method throws a NuSuchElementException if the Deque is empty.
         *
         * @returns the element at the Head of the Deque.
         *
         * @throws NoSuchElementException if the Deque is empty.
         */
        virtual E removeFirst() = 0;

        /**
         * Removes the last element from the Deque and returns it.  Unlike the pollLast
         * method this method throws a NuSuchElementException if the Deque is empty.
         *
         * @returns the element at the Tail of the Deque.
         *
         * @throws NoSuchElementException if the Deque is empty.
         */
        virtual E removeLast() = 0;

        /**
         * Removes the first element from the Deque assigns it to the element reference passed.
         *
         * @param element
         *      Reference to an variable that can be assigned the value of the head of this Deque.
         *
         * @returns true if an element was available to remove, false otherwise.
         */
        virtual bool pollFirst( E& element ) = 0;

        /**
         * Removes the last element from the Deque assigns it to the element reference passed.
         *
         * @param element
         *      Reference to an variable that can be assigned the value of the tail of this Deque.
         *
         * @returns true if an element was available to remove, false otherwise.
         */
        virtual bool pollLast( E& element ) = 0;

        /**
         * Attempts to fetch a reference to the first element in the Deque.  This method does
         * not remove the element from the Deque but simply returns a reference to it.
         *
         * @returns reference to the first element in the Deque.
         *
         * @throws NoSuchElementException if the Deque is empty.
         */
        virtual E& getFirst() = 0;
        virtual const E& getFirst() const = 0;

        /**
         * Attempts to fetch a reference to the last element in the Deque.  This method does
         * not remove the element from the Deque but simply returns a reference to it.
         *
         * @returns reference to the last element in the Deque.
         *
         * @throws NoSuchElementException if the Deque is empty.
         */
        virtual E& getLast() = 0;
        virtual const E& getLast() const = 0;

        /**
         * Retrieves the first element contained in this Deque and assigns its value to the
         * reference value passed the value however is not removed from the Deque.  If this
         * call is successful it returns true.  Unlike getFirst this method does not throw an
         * exception if the Deque is empty.
         *
         * @returns true if an element was assigned to the reference passed, false otherwise.
         */
        virtual bool peekFirst( E& value ) const = 0;

        /**
         * Retrieves the last element contained in this Deque and assigns its value to the
         * reference value passed the value however is not removed from the Deque.  If this
         * call is successful it returns true.  Unlike getLast this method does not throw an
         * exception if the Deque is empty.
         *
         * @returns true if an element was assigned to the reference passed, false otherwise.
         */
        virtual bool peekLast( E& value ) const = 0;

        /**
         * Removes the first occurrence of the specified element from this Deque.  If there is no
         * matching element then the Deque is left unchanged.
         *
         * @param value
         *      The value to be removed from this Deque.
         *
         * @returns true if the Deque was modified as a result of this operation.
         *
         * @throws NullPointerException if the specified element is NULL and this deque is a Collection
         *         of pointers and does not permit null elements.
         */
        virtual bool removeFirstOccurrence( const E& value ) = 0;

        /**
         * Removes the last occurrence of the specified element from this Deque.  If there is no
         * matching element then the Deque is left unchanged.
         *
         * @param value
         *      The value to be removed from this Deque.
         *
         * @returns true if the Deque was modified as a result of this operation.
         *
         * @throws NullPointerException if the specified element is NULL and this deque is a Collection
         *         of pointers and does not permit null elements.
         */
        virtual bool removeLastOccurrence( const E& value ) = 0;

        /**
         * Pushes an element onto the stack represented by this deque (in other words, at the head
         * of this deque) if it is possible to do so immediately without violating capacity restrictions,
         * otherwise it throwing an IllegalStateException if no space is currently available.
         *
         * This method performs the same basic operation as the addFirst method.
         *
         * @param element
         *      The element to be pushed onto the Deque.
         *
         * @throws IllegalStateException if the element cannot be added at this time due to
         *         capacity restrictions
         * @throws NullPointerException if the specified element is NULL and this deque is a Collection
         *         of pointers and does not permit null elements.
         * @throws IllegalArgumentException if some property of the specified element prevents it
         *         from being added to this deque.
         */
        virtual void push( const E& element ) = 0;

        /**
         * Treats this Deque as a stack and attempts to pop an element off the top.  If there's no
         * element to pop then a NuSuchElementException is thrown, otherwise the top element is removed
         * and assigned to the reference passed.
         *
         * This operation performs the same basic function as the removeFirst method.
         *
         * @return the element at the front of this deque which would be the top of a stack.
         *
         * @throws NoSuchElementException if there is nothing on the top of the stack.
         */
        virtual E pop() = 0;

        /**
         * Provides an Iterator over this Collection that traverses the element in reverse order.
         *
         * @returns a new Iterator instance that moves from last to first.
         */
        virtual Iterator<E>* descendingIterator() = 0;
        virtual Iterator<E>* descendingIterator() const = 0;

    };

}}

#endif /* _DECAF_UTIL_DEQUE_H_ */
