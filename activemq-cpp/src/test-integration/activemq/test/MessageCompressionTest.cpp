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

#include "MessageCompressionTest.h"

#include <activemq/util/CMSListener.h>
#include <activemq/exceptions/ActiveMQException.h>
#include <activemq/core/ActiveMQConnection.h>
#include <activemq/commands/Message.h>

#include <decaf/lang/Thread.h>
#include <decaf/util/UUID.h>

using namespace std;
using namespace cms;
using namespace activemq;
using namespace activemq::commands;
using namespace activemq::core;
using namespace activemq::test;
using namespace activemq::util;
using namespace activemq::exceptions;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::util;

////////////////////////////////////////////////////////////////////////////////
namespace{

    string TEST_CLIENT_ID = "MessageCompressionTestClientId";
    string DESTINATION_NAME = "MessageCompressionTestDest";

    // The following text should compress well
    const string TEXT = std::string()
                      + "The quick red fox jumped over the lazy brown dog. " + "The quick red fox jumped over the lazy brown dog. "
                      + "The quick red fox jumped over the lazy brown dog. " + "The quick red fox jumped over the lazy brown dog. "
                      + "The quick red fox jumped over the lazy brown dog. " + "The quick red fox jumped over the lazy brown dog. "
                      + "The quick red fox jumped over the lazy brown dog. " + "The quick red fox jumped over the lazy brown dog. "
                      + "The quick red fox jumped over the lazy brown dog. " + "The quick red fox jumped over the lazy brown dog. "
                      + "The quick red fox jumped over the lazy brown dog. " + "The quick red fox jumped over the lazy brown dog. "
                      + "The quick red fox jumped over the lazy brown dog. " + "The quick red fox jumped over the lazy brown dog. "
                      + "The quick red fox jumped over the lazy brown dog. " + "The quick red fox jumped over the lazy brown dog. "
                      + "The quick red fox jumped over the lazy brown dog. ";

    bool a = true;
    unsigned char b = 123;
    char c = 'c';
    short d = 0x1234;
    int e = 0x12345678;
    long long f = 0x1234567812345678LL;
    string g = "Hello World!";
    bool h = false;
    unsigned char i = 0xFF;
    short j = -0x1234;
    int k = -0x12345678;
    long long l = -0x1234567812345678LL;
    float m = 2.1F;
    double n = 2.3;
}

////////////////////////////////////////////////////////////////////////////////
MessageCompressionTest::MessageCompressionTest() {
}

////////////////////////////////////////////////////////////////////////////////
MessageCompressionTest::~MessageCompressionTest() {
}

////////////////////////////////////////////////////////////////////////////////
void MessageCompressionTest::testTextMessageCompression() {

    ActiveMQConnection* connection =
        dynamic_cast<ActiveMQConnection*>( this->cmsProvider->getConnection() );

    CPPUNIT_ASSERT( connection != NULL );
    CPPUNIT_ASSERT_MESSAGE( "Compression not enabled.", connection->isUseCompression() );

    Session* session = this->cmsProvider->getSession();

    std::auto_ptr<TextMessage> sent( session->createTextMessage( TEXT ) );

    cms::MessageConsumer* consumer = cmsProvider->getConsumer();
    cms::MessageProducer* producer = cmsProvider->getProducer();
    producer->setDeliveryMode( DeliveryMode::NON_PERSISTENT );

    // Send some text messages
    producer->send( sent.get() );

    auto_ptr<cms::Message> message( consumer->receive( 2000 ) );
    CPPUNIT_ASSERT( message.get() != NULL );

    TextMessage* recvd = dynamic_cast<TextMessage*>( message.get() );
    CPPUNIT_ASSERT_MESSAGE( "Received message was not a TextMessage", recvd != NULL );

    CPPUNIT_ASSERT_EQUAL_MESSAGE( "Received text differs from sent text.",
                                  sent->getText(), recvd->getText() );

    commands::Message* amqMsg = dynamic_cast<commands::Message*>( message.get() );
    CPPUNIT_ASSERT_MESSAGE( "Received message was not an AMQ message type", amqMsg != NULL );
    CPPUNIT_ASSERT_MESSAGE( "Received message was not compressed.", amqMsg->isCompressed() );
}

