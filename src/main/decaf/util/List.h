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

#include <list>
#include <algorithm>
#include <decaf/lang/exceptions/NoSuchElementException.h>
#include <decaf/lang/exceptions/IndexOutOfBoundsException.h>
#include <decaf/util/concurrent/Synchronizable.h>
#include <decaf/util/concurrent/Mutex.h>
#include <decaf/util/Iterator.h>

namespace decaf{
namespace util{

    /**
     * Set template that wraps around a std::set to provide a more
     * user-friendly interface and to provide common functions that do
     * not exist in std::list.
     */
    template <typename E> class List : public util::concurrent::Synchronizable {
    private:

        std::list<E> values;
        util::concurrent::Mutex mutex;

    private:

        class ListIterator : public Iterator<E> {
        private:

            typename std::list<E>::iterator current;
            typename std::list<E>::iterator previous;
            typename std::list<E>* list;

        public:

            ListIterator( typename std::list<E>* list ) {
                this->current = list->begin();
                this->previous = list->end();
                this->list = list;
            }
            virtual ~ListIterator() {}

            virtual E next() throw( lang::exceptions::NoSuchElementException ){
                if( this->current == list->end() ) {
                    throw lang::exceptions::NoSuchElementException(
                        __FILE__, __LINE__,
                        "List::Iterator::next - No more elements to return" );
                }

                this->previous = this->current;
                return *( this->current++ );
            }

            virtual bool hasNext() const {
                return ( this->current != list->end() );
            }

            virtual void remove() throw ( lang::exceptions::IllegalStateException,
                                          lang::exceptions::UnsupportedOperationException ){
                if( this->previous == list->end() ) {
                    throw lang::exceptions::IllegalStateException(
                        __FILE__, __LINE__,
                        "List::Iterator::remove - Invalid State to call remove" );
                }

                this->list->erase( this->previous );
                this->previous = this->list->end();
            }
        };

    public:

        /**
         * Default constructor - does nothing.
         */
        List(){}

        /**
         * Copy constructor - copies the content of the given set into this
         * one.
         * @param source The source set.
         */
        List( const List& source ) : decaf::util::concurrent::Synchronizable() {
            copy( source );
        }

        virtual ~List(){}

        /**
         * Comparison, equality is dependant on the method of determining
         * if the element are equal.
         * @param source - List to compare to this one.
         * @returns true if the List passed is equal in value to this one.
         */
        virtual bool equals( const List& source ) const {
            return this->values == source.values;
        }

        /**
         * Returns an iterator for this collection.  The order of Iteration
         * is in no particular order other than the natural ording of the
         * elements in the List class.
         * @returns Iterator<E> for this collection
         */
        Iterator<E>* iterator() {
            return new ListIterator( &values );
        }

        /**
         * Copies the content of the source set into this set.  Erases
         * all existing data in this st.
         * @param source The source object to copy from.
         */
        virtual void copy( const List& source ) {
            // Add all of the entries to this map.
            values = source.values;
        }

        /**
         * Removes all values from this set.
         */
        virtual void clear() {
            values.clear();
        }

        /**
         * Indicates whether or this set contains the given value.
         * @param value The value to look up.
         * @return true if this set contains the value, otherwise false.
         */
        virtual bool contains( const E& value ) const {
            typename std::list<E>::const_iterator iter;
            iter = std::find( values.begin(), values.end(), value );
            return iter != values.end();
        }

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
        virtual size_t indexOf( const E& value )
            throw ( decaf::lang::exceptions::NoSuchElementException ) {

            typename std::list<E>::iterator iter;
            iter = std::find( values.begin(), values.end(), value );

            if( iter == values.end() ) {
                throw decaf::lang::exceptions::NoSuchElementException(
                    __FILE__, __LINE__,
                    "List::indexOf - No matching element in list" );
            }

            return std::distance( values.begin(), iter );
        }

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
        virtual size_t lastIndexOf( const E& value ) {
            typename std::list<E>::reverse_iterator iter;
            iter = std::find( values.rbegin(), values.rend(), value );

            if( iter == values.rend() ) {
                throw decaf::lang::exceptions::NoSuchElementException(
                    __FILE__, __LINE__,
                    "List::lastIndexOf - No matching element in list" );
            }

            // Now reverse the result to get the last index
            return this->size() - std::distance( values.rbegin(), iter ) - 1;
        }

        /**
         * @return if the set contains any element or not, TRUE or FALSE
         */
        virtual bool isEmpty() const {
            return values.empty();
        }

        /**
         * @return The number of elements in this set.
         */
        virtual std::size_t size() const {
            return values.size();
        }

        /**
         * Gets the element contained at position passed
         * @param index - position to get
         * @return value at index
         */
        virtual E get( std::size_t index ) const
            throw ( decaf::lang::exceptions::IndexOutOfBoundsException ) {

            if( index >= this->size() ) {
                throw decaf::lang::exceptions::IndexOutOfBoundsException(
                    __FILE__, __LINE__,
                    "List::get - Index greater than size()" );
            }

            // Advance from begin and return the value at that location.
            typename std::list<E>::const_iterator iter = this->values.begin();
            std::advance( iter, index );
            return *( iter );
        }

        /**
         * Replaces the element at the specified position in this list with the
         * specified element.
         *
         * @param index - index of the element to replace
         * @param element - element to be stored at the specified position
         * @return the element previously at the specified position
         * @throw IndexOutOfBoundsException - if the index is greater tham size
         */
        virtual E set( std::size_t index, E element )
            throw ( decaf::lang::exceptions::IndexOutOfBoundsException ){

            if( index >= this->size() ) {
                throw decaf::lang::exceptions::IndexOutOfBoundsException(
                    __FILE__, __LINE__,
                    "List::get - Index greater than size()" );
            }

            // Advance from begin and return the value at that location
            // after setting the value to the new value.
            typename std::list<E>::iterator iter = this->values.begin();
            std::advance( iter, index );
            E oldValue = *iter;
            *iter = element;

            return oldValue;
        }

        /**
         * Adds the given value to the set.
         * @param value The value to add.
         */
        virtual void add( const E& value ) {
            values.insert( values.end(), value );
        }

        /**
         * Inserts the specified element at the specified position in this list.
         * Shifts the element currently at that position (if any) and any
         * subsequent elements to the right (adds one to their indices).
         *
         * @param index - index at which the specified element is to be inserted
         * @param element - element to be inserted
         * @throw IndexOutOfBoundsException - if the index is greater tham size
         */
        virtual void add( std::size_t index, E element )
            throw ( decaf::lang::exceptions::IndexOutOfBoundsException ){

            if( index > this->size() ) {
                throw decaf::lang::exceptions::IndexOutOfBoundsException(
                    __FILE__, __LINE__,
                    "List::add - Index greater than size()" );
            }

            // Advance from begin and insert the value at that location
            typename std::list<E>::iterator iter = this->values.begin();
            std::advance( iter, index );
            this->values.insert( iter, element );
        }

        /**
         * Removes the value from the set.
         * @param value The value to be removed.
         */
        virtual void remove( const E& value ) {
            values.remove( value );
        }

        /**
         * Removes the element at the specified position in this list.
         * Shifts any subsequent elements to the left (subtracts one from their
         * indices). Returns the element that was removed from the list.
         *
         * @param index - the index of the element to be removed
         * @return the element previously at the specified position
         * @throw IndexOutOfBoundsException - if the index >= size()
         */
        virtual E remove( std::size_t index ) {

            if( index > this->size() ) {
                throw decaf::lang::exceptions::IndexOutOfBoundsException(
                    __FILE__, __LINE__,
                    "List::add - Index greater than size()" );
            }

            // Advance from begin and insert the value at that location
            typename std::list<E>::iterator iter = this->values.begin();
            std::advance( iter, index );
            E oldValue = *iter;
            this->values.erase( iter );

            return oldValue;
        }

        /**
         * @return the all values in this set as a std::vector.
         */
        virtual std::vector<E> toArray() const {
            std::vector<E> valueArray( values.size() );

            typename std::list<E>::const_iterator iter;
            iter=values.begin();
            for( int ix=0; iter != values.end(); ++iter, ++ix ){
                valueArray[ix] = *iter;
            }

            return valueArray;
        }

    public:     // Methods from Synchronizable

        /**
         * Locks the object.
         * @throws Exception
         */
        virtual void lock() throw( lang::Exception ) {
            mutex.lock();
        }

        /**
         * Unlocks the object.
         * @throws Exception
         */
        virtual void unlock() throw( lang::Exception ) {
            mutex.unlock();
        }

        /**
         * Waits on a signal from this object, which is generated
         * by a call to Notify.  Must have this object locked before
         * calling.
         * @throws Exception
         */
        virtual void wait() throw( lang::Exception ) {
            mutex.wait();
        }

        /**
         * Waits on a signal from this object, which is generated
         * by a call to Notify.  Must have this object locked before
         * calling.  This wait will timeout after the specified time
         * interval.
         * @param millisecs the time in millisecsonds to wait, or
         * WAIT_INIFINITE
         * @throws Exception
         */
        virtual void wait( unsigned long millisecs )
            throw( lang::Exception ) {
            mutex.wait( millisecs );
        }

        /**
         * Signals a waiter on this object that it can now wake
         * up and continue.  Must have this object locked before
         * calling.
         * @throws Exception
         */
        virtual void notify() throw(  lang::Exception  ) {
            mutex.notify();
        }

        /**
         * Signals the waiters on this object that it can now wake
         * up and continue.  Must have this object locked before
         * calling.
         * @throws Exception
         */
        virtual void notifyAll() throw(  lang::Exception  ) {
            mutex.notifyAll();
        }

    };

}}

#endif /*_DECAF_UTIL_LIST_H_*/
