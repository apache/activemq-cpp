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

#ifndef _DECAF_UTIL_LRUCACHE_H_
#define _DECAF_UTIL_LRUCACHE_H_

#include <decaf/util/Config.h>

#include <decaf/util/LinkedHashMap.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>

namespace decaf {
namespace util {

    /**
     * A Basic Least Recently Used (LRU) Cache Map.
     *
     * This LRUCache implements the LinkedHashMap class so all the standard Map
     * operations are provided.  When the sive of this LRUCache map exceeds the
     * specified maxCacheSize value then by default the oldest entry is evicted
     * from the Cache.
     *
     * Subclasses can override the LinkedHashMap::onEviction method to perform
     * custom cache eviction processing.
     *
     * @since 1.0
     */
    template<typename K, typename V, typename HASHCODE = HashCode<K> >
    class LRUCache : public LinkedHashMap<K, V, HASHCODE> {
    protected:

        int maxCacheSize;

    public:


        /**
         * Default constructor for an LRU Cache The default capacity is 10000
         */
        LRUCache() : LinkedHashMap<K, V, HASHCODE>(0, 0.75f, true), maxCacheSize(10000) {}

        /**
         * Constructs a LRUCache with a maximum capacity
         *
         * @param maximumCacheSize
         *      The maximum number of cached entries before eviction begins.
         */
        LRUCache(int maximumCacheSize) :
            LinkedHashMap<K, V, HASHCODE>(0, 0.75f, true), maxCacheSize(maximumCacheSize) {

            if (maximumCacheSize <= 0) {
                throw decaf::lang::exceptions::IllegalArgumentException(
                        __FILE__, __LINE__, "Cache size must be greater than zero.");
            }
        }

        /**
         * Constructs an empty LRUCache instance with the specified initial capacity,
         * maximumCacheSize, load factor and ordering mode.
         *
         * @param initialCapacity
         *      The initial capacity of the LRUCache.
         * @param maximumCacheSize
         *      The maximum number of cached entries before eviction begins.
         * @param loadFactor the load factor.
         *      The initial load factor for this LRUCache.
         * @param accessOrder
         *      The ordering mode - true for access-order, false for insertion-order.
         *
         * @throws IllegalArgumentException if the initial capacity is negative or
         *                 the load factor is non-positive.
         */
        LRUCache(int initialCapacity, int maximumCacheSize, float loadFactor, bool accessOrder) :
            LinkedHashMap<K, V, HASHCODE>(initialCapacity, loadFactor, accessOrder), maxCacheSize(maximumCacheSize) {

            if (maximumCacheSize <= 0) {
                throw decaf::lang::exceptions::IllegalArgumentException(
                        __FILE__, __LINE__, "Cache size must be greater than zero.");
            }
        }

        virtual ~LRUCache() {}

        /**
         * Gets the currently configured Max Cache Size setting.
         *
         * @returns the current max cache size value.
         */
        int getMaxCacheSize() const {
            return maxCacheSize;
        }

        /**
         * Sets the maximum size allowed for this LRU Cache.
         *
         * @param size
         * 		The new maximum cache size setting.
         *
         * @throws IllegalArgumentException is size is less than or equal to zero.
         */
        void setMaxCacheSize(int size) {
            if (size <= 0) {
                throw decaf::lang::exceptions::IllegalArgumentException(
                        __FILE__, __LINE__, "Cache size must be greater than zero.");
            }

            this->maxCacheSize = size;
        }

    protected:

        virtual bool removeEldestEntry(const MapEntry<K, V>& eldest DECAF_UNUSED) {
            if (this->size() > maxCacheSize) {
                return true;
            }
            return false;
        }

    };

}}

#endif /* _DECAF_UTIL_LRUCACHE_H_ */
