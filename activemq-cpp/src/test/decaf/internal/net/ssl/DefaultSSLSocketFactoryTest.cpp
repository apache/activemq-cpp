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

#include "DefaultSSLSocketFactoryTest.h"

#include <decaf/internal/net/ssl/DefaultSSLSocketFactory.h>

#include <decaf/io/IOException.h>

using namespace std;
using namespace decaf::lang;
using namespace decaf::io;
using namespace decaf::net;
using namespace decaf::net::ssl;
using namespace decaf::internal;
using namespace decaf::internal::net;
using namespace decaf::internal::net::ssl;

////////////////////////////////////////////////////////////////////////////////
DefaultSSLSocketFactoryTest::DefaultSSLSocketFactoryTest() {
}

////////////////////////////////////////////////////////////////////////////////
DefaultSSLSocketFactoryTest::~DefaultSSLSocketFactoryTest() {
}

////////////////////////////////////////////////////////////////////////////////
void DefaultSSLSocketFactoryTest::testConstructor() {

    DefaultSSLSocketFactory factory( "Error Message" );

    try{
        factory.createSocket();
        CPPUNIT_FAIL( "Should have thrown an Exception" );
    } catch( Exception& ex ) {
        CPPUNIT_ASSERT_EQUAL( std::string("Error Message"), ex.getMessage() );
    }
}

////////////////////////////////////////////////////////////////////////////////
void DefaultSSLSocketFactoryTest::testCreateSocket() {

    std::auto_ptr<SocketFactory> factory( new DefaultSSLSocketFactory( "Test" ) );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an IOException",
        factory->createSocket(),
        IOException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an IOException",
        factory->createSocket( "127.0.0.1", 61616 ),
        IOException );

    SSLSocketFactory* sslFactory = dynamic_cast<SSLSocketFactory*>( factory.get() );

    CPPUNIT_ASSERT( sslFactory != NULL );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an IOException",
        sslFactory->createSocket( NULL, "127.0.0.1", 61616, true ),
        IOException );
}

////////////////////////////////////////////////////////////////////////////////
void DefaultSSLSocketFactoryTest::testGetDefaultCipherSuites() {

    DefaultSSLSocketFactory factory( "Error Message" );
    CPPUNIT_ASSERT( factory.getDefaultCipherSuites().empty() );
}

////////////////////////////////////////////////////////////////////////////////
void DefaultSSLSocketFactoryTest::testGetSupportedCipherSuites() {

    DefaultSSLSocketFactory factory( "Error Message" );
    CPPUNIT_ASSERT( factory.getSupportedCipherSuites().empty() );
}
