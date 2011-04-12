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

#ifndef _DECAF_UTIL_CONCURRENT_CONCURRENTMAP_H_
#define _DECAF_UTIL_CONCURRENT_CONCURRENTMAP_H_

#include <decaf/util/Config.h>
#include <decaf/util/Map.h>

#include <decaf/lang/exceptions/UnsupportedOperationException.h>
#include <decaf/lang/exceptions/IllegalStateException.h>
#include <decaf/util/NoSuchElementException.h>

namespace decaf {
namespace util {
namespace concurrent {

    /**
     * Interface for a Map type that provides additional atomic putIfAbsent, remove,
     * and replace methods alongside the already available Map interface.
     *
     * @since 1.0
     */
    template<typename K, typename V, typename COMPARATOR>
    class ConcurrentMap : public Map<K, V, COMPARATOR>{
    public:

        virtual ~ConcurrentMap() {}

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
        virtual bool putIfAbsent( const K& key, const V& value ) = 0;

        /**
         * Remove entry for key only if currently mapped to given value.
         * Acts as
         * <pre>
         * if( ( map.containsKey( key ) && ( map.get( key ) == value ) ) ) {
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
        virtual bool remove( const K& key, const V& value ) = 0;

        /**
         * Replace entry for key only if currently mapped to given value.
         * Acts as
         * <pre>
         * if( ( map.containsKey( key ) && ( map.get( key ) == oldValue  ) ) {
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
        virtual bool replace( const K& key, const V& oldValue, const V& newValue ) = 0;

        /**
         * Replace entry for key only if currently mapped to some value.
         * Acts as
         * <pre>
         * if( ( map.containsKey( key ) ) {
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
        virtual V replace( const K& key, const V& value ) = 0;

    };

}}}

#endif /* _DECAF_UTIL_CONCURRENT_CONCURRENTMAP_H_ */
