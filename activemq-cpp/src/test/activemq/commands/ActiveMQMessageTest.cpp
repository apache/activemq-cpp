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

#include <activemq/commands/ActiveMQMessage.h>
#include <activemq/commands/ActiveMQTopic.h>
#include <activemq/commands/ActiveMQTempTopic.h>
#include <activemq/commands/ProducerId.h>

#include <decaf/lang/System.h>
#include <decaf/lang/Pointer.h>

using namespace cms;
using namespace std;
using namespace activemq;
using namespace activemq::util;
using namespace activemq::core;
using namespace activemq::commands;
using namespace decaf::lang;

////////////////////////////////////////////////////////////////////////////////
namespace{

    class MyAckHandler : public core::ActiveMQAckHandler {
    public:

        MyAckHandler() : wasAcked(false) {
        }

        /**
         * Method called to acknowledge the message passed
         * @param message Message to Acknowledge
         * @throw CMSException
         */
        virtual void acknowledgeMessage( const commands::Message* message )
            throw ( cms::CMSException ) {

            this->wasAcked = true;
        }

        /**
         * Public indicator that we have received an ack
         */
        bool wasAcked;

    };
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMessageTest::setUp() {

    Pointer<ProducerId> producerId( new ProducerId() );
    producerId->setConnectionId( "testConnectionId" );
    producerId->setSessionId( 11 );
    producerId->setValue( 1 );

    this->cmsMessageId.reset( new MessageId() );
    this->cmsMessageId->setProducerId( producerId );
    this->cmsMessageId->setProducerSequenceId( 12 );

    this->cmsCorrelationID = "testcorrelationid";
    this->cmsDestination.reset( new ActiveMQTopic( "test.topic" ) );
    this->cmsReplyTo.reset( new ActiveMQTempTopic( "test.replyto.topic:001" ) );
    this->cmsDeliveryMode = DeliveryMode::NON_PERSISTENT;
    this->cmsRedelivered = true;
    this->cmsType = "test type";
    this->cmsExpiration = 100000;
    this->cmsPriority = 5;
    this->cmsTimestamp = System::currentTimeMillis();
    this->readOnlyMessage = false;

    for( int i = 0; i < 3; i++ ) {
        this->consumerIDs.push_back( i );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMessageTest::tearDown() {

}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMessageTest::test()
{
    ActiveMQMessage myMessage;
    Pointer<MyAckHandler> ackHandler( new MyAckHandler() );

    CPPUNIT_ASSERT( myMessage.getDataStructureType() == ActiveMQMessage::ID_ACTIVEMQMESSAGE );

    myMessage.setAckHandler( ackHandler );
    myMessage.acknowledge();

    CPPUNIT_ASSERT( ackHandler->wasAcked == true );

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

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMessageTest::testSetReadOnly() {

    ActiveMQMessage msg;
    msg.setReadOnlyProperties( true );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a MessageNotWriteableException",
        msg.setIntProperty( "test", 1 ),
        MessageNotWriteableException);

    msg.setReadOnlyProperties( false );

    CPPUNIT_ASSERT_NO_THROW_MESSAGE(
        "Should have thrown a MessageNotWriteableException",
        msg.setIntProperty( "test", 1 ));
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMessageTest::testSetToForeignJMSID() {

    ActiveMQMessage msg;
    msg.setCMSMessageID( "ID:EMS-SERVER.8B443C380083:429" );
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMessageTest::testEqualsObject() {

    ActiveMQMessage msg1;
    ActiveMQMessage msg2;

    msg1.setMessageId( this->cmsMessageId );
    CPPUNIT_ASSERT( !msg1.equals( &msg2 ) );

    msg2.setMessageId( this->cmsMessageId );
    CPPUNIT_ASSERT( msg1.equals( &msg2 ) );
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMessageTest::testShallowCopy() {

    ActiveMQMessage msg1;
    msg1.setMessageId( this->cmsMessageId );

    ActiveMQMessage msg2;
    msg2.copyDataStructure( &msg1 );

    CPPUNIT_ASSERT( msg1.equals( &msg2 ) );
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMessageTest::testCopy() {

    this->cmsCorrelationID = "testcorrelationid";
    this->cmsDestination.reset( new ActiveMQTopic( "test.topic" ) );
    this->cmsReplyTo.reset( new ActiveMQTempTopic( "test.replyto.topic:001" ) );
    this->cmsRedelivered = true;
    this->cmsType = "test type";
    this->cmsExpiration = 100000;
    this->cmsPriority = 5;
    this->cmsTimestamp = System::currentTimeMillis();
    this->readOnlyMessage = false;

    ActiveMQMessage msg1;
    msg1.setMessageId( this->cmsMessageId );
    msg1.setCMSCorrelationID( this->cmsCorrelationID );
    msg1.setCMSDestination( this->cmsDestination.get() );
    msg1.setCMSReplyTo( this->cmsReplyTo.get() );
    msg1.setCMSDeliveryMode( this->cmsDeliveryMode );
    msg1.setCMSRedelivered( this->cmsRedelivered );
    msg1.setCMSType( this->cmsType );
    msg1.setCMSExpiration( this->cmsExpiration );
    msg1.setCMSPriority( this->cmsPriority );
    msg1.setCMSTimestamp( this->cmsTimestamp );
    msg1.setReadOnlyProperties( true );

    ActiveMQMessage msg2;
    msg2.copyDataStructure( &msg1 );

    CPPUNIT_ASSERT( msg1.getCMSMessageID() == msg2.getCMSMessageID() );
    CPPUNIT_ASSERT( msg1.getCMSCorrelationID() == msg2.getCMSCorrelationID() );
    CPPUNIT_ASSERT( msg1.getCMSDestination() == msg2.getCMSDestination() );
    CPPUNIT_ASSERT( msg1.getCMSReplyTo() == msg2.getCMSReplyTo() );
    CPPUNIT_ASSERT( msg1.getCMSDeliveryMode() == msg2.getCMSDeliveryMode() );
    CPPUNIT_ASSERT( msg1.getCMSRedelivered() == msg2.getCMSRedelivered() );
    CPPUNIT_ASSERT( msg1.getCMSType() == msg2.getCMSType() );
    CPPUNIT_ASSERT( msg1.getCMSExpiration() == msg2.getCMSExpiration() );
    CPPUNIT_ASSERT( msg1.getCMSPriority() == msg2.getCMSPriority() );
    CPPUNIT_ASSERT( msg1.getCMSTimestamp() == msg2.getCMSTimestamp() );
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMessageTest::testGetAndSetCMSMessageID() {

    ActiveMQMessage msg;
    msg.setMessageId( this->cmsMessageId );
    CPPUNIT_ASSERT( msg.getMessageId() == this->cmsMessageId );
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMessageTest::testGetAndSetCMSTimestamp() {

    ActiveMQMessage msg;
    msg.setCMSTimestamp( this->cmsTimestamp );
    CPPUNIT_ASSERT( msg.getCMSTimestamp() == this->cmsTimestamp );
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMessageTest::testGetAndSetCMSCorrelationID() {

    ActiveMQMessage msg;
    msg.setCMSCorrelationID( this->cmsCorrelationID );
    CPPUNIT_ASSERT( msg.getCMSCorrelationID() == this->cmsCorrelationID );
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMessageTest::testGetAndSetCMSDeliveryMode() {

    ActiveMQMessage msg;
    msg.setCMSDeliveryMode( this->cmsDeliveryMode );
    CPPUNIT_ASSERT( msg.getCMSDeliveryMode() == this->cmsDeliveryMode );
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMessageTest::testGetAndSetCMSRedelivered() {

    ActiveMQMessage msg;
    msg.setRedeliveryCounter( 1 );
    CPPUNIT_ASSERT( msg.getCMSRedelivered() == true );
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMessageTest::testGetAndSetCMSType() {

    ActiveMQMessage msg;
    msg.setCMSType( this->cmsType );
    CPPUNIT_ASSERT( msg.getCMSType() == this->cmsType );
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMessageTest::testGetAndSetCMSExpiration() {

    ActiveMQMessage msg;
    msg.setCMSExpiration( this->cmsExpiration );
    CPPUNIT_ASSERT( msg.getCMSExpiration() == this->cmsExpiration );
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMessageTest::testGetAndSetCMSPriority() {

    ActiveMQMessage msg;
    msg.setCMSPriority( this->cmsPriority );
    CPPUNIT_ASSERT( msg.getCMSPriority() == this->cmsPriority );
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMessageTest::testClearProperties() {

    ActiveMQMessage msg;

    std::vector<unsigned char> buffer( 20 );

    msg.setStringProperty( "test", "test" );
    msg.setContent( buffer );
    msg.setMessageId( this->cmsMessageId );
    msg.clearProperties();

    CPPUNIT_ASSERT( !msg.propertyExists( "test" ) );
    CPPUNIT_ASSERT( msg.getCMSMessageID() != "" );
    CPPUNIT_ASSERT( msg.getContent().size() == 20 );
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMessageTest::testPropertyExists() {

    ActiveMQMessage msg;
    msg.setStringProperty( "test", "test" );
    CPPUNIT_ASSERT( msg.propertyExists( "test" ) );
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMessageTest::testGetBooleanProperty() {

    ActiveMQMessage msg;
    std::string name = "booleanProperty";
    msg.setBooleanProperty( name, true );
    CPPUNIT_ASSERT( msg.getBooleanProperty( name ) );
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMessageTest::testGetByteProperty() {

    ActiveMQMessage msg;
    std::string name = "byteProperty";
    msg.setByteProperty( name, (unsigned char)1 );
    CPPUNIT_ASSERT( msg.getByteProperty( name ) == 1 );
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMessageTest::testGetShortProperty() {

    ActiveMQMessage msg;
    std::string name = "shortProperty";
    msg.setShortProperty( name, (short)1 );
    CPPUNIT_ASSERT( msg.getShortProperty( name ) == 1 );
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMessageTest::testGetIntProperty() {

    ActiveMQMessage msg;
    std::string name = "intProperty";
    msg.setIntProperty( name, 1 );
    CPPUNIT_ASSERT( msg.getIntProperty( name ) == 1 );
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMessageTest::testGetLongProperty() {

    ActiveMQMessage msg;
    std::string name = "longProperty";
    msg.setLongProperty( name, 1 );
    CPPUNIT_ASSERT( msg.getLongProperty( name ) == 1 );
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMessageTest::testGetFloatProperty() {

    ActiveMQMessage msg;
    std::string name = "floatProperty";
    msg.setFloatProperty( name, 1.3f );
    CPPUNIT_ASSERT( msg.getFloatProperty( name ) == 1.3f );
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMessageTest::testGetDoubleProperty() {

    ActiveMQMessage msg;
    std::string name = "doubleProperty";
    msg.setDoubleProperty( name, 1.3 );
    CPPUNIT_ASSERT( msg.getDoubleProperty( name ) == 1.3 );
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMessageTest::testGetStringProperty() {

    ActiveMQMessage msg;
    std::string name = "stringProperty";
    msg.setStringProperty( name, name );
    CPPUNIT_ASSERT( msg.getStringProperty( name ) == name );
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMessageTest::testGetPropertyNames() {

    ActiveMQMessage msg;
    std::string name = "floatProperty";
    msg.setFloatProperty( name, 1.3f );

    std::vector<std::string> propertyNames = msg.getPropertyNames();
    std::vector<std::string>::iterator iter = propertyNames.begin();

    for( ; iter != propertyNames.end(); ++iter ) {
        CPPUNIT_ASSERT( *iter == name );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMessageTest::testSetEmptyPropertyName() {

    ActiveMQMessage msg;

    try {
        msg.setStringProperty("", "Cheese");
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( CMSException& e ) {
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMessageTest::testGetAndSetCMSXDeliveryCount() {

    ActiveMQMessage msg;
    msg.setIntProperty( "CMSXDeliveryCount", 1 );
    int count = msg.getIntProperty( "CMSXDeliveryCount" );
    CPPUNIT_ASSERT_MESSAGE( "expected delivery count = 1", count == 1 );
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMessageTest::testClearBody() {

    ActiveMQMessage message;
    message.clearBody();
    CPPUNIT_ASSERT( !message.isReadOnlyBody() );
    CPPUNIT_ASSERT( message.getContent().size() == 0 );
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMessageTest::testBooleanPropertyConversion() {

    ActiveMQMessage msg;
    std::string propertyName = "property";
    msg.setBooleanProperty( propertyName, true );

    CPPUNIT_ASSERT( msg.getBooleanProperty( propertyName ) );
    CPPUNIT_ASSERT( msg.getStringProperty( propertyName ) == "true" );

    try {
        msg.getByteProperty( propertyName );
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageFormatException& e ) {
    }
    try {
        msg.getShortProperty( propertyName );
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageFormatException& e ) {
    }
    try {
        msg.getIntProperty( propertyName );
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageFormatException& e ) {
    }
    try {
        msg.getLongProperty( propertyName );
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageFormatException& e ) {
    }
    try {
        msg.getFloatProperty( propertyName );
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageFormatException& e ) {
    }
    try {
        msg.getDoubleProperty( propertyName );
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageFormatException& e ) {
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMessageTest::testBytePropertyConversion() {

    ActiveMQMessage msg;
    std::string propertyName = "property";
    msg.setByteProperty( propertyName, (unsigned char)1 );

    CPPUNIT_ASSERT( msg.getByteProperty( propertyName ) == 1 );
    CPPUNIT_ASSERT( msg.getShortProperty( propertyName ) == 1 );
    CPPUNIT_ASSERT( msg.getIntProperty( propertyName ) == 1 );
    CPPUNIT_ASSERT( msg.getLongProperty( propertyName ) == 1 );
    CPPUNIT_ASSERT( msg.getStringProperty( propertyName ) == "1" );

    try {
        msg.getBooleanProperty( propertyName );
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageFormatException& e ) {
    }
    try {
        msg.getFloatProperty( propertyName );
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageFormatException& e ) {
    }
    try {
        msg.getDoubleProperty( propertyName );
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageFormatException& e ) {
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMessageTest::testShortPropertyConversion() {

    ActiveMQMessage msg;
    std::string propertyName = "property";
    msg.setShortProperty( propertyName, (short)1 );

    CPPUNIT_ASSERT( msg.getShortProperty( propertyName ) == 1 );
    CPPUNIT_ASSERT( msg.getIntProperty( propertyName ) == 1 );
    CPPUNIT_ASSERT( msg.getLongProperty( propertyName ) == 1 );
    CPPUNIT_ASSERT( msg.getStringProperty( propertyName ) == "1" );

    try {
        msg.getBooleanProperty( propertyName );
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageFormatException& e ) {
    }
    try {
        msg.getByteProperty( propertyName );
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageFormatException& e ) {
    }
    try {
        msg.getFloatProperty( propertyName );
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageFormatException& e ) {
    }
    try {
        msg.getDoubleProperty( propertyName );
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageFormatException& e ) {
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMessageTest::testIntPropertyConversion() {

    ActiveMQMessage msg;
    std::string propertyName = "property";
    msg.setIntProperty( propertyName, (int)1 );

    CPPUNIT_ASSERT( msg.getIntProperty( propertyName ) == 1 );
    CPPUNIT_ASSERT( msg.getLongProperty( propertyName ) == 1 );
    CPPUNIT_ASSERT( msg.getStringProperty( propertyName ) == "1" );

    try {
        msg.getBooleanProperty( propertyName );
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageFormatException& e ) {
    }
    try {
        msg.getByteProperty( propertyName );
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageFormatException& e ) {
    }
    try {
        msg.getShortProperty( propertyName );
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageFormatException& e ) {
    }
    try {
        msg.getFloatProperty( propertyName );
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageFormatException& e ) {
    }
    try {
        msg.getDoubleProperty( propertyName );
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageFormatException& e ) {
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMessageTest::testLongPropertyConversion() {

    ActiveMQMessage msg;
    std::string propertyName = "property";
    msg.setLongProperty( propertyName, 1 );

    CPPUNIT_ASSERT( msg.getLongProperty( propertyName ) == 1 );
    CPPUNIT_ASSERT( msg.getStringProperty( propertyName ) == "1" );

    try {
        msg.getBooleanProperty( propertyName );
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageFormatException& e ) {
    }
    try {
        msg.getByteProperty( propertyName );
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageFormatException& e ) {
    }
    try {
        msg.getShortProperty( propertyName );
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageFormatException& e ) {
    }
    try {
        msg.getIntProperty( propertyName );
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageFormatException& e ) {
    }
    try {
        msg.getFloatProperty( propertyName );
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageFormatException& e ) {
    }
    try {
        msg.getDoubleProperty( propertyName );
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageFormatException& e ) {
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMessageTest::testFloatPropertyConversion() {

    ActiveMQMessage msg;
    std::string propertyName = "property";
    msg.setFloatProperty( propertyName, (float)1.5 );

    CPPUNIT_ASSERT_DOUBLES_EQUAL( msg.getFloatProperty( propertyName ), 1.5, 0 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( msg.getDoubleProperty( propertyName ), 1.5, 0 );
    CPPUNIT_ASSERT( msg.getStringProperty( propertyName ) == "1.5" );

    try {
        msg.getBooleanProperty( propertyName );
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageFormatException& e ) {
    }
    try {
        msg.getByteProperty( propertyName );
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageFormatException& e ) {
    }
    try {
        msg.getShortProperty( propertyName );
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageFormatException& e ) {
    }
    try {
        msg.getIntProperty( propertyName );
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageFormatException& e ) {
    }
    try {
        msg.getLongProperty( propertyName );
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageFormatException& e ) {
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMessageTest::testDoublePropertyConversion() {

    ActiveMQMessage msg;
    std::string propertyName = "property";
    msg.setDoubleProperty( propertyName, 1.5 );

    CPPUNIT_ASSERT_DOUBLES_EQUAL( msg.getDoubleProperty( propertyName ), 1.5, 0 );
    CPPUNIT_ASSERT( msg.getStringProperty( propertyName ) == "1.5" );

    try {
        msg.getBooleanProperty( propertyName );
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageFormatException& e ) {
    }
    try {
        msg.getByteProperty( propertyName );
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageFormatException& e ) {
    }
    try {
        msg.getShortProperty( propertyName );
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageFormatException& e ) {
    }
    try {
        msg.getIntProperty( propertyName );
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageFormatException& e ) {
    }
    try {
        msg.getLongProperty( propertyName );
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageFormatException& e ) {
    }
    try {
        msg.getFloatProperty( propertyName );
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageFormatException& e ) {
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMessageTest::testStringPropertyConversion() {

    ActiveMQMessage msg;
    std::string propertyName = "property";
    std::string stringValue = "true";

    msg.setStringProperty( propertyName, stringValue );

    CPPUNIT_ASSERT( msg.getStringProperty( propertyName ) == stringValue );
    CPPUNIT_ASSERT( msg.getBooleanProperty( propertyName ) == true );

    stringValue = "1";
    msg.setStringProperty( propertyName, stringValue );
    CPPUNIT_ASSERT( msg.getByteProperty( propertyName ) == 1 );
    CPPUNIT_ASSERT( msg.getShortProperty( propertyName ) == 1 );
    CPPUNIT_ASSERT( msg.getIntProperty( propertyName ) == 1 );
    CPPUNIT_ASSERT( msg.getLongProperty( propertyName ) == 1 );

    stringValue = "1.5";
    msg.setStringProperty( propertyName, stringValue );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( msg.getFloatProperty( propertyName ), 1.5, 0 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( msg.getDoubleProperty( propertyName ), 1.5, 0 );

    stringValue = "bad";
    msg.setStringProperty( propertyName, stringValue );
    try {
        msg.getByteProperty( propertyName );
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageFormatException& e ) {
    }
    try {
        msg.getShortProperty( propertyName );
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageFormatException& e ) {
    }
    try {
        msg.getIntProperty( propertyName );
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageFormatException& e ) {
    }
    try {
        msg.getLongProperty( propertyName );
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageFormatException& e ) {
    }
    try {
        msg.getFloatProperty( propertyName );
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageFormatException& e ) {
    }
    try {
        msg.getDoubleProperty( propertyName );
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageFormatException& e ) {
    }

    CPPUNIT_ASSERT( !msg.getBooleanProperty( propertyName ) );
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMessageTest::testReadOnlyProperties() {

    ActiveMQMessage msg;
    std::string propertyName = "property";
    msg.setReadOnlyProperties( true );

    try {
        msg.setStringProperty( propertyName, "test" );
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageNotWriteableException& e ) {
    }
    try {
        msg.setBooleanProperty( propertyName, true );
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageNotWriteableException& e ) {
    }
    try {
        msg.setByteProperty( propertyName, (unsigned char)1 );
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageNotWriteableException& e ) {
    }
    try {
        msg.setShortProperty( propertyName, (short)1 );
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageNotWriteableException& e ) {
    }
    try {
        msg.setIntProperty( propertyName, 1 );
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageNotWriteableException& e ) {
    }
    try {
        msg.setLongProperty( propertyName, 1 );
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageNotWriteableException& e ) {
    }
    try {
        msg.setFloatProperty( propertyName, (float)1.5 );
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageNotWriteableException& e ) {
    }
    try {
        msg.setDoubleProperty( propertyName, 1.5 );
        CPPUNIT_FAIL("Should have thrown exception");
    } catch( MessageNotWriteableException& e ) {
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMessageTest::testIsExpired() {

    ActiveMQMessage msg;
    msg.setCMSExpiration( System::currentTimeMillis() - 100 );
    CPPUNIT_ASSERT( msg.isExpired() );
    msg.setCMSExpiration( System::currentTimeMillis() + 10000 );
    CPPUNIT_ASSERT( !msg.isExpired() );
}
