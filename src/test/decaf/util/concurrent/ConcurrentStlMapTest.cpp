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

#include "ConcurrentStlMapTest.h"
#include <string>
#include <decaf/util/concurrent/ConcurrentStlMap.h>
#include <decaf/util/HashMap.h>
#include <decaf/util/StlMap.h>
#include <decaf/util/ArrayList.h>
#include <decaf/lang/Integer.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>
#include <decaf/lang/exceptions/IllegalStateException.h>

using namespace std;
using namespace decaf;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
namespace {

    const int MAP_SIZE = 1000;

    void populateMap(StlMap<int, std::string>& map) {
        for (int i = 0; i < MAP_SIZE; ++i) {
            map.put(i, Integer::toString(i));
        }
    }

    void populateMap(ConcurrentStlMap<int, std::string>& map) {
        for (int i = 0; i < MAP_SIZE; ++i) {
            map.put(i, Integer::toString(i));
        }
    }

    void populateMap(HashMap<int, std::string>& map) {
        for (int i = 0; i < MAP_SIZE; ++i) {
            map.put(i, Integer::toString(i));
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void ConcurrentStlMapTest::testConstructor() {

    ConcurrentStlMap<string, int> map1;
    CPPUNIT_ASSERT( map1.isEmpty() );
    CPPUNIT_ASSERT( map1.size() == 0 );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should Throw a NoSuchElementException",
        map1.get( "TEST" ),
        decaf::util::NoSuchElementException );

    HashMap<string, int> srcMap;
    srcMap.put( "A", 1 );
    srcMap.put( "B", 1 );
    srcMap.put( "C", 1 );

    ConcurrentStlMap<string, int> destMap( srcMap );

    CPPUNIT_ASSERT( srcMap.size() == 3 );
    CPPUNIT_ASSERT( destMap.size() == 3 );
    CPPUNIT_ASSERT( destMap.get( "B" ) == 1 );
}

////////////////////////////////////////////////////////////////////////////////
void ConcurrentStlMapTest::testConstructorMap() {

    ConcurrentStlMap<int, int> myMap;
    for (int counter = 0; counter < 125; counter++) {
        myMap.put(counter, counter);
    }

    ConcurrentStlMap<int, int> map(myMap);
    for (int counter = 0; counter < 125; counter++) {
        CPPUNIT_ASSERT_MESSAGE("Failed to construct correct HashMap",
            myMap.get(counter) == map.get(counter));
    }
}

////////////////////////////////////////////////////////////////////////////////
void ConcurrentStlMapTest::testContainsKey(){

    ConcurrentStlMap<string, bool> boolMap;
    CPPUNIT_ASSERT(boolMap.containsKey("bob") == false);

    boolMap.put( "bob", true );

    CPPUNIT_ASSERT(boolMap.containsKey("bob") == true );
    CPPUNIT_ASSERT(boolMap.containsKey("fred") == false );
}

////////////////////////////////////////////////////////////////////////////////
void ConcurrentStlMapTest::testClear(){

    ConcurrentStlMap<string, bool> boolMap;
    boolMap.put( "bob", true );
    boolMap.put( "fred", true );

    CPPUNIT_ASSERT(boolMap.size() == 2 );
    boolMap.clear();
    CPPUNIT_ASSERT(boolMap.size() == 0 );

    {
        ConcurrentStlMap<int, std::string> map;
        map.put(1, "one");
        map.put(3, "three");
        map.put(2, "two");

        map.clear();
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Clear failed to reset size", 0, map.size());
        for (int i = 0; i < 125; i++) {
            CPPUNIT_ASSERT_THROW_MESSAGE(
                "Failed to clear all elements",
                map.get(i),
                NoSuchElementException);
        }

        // Check clear on a large loaded map of Integer keys
        ConcurrentStlMap<int, std::string> map2;
        for (int i = -32767; i < 32768; i++) {
            map2.put(i, "foobar");
        }
        map2.clear();
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Failed to reset size on large integer map", 0, map2.size());
        for (int i = -32767; i < 32768; i++) {
            CPPUNIT_ASSERT_THROW_MESSAGE(
                "Failed to clear all elements",
                map2.get(i),
                NoSuchElementException);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void ConcurrentStlMapTest::testCopy() {

    ConcurrentStlMap<string, int> destMap;
    HashMap<string, int> srcMap;
    ConcurrentStlMap<string, int> srcMap2;

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
void ConcurrentStlMapTest::testIsEmpty(){

    ConcurrentStlMap<string, bool> boolMap;
    boolMap.put( "bob", true );
    boolMap.put( "fred", true );

    CPPUNIT_ASSERT(boolMap.isEmpty() == false );
    boolMap.clear();
    CPPUNIT_ASSERT(boolMap.isEmpty() == true );
}

////////////////////////////////////////////////////////////////////////////////
void ConcurrentStlMapTest::testSize(){

    ConcurrentStlMap<string, bool> boolMap;

    CPPUNIT_ASSERT(boolMap.size() == 0 );
    boolMap.put( "bob", true );
    CPPUNIT_ASSERT(boolMap.size() == 1 );
    boolMap.put( "fred", true );
    CPPUNIT_ASSERT(boolMap.size() == 2 );
}

////////////////////////////////////////////////////////////////////////////////
void ConcurrentStlMapTest::testGet() {

    ConcurrentStlMap<string, bool> boolMap;

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
void ConcurrentStlMapTest::testPut() {

    ConcurrentStlMap<string, bool> boolMap;

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
void ConcurrentStlMapTest::testPutAll() {

    ConcurrentStlMap<string, int> destMap;
    HashMap<string, int> srcMap;
    HashMap<string, int> srcMap2;

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
void ConcurrentStlMapTest::testValue() {

    ConcurrentStlMap<string, bool> boolMap;

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
void ConcurrentStlMapTest::testRemove(){
    ConcurrentStlMap<string, bool> boolMap;

    boolMap.put( "fred", true );
    CPPUNIT_ASSERT( boolMap.containsKey("fred") == true );
    boolMap.remove( "fred" );
    CPPUNIT_ASSERT( boolMap.containsKey("fred") == false );
}

////////////////////////////////////////////////////////////////////////////////
void ConcurrentStlMapTest::testContiansValue(){
    ConcurrentStlMap<string, bool> boolMap;

    boolMap.put( "fred", true );
    boolMap.put( "fred1", false );
    CPPUNIT_ASSERT( boolMap.containsValue(true) == true );
    boolMap.remove( "fred" );
    CPPUNIT_ASSERT( boolMap.containsValue(true) == false );
}

////////////////////////////////////////////////////////////////////////////////
void ConcurrentStlMapTest::testEntrySet() {

    ConcurrentStlMap<int, std::string> map;

    for (int i = 0; i < 50; i++) {
        map.put(i, Integer::toString(i));
    }

    Set<MapEntry<int, std::string> >& set = map.entrySet();
    Pointer< Iterator<MapEntry<int, std::string> > > iterator(set.iterator());

    CPPUNIT_ASSERT_MESSAGE("Returned set of incorrect size", map.size() == set.size());
    while (iterator->hasNext()) {
        MapEntry<int, std::string> entry = iterator->next();
        CPPUNIT_ASSERT_MESSAGE("Returned incorrect entry set",
                               map.containsKey(entry.getKey()) && map.containsValue(entry.getValue()));
    }

    iterator.reset(set.iterator());
    set.remove(iterator->next());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Remove on set didn't take", 49, set.size());
}

////////////////////////////////////////////////////////////////////////////////
void ConcurrentStlMapTest::testKeySet() {

    ConcurrentStlMap<int, std::string> map;
    populateMap(map);
    Set<int>& set = map.keySet();
    CPPUNIT_ASSERT_MESSAGE("Returned set of incorrect size()", set.size() == map.size());
    for (int i = 0; i < MAP_SIZE; i++) {
        CPPUNIT_ASSERT_MESSAGE("Returned set does not contain all keys", set.contains(i));
    }

    {
        ConcurrentStlMap<int, std::string> localMap;
        localMap.put(0, "test");
        Set<int>& intSet = localMap.keySet();
        CPPUNIT_ASSERT_MESSAGE("Failed with zero key", intSet.contains(0));
    }
    {
        ConcurrentStlMap<int, std::string> localMap;
        localMap.put(1, "1");
        localMap.put(102, "102");
        localMap.put(203, "203");

        Set<int>& intSet = localMap.keySet();
        Pointer< Iterator<int> > it(intSet.iterator());
        int remove1 = it->next();
        it->hasNext();
        it->remove();
        int remove2 = it->next();
        it->remove();

        ArrayList<int> list;
        list.add(1);
        list.add(102);
        list.add(203);

        list.remove(remove1);
        list.remove(remove2);

        CPPUNIT_ASSERT_MESSAGE("Wrong result", it->next() == list.get(0));
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong size", 1, localMap.size());
        it.reset(intSet.iterator());
        CPPUNIT_ASSERT_MESSAGE("Wrong contents", it->next() == list.get(0));
    }
    {
        ConcurrentStlMap<int, std::string> map2;
        map2.put(1, "1");
        map2.put(4, "4");

        Set<int>& intSet = map2.keySet();
        Pointer< Iterator<int> > it2(intSet.iterator());

        int remove3 = it2->next();
        int next;

        if (remove3 == 1) {
            next = 4;
        } else {
            next = 1;
        }
        it2->hasNext();
        it2->remove();
        CPPUNIT_ASSERT_MESSAGE("Wrong result 2", it2->next() == next);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong size 2", 1, map2.size());
        it2.reset(intSet.iterator());
        CPPUNIT_ASSERT_MESSAGE("Wrong contents 2", it2->next() == next);
    }
}

////////////////////////////////////////////////////////////////////////////////
void ConcurrentStlMapTest::testValues() {

    ConcurrentStlMap<int, std::string> map;
    populateMap(map);

    Collection<std::string>& c = map.values();
    CPPUNIT_ASSERT_MESSAGE("Returned collection of incorrect size()", c.size() == map.size());
    for (int i = 0; i < MAP_SIZE; i++) {
        CPPUNIT_ASSERT_MESSAGE("Returned collection does not contain all keys",
                               c.contains(Integer::toString(i)));
    }

    c.remove("10");
    CPPUNIT_ASSERT_MESSAGE("Removing from collection should alter Map",
                           !map.containsKey(10));
}

////////////////////////////////////////////////////////////////////////////////
void ConcurrentStlMapTest::testEntrySetIterator() {

    ConcurrentStlMap<int, std::string> map;
    populateMap(map);

    int count = 0;
    Pointer< Iterator<MapEntry<int, std::string> > > iterator(map.entrySet().iterator());
    while (iterator->hasNext()) {
        MapEntry<int, std::string> entry = iterator->next();
        CPPUNIT_ASSERT_EQUAL(count, entry.getKey());
        CPPUNIT_ASSERT_EQUAL(Integer::toString(count), entry.getValue());
        count++;
    }

    CPPUNIT_ASSERT_MESSAGE("Iterator didn't cover the expected range", count++ == MAP_SIZE);

    iterator.reset(map.entrySet().iterator());
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IllegalStateException",
        iterator->remove(),
        IllegalStateException);

    count = 0;
    while (iterator->hasNext()) {
        iterator->next();
        iterator->remove();
        count++;
    }

    CPPUNIT_ASSERT_MESSAGE("Iterator didn't remove the expected range", count++ == MAP_SIZE);
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IllegalStateException",
        iterator->remove(),
        IllegalStateException);
}

////////////////////////////////////////////////////////////////////////////////
void ConcurrentStlMapTest::testKeySetIterator() {

    ConcurrentStlMap<int, std::string> map;
    populateMap(map);

    int count = 0;
    Pointer< Iterator<int> > iterator(map.keySet().iterator());
    while (iterator->hasNext()) {
        int key = iterator->next();
        CPPUNIT_ASSERT_EQUAL(count, key);
        count++;
    }

    CPPUNIT_ASSERT_MESSAGE("Iterator didn't cover the expected range", count++ == MAP_SIZE);

    iterator.reset(map.keySet().iterator());
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IllegalStateException",
        iterator->remove(),
        IllegalStateException);

    count = 0;
    while (iterator->hasNext()) {
        iterator->next();
        iterator->remove();
        count++;
    }

    CPPUNIT_ASSERT_MESSAGE("Iterator didn't remove the expected range", count++ == MAP_SIZE);
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IllegalStateException",
        iterator->remove(),
        IllegalStateException);
}

////////////////////////////////////////////////////////////////////////////////
void ConcurrentStlMapTest::testValuesIterator() {

    ConcurrentStlMap<int, std::string> map;
    populateMap(map);

    int count = 0;
    Pointer< Iterator<std::string> > iterator(map.values().iterator());
    while (iterator->hasNext()) {
        std::string value = iterator->next();
        CPPUNIT_ASSERT_EQUAL(Integer::toString(count), value);
        count++;
    }

    CPPUNIT_ASSERT_MESSAGE("Iterator didn't cover the expected range", count++ == MAP_SIZE);

    iterator.reset(map.values().iterator());
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IllegalStateException",
        iterator->remove(),
        IllegalStateException);

    count = 0;
    while (iterator->hasNext()) {
        iterator->next();
        iterator->remove();
        count++;
    }

    CPPUNIT_ASSERT_MESSAGE("Iterator didn't remove the expected range", count++ == MAP_SIZE);
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IllegalStateException",
        iterator->remove(),
        IllegalStateException);
}
