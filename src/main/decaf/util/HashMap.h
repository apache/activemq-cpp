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

#ifndef _DECAF_UTIL_HASHMAP_H_
#define _DECAF_UTIL_HASHMAP_H_

#include <decaf/util/Config.h>

#include <decaf/util/AbstractMap.h>
#include <decaf/util/HashCode.h>
#include <decaf/lang/ArrayPointer.h>

namespace decaf {
namespace util {

    /**
     * Hash table based implementation of the Map interface. This implementation provides all
     * of the optional map operations, and permits null values and the null key.  This class
     * makes no guarantees as to the order of the map; in particular, it does not guarantee
     * that the order will remain constant over time.
     *
     * This implementation provides constant-time performance for the basic operations (get
     * and put), assuming the hash function disperses the elements properly among the buckets.
     * Iteration over collection views requires time proportional to the "capacity" of the
     * HashMap instance (the number of buckets) plus its size (the number of key-value mappings).
     * Thus, it's very important not to set the initial capacity too high (or the load factor too
     * low) if iteration performance is important.
     *
     * An instance of HashMap has two parameters that affect its performance: initial capacity
     * and load factor. The capacity is the number of buckets in the hash table, and the initial
     * capacity is simply the capacity at the time the hash table is created. The load factor is
     * a measure of how full the hash table is allowed to get before its capacity is automatically
     * increased. When the number of entries in the hash table exceeds the product of the load
     * factor and the current capacity, the hash table is rehashed (that is, internal data
     * structures are rebuilt) so that the hash table has approximately twice the number of buckets.
     *
     * As a general rule, the default load factor (.75) offers a good tradeoff between time and
     * space costs. Higher values decrease the space overhead but increase the lookup cost
     * (reflected in most of the operations of the HashMap class, including get and put). The
     * expected number of entries in the map and its load factor should be taken into account
     * when setting its initial capacity, so as to minimize the number of rehash operations. If
     * the initial capacity is greater than the maximum number of entries divided by the load
     * factor, no rehash operations will ever occur.
     *
     * If many mappings are to be stored in a HashMap instance, creating it with a sufficiently
     * large capacity will allow the mappings to be stored more efficiently than letting it
     * perform automatic rehashing as needed to grow the table.
     *
     * Note that this implementation is not synchronized. If multiple threads access a hash map
     * concurrently, and at least one of the threads modifies the map structurally, it must be
     * synchronized externally. (A structural modification is any operation that adds or deletes
     * one or more mappings; merely changing the value associated with a key that an instance
     * already contains is not a structural modification.) This is typically accomplished by
     * synchronizing on some object that naturally encapsulates the map. If no such object
     * exists, the map should be "wrapped" using the Collections::synchronizedMap method.
     * This is best done at creation time, to prevent accidental unsynchronized access to the map:
     *
     *   Map<K, V>* map = Collections::synchronizedMap(new HashMap<K, V>());
     *
     * The iterators returned by all of this class's "collection view methods" are fail-fast:
     * if the map is structurally modified at any time after the iterator is created, in any
     * way except through the iterator's own remove method, the iterator will throw a
     * ConcurrentModificationException. Thus, in the face of concurrent modification, the
     * iterator fails quickly and cleanly, rather than risking arbitrary, non-deterministic
     * behavior at an undetermined time in the future.
     *
     * Note that the fail-fast behavior of an iterator cannot be guaranteed as it is, generally
     * speaking, impossible to make any hard guarantees in the presence of unsynchronized
     * concurrent modification. Fail-fast iterators throw ConcurrentModificationException on a
     * best-effort basis. Therefore, it would be wrong to write a program that depended on this
     * exception for its correctness: the fail-fast behavior of iterators should be used only
     * to detect bugs.
     *
     * @since 1.0
     */
    template<typename K, typename V, typename HASHCODE = HashCode<K> >
    class HashMap : public AbstractMap<K, V> {
    private:

        class HashMapEntry : public MapEntry<K, V> {
        public:

            int origKeyHash;

            HashMapEntry* next;

            HashMapEntry(const K& key, int hash) : MapEntry<K, V>(), origKeyHash(hash), next(NULL) {
                this->setKey(key);
                this->origKeyHash = hash;
            }

            HashMapEntry(const K& key, const V& value) : MapEntry<K, V>(key, value), origKeyHash(0), next(NULL){
                this->origKeyHash = HASHCODE()(key);
            }

        };

    private:

        /**
         * The Hash Code generator for this map's keys.
         */
        HASHCODE hashFunc;

        /*
         * Actual count of entries
         */
        int elementCount;

        /*
         * The internal data structure to hold Entries, Array of MapEntry pointers.
         */
        decaf::lang::ArrayPointer<HashMapEntry*> elementData;

