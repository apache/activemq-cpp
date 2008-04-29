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

#ifndef _DECAF_UTIL_COLLECTION_H_
#define _DECAF_UTIL_COLLECTION_H_

#include <decaf/util/Config.h>
#include <devaf/lang/exceptions/UnsupportedOperationException.h>
#include <devaf/lang/exceptions/NullPointerException.h>
#include <devaf/lang/exceptions/IllegalArgumentException.h>
#include <decaf/util/Iterator.h>

namespace decaf{
namespace util{

    /**
     * The root interface in the collection hierarchy. A collection represents
     * a group of objects, known as its elements. Some collections allow
     * duplicate elements and others do not. Some are ordered and others
     * unordered. This interface is typically used to pass collections
     * around and manipulate them where maximum generality is desired.
     *
     * All general-purpose Collection implementation classes (which typically
     * implement Collection indirectly through one of its subinterfaces) should
     * provide two "standard" constructors: a void (no arguments) constructor,
     * which creates an empty collection, and a constructor with a single argument
     * of type Collection, which creates a new collection with the same elements
     * as its argument. In effect, the latter constructor allows the user to copy
     * any collection, producing an equivalent collection of the desired
     * implementation type. There is no way to enforce this convention
     * (as interfaces cannot contain constructors) but all of the general-purpose
     * Collection implementations in the Decaf platform libraries comply.
     *
     * The "destructive" methods contained in this interface, that is, the methods
     * that modify the collection on which they operate, are specified to throw
     * UnsupportedOperationException if this collection does not support the
     * operation. If this is the case, these methods may, but are not required
     * to, throw an UnsupportedOperationException if the invocation would have
     * no effect on the collection. For example, invoking the addAll(Collection)
     * method on an unmodifiable collection may, but is not required to, throw
     * the exception if the collection to be added is empty.
     *
     * Many methods in Collections Framework interfaces are defined in terms of
     * the equals method. For example, the specification for the
     * contains(Object o) method says: "returns true if and only if this
     * collection contains at least one element e such that
     * (o==null ? e==null : o.equals(e))."
     *
     * @since 1.0
     */
    template< typename T >
    class Collection
    {
    public:

        virtual ~Collection() {}

        /**
         * Returns the number of elements in this collection. If this collection
         * contains more than Integer.MAX_VALUE elements, returns Integer.MAX_VALUE.
         * @returns the number of elements in this collection
         */
        virtual int size() = 0;

        /**
         * @returns true if this collection contains no elements.
         */
        virtual bool isEmpty() = 0;

        /**
         * Returns true if this collection contains the specified element. More
         * formally, returns true if and only if this collection contains at
         * least one element e such that (o==null ? e==null : o.equals(e)).
         * @param value - value to check for presence in the collection
         * @returns true if there is at least one of the elements in the collection
         * @thorws NullPointerException
         */
        virtual bool contains( const E& value )
            throw ( lang::exceptions::NullPointerException ) = 0;

        /**
         * Returns an iterator over the elements in this collection. There are
         * no guarantees concerning the order in which the elements are
         * returned (unless this collection is an instance of some class that
         * provides a guarantee).
         * @returns an Iterator over the elements in this collection
         */
        virtual Iterator<E>* iterator() = 0;

        /**
         * Returns an array containing all of the elements in this collection. If
         * the collection makes any guarantees as to what order its elements are
         * returned by its iterator, this method must return the elements in the
         * same order.
         *
         * This method acts as bridge between array-based and collection-based APIs.
         * @returns an array of the elements in this collection.
         */
        virtual std::vector<E> toArray() = 0;

