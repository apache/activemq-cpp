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

#include "IntegerTest.h"

using namespace std;
using namespace decaf;
using namespace decaf::lang;

void IntegerTest::test(void)
{
    int x = Integer::parseInt("12");
    int y = Integer::parseInt("FF", 16);
    int z = Integer::parseInt("42");

    CPPUNIT_ASSERT( x == 12 );
    CPPUNIT_ASSERT( y == 255 );
    CPPUNIT_ASSERT( z == 42 );

    std::string x1 = Integer::toString( x );
    std::string y1 = Integer::toString( y );
    std::string z1 = Integer::toString( z );

    CPPUNIT_ASSERT( x1 == "12" );
    CPPUNIT_ASSERT( y1 == "255" );
    CPPUNIT_ASSERT( z1 == "42" );

}