        /*
         * modification count, to keep track of structural modifications between the
         * HashMap and the iterator
         */
        int modCount;

        /*
         * maximum ratio of (stored elements)/(storage size) which does not lead to rehash
         */
        float loadFactor;

        /*
         * maximum number of elements that can be put in this map before having to rehash
         */
        int threshold;

    private:

        void computeThreshold() {
            threshold = (int) (elementData.length() * loadFactor);
        }

        static int calculateCapacity(int x) {
            if (x >= 1 << 30) {
                return 1 << 30;
            }

            if (x == 0) {
                return 16;
            }
            x = x -1;
            x |= x >> 1;
            x |= x >> 2;
            x |= x >> 4;
            x |= x >> 8;
            x |= x >> 16;
            return x + 1;
        }

    public:

        /**
         * Creates a new empty HashMap with default configuration settings.
         */
        HashMap() : AbstractMap<K,V>(), hashFunc(), elementCount(0), elementData(), modCount(0), loadFactor(0.75), threshold(0) {
            int capacity = calculateCapacity(12);
            elementCount = 0;
            elementData.reset(NULL, capacity);
            computeThreshold();
        }

        /**
         * Constructs a new HashMap instance with the specified capacity.
         *
         * @param capacity
         * 		The initial capacity of this hash map.
         *
         * @throws IllegalArgumentException when the capacity is less than zero.
         */
        HashMap(int capacity) : AbstractMap<K,V>(), hashFunc(), elementCount(0), elementData(), modCount(0), loadFactor(0.75), threshold(0) {
            if (capacity >= 0) {
                capacity = calculateCapacity(capacity);
                elementCount = 0;
                elementData.reset(NULL, capacity);
                computeThreshold();
            } else {
                throw decaf::lang::exceptions::IllegalArgumentException(
                        __FILE__, __LINE__, "Invalid capacity configuration");
            }
        }

        /**
         * Constructs a new HashMap instance with the specified capacity.
         *
         * @param capacity
         * 		The initial capacity of this hash map.
         * @param loadFactor
         * 		The load factor to use for this hash map.
         *
         * @throws IllegalArgumentException when the capacity is less than zero.
         */
        HashMap(int capacity, float loadFactor) : AbstractMap<K,V>(), hashFunc(), elementCount(0), elementData(), modCount(0), loadFactor(0.75), threshold(0) {
            if (capacity >= 0 && loadFactor > 0) {
                capacity = calculateCapacity(capacity);
                elementCount = 0;
                elementData.reset(NULL, capacity);
                this->loadFactor = loadFactor;
                computeThreshold();
            } else {
                throw decaf::lang::exceptions::IllegalArgumentException(
                        __FILE__, __LINE__, "Invalid configuration");
            }
        }

        /**
         * Creates a new HashMap with default configuration settings and fills it with the contents
         * of the given source Map instance.
         *
         * @param map
         * 		The Map instance whose elements are copied into this HashMap instance.
         */
        HashMap(const Map<K,V>& map) : AbstractMap<K,V>(), hashFunc(), elementCount(0), elementData(), modCount(0), loadFactor(0.75), threshold(0) {
            int capacity = calculateCapacity(map.size());
            elementCount = 0;
            elementData.reset(NULL, capacity);
            computeThreshold();
            putAll(map);
        }

        virtual ~HashMap() {}

    public:

        virtual void clear() {
            if (elementCount > 0) {
                elementCount = 0;
                for (int i = 0; i < elementData.length(); ++i) {
                    delete elementData[i];
                    elementData[i] = NULL;
                }
                modCount++;
            }
        }

        virtual bool isEmpty() const {
            return elementCount == 0;
        }

        virtual int size() const {
            return elementCount;
        }

        virtual bool containsValue(const V& value) const {
            for (int i = 0; i < elementData.length(); i++) {
                HashMapEntry* entry = elementData[i];
                while (entry != NULL) {
                    if (value == entry->getValue()) {
                        return true;
                    }
                    entry = entry->next;
                }
            }
            return false;
        }

    protected:

        void rehash(int capacity) {
            int length = calculateCapacity((capacity == 0 ? 1 : capacity << 1));

            decaf::lang::ArrayPointer<HashMapEntry*> newData(length);
            for (int i = 0; i < elementData.length(); i++) {
                HashMapEntry* entry = elementData[i];
                elementData[i] = NULL;
                while (entry != NULL) {
                    int index = entry->origKeyHash & (length - 1);
                    HashMapEntry* next = entry->next;
                    entry->next = newData[index];
                    newData[index] = entry;
                    entry = next;
                }
            }
            elementData = newData;
            computeThreshold();
        }

        void rehash() {
            rehash(elementData.length());
        }

    };

}}

#endif /* _DECAF_UTIL_HASHMAP_H_ */
