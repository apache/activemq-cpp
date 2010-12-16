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

#ifndef _DECAF_UTIL_ABSTRACTCOLLECTION_H_
#define _DECAF_UTIL_ABSTRACTCOLLECTION_H_

#include <decaf/util/Config.h>
#include <decaf/lang/exceptions/UnsupportedOperationException.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>
#include <decaf/lang/Iterable.h>
#include <decaf/util/Iterator.h>
#include <decaf/util/Collection.h>
#include <decaf/util/concurrent/Synchronizable.h>
#include <decaf/util/concurrent/Mutex.h>
#include <memory>

namespace decaf {
namespace util {

    /**
     * This class provides a skeletal implementation of the Collection  interface, to
     * minimize the effort required to implement this interface.
     *
     * To implement an unmodifiable collection, the programmer needs only to extend this
     * class and provide implementations for the iterator and size methods. (The iterator
     * returned by the iterator method must implement hasNext and next.)
     *
     * To implement a modifiable collection, the programmer must additionally override
     * this class's add method (which otherwise throws an UnsupportedOperationException),
     * and the iterator returned by the iterator method must additionally implement its
     * remove method.
     *
     * The programmer should generally provide a void (no argument) and Collection
     * constructor, as per the recommendation in the Collection interface specification.
     *
     * The documentation for each non-abstract method in this class describes its
     * implementation in detail. Each of these methods may be overridden if the collection
     * being implemented admits a more efficient implementation.
     *
     * @since 1.0
     */
    template< typename E >
    class AbstractCollection : public virtual decaf::util::Collection<E> {
    protected:

        mutable util::concurrent::Mutex mutex;

    public:

        AbstractCollection() : Collection<E>(), mutex() {}

        virtual ~AbstractCollection() {}

        /**
         * Assignment Operator, copy element from the source collection to this
         * collection after clearing any element stored in this collection.
         *
         * @param collection - the collection to copy
         * @return a reference to this collection
         */
        AbstractCollection<E>& operator= ( const AbstractCollection<E>& collection ) {
            this->clear();

            std::auto_ptr< Iterator<E> > iter( collection.iterator() );
            while( iter->hasNext() ) {
                this->add( iter->next() );
            }

            return *this;
        }

        /**
         * Removes all of the elements from this collection (optional operation). The collection
         * will be empty after this method returns.
         *
         * This implementation iterates over this collection, removing each element using the
         * Iterator.remove operation. Most implementations will probably choose to override this
         * method for efficiency.
         *
         * Note that this implementation will throw an UnsupportedOperationException if the
         * iterator returned by this collection's iterator method does not implement the remove
         * method and this collection is non-empty.
         *
         * @throw UnsupportedOperationException
         *        if the clear operation is not supported by this collection
         */
        virtual void clear() {

            std::auto_ptr< Iterator<E> > iter( this->iterator() );
            while( iter->hasNext() ) {
                iter->next();
                iter->remove();
            }
        }

        /**
         * {@inheritDoc}
         *
         * This implementation iterates over the elements in the collection, checking each
         * element in turn for equality with the specified element.
         */
        virtual bool contains( const E& value ) const {

            bool result = false;
            std::auto_ptr< Iterator<E> > iter( this->iterator() );
            while( iter->hasNext() ) {
                if( iter->next() == value ) {
                    result = true;
                }
            }

            return result;
        }

        /**
         * {@inheritDoc}
         *
         * This implementation iterates over the specified collection, checking each element
         * returned by the iterator in turn to see if it's contained in this collection. If
         * all elements are so contained true is returned, otherwise false.
         */
        virtual bool containsAll( const Collection<E>& collection ) const {

            std::auto_ptr< Iterator<E> > iter( collection.iterator() );
            while( iter->hasNext() ) {
                if( !this->contains( iter->next() ) ) {
                    return false;
                }
            }

            return true;
        }

        /**
         * Answers true if this Collection and the one given are the same size and if each
         * element contained in the Collection given is equal to an element contained in this
         * collection.
         *
         * @param collection - The Collection to be compared to this one.
         *
         * @return true if this Collection is equal to the one given.
         */
        virtual bool equals( const Collection<E>& collection ) const {

            if( this == &collection ) {
                return true;
            }

            if( this->size() == collection.size() && this->containsAll( collection ) ) {
                return true;
            }

            return false;
        }

        /**
         * Renders this Collection as a Copy of the given Collection
         *
         * The default implementation iterates over the contents of the given collection adding
         * each to this collection after first calling this Collection's clear method.
         *
         * @param collection
         *      The collection to mirror.
         *
         * @throws UnsupportedOperationExceptio if this is an unmodifiable collection.
         * @throws IllegalStateException if the elements cannot be added at this time due
         *         to insertion restrictions.
         */
        virtual void copy( const Collection<E>& collection ) {
            this->clear();

            std::auto_ptr< Iterator<E> > iter( collection.iterator() );
            while( iter->hasNext() ) {
                this->add( iter->next() );
            }
        }

