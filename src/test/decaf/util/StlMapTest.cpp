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

#include "StlMapTest.h"

#include <string>
#include <decaf/util/StlMap.h>

using namespace std;
using namespace decaf;
using namespace decaf::util;

template <typename K, typename V, typename COMPARATOR = std::less<K> > class STLTestMap :
    public Map<K, V, COMPARATOR> {
private:

    std::map<K,V,COMPARATOR> valueMap;

public:

    STLTestMap() : Map<K,V,COMPARATOR>() {}

    STLTestMap( const STLTestMap& source ) : Map<K,V,COMPARATOR>() {
        copy( source );
    }

    STLTestMap( const Map<K,V,COMPARATOR>& source ) : Map<K,V,COMPARATOR>() {
        copy( source );
    }

    virtual ~STLTestMap() {}

    virtual bool equals( const STLTestMap& source ) const {
        return this->valueMap == source.valueMap;
    }
    virtual bool equals( const Map<K,V,COMPARATOR>& source DECAF_UNUSED ) const {
        return false; // TODO - this->valueMap == source.valueMap;
    }

    virtual void copy( const STLTestMap& source ) {
        valueMap.clear();
        valueMap.insert( source.valueMap.begin(), source.valueMap.end() );
    }
    virtual void copy( const Map<K,V,COMPARATOR>& source DECAF_UNUSED ) {
        // TODO
    }

    virtual void clear() {
        valueMap.clear();
    }

    virtual bool containsKey( const K& key ) const {
        typename std::map<K,V,COMPARATOR>::const_iterator iter;
        iter = valueMap.find(key);
        return iter != valueMap.end();
    }

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

    virtual bool isEmpty() const {
        return valueMap.empty();
    }

    virtual std::size_t size() const {
        return valueMap.size();
    }

    virtual V get( const K& key ) const
        throw( lang::exceptions::NoSuchElementException ) {

        typename std::map<K,V,COMPARATOR>::const_iterator iter;
        iter = valueMap.find(key);
        if( iter == valueMap.end() ){
            throw lang::exceptions::NoSuchElementException(
                __FILE__, __LINE__, "Key does not exist in map" );
        }

        return iter->second;
    }

    virtual void put( const K& key, V value ) {
        valueMap[key] = value;
    }

    virtual void putAll( const StlMap<K,V,COMPARATOR>& other DECAF_UNUSED ) {
        // TODO
    }
    virtual void putAll( const Map<K,V,COMPARATOR>& other DECAF_UNUSED ) {
        // TODO
    }

    virtual void remove( const K& key ) {
        valueMap.erase( key );
    }

    virtual std::vector<K> keySet() const{
        std::vector<K> keys( valueMap.size() );

        typename std::map<K,V,COMPARATOR>::const_iterator iter;
        iter=valueMap.begin();
        for( int ix=0; iter != valueMap.end(); ++iter, ++ix ){
            keys[ix] = iter->first;
        }

        return keys;
    }

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

    virtual void lock() throw( lang::Exception ) {
    }
    virtual void unlock() throw( lang::Exception ) {
    }
    virtual void wait() throw( lang::Exception ) {
    }
    virtual void wait( unsigned long millisecs ) throw( lang::Exception ) {
    }
    virtual void notify() throw( lang::Exception ) {
    }
    virtual void notifyAll() throw( lang::Exception ) {
    }
};

////////////////////////////////////////////////////////////////////////////////
void StlMapTest::testConstructor() {

    StlMap<string, int> map1;
    CPPUNIT_ASSERT( map1.isEmpty() );
    CPPUNIT_ASSERT( map1.size() == 0 );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should Throw a NoSuchElementException",
        map1.get( "TEST" ),
        decaf::lang::exceptions::NoSuchElementException );

    StlMap<string, int> destMap;
    STLTestMap<string, int> srcMap;

    srcMap.put( "A", 1 );
    srcMap.put( "B", 1 );
    srcMap.put( "C", 1 );

    CPPUNIT_ASSERT( srcMap.size() == 3 );
    CPPUNIT_ASSERT( destMap.size() == 0 );

    destMap.copy( srcMap );
    CPPUNIT_ASSERT( srcMap.size() == 3 );
    CPPUNIT_ASSERT( destMap.size() == 3 );

    STLTestMap<string, int> srcMap2;
    srcMap2.put( "D", 1 );
    srcMap2.put( "E", 1 );
    srcMap2.put( "F", 1 );

    destMap.putAll( srcMap2 );
    CPPUNIT_ASSERT( destMap.size() == 6 );

}
