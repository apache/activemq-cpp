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
#include <decaf/io/ByteArrayInputStream.h>
#include <decaf/io/ByteArrayOutputStream.h>

using namespace std;
using namespace decaf;
using namespace decaf::util;
using namespace decaf::io;

////////////////////////////////////////////////////////////////////////////////
void PropertiesTest::setUp() {

    this->testProperties.setProperty( "test.prop", "this is a test property" );
    this->testProperties.setProperty( "bogus.prop", "bogus" );
}

////////////////////////////////////////////////////////////////////////////////
void PropertiesTest::tearDown() {
}

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

////////////////////////////////////////////////////////////////////////////////
void PropertiesTest::testEquals() {

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

    CPPUNIT_ASSERT( !properties2.equals( properties1 ) );

    properties2.copy( properties1 );

    CPPUNIT_ASSERT( properties2.hasProperty( "A" ) == true );
    CPPUNIT_ASSERT( properties2.hasProperty( "B" ) == true );
    CPPUNIT_ASSERT( properties2.hasProperty( "C" ) == true );
    CPPUNIT_ASSERT( properties2.hasProperty( "D" ) == true );

    CPPUNIT_ASSERT( properties2.equals( properties1 ) );
}

////////////////////////////////////////////////////////////////////////////////
void PropertiesTest::testLoadNPE() {

    Properties properties;
    decaf::io::InputStream* nullStream = NULL;
    decaf::io::Reader* nullReader = NULL;

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a NullPointerException",
        properties.load( nullStream ),
        decaf::lang::exceptions::NullPointerException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a NullPointerException",
        properties.load( nullReader ),
        decaf::lang::exceptions::NullPointerException );
}

////////////////////////////////////////////////////////////////////////////////
void PropertiesTest::testLoadInputStream() {

    {
        Properties properties;
        string value(" a= b");
        ByteArrayInputStream stream( (const unsigned char*)value.c_str(), (int)value.size() );
        properties.load( &stream );
        CPPUNIT_ASSERT( string( "b" ) == properties.getProperty( "a" ) );
    }

    {
        Properties properties;
        string value(" a b");
        ByteArrayInputStream stream( (const unsigned char*)value.c_str(), (int)value.size() );
        properties.load( &stream );
        CPPUNIT_ASSERT( string( "b" ) == properties.getProperty( "a" ) );
    }

    {
        Properties properties;
        string value("#comment\na=value");
        ByteArrayInputStream stream( (const unsigned char*)value.c_str(), (int)value.size() );
        properties.load( &stream );
        CPPUNIT_ASSERT( string( "value" ) == properties.getProperty( "a" ) );
    }

    {
        Properties properties;
        string value("#properties file\r\nfred=1\r\n#last comment");
        ByteArrayInputStream stream( (const unsigned char*)value.c_str(), (int)value.size() );
        properties.load( &stream );
        CPPUNIT_ASSERT( string( "1" ) == properties.getProperty( "fred" ) );
    }

}

////////////////////////////////////////////////////////////////////////////////
void PropertiesTest::testPropertyNames() {

    Properties myProps( this->testProperties );

    std::vector<std::string> names = myProps.propertyNames();
    std::vector<std::string>::const_iterator name = names.begin();
    int i = 0;

    CPPUNIT_ASSERT( names.size() == 2 );

    for( ; name != names.end(); ++name, ++i ) {
        CPPUNIT_ASSERT_MESSAGE( "Incorrect names returned",
                                *name == "test.prop" || *name == "bogus.prop" );
    }
}

////////////////////////////////////////////////////////////////////////////////
void PropertiesTest::testPropertyNamesOverride() {

    Properties props( this->testProperties );
    props.setProperty( "test.prop", "anotherValue" );
    props.setProperty( "3rdKey", "3rdValue" );
    std::vector<string> set = props.propertyNames();
    CPPUNIT_ASSERT( 3 == set.size() );
    CPPUNIT_ASSERT( std::find( set.begin(), set.end(), "test.prop" ) != set.end() );
    CPPUNIT_ASSERT( std::find( set.begin(), set.end(), "bogus.prop") != set.end() );
    CPPUNIT_ASSERT( std::find( set.begin(), set.end(), "3rdKey" ) != set.end() );
}

////////////////////////////////////////////////////////////////////////////////
void PropertiesTest::testPropertyNamesScenario1() {

    string keys[] = { "key1", "key2", "key3" };
    string values[] = { "value1", "value2", "value3" };

    std::set<string> keyList;

    Properties properties;
    for( int index = 0; index < 3; index++ ) {
        properties.setProperty( keys[index], values[index] );
        keyList.insert( keyList.begin(), keys[index] );
    }

    Properties properties2( properties );
    std::vector<string> nameSet = properties.propertyNames();
    CPPUNIT_ASSERT_EQUAL( 3, (int)nameSet.size() );
    std::vector<string>::const_iterator iterator = nameSet.begin();
    for( ; iterator != nameSet.end(); ++iterator ) {
        CPPUNIT_ASSERT( keyList.find( *iterator ) != keyList.end() );
    }

    Properties properties3( properties2 );
    nameSet = properties2.propertyNames();
    CPPUNIT_ASSERT_EQUAL( 3, (int)nameSet.size() );
    iterator = nameSet.begin();
    for( ; iterator != nameSet.end(); ++iterator ) {
        CPPUNIT_ASSERT( keyList.find( *iterator ) != keyList.end() );
    }
}

////////////////////////////////////////////////////////////////////////////////
void PropertiesTest::testStoreOutputStream() {

    Properties myProps;
    Properties myProps2;

    myProps.setProperty( "Property A", " aye\\\f\t\n\r" );
    myProps.setProperty( "Property B", "b ee#!=:" );
    myProps.setProperty( "Property C", "see" );

    try {

        ByteArrayOutputStream out;
        myProps.store( &out, "A Header" );
        out.close();

        std::pair<const unsigned char*, int> array = out.toByteArray();
        ByteArrayInputStream in( array.first, array.second, true );
        myProps2.load( &in );
        in.close();

    } catch( IOException& ioe ) {
        CPPUNIT_FAIL( string("IOException occurred reading/writing file : ") + ioe.getMessage() );
    }

    CPPUNIT_ASSERT( myProps.size() == myProps2.size() );
    std::vector<string> nameSet = myProps.propertyNames();
    std::vector<string>::const_iterator iterator = nameSet.begin();
    for( ; iterator != nameSet.end(); ++iterator ) {
        CPPUNIT_ASSERT( string( myProps2.getProperty( *iterator ) ) == string( myProps.getProperty( *iterator ) ) );
    }
}
