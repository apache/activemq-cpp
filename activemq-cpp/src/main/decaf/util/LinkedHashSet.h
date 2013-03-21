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

#ifndef _DECAF_UTIL_LINKEDHASHSET_H_
#define _DECAF_UTIL_LINKEDHASHSET_H_

#include <decaf/util/Config.h>

#include <decaf/util/LinkedHashMap.h>
#include <decaf/util/HashSet.h>
#include <decaf/util/ConcurrentModificationException.h>
#include <decaf/lang/exceptions/UnsupportedOperationException.h>

namespace decaf {
namespace util {

    /**
     * Hash table and linked list implementation of the Set interface, with predictable iteration
     * order. This implementation differs from HashSet in that it maintains a doubly-linked list
     * running through all of its entries. This linked list defines the iteration ordering, which
     * is the order in which elements were inserted into the set (insertion-order). Note that
     * insertion order is not affected if an element is re-inserted into the set. (An element e
     * is reinserted into a set s if s.add(e) is invoked when s.contains(e) would return true
     * immediately prior to the invocation.)
     *
     * This implementation spares its clients from the unspecified, generally chaotic ordering
     * provided by HashSet, without incurring the increased cost associated with TreeSet. It
     * can be used to produce a copy of a set that has the same order as the original, regardless
     * of the original set's implementation:
     *
     *   void foo(const Set<E& s) {
     *       Set<E>* copy = new LinkedHashSet<E>(s);
     *       ...
     *   }
     *
     * This technique is particularly useful if a module takes a set on input, copies it, and
     * later returns results whose order is determined by that of the copy. (Clients generally
     * appreciate having things returned in the same order they were presented.)
     *
     * This class provides all of the optional Set operations, and permits null elements. Like
     * HashSet, it provides constant-time performance for the basic operations (add, contains
     * and remove), assuming the hash function disperses elements properly among the buckets.
     * Performance is likely to be just slightly below that of HashSet, due to the added expense
     * of maintaining the linked list, with one exception: Iteration over a LinkedHashSet requires
     * time proportional to the size of the set, regardless of its capacity. Iteration over a
     * HashSet is likely to be more expensive, requiring time proportional to its capacity.
     *
     * A linked hash set has two parameters that affect its performance: initial capacity and load
     * factor. They are defined precisely as for HashSet. Note, however, that the penalty for
     * choosing an excessively high value for initial capacity is less severe for this class than
     * for HashSet, as iteration times for this class are unaffected by capacity.
     *
     * Note that this implementation is not synchronized. If multiple threads access a linked hash
     * set concurrently, and at least one of the threads modifies the set, it must be synchronized
     * externally. This is typically accomplished by synchronizing on some object that naturally
     * encapsulates the set. If no such object exists, the set should be "wrapped" using the
     * Collections.synchronizedSet method. This is best done at creation time, to prevent accidental
     * unsynchronized access to the set:
     *
     *   Set<E>* s = Collections::synchronizedSet(new LinkedHashSet<E>(...));
     *
     * The iterators returned by this class's iterator method are fail-fast: if the set is modified
     * at any time after the iterator is created, in any way except through the iterator's own
     * remove method, the iterator will throw a ConcurrentModificationException. Thus, in the face
     * of concurrent modification, the iterator fails quickly and cleanly, rather than risking
     * arbitrary, non-deterministic behavior at an undetermined time in the future.
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
    class LinkedHashSet : public HashSet<E, HASHCODE> {
    public:

        /**
         * Constructs a new, empty set; the backing HashMap instance has default initial
         * capacity (16) and load factor (0.75).
         */
        LinkedHashSet() : HashSet<E, HASHCODE>(new LinkedHashMap<E, Set<E>*, HASHCODE>()) {
        }

        /**
         * Constructs a new, empty set; the backing HashMap instance has the specified initial
         * capacity and default load factor (0.75).
         *
         * @param capacity
         *      The initial capacity of this LinkedHashSet.
         */
        LinkedHashSet(int capacity) : HashSet<E, HASHCODE>(new LinkedHashMap<E, Set<E>*, HASHCODE>(capacity)) {
        }

        /**
         * Constructs a new instance of {@code HashSet} with the specified capacity
         * and load factor.
         *
         * @param capacity
         *      The initial capacity for this LinkedHashSet.
         * @param loadFactor
         *      The initial load factor for this LinkedHashSet.
         */
        LinkedHashSet(int capacity, float loadFactor) :
            HashSet<E, HASHCODE>(new LinkedHashMap<E, Set<E>*, HASHCODE>(capacity, loadFactor)) {
        }

        /**
         * Constructs a new set containing the elements in the specified collection.
         *
         * The HashMap is created with default load factor (0.75) and an initial capacity
         * sufficient to contain the elements in the specified collection.
         *
         * @param collection
         *      The collection of elements to add to this LinkedHashSet.
         */
        LinkedHashSet(const Collection<E>& collection) :
            HashSet<E, HASHCODE>(new LinkedHashMap<E, Set<E>*, HASHCODE>(
                (collection.size() < 6 ? 11 : collection.size() * 2))) {

            decaf::lang::Pointer<Iterator<E> > iter(collection.iterator());
            while (iter->hasNext()) {
                this->add(iter->next());
            }
        }

        virtual ~LinkedHashSet() {
        }

        virtual std::string toString() const {
            return "LinkedHashSet";
        }
    };

}}

#endif /* _DECAF_UTIL_LINKEDHASHSET_H_ */
