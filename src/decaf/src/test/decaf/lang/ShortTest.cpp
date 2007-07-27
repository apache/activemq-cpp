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

#include "ShortTest.h"

using namespace std;
using namespace decaf;
using namespace decaf::lang;

////////////////////////////////////////////////////////////////////////////////
ShortTest::ShortTest() {
}

////////////////////////////////////////////////////////////////////////////////
void ShortTest::test() {

    Short short1( 65 );

    CPPUNIT_ASSERT( short1.byteValue() == 65 );
    CPPUNIT_ASSERT( short1.shortValue() == 65 );
    CPPUNIT_ASSERT( short1.intValue() == 65 );
    CPPUNIT_ASSERT( short1.longValue() == 65 );
    CPPUNIT_ASSERT( short1.floatValue() == 65.0f );
    CPPUNIT_ASSERT( short1.doubleValue() == 65.0 );

    //CPPUNIT_ASSERT( short1.toString() == "65" );
    //CPPUNIT_ASSERT( short1.toString( 99 ) == "99" );

}

