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

#include "ActiveMQBytesMessageTest.h"

#include <decaf/util/UUID.h>
#include <activemq/connector/openwire/commands/ActiveMQBytesMessage.h>

using namespace std;
using namespace activemq;
using namespace activemq::util;
using namespace activemq::connector;
using namespace activemq::connector::openwire;
using namespace activemq::connector::openwire::marshal;
using namespace activemq::connector::openwire::commands;

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessageTest::test()
{
    ActiveMQBytesMessage myMessage;

    CPPUNIT_ASSERT( myMessage.getDataStructureType() == ActiveMQBytesMessage::ID_ACTIVEMQBYTESMESSAGE );

    // Write some data
    myMessage.writeBoolean( false );
    myMessage.writeByte( 127 );
    myMessage.writeShort( 32767 );
    myMessage.writeUnsignedShort( 65535 );
    myMessage.writeDouble( 0.5622154 );
    myMessage.writeUTF( "This is a UTF String" );

    try{
        myMessage.readInt();
        CPPUNIT_ASSERT( false );
    } catch(...) {}

    myMessage.writeDouble( 1.012 );
    myMessage.writeFloat( 10.000005f );
    myMessage.writeInt( 15645646 );
    myMessage.writeLong( 0xFFFAAA999LL );
    myMessage.writeString( "This is a test String" );

//    CPPUNIT_ASSERT( myMessage.getBodyLength() != 0 );
//
//    // Get ready to read
//    myMessage.reset();
//
//    CPPUNIT_ASSERT( myMessage.readBoolean() == false );
//    CPPUNIT_ASSERT( myMessage.readByte() == 127 );
//    CPPUNIT_ASSERT( myMessage.readShort() == 32767 );
//    CPPUNIT_ASSERT( myMessage.readUnsignedShort() == 65535 );
//    CPPUNIT_ASSERT( myMessage.readDouble() == 0.5622154 );
//    CPPUNIT_ASSERT( myMessage.readUTF() == "This is a UTF String" );
//    CPPUNIT_ASSERT( myMessage.readDouble() == 1.012 );
//    CPPUNIT_ASSERT( myMessage.readFloat() == 10.000005f );
//    CPPUNIT_ASSERT( myMessage.readInt() == 15645646 );
//    CPPUNIT_ASSERT( myMessage.readLong() == 0xFFFAAA999ULL );
//    CPPUNIT_ASSERT( myMessage.readString() == "This is a test String" );
//
//    myMessage.clearBody();
//
//    CPPUNIT_ASSERT( myMessage.getBodyLength() == 0 );
}