////////////////////////////////////////////////////////////////////////////////
void MessageCompressionTest::testBytesMessageCompression() {

    ActiveMQConnection* connection =
        dynamic_cast<ActiveMQConnection*>( this->cmsProvider->getConnection() );

    CPPUNIT_ASSERT( connection != NULL );
    CPPUNIT_ASSERT_MESSAGE( "Compression not enabled.", connection->isUseCompression() );

    Session* session = this->cmsProvider->getSession();

    std::auto_ptr<BytesMessage> sent( session->createBytesMessage() );

    cms::MessageConsumer* consumer = cmsProvider->getConsumer();
    cms::MessageProducer* producer = cmsProvider->getProducer();
    producer->setDeliveryMode( DeliveryMode::NON_PERSISTENT );

    sent->writeBoolean( a );
    sent->writeByte( b );
    sent->writeChar( c );
    sent->writeShort( d );
    sent->writeInt( e );
    sent->writeLong( f );
    sent->writeString( g );
    sent->writeBoolean( h );
    sent->writeByte( i );
    sent->writeShort( j );
    sent->writeInt( k );
    sent->writeLong( l );
    sent->writeFloat( m );
    sent->writeDouble( n );

    // Send some text messages
    producer->send( sent.get() );

    auto_ptr<cms::Message> message( consumer->receive( 2000 ) );
    CPPUNIT_ASSERT( message.get() != NULL );

    BytesMessage* recvd = dynamic_cast<BytesMessage*>( message.get() );
    CPPUNIT_ASSERT_MESSAGE( "Received message was not a BytesMessage", recvd != NULL );

    CPPUNIT_ASSERT_EQUAL( a, recvd->readBoolean() );
    CPPUNIT_ASSERT_EQUAL( b, recvd->readByte() );
    CPPUNIT_ASSERT_EQUAL( c, recvd->readChar() );
    CPPUNIT_ASSERT_EQUAL( d, recvd->readShort() );
    CPPUNIT_ASSERT_EQUAL( e, recvd->readInt() );
    CPPUNIT_ASSERT_EQUAL( f, recvd->readLong() );
    CPPUNIT_ASSERT_EQUAL( g, recvd->readString() );
    CPPUNIT_ASSERT_EQUAL( h, recvd->readBoolean() );
    CPPUNIT_ASSERT_EQUAL( i, recvd->readByte() );
    CPPUNIT_ASSERT_EQUAL( j, recvd->readShort() );
    CPPUNIT_ASSERT_EQUAL( k, recvd->readInt() );
    CPPUNIT_ASSERT_EQUAL( l, recvd->readLong() );
    CPPUNIT_ASSERT_EQUAL( m, recvd->readFloat() );
    CPPUNIT_ASSERT_EQUAL( n, recvd->readDouble() );

    commands::Message* amqMsg = dynamic_cast<commands::Message*>( message.get() );
    CPPUNIT_ASSERT_MESSAGE( "Received message was not an AMQ message type", amqMsg != NULL );
    CPPUNIT_ASSERT_MESSAGE( "Received message was not compressed.", amqMsg->isCompressed() );
}

////////////////////////////////////////////////////////////////////////////////
void MessageCompressionTest::testStreamMessageCompression() {

    ActiveMQConnection* connection =
        dynamic_cast<ActiveMQConnection*>( this->cmsProvider->getConnection() );

    CPPUNIT_ASSERT( connection != NULL );
    CPPUNIT_ASSERT_MESSAGE( "Compression not enabled.", connection->isUseCompression() );

    Session* session = this->cmsProvider->getSession();

    std::auto_ptr<StreamMessage> sent( session->createStreamMessage() );

    cms::MessageConsumer* consumer = cmsProvider->getConsumer();
    cms::MessageProducer* producer = cmsProvider->getProducer();
    producer->setDeliveryMode( DeliveryMode::NON_PERSISTENT );

    sent->writeBoolean( a );
    sent->writeByte( b );
    sent->writeChar( c );
    sent->writeShort( d );
    sent->writeInt( e );
    sent->writeLong( f );
    sent->writeString( g );
    sent->writeBoolean( h );
    sent->writeByte( i );
    sent->writeShort( j );
    sent->writeInt( k );
    sent->writeLong( l );
    sent->writeFloat( m );
    sent->writeDouble( n );

    // Send some text messages
    producer->send( sent.get() );

    auto_ptr<cms::Message> message( consumer->receive( 2000 ) );
    CPPUNIT_ASSERT( message.get() != NULL );

    StreamMessage* recvd = dynamic_cast<StreamMessage*>( message.get() );
    CPPUNIT_ASSERT_MESSAGE( "Received message was not a StreamMessage", recvd != NULL );

    CPPUNIT_ASSERT_EQUAL( a, recvd->readBoolean() );
    CPPUNIT_ASSERT_EQUAL( b, recvd->readByte() );
    CPPUNIT_ASSERT_EQUAL( c, recvd->readChar() );
    CPPUNIT_ASSERT_EQUAL( d, recvd->readShort() );
    CPPUNIT_ASSERT_EQUAL( e, recvd->readInt() );
    CPPUNIT_ASSERT_EQUAL( f, recvd->readLong() );
    CPPUNIT_ASSERT_EQUAL( g, recvd->readString() );
    CPPUNIT_ASSERT_EQUAL( h, recvd->readBoolean() );
    CPPUNIT_ASSERT_EQUAL( i, recvd->readByte() );
    CPPUNIT_ASSERT_EQUAL( j, recvd->readShort() );
    CPPUNIT_ASSERT_EQUAL( k, recvd->readInt() );
    CPPUNIT_ASSERT_EQUAL( l, recvd->readLong() );
    CPPUNIT_ASSERT_EQUAL( m, recvd->readFloat() );
    CPPUNIT_ASSERT_EQUAL( n, recvd->readDouble() );

    commands::Message* amqMsg = dynamic_cast<commands::Message*>( message.get() );
    CPPUNIT_ASSERT_MESSAGE( "Received message was not an AMQ message type", amqMsg != NULL );
    CPPUNIT_ASSERT_MESSAGE( "Received message was not compressed.", amqMsg->isCompressed() );
}

