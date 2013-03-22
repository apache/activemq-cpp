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

#ifndef _DECAF_UTIL_LINKEDHASHMAP_H_
#define _DECAF_UTIL_LINKEDHASHMAP_H_

#include <decaf/util/Config.h>

#include <decaf/util/HashMap.h>

namespace decaf {
namespace util {

    /**
     * Hashed and linked list implementation of the Map interface, with predictable iteration order.
     *
     * This implementation differs from HashMap in that it maintains a doubly-linked list running
     * through all of its entries. This linked list defines the iteration ordering, which is
     * normally the order in which keys were inserted into the map (insertion-order). Note that
     * insertion order is not affected if a key is re-inserted into the map. (A key k is reinserted
     * into a map m if m.put(k, v) is invoked when m.containsKey(k) would return true immediately
     * prior to the invocation.)
     *
     * This implementation spares its clients from the unspecified, generally chaotic ordering
     * provided by HashMap, without incurring the increased cost associated with TreeMap. It can
     * be used to produce a copy of a map that has the same order as the original, regardless of
     * the original map's implementation:
     *
     *    void foo(Map m) {
     *        Map copy = new LinkedHashMap(m);
     *        ...
     *    }
     *
     * This technique is particularly useful if a module takes a map on input, copies it, and later
     * returns results whose order is determined by that of the copy. (Clients generally appreciate
     * having things returned in the same order they were presented.)
     *
     * A special constructor is provided to create a linked hash map whose order of iteration is the
     * order in which its entries were last accessed, from least-recently accessed to most-recently
     * (access-order). This kind of map is well-suited to building LRU caches. Invoking the put or
     * get method results in an access to the corresponding entry (assuming it exists after the
     * invocation completes). The putAll method generates one entry access for each mapping in the
     * specified map, in the order that key-value mappings are provided by the specified map's entry
     * set iterator. No other methods generate entry accesses. In particular, operations on
     * collection-views do not affect the order of iteration of the backing map.
     *
     * The removeEldestEntry(MapEntry) method may be overridden to impose a policy for removing
     * stale mappings automatically when new mappings are added to the map.  When the entries are
     * about to be removed from the map the onEviction method is called giving subclasses a
     * chance to delete pointer values or perform other cleanup prior to the mapping being
     * removed.
     *
     * This class provides all of the optional Map operations.  Like HashMap, it provides
     * constant-time performance for the basic operations (add, contains and remove), assuming
     * the hash function disperses elements properly among the buckets. Performance is likely
     * to be just slightly below that of HashMap, due to the added expense of maintaining the
     * linked list, with one exception: Iteration over the collection-views of a LinkedHashMap
     * requires time proportional to the size of the map, regardless of its capacity. Iteration
     * over a HashMap is likely to be more expensive, requiring time proportional to its capacity.
     *
     * A linked hash map has two parameters that affect its performance: initial capacity and load
     * factor. They are defined precisely as for HashMap. Note, however, that the penalty for
     * choosing an excessively high value for initial capacity is less severe for this class than
     * for HashMap, as iteration times for this class are unaffected by capacity.
     *
     * Note that this implementation is not synchronized. If multiple threads access a linked hash
     * map concurrently, and at least one of the threads modifies the map structurally, it must
     * be synchronized externally. This is typically accomplished by synchronizing on some object
     * that naturally encapsulates the map. If no such object exists, the map should be "wrapped"
     * using the Collections.synchronizedMap method. This is best done at creation time, to prevent
     * accidental unsynchronized access to the map:
     *
     *   Map<K,V>* m = Collections::synchronizedMap(new LinkedHashMap(...));
     *
     * A structural modification is any operation that adds or deletes one or more mappings or, in
     * the case of access-ordered linked hash maps, affects iteration order. In insertion-ordered
     * linked hash maps, merely changing the value associated with a key that is already contained
     * in the map is not a structural modification. In access-ordered linked hash maps, merely
     * querying the map with get is a structural modification.)
     *
     * The iterators returned by the iterator method of the collections returned by all of this
     * class's collection view methods are fail-fast: if the map is structurally modified at any
     * time after the iterator is created, in any way except through the iterator's own remove
     * method, the iterator will throw a ConcurrentModificationException. Thus, in the face of
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
    template<typename K, typename V, typename HASHCODE = HashCode<K> >
    class LinkedHashMap : public HashMap<K, V, HASHCODE> {
    private:

        class LinkedHashMapEntry : public HashMap<K, V, HASHCODE>::HashMapEntry {
        private:

            LinkedHashMapEntry(const LinkedHashMapEntry&);
            LinkedHashMapEntry& operator= (const LinkedHashMapEntry&);

        public:

            LinkedHashMapEntry* chainForward;
            LinkedHashMapEntry* chainBackward;

        public:

            LinkedHashMapEntry(const K& key, const V& value, int hash) :
                HashMap<K, V, HASHCODE>::HashMapEntry(key, value, hash), chainForward(), chainBackward() {
            }

            LinkedHashMapEntry(const K& key, const V& value) :
                HashMap<K, V, HASHCODE>::HashMapEntry(key, value), chainForward(), chainBackward() {
            }
        };

    private:

        bool accessOrder;
        mutable LinkedHashMapEntry* head;
        mutable LinkedHashMapEntry* tail;

    private:

        class AbstractMapIterator {
        protected:

            int expectedModCount;
            LinkedHashMapEntry* futureEntry;
            LinkedHashMapEntry* currentEntry;
            LinkedHashMap* associatedMap;

        private:

            AbstractMapIterator(const AbstractMapIterator&);
            AbstractMapIterator& operator= (const AbstractMapIterator&);

        public:

            AbstractMapIterator(LinkedHashMap* parent) : expectedModCount(parent->modCount),
                                                         futureEntry(parent->head),
                                                         currentEntry(NULL),
                                                         associatedMap(parent) {
            }

            virtual ~AbstractMapIterator() {}

            void checkConcurrentMod() const {
                if (expectedModCount != associatedMap->modCount) {
                    throw ConcurrentModificationException(
                        __FILE__, __LINE__, "LinkedHashMap modified outside this iterator");
                }
            }

            virtual bool checkHasNext() const {
                return (futureEntry != NULL);
            }

            void makeNext() {
                checkConcurrentMod();
                if (!checkHasNext()) {
                    throw decaf::util::NoSuchElementException(
                        __FILE__, __LINE__, "No next element");
                }
                currentEntry = futureEntry;
                futureEntry = futureEntry->chainForward;
            }

            virtual void doRemove() {
                checkConcurrentMod();
                if (currentEntry == NULL) {
                    throw decaf::lang::exceptions::IllegalStateException(
                        __FILE__, __LINE__, "Remove called before call to next()");
                }

                LinkedHashMapEntry* entry =  currentEntry;
                LinkedHashMapEntry* prev = entry->chainBackward;
                LinkedHashMapEntry* next = entry->chainForward;
                LinkedHashMap* map = associatedMap;

                // currentEntry gets deleted here.
                associatedMap->removeEntry(currentEntry);
                currentEntry = NULL;

                if (prev != NULL) {
                    prev->chainForward = next;
                    if (next != NULL) {
                        next->chainBackward = prev;
                    } else {
                        map->tail = prev;
                    }
                } else {
                    map->head = next;
                    if (next != NULL) {
                        next->chainBackward = NULL;
                    } else {
                        map->tail = NULL;
                    }
                }
                expectedModCount++;
            }

        };

        class EntryIterator : public Iterator< MapEntry<K,V> >, public AbstractMapIterator {
        private:

            EntryIterator(const EntryIterator&);
            EntryIterator& operator= (const EntryIterator&);

        public:

            EntryIterator(LinkedHashMap* parent) : AbstractMapIterator(parent) {
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

            KeyIterator(LinkedHashMap* parent) : AbstractMapIterator(parent) {
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

            ValueIterator(LinkedHashMap* parent) : AbstractMapIterator(parent) {
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

            int expectedModCount;
            const LinkedHashMapEntry* futureEntry;
            const LinkedHashMapEntry* currentEntry;
            const LinkedHashMap* associatedMap;

        private:

            ConstAbstractMapIterator(const ConstAbstractMapIterator&);
            ConstAbstractMapIterator& operator= (const ConstAbstractMapIterator&);

        public:

            ConstAbstractMapIterator(const LinkedHashMap* parent) : expectedModCount(parent->modCount),
                                                                    futureEntry(parent->head),
                                                                    currentEntry(NULL),
                                                                    associatedMap(parent) {
            }

            virtual ~ConstAbstractMapIterator() {}

            virtual bool checkHasNext() const {
                return (futureEntry != NULL);
            }

            void checkConcurrentMod() const {
                if (expectedModCount != associatedMap->modCount) {
                    throw ConcurrentModificationException(
                        __FILE__, __LINE__, "LinkedHashMap modified outside this iterator");
                }
            }

            void makeNext() {
                checkConcurrentMod();
                if (!checkHasNext()) {
                    throw decaf::util::NoSuchElementException(
                        __FILE__, __LINE__, "No next element");
                }
                currentEntry = futureEntry;
                futureEntry = futureEntry->chainForward;
            }
        };

        class ConstEntryIterator : public Iterator< MapEntry<K,V> >, public ConstAbstractMapIterator {
        private:

            ConstEntryIterator(const ConstEntryIterator&);
            ConstEntryIterator& operator= (const ConstEntryIterator&);

        public:

            ConstEntryIterator(const LinkedHashMap* parent) : ConstAbstractMapIterator(parent) {
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

            ConstKeyIterator(const LinkedHashMap* parent) : ConstAbstractMapIterator(parent) {
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

            ConstValueIterator(const LinkedHashMap* parent) : ConstAbstractMapIterator(parent) {
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
        class LinkedHashMapEntrySet : public HashMap<K, V, HASHCODE>::HashMapEntrySet {
        private:

            LinkedHashMap* associatedMap;

        private:

            LinkedHashMapEntrySet(const LinkedHashMapEntrySet&);
            LinkedHashMapEntrySet& operator= (const LinkedHashMapEntrySet&);

        public:

            LinkedHashMapEntrySet(LinkedHashMap* parent) :
                HashMap<K, V, HASHCODE>::HashMapEntrySet(parent), associatedMap(parent) {
            }

            virtual ~LinkedHashMapEntrySet() {}

            virtual Iterator< MapEntry<K, V> >* iterator() {
                return new EntryIterator(associatedMap);
            }

            virtual Iterator< MapEntry<K, V> >* iterator() const {
                return new ConstEntryIterator(associatedMap);
            }
        };

        // Special Set implementation that is backed by this HashMap
        class ConstLinkedHashMapEntrySet : public HashMap<K, V, HASHCODE>::ConstHashMapEntrySet {
        private:

            const LinkedHashMap* associatedMap;

        private:

            ConstLinkedHashMapEntrySet(const ConstLinkedHashMapEntrySet&);
            ConstLinkedHashMapEntrySet& operator= (const ConstLinkedHashMapEntrySet&);

        public:

            ConstLinkedHashMapEntrySet(const LinkedHashMap* parent) :
                HashMap<K, V, HASHCODE>::ConstHashMapEntrySet(parent), associatedMap(parent) {
            }

            virtual ~ConstLinkedHashMapEntrySet() {}

            virtual Iterator< MapEntry<K, V> >* iterator() {
                throw decaf::lang::exceptions::UnsupportedOperationException(
                        __FILE__, __LINE__, "Can't return a non-const iterator for a const collection");
            }

            virtual Iterator< MapEntry<K, V> >* iterator() const {
                return new ConstEntryIterator(associatedMap);
            }
        };

    private:

        class LinkedHashMapKeySet : public HashMap<K, V, HASHCODE>::HashMapKeySet {
        private:

            LinkedHashMap* associatedMap;

        private:

            LinkedHashMapKeySet(const LinkedHashMapKeySet&);
            LinkedHashMapKeySet& operator= (const LinkedHashMapKeySet&);

        public:

            LinkedHashMapKeySet(LinkedHashMap* parent) :
                HashMap<K, V, HASHCODE>::HashMapKeySet(parent), associatedMap(parent) {
            }

            virtual ~LinkedHashMapKeySet() {}

            virtual Iterator<K>* iterator() {
                return new KeyIterator(this->associatedMap);
            }

            virtual Iterator<K>* iterator() const {
                return new ConstKeyIterator(this->associatedMap);
            }
        };

        class ConstLinkedHashMapKeySet : public HashMap<K, V, HASHCODE>::ConstHashMapKeySet {
        private:

            const LinkedHashMap* associatedMap;

        private:

            ConstLinkedHashMapKeySet(const ConstLinkedHashMapKeySet&);
            ConstLinkedHashMapKeySet& operator= (const ConstLinkedHashMapKeySet&);

        public:

            ConstLinkedHashMapKeySet(const LinkedHashMap* parent) :
                HashMap<K, V, HASHCODE>::ConstHashMapKeySet(parent), associatedMap(parent) {
            }

            virtual ~ConstLinkedHashMapKeySet() {}

            virtual Iterator<K>* iterator() const {
                return new ConstKeyIterator(this->associatedMap);
            }
        };

    private:

        class LinkedHashMapValueCollection : public HashMap<K, V, HASHCODE>::HashMapValueCollection {
        private:

            LinkedHashMap* associatedMap;

        private:

            LinkedHashMapValueCollection(const LinkedHashMapValueCollection&);
            LinkedHashMapValueCollection& operator= (const LinkedHashMapValueCollection&);

        public:

            LinkedHashMapValueCollection(LinkedHashMap* parent) :
                HashMap<K, V, HASHCODE>::HashMapValueCollection(parent), associatedMap(parent) {
            }

            virtual ~LinkedHashMapValueCollection() {}

            virtual Iterator<V>* iterator() {
                return new ValueIterator(this->associatedMap);
            }

            virtual Iterator<V>* iterator() const {
                return new ConstValueIterator(this->associatedMap);
            }
        };

        class ConstLinkedHashMapValueCollection : public HashMap<K, V, HASHCODE>::ConstHashMapValueCollection {
        private:

            const LinkedHashMap* associatedMap;

        private:

            ConstLinkedHashMapValueCollection(const ConstLinkedHashMapValueCollection&);
            ConstLinkedHashMapValueCollection& operator= (const ConstLinkedHashMapValueCollection&);

        public:

            ConstLinkedHashMapValueCollection(const LinkedHashMap* parent) :
                HashMap<K, V, HASHCODE>::ConstHashMapValueCollection(parent), associatedMap(parent) {
            }

            virtual ~ConstLinkedHashMapValueCollection() {}

            virtual Iterator<V>* iterator() const {
                return new ConstValueIterator(this->associatedMap);
            }
        };

    public:

        /**
         * Constructs an empty insertion-ordered LinkedHashMap instance with the default
         * initial capacity (16) and load factor (0.75).
         */
        LinkedHashMap() : HashMap<K, V, HASHCODE>(), accessOrder(false), head(), tail() {
        }

