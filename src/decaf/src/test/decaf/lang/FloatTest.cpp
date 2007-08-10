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

#include "FloatTest.h"

#include <decaf/lang/Float.h>
#include <string>

using namespace std;
using namespace decaf;
using namespace decaf::lang;

////////////////////////////////////////////////////////////////////////////////
FloatTest::FloatTest() {
}

////////////////////////////////////////////////////////////////////////////////
void FloatTest::test_ConstructorF() {
    // Test for method decaf.lang.Float(float)
    Float f( 900.89f );
    CPPUNIT_ASSERT_MESSAGE( "Created incorrect float", f.floatValue() == 900.89f );
}

////////////////////////////////////////////////////////////////////////////////
void FloatTest::test_ConstructorString() {
    // Test for method decaf.lang.Float(decaf.lang.String)
    Float f( "900.89" );
    CPPUNIT_ASSERT_MESSAGE("Created incorrect Float", f.floatValue() == 900.89f);
}
