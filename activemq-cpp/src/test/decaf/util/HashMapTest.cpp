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

#include "HashMapTest.h"

#include <decaf/util/Set.h>
#include <decaf/util/Iterator.h>
#include <decaf/util/HashMap.h>
#include <decaf/util/StlMap.h>
#include <decaf/util/ArrayList.h>
#include <decaf/lang/Integer.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>

using namespace std;
using namespace decaf;
using namespace decaf::util;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
namespace {

    const int MAP_SIZE = 1000;

    void populateMap(HashMap<int, std::string>& hashMap) {
        for (int i = 0; i < MAP_SIZE; ++i) {
            hashMap.put(i, Integer::toString(i));
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
HashMapTest::HashMapTest() {
}

////////////////////////////////////////////////////////////////////////////////
HashMapTest::~HashMapTest() {
}

////////////////////////////////////////////////////////////////////////////////
void HashMapTest::setUp() {
}

////////////////////////////////////////////////////////////////////////////////
void HashMapTest::testConstructor() {

    HashMap<int, std::string> map;
    CPPUNIT_ASSERT(map.isEmpty());
    CPPUNIT_ASSERT_EQUAL(0, map.size());
    CPPUNIT_ASSERT_EQUAL(false, map.containsKey(1));
    CPPUNIT_ASSERT_EQUAL(false, map.containsValue("test"));
}

////////////////////////////////////////////////////////////////////////////////
void HashMapTest::testConstructorI() {

    HashMap<int, std::string> map(5);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Created incorrect HashMap", 0, map.size());

    try {
        HashMap<int, std::string> map(-1);
        CPPUNIT_FAIL("Should have thrown IllegalArgumentException for negative arg.");
    } catch (IllegalArgumentException& e) {
    }

    HashMap<int, std::string> empty(0);
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown NoSuchElementException",
        empty.get(1),
        NoSuchElementException);
    empty.put(1, "here");
    CPPUNIT_ASSERT_MESSAGE("cannot get element", empty.get(1) == std::string("here"));
}

////////////////////////////////////////////////////////////////////////////////
void HashMapTest::testConstructorIF() {

    HashMap<int, std::string> map(5, 0.5f);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Created incorrect HashMap", 0, map.size());

    try {
        HashMap<int, std::string> map(0, 0);
        CPPUNIT_FAIL("Should have thrown IllegalArgumentException for negative arg.");
    } catch (IllegalArgumentException& e) {
    }

    HashMap<int, std::string> empty(0, 0.25f);
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown NoSuchElementException",
        empty.get(1),
        NoSuchElementException);
    empty.put(1, "here");
    CPPUNIT_ASSERT_MESSAGE("cannot get element", empty.get(1) == std::string("here"));
}

////////////////////////////////////////////////////////////////////////////////
void HashMapTest::testConstructorMap() {

    HashMap<int, int> myMap;
    for (int counter = 0; counter < 125; counter++) {
        myMap.put(counter, counter);
    }

    HashMap<int, int> hashMap(myMap);
    for (int counter = 0; counter < 125; counter++) {
        CPPUNIT_ASSERT_MESSAGE("Failed to construct correct HashMap",
            myMap.get(counter) == hashMap.get(counter));
    }
}

////////////////////////////////////////////////////////////////////////////////
void HashMapTest::testClear() {

    HashMap<int, std::string> hashMap;
    hashMap.put(1, "one");
    hashMap.put(3, "three");
    hashMap.put(2, "two");

    hashMap.clear();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Clear failed to reset size", 0, hashMap.size());
    for (int i = 0; i < 125; i++) {
        CPPUNIT_ASSERT_THROW_MESSAGE(
            "Failed to clear all elements",
            hashMap.get(i),
            NoSuchElementException);
    }

    // Check clear on a large loaded map of Integer keys
    HashMap<int, std::string> map;
    for (int i = -32767; i < 32768; i++) {
        map.put(i, "foobar");
    }
    map.clear();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Failed to reset size on large integer map", 0, map.size());
    for (int i = -32767; i < 32768; i++) {
        CPPUNIT_ASSERT_THROW_MESSAGE(
            "Failed to clear all elements",
            map.get(i),
            NoSuchElementException);
    }
}

////////////////////////////////////////////////////////////////////////////////
void HashMapTest::testContainsKey() {

    HashMap<int, std::string> hashMap;

    hashMap.put(876, "test");

    CPPUNIT_ASSERT_MESSAGE("Returned false for valid key", hashMap.containsKey(876));
    CPPUNIT_ASSERT_MESSAGE("Returned true for invalid key", !hashMap.containsKey(1));

    HashMap<int, std::string> hashMap2;
    hashMap2.put(0, "test");
    CPPUNIT_ASSERT_MESSAGE("Failed with key", hashMap2.containsKey(0));
    CPPUNIT_ASSERT_MESSAGE("Failed with missing key matching hash", !hashMap2.containsKey(1));
}

////////////////////////////////////////////////////////////////////////////////
void HashMapTest::testContainsValue() {

    HashMap<int, std::string> hashMap;

    hashMap.put(876, "test");

    CPPUNIT_ASSERT_MESSAGE("Returned false for valid value", hashMap.containsValue("test"));
    CPPUNIT_ASSERT_MESSAGE("Returned true for invalid valie", !hashMap.containsValue(""));
}

////////////////////////////////////////////////////////////////////////////////
void HashMapTest::testEntrySet() {

    HashMap<int, std::string> hashMap;

    for (int i = 0; i < 50; i++) {
        hashMap.put(i, Integer::toString(i));
    }

    Set<MapEntry<int, std::string> >& set = hashMap.entrySet();
    Pointer< Iterator<MapEntry<int, std::string> > > iterator(set.iterator());

    CPPUNIT_ASSERT_MESSAGE("Returned set of incorrect size", hashMap.size() == set.size());
    while (iterator->hasNext()) {
        MapEntry<int, std::string> entry = iterator->next();
        CPPUNIT_ASSERT_MESSAGE("Returned incorrect entry set",
                               hashMap.containsKey(entry.getKey()) && hashMap.containsValue(entry.getValue()));
    }

    iterator.reset(set.iterator());
    set.remove(iterator->next());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Remove on set didn't take", 49, set.size());
}

////////////////////////////////////////////////////////////////////////////////
void HashMapTest::testGet() {

    HashMap<int, std::string> hashMap;

    CPPUNIT_ASSERT_THROW_MESSAGE(
            "Should have thrown NoSuchElementException",
            hashMap.get(1),
            NoSuchElementException);
    hashMap.put(22, "HELLO");
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Get returned incorrect value for existing key",
                                 std::string("HELLO"), hashMap.get(22));
}