        /**
         * Returns true if this collection contains no elements.
         *
         * This implementation returns size() == 0.
         *
         * @returns true if the size method return 0.
         */
        virtual bool isEmpty() const {
            return this->size() == 0;
        }

        /**
         * {@inheritDoc}
         *
         * This implementation always throws an UnsupportedOperationException.
         */
        virtual bool add( const E& value DECAF_UNUSED ) {
            throw decaf::lang::exceptions::UnsupportedOperationException(
                __FILE__, __LINE__, "AbstractCollection add is not implemented.");
        }

        /**
         * {@inheritDoc}
         *
         * This implementation iterates over the specified collection, and adds each object
         * returned by the iterator to this collection, in turn.
         *
         * Note that this implementation will throw an UnsupportedOperationException unless add
         * is overridden (assuming the specified collection is non-empty).
         */
        virtual bool addAll( const Collection<E>& collection ) {

            bool result = false;
            std::auto_ptr< Iterator<E> > iter( collection.iterator() );
            while( iter->hasNext() ) {
                result = this->add( iter->next() ) || result;
            }

            return result;
        }

        /**
         * {@inheritDoc}
         *
         * This implementation iterates over the collection looking for the specified element. If
         * it finds the element, it removes the element from the collection using the iterator's
         * remove method.
         *
         * Note that this implementation throws an UnsupportedOperationException if the iterator
         * returned by this collection's iterator method does not implement the remove method and
         * this collection contains the specified object.
         */
        virtual bool remove( const E& value ) {

            std::auto_ptr< Iterator<E> > iter( this->iterator() );
            while( iter->hasNext() ) {
                if( value == iter->next() ) {
                    iter->remove();
                    return true;
                }
            }

            return false;
        }

        /**
         * {@inheritDoc}
         *
         * This implementation iterates over this collection, checking each element returned by
         * the iterator in turn to see if it's contained in the specified collection. If it's so
         * contained, it's removed from this collection with the iterator's remove method.
         *
         * Note that this implementation will throw an UnsupportedOperationException if the
         * iterator returned by the iterator method does not implement the remove method and this
         * collection contains one or more elements in common with the specified collection.
         */
        virtual bool removeAll( const Collection<E>& collection ) {

            bool result = false;
            std::auto_ptr< Iterator<E> > iter( this->iterator() );
            while( iter->hasNext() ) {
                if( collection.contains( iter->next() ) ) {
                    iter->remove();
                    result = true;
                }
            }

            return result;
        }

        /**
         * {@inheritDoc}
         *
         * This implementation iterates over this collection, checking each element returned by
         * the iterator in turn to see if it's contained in the specified collection. If it's not
         * so contained, it's removed from this collection with the iterator's remove method.
         *
         * Note that this implementation will throw an UnsupportedOperationException if the
         * iterator returned by the iterator method does not implement the remove method and this
         * collection contains one or more elements not present in the specified collection.
         */
        virtual bool retainAll( const Collection<E>& collection ) {

            bool result = false;
            std::auto_ptr< Iterator<E> > iter( this->iterator() );
            while( iter->hasNext() ) {
                if( !collection.contains( iter->next() ) ) {
                    iter->remove();
                    result = true;
                }
            }

            return result;
        }

        /**
         * Answers an STL vector containing copies of all elements contained in this Collection.
         * All the elements in the array will not be referenced by the collection. The elements
         * in the returned array will be sorted to the same order as those returned by the
         * iterator of this collection itself if the collection guarantees the order.
         *
         * @return an vector of copies of all the elements from this Collection
         */
        virtual std::vector<E> toArray() const {
            std::vector<E> valueArray;
            valueArray.reserve( this->size() );

            std::auto_ptr< Iterator<E> > iter( this->iterator() );
            while( iter->hasNext() ) {
                valueArray.push_back( iter->next() );
            }

            return valueArray;
        }

    public:

        virtual void lock() {
            mutex.lock();
        }

        virtual bool tryLock() {
            return mutex.tryLock();
        }

        virtual void unlock() {
            mutex.unlock();
        }

        virtual void wait()  {
            mutex.wait();
        }

        virtual void wait( long long millisecs ) {
            mutex.wait( millisecs );
        }

        virtual void wait( long long millisecs, int nanos ) {
            mutex.wait( millisecs, nanos );
        }

        virtual void notify() {
            mutex.notify();
        }

        virtual void notifyAll() {
            mutex.notifyAll();
        }

    };

}}

#endif /*_DECAF_UTIL_ABSTRACTCOLLECTION_H_*/
