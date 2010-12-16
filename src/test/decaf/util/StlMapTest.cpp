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

template <typename K, typename V, typename COMPARATOR = std::less<K> >
class StlTestMap : public Map<K, V, COMPARATOR> {
private:

    std::map<K,V,COMPARATOR> valueMap;
    concurrent::Mutex mutex;

public:

    /**
     * Default constructor - does nothing.
     */
    StlTestMap() : Map<K,V,COMPARATOR>() {}

    /**
     * Copy constructor - copies the content of the given map into this
     * one.
     * @param source The source map.
     */
    StlTestMap( const StlTestMap& source ) : Map<K,V,COMPARATOR>() {
        copy( source );
    }

    /**
     * Copy constructor - copies the content of the given map into this
     * one.
     * @param source The source map.
     */
    StlTestMap( const Map<K,V,COMPARATOR>& source ) : Map<K,V,COMPARATOR>() {
        copy( source );
    }

    virtual ~StlTestMap() {}

    /**
     * {@inheritDoc}
     */
    virtual bool equals( const StlTestMap& source ) const {
        return this->valueMap == source.valueMap;
    }

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
    virtual void copy( const StlTestMap& source ) {
        this->valueMap.clear();
        this->valueMap.insert( source.valueMap.begin(), source.valueMap.end() );
    }

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
            throw util::NoSuchElementException(
                __FILE__, __LINE__, "Key does not exist in map" );
        }

        return iter->second;
    }
    virtual const V& get( const K& key ) const {

        typename std::map<K,V,COMPARATOR>::const_iterator iter;
        iter = valueMap.find( key );
        if( iter == valueMap.end() ){
            throw util::NoSuchElementException(
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
    virtual void putAll( const StlTestMap<K,V,COMPARATOR>& other ) {

        this->valueMap.insert( other.valueMap.begin(), other.valueMap.end() );
    }
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
            throw decaf::util::NoSuchElementException(
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

////////////////////////////////////////////////////////////////////////////////
void StlMapTest::testConstructor() {

    StlMap<string, int> map1;
    CPPUNIT_ASSERT( map1.isEmpty() );
    CPPUNIT_ASSERT( map1.size() == 0 );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should Throw a NoSuchElementException",
        map1.get( "TEST" ),
        decaf::util::NoSuchElementException );

    StlTestMap<string, int> srcMap;
    srcMap.put( "A", 1 );
    srcMap.put( "B", 1 );
    srcMap.put( "C", 1 );

    StlMap<string, int> destMap( srcMap );

    CPPUNIT_ASSERT( srcMap.size() == 3 );
    CPPUNIT_ASSERT( destMap.size() == 3 );
    CPPUNIT_ASSERT( destMap.get( "B" ) == 1 );
}

////////////////////////////////////////////////////////////////////////////////
void StlMapTest::testContainsKey(){

    StlMap<string, bool> boolMap;
    CPPUNIT_ASSERT(boolMap.containsKey("bob") == false);

    boolMap.put( "bob", true );

    CPPUNIT_ASSERT(boolMap.containsKey("bob") == true );
    CPPUNIT_ASSERT(boolMap.containsKey("fred") == false );
}

////////////////////////////////////////////////////////////////////////////////
void StlMapTest::testContiansValue() {

    StlMap<string, bool> boolMap;

    boolMap.put( "fred", true );
    boolMap.put( "fred1", false );
    CPPUNIT_ASSERT( boolMap.containsValue(true) == true );
    boolMap.remove( "fred" );
    CPPUNIT_ASSERT( boolMap.containsValue(true) == false );
}


////////////////////////////////////////////////////////////////////////////////
void StlMapTest::testClear() {

    StlMap<string, bool> boolMap;
    boolMap.put( "bob", true );
    boolMap.put( "fred", true );

    CPPUNIT_ASSERT(boolMap.size() == 2 );
    boolMap.clear();
    CPPUNIT_ASSERT(boolMap.size() == 0 );
}

////////////////////////////////////////////////////////////////////////////////
void StlMapTest::testCopy() {

    StlMap<string, int> destMap;
    StlTestMap<string, int> srcMap;
    StlMap<string, int> srcMap2;

    CPPUNIT_ASSERT( destMap.size() == 0 );

    srcMap.put( "A", 1 );
    srcMap.put( "B", 2 );
    srcMap.put( "C", 3 );
    srcMap.put( "D", 4 );
    srcMap.put( "E", 5 );
    srcMap.put( "F", 6 );

    destMap.copy( srcMap );
    CPPUNIT_ASSERT( destMap.size() == 6 );
    CPPUNIT_ASSERT( destMap.get( "A" ) == 1 );
    CPPUNIT_ASSERT( destMap.get( "B" ) == 2 );
    CPPUNIT_ASSERT( destMap.get( "C" ) == 3 );
    CPPUNIT_ASSERT( destMap.get( "D" ) == 4 );
    CPPUNIT_ASSERT( destMap.get( "E" ) == 5 );
    CPPUNIT_ASSERT( destMap.get( "F" ) == 6 );

    destMap.copy( srcMap2 );
    CPPUNIT_ASSERT( destMap.size() == 0 );

    srcMap2.put( "A", 1 );
    srcMap2.put( "B", 2 );
    srcMap2.put( "C", 3 );
    srcMap2.put( "D", 4 );
    srcMap2.put( "E", 5 );

    destMap.copy( srcMap2 );
    CPPUNIT_ASSERT( destMap.size() == 5 );
}

////////////////////////////////////////////////////////////////////////////////
void StlMapTest::testIsEmpty() {

    StlMap<string, bool> boolMap;
    boolMap.put( "bob", true );
    boolMap.put( "fred", true );

    CPPUNIT_ASSERT(boolMap.isEmpty() == false );
    boolMap.clear();
    CPPUNIT_ASSERT(boolMap.isEmpty() == true );
}

////////////////////////////////////////////////////////////////////////////////
void StlMapTest::testSize() {

    StlMap<string, bool> boolMap;

    CPPUNIT_ASSERT(boolMap.size() == 0 );
    boolMap.put( "bob", true );
    CPPUNIT_ASSERT(boolMap.size() == 1 );
    boolMap.put( "fred", true );
    CPPUNIT_ASSERT(boolMap.size() == 2 );
}

////////////////////////////////////////////////////////////////////////////////
void StlMapTest::testGet() {

    StlMap<string, bool> boolMap;

    boolMap.put( "fred", true );
    CPPUNIT_ASSERT( boolMap.get("fred") == true );

    boolMap.put( "bob", false );
    CPPUNIT_ASSERT( boolMap.get("bob") == false );
    CPPUNIT_ASSERT( boolMap.get("fred") == true );

    try{
        boolMap.get( "mike" );
        CPPUNIT_ASSERT(false);
    } catch( decaf::util::NoSuchElementException& e ){
    }
}

////////////////////////////////////////////////////////////////////////////////
void StlMapTest::testPut() {

    StlMap<string, bool> boolMap;

    boolMap.put( "fred", true );
    CPPUNIT_ASSERT( boolMap.get("fred") == true );

    boolMap.put( "bob", false );
    CPPUNIT_ASSERT( boolMap.get("bob") == false );
    CPPUNIT_ASSERT( boolMap.get("fred") == true );

    boolMap.put( "bob", true );
    CPPUNIT_ASSERT( boolMap.get("bob") == true );
    CPPUNIT_ASSERT( boolMap.get("fred") == true );
}

////////////////////////////////////////////////////////////////////////////////
void StlMapTest::testPutAll() {

    StlMap<string, int> destMap;
    StlTestMap<string, int> srcMap;
    StlTestMap<string, int> srcMap2;

    srcMap.put( "A", 1 );
    srcMap.put( "B", 1 );
    srcMap.put( "C", 1 );

    CPPUNIT_ASSERT( srcMap.size() == 3 );
    CPPUNIT_ASSERT( destMap.size() == 0 );

    srcMap.put( "D", 1 );
    srcMap.put( "E", 1 );
    srcMap.put( "F", 1 );

    destMap.putAll( srcMap );
    CPPUNIT_ASSERT( destMap.size() == 6 );
    destMap.putAll( srcMap2 );
    CPPUNIT_ASSERT( destMap.size() == 6 );
}

////////////////////////////////////////////////////////////////////////////////
void StlMapTest::testRemove() {
    StlMap<string, bool> boolMap;

    boolMap.put( "fred", true );
    CPPUNIT_ASSERT( boolMap.containsKey("fred") == true );
    CPPUNIT_ASSERT( boolMap.remove( "fred" ) == true );
    CPPUNIT_ASSERT( boolMap.containsKey("fred") == false );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NoSuchElementException",
        boolMap.remove( "fred" ),
        decaf::util::NoSuchElementException );
}

