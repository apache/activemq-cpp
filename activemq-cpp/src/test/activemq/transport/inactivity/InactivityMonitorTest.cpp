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

#include "InactivityMonitorTest.h"

#include <activemq/transport/inactivity/InactivityMonitor.h>
#include <activemq/transport/mock/MockTransport.h>
#include <activemq/transport/mock/MockTransportFactory.h>
#include <activemq/transport/TransportListener.h>
#include <activemq/commands/WireFormatInfo.h>
#include <activemq/commands/ActiveMQMessage.h>

#include <decaf/net/URI.h>
#include <decaf/lang/Thread.h>

#include <typeinfo>

using namespace activemq;
using namespace activemq::commands;
using namespace activemq::transport;
using namespace activemq::transport::mock;
using namespace activemq::transport::inactivity;
using namespace activemq::exceptions;
using namespace decaf;
using namespace decaf::net;
using namespace decaf::io;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

namespace {

    ////////////////////////////////////////////////////////////////////////////////
    class MyTransportListener : public TransportListener {
    public:

        bool exceptionFired;
        int commandsReceived;

    public:

        MyTransportListener() : exceptionFired(false), commandsReceived(0) {}

        virtual ~MyTransportListener() {}

        virtual void onCommand(const Pointer<Command> command) {
            this->commandsReceived++;
        }

        virtual void onException(const decaf::lang::Exception& ex) {
            this->exceptionFired = true;
        }

        virtual void transportInterrupted() {
        }

        virtual void transportResumed() {
        }
    };

}

////////////////////////////////////////////////////////////////////////////////
InactivityMonitorTest::InactivityMonitorTest() : transport(), localWireFormatInfo() {
}

////////////////////////////////////////////////////////////////////////////////
InactivityMonitorTest::~InactivityMonitorTest() {
}

////////////////////////////////////////////////////////////////////////////////
void InactivityMonitorTest::setUp() {

    URI uri( "mock://mock?wireformat=openwire" );
    MockTransportFactory factory;

    this->transport = factory.createComposite( uri ).dynamicCast<MockTransport>();

    this->localWireFormatInfo.reset( new WireFormatInfo() );

    this->localWireFormatInfo->setVersion( 5 );
    this->localWireFormatInfo->setMaxInactivityDuration( 3000 );
    this->localWireFormatInfo->setTightEncodingEnabled( false );
}

////////////////////////////////////////////////////////////////////////////////
void InactivityMonitorTest::tearDown() {
    this->transport.reset( NULL );
}

////////////////////////////////////////////////////////////////////////////////
void InactivityMonitorTest::testCreate() {

    InactivityMonitor monitor( this->transport, this->transport->getWireFormat() );

    CPPUNIT_ASSERT( monitor.getInitialDelayTime() == 0 );
    CPPUNIT_ASSERT( monitor.getReadCheckTime() == 0 );
    CPPUNIT_ASSERT( monitor.getWriteCheckTime() == 0 );
    CPPUNIT_ASSERT( monitor.isKeepAliveResponseRequired() == false );
    CPPUNIT_ASSERT( monitor.isClosed() == false );
}

////////////////////////////////////////////////////////////////////////////////
void InactivityMonitorTest::testReadTimeout() {

    MyTransportListener listener;
    InactivityMonitor monitor( this->transport, this->transport->getWireFormat() );
    monitor.setTransportListener( &listener );
    monitor.start();

    // Send the local one for the monitor to record.
    monitor.oneway( this->localWireFormatInfo );

    Thread::sleep( 2000 );

    // Should not have timed out on Read yet.
    CPPUNIT_ASSERT( listener.exceptionFired == false );

    Thread::sleep( 5000 );

    // Channel should have been inactive for to long.
    CPPUNIT_ASSERT( listener.exceptionFired == true );
}

////////////////////////////////////////////////////////////////////////////////
void InactivityMonitorTest::testWriteMessageFail() {

    this->transport->setFailOnKeepAliveSends( true );
    this->transport->setNumSentKeepAlivesBeforeFail( 4 );

    MyTransportListener listener;
    InactivityMonitor monitor( this->transport, this->transport->getWireFormat() );
    monitor.setTransportListener( &listener );
    monitor.start();

    // Send the local one for the monitor to record.
    monitor.oneway( this->localWireFormatInfo );

    Thread::sleep( 2000 );

    Pointer<ActiveMQMessage> message( new ActiveMQMessage() );
    this->transport->fireCommand( message );

    // Should not have timed out on Read yet.
    CPPUNIT_ASSERT( listener.exceptionFired == false );

    Thread::sleep( 5000 );

    // Channel should have been inactive for to long.
    CPPUNIT_ASSERT( listener.exceptionFired == true );
}

////////////////////////////////////////////////////////////////////////////////
void InactivityMonitorTest::testNonFailureSendCase() {

    MyTransportListener listener;
    InactivityMonitor monitor( this->transport, this->transport->getWireFormat() );
    monitor.setTransportListener( &listener );
    monitor.start();

    // Send the local one for the monitor to record.
    monitor.oneway( this->localWireFormatInfo );

    Pointer<ActiveMQMessage> message( new ActiveMQMessage() );
    for( int ix = 0; ix < 20; ++ix ) {
        monitor.oneway( message );
        Thread::sleep( 500 );
        this->transport->fireCommand( message );
        Thread::sleep( 500 );
    }

    // Channel should have been inactive for to long.
    CPPUNIT_ASSERT( listener.exceptionFired == false );
}