        /**
         * Constructs a new LinkedHashMap instance with the specified capacity.
         *
         * @param capacity
         *      The initial capacity of this map.
         *
         * @throws IllegalArgumentException if the capacity is less than zero.
         */
        LinkedHashMap(int capacity) : HashMap<K, V, HASHCODE>(capacity), accessOrder(false), head(), tail() {
        }

        /**
         * Constructs a new {@code LinkedHashMap} instance with the specified
         * capacity and load factor.
         *
         * @param capacity
         *      The initial capacity of this map.
         * @param load
         *      The initial load factor for this map.
         *
         * @throws IllegalArgumentException
         *     If the capacity is less than zero or the load factor is less or equal to zero.
         */
        LinkedHashMap(int capacity, float load) :
            HashMap<K, V, HASHCODE>(capacity, load), accessOrder(false), head(), tail() {

        }

        /**
         * Constructs a new {@code LinkedHashMap} instance with the specified
         * capacity, load factor and a flag specifying the ordering behavior.
         *
         * @param capacity
         *      The initial capacity of this map.
         * @param load
         *      The initial load factor for this map.
         * @param order
         *      True if the ordering should be done based on the last access (from
         *      least-recently accessed to most-recently accessed), and  false if
         *      the ordering should be the order in which the entries were inserted.
         *
         * @throws IllegalArgumentException
         *     If the capacity is less than zero or the load factor is less or equal to zero.
         */
        LinkedHashMap(int capacity, float load, bool order) :
            HashMap<K, V, HASHCODE>(capacity, load), accessOrder(order), head(), tail() {
        }

