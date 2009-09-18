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
    class AbstractCollection : public decaf::util::Collection<E> {
    protected:

        mutable util::concurrent::Mutex mutex;

    public:

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
         * Ensures that this collection contains the specified element (optional operation).
         * Returns true if this collection changed as a result of the call. (Returns false if
         * this collection does not permit duplicates and already contains the specified element.)
         *
         * Collections that support this operation may place limitations on what elements may be
         * added to this collection. Collection classes should clearly specify in their
         * documentation any restrictions on what elements may be added.
         *
         * If a collection refuses to add a particular element for any reason other than that it
         * already contains the element, it must throw an exception (rather than returning false).
         * This preserves the invariant that a collection always contains the specified element
         * after this call returns.
         *
         * This implementation always throws an UnsupportedOperationException.
         *
         * @param value - The element that must be ensured to be in this collection.
         *
         * @return true if the collection was changed as a result of this call.
         *
         * @throw UnsupportedOperationException
         *        if the add operation is not supported by this collection
         * @throw IllegalArgumentException
         *        if some property of the element prevents it from being added to this collection
         * @throw IllegalStateException
         *        if the element cannot be added at this time due to insertion restrictions
         */
        virtual bool add( const E& value DECAF_UNUSED )
            throw ( lang::exceptions::UnsupportedOperationException,
                    lang::exceptions::IllegalArgumentException,
                    lang::exceptions::IllegalStateException ) {

            throw decaf::lang::exceptions::UnsupportedOperationException(
                __FILE__, __LINE__, "AbstractCollection add is not implemented.");
        }

        /**
         * Adds all of the elements in the specified collection to this collection (optional
         * operation). The behavior of this operation is undefined if the specified collection
         * is modified while the operation is in progress. (This implies that the behavior of
         * this call is undefined if the specified collection is this collection, and this
         * collection is nonempty.)
         *
         * This implementation iterates over the specified collection, and adds each object
         * returned by the iterator to this collection, in turn.
         *
         * Note that this implementation will throw an UnsupportedOperationException unless add
         * is overridden (assuming the specified collection is non-empty).
         *
         * @param collection - The Collection whose elements are to be added to this Collection.
         *
         * @return true if the collection was changed as a result of this call.
         *
         * @throw UnsupportedOperationException
         *        if the addAll operation is not supported by this collection
         * @throw IllegalArgumentException
         *        if some property of the element prevents it from being added to this collection
         * @throw IllegalStateException
         *        if the element cannot be added at this time due to insertion restrictions
         */
        virtual bool addAll( const Collection<E>& collection )
            throw ( lang::exceptions::UnsupportedOperationException,
                    lang::exceptions::IllegalArgumentException,
                    lang::exceptions::IllegalStateException ) {

            bool result = false;
            std::auto_ptr< Iterator<E> > iter( collection.iterator() );
            while( iter->hasNext() ) {
                result = this->add( iter->next() ) || result;
            }

            return result;
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
        virtual void clear()
            throw ( lang::exceptions::UnsupportedOperationException ) {

            std::auto_ptr< Iterator<E> > iter( this->iterator() );
            while( iter->hasNext() ) {
                iter->next();
                iter->remove();
            }
        }

        /**
         * Renders this Collection as a Copy of the given Collection
         *
         * This implementation iterates over the contents of the given collection adding each
         * to this collection after first calling this Collection's clear method.
         *
         * @param collection - the collection to mirror.
         */
        virtual void copy( const Collection<E>& collection ) {
            this->clear();

            std::auto_ptr< Iterator<E> > iter( collection.iterator() );
            while( iter->hasNext() ) {
                this->add( iter->next() );
            }
        }

        /**
         * Returns true if this collection contains the specified element.
         *
         * This implementation iterates over the elements in the collection, checking each
         * element in turn for equality with the specified element.
         *
         * @param value - the value whose presence is to be queried for in this Collection.
         *
         * @return true if the value is contained in this collection
         *
         * @throw Exception if an error occurs,
         */
        virtual bool contains( const E& value ) const throw ( lang::Exception ) {

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
         * Returns true if this collection contains all of the elements in the specified
         * collection.
         *
         * This implementation iterates over the specified collection, checking each element
         * returned by the iterator in turn to see if it's contained in this collection. If
         * all elements are so contained true is returned, otherwise false.
         *
         * @param collection
         *        collection to be checked for containment in this collection
         *
         * @return true if this collection contains all of the elements in the specified
         *         collection.
         *
         * @throw Exception if an error occurs,
         */
        virtual bool containsAll( const Collection<E>& collection ) const
            throw ( lang::Exception ) {

            bool result = false;
            std::auto_ptr< Iterator<E> > iter( collection.iterator() );
            while( iter->hasNext() ) {
                result = this->contains( iter->next() ) || result;
            }

            return result;
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
         * Removes a single instance of the specified element from this collection, if it is
         * present (optional operation). More formally, removes the first element e such that
         * e == o, if this collection contains one or more such elements. Returns true if this
         * collection contained the specified element (or equivalently, if this collection
         * changed as a result of the call).
         *
         * This implementation iterates over the collection looking for the specified element. If
         * it finds the element, it removes the element from the collection using the iterator's
         * remove method.
         *
         * Note that this implementation throws an UnsupportedOperationException if the iterator
         * returned by this collection's iterator method does not implement the remove method and
         * this collection contains the specified object.
         *
         * @param value - element to be removed from this collection, if present
         *
         * @return true if an element was removed as a result of this call
         *
         * @throw UnsupportedOperationException
         *        if the remove operation is not supported by this collection.
         * @throw IllegalArgumentException
         *        If the value is not a valid entry for this Collection.
         */
        virtual bool remove( const E& value )
            throw ( lang::exceptions::UnsupportedOperationException,
                    lang::exceptions::IllegalArgumentException ) {

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
         * Removes all of this collection's elements that are also contained in the specified
         * collection (optional operation). After this call returns, this collection will contain
         * no elements in common with the specified collection.
         *
         * This implementation iterates over this collection, checking each element returned by
         * the iterator in turn to see if it's contained in the specified collection. If it's so
         * contained, it's removed from this collection with the iterator's remove method.
         *
         * Note that this implementation will throw an UnsupportedOperationException if the
         * iterator returned by the iterator method does not implement the remove method and this
         * collection contains one or more elements in common with the specified collection.
         *
         * @param collection - collection containing elements to be removed from this collection
         *
         * @return true if this collection changed as a result of the call
         *
         * @throw UnsupportedOperationException
         *        if the remove operation is not supported by this collection
         * @throw IllegalArgumentException.
         */
        virtual bool removeAll( const Collection<E>& collection )
            throw ( lang::exceptions::UnsupportedOperationException,
                    lang::exceptions::IllegalArgumentException ) {

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
         * Retains only the elements in this collection that are contained in the specified
         * collection (optional operation). In other words, removes from this collection all of
         * its elements that are not contained in the specified collection.
         *
         * This implementation iterates over this collection, checking each element returned by
         * the iterator in turn to see if it's contained in the specified collection. If it's not
         * so contained, it's removed from this collection with the iterator's remove method.
         *
         * Note that this implementation will throw an UnsupportedOperationException if the
         * iterator returned by the iterator method does not implement the remove method and this
         * collection contains one or more elements not present in the specified collection.
         *
         * @param collection - collection containing elements to be retained in this collection
         *
         * @return true if this collection changed as a result of the call
         *
         * @throw UnsupportedOperationException
         *        if the remove operation is not supported by this collection
         * @throw IllegalArgumentException.
         */
        virtual bool retainAll( const Collection<E>& collection )
            throw ( lang::exceptions::UnsupportedOperationException,
                    lang::exceptions::IllegalArgumentException ) {

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

        virtual void lock() throw( decaf::lang::exceptions::RuntimeException ) {
            mutex.lock();
        }

        virtual bool tryLock() throw( decaf::lang::exceptions::RuntimeException ) {
            return mutex.tryLock();
        }

        virtual void unlock() throw( decaf::lang::exceptions::RuntimeException ) {
            mutex.unlock();
        }

        virtual void wait() throw( decaf::lang::exceptions::RuntimeException,
                                   decaf::lang::exceptions::IllegalMonitorStateException,
                                   decaf::lang::exceptions::InterruptedException ) {

            mutex.wait();
        }

        virtual void wait( long long millisecs )
            throw( decaf::lang::exceptions::RuntimeException,
                   decaf::lang::exceptions::IllegalMonitorStateException,
                   decaf::lang::exceptions::InterruptedException ) {

            mutex.wait( millisecs );
        }

        virtual void wait( long long millisecs, int nanos )
            throw( decaf::lang::exceptions::RuntimeException,
                   decaf::lang::exceptions::IllegalArgumentException,
                   decaf::lang::exceptions::IllegalMonitorStateException,
                   decaf::lang::exceptions::InterruptedException ) {

            mutex.wait( millisecs, nanos );
        }

        virtual void notify() throw( decaf::lang::exceptions::RuntimeException,
                                     decaf::lang::exceptions::IllegalMonitorStateException ) {

            mutex.notify();
        }

        virtual void notifyAll() throw( decaf::lang::exceptions::RuntimeException,
                                        decaf::lang::exceptions::IllegalMonitorStateException ) {

            mutex.notifyAll();
        }

    };

}}

#endif /*_DECAF_UTIL_ABSTRACTCOLLECTION_H_*/
