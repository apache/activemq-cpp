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

#ifndef _DECAF_UTIL_HASHSET_H_
#define _DECAF_UTIL_HASHSET_H_

#include <decaf/util/Config.h>

#include <decaf/util/AbstractSet.h>
#include <decaf/util/HashMap.h>
#include <decaf/util/HashCode.h>
#include <decaf/lang/Pointer.h>
#include <decaf/lang/Integer.h>
#include <decaf/util/ConcurrentModificationException.h>
#include <decaf/lang/exceptions/UnsupportedOperationException.h>

namespace decaf {
namespace util {

    /**
     * This class implements the Set interface, backed by a hash table (actually a HashMap instance).
     * It makes no guarantees as to the iteration order of the set; in particular, it does not
     * guarantee that the order will remain constant over time.
     *
     * This class offers constant time performance for the basic operations (add, remove, contains
     * and size), assuming the hash function disperses the elements properly among the buckets.
     * Iterating over this set requires time proportional to the sum of the HashSet instance's size
     * (the number of elements) plus the "capacity" of the backing HashMap instance (the number of
     * buckets). Thus, it's very important not to set the initial capacity too high (or the load
     * factor too low) if iteration performance is important.
     *
     * Note that this implementation is not synchronized. If multiple threads access a hash set
     * concurrently, and at least one of the threads modifies the set, it must be synchronized
     * externally. This is typically accomplished by synchronizing on some object that naturally
     * encapsulates the set. If no such object exists, the set should be "wrapped" using the
     * Collections::synchronizedSet method. This is best done at creation time, to prevent
     * accidental unsynchronized access to the set:
     *
     *   Set<E>* s = Collections::synchronizedSet(new HashSet<E>(...));
     *
     * The iterators returned by this class's iterator method are fail-fast: if the set is modified
     * at any time after the iterator is created, in any way except through the iterator's own
     * remove method, the Iterator throws a ConcurrentModificationException. Thus, in the face of
     * concurrent modification, the iterator fails quickly and cleanly, rather than risking arbitrary,
     * non-deterministic behavior at an undetermined time in the future.
     *
     * Note that the fail-fast behavior of an iterator cannot be guaranteed as it is, generally
     * speaking, impossible to make any hard guarantees in the presence of unsynchronized concurrent
     * modification. Fail-fast iterators throw ConcurrentModificationException on a best-effort basis.
     * Therefore, it would be wrong to write a program that depended on this exception for its
     * correctness: the fail-fast behavior of iterators should be used only to detect bugs.
     *
     * @since 1.0
     */
    template<typename E, typename HASHCODE = HashCode<E> >
    class HashSet : public AbstractSet<E> {
    protected:

        HashMap<E, Set<E>*, HASHCODE>* backingMap;

    public:

        /**
         * Constructs a new, empty set; the backing HashMap instance has default initial
         * capacity (16) and load factor (0.75).
         */
        HashSet() : AbstractSet<E>(), backingMap(new HashMap<E, Set<E>*, HASHCODE>()) {
        }

        /**
         * Constructs a new, empty set; the backing HashMap instance has the specified initial
         * capacity and default load factor (0.75).
         *
         * @param capacity
         *      The initial capacity of this HashSet.
         */
        HashSet(int capacity) : AbstractSet<E>(), backingMap(new HashMap<E, Set<E>*, HASHCODE>(capacity)) {
        }

        /**
         * Constructs a new instance of {@code HashSet} with the specified capacity
         * and load factor.
         *
         * @param capacity
         *      The initial capacity for this HashSet.
         * @param loadFactor
         *      The initial load factor for this HashSet.
         */
        HashSet(int capacity, float loadFactor) :
            AbstractSet<E>(), backingMap(new HashMap<E, Set<E>*, HASHCODE>(capacity, loadFactor)) {
        }

        /**
         * Constructs a new set containing the elements in the specified collection.
         *
         * The HashMap is created with default load factor (0.75) and an initial capacity
         * sufficient to contain the elements in the specified collection.
         *
         * @param collection
         *      The collection of elements to add to this HashSet.
         */
        HashSet(const Collection<E>& collection) : AbstractSet<E>(), backingMap() {

            this->backingMap = new HashMap<E, Set<E>*, HASHCODE>(
                (collection.size() < 6 ? 11 : collection.size() * 2));

            decaf::lang::Pointer<Iterator<E> > iter(collection.iterator());
            while (iter->hasNext()) {
                this->add(iter->next());
            }
        }

