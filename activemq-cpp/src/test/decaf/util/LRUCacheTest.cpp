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

#include "LRUCacheTest.h"

#include <decaf/util/LRUCache.h>
#include <decaf/lang/System.h>

#include <string>

using namespace std;
using namespace decaf;
using namespace decaf::util;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
LRUCacheTest::LRUCacheTest() {
}

////////////////////////////////////////////////////////////////////////////////
LRUCacheTest::~LRUCacheTest() {
}

////////////////////////////////////////////////////////////////////////////////
void LRUCacheTest::testConstructor() {

    LRUCache<int, int> underTest(1000);

    for (int count = 0; count < 5000; count++) {
        if (!underTest.containsKey(count)) {
            underTest.put(count, count);
        }
    }
    CPPUNIT_ASSERT_EQUAL_MESSAGE("size is still in order", 1000, underTest.size());
}

////////////////////////////////////////////////////////////////////////////////
void LRUCacheTest::testExceptions() {

    try {
        LRUCache<int, int> underTest(-1);
        CPPUNIT_FAIL("Should have thrown an IllegalArgumentException");
    } catch(IllegalArgumentException& ex) {}

    LRUCache<int, int> underTest(1000);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IllegalArgumentException",
        underTest.setMaxCacheSize(-1),
        IllegalArgumentException );
}

////////////////////////////////////////////////////////////////////////////////
void LRUCacheTest::testChangeMaxCacheSize() {

    LRUCache<int, int> underTest(1000);

    for (int count = 0; count < 5000; count++) {
        if (!underTest.containsKey(count)) {
            underTest.put(count, count);
        }
    }

    CPPUNIT_ASSERT_EQUAL_MESSAGE("size is still in order", 1000, underTest.size());
    underTest.setMaxCacheSize(2000);

    for (int count = 0; count < 5000; count++) {
        if (!underTest.containsKey(count)) {
            underTest.put(count, count);
        }
    }

    CPPUNIT_ASSERT_EQUAL_MESSAGE("size is still in order", 2000, underTest.size());
}

