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

#ifndef _DECAF_UTIL_CONCURRENTSTLMAP_H_
#define _DECAF_UTIL_CONCURRENTSTLMAP_H_

#include <map>
#include <vector>
#include <decaf/util/NoSuchElementException.h>
#include <decaf/util/concurrent/Synchronizable.h>
#include <decaf/util/concurrent/ConcurrentMap.h>
#include <decaf/util/concurrent/Mutex.h>
#include <decaf/util/ConcurrentModificationException.h>
#include <decaf/util/Map.h>
#include <decaf/util/AbstractCollection.h>
#include <decaf/util/AbstractSet.h>
#include <decaf/util/Iterator.h>
#include <decaf/lang/Pointer.h>

namespace decaf{
namespace util{
namespace concurrent{

    /**
     * Map template that wraps around a std::map to provide a more user-friendly
     * interface and to provide common functions that do not exist in std::map.
     *
     * This version of Map extends the ConcurrentMap inteface and implements all
     * the methods defined in that interface.  Unlike a Java ConcurrentHashMap this
     * implementations synchronizes all methods such that any call to this class
     * will block if another thread is already holding a lock, much like the
     * Java HashTable.
     *
     * @since 1.0
     */
    template <typename K, typename V, typename COMPARATOR = std::less<K> >
    class ConcurrentStlMap : public ConcurrentMap<K, V> {
    private:

        std::map<K,V,COMPARATOR> valueMap;
        mutable concurrent::Mutex mutex;
        int modCount;

    private:

        class AbstractMapIterator {
        protected:

            mutable int position;
            int expectedModCount;
            typename std::map<K,V,COMPARATOR>::iterator futureEntry;
            typename std::map<K,V,COMPARATOR>::iterator currentEntry;

            ConcurrentStlMap* associatedMap;

        private:

            AbstractMapIterator(const AbstractMapIterator&);
            AbstractMapIterator& operator= (const AbstractMapIterator&);

        public:

            AbstractMapIterator(ConcurrentStlMap* parent) : position(0),
                                                            expectedModCount(parent->modCount),
                                                            futureEntry(parent->valueMap.begin()),
                                                            currentEntry(parent->valueMap.end()),
                                                            associatedMap(parent) {
            }

            virtual ~AbstractMapIterator() {}

            virtual bool checkHasNext() const {
                synchronized(&this->associatedMap->mutex) {
                    if (futureEntry != this->associatedMap->valueMap.end()) {
                        return true;
                    }
                }
                return false;
            }

            void checkConcurrentMod() const {
                if (expectedModCount != this->associatedMap->modCount) {
                    throw ConcurrentModificationException(
                        __FILE__, __LINE__, "StlMap modified outside this iterator");
                }
            }

            void makeNext() {
                synchronized(&this->associatedMap->mutex) {
                    checkConcurrentMod();

                    if (!checkHasNext()) {
                        throw NoSuchElementException(__FILE__, __LINE__, "No next element");
                    }

                    currentEntry = futureEntry;
                    futureEntry++;
                }
            }

            virtual void doRemove() {
                synchronized(&this->associatedMap->mutex) {
                    checkConcurrentMod();

                    if (currentEntry == this->associatedMap->valueMap.end()) {
                        throw decaf::lang::exceptions::IllegalStateException(
                            __FILE__, __LINE__, "Remove called before call to next()");
                    }

                    this->associatedMap->valueMap.erase(currentEntry);
                    currentEntry = this->associatedMap->valueMap.end();

                    expectedModCount++;
                    associatedMap->modCount++;
                }
            }
        };

        class EntryIterator : public Iterator< MapEntry<K,V> >, public AbstractMapIterator {
        private:

            EntryIterator(const EntryIterator&);
            EntryIterator& operator= (const EntryIterator&);

        public:

            EntryIterator(ConcurrentStlMap* parent) : AbstractMapIterator(parent) {
            }

            virtual ~EntryIterator() {}

            virtual bool hasNext() const {
                return this->checkHasNext();
            }

