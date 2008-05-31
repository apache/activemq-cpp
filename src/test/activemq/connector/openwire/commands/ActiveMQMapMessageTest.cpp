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

#include "ActiveMQMapMessageTest.h"

#include <activemq/connector/openwire/commands/ActiveMQMapMessage.h>

using namespace std;
using namespace activemq;
using namespace activemq::util;
using namespace activemq::connector;
using namespace activemq::connector::openwire;
using namespace activemq::connector::openwire::marshal;
using namespace activemq::connector::openwire::commands;

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMapMessageTest::test()
{
    ActiveMQMapMessage myMessage;

    CPPUNIT_ASSERT( myMessage.getDataStructureType() == ActiveMQMapMessage::ID_ACTIVEMQMAPMESSAGE );

    CPPUNIT_ASSERT( myMessage.getMapNames().size() == 0 );
    CPPUNIT_ASSERT( myMessage.itemExists( "Something" ) == false );

    std::vector<unsigned char> data;

    data.push_back( 2 );
    data.push_back( 4 );
    data.push_back( 8 );
    data.push_back( 16 );
    data.push_back( 32 );

    myMessage.setBoolean( "boolean", false );
    myMessage.setByte( "byte", 127 );
    myMessage.setChar( "char", 'a' );
    myMessage.setShort( "short", 32000 );
    myMessage.setInt( "int", 6789999 );
    myMessage.setLong( "long", 0xFFFAAA33345LL );
    myMessage.setFloat( "float", 0.000012f );
    myMessage.setDouble( "double", 64.54654 );
    myMessage.setBytes( "bytes", data );

    CPPUNIT_ASSERT( myMessage.getBoolean( "boolean" ) == false );
    CPPUNIT_ASSERT( myMessage.getByte( "byte" ) == 127 );
    CPPUNIT_ASSERT( myMessage.getChar( "char" ) == 'a' );
    CPPUNIT_ASSERT( myMessage.getShort( "short" ) == 32000 );
    CPPUNIT_ASSERT( myMessage.getInt( "int" ) == 6789999 );
    CPPUNIT_ASSERT( myMessage.getLong( "long" ) == 0xFFFAAA33345LL );
    CPPUNIT_ASSERT( myMessage.getFloat( "float" ) == 0.000012f );
    CPPUNIT_ASSERT( myMessage.getDouble( "double" ) == 64.54654 );
    CPPUNIT_ASSERT( myMessage.getBytes( "bytes" ) == data );
}
