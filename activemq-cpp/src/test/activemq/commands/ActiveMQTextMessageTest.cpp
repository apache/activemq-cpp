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

#include "ActiveMQTextMessageTest.h"

#include <activemq/commands/ActiveMQTextMessage.h>

using namespace cms;
using namespace std;
using namespace activemq;
using namespace activemq::util;
using namespace activemq::commands;

////////////////////////////////////////////////////////////////////////////////
void ActiveMQTextMessageTest::test() {
    ActiveMQTextMessage myMessage;

    CPPUNIT_ASSERT( myMessage.getDataStructureType() == ActiveMQTextMessage::ID_ACTIVEMQTEXTMESSAGE );

    const char* testText = "This is some test Text";

    myMessage.setText( testText );

    CPPUNIT_ASSERT( myMessage.getText() == testText );

    cms::TextMessage* clonedMessage = myMessage.clone();
    CPPUNIT_ASSERT( clonedMessage != NULL );
    CPPUNIT_ASSERT( clonedMessage->getText() == testText );
    delete clonedMessage;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQTextMessageTest::testShallowCopy() {

    ActiveMQTextMessage msg;
    std::string string1 = "str";
    msg.setText( string1 );

    ActiveMQTextMessage msg2;

    msg2.copyDataStructure( &msg );
    CPPUNIT_ASSERT( msg.getText() == msg2.getText() );
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQTextMessageTest::testGetBytes() {

    ActiveMQTextMessage msg;
    std::string str = "testText";
    msg.setText( str );
    msg.beforeMarshal( NULL );

    ActiveMQTextMessage msg2;
    msg2.setContent( msg.getContent() );

    CPPUNIT_ASSERT( msg2.getText() == str );
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQTextMessageTest::testClearBody() {

    ActiveMQTextMessage textMessage;
    textMessage.setText( "string" );
    textMessage.clearBody();
    CPPUNIT_ASSERT( !textMessage.isReadOnlyBody() );
    CPPUNIT_ASSERT( textMessage.getText() == "" );
    try {
        textMessage.setText( "String" );
        textMessage.getText();
    } catch( MessageNotWriteableException& mnwe ) {
        CPPUNIT_FAIL( "should be writeable" );
    } catch( MessageNotReadableException& mnre ) {
        CPPUNIT_FAIL( "should be readable" );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQTextMessageTest::testReadOnlyBody() {
    ActiveMQTextMessage textMessage;
    textMessage.setText( "test" );
    textMessage.setReadOnlyBody( true );
    try {
        textMessage.getText();
    } catch( MessageNotReadableException& e ) {
        CPPUNIT_FAIL( "should be readable" );
    }
    try {
        textMessage.setText( "test" );
        CPPUNIT_FAIL( "should throw exception" );
    } catch( MessageNotWriteableException& mnwe ) {
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQTextMessageTest::testWriteOnlyBody() {

    ActiveMQTextMessage textMessage;
    textMessage.setReadOnlyBody( false );
    try {
        textMessage.setText( "test" );
        textMessage.getText();
    } catch( MessageNotReadableException& e ) {
        CPPUNIT_FAIL( "should be readable" );
    }
    textMessage.setReadOnlyBody( true );
    try {
        textMessage.getText();
        textMessage.setText( "test" );
        CPPUNIT_FAIL( "should throw exception" );
    } catch( MessageNotReadableException& e ) {
        CPPUNIT_FAIL( "should be readable" );
    } catch( MessageNotWriteableException& mnwe ) {
    }
}