            virtual MapEntry<K, V> next() {
                synchronized(&this->associatedMap->mutex) {
                    this->makeNext();
                    return MapEntry<K, V>(this->currentEntry->first, this->currentEntry->second);
                }

                return MapEntry<K, V>();
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

            KeyIterator(ConcurrentStlMap* parent) : AbstractMapIterator(parent) {
            }

            virtual ~KeyIterator() {}

            virtual bool hasNext() const {
                return this->checkHasNext();
            }

            virtual K next() {
                synchronized(&this->associatedMap->mutex) {
                    this->makeNext();
                    return this->currentEntry->first;
                }

                return K();
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

            ValueIterator(ConcurrentStlMap* parent) : AbstractMapIterator(parent) {
            }

            virtual ~ValueIterator() {}

            virtual bool hasNext() const {
                return this->checkHasNext();
            }

            virtual V next() {
                synchronized(&this->associatedMap->mutex) {
                    this->makeNext();
                    return this->currentEntry->second;
                }

                return V();
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
            typename std::map<K,V,COMPARATOR>::const_iterator futureEntry;
            typename std::map<K,V,COMPARATOR>::const_iterator currentEntry;

            const ConcurrentStlMap* associatedMap;

        private:

            ConstAbstractMapIterator(const ConstAbstractMapIterator&);
            ConstAbstractMapIterator& operator= (const ConstAbstractMapIterator&);

        public:

            ConstAbstractMapIterator(const ConcurrentStlMap* parent) : position(0),
                                                                       expectedModCount(parent->modCount),
                                                                       futureEntry(parent->valueMap.begin()),
                                                                       currentEntry(parent->valueMap.end()),
                                                                       associatedMap(parent) {
            }

            virtual ~ConstAbstractMapIterator() {}

            virtual bool checkHasNext() const {
                synchronized(&this->associatedMap->mutex) {
                    if (futureEntry != this->associatedMap->valueMap.end()) {
                        return true;
                    }
                }
                return false;
            }

            void checkConcurrentMod() const {
                synchronized(&this->associatedMap->mutex) {
                    if (expectedModCount != this->associatedMap->modCount) {
                        throw ConcurrentModificationException(
                            __FILE__, __LINE__, "StlMap modified outside this iterator");
                    }
                }
            }

            void makeNext() {
                synchronized(&this->associatedMap->mutex) {
                    checkConcurrentMod();

                    if (!checkHasNext()) {
                        throw NoSuchElementException(__FILE__, __LINE__, "No next element");
                    }

                    currentEntry = futureEntry;
                    futureEntry++;
                }
            }
        };

        class ConstEntryIterator : public Iterator< MapEntry<K,V> >, public ConstAbstractMapIterator {
        private:

            ConstEntryIterator(const ConstEntryIterator&);
            ConstEntryIterator& operator= (const ConstEntryIterator&);

        public:

            ConstEntryIterator(const ConcurrentStlMap* parent) : ConstAbstractMapIterator(parent) {
            }

            virtual ~ConstEntryIterator() {}

            virtual bool hasNext() const {
                return this->checkHasNext();
            }

            virtual MapEntry<K, V> next() {
                synchronized(&this->associatedMap->mutex) {
                    this->makeNext();
                    return MapEntry<K, V>(this->currentEntry->first, this->currentEntry->second);
                }

                return MapEntry<K, V>();
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

            ConstKeyIterator(const ConcurrentStlMap* parent) : ConstAbstractMapIterator(parent) {
            }

            virtual ~ConstKeyIterator() {}

            virtual bool hasNext() const {
                return this->checkHasNext();
            }

            virtual K next() {
                synchronized(&this->associatedMap->mutex) {
                    this->makeNext();
                    return this->currentEntry->first;
                }

                return K();
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

            ConstValueIterator(const ConcurrentStlMap* parent) : ConstAbstractMapIterator(parent) {
            }

            virtual ~ConstValueIterator() {}

            virtual bool hasNext() const {
                return this->checkHasNext();
            }

            virtual V next() {
                synchronized(&this->associatedMap->mutex) {
                    this->makeNext();
                    return this->currentEntry->second;
                }

                return V();
            }

            virtual void remove() {
                throw lang::exceptions::UnsupportedOperationException(
                    __FILE__, __LINE__, "Cannot write to a const Iterator." );
            }
        };

    private:

        // Special Set implementation that is backed by this HashMap
        class StlMapEntrySet : public AbstractSet< MapEntry<K, V> > {
        private:

            ConcurrentStlMap* associatedMap;

        private:

            StlMapEntrySet(const StlMapEntrySet&);
            StlMapEntrySet& operator= (const StlMapEntrySet&);

        public:

            StlMapEntrySet(ConcurrentStlMap* parent) : AbstractSet< MapEntry<K,V> >(), associatedMap(parent) {
            }

            virtual ~StlMapEntrySet() {}

            virtual int size() const {
                return associatedMap->size();
            }

            virtual void clear() {
                associatedMap->clear();
            }

            virtual bool remove(const MapEntry<K,V>& entry) {
                synchronized(&this->associatedMap->mutex) {
                    if (this->associatedMap->containsKey(entry.getKey()) &&
                        this->associatedMap->get(entry.getKey()) == entry.getValue()) {
                        associatedMap->remove(entry.getKey());
                        return true;
                    }
                }

                return false;
            }

            virtual bool contains(const MapEntry<K,V>& entry) {
                synchronized(&this->associatedMap->mutex) {
                    if (this->associatedMap->containsKey(entry.getKey()) &&
                        this->associatedMap->get(entry.getKey()) == entry.getValue()) {
                        return true;
                    }
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
        class ConstStlMapEntrySet : public AbstractSet< MapEntry<K, V> > {
        private:

            const ConcurrentStlMap* associatedMap;

        private:

            ConstStlMapEntrySet(const ConstStlMapEntrySet&);
            ConstStlMapEntrySet& operator= (const ConstStlMapEntrySet&);

        public:

            ConstStlMapEntrySet(const ConcurrentStlMap* parent) : AbstractSet< MapEntry<K,V> >(), associatedMap(parent) {
            }

            virtual ~ConstStlMapEntrySet() {}

            virtual int size() const {
                return associatedMap->size();
            }

            virtual void clear() {
                throw decaf::lang::exceptions::UnsupportedOperationException(
                        __FILE__, __LINE__, "Can't clear a const collection");
            }

            virtual bool remove(const MapEntry<K,V>& entry DECAF_UNUSED) {
                throw decaf::lang::exceptions::UnsupportedOperationException(
                        __FILE__, __LINE__, "Can't remove from const collection");
            }

            virtual bool contains(const MapEntry<K,V>& entry) {
                synchronized(&this->associatedMap->mutex) {
                    if (this->associatedMap->containsKey(entry.getKey()) &&
                        this->associatedMap->get(entry.getKey()) == entry.getValue()) {
                        return true;
                    }
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

        class StlMapKeySet : public AbstractSet<K> {
        private:

            ConcurrentStlMap* associatedMap;

        private:

            StlMapKeySet(const StlMapKeySet&);
            StlMapKeySet& operator= (const StlMapKeySet&);

        public:

            StlMapKeySet(ConcurrentStlMap* parent) : AbstractSet<K>(), associatedMap(parent) {
            }

            virtual ~StlMapKeySet() {}

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
                synchronized(&this->associatedMap->mutex) {
                    if (this->associatedMap->containsKey(key)) {
                        associatedMap->remove(key);
                        return true;
                    }
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

        class ConstStlMapKeySet : public AbstractSet<K> {
        private:

            const ConcurrentStlMap* associatedMap;

        private:

            ConstStlMapKeySet(const ConstStlMapKeySet&);
            ConstStlMapKeySet& operator= (const ConstStlMapKeySet&);

        public:

            ConstStlMapKeySet(const ConcurrentStlMap* parent) : AbstractSet<K>(), associatedMap(parent) {
            }

            virtual ~ConstStlMapKeySet() {}

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

            virtual bool remove(const K& key DECAF_UNUSED) {
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

        class StlMapValueCollection : public AbstractCollection<V> {
        private:

            ConcurrentStlMap* associatedMap;

        private:

            StlMapValueCollection(const StlMapValueCollection&);
            StlMapValueCollection& operator= (const StlMapValueCollection&);

        public:

            StlMapValueCollection(ConcurrentStlMap* parent) : AbstractCollection<V>(), associatedMap(parent) {
            }

            virtual ~StlMapValueCollection() {}

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

        class ConstStlMapValueCollection : public AbstractCollection<V> {
        private:

            const ConcurrentStlMap* associatedMap;

        private:

            ConstStlMapValueCollection(const ConstStlMapValueCollection&);
            ConstStlMapValueCollection& operator= (const ConstStlMapValueCollection&);

        public:

            ConstStlMapValueCollection(const ConcurrentStlMap* parent) : AbstractCollection<V>(), associatedMap(parent) {
            }

            virtual ~ConstStlMapValueCollection() {}

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

        // Cached values that are only initialized once a request for them is made.
        decaf::lang::Pointer<StlMapEntrySet> cachedEntrySet;
        decaf::lang::Pointer<StlMapKeySet> cachedKeySet;
        decaf::lang::Pointer<StlMapValueCollection> cachedValueCollection;

        // Cached values that are only initialized once a request for them is made.
        mutable decaf::lang::Pointer<ConstStlMapEntrySet> cachedConstEntrySet;
        mutable decaf::lang::Pointer<ConstStlMapKeySet> cachedConstKeySet;
        mutable decaf::lang::Pointer<ConstStlMapValueCollection> cachedConstValueCollection;

    public:

        /**
         * Default constructor - does nothing.
         */
        ConcurrentStlMap() : ConcurrentMap<K,V>(), valueMap(), mutex(), modCount(0),
                             cachedEntrySet(), cachedKeySet(), cachedValueCollection(),
                             cachedConstEntrySet(), cachedConstKeySet(), cachedConstValueCollection() {

        }

        /**
         * Copy constructor - copies the content of the given map into this
         * one.
         * @param source The source map.
         */
        ConcurrentStlMap(const ConcurrentStlMap& source) : ConcurrentMap<K, V>(), valueMap(), mutex(), modCount(0),
                                                           cachedEntrySet(), cachedKeySet(), cachedValueCollection(),
                                                           cachedConstEntrySet(), cachedConstKeySet(), cachedConstValueCollection() {
            copy(source);
        }

        /**
         * Copy constructor - copies the content of the given map into this
         * one.
         * @param source The source map.
         */
        ConcurrentStlMap(const Map<K, V>& source) : ConcurrentMap<K, V>(), valueMap(), mutex(), modCount(0),
                                                    cachedEntrySet(), cachedKeySet(), cachedValueCollection(),
                                                    cachedConstEntrySet(), cachedConstKeySet(), cachedConstValueCollection() {
            copy(source);
        }

        virtual ~ConcurrentStlMap() {}

        /**
         * {@inheritDoc}
         */
        virtual bool equals(const ConcurrentStlMap& source) const {
            synchronized(&mutex) {
                return this->valueMap == source.valueMap;
            }

            return false;
        }

        virtual bool equals(const Map<K, V>& source) const {
            synchronized(&mutex) {
                typename std::auto_ptr< Iterator<K> > iterator(this->keySet().iterator());
                while (iterator->hasNext()) {
                    K key = iterator->next();
                    if (!this->containsKey(key)) {
                        return false;
                    }

                    if (!(this->get(key) == source.get(key))) {
                        return false;
                    }
                }
            }

            return true;
        }

        /**
         * {@inheritDoc}
         */
        virtual void copy(const ConcurrentStlMap& source) {
            synchronized(&mutex) {
                this->valueMap.clear();
                this->valueMap.insert(source.valueMap.begin(), source.valueMap.end());
            }
        }

        virtual void copy(const Map<K, V>& source) {
            synchronized( &mutex ) {
                this->clear();
                this->putAll(source);
            }
        }

        /**
         * {@inheritDoc}
         */
        virtual void clear() {
            synchronized(&mutex) {
                valueMap.clear();
            }
        }

        /**
         * {@inheritDoc}
         */
        virtual bool containsKey(const K& key) const {
            typename std::map<K, V, COMPARATOR>::const_iterator iter;

            synchronized(&mutex) {
                iter = valueMap.find(key);
                return iter != valueMap.end();
            }

            return false;
        }

        /**
         * {@inheritDoc}
         */
        virtual bool containsValue(const V& value) const {
            synchronized(&mutex) {
                if (valueMap.empty()) {
                    return false;
                }

                typename std::map<K, V, COMPARATOR>::const_iterator iter = valueMap.begin();
                for (; iter != valueMap.end(); ++iter) {
                    if ((*iter).second == value) {
                        return true;
                    }
                }
            }

            return false;
        }

        /**
         * {@inheritDoc}
         */
        virtual bool isEmpty() const {
            synchronized(&mutex) {
                return valueMap.empty();
            }

            return true;
        }

        /**
         * {@inheritDoc}
         */
        virtual int size() const {
            synchronized(&mutex) {
                return (int)valueMap.size();
            }

            return 0;
        }

        /**
         * {@inheritDoc}
         */
        virtual V& get(const K& key) {
            typename std::map<K,V,COMPARATOR>::iterator iter;
            synchronized(&mutex) {
                iter = valueMap.find(key);
                if (iter != valueMap.end()) {
                    return iter->second;
                }
            }

            throw NoSuchElementException(
                __FILE__, __LINE__, "Key does not exist in map");
        }

        /**
         * {@inheritDoc}
         */
        virtual const V& get(const K& key) const {
            typename std::map<K,V,COMPARATOR>::const_iterator iter;
            synchronized(&mutex) {
                iter = valueMap.find(key);
                if (iter != valueMap.end()) {
                    return iter->second;
                }
            }

            throw NoSuchElementException(
                __FILE__, __LINE__, "Key does not exist in map");
        }

        /**
         * {@inheritDoc}
         */
        virtual bool put(const K& key, const V& value) {
            bool result = false;
            synchronized(&mutex) {
                if (this->containsKey(key)) {
                    result = true;
                }
                modCount++;
                valueMap[key] = value;
            }
            return result;
        }

        /**
         * {@inheritDoc}
         */
        virtual bool put(const K& key, const V& value, V& oldValue) {
            bool result = false;
            synchronized(&mutex) {
                if (this->containsKey(key)) {
                    result = true;
                    oldValue = valueMap[key];
                }
                modCount++;
                valueMap[key] = value;
            }
            return result;
        }

        /**
         * {@inheritDoc}
         */
        virtual void putAll(const ConcurrentStlMap<K, V, COMPARATOR>& other) {
            synchronized(&mutex) {
                this->valueMap.insert(other.valueMap.begin(), other.valueMap.end());
                this->modCount++;
            }
        }

        /**
         * {@inheritDoc}
         */
        virtual void putAll(const Map<K, V>& other) {
            synchronized(&mutex) {
                typename std::auto_ptr< Iterator<K> > iterator(other.keySet().iterator());
                while (iterator->hasNext()) {
                    K key = iterator->next();
                    this->put(key, other.get(key));
                }
                modCount++;
            }
        }

        /**
         * {@inheritDoc}
         */
        virtual V remove(const K& key) {
            V result = V();
            synchronized(&mutex) {
                typename std::map<K, V, COMPARATOR>::iterator iter = valueMap.find(key);
                if (iter == valueMap.end()) {
                    return result;
                }
                result = iter->second;
                valueMap.erase(iter);
                modCount++;
            }

            return result;
        }

        /**
         * If the specified key is not already associated with a value, associate it with
         * the given value. This is equivalent to
         * <pre>
         *     if( !map.containsKey( key ) ) {
         *         map.put( key, value );
         *         return true;
         *     } else {
         *         return false;
         *     }
         * </pre>
         * except that the action is performed atomically.
         *
         * @param key
         *        The key to map the value to.
         * @param value
         *        The value to map to the given key.
         *
         * @return true if the put operation was performed otherwise return false
         *         which indicates there was a value previously mapped to the key.
         * @throw UnsupportedOperationException
         *        if the put operation is not supported by this map
         */
        bool putIfAbsent(const K& key, const V& value) {
            synchronized(&mutex) {
                if (!this->containsKey(key)) {
                    this->put(key, value);
                    return true;
                }
            }

            return false;
        }

        /**
         * Remove entry for key only if currently mapped to given value.
         * Acts as
         * <pre>
         * if( map.containsKey( key ) && ( map.get( key ) == value ) ) {
         *     map.remove( key );
         *     return true;
         * } else {
         *     return false;
         * }
         * </pre>
         * except that the action is performed atomically.
         *
         * @param key key with which the specified value is associated.
         * @param value value associated with the specified key.
         *
         * @return true if the value was removed, false otherwise
         */
        bool remove(const K& key, const V& value) {
            synchronized(&mutex) {
                if( this->containsKey( key ) && ( this->get( key ) == value ) ) {
                    this->remove(key);
                    return true;
                }
            }

            return false;
        }

        /**
         * Replace entry for key only if currently mapped to given value.
         * Acts as
         * <pre>
         * if( map.containsKey( key ) && ( map.get( key ) == oldValue ) ) {
         *     map.put( key, newValue );
         *     return true;
         * } else {
         *     return false;
         * }
         * </pre>
         * except that the action is performed atomically.
         *
         * @param key key with which the specified value is associated.
         * @param oldValue value expected to be associated with the specified key.
         * @param newValue value to be associated with the specified key.
         *
         * @return true if the value was replaced
         */
        bool replace(const K& key, const V& oldValue, const V& newValue) {
            synchronized(&mutex) {
                if (this->containsKey(key) && (this->get(key) == oldValue)) {
                    this->put(key, newValue);
                    return true;
                }
            }

            return false;
        }

        /**
         * Replace entry for key only if currently mapped to some value.
         * Acts as
         * <pre>
         * if( map.containsKey( key ) ) {
         *     return map.put( key, value );
         * } else {
         *     throw NoSuchElementException(...);
         * };
         * </pre>
         * except that the action is performed atomically.
         *
         * @param key key with which the specified value is associated.
         * @param value value to be associated with the specified key.
         *
         * @return copy of the previous value associated with specified key, or
         *         throws an NoSuchElementException if there was no mapping for key.
         *
         * @throws NoSuchElementException if there was no previous mapping.
         */
        V replace(const K& key, const V& value) {
            synchronized(&mutex) {
                if (this->containsKey(key)) {
                    V result = this->get(key);
                    this->put(key, value);
                    return result;
                }
            }

            throw NoSuchElementException(
                __FILE__, __LINE__, "Value to Replace was not in the Map." );
        }

        virtual Set< MapEntry<K, V> >& entrySet() {
            synchronized(&mutex) {
                if (this->cachedEntrySet == NULL) {
                    this->cachedEntrySet.reset(new StlMapEntrySet(this));
                }
            }
            return *(this->cachedEntrySet);
        }
        virtual const Set< MapEntry<K, V> >& entrySet() const {
            synchronized(&mutex) {
                if (this->cachedConstEntrySet == NULL) {
                    this->cachedConstEntrySet.reset(new ConstStlMapEntrySet(this));
                }
            }
            return *(this->cachedConstEntrySet);
        }

        virtual Set<K>& keySet() {
            synchronized(&mutex) {
                if (this->cachedKeySet == NULL) {
                    this->cachedKeySet.reset(new StlMapKeySet(this));
                }
            }
            return *(this->cachedKeySet);
        }

        virtual const Set<K>& keySet() const {
            synchronized(&mutex) {
                if (this->cachedConstKeySet == NULL) {
                    this->cachedConstKeySet.reset(new ConstStlMapKeySet(this));
                }
            }
            return *(this->cachedConstKeySet);
        }

        virtual Collection<V>& values() {
            synchronized(&mutex) {
                if (this->cachedValueCollection == NULL) {
                    this->cachedValueCollection.reset(new StlMapValueCollection(this));
                }
            }
            return *(this->cachedValueCollection);
        }

        virtual const Collection<V>& values() const {
            synchronized(&mutex) {
                if (this->cachedConstValueCollection == NULL) {
                    this->cachedConstValueCollection.reset(new ConstStlMapValueCollection(this));
                }
            }
            return *(this->cachedConstValueCollection);
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

        virtual void wait() {
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

}}}

#endif /*_DECAF_UTIL_CONCURRENTSTLMAP_H_*/