        virtual ~HashSet() {
            try {
                delete this->backingMap;
            }
            DECAF_CATCHALL_NOTHROW()
        }

    protected:

        /**
         * Protected constructor for use by subclasses that wish to use an alternate type
         * of backing Map.
         *
         * @param backingMap
         *      The instance of the Map type used to back this HashSet.
         */
        HashSet(HashMap<E, Set<E>*, HASHCODE>* backingMap) :
            AbstractSet<E>(), backingMap(backingMap) {
        }

    public:

        HashSet<E>& operator= (const Collection<E>& collection) {
            this->clear();
            this->addAll(collection);
            return *this;
        }

    public:

        /**
         * Adds the specified element to this set if it is not already present. More formally,
         * adds the specified element e to this set if this set contains no element e2 such
         * that (e == e2). If this set already contains the element, the call leaves the set
         * unchanged and returns false.
         *
         * @param object
         *      The object to add.
         *
         * @return true when this HashSet did not already contain the object,false otherwise.
         */
        virtual bool add(const E& value) {
            return this->backingMap->put(value, this);
        }

        /**
         * Removes all elements from this {@code HashSet}, leaving it empty.
         *
         * @see #isEmpty
         * @see #size
         */
        virtual void clear() {
            this->backingMap->clear();
        }

        /**
         * Returns a new {@code HashSet} with the same elements and size as this
         * {@code HashSet}.
         *
         * @return a shallow copy of this {@code HashSet}.
         * @see java.lang.Cloneable
         */
//        virtual Object clone() {
//            try {
//                HashSet<E> clone = (HashSet<E>) super.clone();
//                clone.backingMap = (HashMap<E, HashSet<E>>) backingMap.clone();
//                return clone;
//            } catch (CloneNotSupportedException e) {
//                return null;
//            }
//        }

        /**
         * Searches this {@code HashSet} for the specified object.
         *
         * @param object
         *            the object to search for.
         * @return {@code true} if {@code object} is an element of this
         *         {@code HashSet}, {@code false} otherwise.
         */
        virtual bool contains(const E& value) const {
            return this->backingMap->containsKey(value);
        }

        /**
         * Returns true if this {@code HashSet} has no elements, false otherwise.
         *
         * @return {@code true} if this {@code HashSet} has no elements,
         *         {@code false} otherwise.
         * @see #size
         */
        virtual bool isEmpty() const {
            return this->backingMap->isEmpty();
        }

        /**
         * Returns an Iterator on the elements of this {@code HashSet}.
         *
         * @return an Iterator on the elements of this {@code HashSet}.
         * @see Iterator
         */
        virtual Iterator<E>* iterator() {
            return this->backingMap->keySet().iterator();
        }

        virtual Iterator<E>* iterator() const {
            return this->backingMap->keySet().iterator();
        }

        /**
         * Removes the specified element from this set if it is present. More formally,
         * removes an element e such that (e == value), if this set contains such an element.
         * Returns true if this set contained the element (or equivalently, if this set
         * changed as a result of the call). (This set will not contain the element once
         * the call returns.)
         *
         * @param value
         *      The value to remove from this set.
         *
         * @return true if the value was removed, false otherwise.
         */
        virtual bool remove(const E& value) {
            try {
                this->backingMap->remove(value);
            } catch(decaf::util::NoSuchElementException& ex) {
                return false;
            }

            return true;
        }

        /**
         * Returns the number of elements in this {@code HashSet}.
         *
         * @return the number of elements in this {@code HashSet}.
         */
        virtual int size() const {
            return this->backingMap->size();
        }

        virtual std::string toString() const {

            std::string result;

            result.append("decaf::util::HashSet [  size = ");
            result.append(decaf::lang::Integer::toString(this->size()));
            result.append(" ]");

            return result;
        }

    };

}}

#endif /* _DECAF_UTIL_HASHSET_H_ */
