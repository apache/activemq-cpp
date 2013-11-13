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

#include "StringBufferTest.h"

#include <decaf/lang/String.h>
#include <decaf/lang/StringBuffer.h>
#include <decaf/lang/exceptions/IndexOutOfBoundsException.h>
#include <decaf/lang/exceptions/NegativeArraySizeException.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/exceptions/StringIndexOutOfBoundsException.h>

using namespace std;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
StringBufferTest::StringBufferTest() {
}

////////////////////////////////////////////////////////////////////////////////
StringBufferTest::~StringBufferTest() {
}

////////////////////////////////////////////////////////////////////////////////
void StringBufferTest::testDefaultConstructor() {
    StringBuffer buffer;
    CPPUNIT_ASSERT_EQUAL(16, buffer.capacity());
}

////////////////////////////////////////////////////////////////////////////////
void StringBufferTest::testConstructorInt() {

    StringBuffer sb(24);
    CPPUNIT_ASSERT_EQUAL(24, sb.capacity());

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a NegativeArraySizeException",
        StringBuffer(-1),
        NegativeArraySizeException);

    CPPUNIT_ASSERT_NO_THROW(StringBuffer(0));
}
