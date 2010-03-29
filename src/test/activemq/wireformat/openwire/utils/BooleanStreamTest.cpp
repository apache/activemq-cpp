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

#include "BooleanStreamTest.h"

#include <activemq/wireformat/openwire/utils/BooleanStream.h>
#include <decaf/io/ByteArrayOutputStream.h>
#include <decaf/io/ByteArrayInputStream.h>
#include <decaf/io/DataOutputStream.h>
#include <decaf/io/DataInputStream.h>

using namespace decaf;
using namespace decaf::io;
using namespace activemq;
using namespace activemq::wireformat;
using namespace activemq::wireformat::openwire;
using namespace activemq::wireformat::openwire::utils;

////////////////////////////////////////////////////////////////////////////////
void BooleanStreamTest::test() {

    BooleanStream b1Stream;

    io::ByteArrayOutputStream baoStream;
    io::DataOutputStream daoStream( &baoStream );

    b1Stream.writeBoolean( false );
    b1Stream.writeBoolean( true );
    b1Stream.writeBoolean( false );
    b1Stream.writeBoolean( false );
    b1Stream.writeBoolean( true );
    b1Stream.writeBoolean( false );
    b1Stream.writeBoolean( true );
    b1Stream.writeBoolean( true );

    b1Stream.marshal( &daoStream );

    BooleanStream b2Stream;
    std::pair<const unsigned char*, int> array = baoStream.toByteArray();
    decaf::io::ByteArrayInputStream baiStream( array.first, array.second );
    decaf::io::DataInputStream daiStream( &baiStream );

    b2Stream.unmarshal( &daiStream );

    CPPUNIT_ASSERT( b2Stream.readBoolean() == false );
    CPPUNIT_ASSERT( b2Stream.readBoolean() == true );
    CPPUNIT_ASSERT( b2Stream.readBoolean() == false );
    CPPUNIT_ASSERT( b2Stream.readBoolean() == false );
    CPPUNIT_ASSERT( b2Stream.readBoolean() == true );
    CPPUNIT_ASSERT( b2Stream.readBoolean() == false );
    CPPUNIT_ASSERT( b2Stream.readBoolean() == true );
    CPPUNIT_ASSERT( b2Stream.readBoolean() == true );

    delete [] array.first;
}

////////////////////////////////////////////////////////////////////////////////
void BooleanStreamTest::test2(){

    BooleanStream b1Stream;

    io::ByteArrayOutputStream baoStream;
    io::DataOutputStream daoStream( &baoStream );

    bool value = false;
    for( int i = 0; i < 65536; i++ ) {
        b1Stream.writeBoolean( value );
        value = !value;
    }

    b1Stream.marshal( &daoStream );

    BooleanStream b2Stream;
    std::pair<const unsigned char*, int> array = baoStream.toByteArray();
    decaf::io::ByteArrayInputStream baiStream( array.first, array.second );
    io::DataInputStream daiStream( &baiStream );

    b2Stream.unmarshal( &daiStream );

    value = false;
    for( int i = 0; i < 65536; i++ ) {
        CPPUNIT_ASSERT( b2Stream.readBoolean() == value );
        value = !value;
    }

    delete [] array.first;
}
