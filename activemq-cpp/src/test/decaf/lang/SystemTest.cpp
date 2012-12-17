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

#include "SystemTest.h"

#include <decaf/lang/System.h>
#include <decaf/util/StlMap.h>
#include <decaf/lang/Thread.h>

using namespace std;
using namespace decaf;
using namespace decaf::util;
using namespace decaf::lang;

////////////////////////////////////////////////////////////////////////////////
SystemTest::SystemTest() {
}

////////////////////////////////////////////////////////////////////////////////
void SystemTest::test_availableProcessors() {

    CPPUNIT_ASSERT( System::availableProcessors() >= 1 );
}

////////////////////////////////////////////////////////////////////////////////
void SystemTest::test_getenv() {

    CPPUNIT_ASSERT( System::getenv( "PATH" ) != "" );

    try {
        System::getenv( "PATH_ASDFGHJKL" );
        CPPUNIT_ASSERT( false );
    } catch (...) {
    }
}

////////////////////////////////////////////////////////////////////////////////
void SystemTest::test_getenv2() {

    StlMap<std::string, std::string> values = System::getenv();

    CPPUNIT_ASSERT( values.size() != 0 );
    CPPUNIT_ASSERT( values.containsKey( "PATH" ) || values.containsKey( "Path" ) );
    CPPUNIT_ASSERT( !values.containsKey( "PATH_ASDFGHJKL" ) );
}

////////////////////////////////////////////////////////////////////////////////
void SystemTest::test_setenv() {

    StlMap<std::string, std::string> values1 = System::getenv();
    CPPUNIT_ASSERT( !values1.containsKey( "PATH_ASDFGHJKL" ) );
    System::setenv( "PATH_ASDFGHJKL", "test" );
    StlMap<std::string, std::string> values2 = System::getenv();
    CPPUNIT_ASSERT( values2.containsKey( "PATH_ASDFGHJKL" ) );
    System::unsetenv( "PATH_ASDFGHJKL" );
}

////////////////////////////////////////////////////////////////////////////////
void SystemTest::test_unsetenv() {

    StlMap<std::string, std::string> values1 = System::getenv();
    CPPUNIT_ASSERT( !values1.containsKey( "PATH_ASDFGHJKL" ) );
    System::setenv( "PATH_ASDFGHJKL", "test" );
    StlMap<std::string, std::string> values2 = System::getenv();
    CPPUNIT_ASSERT( values2.containsKey( "PATH_ASDFGHJKL" ) );
    System::unsetenv( "PATH_ASDFGHJKL" );
    StlMap<std::string, std::string> values3 = System::getenv();
    CPPUNIT_ASSERT( !values3.containsKey( "PATH_ASDFGHJKL" ) );
}

////////////////////////////////////////////////////////////////////////////////
void SystemTest::test_currentTimeMillis() {
    CPPUNIT_ASSERT( System::currentTimeMillis() != 0 );

    long long start = System::currentTimeMillis();
    Thread::sleep( 150 );
    long long end = System::currentTimeMillis();

    CPPUNIT_ASSERT_MESSAGE( "First Read isn't less than the second.", start < end );
}

////////////////////////////////////////////////////////////////////////////////
void SystemTest::test_nanoTime() {
    CPPUNIT_ASSERT( System::nanoTime() != 0 );

    long long start = System::nanoTime();
    Thread::sleep( 150 );
    long long end = System::nanoTime();

    CPPUNIT_ASSERT_MESSAGE( "First Read isn't less than the second.", start < end );
}
