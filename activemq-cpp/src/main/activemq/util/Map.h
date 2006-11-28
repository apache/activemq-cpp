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

#ifndef ACTIVEMQ_UTIL_MAP_H_
#define ACTIVEMQ_UTIL_MAP_H_

#include <map>
#include <vector>
#include <activemq/exceptions/NoSuchElementException.h>

namespace activemq{
namespace util{

    /**
     * Map template that wraps around a std::map to provide
     * a more user-friendly interface and to provide common
     * functions that do not exist in std::map.
     */
    template <typename K, typename V> class Map
    {
    private:
    
        std::map<K,V> valueMap;
        
    public:
    
    	Map(){};
    	virtual ~Map(){};
        
        /**
         * Removes all keys and values from this map.
         */
        virtual void clear();
        
        /**
         * Indicates whether or this map contains a value for the
         * given key.
         * @param key The key to look up.
         * @return true if this map contains the value, otherwise false.
         */
        virtual bool contains( const K& key ) const;
        
        /**
         * @return The number of elements (key/value pairs) in this map.
         */
        virtual int count() const;
        
        /**
         * Gets the value for the specified key.
         * @param key The search key.
         * @return The value for the given key.
         * @throws activemq::exceptions::NoSuchElementException
         */
        virtual V getValue( const K& key ) const 
            throw(activemq::exceptions::NoSuchElementException);
            
        /**
         * Sets the value for the specified key.
         * @param key The target key.
         * @param value The value to be set.
         */
        virtual void setValue( const K& key, V value );

        /**
         * Removes the value (key/value pair) for the specified key from 
         * the map.
         * @param key The search key.
         */        
        virtual void remove( const K& key );
        
        /**
         * @return the entire set of keys in this map as a std::vector.
         */
        virtual std::vector<K> getKeys() const;
        
        /**
         * @return the entire set of values in this map as a std::vector.
         */
        virtual std::vector<V> getValues() const;
    };
    
    ////////////////////////////////////////////////////////////////////////////
    template <typename K, typename V>
    void Map<K,V>::clear(){
        valueMap.clear();
    }
    
    ////////////////////////////////////////////////////////////////////////////
    template <typename K, typename V>
    bool Map<K,V>::contains(const K& key) const{
        typename std::map<K,V>::const_iterator iter;
        iter = valueMap.find(key);
        return iter != valueMap.end();
    }
    
    ////////////////////////////////////////////////////////////////////////////
    template <typename K, typename V>
    int Map<K,V>::count() const{
        return valueMap.size();
    }
    
    ////////////////////////////////////////////////////////////////////////////
    template <typename K, typename V>
    V Map<K,V>::getValue( const K& key ) const 
        throw(activemq::exceptions::NoSuchElementException){
            
        typename std::map<K,V>::const_iterator iter;
        iter = valueMap.find(key);
        if( iter == valueMap.end() ){
            throw activemq::exceptions::NoSuchElementException( __FILE__, 
                __LINE__, 
                "Key does not exist in map" );
        }
        
        return iter->second;
    }
        
    ////////////////////////////////////////////////////////////////////////////
    template <typename K, typename V>
    void Map<K,V>::setValue( const K& key, V value ){
        valueMap[key] = value;
    }
    
    ////////////////////////////////////////////////////////////////////////////
    template <typename K, typename V>
    void Map<K,V>::remove( const K& key ){
        valueMap.erase(key);
    }
    
    ////////////////////////////////////////////////////////////////////////////
    template <typename K, typename V>
    std::vector<K> Map<K,V>::getKeys() const{
        std::vector<K> values(valueMap.size());
        
        typename std::map<K,V>::const_iterator iter;
        iter=valueMap.begin();
        for( int ix=0; iter != valueMap.end(); ++iter, ++ix ){
            values[ix] = iter->first;
        }
    }
    
    ////////////////////////////////////////////////////////////////////////////
    template <typename K, typename V>
    std::vector<V> Map<K,V>::getValues() const{
        std::vector<V> values(valueMap.size());
        
        typename std::map<K,V>::const_iterator iter;
        iter=valueMap.begin();
        for( int ix=0; iter != valueMap.end(); ++iter, ++ix ){
            values[ix] = iter->second;
        }
    }

}}

#endif /*ACTIVEMQ_UTIL_MAP_H_*/
