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

#include "ExceptionTest.h"
#include <stdexcept>

using namespace std;
using namespace decaf;
using namespace decaf::lang;

////////////////////////////////////////////////////////////////////////////////
void ExceptionTest::testCtors() {

    Exception exception1;

    CPPUNIT_ASSERT( exception1.getCause() == NULL );
    CPPUNIT_ASSERT( exception1.getMessage() == "" );

    exception1.setMessage( "EXCEPTION_1" );
    CPPUNIT_ASSERT( exception1.getMessage() == "EXCEPTION_1" );

    Exception exception2( __FILE__, __LINE__, "EXCEPTION_2" );

    CPPUNIT_ASSERT( exception2.getCause() == NULL );
    CPPUNIT_ASSERT( exception2.getMessage() == "EXCEPTION_2" );

    Exception exception3( __FILE__, __LINE__, &exception1, "EXCEPTION_3" );

    CPPUNIT_ASSERT( exception3.getCause() != NULL );
    CPPUNIT_ASSERT( std::string( exception3.getCause()->what() ) == "EXCEPTION_1" );
    CPPUNIT_ASSERT( exception3.getMessage() == "EXCEPTION_3" );

    Exception exception4( exception1 );
    CPPUNIT_ASSERT( exception4.getCause() == NULL );
    CPPUNIT_ASSERT( exception4.getMessage() == "EXCEPTION_1" );

    std::runtime_error runtime( "RUNTIME" );
    Exception exception5( &runtime );
    CPPUNIT_ASSERT( exception5.getCause() != NULL );
    CPPUNIT_ASSERT( exception5.getMessage() == "RUNTIME" );
}

////////////////////////////////////////////////////////////////////////////////
void ExceptionTest::testAssign() {

    Exception exception1;

    CPPUNIT_ASSERT( exception1.getCause() == NULL );
    CPPUNIT_ASSERT( exception1.getMessage() == "" );

    exception1.setMessage( "EXCEPTION_1" );
    CPPUNIT_ASSERT( exception1.getMessage() == "EXCEPTION_1" );

    Exception exception2( __FILE__, __LINE__, "EXCEPTION_2" );

    CPPUNIT_ASSERT( exception2.getCause() == NULL );
    CPPUNIT_ASSERT( exception2.getMessage() == "EXCEPTION_2" );

    exception1 = exception2;
    CPPUNIT_ASSERT( exception1.getCause() == NULL );
    CPPUNIT_ASSERT( exception1.getMessage() == "EXCEPTION_2" );
}

////////////////////////////////////////////////////////////////////////////////
void ExceptionTest::testClone() {

    const char* text = "This is a test";
    Exception ex( __FILE__, __LINE__, text );
    CPPUNIT_ASSERT( strcmp( ex.getMessage().c_str(), text ) == 0 );
    CPPUNIT_ASSERT( ex.getCause() == NULL );

    Exception* cloned = ex.clone();

    CPPUNIT_ASSERT( strcmp( cloned->getMessage().c_str(), text ) == 0 );
    CPPUNIT_ASSERT( cloned->getCause() == NULL );

    delete cloned;
}

////////////////////////////////////////////////////////////////////////////////
void ExceptionTest::testInitCause() {

    const char* text = "This is a test";
    Exception ex( __FILE__, __LINE__, text );
    CPPUNIT_ASSERT( strcmp( ex.getMessage().c_str(), text ) == 0 );
    CPPUNIT_ASSERT( ex.getCause() == NULL );

    std::runtime_error exception1("RUNTIME");
    Exception exception2( __FILE__, __LINE__, "EXCEPTION" );

    ex.initCause( &exception1 );
    CPPUNIT_ASSERT( ex.getCause() != NULL );
    CPPUNIT_ASSERT( std::string( ex.getCause()->what() ) == "RUNTIME" );

    ex.initCause( &exception2 );
    CPPUNIT_ASSERT( ex.getCause() != NULL );
    CPPUNIT_ASSERT( std::string( ex.getCause()->what() ) == "EXCEPTION" );

    const Exception* test = dynamic_cast<const Exception*>( ex.getCause() );
    CPPUNIT_ASSERT( test != NULL );
}

////////////////////////////////////////////////////////////////////////////////
void ExceptionTest::testMessage0(){
      const char* text = "This is a test";
      Exception ex( __FILE__, __LINE__, text );
      CPPUNIT_ASSERT( strcmp( ex.getMessage().c_str(), text ) == 0 );
}

////////////////////////////////////////////////////////////////////////////////
void ExceptionTest::testMessage3(){
    Exception ex( __FILE__, __LINE__,
        "This is a test %d %d %d", 1, 100, 1000 );
    CPPUNIT_ASSERT( strcmp( ex.getMessage().c_str(),
                    "This is a test 1 100 1000" ) == 0 );
}