////////////////////////////////////////////////////////////////////////////////
void HashMapTest::testIsEmpty() {

    HashMap<int, std::string> hashMap;

    CPPUNIT_ASSERT_MESSAGE("Returned false for new map", hashMap.isEmpty());
    hashMap.put(1, "1");
    CPPUNIT_ASSERT_MESSAGE("Returned true for non-empty", !hashMap.isEmpty());
    hashMap.clear();
    CPPUNIT_ASSERT_MESSAGE("Returned false for cleared map", hashMap.isEmpty());
}

////////////////////////////////////////////////////////////////////////////////
void HashMapTest::testKeySet() {

    HashMap<int, std::string> hashMap;
    populateMap(hashMap);
    Set<int>& set = hashMap.keySet();
    CPPUNIT_ASSERT_MESSAGE("Returned set of incorrect size()", set.size() == hashMap.size());
    for (int i = 0; i < MAP_SIZE; i++) {
        CPPUNIT_ASSERT_MESSAGE("Returned set does not contain all keys", set.contains(i));
    }

    {
        HashMap<int, std::string> localMap;
        localMap.put(0, "test");
        Set<int>& intSet = localMap.keySet();
        CPPUNIT_ASSERT_MESSAGE("Failed with zero key", intSet.contains(0));
    }
    {
        HashMap<int, std::string> localMap;
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
        HashMap<int, std::string> map2(101);
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
namespace {

    class MyKey {
    private:

        static int COUNTER;

        int id;

    public:

        MyKey() : id(++COUNTER) {
        }

        int hashCode() const {
            return 0;
        }

        bool operator==(const MyKey& key) const {
            return this->id == key.id;
        }

        friend std::ostream& operator<<(std::ostream& stream, const MyKey& key);
    };

    std::ostream& operator<<(std::ostream& stream, const MyKey& key) {
        stream << "MyKey: " << key.id;
        return stream;
    }

    int MyKey::COUNTER = 0;
}

////////////////////////////////////////////////////////////////////////////////
namespace decaf {
namespace util {

    template<>
    struct HashCode<MyKey> {

        typedef MyKey argument_type;
        typedef int result_type;

        int operator()(const MyKey& arg) const {
            return arg.hashCode();
        }
    };

}}

////////////////////////////////////////////////////////////////////////////////
void HashMapTest::testPut() {

    {
        HashMap<std::string, std::string> hashMap(101);
        hashMap.put("KEY", "VALUE");
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Failed to install key/value pair",
                                     std::string("VALUE"), hashMap.get("KEY"));
    }
    {
        // Check my actual key instance is returned
        HashMap<int, std::string> map;
        for (int i = -32767; i < 32768; i++) {
            map.put(i, "foobar");
        }
        int myKey = 0;
        // Put a new value at the old key position
        map.put(myKey, "myValue");
        CPPUNIT_ASSERT(map.containsKey(myKey));
        CPPUNIT_ASSERT_EQUAL(std::string("myValue"), map.get(myKey));
        bool found = false;
        Set<int>& intSet = map.keySet();
        Pointer< Iterator<int> > itr(intSet.iterator());
        while (itr->hasNext()) {
            int key = itr->next();
            found = (key == myKey);
            if (found) {
                break;
            }
        }
        CPPUNIT_ASSERT_MESSAGE("Should find new key instance in hashashMap", found);

        // Add a new key instance and check it is returned
        CPPUNIT_ASSERT_NO_THROW(map.remove(myKey));
        map.put(myKey, "myValue");
        CPPUNIT_ASSERT(map.containsKey(myKey));
        CPPUNIT_ASSERT_EQUAL(std::string("myValue"), map.get(myKey));
        itr.reset(intSet.iterator());
        while (itr->hasNext()) {
            int key = itr->next();
            found = (key == myKey);
            if (found) {
                break;
            }
        }
        CPPUNIT_ASSERT_MESSAGE("Did not find new key instance in hashashMap", found);
    }
    {
        // Ensure keys with identical hashcode are stored separately
        HashMap<MyKey, std::string> map;

        // Put non-equal object with same hashcode
        MyKey aKey;
        CPPUNIT_ASSERT(!map.containsKey(aKey));
        map.put(aKey, "value");
        MyKey aKey2;
        CPPUNIT_ASSERT_THROW_MESSAGE(
                "Should have thrown NoSuchElementException",
                map.remove(aKey2),
                NoSuchElementException);
        MyKey aKey3;
        map.put(aKey3, "foobar");
        CPPUNIT_ASSERT_EQUAL(std::string("foobar"), map.get(aKey3));
        CPPUNIT_ASSERT_EQUAL(std::string("value"), map.get(aKey));
    }
}

////////////////////////////////////////////////////////////////////////////////
void HashMapTest::testPutAll() {

    HashMap<int, std::string> hashMap;
    populateMap(hashMap);

    HashMap<int, std::string> hashMap2;
    hashMap2.putAll(hashMap);
    for (int i = 0; i < 1000; i++) {
        CPPUNIT_ASSERT_MESSAGE("Failed to put all elements into new Map",
                               hashMap2.get(i) == Integer::toString(i));
    }
}

////////////////////////////////////////////////////////////////////////////////
void HashMapTest::testRemove() {

    {
        HashMap<int, std::string> hashMap;
        populateMap(hashMap);

        int size = hashMap.size();
        CPPUNIT_ASSERT_NO_THROW_MESSAGE("Remove returned incorrect value", hashMap.remove(9));
        CPPUNIT_ASSERT_THROW_MESSAGE(
            "Should have thrown a NoSuchElementException on get of non-existent key.",
            hashMap.get(9),
            NoSuchElementException);

        CPPUNIT_ASSERT_MESSAGE("Failed to decrement size", hashMap.size() == (size - 1));
        CPPUNIT_ASSERT_THROW_MESSAGE(
            "Should have thrown a NoSuchElementException on remove of non-existent key.",
            hashMap.remove(9),
            NoSuchElementException);
    }
    {
        HashMap<int, std::string> hashMap;
        for (int i = 0; i < 8192; i++) {
            hashMap.put(i, "const");
        }
        for (int i = 0; i < 8192; i++) {
            hashMap.put(i, Integer::toString(i));
        }
        for (int i = 8191; i >= 0; i--) {
            std::string iValue = Integer::toString(i);
            CPPUNIT_ASSERT_MESSAGE(std::string("Failed to replace value: ") + iValue,
                                   hashMap.containsValue(iValue));
            hashMap.remove(i);
            CPPUNIT_ASSERT_MESSAGE(std::string("Failed to remove same value: ") + iValue,
                                   !hashMap.containsValue(iValue));
        }
    }

    {
        // Ensure keys with identical hashcode are stored separately and removed correctly.
        HashMap<MyKey, std::string> map;

        // Put non-equal object with same hashcode
        MyKey aKey;
        CPPUNIT_ASSERT(!map.containsKey(aKey));
        map.put(aKey, "value");
        MyKey aKey2;
        CPPUNIT_ASSERT_THROW_MESSAGE(
                "Should have thrown NoSuchElementException",
                map.remove(aKey2),
                NoSuchElementException);
        MyKey aKey3;
        map.put(aKey3, "foobar");
        CPPUNIT_ASSERT_EQUAL(std::string("foobar"), map.get(aKey3));
        CPPUNIT_ASSERT_EQUAL(std::string("value"), map.get(aKey));
        map.remove(aKey);
        map.remove(aKey3);
        CPPUNIT_ASSERT(!map.containsKey(aKey));
        CPPUNIT_ASSERT(map.isEmpty());
    }
}

////////////////////////////////////////////////////////////////////////////////
void HashMapTest::testRehash() {
    // This map should rehash on adding the ninth element.
    HashMap<MyKey, int> hashMap(10, 0.5f);

    // Ordered set of keys.
    MyKey keyOrder[9];

    // Store eight elements
    for (int i = 0; i < 8; i++) {
        hashMap.put(keyOrder[i], i);
    }

    // Check expected ordering (inverse of adding order)
    Set<MyKey>& keySet = hashMap.keySet();
    std::vector<MyKey> returnedKeys = keySet.toArray();
    for (int i = 0; i < 8; i++) {
        CPPUNIT_ASSERT_EQUAL(keyOrder[i], returnedKeys[7 - i]);
    }

    // The next put causes a rehash
    hashMap.put(keyOrder[8], 8);
    // Check expected new ordering (adding order)
    returnedKeys = keySet.toArray();
    for (int i = 0; i < 9; i++) {
        CPPUNIT_ASSERT_EQUAL(keyOrder[i], returnedKeys[i]);
    }
}

////////////////////////////////////////////////////////////////////////////////
void HashMapTest::testSize() {
    HashMap<int, std::string> hashMap;
    populateMap(hashMap);

    CPPUNIT_ASSERT_MESSAGE("Returned incorrect size", hashMap.size() == MAP_SIZE);
}

////////////////////////////////////////////////////////////////////////////////
void HashMapTest::testValues() {

    HashMap<int, std::string> hashMap;
    populateMap(hashMap);

    Collection<std::string>& c = hashMap.values();
    CPPUNIT_ASSERT_MESSAGE("Returned collection of incorrect size()", c.size() == hashMap.size());
    for (int i = 0; i < MAP_SIZE; i++) {
        CPPUNIT_ASSERT_MESSAGE("Returned collection does not contain all keys",
                               c.contains(Integer::toString(i)));
    }

    c.remove("10");
    CPPUNIT_ASSERT_MESSAGE("Removing from collection should alter Map",
                           !hashMap.containsKey(10));
}

////////////////////////////////////////////////////////////////////////////////
void HashMapTest::testToString() {

    HashMap<int, std::string> hashMap;
    populateMap(hashMap);
    std::string result = hashMap.toString();
    CPPUNIT_ASSERT_MESSAGE("should return something", result != "");
}

////////////////////////////////////////////////////////////////////////////////
void HashMapTest::testEntrySetIterator() {

    HashMap<int, std::string> map;
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
void HashMapTest::testKeySetIterator() {

    HashMap<int, std::string> map;
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
void HashMapTest::testValuesIterator() {

    HashMap<int, std::string> map;
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
