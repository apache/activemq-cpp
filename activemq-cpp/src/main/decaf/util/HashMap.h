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
#include <decaf/util/AbstractSet.h>
#include <decaf/util/HashCode.h>
#include <decaf/util/ConcurrentModificationException.h>
#include <decaf/lang/exceptions/UnsupportedOperationException.h>
#include <decaf/lang/Pointer.h>
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
        private:

            HashMapEntry(const HashMapEntry&);
            HashMapEntry& operator= (const HashMapEntry&);

        public:

            int origKeyHash;

            HashMapEntry* next;

            HashMapEntry(const K& key, const V& value, int hash) : MapEntry<K, V>(), origKeyHash(hash), next(NULL) {
                this->setKey(key);
                this->setValue(value);
                this->origKeyHash = hash;
            }

            HashMapEntry(const K& key, const V& value) : MapEntry<K, V>(key, value), origKeyHash(0), next(NULL){
                this->origKeyHash = HASHCODE()(key);
            }

        };

    private:

        class AbstractMapIterator {
        protected:

            mutable int position;
            int expectedModCount;
            HashMapEntry* futureEntry;
            HashMapEntry* currentEntry;
            HashMapEntry* prevEntry;

            HashMap* associatedMap;

        private:

            AbstractMapIterator(const AbstractMapIterator&);
            AbstractMapIterator& operator= (const AbstractMapIterator&);

        public:

            AbstractMapIterator(HashMap* parent) : position(0),
                                                   expectedModCount(parent->modCount),
                                                   futureEntry(NULL),
                                                   currentEntry(NULL),
                                                   prevEntry(NULL),
                                                   associatedMap(parent) {
            }

            virtual ~AbstractMapIterator() {}

            virtual bool checkHasNext() const {
                if (futureEntry != NULL) {
                    return true;
                }
                while (position < associatedMap->elementData.length()) {
                    if (associatedMap->elementData[position] == NULL) {
                        position++;
                    } else {
                        return true;
                    }
                }
                return false;
            }

            void checkConcurrentMod() const {
                if (expectedModCount != associatedMap->modCount) {
                    throw ConcurrentModificationException(
                        __FILE__, __LINE__, "HashMap modified outside this iterator");
                }
            }

            void makeNext() {
                checkConcurrentMod();

                if (!checkHasNext()) {
                    throw NoSuchElementException(__FILE__, __LINE__, "No next element");
                }

                if (futureEntry == NULL) {
                    currentEntry = associatedMap->elementData[position++];
                    futureEntry = currentEntry->next;
                    prevEntry = NULL;
                } else {
                    if (currentEntry != NULL){
                        prevEntry = currentEntry;
                    }
                    currentEntry = futureEntry;
                    futureEntry = futureEntry->next;
                }
            }

            virtual void doRemove() {

                checkConcurrentMod();

                if (currentEntry == NULL) {
                    throw decaf::lang::exceptions::IllegalStateException(
                        __FILE__, __LINE__, "Remove called before call to next()");
                }

                if (prevEntry == NULL){
                    int index = currentEntry->origKeyHash & (associatedMap->elementData.length() - 1);
                    associatedMap->elementData[index] = associatedMap->elementData[index]->next;
                } else {
                    prevEntry->next = currentEntry->next;
                }

                delete currentEntry;
                currentEntry = NULL;

                expectedModCount++;
                associatedMap->modCount++;
                associatedMap->elementCount--;
            }
        };

        class EntryIterator : public Iterator< MapEntry<K,V> >, public AbstractMapIterator {
        private:

            EntryIterator(const EntryIterator&);
            EntryIterator& operator= (const EntryIterator&);

        public:

            EntryIterator(HashMap* parent) : AbstractMapIterator(parent) {
            }

            virtual ~EntryIterator() {}

            virtual bool hasNext() const {
                return this->checkHasNext();
            }

            virtual MapEntry<K, V> next() {
                this->makeNext();
                return *(this->currentEntry);
            }

            virtual void remove() {
                this->doRemove();
            }
        };

        class KeyIterator : public Iterator<K>, public AbstractMapIterator {
        private:

            KeyIterator(const KeyIterator&);
            KeyIterator& operator= (const KeyIterator&);

        public:

            KeyIterator(HashMap* parent) : AbstractMapIterator(parent) {
            }

            virtual ~KeyIterator() {}

            virtual bool hasNext() const {
                return this->checkHasNext();
            }

            virtual K next() {
                this->makeNext();
                return this->currentEntry->getKey();
            }

            virtual void remove() {
                this->doRemove();
            }
        };

        class ValueIterator : public Iterator<V>, public AbstractMapIterator {
        private:

            ValueIterator(const ValueIterator&);
            ValueIterator& operator= (const ValueIterator&);

        public:

            ValueIterator(HashMap* parent) : AbstractMapIterator(parent) {
            }

            virtual ~ValueIterator() {}

            virtual bool hasNext() const {
                return this->checkHasNext();
            }

            virtual V next() {
                this->makeNext();
                return this->currentEntry->getValue();
            }

            virtual void remove() {
                this->doRemove();
            }
        };

    private:

        class ConstAbstractMapIterator {
        protected:

            mutable int position;
            int expectedModCount;
            const HashMapEntry* futureEntry;
            const HashMapEntry* currentEntry;
            const HashMapEntry* prevEntry;

            const HashMap* associatedMap;

        private:

            ConstAbstractMapIterator(const ConstAbstractMapIterator&);
            ConstAbstractMapIterator& operator= (const ConstAbstractMapIterator&);

        public:

            ConstAbstractMapIterator(const HashMap* parent) : position(0),
                                                              expectedModCount(parent->modCount),
                                                              futureEntry(NULL),
                                                              currentEntry(NULL),
                                                              prevEntry(NULL),
                                                              associatedMap(parent) {
            }

            virtual ~ConstAbstractMapIterator() {}

            virtual bool checkHasNext() const {
                if (futureEntry != NULL) {
                    return true;
                }
                while (position < associatedMap->elementData.length()) {
                    if (associatedMap->elementData[position] == NULL) {
                        position++;
                    } else {
                        return true;
                    }
                }
                return false;
            }

            void checkConcurrentMod() const {
                if (expectedModCount != associatedMap->modCount) {
                    throw ConcurrentModificationException(
                        __FILE__, __LINE__, "HashMap modified outside this iterator");
                }
            }

            void makeNext() {
                checkConcurrentMod();

                if (!checkHasNext()) {
                    throw NoSuchElementException(__FILE__, __LINE__, "No next element");
                }

                if (futureEntry == NULL) {
                    currentEntry = associatedMap->elementData[position++];
                    futureEntry = currentEntry->next;
                    prevEntry = NULL;
                } else {
                    if (currentEntry != NULL){
                        prevEntry = currentEntry;
                    }
                    currentEntry = futureEntry;
                    futureEntry = futureEntry->next;
                }
            }
        };

        class ConstEntryIterator : public Iterator< MapEntry<K,V> >, public ConstAbstractMapIterator {
        private:

            ConstEntryIterator(const ConstEntryIterator&);
            ConstEntryIterator& operator= (const ConstEntryIterator&);

        public:

            ConstEntryIterator(const HashMap* parent) : ConstAbstractMapIterator(parent) {
            }

            virtual ~ConstEntryIterator() {}

            virtual bool hasNext() const {
                return this->checkHasNext();
            }

            virtual MapEntry<K, V> next() {
                this->makeNext();
                return *(this->currentEntry);
            }

            virtual void remove() {
                throw lang::exceptions::UnsupportedOperationException(
                    __FILE__, __LINE__, "Cannot write to a const Iterator." );
            }
        };

        class ConstKeyIterator : public Iterator<K>, public ConstAbstractMapIterator {
        private:

            ConstKeyIterator(const ConstKeyIterator&);
            ConstKeyIterator& operator= (const ConstKeyIterator&);

        public:

            ConstKeyIterator(const HashMap* parent) : ConstAbstractMapIterator(parent) {
            }

            virtual ~ConstKeyIterator() {}

            virtual bool hasNext() const {
                return this->checkHasNext();
            }

            virtual K next() {
                this->makeNext();
                return this->currentEntry->getKey();
            }

            virtual void remove() {
                throw lang::exceptions::UnsupportedOperationException(
                    __FILE__, __LINE__, "Cannot write to a const Iterator." );
            }
        };

        class ConstValueIterator : public Iterator<V>, public ConstAbstractMapIterator {
        private:

            ConstValueIterator(const ConstValueIterator&);
            ConstValueIterator& operator= (const ConstValueIterator&);

        public:

            ConstValueIterator(const HashMap* parent) : ConstAbstractMapIterator(parent) {
            }

            virtual ~ConstValueIterator() {}

            virtual bool hasNext() const {
                return this->checkHasNext();
            }

            virtual V next() {
                this->makeNext();
                return this->currentEntry->getValue();
            }

            virtual void remove() {
                throw lang::exceptions::UnsupportedOperationException(
                    __FILE__, __LINE__, "Cannot write to a const Iterator." );
            }
        };

    private:

        // Special Set implementation that is backed by this HashMap
        class HashMapEntrySet : public AbstractSet< MapEntry<K, V> > {
        private:

            HashMap* associatedMap;

        private:

            HashMapEntrySet(const HashMapEntrySet&);
            HashMapEntrySet& operator= (const HashMapEntrySet&);

        public:

            HashMapEntrySet(HashMap* parent) : AbstractSet< MapEntry<K,V> >(), associatedMap(parent) {
            }

            virtual ~HashMapEntrySet() {}

            virtual int size() const {
                return associatedMap->elementCount;
            }

            virtual void clear() {
                associatedMap->clear();
            }

            virtual bool remove(const MapEntry<K,V>& entry) {
                HashMapEntry* result = associatedMap->getEntry(entry.getKey());
                if (result != NULL && entry.getValue() == result->getValue()) {
                    associatedMap->removeEntry(result);
                    return true;
                }

                return false;
            }

            virtual bool contains(const MapEntry<K,V>& entry) {
                HashMapEntry* result = associatedMap->getEntry(entry.getKey());
                if (result != NULL && entry.getValue() == result->getValue()) {
                    return true;
                }
                return false;
            }

            virtual Iterator< MapEntry<K, V> >* iterator() {
                return new EntryIterator(associatedMap);
            }

            virtual Iterator< MapEntry<K, V> >* iterator() const {
                return new ConstEntryIterator(associatedMap);
            }
        };

        // Special Set implementation that is backed by this HashMap
        class ConstHashMapEntrySet : public AbstractSet< MapEntry<K, V> > {
        private:

            const HashMap* associatedMap;

        private:

            ConstHashMapEntrySet(const ConstHashMapEntrySet&);
            ConstHashMapEntrySet& operator= (const ConstHashMapEntrySet&);

        public:

            ConstHashMapEntrySet(const HashMap* parent) : AbstractSet< MapEntry<K,V> >(), associatedMap(parent) {
            }

            virtual ~ConstHashMapEntrySet() {}

            virtual int size() const {
                return associatedMap->elementCount;
            }

            virtual void clear() {
                throw decaf::lang::exceptions::UnsupportedOperationException(
                        __FILE__, __LINE__, "Can't clear a const collection");
            }

            virtual bool remove(const MapEntry<K,V>& entry) {
                throw decaf::lang::exceptions::UnsupportedOperationException(
                        __FILE__, __LINE__, "Can't remove from const collection");
            }

            virtual bool contains(const MapEntry<K,V>& entry) {
                HashMapEntry* result = associatedMap->getEntry(entry.getKey());
                if (result != NULL && entry.getValue() == result->getValue()) {
                    return true;
                }
                return false;
            }

            virtual Iterator< MapEntry<K, V> >* iterator() {
                throw decaf::lang::exceptions::UnsupportedOperationException(
                        __FILE__, __LINE__, "Can't return a non-const iterator for a const collection");
            }

            virtual Iterator< MapEntry<K, V> >* iterator() const {
                return new ConstEntryIterator(associatedMap);
            }
        };

    private:

        class HashMapKeySet : public AbstractSet<K> {
        private:

            HashMap* associatedMap;

        private:

            HashMapKeySet(const HashMapKeySet&);
            HashMapKeySet& operator= (const HashMapKeySet&);

        public:

            HashMapKeySet(HashMap* parent) : AbstractSet<K>(), associatedMap(parent) {
            }

            virtual ~HashMapKeySet() {}

            virtual bool contains(const K& key) const {
                return this->associatedMap->containsKey(key);
            }

            virtual int size() const {
                return this->associatedMap->size();
            }

            virtual void clear() {
                this->associatedMap->clear();
            }

            virtual bool remove(const K& key) {
                HashMapEntry* entry = this->associatedMap->removeEntry(key);
                if (entry != NULL) {
                    delete entry;
                    return true;
                }
                return false;
            }

            virtual Iterator<K>* iterator() {
                return new KeyIterator(this->associatedMap);
            }

            virtual Iterator<K>* iterator() const {
                return new ConstKeyIterator(this->associatedMap);
            }
        };

        class ConstHashMapKeySet : public AbstractSet<K> {
        private:

            const HashMap* associatedMap;

        private:

            ConstHashMapKeySet(const ConstHashMapKeySet&);
            ConstHashMapKeySet& operator= (const ConstHashMapKeySet&);

        public:

            ConstHashMapKeySet(const HashMap* parent) : AbstractSet<K>(), associatedMap(parent) {
            }

            virtual ~ConstHashMapKeySet() {}

            virtual bool contains(const K& key) const {
                return this->associatedMap->containsKey(key);
            }

            virtual int size() const {
                return this->associatedMap->size();
            }

            virtual void clear() {
                throw decaf::lang::exceptions::UnsupportedOperationException(
                        __FILE__, __LINE__, "Can't modify a const collection");
            }

            virtual bool remove(const K& key) {
                throw decaf::lang::exceptions::UnsupportedOperationException(
                        __FILE__, __LINE__, "Can't modify a const collection");
            }

            virtual Iterator<K>* iterator() {
                throw decaf::lang::exceptions::UnsupportedOperationException(
                        __FILE__, __LINE__, "Can't return a non-const iterator for a const collection");
            }

            virtual Iterator<K>* iterator() const {
                return new ConstKeyIterator(this->associatedMap);
            }
        };

    private:

        class HashMapValueCollection : public AbstractCollection<V> {
        private:

            HashMap* associatedMap;

        private:

            HashMapValueCollection(const HashMapValueCollection&);
            HashMapValueCollection& operator= (const HashMapValueCollection&);

        public:

            HashMapValueCollection(HashMap* parent) : AbstractCollection<V>(), associatedMap(parent) {
            }

            virtual ~HashMapValueCollection() {}

            virtual bool contains(const V& value) const {
                return this->associatedMap->containsValue(value);
            }

            virtual int size() const {
                return this->associatedMap->size();
            }

            virtual void clear() {
                this->associatedMap->clear();
            }

            virtual Iterator<V>* iterator() {
                return new ValueIterator(this->associatedMap);
            }

            virtual Iterator<V>* iterator() const {
                return new ConstValueIterator(this->associatedMap);
            }
        };

        class ConstHashMapValueCollection : public AbstractCollection<V> {
        private:

            const HashMap* associatedMap;

        private:

            ConstHashMapValueCollection(const ConstHashMapValueCollection&);
            ConstHashMapValueCollection& operator= (const ConstHashMapValueCollection&);

        public:

            ConstHashMapValueCollection(const HashMap* parent) : AbstractCollection<V>(), associatedMap(parent) {
            }

            virtual ~ConstHashMapValueCollection() {}

            virtual bool contains(const V& value) const {
                return this->associatedMap->containsValue(value);
            }

            virtual int size() const {
                return this->associatedMap->size();
            }

            virtual void clear() {
                throw decaf::lang::exceptions::UnsupportedOperationException(
                        __FILE__, __LINE__, "Can't modify a const collection");
            }

            virtual Iterator<V>* iterator() {
                throw decaf::lang::exceptions::UnsupportedOperationException(
                        __FILE__, __LINE__, "Can't return a non-const iterator for a const collection");
            }

            virtual Iterator<V>* iterator() const {
                return new ConstValueIterator(this->associatedMap);
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

        // Cached values that are only initialized once a request for them is made.
        decaf::lang::Pointer<HashMapEntrySet> cachedEntrySet;
        decaf::lang::Pointer<HashMapKeySet> cachedKeySet;
        decaf::lang::Pointer<HashMapValueCollection> cachedValueCollection;

        // Cached values that are only initialized once a request for them is made.
        mutable decaf::lang::Pointer<ConstHashMapEntrySet> cachedConstEntrySet;
        mutable decaf::lang::Pointer<ConstHashMapKeySet> cachedConstKeySet;
        mutable decaf::lang::Pointer<ConstHashMapValueCollection> cachedConstValueCollection;

    private:

        void computeThreshold() {
            threshold = (int) ((float) elementData.length() * loadFactor);
        }

        static int calculateCapacity(int x) {
            if (x >= 1 << 30) {
                return 1 << 30;
            }

            if (x == 0) {
                return 16;
            }
            x = x - 1;
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
        HashMap() : AbstractMap<K,V>(), hashFunc(), elementCount(0), elementData(),
                    modCount(0), loadFactor(0.75), threshold(0),
                    cachedEntrySet(), cachedKeySet(), cachedValueCollection(),
                    cachedConstEntrySet(), cachedConstKeySet(), cachedConstValueCollection() {
            int capacity = calculateCapacity(12);
            elementCount = 0;
            elementData = decaf::lang::ArrayPointer<HashMapEntry*>(capacity);
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
        HashMap(int capacity) : AbstractMap<K,V>(), hashFunc(), elementCount(0),
                                elementData(), modCount(0), loadFactor(0.75), threshold(0),
                                cachedEntrySet(), cachedKeySet(), cachedValueCollection(),
                                cachedConstEntrySet(), cachedConstKeySet(), cachedConstValueCollection() {
            if (capacity >= 0) {
                capacity = calculateCapacity(capacity);
                elementCount = 0;
                elementData = decaf::lang::ArrayPointer<HashMapEntry*>(capacity);
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
        HashMap(int capacity, float loadFactor) : AbstractMap<K,V>(), hashFunc(), elementCount(0),
                                                  elementData(), modCount(0), loadFactor(0.75), threshold(0),
                                                  cachedEntrySet(), cachedKeySet(), cachedValueCollection(),
                                                  cachedConstEntrySet(), cachedConstKeySet(), cachedConstValueCollection() {
            if (capacity >= 0 && loadFactor > 0) {
                capacity = calculateCapacity(capacity);
                elementCount = 0;
                elementData = decaf::lang::ArrayPointer<HashMapEntry*>(capacity);
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
         *      The Map instance whose elements are copied into this HashMap instance.
         */
        HashMap(const HashMap<K,V>& map) : AbstractMap<K,V>(), hashFunc(), elementCount(0), elementData(),
                                           modCount(0), loadFactor(0.75), threshold(0),
                                           cachedEntrySet(), cachedKeySet(), cachedValueCollection(),
                                           cachedConstEntrySet(), cachedConstKeySet(), cachedConstValueCollection() {
            int capacity = calculateCapacity(map.size());
            elementCount = 0;
            elementData = decaf::lang::ArrayPointer<HashMapEntry*>(capacity);
            computeThreshold();
            putAll(map);
        }

        /**
         * Creates a new HashMap with default configuration settings and fills it with the contents
         * of the given source Map instance.
         *
         * @param map
         * 		The Map instance whose elements are copied into this HashMap instance.
         */
        HashMap(const Map<K,V>& map) : AbstractMap<K,V>(), hashFunc(), elementCount(0), elementData(),
                                       modCount(0), loadFactor(0.75), threshold(0),
                                       cachedEntrySet(), cachedKeySet(), cachedValueCollection(),
                                       cachedConstEntrySet(), cachedConstKeySet(), cachedConstValueCollection() {
            int capacity = calculateCapacity(map.size());
            elementCount = 0;
            elementData = decaf::lang::ArrayPointer<HashMapEntry*>(capacity);
            computeThreshold();
            putAll(map);
        }

        virtual ~HashMap() {
            for (int i = 0; i < elementData.length(); i++) {
                HashMapEntry* entry = elementData[i];
                while (entry != NULL) {
                    HashMapEntry* temp = entry;
                    entry = entry->next;
                    delete temp;
                }
            }
        }

    public:

        virtual void clear() {
            if (elementCount > 0) {
                elementCount = 0;
                for (int i = 0; i < elementData.length(); ++i) {
                    HashMapEntry* entry = elementData[i];
                    elementData[i] = NULL;
                    while (entry != NULL) {
                        HashMapEntry* temp = entry;
                        entry = entry->next;
                        delete temp;
                    }
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

        virtual bool containsKey(const K& key) const {
            const HashMapEntry* entry = getEntry(key);
            return entry != NULL;
        }

        virtual bool containsValue(const V& value) const {
            for (int i = 0; i < elementData.length(); i++) {
                const HashMapEntry* entry = elementData[i];
                while (entry != NULL) {
                    if (value == entry->getValue()) {
                        return true;
                    }
                    entry = entry->next;
                }
            }
            return false;
        }

        virtual V& get(const K& key) {
            HashMapEntry* entry = getEntry(key);
            if (entry != NULL) {
                return entry->getValue();
            }

            throw NoSuchElementException(
                    __FILE__, __LINE__, "The specified key is not present in the Map");
        }

        virtual const V& get(const K& key) const {
            const HashMapEntry* entry = getEntry(key);
            if (entry != NULL) {
                return entry->getValue();
            }

            throw NoSuchElementException(
                    __FILE__, __LINE__, "The specified key is not present in the Map");
        }

        virtual bool put(const K& key, const V& value) {
            return this->putImpl(key, value);
        }

        virtual bool put(const K& key, const V& value, V& oldValue) {
            return this->putImpl(key, value, oldValue);
        }

        virtual void putAll(const Map<K, V>& map) {
            if (!map.isEmpty()) {
                putAllImpl(map);
            }
        }

        virtual V remove(const K& key) {
            HashMapEntry* entry = removeEntry(key);
            if (entry != NULL) {
                V oldValue = entry->getValue();
                delete entry;
                return oldValue;
            }

            throw NoSuchElementException(
                __FILE__, __LINE__, "Specified key not present in the Map.");
        }

        virtual Set< MapEntry<K,V> >& entrySet() {
            if (this->cachedEntrySet == NULL) {
                this->cachedEntrySet.reset(new HashMapEntrySet(this));
            }
            return *(this->cachedEntrySet);
        }

        virtual const Set< MapEntry<K,V> >& entrySet() const {
            if (this->cachedConstEntrySet == NULL) {
                this->cachedConstEntrySet.reset(new ConstHashMapEntrySet(this));
            }
            return *(this->cachedConstEntrySet);
        }

        virtual Set<K>& keySet() {
            if (this->cachedKeySet == NULL) {
                this->cachedKeySet.reset(new HashMapKeySet(this));
            }
            return *(this->cachedKeySet);
        }

        virtual const Set<K>& keySet() const {
            if (this->cachedConstKeySet == NULL) {
                this->cachedConstKeySet.reset(new ConstHashMapKeySet(this));
            }
            return *(this->cachedConstKeySet);
        }

        virtual Collection<V>& values() {
            if (this->cachedValueCollection == NULL) {
                this->cachedValueCollection.reset(new HashMapValueCollection(this));
            }
            return *(this->cachedValueCollection);
        }

        virtual const Collection<V>& values() const {
            if (this->cachedConstValueCollection == NULL) {
                this->cachedConstValueCollection.reset(new ConstHashMapValueCollection(this));
            }
            return *(this->cachedConstValueCollection);
        }

        virtual bool equals(const Map<K, V>& source) const {
            return false;
        }

        virtual void copy(const Map<K, V>& source) {
            int capacity = calculateCapacity(source.size());
            this->clear();
            if (capacity > elementData.length()) {
                elementData = decaf::lang::ArrayPointer<HashMapEntry*>(capacity);
            }
            computeThreshold();
            putAll(source);
        }

        virtual std::string toString() const {
            return "HashMap";
        }

    protected:

        HashMapEntry* getEntry(const K& key) const {
            HashMapEntry* result = NULL;

            int hash = hashFunc(key);
            int index = hash & (elementData.length() - 1);
            result = findKeyEntry(key, index, hash);

            return result;
        }

        bool putImpl(const K& key, const V& value, V& oldValue) {
            bool replaced = true;
            HashMapEntry* entry = NULL;

            int hash = hashFunc(key);
            int index = hash & (elementData.length() - 1);

            entry = findKeyEntry(key, index, hash);

            if (entry == NULL) {
                modCount++;
                entry = createHashedEntry(key, index, hash);
                if (++elementCount > threshold) {
                    rehash();
                }
                replaced = false;
            } else {
                oldValue = entry->getValue();
            }

            entry->setValue(value);

            return replaced;
        }

        bool putImpl(const K& key, const V& value) {

            bool replaced = true;
            HashMapEntry* entry = NULL;

            int hash = hashFunc(key);
            int index = hash & (elementData.length() - 1);

            entry = findKeyEntry(key, index, hash);

            if (entry == NULL) {
                modCount++;
                entry = createHashedEntry(key, index, hash);
                if (++elementCount > threshold) {
                    rehash();
                }
                replaced = false;
            }

            entry->setValue(value);

            return replaced;
        }

        void putAllImpl(const Map<K, V>& map) {
            int capacity = elementCount + map.size();
            if (capacity > threshold) {
                rehash(capacity);
            }

            decaf::lang::Pointer<Iterator< MapEntry<K,V> > > iterator(map.entrySet().iterator());
            while (iterator->hasNext()) {
                MapEntry<K, V> entry = iterator->next();
                this->putImpl(entry.getKey(), entry.getValue());
            }
        }

        HashMapEntry* findKeyEntry(const K& key, int index, int keyHash) const {
            HashMapEntry* entry = elementData[index];
            while (entry != NULL && (entry->origKeyHash != keyHash || !(key == entry->getKey()))) {
                entry = entry->next;
            }
            return entry;
        }

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

        HashMapEntry* createEntry(const K& key, int index, const V& value) {
            HashMapEntry* entry = new HashMapEntry(key, value);
            entry->next = elementData[index];
            elementData[index] = entry;
            return entry;
        }

        HashMapEntry* createHashedEntry(const K& key, int index, int hash) {
            HashMapEntry* entry = new HashMapEntry(key, V(), hash);
            entry->next = elementData[index];
            elementData[index] = entry;
            return entry;
        }

        // Removes the given entry from the map and deletes it
        void removeEntry(HashMapEntry* entry) {
            int index = entry->origKeyHash & (elementData.length() - 1);
            HashMapEntry* current = elementData[index];
            if (current == entry) {
                elementData[index] = entry->next;
            } else {
                while (current->next != entry) {
                    current = current->next;
                }
                current->next = entry->next;
            }
            delete entry;
            modCount++;
            elementCount--;
        }

        // Removes but doesn't delete the entry in the map with the given key.
        HashMapEntry* removeEntry(const K& key) {

            int index = 0;
            HashMapEntry* current = NULL;
            HashMapEntry* last = NULL;

            int hash = hashFunc(key);
            index = hash & (elementData.length() - 1);
            current = elementData[index];
            while (current != NULL && !(current->origKeyHash == hash && key == current->getKey())) {
                last = current;
                current = current->next;
            }

            if (current == NULL) {
                return NULL;
            }

            if (last == NULL) {
                elementData[index] = current->next;
            } else {
                last->next = current->next;
            }

            modCount++;
            elementCount--;
            return current;
        }

    };

}}

#endif /* _DECAF_UTIL_HASHMAP_H_ */
