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

#include "SessionStateTest.h"

#include <activemq/state/SessionState.h>
#include <activemq/commands/SessionInfo.h>
#include <decaf/lang/Pointer.h>

using namespace std;
using namespace activemq;
using namespace activemq::state;
using namespace activemq::commands;
using namespace decaf::lang;

////////////////////////////////////////////////////////////////////////////////
void SessionStateTest::test() {

    // Create a Consumer
    Pointer<ConsumerId> cid( new ConsumerId );
    cid->setConnectionId( "CONNECTION" );
    cid->setSessionId( 4096 );
    cid->setValue( 42 );
    Pointer<ConsumerInfo> cinfo( new ConsumerInfo() );
    cinfo->setConsumerId( cid );

    // Create a Producer
    Pointer<ProducerId> pid( new ProducerId );
    pid->setConnectionId( "CONNECTION" );
    pid->setSessionId( 42 );
    pid->setValue( 4096 );
    Pointer<ProducerInfo> pinfo( new ProducerInfo() );
    pinfo->setProducerId( pid );

    // Create a Session
    Pointer<SessionId> id( new SessionId );
    id->setConnectionId( "CONNECTION" );
    id->setValue( 42 );
    Pointer<SessionInfo> info( new SessionInfo );
    info->setSessionId( id );

    SessionState state( info );
    CPPUNIT_ASSERT( state.getInfo() == info );

    state.addProducer( pinfo );
    state.addConsumer( cinfo );

    CPPUNIT_ASSERT( state.getConsumerStates().size() == 1 );
    CPPUNIT_ASSERT( state.getProducerStates().size() == 1 );

    state.removeProducer( pinfo->getProducerId() );
    state.removeConsumer( cinfo->getConsumerId() );

    CPPUNIT_ASSERT( state.getConsumerStates().size() == 0 );
    CPPUNIT_ASSERT( state.getProducerStates().size() == 0 );

    state.addProducer( pinfo );
    state.addProducer( pinfo );
    CPPUNIT_ASSERT( state.getProducerStates().size() == 1 );

}
