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
     *
     * @since 1.0
     */
    template <typename K, typename V, typename COMPARATOR = std::less<K> >
    class ConcurrentStlMap : public ConcurrentMap<K, V, COMPARATOR> {
    private:

        std::map<K,V,COMPARATOR> valueMap;
        mutable concurrent::Mutex mutex;

    public:

        /**
         * Default constructor - does nothing.
         */
        ConcurrentStlMap() : ConcurrentMap<K,V,COMPARATOR>(), valueMap(), mutex() {}

        /**
         * Copy constructor - copies the content of the given map into this
         * one.
         * @param source The source map.
         */
        ConcurrentStlMap( const ConcurrentStlMap& source ) :
            ConcurrentMap<K,V,COMPARATOR>(), valueMap(), mutex() {

            copy( source );
        }

        /**
         * Copy constructor - copies the content of the given map into this
         * one.
         * @param source The source map.
         */
        ConcurrentStlMap( const Map<K,V,COMPARATOR>& source ) :
            ConcurrentMap<K,V,COMPARATOR>(), valueMap(), mutex() {

            copy( source );
        }

        virtual ~ConcurrentStlMap() {}

        /**
         * {@inheritDoc}
         */
        virtual bool equals( const ConcurrentStlMap& source ) const {
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
         * {@inheritDoc}
         */
        virtual void copy( const ConcurrentStlMap& source ) {
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
         * {@inheritDoc}
         */
        virtual void clear() throw( decaf::lang::exceptions::UnsupportedOperationException ) {
            synchronized( &mutex ) {
                valueMap.clear();
            }
        }

        /**
         * {@inheritDoc}
         */
        virtual bool containsKey( const K& key ) const {
            typename std::map<K,V,COMPARATOR>::const_iterator iter;

            synchronized( &mutex ) {
                iter = valueMap.find(key);
                return iter != valueMap.end();
            }

            return false;
        }

        /**
         * {@inheritDoc}
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
         * {@inheritDoc}
         */
        virtual bool isEmpty() const {
            synchronized( &mutex ) {
                return valueMap.empty();
            }

            return true;
        }

        /**
         * {@inheritDoc}
         */
        virtual int size() const {
            synchronized( &mutex ) {
                return (int)valueMap.size();
            }

            return 0;
        }

        /**
         * {@inheritDoc}
         */
        virtual V& get( const K& key ) {

            typename std::map<K,V,COMPARATOR>::iterator iter;

            synchronized( &mutex ) {
                iter = valueMap.find( key );
                if( iter != valueMap.end() ){
                    return iter->second;
                }
            }

            throw NoSuchElementException(
                __FILE__, __LINE__, "Key does not exist in map" );
        }

        /**
         * {@inheritDoc}
         */
        virtual const V& get( const K& key ) const {

            typename std::map<K,V,COMPARATOR>::const_iterator iter;

            synchronized( &mutex ) {
                iter = valueMap.find( key );
                if( iter != valueMap.end() ){
                    return iter->second;
                }
            }

            throw NoSuchElementException(
                __FILE__, __LINE__, "Key does not exist in map" );
        }

        /**
         * {@inheritDoc}
         */
        virtual void put( const K& key, const V& value ) {

            synchronized( &mutex ) {
                valueMap[key] = value;
            }
        }

        /**
         * {@inheritDoc}
         */
        virtual void putAll( const ConcurrentStlMap<K,V,COMPARATOR>& other ) {

            synchronized( &mutex ) {
                this->valueMap.insert( other.valueMap.begin(), other.valueMap.end() );
            }
        }

        /**
         * {@inheritDoc}
         */
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
         * {@inheritDoc}
         */
        virtual V remove( const K& key ) {

            V result;

            synchronized( &mutex ) {
                typename std::map<K,V,COMPARATOR>::iterator iter = valueMap.find( key );
                if( iter == valueMap.end() ) {
                    throw NoSuchElementException(
                        __FILE__, __LINE__, "Key is not present in this Map." );
                }

                result = iter->second;
                valueMap.erase( iter );
            }

            return result;
        }

        /**
         * {@inheritDoc}
         */
        virtual std::vector<K> keySet() const {

            std::vector<K> keys( valueMap.size() );
            synchronized( &mutex ) {

                typename std::map<K,V,COMPARATOR>::const_iterator iter;
                iter=valueMap.begin();
                for( int ix=0; iter != valueMap.end(); ++iter, ++ix ){
                    keys[ix] = iter->first;
                }
            }

            return keys;
        }

        /**
         * {@inheritDoc}
         */
        virtual std::vector<V> values() const {

            std::vector<V> values( valueMap.size() );
            synchronized( &mutex ) {

                typename std::map<K,V,COMPARATOR>::const_iterator iter;
                iter=valueMap.begin();
                for( int ix=0; iter != valueMap.end(); ++iter, ++ix ){
                    values[ix] = iter->second;
                }

            }
            return values;
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
        bool putIfAbsent( const K& key, const V& value ) {

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
        V replace( const K& key, const V& value ) {

            synchronized( &mutex ) {
                if( this->containsKey( key ) ) {
                    V result = this->get( key );
                    this->put( key, value );
                    return result;
                }
            }

            throw NoSuchElementException(
                __FILE__, __LINE__, "Value to Replace was not in the Map." );
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