        /**
         * Constructs a new LinkedHashMap instance containing the mappings
         * from the specified map. The order of the elements is preserved.
         *
         * @param map
         *      The mappings to add to this Map instance.
         */
        LinkedHashMap(const HashMap<K,V>& map) : HashMap<K, V, HASHCODE>(map), accessOrder(false), head(), tail() {
        }

        virtual ~LinkedHashMap() {}

    protected:

        /**
         * This method is queried from the put and putAll methods to check if the
         * eldest member of the map should be deleted before adding the new member.
         * If this map was created with accessOrder = true, then the result of
         * removeEldestEntry is assumed to be false.
         *
         * @param eldest
         *      The entry to check if it should be removed.
         *
         * @return true if the eldest member should be removed.
         */
        virtual bool removeEldestEntry(const MapEntry<K, V>& eldest DECAF_UNUSED) {
            return false;
        }

        /**
         * This method is called when the removeEldestEntry has returned true and a
         * MapEntry is about to be removed from the Map.  This method allows for Maps
         * that contain pointers in their MapEntry object to have a chance to properly
         * delete the pointer when the entry is removed.
         *
         * @param eldest
         *      The MapEntry value that is about to be removed from the Map.
         */
        virtual void onEviction(const MapEntry<K, V>& eldest DECAF_UNUSED) {}

