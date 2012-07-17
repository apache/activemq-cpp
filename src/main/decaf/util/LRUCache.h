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

#include <decaf/util/AbstractMap.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>

namespace decaf {
namespace util {

    /**
     * A Basic Least Recently Used (LRU) Cache Map.
     *
     * @since 1.0
     */
    template<typename K, typename V, typename COMPARATOR = std::less<K> >
    class LRUCache : decaf::util::AbstractMap<K, V> {
    protected:

        int maxCacheSize;

    public:

        LRUCache() : maxCacheSize(1024) {}

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

        virtual bool removeEldestEntry(const typename Map<K,V>::Entry & entry) {
            return false;
        }

    };

}}

#endif /* _DECAF_UTIL_LRUCACHE_H_ */
