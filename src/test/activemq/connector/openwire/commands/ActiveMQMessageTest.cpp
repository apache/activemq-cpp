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

#include "ActiveMQMessageTest.h"

#include <activemq/connector/openwire/commands/ActiveMQMessage.h>

using namespace std;
using namespace activemq;
using namespace activemq::util;
using namespace activemq::connector;
using namespace activemq::connector::openwire;
using namespace activemq::connector::openwire::marshal;
using namespace activemq::connector::openwire::commands;

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMessageTest::test()
{
    ActiveMQMessage myMessage;
    MyAckHandler ackHandler;

    CPPUNIT_ASSERT( myMessage.getDataStructureType() == ActiveMQMessage::ID_ACTIVEMQMESSAGE );

    myMessage.setAckHandler( &ackHandler );
    myMessage.acknowledge();

    CPPUNIT_ASSERT( ackHandler.wasAcked == true );

    CPPUNIT_ASSERT( myMessage.getPropertyNames().size() == 0 );
    CPPUNIT_ASSERT( myMessage.propertyExists( "something" ) == false );

    try {
        myMessage.getBooleanProperty( "somethingElse" );
        CPPUNIT_ASSERT( false );
    } catch(...) {}

    myMessage.setBooleanProperty( "boolean", false );
    myMessage.setByteProperty( "byte", 60 );
    myMessage.setDoubleProperty( "double", 642.5643 );
    myMessage.setFloatProperty( "float", 0.564f );
    myMessage.setIntProperty( "int", 65438746 );
    myMessage.setLongProperty( "long", 0xFFFFFFFF0000000LL );
    myMessage.setShortProperty( "short", 512 );
    myMessage.setStringProperty( "string", "This is a test String" );

    CPPUNIT_ASSERT( myMessage.getBooleanProperty( "boolean" ) == false );
    CPPUNIT_ASSERT( myMessage.getByteProperty( "byte" ) == 60 );
    CPPUNIT_ASSERT( myMessage.getDoubleProperty( "double" ) == 642.5643 );
    CPPUNIT_ASSERT( myMessage.getFloatProperty( "float" ) == 0.564f );
    CPPUNIT_ASSERT( myMessage.getIntProperty( "int" ) == 65438746 );
    CPPUNIT_ASSERT( myMessage.getLongProperty( "long" ) == 0xFFFFFFFF0000000LL );
    CPPUNIT_ASSERT( myMessage.getShortProperty( "short" ) == 512 );
    CPPUNIT_ASSERT( myMessage.getStringProperty( "string" ) == "This is a test String" );

    myMessage.setStringProperty( "JMSXGroupID", "hello" );
    CPPUNIT_ASSERT( myMessage.getStringProperty( "JMSXGroupID" ) == "hello" );
}
