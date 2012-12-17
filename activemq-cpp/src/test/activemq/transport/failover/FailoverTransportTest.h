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

#ifndef _ACTIVEMQ_TRANSPORT_FAILOVER_FAILOVERTRANSPORTTEST_H_
#define _ACTIVEMQ_TRANSPORT_FAILOVER_FAILOVERTRANSPORTTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <activemq/util/Config.h>

#include <activemq/commands/ConnectionInfo.h>
#include <activemq/commands/SessionInfo.h>
#include <activemq/commands/ProducerInfo.h>
#include <activemq/commands/ConsumerInfo.h>

#include <activemq/transport/Transport.h>
#include <decaf/lang/Pointer.h>

namespace activemq {
namespace transport {
namespace failover {

    using decaf::lang::Pointer;
    using namespace activemq::commands;

    class FailoverTransportTest : public CppUnit::TestFixture {

        CPPUNIT_TEST_SUITE( FailoverTransportTest );
        CPPUNIT_TEST( testTransportCreate );
        CPPUNIT_TEST( testTransportCreateWithBackups );
        CPPUNIT_TEST( testTransportCreateFailOnCreate );
        CPPUNIT_TEST( testTransportCreateFailOnCreateSendMessage );
        CPPUNIT_TEST( testFailingBackupCreation );
        CPPUNIT_TEST( testSendOnewayMessage );
        CPPUNIT_TEST( testSendRequestMessage );
        CPPUNIT_TEST( testSendOnewayMessageFail );
        CPPUNIT_TEST( testSendRequestMessageFail );
        CPPUNIT_TEST( testWithOpewireCommands );
        CPPUNIT_TEST( testTransportHandlesConnectionControl );
        CPPUNIT_TEST_SUITE_END();

    public:

        FailoverTransportTest();
        virtual ~FailoverTransportTest();

        // Tests that a Failover Transport can be created, started and stopped.
        void testTransportCreate();

        // Tests that a Failover Transport can be created with backups enabled and
        // be created, started and stopped.
        void testTransportCreateWithBackups();

        // Tests that a Transport will try and reconnect to a Transport that fails
        // at its creation time and will retry until the max reconnect amount has been
        // reached at which point it calls its listeners onException method.
        void testTransportCreateFailOnCreate();

        // Same as above but attempt to send a Message which should result in an Exception
        // being thrown from the oneway method when the max connection attempts have been
        // made.
        void testTransportCreateFailOnCreateSendMessage();

        // Tests that a Transport with a Connected Primary Transport but failing backup
        // transports won't segfault and can be started, and stopped without error.
        void testFailingBackupCreation();

        // Test that messages sent via the Oneway or Request methods are received.
        void testSendOnewayMessage();
        void testSendRequestMessage();

        // Test that messages sent via the Oneway or Request methods are received after
        // the first transport faults on the send and transport 2 is created.
        void testSendOnewayMessageFail();
        void testSendRequestMessageFail();

        // Test the transport using a realistic set of commands being sen through which
        // simulates creation of a Connection, Session, Topics, Producers and Consumers
        // and then removing them all as a normal shutdown would.
        void testWithOpewireCommands();

        // Tests that the Transport correctly handles the ConnectionControl command.
        void testTransportHandlesConnectionControl();

    private:

        Pointer<ConnectionInfo> createConnection();
        Pointer<SessionInfo> createSession( const Pointer<ConnectionInfo>& parent );
        Pointer<ConsumerInfo> createConsumer( const Pointer<SessionInfo>& parent );
        Pointer<ProducerInfo> createProducer( const Pointer<SessionInfo>& parent );

        void disposeOf( const Pointer<SessionInfo>& session,
                        Pointer<Transport>& transport );
        void disposeOf( const Pointer<ConsumerInfo>& consumer,
                        Pointer<Transport>& transport );
        void disposeOf( const Pointer<ProducerInfo>& producer,
                        Pointer<Transport>& transport );

    };

}}}

#endif /*_ACTIVEMQ_TRANSPORT_FAILOVER_FAILOVERTRANSPORTTEST_H_*/
