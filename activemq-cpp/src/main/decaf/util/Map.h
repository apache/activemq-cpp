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

#include <functional>
#include <vector>
#include <decaf/lang/exceptions/UnsupportedOperationException.h>
#include <decaf/util/NoSuchElementException.h>
#include <decaf/util/concurrent/Synchronizable.h>
#include <decaf/util/concurrent/Mutex.h>
#include <decaf/util/Set.h>
#include <decaf/util/MapEntry.h>

namespace decaf{
namespace util{

    /**
     * An object that maps keys to values. A map cannot contain duplicate keys; each key can map
     * to at most one value.
     *
     * The Map interface provides three collection views, which allow a map's contents to be viewed
     * as a set of keys, collection of values, or set of key-value mappings. The order of a map is
     * defined as the order in which the iterators on the map's collection views return their
     * elements. Some map implementations, like the TreeMap class, make specific guarantees as to
     * their order; others, like the HashMap class, do not.
     *
     * Note: great care must be exercised if mutable objects are used as map keys. The behavior
     * of a map is not specified if the value of an object is changed in a manner that affects
     * equals comparisons while the object is a key in the map. A special case of this prohibition
     * is that it is not permissible for a map to contain itself as a key. While it is permissible
     * for a map to contain itself as a value, extreme caution is advised: the equals and hashCode
     * methods are no longer well defined on such a map.
     *
     * All general-purpose map implementation classes should provide two "standard" constructors:
     * a void (no arguments) constructor which creates an empty map, and a constructor with a
     * single argument of type Map, which creates a new map with the same key-value mappings as
     * its argument. In effect, the latter constructor allows the user to copy any map, producing
     * an equivalent map of the desired class.
     *
     * The "destructive" methods contained in this interface, that is, the methods that modify the
     * map on which they operate, are specified to throw UnsupportedOperationException if this map
     * does not support the operation. If this is the case, these methods may, but are not required
     * to, throw an UnsupportedOperationException if the invocation would have no effect on the map.
     * For example, invoking the putAll(Map) method on an unmodifiable map may, but is not required
     * to, throw the exception if the map whose mappings are to be "superimposed" is empty.
     *
     * Some map implementations have restrictions on the keys and values they may contain. For
     * example, some implementations prohibit NULL keys and values, and some have restrictions on
     * the types of their keys. Attempting to insert an ineligible key or value throws an exception,
     * typically NullPointerException or ClassCastException. Attempting to query the presence of an
     * ineligible key or value may throw an exception, or it may simply return false; some
     * implementations will exhibit the former behavior and some will exhibit the latter. More
     * generally, attempting an operation on an ineligible key or value whose completion would not
     * result in the insertion of an ineligible element into the map may throw an exception or it
     * may succeed, at the option of the implementation. Such exceptions are marked as "optional"
     * in the specification for this interface.
     *
     * Many methods in Collections Framework interfaces are defined in terms of the equals method.
     * For example, the specification for the containsKey(Object key) method says: "returns true if
     * and only if this map contains a mapping for a key k such that (key == k)." This specification
     * should not be construed to imply that invoking Map.containsKey with a non-null argument key
     * will cause (key == k) to be invoked for any key k. Implementations are free to implement
     * optimizations whereby the equals invocation is avoided, for example, by first comparing the
     * hash codes of the two keys. (The Object.hashCode() specification guarantees that two objects
     * with unequal hash codes cannot be equal.) More generally, implementations of the various
     * Collections Framework interfaces are free to take advantage of the specified behavior of
     * underlying Object methods wherever the implementor deems it appropriate.
     *
     * @since 1.0
     */
    template <typename K, typename V>
    class Map : public concurrent::Synchronizable {
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
        virtual bool equals(const Map& source) const = 0;

        /**
         * Copies the content of the source map into this map.  Erases
         * all existing data in this map.
         * @param source The source object to copy from.
         */
        virtual void copy(const Map& source) = 0;

        /**
         * Removes all keys and values from this map.
         * @throw UnsupportedOperationException if this map is unmodifiable.
         */
        virtual void clear() = 0;

        /**
         * Indicates whether or this map contains a value for the
         * given key.
         * @param key The key to look up.
         * @return true if this map contains the value, otherwise false.
         */
        virtual bool containsKey(const K& key) const = 0;

        /**
         * Indicates whether or this map contains a value for the
         * given value, i.e. they are equal, this is done by operator==
         * so the types must pass equivalence testing in this manner.
         * @param value The Value to look up.
         * @return true if this map contains the value, otherwise false.
         */
        virtual bool containsValue(const V& value) const = 0;

        /**
         * @return if the Map contains any element or not, TRUE or FALSE
         */
        virtual bool isEmpty() const = 0;

        /**
         * @return The number of elements (key/value pairs) in this map.
         */
        virtual int size() const = 0;

        /**
         * Gets the value mapped to the specified key in the Map.  If there is no
         * element in the map whose key is equivalent to the key provided then a
         * NoSuchElementException is thrown.
         *
         * @param key The search key.
         * @return A reference to the value for the given key.
         *
         * @throws NoSuchElementException if the key requests doesn't exist in the Map.
         */
        virtual V& get(const K& key) = 0;

        /**
         * Gets the value mapped to the specified key in the Map.  If there is no
         * element in the map whose key is equivalent to the key provided then a
         * NoSuchElementException is thrown.
         *
         * @param key The search key.
         * @return A {const} reference to the value for the given key.
         *
         * @throws NoSuchElementException if the key requests doesn't exist in the Map.
         */
        virtual const V& get(const K& key) const = 0;

        /**
         * Sets the value for the specified key.
         * @param key The target key.
         * @param value The value to be set.
         *
         * @throw UnsupportedOperationException if this map is unmodifiable.
         */
        virtual void put(const K& key, const V& value) = 0;

        /**
         * Stores a copy of the Mappings contained in the other Map in this one.
         *
         * @param other
         *      A Map instance whose elements are to all be inserted in this Map.
         *
         * @throws UnsupportedOperationException
         *      If the implementing class does not support the putAll operation.
         */
        virtual void putAll(const Map<K, V>& other) = 0;

        /**
         * Removes the value (key/value pair) for the specified key from
         * the map, returns a copy of the value that was mapped to the key.
         *
         * @param key The search key.
         * @return a copy of the element that was previously mapped to the given key
         *
         * @throw NoSuchElementException if this key is not in the Map.
         * @throw UnsupportedOperationException if this map is unmodifiable.
         */
        virtual V remove(const K& key) = 0;

        /**
         * Returns a Set view of the mappings contained in this map. The set is backed by the
         * map, so changes to the map are reflected in the set, and vice-versa. If the map is
         * modified while an iteration over the set is in progress (except through the iterator's
         * own remove operation, or through the setValue operation on a map entry returned by
         * the iterator) the results of the iteration are undefined. The set supports element
         * removal, which removes the corresponding mapping from the map, via the
         * Iterator.remove, Set.remove, removeAll, retainAll and clear operations. It does not
         * support the add or addAll operations.
         *
         * @returns a new pointer to a Set<MapEntry<K,V>> that is backed by this Map, the returned
         *          pointer is owned by the caller.
         */
        virtual Set< MapEntry<K,V> >* entrySet() = 0;
        virtual Set< MapEntry<K,V> >* entrySet() const = 0;

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
