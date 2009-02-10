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
#include <decaf/lang/exceptions/NoSuchElementException.h>
#include <decaf/util/concurrent/Synchronizable.h>
#include <decaf/util/concurrent/ConcurrentMap.h>
#include <decaf/util/concurrent/Mutex.h>
#include <decaf/util/Map.h>

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
     */
    template <typename K, typename V, typename COMPARATOR = std::less<K> >
    class ConcurrentSTLMap : public ConcurrentMap<K, V, COMPARATOR> {
    private:

        std::map<K,V,COMPARATOR> valueMap;
        mutable concurrent::Mutex mutex;

    public:

        /**
         * Default constructor - does nothing.
         */
        ConcurrentSTLMap() : ConcurrentMap<K,V,COMPARATOR>() {}

        /**
         * Copy constructor - copies the content of the given map into this
         * one.
         * @param source The source map.
         */
        ConcurrentSTLMap( const ConcurrentSTLMap& source ) : ConcurrentMap<K,V,COMPARATOR>() {
            copy( source );
        }

        /**
         * Copy constructor - copies the content of the given map into this
         * one.
         * @param source The source map.
         */
        ConcurrentSTLMap( const Map<K,V,COMPARATOR>& source ) : ConcurrentMap<K,V,COMPARATOR>() {
            copy( source );
        }

        virtual ~ConcurrentSTLMap() {}

        /**
         * Comparison, equality is dependent on the method of determining
         * if the element are equal.
         * @param source - Map to compare to this one.
         * @returns true if the Map passed is equal in value to this one.
         */
        virtual bool equals( const ConcurrentSTLMap& source ) const {
            synchronized( &mutex ) {
                return this->valueMap == source.valueMap;
            }

            return false;
        }

        virtual bool equals( const Map<K,V,COMPARATOR>& source ) const {

            synchronized( &mutex ) {
                std::vector<K> keys = source.keySet();

                typename std::vector<K>::const_iterator iter = keys.begin();
                for( ; iter != keys.end(); ++iter ) {
                    if( !this->containsKey( *iter ) ) {
                        return false;
                    }

                    if( !( this->get( *iter ) == source.get( *iter ) ) ) {
                        return false;
                    }
                }
            }

            return true;
        }

        /**
         * Copies the content of the source map into this map.  Erases
         * all existing data in this map.
         * @param source The source object to copy from.
         */
        virtual void copy( const ConcurrentSTLMap& source ) {
            synchronized( &mutex ) {
                this->valueMap.clear();
                this->valueMap.insert( source.valueMap.begin(), source.valueMap.end() );
            }
        }

        virtual void copy( const Map<K,V,COMPARATOR>& source ) {
            synchronized( &mutex ) {
                this->clear();
                this->putAll( source );
            }
        }

        /**
         * Removes all keys and values from this map.
         */
        virtual void clear() {
            synchronized( &mutex ) {
                valueMap.clear();
            }
        }

        /**
         * Indicates whether or this map contains a value for the
         * given key.
         * @param key The key to look up.
         * @return true if this map contains the value, otherwise false.
         */
        virtual bool containsKey( const K& key ) const {
            typename std::map<K,V,COMPARATOR>::const_iterator iter;

            synchronized( &mutex ) {
                iter = valueMap.find(key);
                return iter != valueMap.end();
            }
        }

        /**
         * Indicates whether or this map contains a value for the
         * given value, i.e. they are equal, this is done by operator==
         * so the types must pass equivalence testing in this manner.
         * @param value The Value to look up.
         * @return true if this map contains the value, otherwise false.
         */
        virtual bool containsValue( const V& value ) const {

            synchronized( &mutex ) {

                if( valueMap.empty() ){
                    return false;
                }

                typename std::map<K,V,COMPARATOR>::const_iterator iter = valueMap.begin();
                for( ; iter != valueMap.end(); ++iter ){
                    if( (*iter).second == value ) {
                        return true;
                    }
                }
            }

            return false;
        }

        /**
         * @return if the Map contains any element or not, TRUE or FALSE
         */
        virtual bool isEmpty() const {
            synchronized( &mutex ) {
                return valueMap.empty();
            }

            return true;
        }

        /**
         * @return The number of elements (key/value pairs) in this map.
         */
        virtual std::size_t size() const {
            synchronized( &mutex ) {
                return valueMap.size();
            }

            return 0;
        }

        /**
         * Gets the value for the specified key.
         * @param key The search key.
         * @return The value for the given key.
         * @throws NoSuchElementException
         */
        virtual V get( const K& key ) const
            throw( lang::exceptions::NoSuchElementException ) {

            typename std::map<K,V,COMPARATOR>::const_iterator iter;

            synchronized( &mutex ) {
                iter = valueMap.find(key);
                if( iter == valueMap.end() ){
                    throw lang::exceptions::NoSuchElementException(
                        __FILE__, __LINE__, "Key does not exist in map" );
                }

                return iter->second;
            }
        }

        /**
         * Sets the value for the specified key.
         * @param key The target key.
         * @param value The value to be set.
         */
        virtual void put( const K& key, V value ) {
            synchronized( &mutex ) {
                valueMap[key] = value;
            }
        }

        /**
         * Stores a copy of the Mappings contained in the other Map in this one.
         * @param key The target key.
         * @param value The value to be set.
         */
        virtual void putAll( const ConcurrentSTLMap<K,V,COMPARATOR>& other ) {
            synchronized( &mutex ) {
                this->valueMap.insert( other.valueMap.begin(), other.valueMap.end() );
            }
        }
        virtual void putAll( const Map<K,V,COMPARATOR>& other ) {
            synchronized( &mutex ) {
                std::vector<K> keys = other.keySet();

                typename std::vector<K>::const_iterator iter = keys.begin();
                for( ; iter != keys.end(); ++iter ) {
                    this->put( *iter, other.get( *iter ) );
                }
            }
        }

        /**
         * Removes the value (key/value pair) for the specified key from
         * the map.
         * @param key The search key.
         */
        virtual void remove( const K& key ) {
            synchronized( &mutex ) {
                valueMap.erase( key );
            }
        }

        /**
         * @return the entire set of keys in this map as a std::vector.
         */
        virtual std::vector<K> keySet() const {
            synchronized( &mutex ) {
                std::vector<K> keys( valueMap.size() );

                typename std::map<K,V,COMPARATOR>::const_iterator iter;
                iter=valueMap.begin();
                for( int ix=0; iter != valueMap.end(); ++iter, ++ix ){
                    keys[ix] = iter->first;
                }

                return keys;
            }
        }

        /**
         * @return the entire set of values in this map as a std::vector.
         */
        virtual std::vector<V> values() const {
            synchronized( &mutex ) {
                std::vector<V> values( valueMap.size() );

                typename std::map<K,V,COMPARATOR>::const_iterator iter;
                iter=valueMap.begin();
                for( int ix=0; iter != valueMap.end(); ++iter, ++ix ){
                    values[ix] = iter->second;
                }

                return values;
            }
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
        bool putIfAbsent( const K& key, const V& value )
            throw( decaf::lang::exceptions::UnsupportedOperationException ) {

            synchronized( &mutex ) {
                if( !this->containsKey( key ) ) {
                    this->put( key, value );
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
        bool remove( const K& key, const V& value ) {
            synchronized( &mutex ) {
                if( this->containsKey( key ) && ( this->get( key ) == value ) ) {
                    this->remove( key );
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
        bool replace( const K& key, const V& oldValue, const V& newValue ) {
            synchronized( &mutex ) {
                if( this->containsKey( key ) && ( this->get( key ) == oldValue ) ) {
                    this->put( key, newValue );
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
        V replace( const K& key, const V& value )
            throw( decaf::lang::exceptions::NoSuchElementException ) {

            synchronized( &mutex ) {
                if( this->containsKey( key ) ) {
                    V result = this->get( key );
                    this->put( key, value );
                    return result;
                }
            }

            throw decaf::lang::exceptions::NoSuchElementException(
                __FILE__, __LINE__, "Value to Replace was not in the Map." );
        }

    public:     // Methods from Synchronizable

        /**
         * Locks the object.
         * @throws ActiveMQException
         */
        virtual void lock() throw( lang::Exception ) {
            mutex.lock();
        }

        /**
         * Unlocks the object.
         * @throws ActiveMQException
         */
        virtual void unlock() throw( lang::Exception ) {
            mutex.unlock();
        }

        /**
         * Waits on a signal from this object, which is generated
         * by a call to Notify.  Must have this object locked before
         * calling.
         * @throws ActiveMQException
         */
        virtual void wait() throw( lang::Exception ) {
            mutex.wait();
        }

        /**
         * Waits on a signal from this object, which is generated
         * by a call to Notify.  Must have this object locked before
         * calling.  This wait will timeout after the specified time
         * interval.
         * @param millisecs the time in milliseconds to wait, or
         * WAIT_INIFINITE
         * @throws ActiveMQException
         */
        virtual void wait( unsigned long millisecs ) throw( lang::Exception ) {
            mutex.wait(millisecs);
        }

        /**
         * Signals a waiter on this object that it can now wake
         * up and continue.  Must have this object locked before
         * calling.
         * @throws ActiveMQException
         */
        virtual void notify() throw( lang::Exception ) {
            mutex.notify();
        }

        /**
         * Signals the waiters on this object that it can now wake
         * up and continue.  Must have this object locked before
         * calling.
         * @throws ActiveMQException
         */
        virtual void notifyAll() throw( lang::Exception ) {
            mutex.notifyAll();
        }
    };

}}}

#endif /*_DECAF_UTIL_CONCURRENTSTLMAP_H_*/
