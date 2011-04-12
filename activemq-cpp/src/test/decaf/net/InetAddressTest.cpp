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

#include "InetAddressTest.h"

#include <decaf/net/InetAddress.h>
#include <decaf/net/UnknownHostException.h>

#include <memory>

using namespace decaf;
using namespace decaf::net;
using namespace decaf::lang;

////////////////////////////////////////////////////////////////////////////////
InetAddressTest::InetAddressTest() {
}

////////////////////////////////////////////////////////////////////////////////
InetAddressTest::~InetAddressTest() {
}

////////////////////////////////////////////////////////////////////////////////
void InetAddressTest::testClone() {

    InetAddress address = InetAddress::getLocalHost();
    CPPUNIT_ASSERT( address.getHostName() != "" );
    CPPUNIT_ASSERT( address.getHostAddress() != "" );

    std::auto_ptr<InetAddress> copy( address.clone() );

    CPPUNIT_ASSERT( address.getHostName() == copy->getHostName() );
    CPPUNIT_ASSERT( address.getHostAddress() == copy->getHostAddress() );
}

////////////////////////////////////////////////////////////////////////////////
void InetAddressTest::testGetByAddress() {

    const unsigned char bytes[] = { 127, 0, 0, 1 };
    InetAddress address = InetAddress::getByAddress( bytes, 4 );

    ArrayPointer<unsigned char> value = address.getAddress();

    CPPUNIT_ASSERT( value.get() != NULL );
    CPPUNIT_ASSERT_EQUAL( bytes[0], value[0] );
    CPPUNIT_ASSERT_EQUAL( bytes[1], value[1] );
    CPPUNIT_ASSERT_EQUAL( bytes[2], value[2] );
    CPPUNIT_ASSERT_EQUAL( bytes[3], value[3] );

    const unsigned char invalid[] = { 1 };

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an UnknownHostException",
        InetAddress::getByAddress( invalid, 1 ),
        UnknownHostException );
}

////////////////////////////////////////////////////////////////////////////////
void InetAddressTest::testGetHostAddress() {

    const unsigned char bytes[] = { 127, 0, 0, 1 };
    InetAddress address = InetAddress::getByAddress( bytes, 4 );
    CPPUNIT_ASSERT_EQUAL( std::string( "127.0.0.1" ), address.getHostAddress() );
}

////////////////////////////////////////////////////////////////////////////////
void InetAddressTest::testGetLocalHost() {

    InetAddress address = InetAddress::getLocalHost();
    CPPUNIT_ASSERT( address.getHostName() != "" );
    CPPUNIT_ASSERT( address.getHostAddress() != "" );
}