    public:

        virtual bool containsValue(const V& value) const {
            LinkedHashMapEntry* entry = head;
            while (entry != NULL) {
                if (value == entry->getValue()) {
                    return true;
                }
                entry = entry->chainForward;
            }
            return false;
        }

        virtual void clear() {
            HashMap<K, V, HASHCODE>::clear();
            this->head = NULL;
            this->tail = NULL;
        }

        virtual bool put(const K& key, const V& value) {
            bool result = this->putImpl(key, value);

            if (this->removeEldestEntry(*head)) {
                this->onEviction(*head);
                this->remove(head->getKey());
            }

            return result;
        }

        virtual bool put(const K& key, const V& value, V& oldValue) {
            bool result = this->putImpl(key, value, oldValue);

            if (this->removeEldestEntry(*head)) {
                this->onEviction(*head);
                this->remove(head->getKey());
            }

            return result;
        }

        virtual V remove(const K& key) {

            LinkedHashMapEntry* toRemove = (LinkedHashMapEntry*) this->removeEntry(key);
            if (toRemove != NULL) {
                LinkedHashMapEntry* prev = toRemove->chainBackward;
                LinkedHashMapEntry* next = toRemove->chainForward;
                if (prev != NULL) {
                    prev->chainForward = next;
                } else {
                    head = next;
                }
                if (next != NULL) {
                    next->chainBackward = prev;
                } else {
                    tail = prev;
                }

                V oldValue = toRemove->getValue();
                delete toRemove;
                return oldValue;
            }

            throw NoSuchElementException(
                __FILE__, __LINE__, "Specified key not present in the Map.");
        }

