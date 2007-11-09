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

#ifndef _ACTIVEMQ_CONNECTOR_STOMP_STOMPCOMMANDWRITERTEST_H_
#define _ACTIVEMQ_CONNECTOR_STOMP_STOMPCOMMANDWRITERTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <activemq/connector/stomp/StompCommandWriter.h>
#include <activemq/connector/stomp/StompCommandReader.h>
#include <activemq/transport/Command.h>
#include <activemq/connector/stomp/commands/ConnectedCommand.h>
#include <activemq/connector/stomp/commands/TextMessageCommand.h>
#include <activemq/connector/stomp/commands/BytesMessageCommand.h>
#include <activemq/connector/stomp/StompTopic.h>
#include <activemq/connector/stomp/commands/CommandConstants.h>

#include <decaf/io/ByteArrayOutputStream.h>
#include <decaf/io/ByteArrayInputStream.h>
#include <algorithm>
#include <string.h>

namespace activemq{
namespace connector{
namespace stomp{

    class StompCommandWriterTest : public CppUnit::TestFixture
    {
        CPPUNIT_TEST_SUITE( StompCommandWriterTest );
        CPPUNIT_TEST( test );
        CPPUNIT_TEST( testWriteAndReads );
        CPPUNIT_TEST_SUITE_END();

        class MyBytesMessageCommand : public commands::BytesMessageCommand {
        public:

            virtual const StompFrame& marshal(void)
                throw (marshal::MarshalException)
            {
                const StompFrame& frame =
                    commands::BytesMessageCommand::marshal();

                // Before we send out the frame tag it with the content length
                // as this is a bytes message and we can't ensure we have only
                // a trailing NULL.
                const_cast< StompFrame* >( &frame )->setCommand(
                    commands::CommandConstants::toString(
                        commands::CommandConstants::MESSAGE ) );

                return frame;
            }

        };

        class MyTextMessageCommand : public commands::TextMessageCommand {
        public:

            virtual const StompFrame& marshal(void)
                throw (marshal::MarshalException)
            {
                const StompFrame& frame =
                    commands::TextMessageCommand::marshal();

                // Before we send out the frame tag it with the content length
                // as this is a bytes message and we can't ensure we have only
                // a trailing NULL.
                const_cast< StompFrame* >( &frame )->setCommand(
                    commands::CommandConstants::toString(
                        commands::CommandConstants::MESSAGE ) );

                return frame;
            }

        };

    public:

        StompCommandWriterTest() {}
        virtual ~StompCommandWriterTest() {}

        void test( void )
        {
            decaf::io::ByteArrayOutputStream boStream;

            StompCommandWriter writer( &boStream );

            const char* result =
                "CONNECTED\nsession:test\n\n\0\n"   // 26 = 26
                "SEND\n"                            // 5
                "destination:/topic/a\n"            // 21
                "message-id:123\n"                  // 15
                "sampleProperty:testvalue\n\n"      // 26
                "testMessage\0\n"                   // 13 = 80
                "SEND\n"                            // 5
                "content-length:9\n"                // 17
                "destination:/topic/a\n"            // 21
                "message-id:123\n"                  // 15
                "sampleProperty:testvalue\n\n"      // 26
                "123456789\0\n";                    // 11 = 95
                                                    //      201
            commands::ConnectedCommand connectedCommand;
            commands::TextMessageCommand textCommand;
            commands::BytesMessageCommand bytesCommand;

            // Sync to expected output
            connectedCommand.setSessionId( "test" );

            // Sync to expected output
            StompTopic topic1("a");
            textCommand.setCMSDestination( &topic1 );
            textCommand.setCMSMessageID( "123" );
            textCommand.getProperties().setProperty(
                "sampleProperty", "testvalue" );
            textCommand.setText( "testMessage" );

            // Sync to expected output
            StompTopic topic2("a");
            bytesCommand.setCMSDestination( &topic2 );
            bytesCommand.setCMSMessageID( "123" );
            bytesCommand.getProperties().setProperty(
                "sampleProperty", "testvalue" );
            bytesCommand.setBodyBytes(
                (const unsigned char*)"123456789", 9 );

            writer.writeCommand( &connectedCommand );
            writer.writeCommand( &textCommand );
            writer.writeCommand( &bytesCommand );

            const unsigned char* alloc = boStream.toByteArray();

            //for( int i = 0; i < 201; ++i )
            //{
            //    std::cout << result[i] << " == " << alloc[i] << std::endl;
            //}

            CPPUNIT_ASSERT( boStream.size() == 201 );

            for( int i = 0; i < 201; ++i )
            {
                CPPUNIT_ASSERT( result[i] == alloc[i] );
            }

            // Use STL Compare
            CPPUNIT_ASSERT(
                memcmp( &result[0], boStream.toByteArray(), 200 ) == 0 );
        }

        void testWriteAndReads() {

            decaf::io::ByteArrayOutputStream boStream;
            decaf::io::ByteArrayInputStream biStream;

            StompCommandWriter writer( &boStream );
            StompCommandReader reader( &biStream );

            MyTextMessageCommand textCommand;
            MyBytesMessageCommand bytesCommand;

            StompTopic topic2("a");
            bytesCommand.setCMSDestination( &topic2 );
            bytesCommand.setCMSMessageID( "123" );
            textCommand.setCMSDestination( &topic2 );
            textCommand.setCMSMessageID( "123" );

            textCommand.setText("This is a TextMessage");

            const int testInt1 = 45678;
            const int testInt2 = 42;

            const std::string testStr1 = "Test String 1";
            const std::string testStr2 = "Test String 2";

            const bool testBool1 = true;
            const bool testBool2 = false;

            bytesCommand.writeInt( testInt1 );
            bytesCommand.writeInt( testInt2 );

            bytesCommand.writeString( testStr1 );
            bytesCommand.writeUTF( testStr2 );

            bytesCommand.writeBoolean( testBool1 );
            bytesCommand.writeBoolean( testBool2 );

            writer.writeCommand( &textCommand );
            writer.writeCommand( &bytesCommand );

            // Copy output Command to the Input Stream
            biStream.setByteArray( boStream.toByteArray(),
                                   boStream.size() );

            commands::TextMessageCommand* textMessage =
                dynamic_cast< commands::TextMessageCommand* >(
                    reader.readCommand() );

            CPPUNIT_ASSERT( textMessage != NULL );
            CPPUNIT_ASSERT( textMessage->getText() != "" );
            std::string text = textMessage->getText();
            CPPUNIT_ASSERT( text == "This is a TextMessage" );

            commands::BytesMessageCommand* bytesMessage =
                dynamic_cast< commands::BytesMessageCommand* >(
                    reader.readCommand() );

            CPPUNIT_ASSERT( bytesMessage != NULL );

            CPPUNIT_ASSERT( bytesMessage->readInt() == testInt1 );
            CPPUNIT_ASSERT( bytesMessage->readInt() == testInt2 );
            CPPUNIT_ASSERT( bytesMessage->readString() == testStr1 );
            CPPUNIT_ASSERT( bytesMessage->readUTF() == testStr2 );
            CPPUNIT_ASSERT( bytesMessage->readBoolean() == testBool1 );
            CPPUNIT_ASSERT( bytesMessage->readBoolean() == testBool2 );

            delete bytesMessage;
            delete textMessage;
        }

    };

}}}

#endif /*_ACTIVEMQ_CONNECTOR_STOMP_STOMPCOMMANDWRITERTEST_H_*/
