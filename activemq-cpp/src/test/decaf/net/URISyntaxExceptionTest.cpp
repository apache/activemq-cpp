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

#include "URISyntaxExceptionTest.h"

#include <decaf/net/URISyntaxException.h>

using namespace decaf;
using namespace decaf::net;

////////////////////////////////////////////////////////////////////////////////
URISyntaxExceptionTest::URISyntaxExceptionTest() {
}

////////////////////////////////////////////////////////////////////////////////
void URISyntaxExceptionTest::test() {

    URISyntaxException e1( __FILE__, __LINE__, "str", "problem", 2);
    CPPUNIT_ASSERT_MESSAGE("returned incorrect reason",
                           e1.getReason() == "problem" );
    CPPUNIT_ASSERT_MESSAGE("returned incorrect input", e1.getInput() == "str" );
    CPPUNIT_ASSERT_MESSAGE("returned incorrect index", 2 == e1.getIndex());

    URISyntaxException e2( __FILE__, __LINE__, "str", "problem");
    CPPUNIT_ASSERT_MESSAGE("returned incorrect reason", e2.getReason() == "problem" );
    CPPUNIT_ASSERT_MESSAGE("returned incorrect input", e2.getInput() == "str" );
    CPPUNIT_ASSERT_MESSAGE("returned incorrect index", -1 == e2.getIndex());
}