        virtual Set< MapEntry<K,V> >& entrySet() {
            if (this->cachedEntrySet == NULL) {
                this->cachedEntrySet.reset(new LinkedHashMapEntrySet(this));
            }
            return *(this->cachedEntrySet);
        }

        virtual const Set< MapEntry<K,V> >& entrySet() const {
            if (this->cachedConstEntrySet == NULL) {
                this->cachedConstEntrySet.reset(new ConstLinkedHashMapEntrySet(this));
            }
            return *(this->cachedConstEntrySet);
        }

        virtual Set<K>& keySet() {
            if (this->cachedKeySet == NULL) {
                this->cachedKeySet.reset(new LinkedHashMapKeySet(this));
            }
            return *(this->cachedKeySet);
        }

        virtual const Set<K>& keySet() const {
            if (this->cachedConstKeySet == NULL) {
                this->cachedConstKeySet.reset(new ConstLinkedHashMapKeySet(this));
            }
            return *(this->cachedConstKeySet);
        }

        virtual Collection<V>& values() {
            if (this->cachedValueCollection == NULL) {
                this->cachedValueCollection.reset(new LinkedHashMapValueCollection(this));
            }
            return *(this->cachedValueCollection);
        }

        virtual const Collection<V>& values() const {
            if (this->cachedConstValueCollection == NULL) {
                this->cachedConstValueCollection.reset(new ConstLinkedHashMapValueCollection(this));
            }
            return *(this->cachedConstValueCollection);
        }

