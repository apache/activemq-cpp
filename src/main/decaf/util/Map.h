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

#ifndef _DECAF_UTIL_MAP_H_
#define _DECAF_UTIL_MAP_H_

#include <vector>
#include <decaf/lang/exceptions/NoSuchElementException.h>
#include <decaf/util/concurrent/Synchronizable.h>
#include <decaf/util/concurrent/Mutex.h>

namespace decaf{
namespace util{

    /**
     * Map template that wraps around a std::map to provide
     * a more user-friendly interface and to provide common
     * functions that do not exist in std::map.
     */
    template <typename K, typename V, typename COMPARATOR = std::less<K> > class Map :
        public concurrent::Synchronizable {
    public:

        template< typename K1, typename V1>
        class Entry {
        private:

            K1 key;
            V1 value;

        public:

            Entry() {}
            virtual ~Entry() {}

            const K1& getKey() const;

            const V1& getValue() const;

            void setValue( const V1& value );

        };

    public:

        /**
         * Default constructor - does nothing.
         */
        Map() {}

        virtual ~Map() {}

        /**
         * Comparison, equality is dependent on the method of determining
         * if the element are equal.
         * @param source - Map to compare to this one.
         * @returns true if the Map passed is equal in value to this one.
         */
        virtual bool equals( const Map& source ) const = 0;

        /**
         * Copies the content of the source map into this map.  Erases
         * all existing data in this map.
         * @param source The source object to copy from.
         */
        virtual void copy( const Map& source ) = 0;

        /**
         * Removes all keys and values from this map.
         */
        virtual void clear() = 0;

        /**
         * Indicates whether or this map contains a value for the
         * given key.
         * @param key The key to look up.
         * @return true if this map contains the value, otherwise false.
         */
        virtual bool containsKey( const K& key ) const = 0;

        /**
         * Indicates whether or this map contains a value for the
         * given value, i.e. they are equal, this is done by operator==
         * so the types must pass equivalence testing in this manner.
         * @param value The Value to look up.
         * @return true if this map contains the value, otherwise false.
         */
        virtual bool containsValue( const V& value ) const = 0;

        /**
         * @return if the Map contains any element or not, TRUE or FALSE
         */
        virtual bool isEmpty() const = 0;

        /**
         * @return The number of elements (key/value pairs) in this map.
         */
        virtual std::size_t size() const = 0;

        /**
         * Gets the value for the specified key.
         * @param key The search key.
         * @return The value for the given key.
         * @throws NoSuchElementException
         */
        virtual V get( const K& key ) const
            throw( lang::exceptions::NoSuchElementException ) = 0;

        /**
         * Sets the value for the specified key.
         * @param key The target key.
         * @param value The value to be set.
         */
        virtual void put( const K& key, V value ) = 0;

        /**
         * Stores a copy of the Mappings contained in the other Map in this one.
         * @param key The target key.
         * @param value The value to be set.
         */
        virtual void putAll( const Map<K,V,COMPARATOR>& other ) = 0;

        /**
         * Removes the value (key/value pair) for the specified key from
         * the map.
         * @param key The search key.
         */
        virtual void remove( const K& key ) = 0;

        /**
         * @return the entire set of keys in this map as a std::vector.
         */
        virtual std::vector<K> keySet() const = 0;

        /**
         * @return the entire set of values in this map as a std::vector.
         */
        virtual std::vector<V> values() const = 0;

    };

}}

#endif /*_DECAF_UTIL_MAP_H_*/
