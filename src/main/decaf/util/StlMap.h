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

#ifndef _DECAF_UTIL_STLMAP_H_
#define _DECAF_UTIL_STLMAP_H_

#include <map>
#include <vector>
#include <decaf/util/NoSuchElementException.h>
#include <decaf/util/concurrent/Synchronizable.h>
#include <decaf/util/concurrent/Mutex.h>
#include <decaf/util/Map.h>

namespace decaf{
namespace util{

    /**
     * Map template that wraps around a std::map to provide
     * a more user-friendly interface and to provide common
     * functions that do not exist in std::map.
     *
     * @since 1.0
     */
    template <typename K, typename V, typename COMPARATOR = std::less<K> >
    class StlMap : public Map<K, V, COMPARATOR> {
    private:

        std::map<K,V,COMPARATOR> valueMap;
        mutable concurrent::Mutex mutex;

    public:

        /**
         * Default constructor - does nothing.
         */
        StlMap() : Map<K,V,COMPARATOR>(), valueMap(), mutex() {}

        /**
         * Copy constructor - copies the content of the given map into this
         * one.
         * @param source The source map.
         */
        StlMap( const StlMap& source ) : Map<K,V,COMPARATOR>(), valueMap(), mutex() {
            copy( source );
        }

        /**
         * Copy constructor - copies the content of the given map into this
         * one.
         * @param source The source map.
         */
        StlMap( const Map<K,V,COMPARATOR>& source ) : Map<K,V,COMPARATOR>(), valueMap(), mutex() {
            copy( source );
        }

        virtual ~StlMap() {}

        /**
         * {@inheritDoc}
         */
        virtual bool equals( const StlMap& source ) const {
            return this->valueMap == source.valueMap;
        }

        /**
         * {@inheritDoc}
         */
        virtual bool equals( const Map<K,V,COMPARATOR>& source ) const {
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

            return true;
        }

        /**
         * {@inheritDoc}
         */
        virtual void copy( const StlMap& source ) {
            this->valueMap.clear();
            this->valueMap.insert( source.valueMap.begin(), source.valueMap.end() );
        }

        /**
         * {@inheritDoc}
         */
        virtual void copy( const Map<K,V,COMPARATOR>& source ) {
            this->clear();
            this->putAll( source );
        }

        /**
         * {@inheritDoc}
         */
        virtual void clear() {
            valueMap.clear();
        }

        /**
         * {@inheritDoc}
         */
        virtual bool containsKey( const K& key ) const {
            typename std::map<K,V,COMPARATOR>::const_iterator iter;
            iter = valueMap.find(key);
            return iter != valueMap.end();
        }

        /**
         * {@inheritDoc}
         */
        virtual bool containsValue( const V& value ) const {

            if( valueMap.empty() ){
                return false;
            }

            typename std::map<K,V,COMPARATOR>::const_iterator iter = valueMap.begin();
            for( ; iter != valueMap.end(); ++iter ){
                if( (*iter).second == value ) {
                    return true;
                }
            }

            return false;
        }

        /**
         * {@inheritDoc}
         */
        virtual bool isEmpty() const {
            return valueMap.empty();
        }

        /**
         * {@inheritDoc}
         */
        virtual int size() const {
            return (int)valueMap.size();
        }

        /**
         * {@inheritDoc}
         */
        virtual V& get( const K& key ) {

            typename std::map<K,V,COMPARATOR>::iterator iter;
            iter = valueMap.find( key );
            if( iter == valueMap.end() ){
                throw NoSuchElementException(
                    __FILE__, __LINE__, "Key does not exist in map" );
            }

            return iter->second;
        }

        /**
         * {@inheritDoc}
         */
        virtual const V& get( const K& key ) const {

            typename std::map<K,V,COMPARATOR>::const_iterator iter;
            iter = valueMap.find( key );
            if( iter == valueMap.end() ){
                throw NoSuchElementException(
                    __FILE__, __LINE__, "Key does not exist in map" );
            }

            return iter->second;
        }

        /**
         * {@inheritDoc}
         */
        virtual void put( const K& key, const V& value ) {
            valueMap[key] = value;
        }

        /**
         * {@inheritDoc}
         */
        virtual void putAll( const StlMap<K,V,COMPARATOR>& other ) {
            this->valueMap.insert( other.valueMap.begin(), other.valueMap.end() );
        }

        /**
         * {@inheritDoc}
         */
        virtual void putAll( const Map<K,V,COMPARATOR>& other ) {

            std::vector<K> keys = other.keySet();

            typename std::vector<K>::const_iterator iter = keys.begin();
            for( ; iter != keys.end(); ++iter ) {

                this->put( *iter, other.get( *iter ) );
            }
        }

        /**
         * {@inheritDoc}
         */
        virtual V remove( const K& key ) {

            typename std::map<K,V,COMPARATOR>::iterator iter = valueMap.find( key );
            if( iter == valueMap.end() ) {
                throw NoSuchElementException(
                    __FILE__, __LINE__, "Key is not present in this Map." );
            }

            V result = iter->second;
            valueMap.erase( iter );
            return result;
        }

        /**
         * {@inheritDoc}
         */
        virtual std::vector<K> keySet() const{
            std::vector<K> keys( valueMap.size() );

            typename std::map<K,V,COMPARATOR>::const_iterator iter;
            iter=valueMap.begin();
            for( int ix=0; iter != valueMap.end(); ++iter, ++ix ){
                keys[ix] = iter->first;
            }

            return keys;
        }

        /**
         * {@inheritDoc}
         */
        virtual std::vector<V> values() const {
            std::vector<V> values( valueMap.size() );

            typename std::map<K,V,COMPARATOR>::const_iterator iter;
            iter=valueMap.begin();
            for( int ix=0; iter != valueMap.end(); ++iter, ++ix ){
                values[ix] = iter->second;
            }

            return values;
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

}}

#endif /*_DECAF_UTIL_STLMAP_H_*/
