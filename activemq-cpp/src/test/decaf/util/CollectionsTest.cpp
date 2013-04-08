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

#include "CollectionsTest.h"

#include <decaf/util/ArrayList.h>
#include <decaf/util/LinkedList.h>
#include <decaf/util/Collections.h>

using namespace decaf;
using namespace decaf::util;

////////////////////////////////////////////////////////////////////////////////
CollectionsTest::CollectionsTest() {
}

////////////////////////////////////////////////////////////////////////////////
CollectionsTest::~CollectionsTest() {
}

////////////////////////////////////////////////////////////////////////////////
void CollectionsTest::testReverseList() {

    {
        ArrayList<int> list;

        for (int i = 0; i < 50; ++i) {
            list.add(i);
        }

        Collections::reverse(list);

        CPPUNIT_ASSERT_EQUAL(0, list.get(49));
        CPPUNIT_ASSERT_EQUAL(49, list.get(0));
    }

    {
        LinkedList<int> list;

        for (int i = 0; i < 50; ++i) {
            list.add(i);
        }

        Collections::reverse(list);

        CPPUNIT_ASSERT_EQUAL(0, list.getLast());
        CPPUNIT_ASSERT_EQUAL(49, list.getFirst());
    }
}
