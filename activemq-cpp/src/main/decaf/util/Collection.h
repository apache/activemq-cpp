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
#include <decaf/lang/exceptions/UnsupportedOperationException.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>
#include <decaf/lang/Iterable.h>
#include <decaf/util/Iterator.h>
#include <decaf/util/concurrent/Synchronizable.h>

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
    template< typename E >
    class Collection : public virtual lang::Iterable<E>,
                       public virtual util::concurrent::Synchronizable {
    public:

        virtual ~Collection() {}

        /**
         * Renders this Collection as a Copy of the given Collection
         *
         * @param collection
         *      The collection to mirror.
         *
         * @throws UnsupportedOperationExceptio if this is an unmodifiable collection.
         * @throws IllegalStateException if the elements cannot be added at this time due
         *         to insertion restrictions.
         */
        virtual void copy( const Collection<E>& collection ) = 0;

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
         * @param value
         *      The reference to the element to add to this Collection.
         *
         * @returns true if the element was added to this Collection.
         *
         * @throws UnsupportedOperationExceptio if this is an unmodifiable collection.
         * @throws NullPointerException if the Collection is a container of pointers
         *         and does not allow NULL values.
         * @throws IllegalArgumentException if some property of the element prevents it
         *         from being added to this collection
         * @throws IllegalStateException if the element cannot be added at this time due
         *         to insertion restrictions.
         */
        virtual bool add( const E& value ) = 0;

        /**
         * Adds all of the elements in the specified collection to this
         * collection. The behavior of this operation is undefined if the
         * specified collection is modified while the operation is in progress.
         * (This implies that the behavior of this call is undefined if the
         * specified collection is this collection, and this collection is
         * nonempty.)
         *
         * @param collection
         *      The Collection whose elements are added to this one.
         *
         * @return true if this collection changed as a result of the call
         *
         * @throws UnsupportedOperationExceptio if this is an unmodifiable collection.
         * @throws NullPointerException if the Collection is a container of pointers
         *         and does not allow NULL values.
         * @throws IllegalArgumentException if some property of an element prevents it
         *         from being added to this collection
         * @throws IllegalStateException if an element cannot be added at this time due
         *         to insertion restrictions.
         */
        virtual bool addAll( const Collection<E>& collection ) = 0;

        /**
         * Removes all of the elements from this collection (optional operation).
         * This collection will be empty after this method returns unless it throws
         * an exception.
         *
         * @throws UnsupportedOperationExceptio if this is an unmodifiable collection.
         */
        virtual void clear() = 0;

        /**
         * Returns true if this collection contains the specified element. More
         * formally, returns true if and only if this collection contains at
         * least one element e such that (value == NULL ? e == NULL : value == e ).
         *
         * @param value
         *      The value to check for presence in the collection.
         *
         * @returns true if there is at least one of the elements in the collection
         *
         * @throws NullPointerException if the Collection contains pointers and the
         *         Collection does not allow for NULL elements (optional check).
         */
        virtual bool contains( const E& value ) const = 0;

        /**
         * Returns true if this collection contains all of the elements in
         * the specified collection.
         *
         * @param collection
         *      The Collection to compare to this one.
         *
         * @throws NullPointerException if the Collection contains pointers and the
         *         Collection does not allow for NULL elements (optional check).
         */
        virtual bool containsAll( const Collection<E>& collection ) const = 0;

        /**
         * Compares the passed collection to this one, if they contain the
         * same elements, i.e. all their elements are equivalent, then it
         * returns true.
         *
         * @returns true if the Collections contain the same elements.
         */
        virtual bool equals( const Collection<E>& value ) const = 0;

        /**
         * @returns true if this collection contains no elements.
         */
        virtual bool isEmpty() const = 0;

        /**
         * Removes a single instance of the specified element from the
         * collection.  More formally, removes an element e such that
         * (value == NULL ? e == NULL : value == e), if this collection contains one
         * or more such elements. Returns true if this collection contained
         * the specified element (or equivalently, if this collection changed
         * as a result of the call).
         *
         * @param value
         *      The reference to the element to remove from this Collection.
         *
         * @returns true if the collection was changed, false otherwise.
         *
         * @throws UnsupportedOperationExceptio if this is an unmodifiable collection.
         * @throws NullPointerException if the Collection is a container of pointers
         *         and does not allow NULL values.
         */
        virtual bool remove( const E& value ) = 0;

        /**
         * Removes all this collection's elements that are also contained in
         * the specified collection (optional operation). After this call returns,
         * this collection will contain no elements in common with the specified
         * collection.
         *
         * @param collection
         *      The Collection whose elements are to be removed from this one.
         *
         * @returns true if the collection changed as a result of this call.
         *
         * @throws UnsupportedOperationExceptio if this is an unmodifiable collection.
         * @throws NullPointerException if the Collection is a container of pointers
         *         and does not allow NULL values.
         */
        virtual bool removeAll( const Collection<E>& collection ) = 0;

        /**
         * Retains only the elements in this collection that are contained in the
         * specified collection (optional operation). In other words, removes from
         * this collection all of its elements that are not contained in the
         * specified collection.
         *
         * @param collection
         *      The Collection whose elements are to be retained.
         *
         * @returns true if the collection changed as a result of this call.
         *
         * @throws UnsupportedOperationExceptio if this is an unmodifiable collection.
         * @throws NullPointerException if the Collection is a container of pointers
         *         and does not allow NULL values.
         */
        virtual bool retainAll( const Collection<E>& collection ) = 0;

        /**
         * Returns the number of elements in this collection. If this collection
         * contains more than Integer::MAX_VALUE elements, returns Integer::MAX_VALUE.
         *
         * @returns the number of elements in this collection
         */
        virtual int size() const = 0;

        /**
         * Returns an array containing all of the elements in this collection. If
         * the collection makes any guarantees as to what order its elements are
         * returned by its iterator, this method must return the elements in the
         * same order.
         *
         * This method acts as bridge between array-based and collection-based APIs.
         *
         * @returns an array of the elements in this collection in the form of an
         *          STL vector.
         */
        virtual std::vector<E> toArray() const = 0;

    };

}}

#endif /*_DECAF_UTIL_COLLECTION_H_*/
