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

#include "ActiveMQStreamMessageTest.h"

#include <activemq/commands/ActiveMQStreamMessage.h>

using namespace std;
using namespace activemq;
using namespace activemq::commands;

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessageTest::testSetAndGet() {

    ActiveMQStreamMessage myMessage;

    CPPUNIT_ASSERT( myMessage.getDataStructureType() == ActiveMQStreamMessage::ID_ACTIVEMQSTREAMMESSAGE );

    std::vector<unsigned char> data;
    data.push_back( 2 );
    data.push_back( 4 );
    data.push_back( 8 );
    data.push_back( 16 );
    data.push_back( 32 );
    std::vector<unsigned char> readData( data.size() );

    myMessage.writeBoolean( false );
    myMessage.writeByte( 127 );
    myMessage.writeChar( 'a' );
    myMessage.writeShort( 32000 );
    myMessage.writeInt( 6789999 );
    myMessage.writeLong( 0xFFFAAA33345LL );
    myMessage.writeFloat( 0.000012f );
    myMessage.writeDouble( 64.54654 );
    myMessage.writeBytes( data );

    myMessage.reset();

    CPPUNIT_ASSERT( myMessage.readBoolean() == false );
    CPPUNIT_ASSERT( myMessage.readByte() == 127 );
    CPPUNIT_ASSERT( myMessage.readChar() == 'a' );
    CPPUNIT_ASSERT( myMessage.readShort() == 32000 );
    CPPUNIT_ASSERT( myMessage.readInt() == 6789999 );
    CPPUNIT_ASSERT( myMessage.readLong() == 0xFFFAAA33345LL );
    CPPUNIT_ASSERT( myMessage.readFloat() == 0.000012f );
    CPPUNIT_ASSERT( myMessage.readDouble() == 64.54654 );
    CPPUNIT_ASSERT( myMessage.readBytes( readData ) == data.size() );
}