        /**
         * Returns true if this collection changed as a result of the call.
         * (Returns false if this collection does not permit duplicates and
         * already contains the specified element.)
         *
         * Collections that support this operation may place limitations on
         * what elements may be added to this collection. In particular,
         * some collections will refuse to add null elements, and others
         * will impose restrictions on the type of elements that may be
         * added. Collection classes should clearly specify in their
         * documentation any restrictions on what elements may be added.
         *
         * If a collection refuses to add a particular element for any
         * reason other than that it already contains the element, it must
         * throw an exception (rather than returning false). This preserves
         * the invariant that a collection always contains the specified
         * element after this call returns.
         *
         * For non-pointer values, i.e. class instances or string's the object
         * will be copied into the collection, thus the object must support
         * being copied, must not hide the copy constructor and assignment
         * operator.
         *
         * @param value - reference to the element to add.
         * @returns true if the element was added
         * @throw UnsupportedOperationException
         * @throw NullPointerException
         * @throw IllegalArgumentException
         */
        virtual bool add( const E& value )
            throw ( lang::exceptions::UnsupportedOperationException,
                    lang::exceptions::NullPointerException,
                    lang::exceptions::IllegalArgumentException ) = 0;

        /**
         * Removes a single instance of the specified element from the
         * collection.  More formally, removes an element e such that
         * (o==null ? e==null : o.equals(e)), if this collection contains one
         * or more such elements. Returns true if this collection contained
         * the specified element (or equivalently, if this collection changed
         * as a result of the call).
         * @param value - reference to the element to remove.
         * @returns true if the collection was changed
         * @throw UnsupportedOperationException
         * @throw NullPointerException
         */
        virtual bool remove( const E& value )
            throw ( lang::exceptions::UnsupportedOperationException,
                    lang::exceptions::NullPointerException ) = 0;

        /**
         * Returns true if this collection contains all of the elements in
         * the specified collection.
         * @param source - Collection to compare to this one.
         * @thorw Exception
         */
        virtual bool containsAll( const Collection<E>& source )
            throw ( lang::Exception ) = 0;

        /**
         * Adds all of the elements in the specified collection to this
         * collection. The behavior of this operation is undefined if the
         * specified collection is modified while the operation is in progress.
         * (This implies that the behavior of this call is undefined if the
         * specified collection is this collection, and this collection is
         * nonempty.)
         * @param source - Collection whose elements are added to this one.
         * @return true if this collection changed as a result of the call
         * @throw UnsupportedOperationException
         * @throw NullPointerException
         * @throw IllegalArgumentException
         */
        virtual bool addAll( const Collection<E>& source )
            throw ( lang::exceptions::UnsupportedOperationException,
                    lang::exceptions::NullPointerException,
                    lang::exceptions::IllegalArgumentException ) = 0;

        /**
         * Removes all this collection's elements that are also contained in
         * the specified collection (optional operation). After this call returns,
         * this collection will contain no elements in common with the specified
         * collection.
         * @param value - The Collection whose elements are to be removed
         * @returns true if the collection changed as a result of this call
         * @throw UnsupportedOperationException
         * @throw NullPointerException
         * @throw IllegalArgumentException
         */
        virtual bool removeAll( const Collection<E>& value )
            throw ( lang::exceptions::UnsupportedOperationException,
                    lang::exceptions::NullPointerException,
                    lang::exceptions::IllegalArgumentException ) = 0;

        /**
         * Retains only the elements in this collection that are contained in the
         * specified collection (optional operation). In other words, removes from
         * this collection all of its elements that are not contained in the
         * specified collection.
         * @param value - The Collection whose elements are to be retained
         * @returns true if the collection changed as a result of this call
         * @throw UnsupportedOperationException
         * @throw NullPointerException
         * @throw IllegalArgumentException
         */
        virtual bool retainAll( const Collection<E> value )
            throw ( lang::exceptions::UnsupportedOperationException,
                    lang::exceptions::NullPointerException,
                    lang::exceptions::IllegalArgumentException ) = 0;

        /**
         * Removes all of the elements from this collection (optional operation).
         * This collection will be empty after this method returns unless it throws
         * an exception.
         * @throw UnsupportedOperationException
         */
        virtual void clear()
            throw ( lang::exceptions::UnsupportedOperationException ) = 0;

        /**
         * Compares the passed collection to this one, if they contain the
         * same elements, i.e. all their elements are equivalent, then it
         * returns true.
         * @returns true if the Collections contain the same elements.
         */
        virtual bool equals( const Collection<E>& value ) = 0;

    };

}}

#endif /*_DECAF_UTIL_COLLECTION_H_*/
