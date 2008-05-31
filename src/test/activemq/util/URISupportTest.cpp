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

#include "URISupportTest.h"

#include <activemq/util/URISupport.h>
#include <decaf/util/Properties.h>
#include <decaf/lang/System.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>

using namespace std;
using namespace decaf;
using namespace decaf::util;
using namespace activemq;
using namespace activemq::util;

////////////////////////////////////////////////////////////////////////////////
void URISupportTest::test() {

    string test = "?option1=test1&option2=test2";

    Properties map = URISupport::parseQuery( test );

    CPPUNIT_ASSERT( map.hasProperty( "option1" ) == true );
    CPPUNIT_ASSERT( map.hasProperty( "option2" ) == true );

    CPPUNIT_ASSERT( map.getProperty( "option1", "" ) == "test1" );
    CPPUNIT_ASSERT( map.getProperty( "option2", "" ) == "test2" );

    string test2 = "option&option";

    try{
        map = URISupport::parseQuery( test2 );
        CPPUNIT_ASSERT( false );
    } catch(...) {}

    string test3 = "option1=test1&option2=test2";

    map = URISupport::parseQuery( test3 );

    CPPUNIT_ASSERT( map.hasProperty( "option1" ) == true );
    CPPUNIT_ASSERT( map.hasProperty( "option2" ) == true );

    CPPUNIT_ASSERT( map.getProperty( "option1", "" ) == "test1" );
    CPPUNIT_ASSERT( map.getProperty( "option2", "" ) == "test2" );

    string test4 = "http://cause.exception.com?option1=test1&option2=test2";

    try{
        map = URISupport::parseQuery( test4 );
    } catch(...) {
        CPPUNIT_ASSERT( false );
    }
}

////////////////////////////////////////////////////////////////////////////////
void URISupportTest::testURIParseEnv() {

    string test = "tcp://localhost:61616?option1=test1&option2=test2";

    Properties map;

    URISupport::parseURL( test, map );

    CPPUNIT_ASSERT( map.hasProperty( "option1" ) == true );
    CPPUNIT_ASSERT( map.hasProperty( "option2" ) == true );

    CPPUNIT_ASSERT( map.getProperty( "option1", "" ) == "test1" );
    CPPUNIT_ASSERT( map.getProperty( "option2", "" ) == "test2" );

    decaf::lang::System::setenv( "TEST_CPP_AMQ", "test2" );

    test = "tcp://localhost:61616?option1=test1&option2=${TEST_CPP_AMQ}";
    map.clear();
    URISupport::parseURL( test, map );
    CPPUNIT_ASSERT( map.hasProperty( "option2" ) == true );
    CPPUNIT_ASSERT( map.getProperty( "option2", "" ) == "test2" );

    test = "tcp://localhost:61616?option1=test1&option2=${TEST_CPP_AMQ_XXX}";
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should Throw an IllegalArgumentException",
        URISupport::parseURL( test, map ),
        decaf::lang::exceptions::IllegalArgumentException );

    test = "tcp://localhost:61616?option1=test1&option2=${}";
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should Throw an IllegalArgumentException",
        URISupport::parseURL( test, map ),
        decaf::lang::exceptions::IllegalArgumentException );

    test = "tcp://localhost:61616?option1=test1&option2=$X}";
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should Throw an IllegalArgumentException",
        URISupport::parseURL( test, map ),
        decaf::lang::exceptions::IllegalArgumentException );

    test = "tcp://localhost:61616?option1=test1&option2=${X";
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should Throw an IllegalArgumentException",
        URISupport::parseURL( test, map ),
        decaf::lang::exceptions::IllegalArgumentException );

    test = "tcp://localhost:61616?option1=test1&option2=$X";
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should Throw an IllegalArgumentException",
        URISupport::parseURL( test, map ),
        decaf::lang::exceptions::IllegalArgumentException );
}