        virtual std::string toString() const {
            return "LinkedHashMap";
        }

    protected:

        virtual LinkedHashMapEntry* getEntry(const K& key) const {
            LinkedHashMapEntry* result = NULL;

            int hash = this->hashFunc(key);
            int index = hash & (this->elementData.length() - 1);
            result = (LinkedHashMapEntry*) this->findKeyEntry(key, index, hash);

            if (result != NULL && accessOrder && tail != result) {
                LinkedHashMapEntry* prev = result->chainBackward;
                LinkedHashMapEntry* next = result->chainForward;
                next->chainBackward = prev;
                if (prev != NULL) {
                    prev->chainForward = next;
                } else {
                    head = next;
                }
                result->chainForward = NULL;
                result->chainBackward = tail;
                tail->chainForward = result;
                tail = result;
            }

            return result;
        }

        virtual typename HashMap<K, V, HASHCODE>::HashMapEntry* createEntry(const K& key, int index, const V& value) {
            LinkedHashMapEntry* entry = new LinkedHashMapEntry(key, value);
            entry->next = this->elementData[index];
            this->elementData[index] = entry;
            linkEntry(entry);
            return entry;
        }

        virtual typename HashMap<K, V, HASHCODE>::HashMapEntry* createHashedEntry(const K& key, int index, int hash) {
            LinkedHashMapEntry* entry = new LinkedHashMapEntry(key, V(), hash);
            entry->next = this->elementData[index];
            this->elementData[index] = entry;
            linkEntry(entry);
            return entry;
        }

        void linkEntry(LinkedHashMapEntry* entry) {
            if (tail == entry) {
                return;
            }

            if (head == NULL) {
                // Check if the map is empty
                head = tail = entry;
                return;
            }

            // we need to link the new entry into either the head or tail
            // of the chain depending on if the LinkedHashMap is accessOrder or not
            LinkedHashMapEntry* prev = entry->chainBackward;
            LinkedHashMapEntry* next = entry->chainForward;
            if (prev == NULL) {
                if (next != NULL) {
                    // The entry must be the head but not the tail
                    if (accessOrder) {
                        head = next;
                        next->chainBackward = NULL;
                        entry->chainBackward = tail;
                        entry->chainForward = NULL;
                        tail->chainForward = entry;
                        tail = entry;
                    }
                } else {
                    // This is a new entry
                    entry->chainBackward = tail;
                    entry->chainForward = NULL;
                    tail->chainForward = entry;
                    tail = entry;
                }
                return;
            }

            if (next == NULL) {
                // The entry must be the tail so we can't get here
                return;
            }

            // The entry is neither the head nor tail
            if (accessOrder) {
                prev->chainForward = next;
                next->chainBackward = prev;
                entry->chainForward = NULL;
                entry->chainBackward = tail;
                tail->chainForward = entry;
                tail = entry;
            }
        }

        virtual bool putImpl(const K& key, const V& value) {
            V oldValue;
            return putImpl(key, value, oldValue);
        }

        virtual bool putImpl(const K& key, const V& value, V& oldValue) {

            LinkedHashMapEntry* entry;
            if (this->elementCount == 0) {
                head = tail = NULL;
            }

            bool replaced = true;
            int hash = this->hashFunc(key);
            int index = hash & (this->elementData.length() - 1);

            entry = (LinkedHashMapEntry*) this->findKeyEntry(key, index, hash);
            if (entry == NULL) {
                this->modCount++;
                if (++this->elementCount > this->threshold) {
                    this->rehash();
                    index = hash & (this->elementData.length() - 1);
                }
                entry = (LinkedHashMapEntry*) this->createHashedEntry(key, index, hash);
                replaced = false;
            } else {
                this->linkEntry(entry);
                oldValue = entry->getValue();
            }

            entry->setValue(value);
            return replaced;
        }

    };

}}

#endif /* _DECAF_UTIL_LINKEDHASHMAP_H_ */
