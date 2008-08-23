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

#include "ActiveMQExceptionTest.h"
#include <activemq/exceptions/ExceptionDefines.h>
#include <decaf/lang/Exception.h>
#include <decaf/lang/exceptions/UnsupportedOperationException.h>

using namespace activemq;
using namespace activemq::exceptions;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
void ActiveMQExceptionTest::testMacros() {

    try{

        try{
            try{
                throw UnsupportedOperationException( __FILE__, __LINE__, "EXCEPTION" );
                CPPUNIT_FAIL( "Should not get this far." );
            }
            AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
        }
        AMQ_CATCH_RETHROW( ActiveMQException )

    } catch( ActiveMQException& ex ) {

        CPPUNIT_ASSERT( ex.getCause() != NULL );

        const UnsupportedOperationException* cause =
            dynamic_cast<const UnsupportedOperationException*>( ex.getCause() );

        CPPUNIT_ASSERT( cause != NULL );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQExceptionTest::testMessage0(){
    char* text = "This is a test";
      ActiveMQException ex( __FILE__, __LINE__, text );
      CPPUNIT_ASSERT( strcmp( ex.getMessage().c_str(), text ) == 0 );
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQExceptionTest::testMessage3(){
    ActiveMQException ex( __FILE__, __LINE__,
        "This is a test %d %d %d", 1, 100, 1000 );
    CPPUNIT_ASSERT( strcmp( ex.getMessage().c_str(),
                    "This is a test 1 100 1000" ) == 0 );
}
