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

#include "HashSetTest.h"

#include <decaf/util/HashSet.h>
#include <decaf/util/Iterator.h>
#include <decaf/util/HashMap.h>
#include <decaf/util/StlMap.h>
#include <decaf/util/ArrayList.h>
#include <decaf/util/LinkedList.h>
#include <decaf/lang/Integer.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>

using namespace std;
using namespace decaf;
using namespace decaf::util;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
namespace {

    const int SET_SIZE = 1000;

    void populateSet(HashSet<int>& hashSet) {
        for (int i = 0; i < SET_SIZE; ++i) {
            hashSet.add(i);
        }
    }

    void populateSet(HashSet<int>& hashSet, int count) {
        for (int i = 0; i < count; ++i) {
            hashSet.add(i);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
HashSetTest::HashSetTest() {
}

////////////////////////////////////////////////////////////////////////////////
HashSetTest::~HashSetTest() {
}

////////////////////////////////////////////////////////////////////////////////
void HashSetTest::testConstructor() {

    HashSet<int> set;
    CPPUNIT_ASSERT(set.isEmpty());
    CPPUNIT_ASSERT_EQUAL(0, set.size());
    CPPUNIT_ASSERT_EQUAL(false, set.contains(1));
}

////////////////////////////////////////////////////////////////////////////////
void HashSetTest::testConstructorI() {

    HashSet<int> set;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Created incorrect HashSet", 0, set.size());

    try {
        HashSet<int> set(-1);
    } catch (IllegalArgumentException& e) {
        return;
    }

    CPPUNIT_FAIL("Failed to throw IllegalArgumentException for capacity < 0");
}

////////////////////////////////////////////////////////////////////////////////
void HashSetTest::testConstructorIF() {

    HashSet<int> set(5, 0.5);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Created incorrect HashSet", 0, set.size());

    try {
        HashSet<int> set(0, 0);
    } catch (IllegalArgumentException& e) {
        return;
    }

    CPPUNIT_FAIL("Failed to throw IllegalArgumentException for initial load factor <= 0");
}

////////////////////////////////////////////////////////////////////////////////
void HashSetTest::testConstructorCollection() {

    ArrayList<int> intList;
    intList.add(1);
    intList.add(1);
    intList.add(2);
    intList.add(3);
    intList.add(4);

    HashSet<int> set(intList);
    for (int counter = 0; counter < intList.size(); counter++) {
        CPPUNIT_ASSERT_MESSAGE("HashSet does not contain correct elements",
                               set.contains(intList.get(counter)));
    }

    CPPUNIT_ASSERT_MESSAGE("HashSet created from collection incorrect size",
                           set.size() == intList.size() - 1);
}

//////////////////////////////////////////////////////////////////////////////
void HashSetTest::testEquals() {

    HashSet<int> set1;
    populateSet(set1);
    HashSet<int> set2;
    populateSet(set2);

    CPPUNIT_ASSERT(set1.equals(set2));
    CPPUNIT_ASSERT(set2.equals(set1));

    set1.add(SET_SIZE + 1);
    CPPUNIT_ASSERT(!set1.equals(set2));
    CPPUNIT_ASSERT(!set2.equals(set1));
    set2.add(SET_SIZE + 1);
    CPPUNIT_ASSERT(set1.equals(set2));
    CPPUNIT_ASSERT(set2.equals(set1));
}

////////////////////////////////////////////////////////////////////////////////
void HashSetTest::testAdd() {

    HashSet<int> set;
    populateSet(set);
    int size = set.size();

    set.add(8);
    CPPUNIT_ASSERT_MESSAGE("Added element already contained by set", set.size() == size);
    set.add(-9);
    CPPUNIT_ASSERT_MESSAGE("Failed to increment set size after add", set.size() == size + 1);
    CPPUNIT_ASSERT_MESSAGE("Failed to add element to set", set.contains(-9));
}

////////////////////////////////////////////////////////////////////////////////
void HashSetTest::testClear() {

    HashSet<int> set;
    populateSet(set);

    CPPUNIT_ASSERT(set.size() > 0);
    set.clear();
    CPPUNIT_ASSERT(set.size() == 0);
    CPPUNIT_ASSERT(!set.contains(1));
}

////////////////////////////////////////////////////////////////////////////////
void HashSetTest::testContains() {

    HashSet<int> set;
    populateSet(set);

    CPPUNIT_ASSERT_MESSAGE("Returned false for valid object", set.contains(90));
    CPPUNIT_ASSERT_MESSAGE("Returned true for invalid Object", !set.contains(SET_SIZE + 1));
}

////////////////////////////////////////////////////////////////////////////////
void HashSetTest::testIsEmpty() {
    HashSet<int> set;
    CPPUNIT_ASSERT_MESSAGE("Empty set returned true", set.isEmpty());
    set.add(1);
    CPPUNIT_ASSERT_MESSAGE("Non-empty set returned true", !set.isEmpty());

    CPPUNIT_ASSERT_MESSAGE("Empty set returned false", HashSet<std::string>().isEmpty());
}

////////////////////////////////////////////////////////////////////////////////
void HashSetTest::testIterator() {

    HashSet<int> set;
    populateSet(set);
    Pointer< Iterator<int> > iter(set.iterator());
    int x = 0;
    while (iter->hasNext()) {
        CPPUNIT_ASSERT_MESSAGE("Failed to iterate over all elements", set.contains(iter->next()));
        ++x;
    }
    CPPUNIT_ASSERT_MESSAGE("Returned iteration of incorrect size", set.size() == x);

    {
        HashSet<string> set;

        set.add( "fred1" );
        set.add( "fred2" );
        set.add( "fred3" );

        Iterator<string>* iterator1 = set.iterator();
        CPPUNIT_ASSERT( iterator1 != NULL );
        CPPUNIT_ASSERT( iterator1->hasNext() == true );

        int count = 0;
        while( iterator1->hasNext() ) {
            iterator1->next();
            ++count;
        }

        CPPUNIT_ASSERT( count == set.size() );

        Iterator<string>* iterator2 = set.iterator();

        while( iterator2->hasNext() ) {
            iterator2->next();
            iterator2->remove();
        }

        CPPUNIT_ASSERT( set.isEmpty() );

        delete iterator1;
        delete iterator2;
    }
}

////////////////////////////////////////////////////////////////////////////////
void HashSetTest::testRemove() {

    HashSet<int> set;
    populateSet(set);
    int size = set.size();
    set.remove(98);
    CPPUNIT_ASSERT_MESSAGE("Failed to remove element", !set.contains(98));
    CPPUNIT_ASSERT_MESSAGE("Failed to decrement set size", set.size() == size - 1);
}

////////////////////////////////////////////////////////////////////////////////
void HashSetTest::testSize() {

    HashSet<int> set;
    populateSet(set);

    CPPUNIT_ASSERT_MESSAGE("Returned incorrect size", set.size() == SET_SIZE);
    set.clear();
    CPPUNIT_ASSERT_MESSAGE("Cleared set returned non-zero size", 0 == set.size());
}

////////////////////////////////////////////////////////////////////////////////
void HashSetTest::testToString() {
    HashSet<std::string> s;
    std::string result = s.toString();
    CPPUNIT_ASSERT_MESSAGE("toString returned bad value", result.find("HashSet") != std::string::npos);
}

////////////////////////////////////////////////////////////////////////////////
void HashSetTest::testToArray() {

    HashSet<int> set;
    populateSet(set);

    std::vector<int> array = set.toArray();
    CPPUNIT_ASSERT((int)array.size() == SET_SIZE);
}

////////////////////////////////////////////////////////////////////////////////
void HashSetTest::testCopy1() {

    HashSet<int> set1;

    for (int i = 0; i < 50; ++i) {
        set1.add(i);
    }

    HashSet<int> set2;

    set2.copy(set1);

    CPPUNIT_ASSERT(set1.size() == set2.size());

    for (int i = 0; i < 50; ++i) {
        CPPUNIT_ASSERT(set2.contains(i));
    }

    CPPUNIT_ASSERT(set2.equals(set1));
}

////////////////////////////////////////////////////////////////////////////////
void HashSetTest::testCopy2() {

    LinkedList<int> collection;

    for (int i = 0; i < 50; ++i) {
        collection.add(i);
    }

    HashSet<int> set;

    set.copy(collection);

    CPPUNIT_ASSERT(collection.size() == set.size());

    for (int i = 0; i < 50; ++i) {
        CPPUNIT_ASSERT(set.contains(i));
    }

    CPPUNIT_ASSERT(set.equals(collection));
}

//////////////////////////////////////////////////////////////////////////////
void HashSetTest::testRemoveAll() {

    HashSet<int> set;
    populateSet(set, 3);

    ArrayList<int> collection;
    collection.add(1);
    collection.add(2);

    set.removeAll(collection);

    CPPUNIT_ASSERT_EQUAL(1, set.size());

    HashSet<int> set2;
    set2.removeAll(collection);
    CPPUNIT_ASSERT_EQUAL(0, set2.size());

    HashSet<int> set3;
    populateSet(set3, 3);
    collection.clear();

    set3.removeAll(collection);
    CPPUNIT_ASSERT_EQUAL(3, set3.size());
}

//////////////////////////////////////////////////////////////////////////////
void HashSetTest::testRetainAll() {

    HashSet<int> set;
    populateSet(set, 3);

    ArrayList<int> collection;
    collection.add(1);
    collection.add(2);

    set.retainAll(collection);

    CPPUNIT_ASSERT_EQUAL(2, set.size());

    HashSet<int> set2;
    set2.retainAll(collection);
    CPPUNIT_ASSERT_EQUAL(0, set2.size());

    HashSet<int> set3;
    populateSet(set3, 3);
    collection.clear();

    set3.retainAll(collection);
    CPPUNIT_ASSERT_EQUAL(0, set3.size());
}

