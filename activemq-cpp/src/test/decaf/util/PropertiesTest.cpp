/**
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "PropertiesTest.h"

#include <memory>
#include <decaf/util/Properties.h>

using namespace std;
using namespace decaf;
using namespace decaf::util;

////////////////////////////////////////////////////////////////////////////////
void PropertiesTest::testPutAndGet() {

    Properties properties;

    CPPUNIT_ASSERT( properties.isEmpty() == true );
    CPPUNIT_ASSERT( properties.getProperty( "Bob" ) == NULL );
    CPPUNIT_ASSERT( properties.getProperty( "Bob", "Steve" ) == "Steve" );

    CPPUNIT_ASSERT( properties.size() == 0 );

    properties.setProperty( "Bob", "Foo" );
    CPPUNIT_ASSERT( properties.isEmpty() == false );

    CPPUNIT_ASSERT( properties.hasProperty( "Steve" ) == false );
    properties.setProperty( "Steve", "Foo" );
    CPPUNIT_ASSERT( properties.hasProperty( "Steve" ) == true );

    CPPUNIT_ASSERT( properties.size() == 2 );

    CPPUNIT_ASSERT( properties.getProperty( "Bob" ) == std::string( "Foo" ) );
    CPPUNIT_ASSERT( properties.getProperty( "Bob", "Steve" ) == "Foo" );
}

////////////////////////////////////////////////////////////////////////////////
void PropertiesTest::testAssign() {

    Properties properties1;
    Properties properties2;

    properties1.setProperty( "A", "A" );
    properties1.setProperty( "B", "B" );
    properties1.setProperty( "C", "C" );
    properties1.setProperty( "D", "D" );

    CPPUNIT_ASSERT( properties2.hasProperty( "A" ) == false );
    CPPUNIT_ASSERT( properties2.hasProperty( "B" ) == false );
    CPPUNIT_ASSERT( properties2.hasProperty( "C" ) == false );
    CPPUNIT_ASSERT( properties2.hasProperty( "D" ) == false );

    properties2 = properties1;

    CPPUNIT_ASSERT( properties2.hasProperty( "A" ) == true );
    CPPUNIT_ASSERT( properties2.hasProperty( "B" ) == true );
    CPPUNIT_ASSERT( properties2.hasProperty( "C" ) == true );
    CPPUNIT_ASSERT( properties2.hasProperty( "D" ) == true );

    CPPUNIT_ASSERT( properties1.size() == properties2.size() );
}

////////////////////////////////////////////////////////////////////////////////
void PropertiesTest::testCopy() {

    Properties properties2;
    Properties properties1;

    properties1.setProperty( "A", "A" );
    properties1.setProperty( "B", "B" );
    properties1.setProperty( "C", "C" );
    properties1.setProperty( "D", "D" );

    CPPUNIT_ASSERT( properties2.hasProperty( "A" ) == false );
    CPPUNIT_ASSERT( properties2.hasProperty( "B" ) == false );
    CPPUNIT_ASSERT( properties2.hasProperty( "C" ) == false );
    CPPUNIT_ASSERT( properties2.hasProperty( "D" ) == false );

    properties2.copy( properties1 );

    CPPUNIT_ASSERT( properties2.hasProperty( "A" ) == true );
    CPPUNIT_ASSERT( properties2.hasProperty( "B" ) == true );
    CPPUNIT_ASSERT( properties2.hasProperty( "C" ) == true );
    CPPUNIT_ASSERT( properties2.hasProperty( "D" ) == true );

    CPPUNIT_ASSERT( properties1.size() == properties2.size() );
}

////////////////////////////////////////////////////////////////////////////////
void PropertiesTest::testClone() {

    Properties properties1;
    std::auto_ptr<Properties> properties2;

    properties1.setProperty( "A", "A" );
    properties1.setProperty( "B", "B" );
    properties1.setProperty( "C", "C" );
    properties1.setProperty( "D", "D" );

    properties2.reset( properties1.clone() );

    CPPUNIT_ASSERT( properties2->hasProperty( "A" ) == true );
    CPPUNIT_ASSERT( properties2->hasProperty( "B" ) == true );
    CPPUNIT_ASSERT( properties2->hasProperty( "C" ) == true );
    CPPUNIT_ASSERT( properties2->hasProperty( "D" ) == true );

    CPPUNIT_ASSERT( properties1.size() == properties2->size() );
}

////////////////////////////////////////////////////////////////////////////////
void PropertiesTest::testRemove() {

    Properties properties1;

    properties1.setProperty( "A", "A" );
    properties1.setProperty( "B", "B" );
    properties1.setProperty( "C", "C" );
    properties1.setProperty( "D", "D" );

    properties1.remove( "A" );
    properties1.remove( "C" );

    CPPUNIT_ASSERT( properties1.hasProperty( "A" ) == false );
    CPPUNIT_ASSERT( properties1.hasProperty( "B" ) == true );
    CPPUNIT_ASSERT( properties1.hasProperty( "C" ) == false );
    CPPUNIT_ASSERT( properties1.hasProperty( "D" ) == true );

    CPPUNIT_ASSERT( properties1.size() == 2 );
}

////////////////////////////////////////////////////////////////////////////////
void PropertiesTest::testClear() {

    Properties properties1;

    properties1.setProperty( "A", "A" );
    properties1.setProperty( "B", "B" );
    properties1.setProperty( "C", "C" );
    properties1.setProperty( "D", "D" );

    CPPUNIT_ASSERT( properties1.size() == 4 );
    CPPUNIT_ASSERT( properties1.isEmpty() == false );

    properties1.clear();

    CPPUNIT_ASSERT( properties1.size() == 0 );
    CPPUNIT_ASSERT( properties1.isEmpty() == true );
}