////////////////////////////////////////////////////////////////////////////////
void MessageCompressionTest::testMapMessageCompression() {

    ActiveMQConnection* connection =
        dynamic_cast<ActiveMQConnection*>( this->cmsProvider->getConnection() );

    CPPUNIT_ASSERT( connection != NULL );
    CPPUNIT_ASSERT_MESSAGE( "Compression not enabled.", connection->isUseCompression() );

    Session* session = this->cmsProvider->getSession();

    std::auto_ptr<MapMessage> sent( session->createMapMessage() );

    cms::MessageConsumer* consumer = cmsProvider->getConsumer();
    cms::MessageProducer* producer = cmsProvider->getProducer();
    producer->setDeliveryMode( DeliveryMode::NON_PERSISTENT );

    sent->setBoolean( "a", a );
    sent->setByte( "b", b );
    sent->setChar( "c", c );
    sent->setShort( "d", d );
    sent->setInt( "e", e );
    sent->setLong( "f", f );
    sent->setString( "g", g );
    sent->setBoolean( "h", h );
    sent->setByte( "i", i );
    sent->setShort( "j", j );
    sent->setInt( "k", k );
    sent->setLong( "l", l );
    sent->setFloat( "m", m );
    sent->setDouble( "n", n );

    // Send some text messages
    producer->send( sent.get() );

    auto_ptr<cms::Message> message( consumer->receive( 2000 ) );
    CPPUNIT_ASSERT( message.get() != NULL );

    MapMessage* recvd = dynamic_cast<MapMessage*>( message.get() );
    CPPUNIT_ASSERT_MESSAGE( "Received message was not a MapMessage", recvd != NULL );

    CPPUNIT_ASSERT_EQUAL( a, recvd->getBoolean( "a" ) );
    CPPUNIT_ASSERT_EQUAL( b, recvd->getByte( "b" ) );
    CPPUNIT_ASSERT_EQUAL( c, recvd->getChar( "c" ) );
    CPPUNIT_ASSERT_EQUAL( d, recvd->getShort( "d" ) );
    CPPUNIT_ASSERT_EQUAL( e, recvd->getInt( "e" ) );
    CPPUNIT_ASSERT_EQUAL( f, recvd->getLong( "f" ) );
    CPPUNIT_ASSERT_EQUAL( g, recvd->getString( "g" ) );
    CPPUNIT_ASSERT_EQUAL( h, recvd->getBoolean( "h" ) );
    CPPUNIT_ASSERT_EQUAL( i, recvd->getByte( "i" ) );
    CPPUNIT_ASSERT_EQUAL( j, recvd->getShort( "j" ) );
    CPPUNIT_ASSERT_EQUAL( k, recvd->getInt( "k" ) );
    CPPUNIT_ASSERT_EQUAL( l, recvd->getLong( "l" ) );
    CPPUNIT_ASSERT_EQUAL( m, recvd->getFloat( "m" ) );
    CPPUNIT_ASSERT_EQUAL( n, recvd->getDouble( "n" ) );

    commands::Message* amqMsg = dynamic_cast<commands::Message*>( message.get() );
    CPPUNIT_ASSERT_MESSAGE( "Received message was not an AMQ message type", amqMsg != NULL );
    CPPUNIT_ASSERT_MESSAGE( "Received message was not compressed.", amqMsg->isCompressed() );
}
